# SPDX-License-Identifier: MIT

"""
kem_fo_cache_oracle.py
==================
Statistical analysis tool for detecting cache timing side-channels in Key
Encapsulation Mechanism (KEM) decapsulation routines.

Background
----------
Some KEM schemes (e.g. FrodoKEM) store a rejection symbol in the secret key.
A correct constant-time implementation should access the rejection symbol
regardless of whether the input ciphertext is valid or invalid. A faulty
implementation may only access (and thus cache) the rejection symbol when the
ciphertext is *invalid*, leaking information through cache timing:

- **Good ciphertext** (``good=1``): rejection symbol is *not* used → cache cold
  → probe is slow.
- **Bad ciphertext**  (``good=0``): rejection symbol *is* used → cache warm  →
  probe is fast.

If the two probe distributions are statistically distinguishable, a timing
channel is present.

Input CSV format
----------------
``epoch,sample,good,probe,ctrl``

- ``epoch``  – integer epoch index (outer repetition)
- ``sample`` – integer sample index within the epoch
- ``good``   – 1 if the ciphertext passed to decapsulation was valid, else 0
- ``probe``  – cycle count for reading the rejection symbol (target)
- ``ctrl``   – cycle count for reading a control memory location (baseline)

Usage
-----
.. code-block:: bash

    python timing_analysis.py samples.csv
    python timing_analysis.py samples.csv --trim 0.05 --alpha 0.01
    python timing_analysis.py samples.csv --trim 0.10 --no-plot

Dependencies
------------
``pandas``, ``numpy``, ``scipy``, ``matplotlib``
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import NamedTuple

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.stats import mannwhitneyu


# ---------------------------------------------------------------------------
# Data types
# ---------------------------------------------------------------------------


class EpochResult(NamedTuple):
    """Statistical result for a single epoch.

    :param epoch: Epoch index.
    :param n_good: Number of good-ciphertext samples after trimming.
    :param n_bad: Number of bad-ciphertext samples after trimming.
    :param n_discarded: Total samples discarded as outliers.
    :param median_good: Median probe ticks for good ciphertext.
    :param median_bad: Median probe ticks for bad ciphertext.
    :param median_ctrl_good: Median control ticks for good ciphertext.
    :param median_ctrl_bad: Median control ticks for bad ciphertext.
    :param u_statistic: Mann-Whitney U statistic (probe column).
    :param p_value: Two-sided p-value from Mann-Whitney U test (probe column).
    :param ctrl_p_value: Two-sided p-value from Mann-Whitney U test (ctrl
        column).  Should be non-significant if the channel is specific to the
        rejection symbol and not overall noise.
    """

    epoch: int
    n_good: int
    n_bad: int
    n_discarded: int
    median_good: float
    median_bad: float
    median_ctrl_good: float
    median_ctrl_bad: float
    u_statistic: float
    p_value: float
    ctrl_p_value: float


class AnalysisResult(NamedTuple):
    """Aggregate result across all epochs.

    :param epoch_results: Per-epoch results.
    :param combined_p_value: Mann-Whitney U p-value computed on the full
        (trimmed) pooled dataset.
    :param combined_ctrl_p_value: Same test on the control column.
    :param significant_epochs: Number of epochs where ``p_value < alpha``.
    :param total_epochs: Total number of epochs analysed.
    :param alpha: Significance level used.
    """

    epoch_results: list[EpochResult]
    combined_p_value: float
    combined_ctrl_p_value: float
    significant_epochs: int
    total_epochs: int
    alpha: float


# ---------------------------------------------------------------------------
# Core routines  (importable for unit tests)
# ---------------------------------------------------------------------------


def load_csv(path: str | Path) -> pd.DataFrame:
    """Load and validate the input CSV file.

    :param path: Path to the CSV file with columns
        ``epoch,sample,good,probe,ctrl``.
    :returns: Validated :class:`pandas.DataFrame`.
    :raises FileNotFoundError: If *path* does not exist.
    :raises ValueError: If required columns are missing or dtypes are wrong.
    """
    path = Path(path)
    if not path.exists():
        raise FileNotFoundError(f"Input file not found: {path}")

    df = pd.read_csv(path)

    required = {"epoch", "sample", "good", "probe", "ctrl"}
    missing = required - set(df.columns)
    if missing:
        raise ValueError(f"CSV is missing required columns: {missing}")

    for col in ("epoch", "sample", "good", "probe", "ctrl"):
        df[col] = pd.to_numeric(df[col], errors="raise")

    if not set(df["good"].unique()).issubset({0, 1}):
        raise ValueError("Column 'good' must contain only 0 or 1.")

    return df


def trim_outliers(series: pd.Series, trim_frac: float) -> pd.Series:
    """Remove outliers from a series by dropping the top and bottom tail.

    Samples outside the ``[trim_frac, 1 - trim_frac]`` quantile range are
    discarded.  Only the upper tail is trimmed by default when ``trim_frac``
    is small, because cache timing distributions are right-skewed (the long
    tail consists of preemption artefacts, not genuine cache hits).

    :param series: Raw cycle-count samples for one group in one epoch.
    :param trim_frac: Fraction of samples to discard from *each* tail.
        For example, ``0.05`` discards the bottom 5 % and top 5 %.
        Use ``0.0`` to disable trimming.
    :returns: Filtered series with the index reset.
    :raises ValueError: If ``trim_frac`` is not in ``[0, 0.5)``.
    """
    if not (0.0 <= trim_frac < 0.5):
        raise ValueError(f"trim_frac must be in [0, 0.5), got {trim_frac}")
    if trim_frac == 0.0 or len(series) == 0:
        return series.reset_index(drop=True)

    lo = series.quantile(trim_frac)
    hi = series.quantile(1.0 - trim_frac)
    return series[(series >= lo) & (series <= hi)].reset_index(drop=True)


def analyse_epoch(
    epoch_df: pd.DataFrame,
    epoch_id: int,
    trim_frac: float,
) -> EpochResult:
    """Compute statistics for a single epoch.

    Separates the epoch into good-ciphertext (``good=1``) and
    bad-ciphertext (``good=0``) groups, trims outliers from each group
    independently, then runs a two-sided Mann-Whitney U test on both the
    ``probe`` and ``ctrl`` columns.

    Mann-Whitney U is preferred over a t-test here because cache timing
    distributions are typically non-normal and right-skewed.

    :param epoch_df: Rows belonging to this epoch.
    :param epoch_id: Epoch index (used only for the returned result).
    :param trim_frac: Passed to :func:`trim_outliers`.
    :returns: :class:`EpochResult` for this epoch.
    :raises ValueError: If either group is empty after trimming.
    """
    n_before = len(epoch_df)

    good_df = epoch_df[epoch_df["good"] == 1]
    bad_df = epoch_df[epoch_df["good"] == 0]

    probe_good = trim_outliers(good_df["probe"].reset_index(drop=True), trim_frac)
    probe_bad = trim_outliers(bad_df["probe"].reset_index(drop=True), trim_frac)
    ctrl_good = trim_outliers(good_df["ctrl"].reset_index(drop=True), trim_frac)
    ctrl_bad = trim_outliers(bad_df["ctrl"].reset_index(drop=True), trim_frac)

    n_after = len(probe_good) + len(probe_bad)
    n_discarded = n_before - n_after

    if len(probe_good) == 0 or len(probe_bad) == 0:
        raise ValueError(
            f"Epoch {epoch_id}: one group is empty after trimming "
            f"(good={len(probe_good)}, bad={len(probe_bad)}). "
            "Reduce trim_frac or check your data."
        )

    u_stat, p_val = mannwhitneyu(probe_good, probe_bad, alternative="two-sided")
    _, ctrl_p_val = mannwhitneyu(ctrl_good, ctrl_bad, alternative="two-sided")

    return EpochResult(
        epoch=epoch_id,
        n_good=len(probe_good),
        n_bad=len(probe_bad),
        n_discarded=n_discarded,
        median_good=float(np.median(probe_good)),
        median_bad=float(np.median(probe_bad)),
        median_ctrl_good=float(np.median(ctrl_good)),
        median_ctrl_bad=float(np.median(ctrl_bad)),
        u_statistic=float(u_stat),
        p_value=float(p_val),
        ctrl_p_value=float(ctrl_p_val),
    )


def analyse(
    df: pd.DataFrame,
    trim_frac: float = 0.05,
    alpha: float = 0.05,
) -> AnalysisResult:
    """Run the full analysis pipeline across all epochs.

    For each epoch the data is trimmed and a Mann-Whitney U test is
    computed.  A combined test is also run on the full pooled trimmed
    dataset.

    The control column (``ctrl``) is tested in parallel.  A channel is
    considered *specific* to the rejection symbol if:

    - ``probe`` p-value < ``alpha``  (significant difference in probe)
    - ``ctrl``  p-value ≥ ``alpha``  (no significant difference in control)

    :param df: DataFrame as returned by :func:`load_csv`.
    :param trim_frac: Fraction of samples to trim from each tail per epoch
        per group.  Defaults to ``0.05`` (5 % each tail).
    :param alpha: Significance level for per-epoch verdict.  Defaults to
        ``0.05``.
    :returns: :class:`AnalysisResult` aggregating all epochs.
    """
    epoch_results: list[EpochResult] = []

    pooled_probe_good: list[pd.Series] = []
    pooled_probe_bad: list[pd.Series] = []
    pooled_ctrl_good: list[pd.Series] = []
    pooled_ctrl_bad: list[pd.Series] = []

    for epoch_id, epoch_df in df.groupby("epoch"):
        result = analyse_epoch(epoch_df, int(epoch_id), trim_frac)
        epoch_results.append(result)

        # accumulate trimmed data for combined test
        good_df = epoch_df[epoch_df["good"] == 1]
        bad_df = epoch_df[epoch_df["good"] == 0]
        pooled_probe_good.append(
            trim_outliers(good_df["probe"].reset_index(drop=True), trim_frac)
        )
        pooled_probe_bad.append(
            trim_outliers(bad_df["probe"].reset_index(drop=True), trim_frac)
        )
        pooled_ctrl_good.append(
            trim_outliers(good_df["ctrl"].reset_index(drop=True), trim_frac)
        )
        pooled_ctrl_bad.append(
            trim_outliers(bad_df["ctrl"].reset_index(drop=True), trim_frac)
        )

    all_probe_good = pd.concat(pooled_probe_good, ignore_index=True)
    all_probe_bad = pd.concat(pooled_probe_bad, ignore_index=True)
    all_ctrl_good = pd.concat(pooled_ctrl_good, ignore_index=True)
    all_ctrl_bad = pd.concat(pooled_ctrl_bad, ignore_index=True)

    _, combined_p = mannwhitneyu(all_probe_good, all_probe_bad, alternative="two-sided")
    _, combined_ctrl_p = mannwhitneyu(
        all_ctrl_good, all_ctrl_bad, alternative="two-sided"
    )

    significant = sum(1 for r in epoch_results if r.p_value < alpha)

    return AnalysisResult(
        epoch_results=epoch_results,
        combined_p_value=float(combined_p),
        combined_ctrl_p_value=float(combined_ctrl_p),
        significant_epochs=significant,
        total_epochs=len(epoch_results),
        alpha=alpha,
    )


# ---------------------------------------------------------------------------
# Reporting
# ---------------------------------------------------------------------------


def print_report(result: AnalysisResult) -> None:
    """Print a human-readable summary of the analysis to stdout.

    :param result: Output of :func:`analyse`.
    """
    alpha = result.alpha
    header = (
        f"{'Epoch':>6}  {'N_good':>7}  {'N_bad':>7}  {'Disc':>5}  "
        f"{'Med(good)':>10}  {'Med(bad)':>9}  "
        f"{'p_probe':>10}  {'p_ctrl':>10}  {'Verdict'}"
    )
    print("\n" + "=" * len(header))
    print("Per-epoch results")
    print("=" * len(header))
    print(header)
    print("-" * len(header))

    for r in result.epoch_results:
        probe_sig = r.p_value < alpha
        ctrl_sig = r.ctrl_p_value < alpha
        if probe_sig and not ctrl_sig:
            verdict = "CHANNEL (specific)"
        elif probe_sig and ctrl_sig:
            verdict = "CHANNEL (+ noise?)"
        else:
            verdict = "no signal"

        print(
            f"{r.epoch:>6}  {r.n_good:>7}  {r.n_bad:>7}  {r.n_discarded:>5}  "
            f"{r.median_good:>10.1f}  {r.median_bad:>9.1f}  "
            f"{r.p_value:>10.4e}  {r.ctrl_p_value:>10.4e}  {verdict}"
        )

    print("=" * len(header))
    print(f"\nCombined test (all epochs pooled, trimmed):")
    print(f"  probe  p-value : {result.combined_p_value:.4e}")
    print(f"  ctrl   p-value : {result.combined_ctrl_p_value:.4e}")
    print(
        f"\nSignificant epochs (p < {alpha}): "
        f"{result.significant_epochs} / {result.total_epochs}"
    )

    probe_detected = result.combined_p_value < alpha
    ctrl_detected = result.combined_ctrl_p_value < alpha

    print("\n=== VERDICT ===")
    if probe_detected and not ctrl_detected:
        print("SECRET-DEPENDENT cache timing channel DETECTED")
        print("  probe is significant; ctrl is not → signal is specific to")
        print("  the rejection symbol, not overall system noise.")
    elif probe_detected and ctrl_detected:
        print("Timing difference detected in BOTH probe and ctrl.")
        print("  Cannot rule out that the signal is dominated by system noise.")
        print("  Consider increasing sample count or reducing system load.")
    else:
        print("No significant timing channel detected at alpha =", alpha)


# ---------------------------------------------------------------------------
# Plotting
# ---------------------------------------------------------------------------


def plot_violins(
    df: pd.DataFrame,
    result: AnalysisResult,
    trim_frac: float,
    output_path: str | Path | None = None,
) -> None:
    """Render a violin plot grid showing probe and ctrl distributions.

    One column per epoch, two rows (probe / ctrl), two violins per cell
    (good ciphertext in blue, bad ciphertext in orange).  A horizontal
    dashed line marks the per-epoch median for each group.

    :param df: Original (untrimmed) DataFrame from :func:`load_csv`.
    :param result: Output of :func:`analyse`.
    :param trim_frac: Used to re-trim data for plotting (should match the
        value passed to :func:`analyse`).
    :param output_path: If given, save the figure to this path instead of
        displaying it interactively.
    """
    epochs = sorted(df["epoch"].unique())
    n_epochs = len(epochs)
    fig, axes = plt.subplots(
        2,
        n_epochs,
        figsize=(max(6, 3 * n_epochs), 8),
        sharey="row",
        squeeze=False,
    )

    col_good = "#4C8BCC"  # blue
    col_bad = "#E07B39"  # orange

    row_labels = ["probe (rejection symbol)", "ctrl (control location)"]

    for col_idx, epoch_id in enumerate(epochs):
        epoch_df = df[df["epoch"] == epoch_id]
        good_df = epoch_df[epoch_df["good"] == 1]
        bad_df = epoch_df[epoch_df["good"] == 0]

        groups = {
            "probe": (
                trim_outliers(good_df["probe"].reset_index(drop=True), trim_frac),
                trim_outliers(bad_df["probe"].reset_index(drop=True), trim_frac),
            ),
            "ctrl": (
                trim_outliers(good_df["ctrl"].reset_index(drop=True), trim_frac),
                trim_outliers(bad_df["ctrl"].reset_index(drop=True), trim_frac),
            ),
        }

        # retrieve matching EpochResult for annotation
        er = next(r for r in result.epoch_results if r.epoch == epoch_id)

        for row_idx, (col_name, (good_vals, bad_vals)) in enumerate(groups.items()):
            ax = axes[row_idx][col_idx]

            parts_good = ax.violinplot(
                good_vals, positions=[1], showmedians=True, widths=0.7
            )
            parts_bad = ax.violinplot(
                bad_vals, positions=[2], showmedians=True, widths=0.7
            )

            for pc in parts_good["bodies"]:
                pc.set_facecolor(col_good)
                pc.set_alpha(0.7)
            for pc in parts_bad["bodies"]:
                pc.set_facecolor(col_bad)
                pc.set_alpha(0.7)
            for key in ("cmins", "cmaxes", "cbars", "cmedians"):
                parts_good[key].set_color(col_good)
                parts_bad[key].set_color(col_bad)

            ax.set_xticks([1, 2])
            ax.set_xticklabels(["good ct", "bad ct"], fontsize=8)

            # p-value annotation
            p_val = er.p_value if col_name == "probe" else er.ctrl_p_value
            sig = "✓ sig" if p_val < result.alpha else "✗ n.s."
            ax.set_title(
                f"epoch {epoch_id}\np={p_val:.2e}  {sig}",
                fontsize=8,
            )

            if col_idx == 0:
                ax.set_ylabel(f"{row_labels[row_idx]}\n(ticks)", fontsize=8)

    # shared legend
    from matplotlib.patches import Patch

    legend_elements = [
        Patch(facecolor=col_good, alpha=0.7, label="good ciphertext (ct valid)"),
        Patch(facecolor=col_bad, alpha=0.7, label="bad ciphertext  (ct invalid)"),
    ]
    fig.legend(
        handles=legend_elements,
        loc="lower center",
        ncol=2,
        fontsize=9,
        bbox_to_anchor=(0.5, 0.0),
    )

    fig.suptitle(
        f"Cache timing side-channel analysis\n"
        f"combined probe p={result.combined_p_value:.2e}  |  "
        f"combined ctrl p={result.combined_ctrl_p_value:.2e}  |  "
        f"trim={trim_frac:.0%}  α={result.alpha}",
        fontsize=11,
    )
    fig.tight_layout(rect=[0, 0.06, 1, 1])

    if output_path:
        fig.savefig(output_path, dpi=150, bbox_inches="tight")
        print(f"Plot saved to {output_path}")
    else:
        plt.show()


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------


def build_arg_parser() -> argparse.ArgumentParser:
    """Build the command-line argument parser.

    :returns: Configured :class:`argparse.ArgumentParser`.
    """
    parser = argparse.ArgumentParser(
        prog="timing_analysis",
        description=(
            "Detect cache timing side-channels in KEM decapsulation traces. "
            "Reads a CSV with columns epoch,sample,good,probe,ctrl and runs "
            "Mann-Whitney U tests to determine whether good and bad ciphertext "
            "probe times come from the same distribution."
        ),
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument(
        "csv",
        metavar="FILE",
        help="Path to input CSV file.",
    )
    parser.add_argument(
        "--trim",
        type=float,
        default=0.05,
        metavar="FRAC",
        help=(
            "Fraction of samples to discard from each tail per epoch per group "
            "(e.g. 0.05 removes the bottom 5%% and top 5%%)."
        ),
    )
    parser.add_argument(
        "--alpha",
        type=float,
        default=0.05,
        metavar="ALPHA",
        help="Significance level for per-epoch and combined verdict.",
    )
    parser.add_argument(
        "--no-plot",
        action="store_true",
        help="Skip violin plot rendering.",
    )
    parser.add_argument(
        "--save-plot",
        metavar="PATH",
        default=None,
        help="Save plot to PATH instead of displaying interactively.",
    )
    return parser


def main(argv: list[str] | None = None) -> int:
    """Entry point for the command-line interface.

    :param argv: Argument list (defaults to ``sys.argv[1:]``).
    :returns: Exit code (0 on success, non-zero on error).
    """
    parser = build_arg_parser()
    args = parser.parse_args(argv)

    try:
        df = load_csv(args.csv)
    except (FileNotFoundError, ValueError) as exc:
        print(f"Error loading CSV: {exc}", file=sys.stderr)
        return 1

    result = analyse(df, trim_frac=args.trim, alpha=args.alpha)
    print_report(result)

    if not args.no_plot:
        plot_violins(
            df,
            result,
            trim_frac=args.trim,
            output_path=args.save_plot,
        )

    return 0


if __name__ == "__main__":
    sys.exit(main())

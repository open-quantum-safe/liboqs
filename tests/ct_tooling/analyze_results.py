# SPDX-License-Identifier: MIT

#!/usr/bin/env python3

"""
Analyze constant-time results across different optimization levels.
Parses summary files to compare warning counts for KEMs and SIGs.
Generates CSV reports and visualization graphs.
"""

import os
import re
import glob
from collections import defaultdict
import csv
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('Agg')
import numpy as np
import argparse
import sys

parser = argparse.ArgumentParser(description='Analyze constant-time test results')
parser.add_argument('--tool', '-t', type=str, help='Constant-time tool used for analysis')
parser.add_argument('--input', '-i', type=str, help='General input directory containing build information')
parser.add_argument('--output', '-o', type=str, default='.', help='Output directory for analysis results')
args = parser.parse_args()

cwd = os.getcwd()
LOG_BASE_DIR = os.path.abspath(os.path.join(cwd, args.input)) if args.input else cwd
OUTPUT_DIR = os.path.abspath(os.path.join(cwd, args.output)) if args.output else cwd

def parse_summary_file(filepath):
    """
    Pattern to match Valgrind test results:
        Example:  "Testing KEM: Kyber512 ... FAIL (Valgrind/MemSan warnings)"
                  "  → Found 150 Valgrind warnings"
    """
    results = {}
    
    with open(filepath, 'r') as f:
        content = f.read()
    
    header_re = re.compile(r'^Testing (?:KEM|SIG): (\S+) \.\.\. (?:PASS|FAIL)', re.MULTILINE)
    count_re = re.compile(r'(?:→|->)?\s*(?:Found|Reached cap).*?(\d+).*?warnings', re.IGNORECASE)

    matches = list(header_re.finditer(content))
    for idx, m in enumerate(matches):
        alg_name = m.group(1)

        # Skip SPHINCS and SLH_DSA algorithms
        if 'SPHINCS' in alg_name or 'SLH_DSA' in alg_name:
            continue

        start = m.end()
        end = matches[idx+1].start() if idx+1 < len(matches) else len(content)
        block = content[start:end]

        c = count_re.search(block)
        if c:
            error_count = int(c.group(1))
        else:
            error_count = 0

        results[alg_name] = error_count
    
    return results

def analyze_logs():
    """
    Expected layout:
        <LOG_BASE_DIR>/<BUILD_NAME>/<OPT_LEVEL>/kem/kem_summary_*.txt
        <LOG_BASE_DIR>/<BUILD_NAME>/<OPT_LEVEL>/sig/sig_summary_*.txt
    """

    kem_data = defaultdict(lambda: defaultdict(dict))  # {build: {opt: {alg: warnings}}}
    sig_data = defaultdict(lambda: defaultdict(dict))

    if not os.path.isdir(LOG_BASE_DIR):
        print(f"Error: Input directory '{LOG_BASE_DIR}' does not exist.", file=sys.stderr)
        sys.exit(1)

    for build_dir in sorted(glob.glob(os.path.join(LOG_BASE_DIR, '*'))):
        build_name = os.path.basename(build_dir)
        for opt_dir in sorted(glob.glob(os.path.join(build_dir, '*'))):
            opt_level = os.path.basename(opt_dir)
            for test_type in ['kem', 'sig']:
                test_dir = os.path.join(opt_dir, test_type)
                if not os.path.isdir(test_dir):
                    continue

                for summary_file in glob.glob(os.path.join(test_dir, f'{test_type}_summary_*.txt')):
                    results = parse_summary_file(summary_file)
                    if test_type == 'kem':
                        kem_data[build_name][opt_level].update(results)
                    else:
                        sig_data[build_name][opt_level].update(results)

    return kem_data, sig_data

def sort_optimization_levels(opt_levels):
    priority = {
        'O0': 0, 
        'O1': 1, 
        'O2-NOVEC': 2, 
        'O2': 3, 
        'OS': 4, 
        'O3-NOVEC': 5, 
        'O3': 6, 
        'OFAST': 7
    }
    return sorted(opt_levels, key=lambda x: priority.get(x.upper(), 99))

def generate_csv_file(data, alg_type, opt_levels, output_dir):

    csv_path = os.path.join(output_dir, f'{alg_type}_warnings_per_opt_level.csv')
    with open(csv_path, 'w', newline='') as csvfile:
        all_algs = set()
        for opt_data in data.values():
            all_algs.update(opt_data.keys())
        all_algs = sorted(all_algs)

        writer = csv.writer(csvfile)
        writer.writerow(['Algorithm'] + opt_levels)

        for alg in all_algs:
            row = [alg]
            for opt in opt_levels:
                row.append(data[opt].get(alg, 'N/A'))
            writer.writerow(row)

def heatmap_warnings_per_alg_and_opt_level(data, opt_levels, alg_type, tool, output_dir):
    
    # Sort algorithms by total warnings (descending). For ties, sort alphabetically ascending
    def _norm_name(n):
        return re.sub(r'[^0-9a-z]', '', n.lower())
    
    all_algs = set()
    for opt in opt_levels:
        all_algs.update(data.get(opt, {}).keys())
        alg_total = {alg: sum([data.get(opt, {}).get(alg, 0) for opt in opt_levels]) for alg in all_algs}
    alg_names = [a for a, _ in sorted(alg_total.items(), key=lambda x: (-x[1], _norm_name(x[0])))]
    
    matrix = np.zeros((len(alg_names), len(opt_levels)), dtype=int)
    for i, alg in enumerate(alg_names):
        for j, opt in enumerate(opt_levels):
            matrix[i, j] = data.get(opt, {}).get(alg, 0)

    CMAP_BY_TYPE = {'KEM': 'Blues', 'SIG': 'Oranges'}
    cmap = CMAP_BY_TYPE.get(alg_type.upper(), 'YlOrRd')

    fig_height = max(8, len(alg_names) * 0.25)
    fig, ax = plt.subplots(figsize=(12, fig_height))

    max_val = int(matrix.max()) if matrix.size else 0
    vmax = max(1, max_val)

    raster = True if len(alg_names) > 200 else False
    im = ax.imshow(matrix, aspect='auto', cmap=cmap, vmin=0, vmax=vmax, rasterized=raster)

    ax.set_xticks(np.arange(len(opt_levels)))
    ax.set_xticklabels(opt_levels)
    plt.setp(ax.get_xticklabels(), rotation=45, ha='right', rotation_mode='anchor')

    ax.set_yticks(np.arange(len(alg_names)))
    ax.set_yticklabels(alg_names)

    cbar = plt.colorbar(im, ax=ax)
    cbar.set_label('Number of Warnings', rotation=270, labelpad=20, fontsize=12)

    for i in range(len(alg_names)):
        for j in range(len(opt_levels)):
            ax.text(j, i, int(matrix[i, j]), ha='center', va='center', color='black', fontsize=8)

    ax.set_title(f'{tool.upper()} {alg_type.upper()} Warnings per Algorithm by Optimization Level',
                 fontsize=14, fontweight='bold', pad=20)
    ax.set_xlabel('Optimization Level', fontsize=12)
    ax.set_ylabel(f'{alg_type.upper()} Algorithms', fontsize=12)
    plt.tight_layout()

    graph_path = os.path.join(output_dir, f"{alg_type.upper()}_total_warnings_per_alg_by_opt_level.png")
    plt.savefig(graph_path, dpi=300, bbox_inches='tight')
    plt.close()

def bar_chart_total_warnings_per_opt_level(kem_data, sig_data, opt_levels, tool, output_dir):

    fig, ax = plt.subplots(figsize=(14, 6))
    
    kem_totals = [sum(kem_data[opt].values()) for opt in opt_levels]
    sig_totals = [sum(sig_data[opt].values()) for opt in opt_levels]

    x = np.arange(len(opt_levels))
    width = 0.35
    ax.bar(x - width/2, kem_totals, width, label='KEM', color='#2b8cbe')
    ax.bar(x + width/2, sig_totals, width, label='SIG', color='#f16913')
    ax.set_xlabel('Optimization Level', fontsize=12)
    ax.set_ylabel('Total Warnings', fontsize=12)
    ax.set_title(f'{tool.upper()} Total Warnings by Optimization Level', fontsize=14, fontweight='bold', pad=20)
    ax.set_xticks(x)
    ax.set_xticklabels(opt_levels, rotation=45, ha='right')
    ax.legend()
    ax.grid(axis='y', alpha=0.3)
    plt.tight_layout()
    outpath = os.path.join(output_dir, f'{tool}_total_warnings_kem_vs_sig.png')
    plt.savefig(outpath, dpi=300, bbox_inches='tight')
    plt.close()

def line_chart_avg_warnings_per_opt_level(kem_data, sig_data, opt_levels, tool, output_dir):

    kem_avgs = [np.mean(list(kem_data.get(opt, {}).values())) if kem_data.get(opt) else 0 for opt in opt_levels]
    sig_avgs = [np.mean(list(sig_data.get(opt, {}).values())) if sig_data.get(opt) else 0 for opt in opt_levels]

    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(opt_levels, kem_avgs, marker='o', label='KEM', color='#2b8cbe')
    ax.plot(opt_levels, sig_avgs, marker='o', label='SIG', color='#f16913')
    ax.set_xlabel('Optimization Level',fontsize=12)
    ax.set_ylabel('Average Warnings', fontsize=12)
    ax.set_title(f'{tool.upper()} Average Warnings per Optimization Level', fontsize=14, fontweight='bold', pad=20)
    ax.legend()
    ax.grid(True, alpha=0.3)
    plt.tight_layout()
    outpath = os.path.join(output_dir, f'{tool}_avg_warnings_kem_vs_sig.png')
    plt.savefig(outpath, dpi=300, bbox_inches='tight')
    plt.close()

def generate_reports(kem_data, sig_data):
    for build_name in kem_data.keys():
        build_output_dir = os.path.join(OUTPUT_DIR, build_name)
        os.makedirs(build_output_dir, exist_ok=True)

        opt_levels = sort_optimization_levels(kem_data[build_name].keys())

        generate_csv_file(kem_data[build_name], 'KEM', opt_levels, build_output_dir)
        generate_csv_file(sig_data[build_name], 'SIG', opt_levels, build_output_dir)

        heatmap_warnings_per_alg_and_opt_level(kem_data[build_name], opt_levels, 'KEM', args.tool, build_output_dir)
        heatmap_warnings_per_alg_and_opt_level(sig_data[build_name], opt_levels, 'SIG', args.tool, build_output_dir)

        bar_chart_total_warnings_per_opt_level(kem_data[build_name], sig_data[build_name], opt_levels, args.tool, build_output_dir)
        line_chart_avg_warnings_per_opt_level(kem_data[build_name], sig_data[build_name], opt_levels, args.tool, build_output_dir)

def main():
    kem_data, sig_data = analyze_logs()
    generate_reports(kem_data, sig_data)

if __name__ == "__main__":
    main()

# SPDX-License-Identifier: MIT

"""
Tests for scripts in the scripts/ directory.

Addresses https://github.com/open-quantum-safe/liboqs/issues/1408
by exercising scripts that were previously untested.
"""

import glob
import json
import os
import shutil
import subprocess
import sys
import tempfile

import pytest
import yaml


LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SCRIPTS_DIR = os.path.join(LIBOQS_ROOT, 'scripts')


def run_script(command, working_dir=LIBOQS_ROOT, env=None, expected_returncode=0):
    env_ = os.environ.copy()
    if env is not None:
        env_.update(env)
    result = subprocess.run(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd=working_dir,
        env=env_,
    )
    output = result.stdout.decode('utf-8')
    if result.returncode != expected_returncode:
        print(output)
        assert False, "Command {} returned {} (expected {})".format(
            ' '.join(command), result.returncode, expected_returncode)
    return output


# --------------------------------------------------------------------------
# scripts/doxyfy.py
# --------------------------------------------------------------------------
class TestDoxyfy:
    """Test the doxyfy.py script that reformats markdown anchors for doxygen."""

    def test_heading_anchor_insertion(self, tmp_path):
        """Headings (lines starting with #) should get doxygen anchor tags."""
        input_md = "# My Heading\n\nSome text.\n"
        infile = tmp_path / "input.md"
        outfile = tmp_path / "output.md"
        infile.write_text(input_md)
        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'doxyfy.py'),
                     str(infile), str(outfile)])
        result = outfile.read_text()
        assert '{#my-heading}' in result.lower() or '{#my-Heading}' in result

    def test_subheading_anchor(self, tmp_path):
        """Subheadings should also receive anchors."""
        input_md = "## Sub Heading\n\nMore text.\n"
        infile = tmp_path / "input.md"
        outfile = tmp_path / "output.md"
        infile.write_text(input_md)
        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'doxyfy.py'),
                     str(infile), str(outfile)])
        result = outfile.read_text()
        assert '{#' in result

    def test_no_data_loss(self, tmp_path):
        """Output should preserve all original content lines."""
        input_md = "# Title\n\nParagraph one.\n\nParagraph two.\n"
        infile = tmp_path / "input.md"
        outfile = tmp_path / "output.md"
        infile.write_text(input_md)
        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'doxyfy.py'),
                     str(infile), str(outfile)])
        result = outfile.read_text()
        assert "Paragraph one." in result
        assert "Paragraph two." in result

    def test_wrong_args_exits_nonzero(self, tmp_path):
        """Calling with wrong number of args should exit with code 1."""
        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'doxyfy.py')],
                   expected_returncode=1)

    def test_nonexistent_input_exits_nonzero(self, tmp_path):
        """Calling with a non-existent input file should exit with code 1."""
        outfile = tmp_path / "output.md"
        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'doxyfy.py'),
                     '/nonexistent/path.md', str(outfile)],
                   expected_returncode=1)

    def test_internal_anchor_links(self, tmp_path):
        """Lines with internal anchor links like [text](#anchor) should be reformatted."""
        input_md = "# Main\n\nSee [section](#Some Section).\n"
        infile = tmp_path / "input.md"
        outfile = tmp_path / "output.md"
        infile.write_text(input_md)
        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'doxyfy.py'),
                     str(infile), str(outfile)])
        result = outfile.read_text()
        assert "](#" in result

    def test_underline_heading(self, tmp_path):
        """=== style headings should be handled."""
        input_md = "Title\n===\nBody text.\n"
        infile = tmp_path / "input.md"
        outfile = tmp_path / "output.md"
        infile.write_text(input_md)
        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'doxyfy.py'),
                     str(infile), str(outfile)])
        result = outfile.read_text()
        assert "Title" in result
        assert "Body text." in result


# --------------------------------------------------------------------------
# scripts/format_docs_yaml.py
# --------------------------------------------------------------------------
class TestFormatDocsYaml:
    """Test the format_docs_yaml.py script that normalizes YAML formatting."""

    def test_idempotent_on_kem_yamls(self, tmp_path):
        """Running format_docs_yaml on existing YAMLs should be idempotent
        (no content changes on already-formatted files)."""
        kem_yamls = glob.glob(os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', 'kem', '*.yml'))
        if not kem_yamls:
            pytest.skip("No KEM YAML files found")

        fake_root = tmp_path / "liboqs"
        kem_dir = fake_root / "docs" / "algorithms" / "kem"
        sig_dir = fake_root / "docs" / "algorithms" / "sig"
        kem_dir.mkdir(parents=True)
        sig_dir.mkdir(parents=True)

        test_yaml = kem_yamls[0]
        dest = kem_dir / os.path.basename(test_yaml)
        shutil.copy2(test_yaml, dest)
        original_content = dest.read_text()

        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'format_docs_yaml.py'),
                     '--liboqs-root', str(fake_root)])

        with open(dest, 'r') as fh:
            formatted = yaml.safe_load(fh.read())
        with open(test_yaml, 'r') as fh:
            original = yaml.safe_load(fh.read())

        assert formatted == original, "YAML content changed after formatting"

    def test_formats_sig_yamls(self, tmp_path):
        """Script should also process sig YAML files without errors."""
        sig_yamls = glob.glob(os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', 'sig', '*.yml'))
        if not sig_yamls:
            pytest.skip("No SIG YAML files found")

        fake_root = tmp_path / "liboqs"
        kem_dir = fake_root / "docs" / "algorithms" / "kem"
        sig_dir = fake_root / "docs" / "algorithms" / "sig"
        kem_dir.mkdir(parents=True)
        sig_dir.mkdir(parents=True)

        test_yaml = sig_yamls[0]
        dest = sig_dir / os.path.basename(test_yaml)
        shutil.copy2(test_yaml, dest)

        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'format_docs_yaml.py'),
                     '--liboqs-root', str(fake_root)])

        with open(dest, 'r') as fh:
            result = yaml.safe_load(fh.read())
        assert result is not None
        assert 'name' in result


# --------------------------------------------------------------------------
# scripts/git_commit.sh
# --------------------------------------------------------------------------
class TestGitCommit:
    """Test the git_commit.sh script that retrieves the current git hash."""

    @pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script, not for Windows")
    def test_returns_commit_hash(self):
        """In a git repo, the script should return a valid hex commit hash."""
        output = run_script(['bash', os.path.join(SCRIPTS_DIR, 'git_commit.sh')])
        hex_part = output.split()[0] if output else ""
        assert len(hex_part) >= 7, "Expected a git hash, got: {}".format(output)
        assert all(c in '0123456789abcdef' for c in hex_part), \
            "Expected hex hash, got: {}".format(hex_part)

    @pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script, not for Windows")
    def test_output_in_non_git_dir(self, tmp_path):
        """In a non-git directory, should output 'unknown'."""
        output = run_script(
            ['bash', os.path.join(SCRIPTS_DIR, 'git_commit.sh')],
            working_dir=str(tmp_path))
        assert 'unknown' in output


# --------------------------------------------------------------------------
# scripts/genkatsha256.sh
# --------------------------------------------------------------------------
class TestGenkatSha256:
    """Test the genkatsha256.sh script that creates .sha256 from .kat files."""

    @pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script, not for Windows")
    def test_creates_sha256_files(self, tmp_path):
        """Should create .kat.sha256 files for each .kat file."""
        kat1 = tmp_path / "algo1.kat"
        kat2 = tmp_path / "algo2.kat"
        kat1.write_text("test kat data 1\n")
        kat2.write_text("test kat data 2\n")

        run_script(
            ['bash', os.path.join(SCRIPTS_DIR, 'genkatsha256.sh')],
            working_dir=str(tmp_path))

        sha1 = tmp_path / "algo1.kat.sha256"
        sha2 = tmp_path / "algo2.kat.sha256"
        assert sha1.exists(), "algo1.kat.sha256 not created"
        assert sha2.exists(), "algo2.kat.sha256 not created"

        hash1 = sha1.read_text().strip()
        assert len(hash1) == 64, "SHA256 hash should be 64 hex chars, got: {}".format(hash1)
        assert all(c in '0123456789abcdef' for c in hash1)

    @pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script, not for Windows")
    def test_deterministic_output(self, tmp_path):
        """Same input .kat file should always produce the same hash."""
        kat = tmp_path / "test.kat"
        kat.write_text("deterministic content\n")

        run_script(
            ['bash', os.path.join(SCRIPTS_DIR, 'genkatsha256.sh')],
            working_dir=str(tmp_path))
        hash1 = (tmp_path / "test.kat.sha256").read_text().strip()

        run_script(
            ['bash', os.path.join(SCRIPTS_DIR, 'genkatsha256.sh')],
            working_dir=str(tmp_path))
        hash2 = (tmp_path / "test.kat.sha256").read_text().strip()

        assert hash1 == hash2, "Hashes should be deterministic"

    @pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script, not for Windows")
    def test_no_kat_files_is_harmless(self, tmp_path):
        """Running in a directory with no .kat files should not fail."""
        run_script(
            ['bash', os.path.join(SCRIPTS_DIR, 'genkatsha256.sh')],
            working_dir=str(tmp_path))


# --------------------------------------------------------------------------
# scripts/update_docs_from_yaml.py
# --------------------------------------------------------------------------
class TestUpdateDocsFromYaml:
    """Test the update_docs_from_yaml.py script that generates .md from .yml."""

    def test_generates_kem_markdown(self, tmp_path):
        """Should generate .md files for each KEM .yml file."""
        kem_yamls = glob.glob(os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', 'kem', '*.yml'))
        if not kem_yamls:
            pytest.skip("No KEM YAML files found")

        fake_root = tmp_path / "liboqs"
        for subdir in ['kem', 'sig', 'sig_stfl']:
            src = os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', subdir)
            dst = fake_root / 'docs' / 'algorithms' / subdir
            if os.path.isdir(src):
                shutil.copytree(src, dst)

        readme_src = os.path.join(LIBOQS_ROOT, 'README.md')
        if os.path.isfile(readme_src):
            shutil.copy2(readme_src, fake_root / 'README.md')

        algorithms_src = os.path.join(LIBOQS_ROOT, 'ALGORITHMS.md')
        if os.path.isfile(algorithms_src):
            shutil.copy2(algorithms_src, fake_root / 'ALGORITHMS.md')

        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'update_docs_from_yaml.py'),
                     '--liboqs-root', str(fake_root)])

        generated_mds = glob.glob(str(fake_root / 'docs' / 'algorithms' / 'kem' / '*.md'))
        assert len(generated_mds) > 0, "No KEM .md files were generated"

        with open(generated_mds[0], 'r') as fh:
            content = fh.read()
        assert '# ' in content, "Generated markdown should have a heading"
        assert 'Algorithm type' in content, "Generated markdown should describe algorithm type"

    def test_generates_sig_markdown(self, tmp_path):
        """Should generate .md files for each SIG .yml file."""
        sig_yamls = glob.glob(os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', 'sig', '*.yml'))
        if not sig_yamls:
            pytest.skip("No SIG YAML files found")

        fake_root = tmp_path / "liboqs"
        for subdir in ['kem', 'sig', 'sig_stfl']:
            src = os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', subdir)
            dst = fake_root / 'docs' / 'algorithms' / subdir
            if os.path.isdir(src):
                shutil.copytree(src, dst)

        readme_src = os.path.join(LIBOQS_ROOT, 'README.md')
        if os.path.isfile(readme_src):
            shutil.copy2(readme_src, fake_root / 'README.md')

        algorithms_src = os.path.join(LIBOQS_ROOT, 'ALGORITHMS.md')
        if os.path.isfile(algorithms_src):
            shutil.copy2(algorithms_src, fake_root / 'ALGORITHMS.md')

        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'update_docs_from_yaml.py'),
                     '--liboqs-root', str(fake_root)])

        generated_mds = glob.glob(str(fake_root / 'docs' / 'algorithms' / 'sig' / '*.md'))
        assert len(generated_mds) > 0, "No SIG .md files were generated"

    def test_idempotent(self, tmp_path):
        """Running twice should produce identical output."""
        fake_root = tmp_path / "liboqs"
        for subdir in ['kem', 'sig', 'sig_stfl']:
            src = os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', subdir)
            dst = fake_root / 'docs' / 'algorithms' / subdir
            if os.path.isdir(src):
                shutil.copytree(src, dst)

        readme_src = os.path.join(LIBOQS_ROOT, 'README.md')
        if os.path.isfile(readme_src):
            shutil.copy2(readme_src, fake_root / 'README.md')

        algorithms_src = os.path.join(LIBOQS_ROOT, 'ALGORITHMS.md')
        if os.path.isfile(algorithms_src):
            shutil.copy2(algorithms_src, fake_root / 'ALGORITHMS.md')

        cmd = [sys.executable, os.path.join(SCRIPTS_DIR, 'update_docs_from_yaml.py'),
               '--liboqs-root', str(fake_root)]

        run_script(cmd)
        first_run_files = {}
        for md in glob.glob(str(fake_root / 'docs' / 'algorithms' / 'kem' / '*.md')):
            with open(md, 'r') as fh:
                first_run_files[os.path.basename(md)] = fh.read()

        run_script(cmd)
        for md in glob.glob(str(fake_root / 'docs' / 'algorithms' / 'kem' / '*.md')):
            with open(md, 'r') as fh:
                second = fh.read()
            basename = os.path.basename(md)
            assert first_run_files[basename] == second, \
                "Output changed on second run for {}".format(basename)


# --------------------------------------------------------------------------
# scripts/update_cbom.py
# --------------------------------------------------------------------------
class TestUpdateCbom:
    """Test the update_cbom.py script that generates a Cryptography BOM."""

    def test_generates_valid_json(self, tmp_path):
        """CBOM output should be valid JSON with expected top-level keys."""
        fake_root = tmp_path / "liboqs"
        for subdir in ['kem', 'sig', 'sig_stfl']:
            src = os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', subdir)
            dst = fake_root / 'docs' / 'algorithms' / subdir
            if os.path.isdir(src):
                shutil.copytree(src, dst)
        (fake_root / 'docs').mkdir(parents=True, exist_ok=True)

        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'update_cbom.py'),
                     '--liboqs-root', str(fake_root),
                     '--liboqs-version', '0.0.0-test'])

        cbom_path = fake_root / 'docs' / 'cbom.json'
        assert cbom_path.exists(), "cbom.json was not created"

        with open(cbom_path, 'r') as fh:
            cbom = json.load(fh)

        assert cbom['bomFormat'] == 'CycloneDX'
        assert cbom['specVersion'] == '1.6'
        assert 'components' in cbom
        assert 'dependencies' in cbom
        assert 'metadata' in cbom
        assert len(cbom['components']) > 0, "CBOM should contain algorithm components"

    def test_contains_algorithm_components(self, tmp_path):
        """CBOM components should include cryptographic-asset entries."""
        fake_root = tmp_path / "liboqs"
        for subdir in ['kem', 'sig', 'sig_stfl']:
            src = os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', subdir)
            dst = fake_root / 'docs' / 'algorithms' / subdir
            if os.path.isdir(src):
                shutil.copytree(src, dst)
        (fake_root / 'docs').mkdir(parents=True, exist_ok=True)

        run_script([sys.executable, os.path.join(SCRIPTS_DIR, 'update_cbom.py'),
                     '--liboqs-root', str(fake_root),
                     '--liboqs-version', '0.0.0-test'])

        with open(fake_root / 'docs' / 'cbom.json', 'r') as fh:
            cbom = json.load(fh)

        crypto_assets = [c for c in cbom['components']
                         if c.get('type') == 'cryptographic-asset']
        assert len(crypto_assets) > 0, "Should have cryptographic-asset components"

    def test_idempotent_on_unchanged_algorithms(self, tmp_path):
        """Running twice with same input should not change algorithm content."""
        fake_root = tmp_path / "liboqs"
        for subdir in ['kem', 'sig', 'sig_stfl']:
            src = os.path.join(LIBOQS_ROOT, 'docs', 'algorithms', subdir)
            dst = fake_root / 'docs' / 'algorithms' / subdir
            if os.path.isdir(src):
                shutil.copytree(src, dst)
        (fake_root / 'docs').mkdir(parents=True, exist_ok=True)

        cmd = [sys.executable, os.path.join(SCRIPTS_DIR, 'update_cbom.py'),
               '--liboqs-root', str(fake_root),
               '--liboqs-version', '0.0.0-test']

        run_script(cmd)
        with open(fake_root / 'docs' / 'cbom.json', 'r') as fh:
            first = json.load(fh)

        run_script(cmd)
        with open(fake_root / 'docs' / 'cbom.json', 'r') as fh:
            second = json.load(fh)

        first_algs = sorted([c['bom-ref'] for c in first['components']
                             if c.get('type') == 'cryptographic-asset'])
        second_algs = sorted([c['bom-ref'] for c in second['components']
                              if c.get('type') == 'cryptographic-asset'])
        assert first_algs == second_algs


# --------------------------------------------------------------------------
# scripts/genkatdict.py
# --------------------------------------------------------------------------
class TestGenkatDict:
    """Test the genkatdict.py script that builds a JSON dict from .kat.sha256 files."""

    def test_produces_valid_json(self, tmp_path):
        """Should read .kat.sha256 files and write a valid kats.json."""
        sha_file1 = tmp_path / "algo1.kat.sha256"
        sha_file2 = tmp_path / "algo2.kat.sha256"
        sha_file1.write_text("abc123def456")
        sha_file2.write_text("789xyz000111")

        script_path = os.path.join(SCRIPTS_DIR, 'genkatdict.py')
        result = run_script(
            [sys.executable, script_path],
            working_dir=str(tmp_path))
        assert "algo1" in result, "Script should log algo1"
        assert "algo2" in result, "Script should log algo2"

        kats_json = tmp_path / "kats.json"
        assert kats_json.exists(), "kats.json was not created"
        with open(kats_json, 'r') as fh:
            data = json.load(fh)
        assert data["algo1"] == "abc123def456"
        assert data["algo2"] == "789xyz000111"

    def test_empty_directory_produces_empty_dict(self, tmp_path):
        """With no .kat.sha256 files, should produce an empty JSON object."""
        script_path = os.path.join(SCRIPTS_DIR, 'genkatdict.py')
        run_script(
            [sys.executable, script_path],
            working_dir=str(tmp_path))

        kats_json = tmp_path / "kats.json"
        assert kats_json.exists(), "kats.json was not created"
        with open(kats_json, 'r') as fh:
            data = json.load(fh)
        assert data == {}


if __name__ == "__main__":
    pytest.main(sys.argv)

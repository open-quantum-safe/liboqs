#!/bin/bash

export LIBOQS_HEAD="${LIBOQS_HEAD:-main}"
export LIBOQS_URL="${LIBOQS_URL:-git@github.com:open-quantum-safe/liboqs.git}"
export LIBOQS_DIR="${LIBOQS_DIR:-$HOME/liboqs}"

if [ -d "$HOME/artifacts" ]; then
    echo "artifacts directory found"
else
    echo "artifacts directory not found"
fi

mv_artifacts() {
    # Check if the artifacts directory exists
    if [ ! -d "$HOME/artifacts" ]; then
        echo "Error: artifacts directory does not exist."
        return 1
    fi

    # Copy the provided path(s) to the artifacts directory
    cp -r "$@" "$HOME/artifacts/"
    
    # Check if the copy was successful
    if [ $? -eq 0 ]; then
        echo "Successfully moved artifacts to $HOME/artifacts/"
    else
        echo "Error: Failed to move artifacts."
        return 1
    fi
}


echo "Cloning $LIBOQS_URL into $LIBOQS_DIR"
git clone --depth 1 --single-branch --branch $LIBOQS_HEAD $LIBOQS_URL $LIBOQS_DIR
cd $LIBOQS_DIR
python -m pip install -U pip wheel setuptools pytest pytest-xdist \
    && python -m pip install -r $LIBOQS_DIR/scripts/copy_from_upstream/requirements.txt

case "$1" in
    test_all)
        rm -rf build
        mkdir -p build
        cd build
        cmake -GNinja .. && ninja
        cd ..
        python3 -m pytest --numprocesses=auto --junit-xml build/pytest.xml
        mv_artifacts build/pytest.xml
        ;;
    "/bin/bash"|"shell")
        exec /bin/bash -i
        ;;
    *)
        echo "Executing command: $1"
        exec "$@"
        ;;
esac

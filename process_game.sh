#!/bin/bash
set -e

INPUT_ZIP="/input/game.zip"
OUTPUT_DIR="/output"
BUILD_DIR="/src/build"

if [ ! -f "$INPUT_ZIP" ]; then
    echo "Error: Game zip not found at $INPUT_ZIP"
    exit 1
fi

echo "Ensuring engine is built..."
chmod +x /src/docker_build.sh /src/extra/make_mapping.sh /src/extra/dump.sh
/src/docker_build.sh

echo "Processing game..."
cd "/src/build"

# Clean up previous runs if any (though inside container it should be fresh)
rm -rf gameasync preload

# Unzip game
echo "Unzipping game..."
mkdir -p gameasync
unzip -q "$INPUT_ZIP" -d gameasync_tmp

# Move contents so that Data/ etc are at top level of gameasync
# Sometimes zips have a top level folder
if [ -d "gameasync_tmp/Data" ]; then
    mv gameasync_tmp/* gameasync/
else
    # Find where Data is
    DATA_DIR=$(find gameasync_tmp -type d -name "Data" | head -n 1)
    if [ -z "$DATA_DIR" ]; then
         echo "Warning: Data directory not found in zip. Assuming flat structure or specialized zip."
         mv gameasync_tmp/* gameasync/
    else
         PARENT_DIR=$(dirname "$DATA_DIR")
         mv "$PARENT_DIR"/* gameasync/
    fi
fi
rm -rf gameasync_tmp

cd gameasync

# Copy standard rgss1 if custom not present
if [ ! -f "rgss.rb" ]; then
    cp ../rgss.rb .
fi

# Make mappings
echo "Generating file mappings..."
bash ../make_mapping.sh

# Preload data
echo "Generating preload data..."
# The original script does this:
# cp ../../extra/dump* .
# for f in Data/*; do ./dump.sh "$f"; done
# mv preload ..

cp ../dump.sh ../dump.rb ../dump_rgss.rb .
rm -rf preload
mkdir -p preload

# Only process if Data folder exists
if [ -d "Data" ]; then
    for f in Data/*; do
        if [ -f "$f" ]; then
            bash ./dump.sh "$f" > /dev/null
            echo "Processed file: $f"
        fi
    done
fi

# Move preload out
if [ -d "preload" ]; then
    mv preload ..
fi

# Cleanup dump scripts
rm -f dump.sh dump.rb dump_rgss.rb

cd ..

# Final assembly
echo "Assembling output..."
mkdir -p "$OUTPUT_DIR"
cp mkxp.html "$OUTPUT_DIR/index.html"
cp mkxp.wasm "$OUTPUT_DIR/"
cp mkxp.js "$OUTPUT_DIR/"
cp -r js "$OUTPUT_DIR/"
cp -r gameasync "$OUTPUT_DIR/"
if [ -d "preload" ]; then
    cp -r preload "$OUTPUT_DIR/"
fi
cp *.webmanifest "$OUTPUT_DIR/" || true

# Permission fix (optional, but good for docker mounted volumes)
chmod -R 777 "$OUTPUT_DIR"

echo "Done! Game built to $OUTPUT_DIR"

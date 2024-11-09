#!/bin/bash
set -e

script_name=$0
script_path=$(dirname "$0")

echo "Checking original disc files..."
$script_path/check_disc.sh

echo "Building the GDI data track..."

if [ ! -f build/output/tbg.bin ]; then
    echo "The game binary was not found at build/output/tbg.bin"
    echo "Please build the game binary first using make"
    exit 1
fi

cp build/output/tbg.bin $TBG_DISC/root/1ST_READ.BIN

buildgdi -raw -data $TBG_DISC/root/ -ip $TBG_DISC/IP.BIN -output $TBG_DISC/
#../GDIbuilder/repo/buildgdi/bin/Release/net6.0/linux-x64/buildgdi -raw -data $TBG_DISC/root/ -ip $TBG_DISC/IP.BIN -output $TBG_DISC/

echo "Data track built successfully at $TBG_DISC/track03.bin"
echo "Replace it it with the original one and run or burn the game."

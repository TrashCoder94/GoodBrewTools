#!/bin/sh

echo "Running Debug GBHeaderToolTest executable!" 
chmod +x Binaries/Debug-linux-x86_64/GBHeaderTool/GBHeaderTool
chmod +x Binaries/Debug-linux-x86_64/GBHeaderToolTest/GBHeaderToolTest
cd Binaries/Debug-linux-x86_64/GBHeaderToolTest
./GBHeaderToolTest
if [ $? -ne 0 ] 
then
echo "Running debug GBHeaderToolTest executable failed, exiting out..."
exit 1
fi

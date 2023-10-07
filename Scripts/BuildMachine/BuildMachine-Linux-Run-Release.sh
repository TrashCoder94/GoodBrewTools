#!/bin/sh

echo "Running Release GBHeaderToolTest executable!"
chmod +x Binaries/Release-linux-x86_64/GBHeaderTool/GBHeaderTool
chmod +x Binaries/Release-linux-x86_64/GBHeaderToolTest/GBHeaderToolTest
cd Binaries/Release-linux-x86_64/GBHeaderToolTest
./GBHeaderToolTest
if [ $? -ne 0 ] 
then
echo "Running release GBHeaderToolTest executable failed, exiting out..."
exit 1
fi

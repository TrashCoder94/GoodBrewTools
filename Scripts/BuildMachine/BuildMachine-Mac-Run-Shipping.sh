#!/bin/sh

echo "Running Shipping GBHeaderToolTest executable!"
chmod +x Binaries/Shipping-macosx-x86_64/GBHeaderTool/GBHeaderTool
chmod +x Binaries/Shipping-macosx-x86_64/GBHeaderToolTest/GBHeaderToolTest
cd Binaries/Shipping-macosx-x86_64/GBHeaderToolTest
./GBHeaderToolTest
if [ $? -ne 0 ] 
then
echo "Running shipping GBHeaderToolTest executable failed, exiting out..."
exit 1
fi

#!/bin/sh

echo "Generating Project with Premake!"
chmod +x ThirdParty/premake/Binaries/premake5_mac
ThirdParty/premake/Binaries/premake5_mac --file=premake5.lua gmake2
if [ $? -ne 0 ] 
then
echo "Generating project files failed, exiting out..."
exit 1
fi

echo "Building Shipping Configuration"
make config=shipping_x86_64
if [ $? -ne 0 ] 
then
echo "Building Shipping Configuration failed, exiting out..."
exit 1
fi

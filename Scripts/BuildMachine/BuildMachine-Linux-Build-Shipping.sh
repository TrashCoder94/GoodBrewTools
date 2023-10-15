#!/bin/sh

echo "Generating Project with Premake!"
chmod +x ThirdParty/premake/Binaries/premake5
ThirdParty/premake/Binaries/premake5 --file=premake5.lua gmake2
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

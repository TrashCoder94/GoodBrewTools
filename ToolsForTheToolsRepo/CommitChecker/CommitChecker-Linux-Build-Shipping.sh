#!/bin/sh

cd ..
cd ..

echo "Generating Project with Premake!"
chmod +x ThirdParty/premake/Binaries/premake5
ThirdParty/premake/Binaries/premake5 --file=premake5.lua gmake2
if [ $? -ne 0 ] 
then
echo "Error: Generating project files failed"
exit 1
fi

echo "Building Shipping Configuration"
make -B config=shipping_x86_64
if [ $? -ne 0 ] 
then
echo "Error: Building Shipping Configuration failed"
exit 1
fi

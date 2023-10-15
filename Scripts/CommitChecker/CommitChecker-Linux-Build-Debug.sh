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

echo "Building Debug Configuration"
make -B config=debug_x86_64
if [ $? -ne 0 ] 
then
echo "Error: Building Debug Configuration failed"
exit 1
fi

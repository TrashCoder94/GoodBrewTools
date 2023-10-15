@echo off
pushd %~dp0\..\..\

echo "Generating Project with Premake"
call ThirdParty\premake\Binaries\premake5.exe vs2022
devenv GBTools.sln /rebuild Shipping /project Premake
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building Shipping Configuration"
devenv GBTools.sln /rebuild Shipping
if %errorlevel% neq 0 exit /b %errorlevel%

popd
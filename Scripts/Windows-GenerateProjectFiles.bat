@echo off
pushd %~dp0\..\

echo "Generating Projects with Premake"
call ThirdParty\premake\Binaries\premake5.exe vs2022
devenv GBTools.sln /rebuild Debug /project Premake
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Restoring nuget packages"
nuget restore GBTools.sln

popd
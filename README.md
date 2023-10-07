# GoodBrewTools

[![Build](https://github.com/TrashCoder94/GoodBrewTools/actions/workflows/build.yml/badge.svg)](https://github.com/TrashCoder94/GoodBrewTools/actions/workflows/build.yml)
[![Test](https://github.com/TrashCoder94/GoodBrewTools/actions/workflows/test.yml/badge.svg)](https://github.com/TrashCoder94/GoodBrewTools/actions/workflows/test.yml)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/9cd29dd14f8c4a3f850a6d0fd89674eb)](https://app.codacy.com/gh/TrashCoder94/GoodBrewTools/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

## Prerequisites

### Linux
```
sudo apt-get update && sudo apt-get install xorg-dev libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libdrm-dev libgbm-dev xvfb fluxbox
```

## How To Setup

1.	Clone using your source control provider (I use GitHub Desktop for example :octocat:).
2.	Go to `/GoodBrewTools/Scripts/` and run the `GenerateProjectFiles` script for your os.
3.	Premake should have generated `GBTools.sln` (or platform equivalent) in your root folder. Open that and you should have code access now!

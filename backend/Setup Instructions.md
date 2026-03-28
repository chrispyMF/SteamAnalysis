# Backend Setup Guide

## Requirements
- gcc (or g++) compiler version greater than 5.4.0 (11 or above suggested)
- CMake version greater than 3.5
- vcpkg (if building on Windows)
- Git

---

# Installing Prerequisites

## Windows
### Installing Vcpkg
Install vcpkg through git:
```bash
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.bat
```

Add vcpkg to Windows environment variable *path*:
```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
$env:PATH = "$env:VCPKG_ROOT;$env:PATH"
```
Now you can check if vcpkage was installed correctly by typing `vcpkg` or `vcpkg.exe`.
- If this does not work, you will probably have to manually add vcpkg to *path* through the Control Panel.

### Installing Drogon
Now install Drogon framework through vcpkg:
```bash
vcpkg install drogon[core,ctl]:x64-windows
```

To use `drogon_ctl` command, you need to add some variables to *path*:
```bash
C:\dev\vcpkg\installed\x64-windows\tools\drogon
```
```bash
C:\dev\vcpkg\installed\x64-windows\bin
```
```bash
C:\dev\vcpkg\installed\x64-windows\lib
```
```bash
C:\dev\vcpkg\installed\x64-windows\include
```
```bash
C:\dev\vcpkg\installed\x64-windows\share
```
```bash
C:\dev\vcpkg\installed\x64-windows\debug\bin
```
```bash
C:\dev\vcpkg\installed\x64-windows\debug\lib
```

### Installing libcurl
Install libcurl through vcpkg:
```bash
vcpkg install curl
```

## Linux

### Library Dependencies
First you need to install the Drogon library dependencies:
- jsoncpp
  ```bash 
  sudo apt install libjsoncpp-dev 
  ```
- uuid
  ```bash
  sudo apt install uuid-dev
  ```
- zlib
  ```bash
  sudo apt install zlib1g-dev
  ```
- OpenSSL
  ```bash
  sudo apt install openssl libssl-dev
  ```

### Installing Drogon
Install by source:
```bash
cd $WORK_PATH
git clone https://github.com/drogonframework/drogon
cd drogon
git submodule update --init
mkdir build
cd build
cmake ..
make && sudo make install 
```

### Installing libcurl
Install through package manager. The exact command to use changes depending on you Linux distro.
```bash
sudo apt install libcurl14-openssl-dev
```

---

# Building Executable

## Windows
Run the PowerShell script, `build.ps1` in `backend/scripts`

You can modify the build by adding some arguments. To specify the type of build (e.g. Debug, Release), you add the `-Config` flag. If the flag is not provided, the script chooses `Release` as default.
```powershell
./build.ps1 -Config Debug 
```

Adding the `-Clean` flag will remove all the build files and rebuild again.
```powershell
./build.ps1 -Clean
```

You can add the `-Generator` flag to specify which CMake generator to use.
```powershell
./build.ps1 -Clean -Generator "Ninja"
```
**Note:** If the generator you use is different from the first time you build, CMake will throw some errors. Make sure you use the `-Clean` flag if you are going to use a different CMake generator.

## Linux
Run the bash script `build.sh` in `backend/scripts`

You can also modify the build by adding some arguments. The first argument specifies the build type.
```bash
./build.sh Debug
```
The second argument `clean` specifies if you want to remove all build files, and then rebuild.
```bash
./build.sh Release clean
```
<#
Usage:
  ./scripts/build.ps1
  ./scripts/build.ps1 -Config Debug
  ./scripts/build.ps1 -Clean
  ./scripts/build.ps1 -Generator "Visual Studio 17 2022"
#>

param(
  [ValidateSet("Debug","Release","RelWithDebInfo","MinSizeRel")]
  [string]$Config = "Release",

  [switch]$Clean,

  [string]$Generator = ""
)

$ErrorActionPreference = "Stop"

# Resolve project root (assumes script lives in backend/scripts/)
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RootDir = Resolve-Path (Join-Path $ScriptDir "..")
$BuildDir = Join-Path $RootDir "build"
$InstallDir = Join-Path $RootDir "install"

Write-Host "Root:   $RootDir"
Write-Host "Build:  $BuildDir"
Write-Host "Config: $Config"

if ($Clean) {
  if (Test-Path $BuildDir) { Remove-Item -Recurse -Force $BuildDir }
  if (Test-Path $InstallDir) { Remove-Item -Recurse -Force $InstallDir }
  Write-Host "Cleaned build/ and install/."
}

New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
New-Item -ItemType Directory -Force -Path $InstallDir | Out-Null

# Build up cmake configure args
$cmakeArgs = @(
  "-S", $RootDir,
  "-B", $BuildDir,
  "-DCMAKE_BUILD_TYPE=$Config",
  "-DCMAKE_INSTALL_PREFIX=$InstallDir"
)

# Generator (optional). If empty, CMake picks a default.
if (-not [string]::IsNullOrWhiteSpace($Generator)) {
  $cmakeArgs += "-G"
  $cmakeArgs += $Generator
  Write-Host "Generator: $Generator"
}

Write-Host "`n=== Configure ==="
cmake @cmakeArgs

Write-Host "`n=== Build ==="
# For multi-config generators (Visual Studio), --config matters.
cmake --build $BuildDir --config $Config -j

Write-Host "`n=== Install (optional) ==="
cmake --install $BuildDir --config $Config

Write-Host "`nDone."
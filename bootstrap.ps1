param(
    [ValidateSet('Debug', 'Release')]
    [string]$BuildType = 'Debug',

    [switch]$InstallDeps,

    [switch]$Help
)

$ErrorActionPreference = 'Stop'

Set-StrictMode -Version Latest

function Print-Error {
    param([string]$Message)
    Write-Error "[$([datetime]::UtcNow.ToString('u'))] $Message"
}

function Show-Usage {
    $ScriptName = $MyInvocation.MyCommand.Name

    Write-Host "Usage: $ScriptName [-BuildType Debug|Release] [-InstallDeps] [-Help]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -BuildType <TYPE>    Specify the build type (Debug or Release). Default is Debug."
    Write-Host "  -InstallDeps         Attempt to install required dependencies. Requires Administrator privileges."
    Write-Host "  -Help                Show this help message."
}

function Install-Dependencies {
    $isAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
    if (-not $isAdmin) {
        Print-Error 'Dependency installation requires Administrator privileges.'
        Print-Error 'Please re-run this script with "Run as Administrator" or install dependencies manually.'
        exit 1
    }

    $choco = Get-Command choco -ErrorAction SilentlyContinue
    if (-not $choco) {
        Write-Host 'Chocolatey not found. Attempting to install Chocolatey...'
        try {
            Set-ExecutionPolicy Bypass -Scope Process -Force
            [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
            Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

            $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path", "User")

            $choco = Get-Command choco -ErrorAction SilentlyContinue
            if (-not $choco) {
                Print-Error 'Chocolatey installation failed. Please install Chocolatey manually from https://chocolatey.org/install'
                exit 1
            }
            Write-Host 'Chocolatey installed successfully.' -ForegroundColor Green
        } catch {
            Print-Error 'Failed to install Chocolatey.'
            $_ | Out-String | Print-Error
            exit 1
        }
    }

    Write-Host 'Attempting to install required dependencies...'
    $PACKAGES = @('cmake', 'ninja', 'mingw')

    Write-Host "Using Chocolatey to install packages: $($PACKAGES -join ' ')"
    try {
        choco install $PACKAGES -y
    } catch {
        Print-Error 'Failed to install dependencies.'
        $_ | Out-String | Print-Error
        exit 1
    }
    Write-Host 'Dependencies installed successfully.' -ForegroundColor Green
}

if ($Help) {
    Show-Usage
    exit 0
}

if ($InstallDeps) {
    Install-Dependencies

    Write-Host 'Refreshing PATH environment variable for the current session...'
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path", "User")
}

$PROJECT_ROOT = $PSScriptRoot
$CMAKE_DIR = Join-Path -Path $PROJECT_ROOT -ChildPath 'cmake'

New-Item -ItemType Directory -Path $CMAKE_DIR -Force | Out-Null

$CPM_PATH = Join-Path $CMAKE_DIR 'CPM.cmake'
if (-not (Test-Path -Path $CPM_PATH)) {
    Write-Host 'Downloading CPM.cmake...'
    $CPM_URL = 'https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake'

    try {
        Invoke-WebRequest -Uri $CPM_URL -OutFile $CPM_PATH
    } catch {
        Print-Error 'Failed to download CPM.cmake.'
        $_ | Out-String | Print-Error
        exit 1
    }
}

Write-Host 'Configuring project...'
cmake --preset $BuildType

Write-Host 'Building project...'
cmake --build --preset $BuildType

Write-Host 'Build completed successfully.' -ForegroundColor Green
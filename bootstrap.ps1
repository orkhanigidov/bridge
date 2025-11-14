param(
    [ValidateSet('Debug', 'Release')]
    [string]$BuildType = 'Debug',

    [switch]$InstallDeps,

    [switch]$BuildShared,

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
    Write-Host "  -InstallDeps         Attempt to install required dependencies."
    Write-Host "  -BuildShared         Build the engine as shared library (DLL)."
    Write-Host "  -Help                Show this help message."
}

function Install-Dependencies {
    $scoop = Get-Command scoop -ErrorAction SilentlyContinue
    if (-not $scoop) {
        Write-Host 'Scoop not found. Attempting to install Scoop...'
        try {
            Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
            Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression

            $scoop = Get-Command scoop -ErrorAction SilentlyContinue
            if (-not $scoop) {
                Print-Error 'Scoop installation failed. Please install Scoop manually from https://scoop.sh/'
                exit 1
            }
            Write-Host 'Scoop installed successfully.' -ForegroundColor Green
        } catch {
            Print-Error 'Failed to install Scoop.'
            $_ | Out-String | Print-Error
            exit 1
        }
    }

    Write-Host 'Attempting to install required dependencies...'
    $PACKAGES = @('main/cmake', 'main/ninja', 'main/mingw-winlibs')

    Write-Host "Using Scoop to install packages: $($PACKAGES -join ' ')"
    try {
        scoop install $PACKAGES
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

$cmakeConfigureArgs = @(
    "--preset", $BuildType
)

if ($BuildShared) {
    Write-Host 'Configuring to build as a SHARED library.' -ForegroundColor Yellow
    $cmakeConfigureArgs += "-DBUILD_SHARED_LIB:BOOL=ON"
} else {
    Write-Host 'Configuring to build as an EXECUTABLE.' -ForegroundColor Yellow
    $cmakeConfigureArgs += "-DBUILD_SHARED_LIB:BOOL=OFF"
}

Write-Host "Configuring project (cmake $($cmakeConfigureArgs -join ' '))..."
cmake @cmakeConfigureArgs

Write-Host 'Building project...'
cmake --build --preset $BuildType

Write-Host 'Build completed successfully.' -ForegroundColor Green
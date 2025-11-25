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
    $PACKAGES = @('main/cmake', 'main/ninja', 'main/mingw', 'main/python')

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

$cmakeConfigureArgs = @("--preset", $BuildType)

if ($BuildShared) {
    Write-Host 'Configuring to build as a SHARED library.' -ForegroundColor Yellow
    $cmakeConfigureArgs += "-DBUILD_SHARED_LIB:BOOL=ON"
} else {
    Write-Host 'Configuring to build as an EXECUTABLE.' -ForegroundColor Yellow
    $cmakeConfigureArgs += "-DBUILD_SHARED_LIB:BOOL=OFF"
}

Write-Host "Configuring project (cmake $($cmakeConfigureArgs -join ' '))..."
cmake @cmakeConfigureArgs
if ($LASTEXITCODE -ne 0) {
    Print-Error "CMake configure failed with exit code $LASTEXITCODE."
    exit $LASTEXITCODE
}

Write-Host 'Building project...'
cmake --build --preset $BuildType
if ($LASTEXITCODE -eq 0) {
    Write-Host 'Build completed successfully.' -ForegroundColor Green
} else {
    Print-Error "Build failed with exit code $LASTEXITCODE."
    exit $LASTEXITCODE
}
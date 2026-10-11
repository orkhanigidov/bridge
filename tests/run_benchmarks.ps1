$SERVER_BIN = "../build/Engine"

function Start-Server {
  Write-Host "Starting Engine Server..."
  $global:SERVER_PROCESS = Start-Process -FilePath $SERVER_BIN -NoNewWindow -PassThru
  Write-Host "Server PID: $($global:SERVER_PROCESS.Id)"
  Start-Sleep -Seconds 2
}

function Stop-Server {
  Write-Host "Stopping Engine Server..."
  Stop-Process -Id $global:SERVER_PROCESS.Id -ErrorAction SilentlyContinue
  Wait-Process -Id $global:SERVER_PROCESS.Id -ErrorAction SilentlyContinue
  Write-Host "Server Stopped."
  Start-Sleep -Seconds 1
}

Start-Server
Write-Host ">>> Running PERFORMANCE Test..."
python test_performance.py
Stop-Server

Write-Host "--------------------------------------"

Start-Server
Write-Host ">>> Running CONCURRENCY Test..."
python test_concurrency.py
Stop-Server

Write-Host "--------------------------------------"

Start-Server
Write-Host ">>> Running MEMORY STABILITY Test..."
python test_memory_stress.py
Stop-Server

Write-Host "--------------------------------------"

Start-Server
Write-Host ">>> Running SECURITY Test..."
python test_security.py
Stop-Server

Write-Host "--------------------------------------"
Write-Host ""
Write-Host "ALL TESTS COMPLETED."
#!/bin/bash

SERVER_BIN="../build/Engine"

start_server() {
  echo "Starting Engine Server..."
  $SERVER_BIN > /dev/null 2>&1 &
  SERVER_PID=$!
  echo "Server PID: $SERVER_PID"
  sleep 2
}

stop_server() {
  echo "Stopping Engine Server..."
  kill $SERVER_PID
  wait $SERVER_PID 2>/dev/null
  echo "Server Stopped."
  sleep 1
}

start_server
echo
echo ">>> Running PERFORMANCE Test..."
python performance_test.py
stop_server

echo "--------------------------------------"

start_server
echo ">>> Running CONCURRENCY Test..."
python concurrency_test.py
stop_server

echo "--------------------------------------"

start_server
echo ">>> Running MEMORY STABILITY Test..."
python memory_stress_test.py
stop_server

echo "--------------------------------------"

start_server
echo ">>> Running SECURITY Test..."
python security_test.py
stop_server

echo "--------------------------------------"
echo
echo "ALL TESTS COMPLETED."
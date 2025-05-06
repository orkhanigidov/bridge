#pragma once

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <nlohmann/json.hpp>
#include <rttr/registration>
#include <rttr/variant.h>
#include <zmq.hpp>
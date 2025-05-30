#pragma once

#include <atomic>
#include <chrono>
#include <csignal>
#include <functional>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

// 3rd party includes
#include <nlohmann/json.hpp>
#include <rttr/registration>
#include <rttr/variant.h>
#include <zmq.hpp>
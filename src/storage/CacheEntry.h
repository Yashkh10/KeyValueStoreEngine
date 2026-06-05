#pragma once

#include <string>
#include <chrono>
#include <cstdint>

struct CacheEntry {
    std::string value;
    std::chrono::steady_clock::time_point expiryTime;
    bool hasExpiry = false;
    uint64_t version = 0;
};
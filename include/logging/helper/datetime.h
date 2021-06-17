#pragma once

#include <ctime>

namespace logging {
/**
 * @brief Converts given time since epoch into calendar LOCAL SYSTEM time in a thread-safe way.
 * 
 * @param dt    Pointer to the std::tm structure to be filled in.
 * @param t     Unix time.
 * @return std::tm* copy of the dt pointer, or nullptr on error 
 */
std::tm* local_datetime(std::tm* dt, std::time_t t);

/**
 * @brief Converts given time since epoch into calendar UTC time in a thread-safe way.
 * 
 * @param dt    Pointer to the std::tm structure to be filled in.
 * @param t     Unix time.
 * @return std::tm* copy of the dt pointer, or nullptr on error 
 */
std::tm* utc_datetime(std::tm* dt, std::time_t t);

}
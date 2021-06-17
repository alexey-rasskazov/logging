#define __STDC_WANT_LIB_EXT1__ 1
#if defined(_MSC_VER)
#define __STDC_WANT_SECURE_LIB__ 1
#endif

#include <logging/helper/datetime.h>
#include <time.h>

#ifdef __STDC_LIB_EXT1__
#   define SECURE_EXT1
#else

#if defined(_MSC_VER)
#   if defined(__STDC_SECURE_LIB__) 
#       define SECURE_EXT1
#   else
#       define INSECURE
#   endif
#else
#   ifdef __unix__
#       define SECURE_R
#   else
#       define INSECURE
#   endif
#endif

#endif

#ifdef INSECURE
#include <mutex>
static std::mutex time_mutex;
#endif

/*
 * Local system time.
 * Uses localtime_s or localtime_r or localtime and mutex
 * to convert time.
 */
std::tm* logging::local_datetime(std::tm* dt, std::time_t t)
{
    if (!dt) {
        return nullptr;
    }

#ifdef SECURE_EXT1
    if (localtime_s(dt, &t) != 0) {
        return nullptr;
    }
#endif

#ifdef SECURE_R
    if (localtime_r(&t, dt) == nullptr) {
        return nullptr;
    }
#endif

#ifdef INSECURE
    std::lock_guard<std::mutex> lock(time_mutex);
    std::tm* tmp = std::localtime(&t);
    if (tmp == nullptr) {
        return nullptr;
    }
    *dt = *tmp;
#endif

    return dt;
}

/*
 * UTC time.
 * Uses gmtime_s or gmtime_r or gmtime and mutex
 * to convert time.
 */
std::tm* logging::utc_datetime(std::tm* dt, std::time_t t)
{
    if (!dt) {
        return nullptr;
    }

#ifdef SECURE_EXT1
    if (gmtime_s(dt, &t) != 0) {
        return nullptr;
    }
#endif

#ifdef SECURE_R
    if (gmtime_r(&t, dt) == nullptr) {
        return nullptr;
    }
#endif

#ifdef INSECURE
    std::lock_guard<std::mutex> lock(time_mutex);
    std::tm* tmp = std::gmtime(&t);
    if (tmp == nullptr) {
        return nullptr;
    }
    *dt = *tmp;
#endif

    return dt;
}
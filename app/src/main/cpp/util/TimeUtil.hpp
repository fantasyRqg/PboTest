//
// Created by ranqingguo on 7/5/17.
//

#ifndef PBOTEST_TIMEUTIL_HPP
#define PBOTEST_TIMEUTIL_HPP


#include <linux/time.h>
#include <cstdint>
#include <time.h>

int64_t systemnanotime() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000LL + now.tv_nsec;
}


#endif //PBOTEST_TIMEUTIL_HPP

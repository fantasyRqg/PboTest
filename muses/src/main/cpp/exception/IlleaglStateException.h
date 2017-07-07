//
// Created by ranqingguo on 7/5/17.
//

#ifndef MUSES_ILLEAGLSTATEEXCEPTION_H
#define MUSES_ILLEAGLSTATEEXCEPTION_H

#include <stdexcept>

class IllegalStateException : public std::runtime_error {
public:
    IllegalStateException(const std::string &__arg) noexcept : runtime_error(__arg) {}
};

#endif //MUSES_ILLEAGLSTATEEXCEPTION_H

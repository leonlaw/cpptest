//
// Created by kenny on 11/2/2020.
//
#include "base_class.h"
#include <iostream>

namespace base_class {
    BaseClass::BaseClass() {
        std::cout << "BaseClass()" << std::endl;
    }

    BaseClass::~BaseClass() {
        std::cout << "~BaseClass()" << std::endl;
    }

    BaseClass::BaseClass(BaseClass const &) {
        std::cout << "BaseClass(BaseClass const&) " << std::endl;
    }

    BaseClass::BaseClass(BaseClass &&) noexcept {
        std::cout << "BaseClass(BaseClass &&) " << std::endl;
    }

    BaseClass &BaseClass::operator=(BaseClass const &) {
        std::cout << "BaseClass &operator=(BaseClass const &) " << std::endl;
        return *this;
    }

    BaseClass &BaseClass::operator=(BaseClass &&) noexcept {
        std::cout << "BaseClass &operator=(BaseClass &&) " << std::endl;
        return *this;
    }

    void BaseClass::fun() const {
        std::cout << "    BaseClass" << std::endl;
    }
}
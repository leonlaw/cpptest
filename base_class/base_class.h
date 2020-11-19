//
// Created by kenny on 11/2/2020.
//

#ifndef CPPTEST_BASE_CLASS_H
#define CPPTEST_BASE_CLASS_H

namespace base_class {
    class BaseClass {
    public:
        BaseClass();

        virtual ~BaseClass();

        BaseClass(BaseClass const &);

        BaseClass(BaseClass &&) noexcept;

        BaseClass &operator=(BaseClass const &);

        BaseClass &operator=(BaseClass &&) noexcept;

        virtual void fun() const;
    };
}

#endif //CPPTEST_BASE_CLASS_H
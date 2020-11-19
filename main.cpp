#include <iostream>
#include <vector>

#include "base_class.h"
#include "callback.h"
#include "functional_programming.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;
    //callback::test();
    //functional_programming::test();

    {
        //move append test
        using namespace base_class;
        std::vector<BaseClass> src(4);
        std::vector<BaseClass> dst;
        dst.reserve(4);
        std::move(src.begin(), src.end(), std::back_inserter(dst));
    }

    return 0;
}

#include "CY/safety.hpp"
#include "CY/types.hpp"
#include <cassert>
#include <cstdio>
#include <exception>

cy::Result<void, str> Write(str                               msg,
                            bool /* i know, pretty lazy... */ fail = false)
{
    if (fail)
        return cy::Err("Oh, no! Something really bad actually happened!");

    std::printf("%s\n", msg);
    return cy::Ok();
}

int32 main(void)
{
    auto a = Write("Hi, mom!", false);

    try {
        auto msg = a.UnwrapErr();
        std::printf("An error ocurred: %s\n", msg);
    } catch (std::exception e) {
        std::printf("UnwrapErr() responded with: %s\n", e.what());
    }

    return 0;
}
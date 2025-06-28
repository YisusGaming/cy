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
    std::printf("\n-----------------------TESTING: "
                "Result-------------------------\n\n");

    auto a = Write("Hi, mom!", false);

    try {
        auto msg = a.unwrap_err();
        std::printf("An error ocurred: %s\n", msg);
    } catch (std::exception e) {
        std::printf("[OK, expected] UnwrapErr() responded with: %s\n",
                    e.what());
    }

    int32 b = 10;
    auto  result = cy::Result<int &, std::exception>(cy::Ok<int &>(b));

    assert(result.is_ok() && !result.is_err());

    auto ok = result.ok();
    assert(ok.is_some());

    try {
        result.unwrap();
    } catch (std::exception e) {
        std::printf("[OK, expected] After moving via .ok(), .unwrap() responds "
                    "with: %s\n",
                    e.what());
    }

    auto err = result.err();
    assert(err.is_none());

    int32 &b_recovered = ok.unwrap();
    assert(&b == &b_recovered);
    std::printf("%p == %p succeeded!\n", &b, &b_recovered);
    assert(b == b_recovered);
    std::printf("%i == %i succeeded!\n", b, b_recovered);

    std::printf("\n-----------------------OK-------------------------\n");
    return 0;
}
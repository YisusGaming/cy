#include "CY/safety.hpp"
#include "CY/types.hpp"
#include <cassert>
#include <cstdio>
#include <string>

// #define STATIC_ASSERT_FAIL
#if defined(STATIC_ASSERT_FAIL)
struct Test
{
    int32 a;
    Test(int32 a)
        : a(a)
    {
    }
};
cy::Maybe<void> maybe_V;
cy::Some<void>  some_V;
cy::Maybe<Test> maybe_NoDefaultC{ cy::None() };
#endif

struct Find
{
    char  c;
    usize indx;

    Find() = default;

    Find(char c, usize i)
        : c(c)
        , indx(i)
    {
    }

    Find(const Find &) = delete;
    Find &operator=(const Find &) = delete;

    Find(Find &&) = default;
    Find &operator=(Find &&) = default;
};

cy::Maybe<Find> FindCharInString(std::string const &str, char fc)
{
    for (usize i = 0; i < str.size(); i++) {
        char c = str[i];

        if (c == fc) {
            return cy::Some(Find(c, i));
        }
    }

    return cy::None();
}

int32 main()
{
    std::string test = "I am an epic test string!";
    auto        maybe = FindCharInString(test, 'a');

    if (maybe.has_Value()) {
        auto a = maybe.Unwrap();

        std::printf("Found '%c' at index %i!\n", a.c, a.indx);
    } else {
        std::printf("Nothing found!\n");
    }

    return 0;
}
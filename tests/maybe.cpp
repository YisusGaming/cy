#include "CY/safety.hpp"
#include "CY/types.hpp"
#include <cassert>
#include <cstdio>
#include <memory>
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
cy::Maybe<Test> maybe_NoDefaultC{
    cy::None()
}; // no longer fails, this is allowed now!
#endif

struct Find
{
    char  c;
    usize indx;

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

std::string              test1 = "Epic testing over here";
std::string              test2 = "yessir I agree";
std::string              test3 = "How many more strings do I need?";
std::string              test4 = "Pfft, I don't know.";
cy::Maybe<std::string &> GetString(usize indx)
{
    if (indx == 0)
        return cy::Some<std::string &>(test1);
    else if (indx == 1)
        return cy::Some<std::string &>(test2);
    else if (indx == 2)
        return cy::Some<std::string &>(test3);
    else if (indx == 3)
        return cy::Some<std::string &>(test4);

    return cy::None();
}

int32 main()
{
    std::string test = "I am an epic test string!";
    auto        maybe = FindCharInString(test, 'a');

    if (maybe.is_some()) {
        auto a = maybe.unwrap();

        std::printf("Found '%c' at index %i!\n", a.c, a.indx);
    } else {
        std::printf("Nothing found!\n");
    }

    auto &str = GetString(0).unwrap();
    assert(str == test1);
    assert(std::addressof(str) == &test1);

    auto other_str = GetString(29);
    assert(other_str.is_some() == false && other_str.is_none() == true);

    int32 number = 18;
    auto  thing = cy::Maybe(cy::Some(number));
    auto  other_thing = thing.map<float32>(
        [](auto value) { return static_cast<float32>(value); });

    assert(number == other_thing.get());

    return 0;
}
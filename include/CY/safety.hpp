#pragma once

#include <algorithm>
#include <exception>
#include <type_traits>
namespace cy {
template<typename T>
class Maybe;

template<typename T>
class Some
{
    static_assert(std::is_void_v<T>, "Maybe<void> is invalid.");

  private:
    T val;

  public:
    explicit constexpr Some(T const &val)
        : val(val)
    {
    }
    explicit constexpr Some(T &&val)
        : val(std::move(val))
    {
    }

    constexpr T const &get() const & { return this->val; }
    constexpr T      &&get()      &&{ return std::move(this->val); }

    constexpr operator Maybe<T>() const & { return Maybe(Some(val)); }
    constexpr operator Maybe<T>() && { return Maybe(Some(std::move(val))); }
};

class None
{
  public:
    constexpr None() = default;
};

template<typename T>
class Maybe
{
    static_assert(std::is_void_v<T>, "Maybe<void> is invalid.");
    static_assert(!std::is_default_constructible_v<T>,
                  "T is required to be default constructible.");

  private:
    T    val;
    bool has_Val;

    constexpr T const &some_unchecked() const & { return this->val; }
    constexpr T       &some_unchecked()       &{ return this->val; }
    constexpr T      &&some_unchecked()      &&{ return std::move(this->val); }

  public:
    constexpr Maybe(const Maybe &) = default;
    constexpr Maybe(Maybe &&) = default;
    constexpr Maybe &operator=(const Maybe &) = default;
    constexpr Maybe &operator=(Maybe &&) = default;

    constexpr Maybe(Some<T> val)
        : val(std::move(val.get()))
        , has_Val(true)
    {
    }

    constexpr Maybe(None)
        : has_Val(false)
    {
    }

    constexpr bool     has_Value() const { return this->has_Val; }
    constexpr T const &Some() const &
    {
        if (!has_Value())
            throw std::exception("Called Some() on a None value.");
        return this->some_unchecked();
    }
    constexpr T &Some() &
    {
        if (!has_Value())
            throw std::exception("Called Some() on a None value.");
        return this->some_unchecked();
    }
    constexpr T CloneSome() const &
    {
        if (!has_Value())
            throw std::exception("Called CloneSome() on a None value.");
        return this->some_unchecked();
    }

    constexpr T &&Unwrap()
    {
        if (!has_Value())
            throw std::exception("Called Unwrap() on a None value.");

        return std::move(*this).some_unchecked();
    }
};
}
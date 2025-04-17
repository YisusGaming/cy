#pragma once

#include <algorithm>
namespace cy {
template<typename>
class Maybe;

template<typename T>
class Some
{
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

    constexpr T const &value() const & { return this->val; }
    constexpr T      &&value()      &&{ return std::move(this->val); }

    constexpr operator Maybe<T>() const & { return Maybe(Some(val)); }
    constexpr operator Maybe<T>() && { return Maybe(Some(std::move(val))); }
};

class None
{
  public:
    constexpr None() = default;
};
}
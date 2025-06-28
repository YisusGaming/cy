/**
 * @file safety.hpp
 * @author Jesús Blanco
 * @brief General code safety with value/error or value/none classes.
 * @version 2.0.0
 * @date 2025-06-27
 *
 * @copyright Copyright (c) Jesús Blanco. See LICENSE for details.
 *
 */

#pragma once

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <type_traits>
namespace cy {
template<typename T>
class Maybe;

template<typename T>
class Some
{
    static_assert(!std::is_void_v<T>, "Maybe<void> is invalid.");

  private:
    T val;

  public:
    friend Maybe<T>;

    constexpr Some(T value)
        : val(std::move(value))
    {
    }

    inline constexpr T const &get() const & { return this->val; }
    inline constexpr T       &get()       &{ return this->val; }
    inline const T          &&take() { return std::move(this->val); }
};

template<typename T>
class Some<T &>
{
    static_assert(!std::is_void_v<T>, "Maybe<void> is invalid.");

  private:
    T *val;

  public:
    friend Maybe<T &>;

    constexpr Some(T &value)
        : val(&value)
    {
    }

    inline constexpr T const &get() const & { return *this->val; }
    inline constexpr T       &get()       &{ return *this->val; }
};

class None
{
  public:
    constexpr None() = default;
};

template<typename T>
class Maybe
{
    static_assert(!std::is_void_v<T>, "Maybe<void> is invalid.");

  private:
    bool has_value;
    T    value;

    inline T const &get_unchecked() const & { return this->value; }
    inline T       &get_unchecked()       &{ return this->value; }
    inline T      &&unwrap_unchecked()
    {
        this->has_value = false;
        return std::move(this->value);
    }

  public:
    constexpr Maybe(Some<T> some)
        : has_value(true)
        , value(std::move(some.val))
    {
        if constexpr (std::is_class_v<T>) {
            static_assert(
                std::is_default_constructible_v<T>,
                "Maybe<class T> requires T to be default constructible.");
        }
    }

    constexpr Maybe(None)
        : has_value(false)
    {
        if constexpr (std::is_class_v<T>) {
            static_assert(
                std::is_default_constructible_v<T>,
                "Maybe<class T> requires T to be default constructible.");
        }
    }

    constexpr Maybe()
        : has_value(false)
    {
        if constexpr (std::is_class_v<T>) {
            static_assert(
                std::is_default_constructible_v<T>,
                "Maybe<class T> requires T to be default constructible.");
        }
    }

    inline constexpr bool is_some() const { return this->has_value; }
    inline constexpr bool is_none() const { return !this->is_some(); }

    T const &get() const &
    {
        if (!this->has_value)
            throw std::runtime_error("Called .get() on a none value");

        return this->get_unchecked();
    }
    T &get() &
    {
        if (!this->has_value)
            throw std::runtime_error("Called .get() on a none value");

        return this->get_unchecked();
    }
    T &&unwrap()
    {
        if (!this->has_value)
            throw std::runtime_error("Called .unwrap() on a none value");

        return this->unwrap_unchecked();
    }

    template<typename U>
    Maybe<U> map(std::function<U(T)> func)
    {
        if (this->has_value) {
            return Some(func(this->unwrap_unchecked()));
        }

        return None();
    }
};
template<typename T>
class Maybe<T &>
{
    static_assert(!std::is_void_v<T>, "Maybe<void> is invalid.");

  private:
    bool has_value;
    T   *value;

    inline T &unwrap_unchecked()
    {
        this->has_value = false;
        return *this->value;
    }

  public:
    constexpr Maybe(Some<T &> some)
        : has_value(true)
        , value(some.val)
    {
    }

    constexpr Maybe(None)
        : has_value(false)
    {
    }

    constexpr Maybe()
        : has_value(false)
    {
        if constexpr (std::is_class_v<T>) {
            static_assert(
                std::is_default_constructible_v<T>,
                "Maybe<class T> requires T to be default constructible.");
        }
    }

    inline constexpr bool is_some() const { return this->has_value; }
    inline constexpr bool is_none() const { return !this->is_some(); }

    T &unwrap()
    {
        if (!this->has_value)
            throw std::runtime_error("Called .unwrap() on a none value");

        return this->unwrap_unchecked();
    }

    template<typename U>
    Maybe<U> map(std::function<U(T &)> func)
    {
        if (this->has_value) {
            return Some(func(this->unwrap_unchecked()));
        }

        return None();
    }
};
}
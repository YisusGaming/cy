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
    union
    {
        T value;
    };

    inline T const &get_unchecked() const & { return this->value; }
    inline T       &get_unchecked()       &{ return this->value; }
    inline T      &&unwrap_unchecked()
    {
        this->has_value = false;
        return std::move(this->value);
    }

  public:
    ~Maybe()
    {
        if (this->has_value)
            this->value.~T();
    }

    constexpr Maybe(Some<T> some)
        : has_value(true)
        , value(std::move(some.val))
    {
    }

    constexpr Maybe(None)
        : has_value(false)
    {
    }

    constexpr Maybe()
        : has_value(false)
    {
    }

    inline constexpr bool is_some() const { return this->has_value; }
    inline constexpr bool is_none() const { return !this->is_some(); }

    constexpr T const &get() const &
    {
        if (!this->has_value)
            throw std::runtime_error("Called .get() on a none value");

        return this->get_unchecked();
    }
    constexpr T &get() &
    {
        if (!this->has_value)
            throw std::runtime_error("Called .get() on a none value");

        return this->get_unchecked();
    }
    constexpr T &&unwrap()
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

    constexpr T &unwrap()
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

template<typename T>
class Ok
{
  private:
    T value;

  public:
    constexpr Ok(T v)
        : value(std::move(v))
    {
    }

    inline T const &get() const & { return this->value; }
    inline T       &get()       &{ return this->value; }
    inline T      &&take() { return std::move(this->value); }
};

template<typename E>
class Err
{
    static_assert(!std::is_void_v<E>,
                  "Result<T, void> is invalid. Use Maybe<T> instead.");

  private:
    E err;

  public:
    constexpr Err(E error)
        : err(std::move(error))
    {
    }

    inline E const &get() const & { return this->err; }
    inline E       &get()       &{ return this->err; }
    inline E      &&take() { return std::move(this->err); }
};

template<typename T, typename E>
class [[nodiscard("Result must be handled.")]] Result
{
  private:
    bool is_error;
    bool has_data;

    union
    {
        T value;
        E error;
    };

    inline T const &get_unchecked() const & { return this->value; }
    inline T       &get_unchecked()       &{ return this->value; }

    inline E const &get_err_unchecked() const & { return this->error; }
    inline E       &get_err_unchecked()       &{ return this->error; }

    inline T &&unwrap_unchecked()
    {
        this->has_data = false;
        return std::move(this->value);
    }

    inline E &&unwrap_err_unchecked()
    {
        this->has_data = false;
        return std::move(this->error);
    }

  public:
    ~Result()
    {
        if (this->is_error && this->has_data)
            this->error.~E();
        else if (this->has_data)
            this->value.~T();
    }

    constexpr Result(Ok<T> ok)
        : is_error(false)
        , has_data(true)
        , value(ok.take())
    {
    }

    constexpr Result(Err<E> err)
        : is_error(true)
        , has_data(true)
        , error(err.take())
    {
    }

#define CHECK_IF_VALID_T(func)                                                 \
    if (this->is_error)                                                        \
        throw std::runtime_error("Called " func " on an error value");         \
    if (!this->has_data)                                                       \
        throw std::runtime_error(                                              \
            "Called " func                                                     \
            " on a moved value (Result had T, but was unwrapped)");

#define CHECK_IF_VALID_E(func)                                                 \
    if (!this->is_error)                                                       \
        throw std::runtime_error("Called " func " on an ok value");            \
    if (!this->has_data)                                                       \
        throw std::runtime_error(                                              \
            "Called " func                                                     \
            " on a moved value (Result had E, but was unwrapped)");

    constexpr T const &get() const &
    {
        CHECK_IF_VALID_T(".get()");

        return this->get_unchecked();
    }

    constexpr T &get() &
    {
        CHECK_IF_VALID_T(".get()");

        return this->get_unchecked();
    }

    constexpr E const &get_err() const &
    {
        CHECK_IF_VALID_E(".get_err()");

        return this->get_err_unchecked();
    }

    constexpr E &get_err() &
    {
        CHECK_IF_VALID_E(".get_err()");

        return this->get_err_unchecked();
    }

    constexpr T &&unwrap()
    {
        CHECK_IF_VALID_T(".unwrap()");

        return this->unwrap_unchecked();
    }

    constexpr E &&unwrap_err()
    {
        CHECK_IF_VALID_E(".unwrap_err()");

        return this->unwrap_err_unchecked();
    }

    constexpr Maybe<T> ok()
    {
        if (!this->is_error && this->has_data) {
            return Some(this->unwrap_unchecked());
        }

        return None();
    }

    constexpr Maybe<E> err()
    {
        if (this->is_error && this->has_data) {
            return Some(this->unwrap_err_unchecked());
        }

        return None();
    }
};
}
/**
 * @file safety.hpp
 * @author Jesús Blanco
 * @brief General code safety with value/error or value/none classes.
 * @version 1.0.0
 * @date 2025-04-19
 *
 * @copyright Copyright (c) Jesús Blanco. See LICENSE for details.
 *
 */

#pragma once

#include <algorithm>
#include <exception>
#include <type_traits>
namespace cy {
template<typename T>
class Maybe;

/**
 * @brief `Some` owns a value of type `T`.
 *
 * @tparam T The type of value owned by `Some`.
 * @sa @ref None
 * @sa @ref Maybe<T>
 *
 * @since v1.0.0
 */
template<typename T>
class Some
{
    static_assert(!std::is_void_v<T>, "Maybe<void> is invalid.");

  private:
    T val;

  public:
    /**
     * @brief Constructs a new `Some` by copying `val`.
     *
     * @sa @ref None
     * @sa @ref Maybe<T>
     *
     * @since v1.0.0
     */
    explicit constexpr Some(T const &val)
        : val(val)
    {
    }
    /**
     * @brief Constructs a new `Some` by moving `val`.
     *
     * @sa @ref None
     * @sa @ref Maybe<T>
     *
     * @since v1.0.0
     */
    explicit constexpr Some(T &&val)
        : val(std::move(val))
    {
    }

    /**
     * @return Gets a const reference (`T const&`) to the value owned by `Some`.
     * @since v1.0.0
     */
    constexpr T const &get() const & { return this->val; }
    /**
     * @return Gets a reference (`T&`) to the value owned by `Some`.
     * @since v1.0.0
     */
    constexpr T &get() & { return this->val; }
    /**
     * @return Moves the value out of `Some`.
     * @since v1.0.0
     */
    constexpr T &&get() && { return std::move(this->val); }
};

/**
 * @attention This is a template specialization for `T&`. `Some<T&>` holds a
 * pointer to `T` in order to initialize `Maybe<T&>`, which requires a `T*`.
 * @brief Contains a reference to `T`.
 *
 * @tparam T The type of value.
 * @sa @ref None
 * @sa @ref Maybe<T>
 *
 * @since v1.0.0
 */
template<typename T>
class Some<T &>
{
    static_assert(!std::is_void_v<T>, "Maybe<void> is invalid.");

  private:
    T *val;

  public:
    friend Maybe<T &>;

    /**
     * @brief Initializes the reference in `Some`.
     * @since v1.0.0
     */
    explicit constexpr Some(T &value)
        : val(&value)
    {
    }

    /**
     * @return Gets a const reference (`T const&`) to the value.
     * @since v1.0.0
     */
    constexpr T const &get() const & { return *this->val; }
    /**
     * @return Gets a reference (`T&`) to the value.
     * @since v1.0.0
     */
    constexpr T &get() & { return *this->val; }
};

/**
 * @brief `None` represents the absence of a value. It stores nothing...
 *
 * @sa @ref Some<T>
 * @sa @ref Maybe<T>
 *
 * @since v1.0.0
 */
class None
{
  public:
    constexpr None() = default;
};

/**
 * @brief `Maybe` is used to handle cases where a value of type `T` might or
 * might not exist.
 *
 * If constructed with `Some<T>`, `Maybe<T>` will now own the value of type `T`.
 *
 * @sa @ref None
 * @sa @ref Some<T>
 *
 * @since v1.0.0
 */
template<typename T>
class Maybe
{
    static_assert(!std::is_void_v<T>, "Maybe<void> is invalid.");

  private:
    T    val;
    bool has_Val;

    /**
     * @return Gets a const reference (`T const&`) to the value owned by
     * `Maybe<T>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr T const &some_unchecked() const & { return this->val; }
    /**
     * @return Gets a reference (`T&`) to the value owned by
     * `Maybe<T>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr T &some_unchecked() & { return this->val; }
    /**
     * @return Moves the value out of `Maybe<T>` without checking if it's
     * actually got a value.
     * @since v1.0.0
     */
    constexpr T &&some_unchecked() && { return std::move(this->val); }

  public:
    constexpr Maybe(const Maybe &) = default;
    constexpr Maybe(Maybe &&) = default;
    constexpr Maybe &operator=(const Maybe &) = default;
    constexpr Maybe &operator=(Maybe &&) = default;

    /**
     * @brief Constructs a `Maybe<T>` with a value (`Some<T>` case).
     * @sa @ref Some<T>
     * @since v1.0.0
     */
    constexpr Maybe(Some<T> val) noexcept
        : val(std::move(val).get())
        , has_Val(true)
    {
        if constexpr (std::is_class_v<T>) {
            static_assert(
                std::is_default_constructible_v<T>,
                "Maybe<class T> requires T to be default constructible.");
        }
    }

    /**
     * @brief Constructs a `Maybe<T>` with nothing (`None` case).
     * @sa @ref None
     * @since v1.0.0
     */
    constexpr Maybe(None) noexcept
        : has_Val(false)
    {
        if constexpr (std::is_class_v<T>) {
            static_assert(
                std::is_default_constructible_v<T>,
                "Maybe<class T> requires T to be default constructible.");
        }
    }

    /**
     * @return true if `Maybe<T>` contains a value (`Some<T>` case).
     * @return false if `Maybe<T>` contains nothing (`None` case).
     */
    constexpr bool has_Value() const { return this->has_Val; }
    /**
     * @return Gets a const reference (`T const&`) to the value owned by
     * `Maybe<T>`.
     *
     * @exception std::exception Thrown if `Maybe<T>` doesn't actually have a
     * value.
     * @sa Some<T>
     *
     * @since v1.0.0
     */
    constexpr T const &Some() const &
    {
        if (!has_Value())
            throw std::exception("Called Some() on a None value.");
        return this->some_unchecked();
    }
    /**
     * @return Gets a reference (`T&`) to the value owned by `Maybe<T>`.
     *
     * @exception std::exception Thrown if `Maybe<T>` doesn't actually have a
     * value.
     * @sa Some<T>
     *
     * @since v1.0.0
     */
    constexpr T &Some() &
    {
        if (!has_Value())
            throw std::exception("Called Some() on a None value.");
        return this->some_unchecked();
    }
    /**
     * @brief Creates a new object of type `T` by copying it from the one owned
     * by `Maybe<T>`
     * @remark `T` must be copy constructible.
     * @return A new object of type `T`.
     *
     * @exception std::exception Thrown if `Maybe<T>` doesn't actually have a
     * value.
     * @sa Some<T>
     *
     * @since v1.0.0
     */
    constexpr T CloneSome() const &
    {
        static_assert(std::is_copy_constructible_v<T>,
                      "CloneSome() requires T to be copyable.");

        if (!has_Value())
            throw std::exception("Called CloneSome() on a None value.");
        return this->some_unchecked();
    }
    /**
     * @return Moves the value out of `Maybe<T>`.
     *
     * @exception std::exception Thrown if `Maybe<T>` doesn't actually have a
     * value.
     * @sa Some<T>
     *
     * @since v1.0.0
     */
    constexpr T &&Unwrap()
    {
        if (!has_Value())
            throw std::exception("Called Unwrap() on a None value.");

        return std::move(*this).some_unchecked();
    }
};

/**
 * @attention This is a template specialization for `T&`. Since `Maybe<T&>` may
 * be constructed without a value, it cannot actually hold a referece
 * (references must be initialized always). Instead, it holds a pointer to `T`
 * (`T*`).
 * @brief `Maybe` is used to handle cases where a value of type `T&` might or
 * might not exist.
 *
 * @sa @ref None
 * @sa @ref Some<T>
 *
 * @since v1.0.0
 */
template<typename T>
class Maybe<T &>
{
    static_assert(!std::is_void_v<T>, "Maybe<void> is invalid.");

  private:
    T   *val;
    bool has_Val;

    /**
     * @return Gets the const reference (`T const&`) stored by `Maybe<T&>`
     * without checking if it's actually got a reference.
     * @since v1.0.0
     */
    constexpr T const &some_unchecked() const & { return *this->val; }
    /**
     * @return Gets the reference (`T &`) stored by `Maybe<T&>` without checking
     * if it's actually got a reference.
     * @since v1.0.0
     */
    constexpr T &some_unchecked() & { return *this->val; }
    /**
     * @return Moves the reference (`T &`) out of `Maybe<T&>` without checking
     * if it's actually got a reference.
     * @since v1.0.0
     */
    constexpr T &some_unchecked() && { return *this->val; }

  public:
    constexpr Maybe(const Maybe &) = default;
    constexpr Maybe(Maybe &&) = default;
    constexpr Maybe &operator=(const Maybe &) = default;
    constexpr Maybe &operator=(Maybe &&) = default;

    /**
     * @brief Constructs a `Maybe<T&>` with a reference (`Some<T&>` case).
     * @sa @ref Some<T&>
     * @since v1.0.0
     */
    constexpr Maybe(Some<T &> val) noexcept
        : val(val.val)
        , has_Val(true)
    {
    }

    /**
     * @brief Constructs a `Maybe<T&>` with nothing (`None` case).
     * @sa @ref Some<T&>
     * @since v1.0.0
     */
    constexpr Maybe(None) noexcept
        : has_Val(false)
    {
    }

    /**
     * @return true if `Maybe<T&>` contains a value (`Some<T&>` case).
     * @return false if `Maybe<T&>` contains nothing (`None` case).
     */
    constexpr bool has_Value() const { return this->has_Val; }
    /**
     * @return Gets the const reference (`T const&`) stored by `Maybe<T&>`.
     *
     * @exception std::exception Thrown if `Maybe<T&>` doesn't actually have a
     * reference.
     * @sa Some<T&>
     *
     * @since v1.0.0
     */
    constexpr T const &Some() const &
    {
        if (!has_Value())
            throw std::exception("Called Some() on a None value.");

        return this->some_unchecked();
    }
    /**
     * @return Gets the reference (`T&`) stored by `Maybe<T&>`.
     *
     * @exception std::exception Thrown if `Maybe<T&>` doesn't actually have a
     * reference.
     * @sa Some<T&>
     *
     * @since v1.0.0
     */
    constexpr T &Some() &
    {
        if (!has_Value())
            throw std::exception("Called Some() on a None value.");

        return this->some_unchecked();
    }
    /**
     * @return Moves the reference (`T&`) out of `Maybe<T&>`.
     *
     * @exception std::exception Thrown if `Maybe<T&>` doesn't actually have a
     * reference.
     * @sa Some<T&>
     *
     * @since v1.0.0
     */
    constexpr T &Unwrap()
    {
        if (!has_Value())
            throw std::exception("Called Unwrap() on a None value.");

        return std::move(*this).some_unchecked();
    }
};
}
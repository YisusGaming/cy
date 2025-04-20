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

/**
 * @brief `Ok<T>` owns a value of type `T`, representing success.
 *
 * @tparam T The type of value owned by `Ok<T>`.
 * @sa @ref Err<E>
 * @sa @ref Result<T, E>
 *
 * @since v1.0.0
 */
template<typename T>
class Ok
{
  private:
    T val;

  public:
    /**
     * @brief Constructs a new `Ok<T>` by copying `value`.
     *
     * @sa @ref Err<E>
     * @sa @ref Result<T, E>
     *
     * @since v1.0.0
     */
    explicit constexpr Ok(T const &value)
        : val(value)
    {
    }
    /**
     * @brief Constructs a new `Ok<T>` by moving `value`.
     *
     * @sa @ref Err<E>
     * @sa @ref Result<T, E>
     *
     * @since v1.0.0
     */
    explicit constexpr Ok(T &&value)
        : val(std::move(value))
    {
    }

    /**
     * @return Gets a const reference (`T const&`) to the value owned by
     * `Ok<T>`.
     * @since v1.0.0
     */
    constexpr T const &get() const & { return this->val; }
    /**
     * @return Gets a reference (`T&`) to the value owned by `Ok<T>`.
     * @since v1.0.0
     */
    constexpr T &get() & { return this->val; }
    /**
     * @return Moves the value out of `Ok<T>`.
     * @since v1.0.0
     */
    constexpr T &&get() && { return std::move(this->val); }
};

template<typename T, typename E>
class Result;

/**
 * @attention This is a template specialization for `T&`. `Ok<T&>` holds a
 * pointer to `T` in order to initialize `Result<T&, E>`, which requires a `T*`.
 * @brief Contains a reference to `T`.
 *
 * @tparam T The type of value.
 * @sa @ref Err<E>
 * @sa @ref Result<T&, E>
 *
 * @since v1.0.0
 */
template<typename T>
class Ok<T &>
{
  private:
    T *val;

  public:
    /**
     * @brief Initializes the reference in `Ok`.
     * @since v1.0.0
     */
    explicit constexpr Ok(T &value)
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
 * @attention This is a template specialization for when `Ok` is not required to
 * contain a value, it only represents a successful result.
 * @brief Initializes `Result<void, E>`.
 *
 * @sa @ref Err<E>
 * @sa @ref Result<T, E>
 *
 * @since v1.0.0
 */
template<>
class Ok<void>
{
  public:
    constexpr Ok() = default;
};
Ok() -> Ok<void>;

/**
 * @brief `Err<E>` owns an error of type `E` ('error' may be any kind of value,
 * really).
 *
 * @tparam T The error owned by `Err<E>`.
 * @sa @ref Ok<T>
 * @sa @ref Result<T, E>
 *
 * @since v1.0.0
 */
template<typename E>
class Err
{
    static_assert(!std::is_reference_v<E>, "Err<E> must own the error value.");

  private:
    E err;

  public:
    /**
     * @brief Constructs a new `Err<E>` by copying `error`.
     *
     * @sa @ref Ok<T>
     * @sa @ref Result<T, E>
     *
     * @since v1.0.0
     */
    explicit constexpr Err(E const &error)
        : err(error)
    {
    }
    /**
     * @brief Constructs a new `Err<E>` by moving `error`.
     *
     * @sa @ref Ok<T>
     * @sa @ref Result<T, E>
     *
     * @since v1.0.0
     */
    explicit constexpr Err(E &&error)
        : err(std::move(error))
    {
    }

    /**
     * @return Gets a const reference (`T const&`) to the value owned by
     * `Err<E>`.
     * @since v1.0.0
     */
    constexpr E const &get() const & { return this->err; }
    /**
     * @return Gets a reference (`T&`) to the value owned by `Err<E>`.
     * @since v1.0.0
     */
    constexpr E &get() & { return this->err; }
    /**
     * @return Moves the value out of `Err<E>`.
     * @since v1.0.0
     */
    constexpr E &&get() && { return std::move(this->err); }
};
Err(char const[]) -> Err<char const *>;

/**
 * @brief `Result<T, E>` is used to handle cases where an operation might
 * succeed (resulting in `Ok<T>`), or might fail with an error (resulting in
 * `Err<E>`).
 *
 * If constructed with `Ok<T>`, `Result<T, E>>` will now own the value of
 * type `T`. If constructed with `Err<E>`, `Result<T, E>>` will now own the
 * error of type `E`.
 *
 * @sa @ref Err<E>
 * @sa @ref Ok<T>
 *
 * @since v1.0.0
 */
template<typename T, typename E>
class [[nodiscard("Result must be handled.")]] Result
{
    static_assert(!std::is_reference_v<E>, "Err<E> must own the error value.");

  private:
    union
    {
        T val;
        E err;
    };
    bool is_Error;

    /**
     * @return Gets a const reference (`T const&`) to the Ok value owned by
     * `Result<T, E>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr T const &ok_unchecked() const & { return this->val; }
    /**
     * @return Gets a reference (`T&`) to the Ok value owned by
     * `Result<T, E>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr T &ok_unchecked() & { return this->val; }
    /**
     * @return Moves the Ok value out of `Result<T, E>` without checking if it's
     * actually got a value.
     * @since v1.0.0
     */
    constexpr T &&ok_unchecked() && { return std::move(this->val); }

    /**
     * @return Gets a const reference (`T const&`) to the Error value owned by
     * `Result<T, E>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr E const &err_unchecked() const & { return this->err; }
    /**
     * @return Gets a reference (`T&`) to the Error value owned by
     * `Result<T, E>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr E &err_unchecked() & { return this->err; }
    /**
     * @return Moves the Error value out of `Result<T, E>` without checking if
     * it's actually got a value.
     * @since v1.0.0
     */
    constexpr E &&err_unchecked() && { return std::move(this->err); }

  public:
    constexpr Result(const Result &) = default;
    constexpr Result(Result &&) = default;
    constexpr Result &operator=(const Result &) = default;
    constexpr Result &operator=(Result &&) = default;

    /**
     * @brief Constructs a `Result<T, E>` with an Ok value.
     * @sa @ref Ok<T>
     * @since v1.0.0
     */
    constexpr Result(Ok<T> value) noexcept
        : val(std::move(value).get())
        , is_Error(false)
    {
        if constexpr (std::is_class_v<T>) {
            static_assert(
                std::is_default_constructible_v<T>,
                "Result<class T, E> requires T to be default constructible.");
        }
        if constexpr (std::is_class_v<E>) {
            static_assert(
                std::is_default_constructible_v<E>,
                "Result<T, class E> requires E to be default constructible.");
        }
    }

    /**
     * @brief Constructs a `Result<T, E>` with an Error value.
     * @sa @ref Err<E>
     * @since v1.0.0
     */
    constexpr Result(Err<E> error) noexcept
        : err(std::move(error).get())
        , is_Error(true)
    {
        if constexpr (std::is_class_v<T>) {
            static_assert(
                std::is_default_constructible_v<T>,
                "Result<class T, E> requires T to be default constructible.");
        }
        if constexpr (std::is_class_v<E>) {
            static_assert(
                std::is_default_constructible_v<E>,
                "Result<T, class E> requires E to be default constructible.");
        }
    }

    /**
     * @return false if `Result<T, E>` contains a value (`Ok<T>` case).
     * @return true if `Result<T, E>` contains an error (`Err<E>` case).
     */
    constexpr bool is_Err() const { return this->is_Error; }
    /**
     * @return Gets a const reference (`T const&`) to the Ok value owned by
     * `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Ok value.
     * @sa Ok<T>
     *
     * @since v1.0.0
     */
    constexpr T const &Ok() const &
    {
        if (is_Err())
            throw std::exception("Called Ok() on an Error value.");
        return this->ok_unchecked();
    }
    /**
     * @return Gets a reference (`T &`) to the Ok value owned by
     * `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Ok value.
     * @sa Ok<T>
     *
     * @since v1.0.0
     */
    constexpr T &Ok() &
    {
        if (is_Err())
            throw std::exception("Called Ok() on an Error value.");
        return this->ok_unchecked();
    }
    /**
     * @brief Creates a new object of type `T` by copying it from the one owned
     * by `Result<T, E>`
     * @remark `T` must be copy constructible.
     * @return A new object of type `T`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Ok value.
     * @sa Ok<T>
     *
     * @since v1.0.0
     */
    constexpr T CloneOk() const &
    {
        static_assert(std::is_copy_constructible_v<T>,
                      "CloneOk() requires T to be copyable.");

        if (is_Err())
            throw std::exception("Called CloneOk() on an Error value.");
        return this->ok_unchecked();
    }
    /**
     * @return Moves the ok value out of `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Ok value.
     * @sa Ok<T>
     *
     * @since v1.0.0
     */
    constexpr T &&Unwrap()
    {
        if (is_Err())
            throw std::exception("Called Unwrap() on a Error value.");

        return std::move(*this).ok_unchecked();
    }

    /**
     * @return Gets a const reference (`E const&`) to the Error value owned by
     * `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Error value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E const &Err() const &
    {
        if (!is_Err())
            throw std::exception("Called Err() on an Ok value.");
        return this->err_unchecked();
    }
    /**
     * @return Gets a reference (`E &`) to the Error value owned by
     * `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Error value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E &Err() &
    {
        if (!is_Err())
            throw std::exception("Called Err() on an Ok value.");
        return this->err_unchecked();
    }
    /**
     * @brief Creates a new object of type `E` by copying it from the one owned
     * by `Result<T, E>`
     * @remark `E` must be copy constructible.
     * @return A new object of type `E`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Error value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E CloneErr() const &
    {
        static_assert(std::is_copy_constructible_v<E>,
                      "CloneErr() requires E to be copyable.");

        if (!is_Err())
            throw std::exception("Called CloneErr() on an Ok value.");
        return this->err_unchecked();
    }
    /**
     * @return Moves the Err value out of `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Err value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E &&UnwrapErr()
    {
        if (!is_Err())
            throw std::exception("Called UnwrapErr() on a Ok value.");

        return std::move(*this).err_unchecked();
    }
};

/**
 * @brief `Result<T&, E>` is used to handle cases where an operation might
 * succeed (resulting in `Ok<T&>`), or might fail with an error (resulting in
 * `Err<E>`).
 *
 * If constructed with `Err<E>`, `Result<T, E>>` will now own the error of type
 * `E`.
 *
 * @sa @ref Err<E>
 * @sa @ref Ok<T&>
 *
 * @since v1.0.0
 */
template<typename T, typename E>
class [[nodiscard("Result must be handled.")]] Result<T &, E>
{
    static_assert(!std::is_reference_v<E>, "Err<E> must own the error value.");

  private:
    union
    {
        T *val;
        E  err;
    };
    bool is_Error;

    /**
     * @return Gets the const reference (`T const&`) stored by `Result<T&, E>`
     * without checking if it's actually got a reference.
     * @since v1.0.0
     */
    constexpr T const &ok_unchecked() const & { return *this->val; }
    /**
     * @return Gets the reference (`T &`) stored by `Result<T&, E>` without
     * checking if it's actually got a reference.
     * @since v1.0.0
     */
    constexpr T &ok_unchecked() & { return *this->val; }
    /**
     * @return Moves the reference (`T &`) out of `Result<T&, E>` without
     * checking if it's actually got a reference.
     * @since v1.0.0
     */
    constexpr T &ok_unchecked() && { return *this->val; }

    /**
     * @return Gets a const reference (`T const&`) to the Error value owned by
     * `Result<T, E>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr E const &err_unchecked() const & { return this->err; }
    /**
     * @return Gets a reference (`T&`) to the Error value owned by
     * `Result<T, E>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr E &err_unchecked() & { return this->err; }
    /**
     * @return Moves the Error value out of `Result<T, E>` without checking if
     * it's actually got a value.
     * @since v1.0.0
     */
    constexpr E &&err_unchecked() && { return std::move(this->err); }

  public:
    constexpr Result(const Result &) = default;
    constexpr Result(Result &&) = default;
    constexpr Result &operator=(const Result &) = default;
    constexpr Result &operator=(Result &&) = default;

    /**
     * @brief Constructs a `Result<T&, E>` with an Ok value.
     * @sa @ref Ok<T&>
     * @since v1.0.0
     */
    constexpr Result(Ok<T &> value) noexcept
        : val(&value.get())
        , is_Error(false)
    {
        if constexpr (std::is_class_v<E>) {
            static_assert(
                std::is_default_constructible_v<E>,
                "Result<T, class E> requires E to be default constructible.");
        }
    }

    /**
     * @brief Constructs a `Result<T, E>` with an Error value.
     * @sa @ref Err<E>
     * @since v1.0.0
     */
    constexpr Result(Err<E> error) noexcept
        : err(std::move(error).get())
        , is_Error(true)
    {
        if constexpr (std::is_class_v<E>) {
            static_assert(
                std::is_default_constructible_v<E>,
                "Result<T, class E> requires E to be default constructible.");
        }
    }

    /**
     * @return false if `Result<T, E>` contains a value (`Ok<T>` case).
     * @return true if `Result<T, E>` contains an error (`Err<E>` case).
     */
    constexpr bool is_Err() const { return this->is_Error; }
    /**
     * @return Gets a const reference (`T const&`) to the Ok value owned by
     * `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Ok value.
     * @sa Ok<T>
     *
     * @since v1.0.0
     */
    constexpr T const &Ok() const &
    {
        if (is_Err())
            throw std::exception("Called Ok() on an Error value.");
        return this->ok_unchecked();
    }
    /**
     * @return Gets the reference (`T &`) in `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Ok value.
     * @sa Ok<T>
     *
     * @since v1.0.0
     */
    constexpr T &Ok() &
    {
        if (is_Err())
            throw std::exception("Called Ok() on an Error value.");
        return this->ok_unchecked();
    }
    /**
     * @return Moves the reference out of `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Ok value.
     * @sa Ok<T>
     *
     * @since v1.0.0
     */
    constexpr T &Unwrap()
    {
        if (is_Err())
            throw std::exception("Called Unwrap() on a Error value.");

        return std::move(*this).ok_unchecked();
    }

    /**
     * @return Gets a const reference (`E const&`) to the Error value owned by
     * `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Error value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E const &Err() const &
    {
        if (!is_Err())
            throw std::exception("Called Err() on an Ok value.");
        return this->err_unchecked();
    }
    /**
     * @return Gets a reference (`E &`) to the Error value owned by
     * `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Error value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E &Err() &
    {
        if (!is_Err())
            throw std::exception("Called Err() on an Ok value.");
        return this->err_unchecked();
    }
    /**
     * @brief Creates a new object of type `E` by copying it from the one owned
     * by `Result<T, E>`
     * @remark `E` must be copy constructible.
     * @return A new object of type `E`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Error value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E CloneErr() const &
    {
        static_assert(std::is_copy_constructible_v<E>,
                      "CloneErr() requires E to be copyable.");

        if (!is_Err())
            throw std::exception("Called CloneErr() on an Ok value.");
        return this->err_unchecked();
    }
    /**
     * @return Moves the Err value out of `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Err value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E &&UnwrapErr()
    {
        if (!is_Err())
            throw std::exception("Called UnwrapErr() on a Ok value.");

        return std::move(*this).err_unchecked();
    }
};

/**
 * @brief `Result<void, E>` is used to handle cases where an operation might
 * succeed (resulting in `Ok<void>`), or might fail with an error (resulting in
 * `Err<E>`).
 *
 * If constructed with `Err<E>`, `Result<T, E>>` will now own the error of type
 * `E`.
 *
 * @sa @ref Err<E>
 * @sa @ref Ok<void>
 *
 * @since v1.0.0
 */
template<typename E>
class [[nodiscard("Result must be handled.")]] Result<void, E>
{
    static_assert(!std::is_reference_v<E>, "Err<E> must own the error value.");

  private:
    E    err;
    bool is_Error;

    /**
     * @return Gets a const reference (`T const&`) to the Error value owned by
     * `Result<T, E>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr E const &err_unchecked() const & { return this->err; }
    /**
     * @return Gets a reference (`T&`) to the Error value owned by
     * `Result<T, E>` without checking if it's actually got a value.
     * @since v1.0.0
     */
    constexpr E &err_unchecked() & { return this->err; }
    /**
     * @return Moves the Error value out of `Result<T, E>` without checking if
     * it's actually got a value.
     * @since v1.0.0
     */
    constexpr E &&err_unchecked() && { return std::move(this->err); }

  public:
    constexpr Result(const Result &) = default;
    constexpr Result(Result &&) = default;
    constexpr Result &operator=(const Result &) = default;
    constexpr Result &operator=(Result &&) = default;

    /**
     * @brief Constructs a `Result<T&, E>` with an Ok value.
     * @sa @ref Ok<T&>
     * @since v1.0.0
     */
    constexpr Result(Ok<void>) noexcept
        : is_Error(false)
    {
        if constexpr (std::is_class_v<E>) {
            static_assert(
                std::is_default_constructible_v<E>,
                "Result<T, class E> requires E to be default constructible.");
        }
    }

    /**
     * @brief Constructs a `Result<T, E>` with an Error value.
     * @sa @ref Err<E>
     * @since v1.0.0
     */
    constexpr Result(Err<E> error) noexcept
        : err(std::move(error).get())
        , is_Error(true)
    {
        if constexpr (std::is_class_v<E>) {
            static_assert(
                std::is_default_constructible_v<E>,
                "Result<T, class E> requires E to be default constructible.");
        }
    }

    /**
     * @return false if `Result<T, E>` contains a value (`Ok<T>` case).
     * @return true if `Result<T, E>` contains an error (`Err<E>` case).
     */
    constexpr bool is_Err() const { return this->is_Error; }

    /**
     * @return Gets a const reference (`E const&`) to the Error value owned by
     * `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Error value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E const &Err() const &
    {
        if (!is_Err())
            throw std::exception("Called Err() on an Ok value.");
        return this->err_unchecked();
    }
    /**
     * @return Gets a reference (`E &`) to the Error value owned by
     * `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Error value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E &Err() &
    {
        if (!is_Err())
            throw std::exception("Called Err() on an Ok value.");
        return this->err_unchecked();
    }
    /**
     * @brief Creates a new object of type `E` by copying it from the one owned
     * by `Result<T, E>`
     * @remark `E` must be copy constructible.
     * @return A new object of type `E`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Error value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E CloneErr() const &
    {
        static_assert(std::is_copy_constructible_v<E>,
                      "CloneErr() requires E to be copyable.");

        if (!is_Err())
            throw std::exception("Called CloneErr() on an Ok value.");
        return this->err_unchecked();
    }
    /**
     * @return Moves the Err value out of `Result<T, E>`.
     *
     * @exception std::exception Thrown if `Result<T, E>` doesn't actually have
     * an Err value.
     * @sa Err<E>
     *
     * @since v1.0.0
     */
    constexpr E &&UnwrapErr()
    {
        if (!is_Err())
            throw std::exception("Called UnwrapErr() on a Ok value.");

        return std::move(*this).err_unchecked();
    }
};
}
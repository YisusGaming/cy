/**
 * @file types.hpp
 * @author Jesús Blanco
 * @brief The custom typenames for CY.
 * @version 1.0.1
 * @date 2025-04-19
 *
 * @copyright Copyright (c) Jesús Blanco. See LICENSE for details.
 *
 */

#pragma once

#include <stdint.h>

#define fnptr(fn, ...) (*fn)(__VA_ARGS__)

/// @brief A read-only string (char const*) type.
typedef char const *str;
/// @brief A modifiable string (char *) type.
typedef char *ncstr;

#if !defined(CY_SHORT_TYPENAMES)
/// @brief The 8-bit unsigned integer type.
typedef uint8_t uint8;
/// @brief The 16-bit unsigned integer type.
typedef uint16_t uint16;
/// @brief The 32-bit unsigned integer type.
typedef uint32_t uint32;
/// @brief The 64-bit unsigned integer type.
typedef uint64_t uint64;

/// @brief The 8-bit signed integer type.
typedef int8_t int8;
/// @brief The 16-bit signed integer type.
typedef int16_t int16;
/// @brief The 32-bit signed integer type.
typedef int32_t int32;
/// @brief The 64-bit signed integer type.
typedef int64_t int64;

/// @brief The 32-bit floating-point type.
typedef float float32;
/// @brief The 64-bit floating-point type.
typedef double float64;

/// @brief The 32-bit unsigned size type.
typedef size_t usize;
#else
/// @brief The 8-bit unsigned integer type.
typedef uint8_t u8;
/// @brief The 16-bit unsigned integer type.
typedef uint16_t u16;
/// @brief The 32-bit unsigned integer type.
typedef uint32_t u32;
/// @brief The 64-bit unsigned integer type.
typedef uint64_t u64;

/// @brief The 8-bit signed integer type.
typedef int8_t i8;
/// @brief The 16-bit signed integer type.
typedef int16_t i16;
/// @brief The 32-bit signed integer type.
typedef int32_t i32;
/// @brief The 64-bit signed integer type.
typedef int64_t i64;

/// @brief The 32-bit floating-point type.
typedef float f32;
/// @brief The 64-bit floating-point type.
typedef double f64;

/// @brief The 32-bit unsigned size type.
typedef size_t usz;
#endif
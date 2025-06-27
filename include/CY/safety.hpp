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
#include <exception>
#include <type_traits>
namespace cy {
template<typename T>
class Maybe;
}
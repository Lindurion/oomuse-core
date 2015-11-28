/**
 * Copyright 2015 Eric W. Barndollar. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * =============================================================================
 * This file provides sized int types like int32 and uint64, which are just
 * shorter to type aliases for int32_t, uint64_t, etc.
 */

#ifndef OOMUSE_CORE_INT_TYPES_H
#define OOMUSE_CORE_INT_TYPES_H

#include <cstdint>


/**
 * If using oomuse libraries with a project that already defines int32, uint64,
 * etc., just include them and define OOMUSE_SKIP_INT_TYPES before including any
 * oomuse headers.
 */
#ifndef OOMUSE_SKIP_INT_TYPES

/** 8-bit unsigned integer. */
using uint8 = uint8_t;

/** 16-bit unsigned integer. */
using uint16 = uint16_t;

/** 32-bit unsigned integer. */
using uint32 = uint32_t;

/** 64-bit unsigned integer. */
using uint64 = uint64_t;

/** 8-bit signed integer. */
using int8 = int8_t;

/** 16-bit signed integer. */
using int16 = int16_t;

/** 32-bit signed integer. */
using int32 = int32_t;

/** 64-bit signed integer. */
using int64 = int64_t;

#endif  // OOMUSE_SKIP_INT_TYPES


// Verify type sizes. If something fails here, then either <cstdint> has
// problems or OOMUSE_SKIP_INT_TYPES is being used and these types weren't
// defined correctly.
static_assert(sizeof(uint8) == 1, "uint8 isn't 8 bits");
static_assert(sizeof(uint16) == 2, "uint16 isn't 16 bits");
static_assert(sizeof(uint32) == 4, "uint32 isn't 32 bits");
static_assert(sizeof(uint64) == 8, "uint64 isn't 64 bits");
static_assert(sizeof(int8) == 1, "int8 isn't 8 bits");
static_assert(sizeof(int16) == 2, "int16 isn't 16 bits");
static_assert(sizeof(int32) == 4, "int32 isn't 32 bits");
static_assert(sizeof(int64) == 8, "int64 isn't 64 bits");

// Also check float & double sizes, since they are assumed to be 32 & 64 bit.
static_assert(sizeof(float) == 4, "float isn't 32 bits");
static_assert(sizeof(double) == 8, "double isn't 64 bits");


#endif  // OOMUSE_CORE_INT_TYPES_H

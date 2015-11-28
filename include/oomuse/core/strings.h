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
 * Simple string utilities.
 */

#ifndef OOMUSE_CORE_STRINGS_H
#define OOMUSE_CORE_STRINGS_H

#include <locale>
#include <string>

namespace oomuse {
namespace strings {


/** Returns true if str is empty or all whitespace, by std::isspace(). */
bool isEmptyOrWhitespace(const std::string& str);

/**
 * Returns true if str is empty or all whitespace, by std::isspace() for the
 * given locale.
 */
bool isEmptyOrWhitespace(const std::string& str, const std::locale& loc);

/** Returns true if str starts with prefix. */
bool startsWith(const std::string& str, const std::string& prefix);

/** Returns true if str ends with suffix. */
bool endsWith(const std::string& str, const std::string& suffix);

/** Returns new string with whitespace, by std::isspace(), trimmed from ends. */
std::string trimWhitespace(const std::string& str);

/**
 * Returns new string with whitespace trimmed from both ends, by
 * std::isspace() for the given locale.
 */
std::string trimWhitespace(const std::string& str, const std::locale& loc);

/** Returns a new string with all characters converted via std::tolower(). */
std::string toLowerCase(const std::string& str);

/**
 * Returns a new string with all characters converted via std::tolower() for
 * the given locale.
 */
std::string toLowerCase(const std::string& str, const std::locale& loc);

/** Returns a new string with all characters converted via std::toupper(). */
std::string toUpperCase(const std::string& str);

/**
 * Returns a new string with all characters converted via std::toupper() for
 * the given locale.
 */
std::string toUpperCase(const std::string& str, const std::locale& loc);


}  // namespace strings
}  // namespace oomuse

#endif  // OOMUSE_CORE_STRINGS_H

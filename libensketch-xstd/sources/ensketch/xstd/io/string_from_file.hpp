// Copyright © 2024 Markus Pawellek
//
// This file is part of `xstd`.
//
// `xstd` is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// `xstd` is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with `xstd`. If not, see <https://www.gnu.org/licenses/>.
//
#pragma once
#include <ensketch/xstd/io/defaults.hpp>

namespace ensketch::xstd {

/// Return the content of a file given by its path as a standard `string` object.
///
inline auto string_from_file(const std::filesystem::path& path) -> string {
  using namespace std;

  // We will read all characters as block and open the file in binary mode.
  // Make sure to jump to its end for directly reading its size.
  ifstream file{path, ios::binary | ios::ate};
  if (!file)
    throw runtime_error(format("Failed to open file '{}'.", path.string()));

  // Read the file's size.
  auto size = file.tellg();

  // Prepare the result string with a sufficiently large buffer.
  string result(size, '\0');

  // Go back to the start and read all characters at once in a block.
  file.seekg(0);
  file.read(result.data(), size);

  return result;
};

}  // namespace ensketch::xstd

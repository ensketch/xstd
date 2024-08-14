#pragma once
#include <ensketch/xstd/utility.hpp>

namespace ensketch::xstd {

/// Returns the content of a file given by its path as a standard `string` object.
///
inline auto string_from_file(const std::filesystem::path& path) -> string {
  using namespace std;

  // We will read all characters as block and open the file in binary mode.
  // Make sure to jump to its end for directly reading its size.
  //
  ifstream file{path, ios::binary | ios::ate};
  if (!file)
    throw runtime_error(format("Failed to open file '{}'.", path.string()));

  // Read the file's size.
  //
  auto size = file.tellg();

  // Prepare the result string with a sufficiently large buffer.
  //
  string result(size, '\0');

  // Go back to the start and read all characters at once in a block.
  //
  file.seekg(0);
  file.read(result.data(), size);

  return result;
};

}  // namespace ensketch::xstd

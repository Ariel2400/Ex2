#include <fstream>
#include <iterator>
#include <string>

/**
 * @brief Read the content of a file at path filePath.
 *
 * @param filePath The relative or absolute path to the file.
 * @return std::string File's content
 */
std::string readFileContent(const std::string& filePath) {
  // Opens input-only file (ifstream) in binary mode.
  std::ifstream in(filePath, std::ios::binary);

  // The file is in a bad state. The error can be retrieved
  //	using the global `errno` in linux (#include <cerrno>).
  if (!in) {
    // Error here...
  }

  // Read the file to a vector. This is not the most effecient
  //	method to read a file.
  auto content = std::string{std::istreambuf_iterator<char>{in},
                             std::istreambuf_iterator<char>{}};

  // After reading the file, it should meet EOF (end of file). If
  //  it did not, it means that an error occurred.
  if (!in.eof()) {
    // Unlikly to happen error here...
  }

  return content;
}

/**
 * @brief Writes content to a file. If the file exists, removes
 *  previos content. Otherwise, creates a new file.
 *
 * @param filePath The relative or absolute path to the file.
 * @param content Content to write.
 */
void writeFileContent(const std::string& filePath, const std::string& content) {
  // Opens output-only file (ofstream) in binary mode, and truncates all
  //    existing content from the file.
  std::ofstream out(filePath, std::ios::binary | std::ios::trunc);

  // The file is in a bad state.
  if (!out) {
    // Error here...
  }

  out.write(content.data(), static_cast<std::streamsize>(content.length()));
  if (!out) {
    // Unlikly to happen error here...
  }
}

#include <string>

namespace testing {
namespace bmp {
/**
 * @brief Receives a path to a BMP image on the disk and rotates it 90 degrees clockwise,
 *  writing the resulting image to the disk.
 *
 * @param imagePath Path of a BMP image that should be rotated. The path might be invalid
 *  or contain a non-BMP file.
 * @param outputPath Path of the resulting BMP image. can be assumed to 
 *  reside in a valid directory, however, cannot be assumed to exist. If the
 *  path already exists, the file previously existing in the path should be
 *  overriden.
 */
void rotate_image(const std::string& imagePath, const std::string& outputPath);

/**
 * @brief Receives a path to a BMP image on the disk and converts it to grayscale,
 *  writing the resulting image to the disk.
 * 
 * @param imagePath Path of a BMP image that should be rotated.
 * @param outputPath Path of the resulting BMP image.
 */
void convert_to_grayscale(const std::string& imagePath,
                        const std::string& outputPath);
}
}
#pragma once

#include <functional>
#include <string>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

namespace CesiumCpp {

namespace Common {
/**
 * @brief Sleeps for the given number of milliseconds.
 *
 * This will just block the calling thread for the given number of
 * milliseconds, and print a log message.
 *
 * If the given number is not positive, then this call will not block.
 *
 * @param ms The number of milliseconds
 */
void sleepMsLogged(int32_t ms);

/**
 * @brief Execute the given function, printing some timing information.
 *
 * @param name The name to be printed for the times function
 * @param function The function
 */
void runTimed(const std::string &name, std::function<void()> function);

/**
 * @brief Read the specified file.
 *
 * Fully reads the specified file, and returns it as a vector
 * of bytes. If the file cannot be read, then an empty vector
 * is returned. Yes, this does not differentiate between the
 * file being empty and the filename being invalid, but this
 * is not a library that other people will RELY on, so I don't
 * care right now.
 *
 * @param fileName The file name
 * @return The read file data.
 */
std::vector<std::byte> readFile(const std::string &fileName);

/**
 * @brief Writes the given data to the specified file.
 *
 * This will blatantly overwrite any existing file. Be careful.
 *
 * @param fileName The file name
 * @param data The data
 */
void writeFile(const std::string &fileName, const std::vector<std::byte> &data);

} // namespace Common
} // namespace CesiumCpp

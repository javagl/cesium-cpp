
#include "Common/Common.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <cstddef>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace CesiumCpp {

namespace Common {

void runTimed(const std::string &name, std::function<void()> function) {
  auto start = std::chrono::high_resolution_clock::now();
  function();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
  double ms = duration.count() / 1e6;
  SPDLOG_INFO("{0}: {1} ms", name, ms);
}

void sleepMsLogged(const int32_t ms) {

  if (ms > 0) {
    SPDLOG_TRACE("Sleeping for {0}ms", ms);
    std::chrono::milliseconds duration{std::chrono::milliseconds(ms)};
    std::this_thread::sleep_for(duration);
    SPDLOG_TRACE("Sleeping for {0}ms DONE", duration.count());
  }
}

std::vector<std::byte> readFile(const std::string &fileName) {
  std::ifstream file(fileName, std::ios::binary | std::ios::ate);
  if (!file) {
    return std::vector<std::byte>();
  }
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<std::byte> buffer(static_cast<size_t>(size));
  file.read(reinterpret_cast<char *>(buffer.data()), size);
  return buffer;
}

void writeFile(const std::string &fileName,
               const std::vector<std::byte> &data) {
  std::ofstream file(fileName, std::ios::binary | std::ios::ate);
  file.write(reinterpret_cast<const char *>(data.data()), data.size());
}

} // namespace Common

} // namespace CesiumCpp

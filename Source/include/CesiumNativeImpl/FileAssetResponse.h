#pragma once

#include <CesiumAsync/IAssetResponse.h>

#include <string>
#include <vector>

namespace CesiumCpp {

namespace CesiumNativeImpl {
/**
 * @brief Implementation of an IAssetResponse for data from a file.
 */
class FileAssetResponse : public CesiumAsync::IAssetResponse {
public:
  /**
   * @brief Creates a new instance.
   *
   * @param fileName The file name.
   * @param statusCode The HTTP status code
   * @param contentType The content type.
   * @param fileData The file data.
   */
  FileAssetResponse(const std::string &fileName, uint16_t statusCode,
                    const std::string &contentType,
                    const CesiumAsync::HttpHeaders &headers,
                    const std::vector<std::byte> &fileData);

  uint16_t statusCode() const noexcept override;

  virtual const CesiumAsync::HttpHeaders &headers() const override {
    return this->_headers;
  }

  std::string contentType() const noexcept override;

  std::span<const std::byte> data() const noexcept override;

private:
  std::string _fileName;
  uint16_t _statusCode;
  std::string _contentType;
  CesiumAsync::HttpHeaders _headers;

  std::vector<std::byte> _fileData;
};
} // namespace CesiumNativeImpl
} // namespace CesiumCpp

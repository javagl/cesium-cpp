#include "FileAssetResponse.h"

#include "Cesium3DTilesSelection/spdlog-cesium.h"

#include <gsl/span>

#include <string>
#include <vector>

namespace Cesium3DTilesTests {
FileAssetResponse::FileAssetResponse(
    const std::string& fileName,
    uint16_t statusCode,
    const std::string& contentType,
    const CesiumAsync::HttpHeaders& headers,
    const std::vector<std::byte>& fileData)
    : _fileName(fileName),
      _statusCode(statusCode),
      _contentType(contentType),
      _headers(headers),
      _fileData(fileData) // TODO: Yeah, will be copied. However...
{
  SPDLOG_TRACE(
      "Created FileAssetResponse with status {} for {} data from {}",
      statusCode,
      contentType,
      fileName);
}

uint16_t FileAssetResponse::statusCode() const noexcept { return _statusCode; }

std::string FileAssetResponse::contentType() const noexcept {
  return _contentType;
}

gsl::span<const std::byte> FileAssetResponse::data() const noexcept {
  return gsl::span<const std::byte>(_fileData.data(), _fileData.size());
}

} // namespace Cesium3DTilesTests
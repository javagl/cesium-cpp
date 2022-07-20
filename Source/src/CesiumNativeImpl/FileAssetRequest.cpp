#include "CesiumNativeImpl/FileAssetRequest.h"

#include "CesiumNativeImpl/FileAssetResponse.h"
#include "Common/Common.h"

#include <CesiumAsync/IAssetAccessor.h>
#include <CesiumAsync/IAssetRequest.h>

#include <spdlog/spdlog.h>

#include <filesystem>
#include <functional>
#include <iomanip>
#include <string>

namespace CesiumCpp {
namespace CesiumNativeImpl {

FileAssetRequest::FileAssetRequest(
    const std::string &method, const std::string &url,
    const CesiumAsync::HttpHeaders &headers) noexcept
    : _method(method), _url(url), _headers(headers), _pResponse(nullptr) {
  SPDLOG_TRACE("Created FileAssetRequest with {0}", url);

  uint16_t statusCode = 200;
  std::vector<std::byte> contents = CesiumCpp::Common::readFile(url);

  // TODO: The content type is ONLY determined by the file extension here!
  // This may be sufficient for now, but in the future, it may have to
  // examine the contents.
  std::string actualExtension = std::filesystem::path(url).extension().string();
  std::vector<std::string> extensions{".json", ".b3dm", ".cmpt", ".glTF", ".glb", ".subtree"};
  for (const std::string &extension : extensions) {
    if (actualExtension == extension) {
      std::string contentType = extension;
      _pResponse = std::make_unique<FileAssetResponse>(
          url, statusCode, contentType, headers, contents);
    }
  }
  if (!_pResponse) {
    SPDLOG_ERROR("Unkonwn content type for {0}", url);
  }
}

const std::string &FileAssetRequest::url() const noexcept { return this->_url; }

CesiumAsync::IAssetResponse *FileAssetRequest::response() const noexcept {
  return this->_pResponse.get();
}

} // namespace CesiumNativeImpl
} // namespace CesiumCpp

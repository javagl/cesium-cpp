
#include "CesiumNativeImpl/FileAssetAccessor.h"
#include "CesiumNativeImpl/FileAssetRequest.h"

#include <CesiumAsync/AsyncSystem.h>
#include <CesiumAsync/IAssetAccessor.h>
#include <CesiumAsync/IAssetRequest.h>

#include <memory>

namespace CesiumCpp {
namespace CesiumNativeImpl {

CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>>
FileAssetAccessor::get(const CesiumAsync::AsyncSystem &asyncSystem,
                       const std::string &url,
                       const std::vector<THeader> &headers) {
  std::shared_ptr<CesiumAsync::IAssetRequest> value =
      std::make_shared<FileAssetRequest>("GET", url,
                                         CesiumAsync::HttpHeaders{});
  CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> result =
      asyncSystem.createResolvedFuture(
          std::shared_ptr<CesiumAsync::IAssetRequest>(value));
  return result;
}

CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>>
FileAssetAccessor::request(
    const CesiumAsync::AsyncSystem &asyncSystem, const std::string &verb,
    const std::string &url,
    const std::vector<THeader> &headers,
    const std::span<const std::byte> &contentPayload) {
  std::shared_ptr<CesiumAsync::IAssetRequest> value =
      std::make_shared<FileAssetRequest>(verb, url, CesiumAsync::HttpHeaders{});
  CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> result =
      asyncSystem.createResolvedFuture(
          std::shared_ptr<CesiumAsync::IAssetRequest>(value));
  return result;
}

void FileAssetAccessor::tick() noexcept {
  // Does nothing
}
} // namespace CesiumNativeImpl
} // namespace CesiumCpp

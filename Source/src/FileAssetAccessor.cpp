
#include "FileAssetAccessor.h"
#include "FileAssetRequest.h"

#include "CesiumAsync/IAssetAccessor.h"
#include "CesiumAsync/IAssetRequest.h"

#include <memory>

namespace Cesium3DTilesTests {
CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>>
FileAssetAccessor::requestAsset(
    const CesiumAsync::AsyncSystem& asyncSystem,
    const std::string& url,
    const std::vector<CesiumAsync::IAssetAccessor::THeader>& /*headers*/
) {
  std::shared_ptr<CesiumAsync::IAssetRequest> value =
      std::make_shared<FileAssetRequest>(
          "GET",
          url,
          CesiumAsync::HttpHeaders{});
  CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> result =
      asyncSystem.createResolvedFuture(
          std::shared_ptr<CesiumAsync::IAssetRequest>(value));
  return result;
}

CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>>
FileAssetAccessor::post(
    const CesiumAsync::AsyncSystem& asyncSystem,
    const std::string& url,
    const std::vector<THeader>& headers,
    const gsl::span<const std::byte>& /*contentPayload*/) {
  return this->requestAsset(asyncSystem, url, headers);
};

void FileAssetAccessor::tick() noexcept {
  // Does nothing
}
} // namespace Cesium3DTilesTests

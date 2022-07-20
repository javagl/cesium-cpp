#pragma once

#include <CesiumAsync/IAssetAccessor.h>
#include <CesiumAsync/IAssetRequest.h>

#include <memory>

namespace CesiumCpp {

namespace CesiumNativeImpl {
/**
 * @brief Implementation of an IAssetAccessor that accesses files
 */
class FileAssetAccessor : public CesiumAsync::IAssetAccessor {
public:
  CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>>
  get(const CesiumAsync::AsyncSystem &asyncSystem, const std::string &url,
      const std::vector<THeader> &headers = {}) override;

  virtual CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>>
  request(const CesiumAsync::AsyncSystem &asyncSystem, const std::string &verb,
          const std::string &url,
          const std::vector<THeader> &headers = std::vector<THeader>(),
          const gsl::span<const std::byte> &contentPayload = {}) override;

  void tick() noexcept override;
};

} // namespace CesiumNativeImpl
} // namespace CesiumCpp

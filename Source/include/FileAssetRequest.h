#pragma once

#include "CesiumAsync/IAssetAccessor.h"
#include "CesiumAsync/IAssetRequest.h"

#include <functional>
#include <string>
#include <vector>

namespace Cesium3DTilesTests {
/**
 * @brief Implementation of an IAssetRequest that is backed by a file
 */
class FileAssetRequest : public CesiumAsync::IAssetRequest {
public:
  /**
   * @brief Creates a new instance
   *
   * @param url The URL (file path)
   * @param headers The headers
   */
  FileAssetRequest(
      const std::string& method,
      const std::string& url,
      const CesiumAsync::HttpHeaders& headers) noexcept;

  virtual const std::string& method() const override { return this->_method; }
  virtual const CesiumAsync::HttpHeaders& headers() const override {
    return this->_headers;
  }

  const std::string& url() const noexcept override;

  CesiumAsync::IAssetResponse* response() const noexcept override;

private:
  std::string _method;
  std::string _url;
  CesiumAsync::HttpHeaders _headers;
  std::unique_ptr<CesiumAsync::IAssetResponse> _pResponse;
};
} // namespace Cesium3DTilesTests
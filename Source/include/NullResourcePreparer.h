#pragma once

#include "Cesium3DTilesSelection/IPrepareRendererResources.h"
#include "Cesium3DTilesSelection/spdlog-cesium.h"

#include "Cesium3DTilesSelection/RasterOverlayTile.h"
#include "Cesium3DTilesSelection/Tile.h"
#include "CesiumGeometry/Rectangle.h"

#include <glm/glm.hpp>

#include <cstdint>

namespace Cesium3DTilesTests {
/**
 * @brief Implementation of IPrepareRendererResources that does nothing.
 *
 * Except for logging.
 */
class NullResourcePreparer : public Cesium3DTilesSelection::IPrepareRendererResources {
public:
  NullResourcePreparer();

  void* prepareInLoadThread(
      const CesiumGltf::Model& model,
      const glm::dmat4& transform) override;

  void* prepareInMainThread(Cesium3DTilesSelection::Tile& tile, void* pLoadThreadResult)
      override;

  void free(
      Cesium3DTilesSelection::Tile& tile,
      void* pLoadThreadResult,
      void* pMainThreadResult) noexcept override;

  void*
  prepareRasterInLoadThread(const CesiumGltf::ImageCesium& image) override;

  void* prepareRasterInMainThread(
      const Cesium3DTilesSelection::RasterOverlayTile& rasterTile,
      void* pLoadThreadResult) override;

  void freeRaster(
      const Cesium3DTilesSelection::RasterOverlayTile& rasterTile,
      void* pLoadThreadResult,
      void* pMainThreadResult) noexcept override;

  void attachRasterInMainThread(
      const Cesium3DTilesSelection::Tile& tile,
      uint32_t overlayTextureCoordinateID,
      const Cesium3DTilesSelection::RasterOverlayTile& rasterTile,
      void* pMainThreadRendererResources,
      const CesiumGeometry::Rectangle& textureCoordinateRectangle,
      const glm::dvec2& translation,
      const glm::dvec2& scale) override;

  void detachRasterInMainThread(
      const Cesium3DTilesSelection::Tile& tile,
      uint32_t overlayTextureCoordinateID,
      const Cesium3DTilesSelection::RasterOverlayTile& rasterTile,
      void* pMainThreadRendererResources,
      const CesiumGeometry::Rectangle& textureCoordinateRectangle) noexcept
      override;
};
} // namespace Cesium3DTilesTests
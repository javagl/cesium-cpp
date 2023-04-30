#pragma once

#include <Cesium3DTilesSelection/IPrepareRendererResources.h>
#include <CesiumAsync/AsyncSystem.h>

#include <glm/glm.hpp>

class Cesium3DTilesSelection::Tile;
class Cesium3DTilesSelection::RasterOverlayTile;
class CesiumGltf::ImageCesium;
class CesiumGltf::Model;
class CesiumGeometry::Rectangle;

namespace CesiumCpp {
namespace CesiumNativeImpl {

/**
 * @brief Implementation of IPrepareRendererResources that does nothing.
 *
 * Except for logging.
 */
class NullResourcePreparer
    : public Cesium3DTilesSelection::IPrepareRendererResources {
public:
  NullResourcePreparer();

  CesiumAsync::Future<Cesium3DTilesSelection::TileLoadResultAndRenderResources>
  prepareInLoadThread(const CesiumAsync::AsyncSystem &asyncSystem,
                      Cesium3DTilesSelection::TileLoadResult &&tileLoadResult,
                      const glm::dmat4 &transform,
                      const std::any &rendererOptions) override;

  void *prepareInMainThread(Cesium3DTilesSelection::Tile &tile,
                            void *pLoadThreadResult) override;

  void free(Cesium3DTilesSelection::Tile &tile, void *pLoadThreadResult,
            void *pMainThreadResult) noexcept override;

  void *prepareRasterInLoadThread(CesiumGltf::ImageCesium &image,
                                  const std::any &rendererOptions) override;

  void *prepareRasterInMainThread(
      Cesium3DTilesSelection::RasterOverlayTile &rasterTile,
      void *pLoadThreadResult) override;

  void freeRaster(const Cesium3DTilesSelection::RasterOverlayTile &rasterTile,
                  void *pLoadThreadResult,
                  void *pMainThreadResult) noexcept override;

  void attachRasterInMainThread(
      const Cesium3DTilesSelection::Tile &tile,
      int32_t overlayTextureCoordinateID,
      const Cesium3DTilesSelection::RasterOverlayTile &rasterTile,
      void *pMainThreadRendererResources, const glm::dvec2 &translation,
      const glm::dvec2 &scale) override;

  void detachRasterInMainThread(
      const Cesium3DTilesSelection::Tile &tile,
      int32_t overlayTextureCoordinateID,
      const Cesium3DTilesSelection::RasterOverlayTile &rasterTile,
      void *pMainThreadRendererResources) noexcept override;
};
} // namespace CesiumNativeImpl
} // namespace CesiumCpp

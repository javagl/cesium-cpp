#pragma once

#include "Cesium3DTilesSelection/IPrepareRendererResources.h"

#include <glm/glm.hpp>

class Cesium3DTilesSelection::Tile;
class Cesium3DTilesSelection::RasterOverlayTile;
class CesiumGltf::ImageCesium;
class CesiumGltf::Model;
class CesiumGeometry::Rectangle;

namespace CesiumCpp {
  namespace CesiumNative {

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
        int32_t overlayTextureCoordinateID,
        const Cesium3DTilesSelection::RasterOverlayTile& rasterTile,
        void* pMainThreadRendererResources,
        const glm::dvec2& translation,
        const glm::dvec2& scale) override;

      void detachRasterInMainThread(
        const Cesium3DTilesSelection::Tile& tile,
        int32_t overlayTextureCoordinateID,
        const Cesium3DTilesSelection::RasterOverlayTile& rasterTile,
        void* pMainThreadRendererResources) noexcept
        override;
    };
  } // namespace CesiumNative
} // namespace CesiumCpp

#pragma once

#include <chrono>
#include <string>

#include <Cesium3DTilesSelection/TileID.h>
#include <Cesium3DTilesSelection/ViewState.h>
#include <Cesium3DTilesSelection/ViewUpdateResult.h>
#include <Cesium3DTilesSelection/TilesetExternals.h>

namespace CesiumCpp {

  namespace Utils {

    /**
     * @brief Creates a default TilesetExternals instance.
     * 
     * @return The instance
     */
    Cesium3DTilesSelection::TilesetExternals createDefaultExternals();

    /**
     * @brief Creates a default ViewState.
     *
     * The configuration of this ViewState is not specified.
     *
     * @param p The position
     * @param d The direction
     * @return The ViewState
     */
    Cesium3DTilesSelection::ViewState
      createViewState(const glm::dvec3& p, const glm::dvec3 d);

    /**
     * @brief Creates a default ViewState.
     *
     * The configuration of this ViewState is not specified.
     *
     * @return The ViewState
     */
    Cesium3DTilesSelection::ViewState createViewState();

    /**
     * @brief Returns a formatted string summary of the given ViewUpdateResult.
     *
     * The exact format is not specified. It should be easy to read, though...
     *
     * @param r The ViewUpdateResult
     * @return The string
     */
    std::string viewUpdateResultToString(const Cesium3DTilesSelection::ViewUpdateResult& r);

    /**
     * @brief Print a string representation of the given ViewUpdateResult
     *
     * As created with `viewUpdateResultToString`
     *
     * @param r The ViewUpdateResult
     */
    void printViewUpdateResult(const Cesium3DTilesSelection::ViewUpdateResult& r);

    /**
     * @brief Create a short string for the given TileID
     *
     * The exact format is not specified.
     *
     * @param tileId The tile ID
     * @return The string
     */
    std::string createTileIdString(const Cesium3DTilesSelection::TileID& tileId);
  } // namespace Utils
} // namespace CesiumCpp

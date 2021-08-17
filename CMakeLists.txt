cmake_minimum_required(VERSION 3.15)

project(
    cesium-cpp
    VERSION 0.0.1
    LANGUAGES CXX
)


#-----------------------------------------------------------------------------
# Just throw everything on a huge pile, because ... CMake

add_subdirectory(extern/cesium-native)



#-----------------------------------------------------------------------------
# Define the target executable 

set(TARGET cesium-cpp)

# Using GLOB, because it's easy, although a bad practice...
file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Source/src/*.cpp)
file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Source/include/*.h)
source_group("Source Files" FILES ${sources})
source_group("Header Files" FILES ${headers})
add_executable(${TARGET} ${sources} ${headers})

target_include_directories(${TARGET}
    PUBLIC
        $<INSTALL_INTERFACE:Source/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/src
)

target_link_libraries(${TARGET}
    PUBLIC
        Cesium3DTilesSelection
        CesiumAsync
        CesiumGeometry
        CesiumUtility
        CesiumAsync
        CesiumGltf
        CesiumGltfReader
        CesiumGltfWriter
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)


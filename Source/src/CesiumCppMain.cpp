
#include "Common/Common.h"
#include "GltfTests/GltfTests.h"
#include "TilesTests/TilesTests.h"
#include "Utils/Utils.h"

#include <iostream>
#include <string>

std::optional<std::string> findValue(int argc, char **argv,
                                     const std::string &option) {
  for (int i = 0; i < argc; i++) {
    std::string s(argv[i]);
    if (s == option) {
      if (i < argc - 1) {
        return std::string(argv[i + 1]);
      }
    }
  }
  return std::nullopt;
}

std::vector<std::string> tokenize(const std::string &s,
                                  const std::string &delimiter) {
  std::vector<std::string> tokens;
  size_t start = 0;
  size_t end = s.find(delimiter);
  while (end != std::string::npos) {
    std::string token = s.substr(start, end - start);
    tokens.push_back(token);
    start = end + delimiter.length();
    end = s.find(delimiter, start);
  }
  std::string token = s.substr(start);
  tokens.push_back(token);
  return tokens;
}

void printUsage() {
  std::cout << "Usage: " << std::endl;
  std::cout << "" << std::endl;

  std::cout << "- Reading a tileset and doing an unspecified test: "
            << std::endl;
  std::cout << "" << std::endl;
  std::cout << "  cesium-cpp -tileset C:/Example/tileset.json" << std::endl;
  std::cout << "" << std::endl;

  std::cout << "" << std::endl;

  std::cout << "- Reading all glTF sample models: " << std::endl;
  std::cout << "" << std::endl;
  std::cout << "  cesium-cpp -gltf C:/sampleModelsBaseDir/" << std::endl;
  std::cout << "" << std::endl;

  std::cout << "- Reading a specific glTF sample model: " << std::endl;
  std::cout << "" << std::endl;
  std::cout << "  cesium-cpp -gltf "
               "C:/sampleModelsBaseDir/"
               ",MorphStressTest,glTF-Binary,MorphStressTest.glb"
            << std::endl;
  std::cout << "" << std::endl;
}

int main(int argc, char **argv) {

  // std::string defaultTilesetUrl =
  //     "C:/Develop/CesiumUnreal/cesium-cpp/Data/Icospheres/tileset.json";
  // CesiumCpp::TilesTests::runBasicCesiumNativeExample(defaultTilesetUrl)

  std::optional<std::string> optionalTilesetUrl =
      findValue(argc, argv, "-tileset");
  if (optionalTilesetUrl.has_value()) {
    const std::string tilesetUrl = optionalTilesetUrl.value();
    CesiumCpp::TilesTests::runBasicCesiumNativeExample(tilesetUrl);
    return 0;
  }

  std::optional<std::string> optionalGltfInput = findValue(argc, argv, "-gltf");
  if (optionalGltfInput.has_value()) {
    const std::string gltfInput = optionalGltfInput.value();
    std::vector<std::string> tokens = tokenize(gltfInput, ",");
    if (tokens.size() == 1) {
      std::string baseUrl = tokens[0];
      CesiumCpp::GltfTests::testReadSampleModels(baseUrl);
      return 0;
    }
    if (tokens.size() == 4) {
      std::string baseUrl = tokens[0];
      std::string name = tokens[1];
      std::string variantName = tokens[2];
      std::string variantFileName = tokens[3];
      CesiumCpp::GltfTests::testReadSampleModel(baseUrl, name, variantName,
                                                variantFileName);
    }
    return 0;
  }
  printUsage();
  return 0;
}

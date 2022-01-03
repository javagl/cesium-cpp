#include "GltfTests/GltfTests.h"

#include "Common/Common.h"

#include <CesiumGltfReader/GltfReader.h>

#include <spdlog/spdlog.h>

#include <iostream>

namespace CesiumCpp {

namespace GltfTests {

DataReader createFileReader(const std::string &basePath) {
  DataReader dataReader = [&basePath](const std::string &relativePath) {
    std::string fullPath = basePath + relativePath;
    std::vector<std::byte> data = CesiumCpp::Common::readFile(fullPath.c_str());
    return data;
  };
  return dataReader;
}

std::vector<SampleModel> readSampleModels(const DataReader &dataReader) {
  std::vector<SampleModel> sampleModels;

  std::vector<std::byte> indexFileData = dataReader("model-index.json");
  rapidjson::Document document;
  document.Parse(reinterpret_cast<const char *>(indexFileData.data()),
                 indexFileData.size());
  if (document.HasParseError()) {
    SPDLOG_ERROR("Could not read model-index.json");
    return sampleModels;
  }
  if (!document.IsArray()) {
    SPDLOG_ERROR("Index file result is not an array");
    return sampleModels;
  }
  for (rapidjson::SizeType i = 0; i < document.Size(); i++) {
    const rapidjson::Value &entry = document[i];

    std::optional sampleModel = processSampleModel(i, entry);
    if (sampleModel.has_value()) {
      sampleModels.push_back(sampleModel.value());
    }
  }
  return sampleModels;
}

std::optional<SampleModel> processSampleModel(rapidjson::SizeType index,
                                              const rapidjson::Value &entry) {
  if (!entry.IsObject()) {
    SPDLOG_ERROR("Entry {} is not an object", index);
    return std::nullopt;
  }

  if (!entry.HasMember("name")) {
    SPDLOG_ERROR("Entry {} has no name", index);
    return std::nullopt;
  }
  const rapidjson::Value &nameValue = entry["name"];
  if (!nameValue.IsString()) {
    SPDLOG_ERROR("Entry {} does not have a name string", index);
    return std::nullopt;
  }
  std::string name = entry["name"].GetString();

  if (!entry.HasMember("variants")) {
    SPDLOG_ERROR("Entry {} has no variants", index);
    return std::nullopt;
  }
  const rapidjson::Value &variants = entry["variants"];
  if (!variants.IsObject()) {
    SPDLOG_ERROR("Entry {} is has no valid variants", index);
    return std::nullopt;
  }

  std::vector<SampleModelVariant> sampleModelVariants =
      processVariants(name, variants);
  return std::optional<SampleModel>({name, sampleModelVariants});
}

std::vector<SampleModelVariant>
processVariants(const std::string &name, const rapidjson::Value &variants) {
  std::vector<SampleModelVariant> sampleModelVariants;
  for (rapidjson::Value::ConstMemberIterator itr = variants.MemberBegin();
       itr != variants.MemberEnd(); ++itr) {
    std::string variantName = itr->name.GetString();
    const rapidjson::Value &variantFileNameValue = itr->value;
    if (!variantFileNameValue.IsString()) {
      SPDLOG_ERROR("Variant in {} does not have a valid name", name);
      continue;
    }
    std::string variantFileName = variantFileNameValue.GetString();
    sampleModelVariants.push_back({variantName, variantFileName});
  }
  return sampleModelVariants;
}

bool testReadModel(const DataReader &dataReader, const std::string &name,
                   const std::string &variantName,
                   const std::string &variantFileName) {
  std::string subPath = name + "/" + variantName + "/" + variantFileName;
  std::vector<std::byte> data = dataReader(subPath);
  try {
    const std::byte *dataBegin =
        reinterpret_cast<const std::byte *>(data.data());
    CesiumGltfReader::GltfReader gltfReader;
    CesiumGltfReader::ModelReaderResult modelReaderResult =
        gltfReader.readModel(gsl::span(dataBegin, data.size()));
    bool success = modelReaderResult.model.has_value();
    SPDLOG_INFO("Model {} variant {} : {}", name, variantName,
                (success ? "PASSED" : " !!! FAILED !!! "));
    if (!modelReaderResult.warnings.empty()) {
      for (const auto &warning : modelReaderResult.warnings) {
        SPDLOG_WARN("    {}", warning);
      }
    }
    if (!modelReaderResult.errors.empty()) {
      for (const auto &error : modelReaderResult.errors) {
        SPDLOG_ERROR("    {}", error);
      }
    }
    return success;
  } catch (...) {
    SPDLOG_INFO("Model {} variant {} : caused an unknown error!", name,
                variantName);
    return false;
  }
}

bool testReadSampleModel(const std::string &basePath, const std::string &name,
                         const std::string &variantName,
                         const std::string &variantFileName) {
  DataReader dataReader = createFileReader(basePath);
  bool success = testReadModel(dataReader, name, variantName, variantFileName);
  return success;
}

bool testReadSampleModels(const std::string &basePath) {
  std::vector<std::string> variantsToTest{"glTF", "glTF-Binary",
                                          "glTF-Embedded", "glTF-Draco"};
  bool allPassed = true;
  DataReader dataReader = createFileReader(basePath);
  std::vector<SampleModel> sampleModels = readSampleModels(dataReader);
  int counter = 0;
  int successCounter = 0;
  for (const auto &sampleModel : sampleModels) {

    const std::string &name = sampleModel.name;

    const auto &variants = sampleModel.sampleModelVariants;

    for (const auto &variant : variants) {

      const std::string &variantName = variant.variantName;
      const std::string &variantFileName = variant.variantFileName;
      if (std::find(variantsToTest.begin(), variantsToTest.end(),
                    variantName) != variantsToTest.end()) {
        bool success =
            testReadModel(dataReader, name, variantName, variantFileName);
        if (success) {
          successCounter++;
        }
        allPassed &= success;
        counter++;
      }
    }
  }
  SPDLOG_INFO("Tested {} models, with {} failures", counter,
              counter - successCounter);
  return allPassed;
}
} // namespace GltfTests

} // namespace CesiumCpp

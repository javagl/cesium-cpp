
#include "CesiumNativeImpl/SimpleTaskProcessorEx.h"

#include "Common/Common.h"

#include <spdlog/spdlog.h>

#include <thread>

namespace CesiumCpp {

namespace CesiumNativeImpl {

SimpleTaskProcessorEx::SimpleTaskProcessorEx()
    : _sleepDurationMs(50), _blocking(false) {
  // Nothing else to do here
}

SimpleTaskProcessorEx::SimpleTaskProcessorEx(int32_t sleepDurationMs,
                                             bool blocking)
    : _sleepDurationMs(sleepDurationMs), _blocking(blocking) {
  // Nothing else to do here
}

void SimpleTaskProcessorEx::startTask(std::function<void()> f) {
  SPDLOG_TRACE("Called SimpleTaskProcessorEx::startTask");

  std::thread thread([this, f] {
    SPDLOG_TRACE("SimpleTaskProcessorEx thread running");
    try {
      CesiumCpp::Common::sleepMsLogged(_sleepDurationMs);
      f();
    } catch (const std::exception &e) {
      SPDLOG_ERROR("SimpleTaskProcessorEx: Exception in background thread: {0}",
                   e.what());
    }
    SPDLOG_TRACE("SimpleTaskProcessorEx thread running DONE");
  });

  if (_blocking) {
    thread.join();
  } else {
    thread.detach();
  }
}
} // namespace CesiumNativeImpl
} // namespace CesiumCpp

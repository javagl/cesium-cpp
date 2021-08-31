#pragma once

#include "CesiumAsync/ITaskProcessor.h"

#include <chrono>
#include <cstdint>

namespace CesiumCpp {
  namespace CesiumNative {
    /**
     * @brief Simple implementation of an ITaskProcessor.
     */
    class SimpleTaskProcessorEx : public CesiumAsync::ITaskProcessor {

    public:
      /**
       * @brief Creates a new instance.
       *
       * By default, this will use a delay of 50ms before starting a
       * task, and the execution will be NON-blocking
       */
      SimpleTaskProcessorEx();

      /**
       * @brief Creates a new instance.
       *
       * @param sleepDurationMs The duration, in milliseconds, to sleep
       * before actually processing the task. If this is not positive,
       * then there will be no delay
       * @blocking Whether the execution should block until the task
       * is finished (usually supposed to be `false`).
       */
      SimpleTaskProcessorEx(int32_t sleepDurationMs, bool blocking);

      void startTask(std::function<void()> f) override;

    private:
      int32_t _sleepDurationMs;
      bool _blocking;
    };

  } // namespace CesiumNative
} // namespace CesiumCpp


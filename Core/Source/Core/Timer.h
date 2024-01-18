#pragma once

#include <chrono>

namespace CryptCore {

  class Timer {
  public:
    Timer() { Reset(); };
    
    void Reset() {
      startTime = std::chrono::high_resolution_clock::now();
    }

    float Elapsed() {
      return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - startTime
      ).count() * 0.001f * 0.001f * 0.001f; // Convert nanoseconds to seconds
    }

    float ElapsedMilli() {
      return Elapsed() * 1000.0f; // Convert seconds to milliseconds
    }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
  };

}
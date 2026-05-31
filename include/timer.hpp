#pragma once


#include <chrono>
#include <iostream>

class [[nodiscard]] timer {
  public:
    constexpr timer() noexcept : start_{std::chrono::high_resolution_clock::now()} {}

    ~timer() {
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsed = end - start_;
      std::clog << "Function ran for: " << elapsed.count() << " ms\n"; 
    }

  private:
    std::chrono::steady_clock::time_point start_;
};

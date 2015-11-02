#include <stout/gtest.hpp>

#include <gtest/gtest.h>

#include "pwave/scenario.hpp"

namespace pwave {

TEST(PwaveTest, ScenarioExample) {
  const int32_t LOAD_ITERATIONS = 100;

  SignalScenario signalGen =
    SignalScenario(LOAD_ITERATIONS)
      .use(math::const10Function)
      .after(12).add(-24.05)
      .after(2).use(new SymetricNoiseGenerator(3))
      .after(23).use(math::linearFunction);

  ITERATE_SIGNAL(signalGen) {
    double_t result = (*signalGen)();
    // See result as CSV:
    (*signalGen).printCSVLine(result);
  }
}

}  // namespace pwave


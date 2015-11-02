#include <stout/gtest.hpp>

#include <gtest/gtest.h>

#include "pwave/generator.hpp"

namespace pwave {

using ::testing::DoAll;

TEST(PwaveTest, ScenarioStableSignal) {
  const double_t THRESHOLD = 0.01;
  const int32_t LOAD_ITERATIONS = 100;


  //EXPECT_NEAR((*loadGen).clearValue(), result, THRESHOLD);
}

}  // namespace pwave


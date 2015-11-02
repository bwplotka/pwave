#ifndef PWAVE_GENERATOR_HPP
#define PWAVE_GENERATOR_HPP

#include <cmath>
#include <iostream>

#include "stout/lambda.hpp"

#include "noise.hpp"

namespace pwave {

constexpr double_t DEFAULT_TIME_WINDOW = 1;
constexpr double_t DEFAULT_START_TIMESTAMP = 34223425;


//! Math Functions - used for load model.
namespace math {

inline double_t linearFunction(double_t x) {
  return x;
}


inline double_t sinFunction(double_t x) {
  return sin(x) + cos(x);
}

}  // namespace math


/**
 * Sample primitive.
 */
struct SignalSample {
  SignalSample() {}

  explicit SignalSample(
      double_t _value, double_t _noise, double_t _timestamp)
      : value(_value), noise(_noise), timestamp(_timestamp) {}

  // Get a sample with noise introduced.
  double_t operator()() const {
    return value + noise;
  }

  // Get a sample without any noise.
  double_t clearValue() const {
    return value;
  }

  // Print Sample in CSV format.
  // Columns: Value; Noise+Value; Result \n
  void printCSVLine(double_t result) const {
    std::cout << value << "; "
        << this->operator()() << "; "
        << result << std::endl;
  }

  double_t value;
  double_t noise;
  double_t timestamp;
};


/**
 * Main class for Signal Generator.
 * It generates samples for each loop iteration (increment function).
 * Features:
 * - Generated signal is modeled via input function.
 * - Noise can be introduced via Noise Generators.
 * - Stop condition after iterations max exceeded.
 * - Optionally you can start iteration from defied value.
 * - Optionally you can make complicated scenario by modifying signal via
 *    public modifier field.
 */
class SignalGenerator {
 public:
  explicit SignalGenerator(
      const lambda::function<double_t(double_t)>& _modelFunction,
      NoiseGenerator* _noiseGen,
      const int32_t _iterations)
    : SignalGenerator(_modelFunction, _noiseGen, 0, _iterations) {}

  explicit SignalGenerator(
      const lambda::function<double_t(double_t)>& _modelFunction,
      NoiseGenerator* _noiseGen,
      const int32_t _iteration,
      const int32_t _iterations)
      : modelFunction(_modelFunction),
        noiseGen(_noiseGen),
        iteration(_iteration),
        iterations(_iterations),
        done(false),
        i(_modelFunction(_iteration), 0, DEFAULT_START_TIMESTAMP) {}

  ~LoadGenerator() {}

  typedef SignalSample const& reference;
  typedef SignalSample const* pointer;

  bool end() {
    return !done;
  }

  reference operator*() const { return i; }
  pointer operator->() const { return &i; }

  // Main Signal generation logic.
  SignalGenerator& operator++() {
    if (done) return *this;

    iteration++;
    // Stop condition.
    if (iteration >= iterations) {
      done = true;
      return *this;
    }

    // Applying modelFunction.
    i.value = modifier + modelFunction(iteration);
    i.timestamp += timeWindow;
    // Apply optional noise.
    i.noise = noiseGen->generate(iteration);

    if (dbg) std::cout << iteration << std::endl;
    return *this;
  }

  SignalGenerator operator++(int32_t) {
    SignalGenerator const tmp(*this);
    ++*this;
    return tmp;
  }

  double_t modifier = 0;
  int32_t iteration;
  bool dbg = false;

 protected:
  const lambda::function<double_t(double_t)>& modelFunction;
  NoiseGenerator* noiseGen;
  const int32_t iterations;
  bool done;
  SignalSample i;

  double_t timeWindow = DEFAULT_TIME_WINDOW;
};

}  // namespace pwave

#endif  // PWAVE_GENERATOR_HPP

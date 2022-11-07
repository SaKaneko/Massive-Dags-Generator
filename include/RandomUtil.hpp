#pragma once
#include <iostream>
#include <random>

namespace MDG {

double BoundNormalDistribution(double AVER, double sigma, double lower_bound,
                               double upper_bound) {
  double ret;
  std::random_device seed_gen;
  std::default_random_engine engine(seed_gen());
  std::normal_distribution<> dist(AVER, sigma);
  while (1) {
    ret = dist(engine);
    if (ret > lower_bound && ret < upper_bound) {
      break;
    }
  }
  return ret;
}
} // namespace MDG
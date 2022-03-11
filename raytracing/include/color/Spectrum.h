#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

namespace color {

using Lambda = double;
using Intensity = double;
using Lambdas = std::vector<Lambda>;
using Intensities = std::vector<Lambda>;

template <size_t nLambdas>
class Spectrum {
 public:
  explicit Spectrum(Intensity c = 0) : m_intensities(nLambdas, c) {}

  Spectrum(Intensities const&& intensities)
      : m_intensities(std::move(intensities)) {
    assert(m_nLambdas == intensities.size());
  }

  Intensity luminance() const {
    return std::reduce(m_intensities.begin(), m_intensities.end()) / nLambdas;
  }

  Lambdas const& lambdas() const { return m_lambdas; }
  Intensities const& intensities() const { return m_intensities; }

  Spectrum<nLambdas> operator*(Spectrum<nLambdas> const& s2) {
    Spectrum<nLambdas> result;
    std::transform(m_intensities.begin(), m_intensities.end(),
                   s2.m_intensities.begin(), result.m_intensities.begin(),
                   std::multiplies<Intensity>());
    return result;
  }

  Spectrum<nLambdas>& operator+=(Spectrum<nLambdas> const& s2) {
    std::transform(m_intensities.begin(), m_intensities.end(),
                   s2.m_intensities.begin(), m_intensities.begin(),
                   std::plus<Intensity>());
    return *this;
  }

  Spectrum<nLambdas>& operator*=(Spectrum<nLambdas> const& s2) {
    std::transform(m_intensities.begin(), m_intensities.end(),
                   s2.m_intensities.begin(), m_intensities.begin(),
                   std::multiplies<Intensity>());
    return *this;
  }

  Spectrum<nLambdas> operator+(Spectrum<nLambdas> const& s2) const {
    Spectrum<nLambdas> result;
    std::transform(m_intensities.begin(), m_intensities.end(),
                   s2.m_intensities.begin(), result.m_intensities.begin(),
                   std::plus<Intensity>());
    return result;
  }

  Spectrum<nLambdas>& operator/=(Intensity c) {
    for (auto& i : m_intensities) i /= c;

    return *this;
  }

  Spectrum<nLambdas> operator*(Intensity c) const {
    Spectrum<nLambdas> result;
    std::transform(
        m_intensities.begin(), m_intensities.end(),
        result.m_intensities.begin(),
        std::bind(std::multiplies<Intensity>(), std::placeholders::_1, c));
    return result;
  }

  Spectrum<nLambdas> operator/(Intensity c) const {
    Spectrum<nLambdas> result;
    std::transform(
        m_intensities.begin(), m_intensities.end(),
        result.m_intensities.begin(),
        std::bind(std::divides<Intensity>(), std::placeholders::_1, c));
    return result;
  }

 private:
  static const size_t m_nLambdas = nLambdas;
  static Lambdas m_lambdas;
  Intensities m_intensities;
};

template <size_t nLambdas>
inline std::ostream& operator<<(std::ostream& os, Spectrum<nLambdas> const& s) {
  Intensities const& ints = s.intensities();

  os << "[";
  for (size_t i = 0; i < ints.size(); ++i) {
    std::cout << ints[i];
    if (i < ints.size() - 1) std::cout << ",";
  }
  os << "]";
  return os;
}

using SColor = Spectrum<3>;

}  // namespace color
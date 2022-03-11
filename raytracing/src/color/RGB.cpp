#include <color/RGB.h>

namespace color {

struct SpectrumVal {
  Lambda lambda;
  Intensity r, g, b;
};

std::vector<SpectrumVal> matchFunc{
    {392, 0.0022, -0.0006, 0.0090}, {408, 0.0290, -0.0095, 0.1440},
    {425, 0.0760, -0.0340, 0.6300}, {444, 0.0000, 0.0000, 1.0000},
    {465, -0.2250, 0.1630, 0.7400}, {487, -0.4230, 0.4410, 0.2160},
    {512, -0.3220, 0.8370, 0.0278}, {540, 0.5610, 1.0540, -0.0082},
    {571, 2.2400, 0.7590, -0.0078}, {606, 3.0800, 0.1790, -0.0026},
    {645, 1.0000, 0.0000, 0.0000},  {689, 0.0601, -0.0005, 0.0000}};

void ColorMatch(Lambda lambda, Intensity &r, Intensity &g, Intensity &b) {
  for (size_t i = 1; i < matchFunc.size(); ++i) {
    if (lambda < matchFunc.at(i).lambda) {
      Lambda la2 = lambda - matchFunc.at(i - 1).lambda;
      Lambda la1 = matchFunc.at(i).lambda - lambda;
      Lambda la = la1 + la2;
      r = (la1 * matchFunc.at(i - 1).r + la2 * matchFunc.at(i).r) / la;
      g = (la1 * matchFunc.at(i - 1).g + la2 * matchFunc.at(i).g) / la;
      b = (la1 * matchFunc.at(i - 1).b + la2 * matchFunc.at(i).b) / la;
      break;
    }
  }
}

}  // namespace color
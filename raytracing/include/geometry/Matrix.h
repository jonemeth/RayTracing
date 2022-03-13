#pragma once

#include <iostream>

#include <geometry/types.h>

namespace geometry {

template <size_t N, size_t M>
struct Matrix {
  using Value = Coord;
  Value values[N][M];

  Value det() {
    if (N != M) throw "";
    if (N != 3) throw "";
    return values[0][0] * values[1][1] * values[2][2] +
           values[0][1] * values[1][2] * values[2][0] +
           values[0][2] * values[1][0] * values[2][1] -
           values[2][0] * values[1][1] * values[0][2] -
           values[2][1] * values[1][2] * values[0][0] -
           values[2][2] * values[1][0] * values[0][1];
  }
  Matrix<N, N> inv() {
    if (N != M) throw "";
    if (N != 3) throw "";
    Value D = det();

    if (D < 1e-8 && D > -1e-8) throw "";

    Value a = (values[1][1] * values[2][2] - values[2][1] * values[1][2]) / D;
    Value b = (values[1][0] * values[2][2] - values[2][0] * values[1][2]) / D;
    Value c = (values[1][0] * values[2][1] - values[2][0] * values[1][1]) / D;

    Value d = (values[0][1] * values[2][2] - values[2][1] * values[0][2]) / D;
    Value e = (values[0][0] * values[2][2] - values[2][0] * values[0][2]) / D;
    Value f = (values[0][0] * values[2][1] - values[2][0] * values[0][1]) / D;

    Value g = (values[0][1] * values[1][2] - values[1][1] * values[0][2]) / D;
    Value h = (values[0][0] * values[1][2] - values[1][0] * values[0][2]) / D;
    Value i = (values[0][0] * values[1][1] - values[1][0] * values[0][1]) / D;

    return {{{a, -d, g}, {-b, e, -h}, {c, -f, i}}};
  }

  template <size_t K>
  Matrix<N, K> operator*(Matrix<M, K> const &mat) const {
    Matrix<N, K> r;
    for (size_t n = 0; n < N; ++n)
      for (size_t k = 0; k < K; ++k) {
        r.values[n][k] = 0.0;
        for (size_t m = 0; m < M; ++m)
          r.values[n][k] += values[n][m] * mat.values[m][k];
      }
    return r;
  }
};

template <size_t N, size_t M>
inline std::ostream &operator<<(std::ostream &os, Matrix<N, M> const &m) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      std::cout << m.values[i][j] << (j < M - 1 ? " " : "");
    }
    if (i < N - 1) std::cout << std::endl;
  }
  return os;
}

}  // namespace geometry
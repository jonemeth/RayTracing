#pragma once

#include <geometry/types.h>

#include <cmath>
#include <iostream>

namespace geometry {

static inline double det4x4(const double m[16]) {
  double inv0 = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] -
                m[9] * m[6] * m[15] + m[9] * m[7] * m[14] +
                m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

  double inv4 = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] +
                m[8] * m[6] * m[15] - m[8] * m[7] * m[14] -
                m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

  double inv8 = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] -
                m[8] * m[5] * m[15] + m[8] * m[7] * m[13] +
                m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

  double inv12 = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] +
                 m[8] * m[5] * m[14] - m[8] * m[6] * m[13] -
                 m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

  return m[0] * inv0 + m[1] * inv4 + m[2] * inv8 + m[3] * inv12;
}

static inline bool inv4x4(const double m[16], double invOut[16]) {
  double inv[16], det;
  int i;

  inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
           m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

  inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] -
           m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

  inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
           m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

  inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

  det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

  if (det == 0) return false;

  inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] -
           m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

  inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
           m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

  inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
           m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

  inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

  inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
           m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

  inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
           m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

  inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

  inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

  inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] -
           m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

  inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
           m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

  inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

  inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

  det = 1.0 / det;

  for (i = 0; i < 16; i++) invOut[i] = inv[i] * det;

  return true;
}

template <size_t N, size_t M>
struct Matrix {
  using Value = Coord;
  Value values[N][M];

  Value det() {
    if (N != M) throw "Requested determinant of a non-square matrix";
    if (N == 3)
      return values[0][0] * values[1][1] * values[2][2] +
             values[0][1] * values[1][2] * values[2][0] +
             values[0][2] * values[1][0] * values[2][1] -
             values[2][0] * values[1][1] * values[0][2] -
             values[2][1] * values[1][2] * values[0][0] -
             values[2][2] * values[1][0] * values[0][1];
    if (N == 4) return det4x4(static_cast<Value *>(&values[0][0]));

    throw "Determinant computation is not implemented for this matrix size!";
  }
  Matrix<N, N> inv() {
    if (N != M) throw "Trying to invert a non-square matrix!";
    if (N == 3) {
      Value D = det();

      if (D < 1e-8 && D > -1e-8) throw "Trying to invert a singular matrix";

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
    } else if (N == 4) {
      Matrix<N, N> I;
      if (!inv4x4(static_cast<Value *>(&values[0][0]),
                  static_cast<Value *>(&I.values[0][0])))
        throw "Trying to invert a singular matrix";
      return I;
    }
    throw "Inverse computation is not implemented for this matrix size!";
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

inline Matrix<4, 4> Identity3D() {
  return Matrix<4, 4>{{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

inline Matrix<4, 4> Translate3D(Coord x, Coord y, Coord z) {
  return Matrix<4, 4>{{{1, 0, 0, x}, {0, 1, 0, y}, {0, 0, 1, z}, {0, 0, 0, 1}}};
}

inline Matrix<4, 4> Scale3D(Coord x, Coord y, Coord z) {
  return Matrix<4, 4>{{{x, 0, 0, 0}, {0, y, 0, 0}, {0, 0, z, 0}, {0, 0, 0, 1}}};
}

inline Matrix<4, 4> Scale3D(Coord s) {
  return Matrix<4, 4>{{{s, 0, 0, 0}, {0, s, 0, 0}, {0, 0, s, 0}, {0, 0, 0, 1}}};
}

inline Matrix<4, 4> RotateZ3D(Coord alpha) {
  Coord ca = std::cos(alpha), sa = std::sin(alpha);
  return Matrix<4, 4>{
      {{ca, -sa, 0, 0}, {sa, ca, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

inline Matrix<4, 4> RotateY3D(Coord alpha) {
  Coord ca = std::cos(alpha), sa = std::sin(alpha);
  return Matrix<4, 4>{
      {{ca, 0, sa, 0}, {0, 1, 0, 0}, {-sa, 0, ca, 0}, {0, 0, 0, 1}}};
}

inline Matrix<4, 4> RotateX3D(Coord alpha) {
  Coord ca = std::cos(alpha), sa = std::sin(alpha);
  return Matrix<4, 4>{
      {{1, 0, 0, 0}, {0, ca, -sa, 0}, {0, sa, ca, 0}, {0, 0, 0, 1}}};
}

}  // namespace geometry
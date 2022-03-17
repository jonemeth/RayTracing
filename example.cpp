#include <color/Image.h>
#include <color/Spectrum.h>
#include <geometry/Matrix.h>
#include <modelling/Camera.h>
#include <modelling/Emitter.h>
#include <modelling/Primitive.h>
#include <rendering/RenderScene.h>
#include <rendering/render.h>

#include <chrono>
#include <iostream>
#include <memory>

int example() {
  namespace c = color;
  namespace g = geometry;
  namespace m = modelling;

  double angle = -45.0 * M_PI / 180.0;
  modelling::Camera camera(
      {0.0, 5.0, 0.0}, {4.0 / 3.0, 0.0, 0.0},
      {0.0, std::cos(angle), std::sin(angle)},
      {0.0, 5.0 - 3.5 * std::sin(angle), 3.5 * std::cos(angle)});

  // Normal maps
  std::shared_ptr<m::NormalMap> ballNormal = std::make_shared<m::NormalMap>(
      "resources/basketballNormal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> dropsNormal = std::make_shared<m::NormalMap>(
      "resources/dropsNormal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> brick1Normal = std::make_shared<m::NormalMap>(
      "resources/brick1Normal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> earthNormal = std::make_shared<m::NormalMap>(
      "resources/earthNormal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> stoneNormal = std::make_shared<m::NormalMap>(
      "resources/stoneNormal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> concrete1Normal =
      std::make_shared<m::NormalMap>("resources/concrete1Normal.png",
                                     m::NormalMap::Bilinear);

  // Textures
  std::shared_ptr<m::Texture> ballText = std::make_shared<m::Texture>(
      "resources/basketball.png", m::Texture::Bilinear);
  std::shared_ptr<m::Texture> earthText =
      std::make_shared<m::Texture>("resources/earth.png", m::Texture::Bilinear);
  std::shared_ptr<m::Texture> billiardText = std::make_shared<m::Texture>(
      "resources/billiard.png", m::Texture::Bilinear);
  std::shared_ptr<m::Texture> stoneText =
      std::make_shared<m::Texture>("resources/stone.png", m::Texture::Bilinear);
  std::shared_ptr<m::Texture> brick1Text = std::make_shared<m::Texture>(
      "resources/brick1.png", m::Texture::Bilinear);
  std::shared_ptr<m::Texture> concrete1Text = std::make_shared<m::Texture>(
      "resources/concrete1.png", m::Texture::Bilinear);

  // Materials
  std::shared_ptr<m::Material> stoneMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.8, 0.8, 0.8}), c::SColor({0.1, 0.1, 0.1}), 8.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.0, 0.0, 0.0}), 0.0, stoneText);

  std::shared_ptr<m::Material> billiardMat =
      std::make_shared<m::GeneralMaterial>(
          c::SColor({1.0, 1.0, 1.0}), c::SColor({0.3, 0.3, 0.3}), 16.0,
          c::SColor({0.7, 0.7, 0.7}), c::SColor({0.0, 0.0, 0.0}), 0.0,
          billiardText);

  std::shared_ptr<m::Material> earthMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({1.0, 1.0, 1.0}), c::SColor({0.2, 0.2, 0.2}), 64.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.0, 0.0, 0.0}), 0.0, earthText);

  std::shared_ptr<m::Material> brick1Mat = std::make_shared<m::GeneralMaterial>(
      c::SColor({1.0, 1.0, 1.0}), c::SColor({0.2, 0.2, 0.2}), 32.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.0, 0.0, 0.0}), 0.0, brick1Text);

  std::shared_ptr<m::Material> concrete1Mat =
      std::make_shared<m::GeneralMaterial>(
          c::SColor({1.0, 1.0, 1.0}), c::SColor({0.2, 0.2, 0.2}), 64.0,
          c::SColor({0.0, 0.0, 0.0}), c::SColor({0.0, 0.0, 0.0}), 0.0,
          concrete1Text);

  std::shared_ptr<m::Material> ballMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({1.0, 1.0, 1.0}), c::SColor({0.0, 0.0, 0.0}), 0.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.0, 0.0, 0.0}), 0.0, ballText);

  std::shared_ptr<m::Material> glassMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.11, 0.1, 0.1}), 64.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({1.0, 1.0, 1.0}), 1.1);

  std::shared_ptr<m::Material> mirrorMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.2, 0.2, 0.2}), 64.0,
      c::SColor({1.0, 1.0, 1.0}), c::SColor({0.0, 0.0, 0.0}), 0.0);

  rendering::RenderScene scene(camera);

  scene.primitives.emplace_back(
      std::make_shared<m::Torus>(1.0, 0.5,
                                 g::Translate3D(5.0, -2.8, -8.0) *
                                     g::RotateY3D(-1.57) * g::RotateX3D(-0.8),
                                 mirrorMat));

  scene.primitives.emplace_back(std::make_shared<m::Torus>(
      1.0, 0.5,
      g::Translate3D(-4.0, -3.5, -5.3) * g::RotateY3D(0) * g::RotateX3D(-1.57),
      stoneMat, stoneNormal));

  scene.primitives.emplace_back(
      std::make_shared<m::Sphere>(geometry::Point3D{-3.0, -2.5, -8.0}, 1.5,
                                  g::RotateY3D(-0.2), earthMat, earthNormal));

  scene.primitives.emplace_back(
      std::make_shared<m::Sphere>(g::Point3D{-0.8, -2.5, -6.5}, 1.5,
                                  g::Identity3D(), glassMat, dropsNormal));

  scene.primitives.emplace_back(std::make_shared<m::Sphere>(
      g::Point3D{0.0, -2.5, -10.0}, 1.5, g::Identity3D(), mirrorMat));

  scene.primitives.emplace_back(std::make_shared<m::Sphere>(
      g::Point3D{2.3, -2.8, -6.0}, 1.2, g::RotateX3D(-0.7) * g::RotateY3D(1.3),
      billiardMat));

  scene.primitives.emplace_back(std::make_shared<m::Sphere>(
      g::Point3D{2.6, -2.8, -9.0}, 1.2, g::RotateY3D(0.5) * g::RotateZ3D(0.5),
      ballMat, ballNormal));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-10, -4, -12}, g::Point3D{-10, 16, -12},
      g::Point3D{10, -4, -12}, brick1Mat, g::Point2D{0, 1}, g::Point2D{0, 0},
      g::Point2D{1, 1}, brick1Normal, g::Point3D{1, 0, 0},
      g::Point3D{0, -1, 0}));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-10, 16, -12}, g::Point3D{10, 16, -12},
      g::Point3D{10, -4, -12}, brick1Mat, g::Point2D{0, 0}, g::Point2D{1, 0},
      g::Point2D{1, 1}, brick1Normal, g::Point3D{1, 0, 0},
      g::Point3D{0, -1, 0}));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-10, -4, 8}, g::Point3D{-10, -4, -12}, g::Point3D{10, -4, -12},
      concrete1Mat, g::Point2D{0, 2}, g::Point2D{0, 0}, g::Point2D{2, 0},
      concrete1Normal, g::Point3D{1, 0, 0}, g::Point3D{0, 0, -1}));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-10, -4, 8}, g::Point3D{10, -4, -12}, g::Point3D{10, -4, 8},
      concrete1Mat, g::Point2D{0, 2}, g::Point2D{2, 0}, g::Point2D{2, 2},
      concrete1Normal, g::Point3D{1, 0, 0}, g::Point3D{0, 0, -1}));

  scene.emitters.emplace_back(std::make_shared<m::SphereLight>(
      g::Point3D{4.0, 3.0, -0.0}, 0.4, c::SColor({6.3, 2.3, 1.4}) * 100.0));

  scene.emitters.emplace_back(std::make_shared<m::SphereLight>(
      g::Point3D{-4.0, 3.0, -0.0}, 0.4, c::SColor({6.3, 2.3, 1.4}) * 100.0));

  color::ImageSize imageSize{2 * 320, 2 * 240};  //{640, 480};
  auto start = std::chrono::steady_clock::now();
  color::ImageData imageData = render(scene, imageSize, 8);
  auto end = std::chrono::steady_clock::now();

  std::cout << "Elapsed time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << " ms" << std::endl;
  color::saveImage("example.png", {imageSize, imageData});
  return 0;
}

int main() {
  try {
    return example();
  } catch (char const* str) {
    std::cout << "Exception: " << str << std::endl;
  }
  return 1;
}
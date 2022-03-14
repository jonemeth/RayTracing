
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
  /*
    geometry::Matrix<3,3> mat{ {{1, -2, 1}, {-1, 1, -2}, {4, 5, 1}} };
  //  geometry::Matrix<3,3> mat{ {{1, 2, 3}, {0, 1, 4}, {5, 6, 0}} };
    std::cout << mat << std::endl << std::endl;
    std::cout << mat.det() << std::endl << std::endl;

    std::cout << mat.inv() << std::endl << std::endl;
    std::cout << mat.inv()*mat << std::endl << std::endl;
    std::cout << mat*mat.inv() << std::endl << std::endl;

    return 0;*/

  /*color::Image im = color::loadImage("./resources/chess.png");
  color::saveImage("out.png", im);
  return 0;*/

  double angle = -45.0 * M_PI / 180.0;
  modelling::Camera camera(
      {0.0, 5.0, 0.0}, {4.0 / 3.0, 0.0, 0.0},
      {0.0, std::cos(angle), std::sin(angle)},
      {0.0, 5.0 - 3.0 * std::sin(angle), 3.0 * std::cos(angle)});

  // Normal maps
  std::shared_ptr<m::NormalMap> normalTile = std::make_shared<m::NormalMap>(
      "resources/woodNormal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> ballNormal = std::make_shared<m::NormalMap>(
      "resources/basketballNormal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> normalDrops = std::make_shared<m::NormalMap>(
      "resources/dropsNormal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> normalWall = std::make_shared<m::NormalMap>(
      "resources/wallNormal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> earthNormal = std::make_shared<m::NormalMap>(
      "resources/earthNormal.png", m::NormalMap::Bilinear);
  std::shared_ptr<m::NormalMap> waterNormal = std::make_shared<m::NormalMap>(
      "resources/waterNormal.png", m::NormalMap::Bilinear);
  // Textures
  std::shared_ptr<m::Texture> chessText =
      std::make_shared<m::Texture>("resources/wood.png", m::Texture::Nearest);
  std::shared_ptr<m::Texture> ballText = std::make_shared<m::Texture>(
      "resources/basketball.png", m::Texture::Bilinear);
  std::shared_ptr<m::Texture> earthText =
      std::make_shared<m::Texture>("resources/earth.png", m::Texture::Nearest);
  std::shared_ptr<m::Texture> billiardText = std::make_shared<m::Texture>(
      "resources/billiard.png", m::Texture::Nearest);

  // Materials
  std::shared_ptr<m::Material> redMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({1.0, 1.0, 1.0}), c::SColor({0.3, 0.3, 0.3}), 16.0,
      c::SColor({0.7, 0.7, 0.7}), c::SColor({0.0, 0.0, 0.0}), 0.0,
      billiardText);

  std::shared_ptr<m::Material> earthMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({1.0, 1.0, 1.0}), c::SColor({0.2, 0.2, 0.2}), 64.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.0, 0.0, 0.0}), 0.0, earthText);

  std::shared_ptr<m::Material> greenMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.1, 0.6, 0.1}), c::SColor({0.1, 0.1, 0.1}), 16.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.0, 0.0, 0.0}), 0.0);

  std::shared_ptr<m::Material> grayMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.8, 0.8, 0.8}), c::SColor({0.2, 0.2, 0.2}), 32.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.0, 0.0, 0.0}), 0.0, chessText);

  std::shared_ptr<m::Material> ballMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({1.0, 1.0, 1.0}), c::SColor({0.0, 0.0, 0.0}), 0.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.0, 0.0, 0.0}), 0.0, ballText);

  std::shared_ptr<m::Material> glassMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.2, 0.2, 0.2}), 32.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({1.0, 1.0, 1.0}), 1.1);

  std::shared_ptr<m::Material> mirrorMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.5, 0.5, 0.5}), 64.0,
      c::SColor({1.0, 1.0, 1.0}), c::SColor({0.0, 0.0, 0.0}), 0.0);

  rendering::RenderScene scene(camera);

  scene.primitives.emplace_back(std::make_shared<m::Sphere>(
      g::Point3D{-2.0, -2.5, -8.0}, 1.5, earthMat, earthNormal));

  scene.primitives.emplace_back(std::make_shared<m::Sphere>(
      g::Point3D{0.2, -2.5, -6.5}, 1.5, glassMat, normalDrops));

  scene.primitives.emplace_back(std::make_shared<m::Sphere>(
      g::Point3D{1.0, -2.5, -10.0}, 1.5, mirrorMat));

  scene.primitives.emplace_back(
      std::make_shared<m::Sphere>(g::Point3D{3.3, -2.8, -6.0}, 1.2, redMat));

  scene.primitives.emplace_back(std::make_shared<m::Sphere>(
      g::Point3D{-5.0, -2.8, -10.0}, 1.2, mirrorMat, waterNormal));

  scene.primitives.emplace_back(std::make_shared<m::Sphere>(
      g::Point3D{3.6, -2.8, -9.0}, 1.2, ballMat, ballNormal));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-10, -4, -12}, g::Point3D{-10, 16, -12},
      g::Point3D{10, -4, -12}, greenMat, g::Point2D{0, 1}, g::Point2D{0, 0},
      g::Point2D{1, 1}, normalWall, g::Point3D{1, 0, 0}, g::Point3D{0, 1, 0}));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-10, 16, -12}, g::Point3D{10, 16, -12},
      g::Point3D{10, -4, -12}, greenMat, g::Point2D{0, 0}, g::Point2D{1, 0},
      g::Point2D{1, 1}, normalWall, g::Point3D{1, 0, 0}, g::Point3D{0, 1, 0}));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-10, -4, 8}, g::Point3D{-10, -4, -12}, g::Point3D{10, -4, -12},
      grayMat, g::Point2D{0, 1}, g::Point2D{0, 0}, g::Point2D{1, 0}, normalTile,
      g::Point3D{1, 0, 0}, g::Point3D{0, 0, -1}));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-10, -4, 8}, g::Point3D{10, -4, -12}, g::Point3D{10, -4, 8},
      grayMat, g::Point2D{0, 1}, g::Point2D{1, 0}, g::Point2D{1, 1}, normalTile,
      g::Point3D{1, 0, 0}, g::Point3D{0, 0, -1}));

  scene.emitters.emplace_back(std::make_shared<m::SphereLight>(
      g::Point3D{6.0, 3.0, -3.0}, 0.4, c::SColor({6.3, 2.3, 1.4}) * 100.0));

  scene.emitters.emplace_back(std::make_shared<m::SphereLight>(
      g::Point3D{-6.0, 3.0, -0.0}, 0.4, c::SColor({6.3, 2.3, 1.4}) * 100.0));

  color::ImageSize imageSize{1280, 960}; //{640, 480};
  auto start = std::chrono::steady_clock::now();
  color::ImageData imageData = render(scene, imageSize, 12);
  auto end = std::chrono::steady_clock::now();

  std::cout << "Elapsed time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << " ms" << std::endl;
  color::saveImage("render.png", {imageSize, imageData});
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
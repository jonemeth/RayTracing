
#include <color/Spectrum.h>
#include <modelling/Camera.h>
#include <modelling/Emitter.h>
#include <modelling/Primitive.h>
#include <color/Image.h>
#include <rendering/RenderScene.h>
#include <rendering/render.h>

#include <chrono>
#include <iostream>
#include <memory>

int main() {
  namespace c = color;
  namespace g = geometry;
  namespace m = modelling;

  double angle = -45.0 * M_PI / 180.0;
  modelling::Camera camera(
      {0.0, 5.0, 0.0}, {4.0 / 3.0, 0.0, 0.0},
      {0.0, std::cos(angle), std::sin(angle)},
      {0.0, 5.0 - 3.0 * std::sin(angle), 3.0 * std::cos(angle)});

  std::shared_ptr<m::Material> redMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.0, 0.0, 1.0}), c::SColor({0.3, 0.3, 0.6}), 16.0,
      c::SColor({0.7, 0.7, 1.0}), c::SColor({0.0, 0.0, 0.0}), 0.0);

  std::shared_ptr<m::Material> blueMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({1.0, 0.0, 0.0}), c::SColor({0.6, 0.3, 0.3}), 16.0,
      c::SColor({1.0, 0.7, 0.7}), c::SColor({0.0, 0.0, 0.0}), 0.0);

  std::shared_ptr<m::Material> greenMat =
      std::make_shared<m::DiffuseMaterial>(c::SColor({0.5, 1.0, 0.5}));

  std::shared_ptr<m::Material> grayMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.8, 0.8, 0.8}), c::SColor({0.1, 0.1, 0.1}), 32.0,
      c::SColor({0.9, 0.9, 0.9}), c::SColor({0.0, 0.0, 0.0}), 0.0);

  std::shared_ptr<m::Material> glassMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.8, 0.8, 0.8}), 64.0,
      c::SColor({0.0, 0.0, 0.0}), c::SColor({1.0, 0.9, 0.9}), 1.1);

  std::shared_ptr<m::Material> MirrorMat = std::make_shared<m::GeneralMaterial>(
      c::SColor({0.0, 0.0, 0.0}), c::SColor({0.5, 0.5, 0.5}), 64.0,
      c::SColor({1.0, 1.0, 1.0}), c::SColor({0.0, 0.0, 0.0}), 0.0);

  rendering::RenderScene scene(camera);

  scene.primitives.emplace_back(
      std::make_shared<m::Sphere>(g::Point3D{-2.0, -2.5, -8.0}, 1.5, blueMat));

  scene.primitives.emplace_back(
      std::make_shared<m::Sphere>(g::Point3D{0.2, -2.5, -6.5}, 1.5, glassMat));

  scene.primitives.emplace_back(
      std::make_shared<m::Sphere>(g::Point3D{1.0, -2.5, -10.0}, 1.5, MirrorMat));

  scene.primitives.emplace_back(
      std::make_shared<m::Sphere>(g::Point3D{3.3, -2.8, -6.0}, 1.2, redMat));

  scene.primitives.emplace_back(
      std::make_shared<m::Sphere>(g::Point3D{-5.0, -2.8, -10.0}, 1.2, MirrorMat));

  scene.primitives.emplace_back(
      std::make_shared<m::Sphere>(g::Point3D{3.6, -2.8, -9.0}, 1.2, MirrorMat));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-20, -4, -12}, g::Point3D{-20, 4, -12}, g::Point3D{20, -4, -12},
      greenMat));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-20, 4, -12}, g::Point3D{20, 4, -12}, g::Point3D{20, -4, -12},
      greenMat));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-20, -4, 0}, g::Point3D{-20, -4, -12}, g::Point3D{20, -4, -12},
      grayMat));

  scene.primitives.emplace_back(std::make_shared<m::Triangle>(
      g::Point3D{-20, -4, 0}, g::Point3D{20, -4, -12}, g::Point3D{20, -4, 0},
      grayMat));

  scene.emitters.emplace_back(std::make_shared<m::SphereLight>(
      g::Point3D{3.0, 5.0, -7.0}, 0.4, c::SColor({6.3, 2.3, 1.4}) * 80.0));

  scene.emitters.emplace_back(std::make_shared<m::SphereLight>(
      g::Point3D{-6.0, 5.0, -0.0}, 0.4, c::SColor({6.3, 2.3, 1.4}) * 80.0));

  
  rendering::ImageSize imageSize{320, 240};
  auto start = std::chrono::steady_clock::now();
  rendering::ImageData imageData = render(scene, imageSize);
  auto end = std::chrono::steady_clock::now();

  std::cout << "Elapsed time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << " ms" << std::endl;
  rendering::saveImage("render.png", {imageSize, imageData});
}

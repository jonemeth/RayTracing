#pragma once

#include <modelling/Camera.h>
#include <modelling/Primitive.h>
#include <modelling/Emitter.h>

#include <memory>
#include <vector>

namespace rendering {

struct RenderScene {
  RenderScene(modelling::Camera camera_) : camera(camera_) {}

  modelling::Camera camera;
  std::vector<std::shared_ptr<modelling::Primitive>> primitives{};
  std::vector<std::shared_ptr<modelling::Emitter>> emitters{};
};

}  // namespace rendering
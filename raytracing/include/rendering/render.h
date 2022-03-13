#pragma once

#include <color/Image.h>
#include <rendering/RenderScene.h>

namespace rendering {

color::ImageData render(RenderScene const& renderScene, color::ImageSize imageSize, size_t gridSize=4);

}  // namespace rendering
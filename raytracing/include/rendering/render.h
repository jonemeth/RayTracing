#pragma once

#include <color/Image.h>
#include <rendering/RenderScene.h>

namespace rendering {

color::ImageData render(RenderScene const& renderScene,
                        color::ImageSize imageSize, size_t gridSize = 4,
                        size_t maxDepth = 32);

}  // namespace rendering
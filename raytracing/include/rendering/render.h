#pragma once

#include <rendering/Image.h>
#include <rendering/RenderScene.h>

namespace rendering {

ImageData render(RenderScene const& renderScene, ImageSize imageSize);

}  // namespace rendering
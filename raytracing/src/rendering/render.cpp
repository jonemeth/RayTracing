#include <rendering/render.h>

namespace rendering {

#define MAXDEPTH (32)

struct Intersection {
  std::shared_ptr<modelling::Primitive> primitive;
  geometry::Coord x;
};

Intersection intersect(RenderScene const& renderScene, geometry::Ray ray) {
  std::shared_ptr<modelling::Primitive> visiblePrimitive;
  geometry::Coord smallestDistance =
      std::numeric_limits<geometry::Coord>::max();

  for (auto const& primitive : renderScene.primitives) {
    geometry::Coord distance = primitive->intersect(ray);
    if (distance > 0.0 && distance < smallestDistance) {
      smallestDistance = distance;
      visiblePrimitive = primitive;
    }
  }
  return {
      visiblePrimitive,
      smallestDistance};  // ray.sta  rt + smallestDistance * ray.direction};
}

color::SColor intersectShadow(RenderScene const& renderScene,
                              geometry::Ray rayToLight,
                              geometry::Coord lightDist) {
  color::SColor attn(1.0);

  for (auto const& primitive : renderScene.primitives) {
    geometry::Coord t = primitive->intersect(rayToLight);

    if (t > 1e-4 && t < lightDist) attn *= primitive->transparency();

    if (attn.luminance() < 1e-2) return attn;
  }
  return attn;
}

color::SColor directLightSource(RenderScene const& renderScene,
                                std::shared_ptr<modelling::Primitive> primitive,
                                geometry::Point3D const& x,
                                geometry::Normal3D const& N,
                                geometry::Normal3D const& V) {
  color::SColor c(0.0);

  for (auto const emitter : renderScene.emitters) {
    auto [Le, lightPos, rayToLight] = emitter->emission(x, N);
    if (Le.luminance() < 1e-2) continue;

    geometry::Vector3D L = lightPos - x;
    geometry::Coord lightDist = L.length();

    color::SColor atten = intersectShadow(renderScene, rayToLight, lightDist);

    c += atten * primitive->BRDF(L, N, V) * Le;
  }
  return c;
}

/*
std::vector<size_t> reflectionSchedule = {1, 1}; //4, 3, 2, 1, 1};
color::SColor traceGlobal2(RenderScene const& renderScene, geometry::Ray ray,
                          size_t d) {
  if (d > MAXDEPTH) return color::SColor(0);

  auto [primitive, t] = intersect(renderScene, ray);

  if (!primitive) return color::SColor(0.0);
  geometry::Point3D x = ray.start + t * ray.direction;
  geometry::Normal3D normal = primitive->normal(x);
  color::SColor c =
      directLightSource(renderScene, primitive, x, normal, -ray.direction);

  modelling::Reflections reflections = primitive->reflections(
      normal, -ray.direction,
      ((d < reflectionSchedule.size()) ? reflectionSchedule[d] : 1));

  color::SColor ac(0);

  for (auto const& reflection : reflections) {
    if (reflection.prob < 1e-3) continue;

    geometry::Coord cost = reflection.dir * normal;
    if (cost < 0) cost = -cost;
    if (cost > 1e-3) {
      color::SColor w = reflection.color * cost * reflection.prob;
      if (w.luminance() > 1e-3) {
        ac += traceGlobal2(renderScene, {x, reflection.dir}, d + 1) * w;
      }
    }
  }

  return c + ac / static_cast<color::Intensity>(reflections.size());
}*/

/*color::SColor traceGlobal(RenderScene const& renderScene, geometry::Ray ray,
                          size_t d) {
  if (d > MAXDEPTH) return color::SColor(0);

  auto [primitive, t] = intersect(renderScene, ray);

  if (!primitive) return color::SColor(0.0);
  geometry::Point3D x = ray.start + t * ray.direction;
  geometry::Normal3D normal = primitive->normal(x);
  color::SColor c =
      directLightSource(renderScene, primitive, x, normal, -ray.direction);

  modelling::Reflection reflection =
      primitive->reflection(normal, -ray.direction);

  if (reflection.prob < 1e-2) return c;

  geometry::Coord cost = reflection.dir * normal;
  if (cost < 0) cost = -cost;
  if (cost > 1e-2) {
    color::SColor w = reflection.color * cost * reflection.prob;
    if (w.luminance() > 1e-2) {
      c += traceGlobal(renderScene, {x, reflection.dir}, d + 1) * w;
    }
  }

  return c;
}*/

color::SColor traceGlobal(RenderScene const& renderScene, geometry::Ray ray) {
  color::SColor c(0);
  color::SColor w(1);

  for (size_t i = 0; i < MAXDEPTH; ++i) {
    auto [primitive, t] = intersect(renderScene, ray);

    if (!primitive) break;
    geometry::Point3D x = ray.start + t * ray.direction;
    geometry::Normal3D normal = primitive->normal(x);
    c += w *
         directLightSource(renderScene, primitive, x, normal, -ray.direction);

    modelling::Reflection reflection =
        primitive->reflection(normal, -ray.direction);

    if (reflection.prob < 1e-2) break;

    geometry::Coord cost = reflection.dir * normal;
    if (cost < 0) cost = -cost;
    if (cost < 1e-2) break;

    w *= reflection.color * cost * reflection.prob;
    if (w.luminance() < 1e-2) break;
    ray = geometry::Ray{x, reflection.dir};
  }

  return c;
}

color::ImageData render(RenderScene const& renderScene, color::ImageSize imageSize,
                 size_t gridSize) {
  color::ImageData imageData;
  imageData.reserve(imageSize.height * imageSize.width);

  using RaySamples = std::vector<geometry::Ray>;

  std::vector<RaySamples> allRaySamples(imageSize.height * imageSize.width);

  for (size_t i = 0; i < imageSize.height; ++i) {
    for (size_t j = 0; j < imageSize.width; ++j) {
      for (size_t u = 0; u < gridSize; ++u) {
        for (size_t v = 0; v < gridSize; ++v) {
          geometry::Coord ii = static_cast<geometry::Coord>(i) +
                               (0.5 + static_cast<geometry::Coord>(u)) /
                                   static_cast<geometry::Coord>(gridSize);
          geometry::Coord jj = static_cast<geometry::Coord>(j) +
                               (0.5 + static_cast<geometry::Coord>(v)) /
                                   static_cast<geometry::Coord>(gridSize);

          geometry::Coord y = -(
              2 * ii / static_cast<geometry::Coord>(imageSize.height - 1) - 1);
          geometry::Coord x =
              2 * jj / static_cast<geometry::Coord>(imageSize.width - 1) - 1;

          allRaySamples[i * imageSize.width + j].push_back(
              renderScene.camera.getRay(x, y));
        }
      }
    }
  }

  for (auto const& raySamples : allRaySamples) {
    color::SColor c(0);

    for (auto const& ray : raySamples) c += traceGlobal(renderScene, ray);

    c /= static_cast<color::Intensity>(gridSize * gridSize);

    imageData.emplace_back(color::RGB(c));
  }

  return imageData;
}

}  // namespace rendering
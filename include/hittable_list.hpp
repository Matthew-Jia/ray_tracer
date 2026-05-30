#pragma once

#include "aabb.hpp"
#include "hittable.hpp"

#include <memory>
#include <vector>

class [[nodiscard]] hittable_list : public hittable {
  public:
    std::vector<std::shared_ptr<hittable>> objects;

    constexpr hittable_list() noexcept = default;

    explicit constexpr hittable_list(std::shared_ptr<hittable> object) noexcept { add(std::move(object)); }

    constexpr void clear() noexcept { objects.clear(); }

    constexpr void add(std::shared_ptr<hittable> object) noexcept
    {
      bbox_ = aabb{bbox_, object->bounding_box()};
      objects.push_back(std::move(object));
    }

    [[nodiscard]] constexpr bool hit(const ray &r, interval ray_t, hit_record &rec) const noexcept override
    {
      hit_record temp_rec;
      bool hit_anything = false;

      for (const auto &obj : objects)
      {
        if (obj->hit(r, ray_t, temp_rec))
        {
          hit_anything = true;
          ray_t.max = temp_rec.t;
          rec = temp_rec;
        }
      }

      return hit_anything;
    }

    constexpr aabb bounding_box() const noexcept override { return bbox_; }

  private:
    aabb bbox_;
};

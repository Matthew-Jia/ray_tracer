#pragma once

#include "aabb.hpp"
#include "hittable.hpp"

#include <memory>
#include <vector>

class [[nodiscard]] hittable_list : public hittable {
  public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() {}
    explicit hittable_list(std::shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<hittable> object)
    {
      objects.push_back(object);
      bbox_ = aabb{bbox_, object->bounding_box()};
    }

    [[nodiscard]] bool hit(const ray &r, interval ray_t, hit_record &rec) const override
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

    aabb bounding_box() const override { return bbox_; }

  private:
    aabb bbox_;
};

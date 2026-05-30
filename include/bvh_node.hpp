#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"


#include <algorithm>
#include <memory>
#include <span>

class [[nodiscard]] bvh_node : public hittable {
  public:
    explicit constexpr bvh_node(hittable_list &list) : bvh_node(list.objects) {}

    explicit constexpr bvh_node(std::span<std::shared_ptr<hittable>> objects)
    {
      assert(!objects.empty());
      bbox_ = aabb::empty;
      for (const auto &obj : objects)
        bbox_ = aabb(bbox_, obj->bounding_box());

      int axis = bbox_.longest_axis();

      if (objects.size() == 1)
      {
        left_ = right_ = objects[0];
      } 
      else if (objects.size() == 2)
      {
        left_ = objects[0];
        right_ = objects[1];
      }
      else
      {
        std::ranges::sort(objects, {}, [axis](const auto &obj){
          return obj->bounding_box().axis_interval(axis).min;
        });

        size_t mid = objects.size() / 2;

        left_ = std::make_shared<bvh_node>(objects.subspan(0, mid));
        right_ = std::make_shared<bvh_node>(objects.subspan(mid));
      }
    }

    constexpr bool hit(const ray &r, interval ray_t, hit_record &rec) const noexcept override
    {
      if (!bbox_.hit(r, ray_t))
        return false;

      bool hit_left = left_->hit(r, ray_t, rec);
      bool hit_right = right_->hit(r, interval{ray_t.min, hit_left ? rec.t : ray_t.max}, rec);

      return hit_left || hit_right;
    }

    constexpr aabb bounding_box() const noexcept override { return bbox_; }

  private:
    std::shared_ptr<hittable> left_;
    std::shared_ptr<hittable> right_;
    aabb bbox_;
};


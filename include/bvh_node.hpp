#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

#include <span>
#include <memory>

class [[nodiscard]] bvh_node : public hittable {
  public:
    explicit
    bvh_node(hittable_list &list) : bvh_node(list.objects) {}

    explicit
    bvh_node(std::span<std::shared_ptr<hittable>> objects)
    {
      assert(!objects.empty());
      bbox_ = aabb::empty;
      for (const auto &obj : objects)
        bbox_ = aabb(bbox_, obj->bounding_box());

      int axis = bbox_.longest_axis();

      // int axis = random_int(0,2);

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
        std::sort(objects.begin(), objects.end(), [&axis](const auto &a, const auto &b){
          return box_compare(a, b, axis);
        });

        int mid = objects.size() / 2;

        left_ = std::make_shared<bvh_node>(objects.subspan(0, mid));
        right_ = std::make_shared<bvh_node>(objects.subspan(mid));
      }
      // bbox_ = aabb(left_->bounding_box(), right_->bounding_box());
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
      if (!bbox_.hit(r, ray_t))
        return false;

      bool hit_left = left_->hit(r, ray_t, rec);
      bool hit_right = right_->hit(r, interval{ray_t.min, hit_left ? rec.t : ray_t.max}, rec);

      return hit_left || hit_right;
    }

    aabb bounding_box() const override { return bbox_; }

  private:
    std::shared_ptr<hittable> left_;
    std::shared_ptr<hittable> right_;
    aabb bbox_;
    
    // primitive as hell comparator
    [[nodiscard]]
    static bool box_compare(
        const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis_index
    )
    {
      auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
      auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
      return a_axis_interval.min < b_axis_interval.min;
    }
};


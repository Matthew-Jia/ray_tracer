#pragma once

#include "aabb.hpp"
#include "common.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

#include <span>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class [[nodiscard]] bvh_node : hittable {
  public:
    explicit bvh_node(hittable_list &list) : bvh_node(list.objects) {}

    // change this to a span
    explicit bvh_node(std::span<shared_ptr<hittable>> objects)
    {
      int axis = random_int(0, 2);

      auto comparator = (axis == 0) ? box_x_compare
                      : (axis == 1) ? box_y_compare
                                    : box_z_compare; 

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
        std::sort(objects.begin(), objects.end(), comparator);

        int mid = objects.size() / 2;

        left_ = make_shared<bvh_node>(objects.subspan(0, mid));
        right_ = make_shared<bvh_node>(objects.subspan(mid));
      }

      bbox_ = aabb{left_->bounding_box(), right_->bounding_box()};
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
    shared_ptr<hittable> left_;
    shared_ptr<hittable> right_;
    aabb bbox_;
    
    [[nodiscard]] static bool box_compare(
        const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index
    )
    {
      auto a_axis
    }
};


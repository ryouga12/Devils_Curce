#include "Camera.h"

KonCamera::KonCamera()
{
  
}

void KonCamera::update(const tnl::Vector3& target_pos , const int& MAP_WIDTH ,const int& MAP_HEIGHT)
{
    // �J�����̒��S���W���v�Z
    tnl::Vector3 camera_center = tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

    // �J�����̈ʒu���X�V
    target_ += (target_pos - target_) * TRACKING_SPPED;
    
    // �J�����̈ʒu�𐧌�
    // �J�����̍�����W���v�Z
    tnl::Vector3 camera_top_left = target_ - camera_center;

    // �J�����̉E�����W���v�Z
    tnl::Vector3 camera_bottom_right = camera_top_left + tnl::Vector3(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, 0);

    // �\���̈���Ɏ��߂�
    if (static_cast<int>(camera_top_left.x) < 0)
    {
        target_.x += - static_cast<int>(camera_top_left.x);
    }
    else if (static_cast<int>(camera_bottom_right.x) > MAP_WIDTH)
    {
        target_.x -= static_cast<int>(camera_bottom_right.x) - MAP_WIDTH;
    }

    if (static_cast<int>(camera_top_left.y) < 0) {
        target_.y += - static_cast<int>(camera_top_left.y);
    }

    else if (static_cast<int>(camera_bottom_right.y) > MAP_HEIGHT)
    {
        target_.y -= static_cast<int>(camera_bottom_right.y) - MAP_HEIGHT;
    }

}

void KonCamera::draw()
{

}



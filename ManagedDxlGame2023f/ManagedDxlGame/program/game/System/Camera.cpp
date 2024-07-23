#include "Camera.h"

KonCamera::KonCamera() : shake_duration_(0), shake_magnitude_(0), shake_elapsed_(0)
{
  
}

void KonCamera::update(tnl::Vector3 target_pos , int MAP_WIDTH ,int MAP_HEIGHT)
{
    //カメラシェイクを起こす
    if (is_shaking_) {
        CameraShake();
    }
    //通常のカメラはプレイヤーを追跡する
    else {
        // カメラの中心座標を計算
        tnl::Vector3 camera_center = tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

        // カメラの位置を更新
        target_ += (target_pos - target_) * 0.05f;

        // カメラの位置を制限
        // カメラの左上座標を計算
        tnl::Vector3 camera_top_left = target_ - camera_center;

        // カメラの右下座標を計算
        tnl::Vector3 camera_bottom_right = camera_top_left + tnl::Vector3(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, 0);

        // 表示領域内に収める
        if (static_cast<int>(camera_top_left.x) < 0)
        {
            target_.x += -static_cast<int>(camera_top_left.x);
        }
        else if (static_cast<int>(camera_bottom_right.x) > MAP_WIDTH)
        {
            target_.x -= static_cast<int>(camera_bottom_right.x) - MAP_WIDTH;
        }

        if (static_cast<int>(camera_top_left.y) < 0) {
            target_.y += -static_cast<int>(camera_top_left.y);
        }

        else if (static_cast<int>(camera_bottom_right.y) > MAP_HEIGHT)
        {
            target_.y -= static_cast<int>(camera_bottom_right.y) - MAP_HEIGHT;
        }
    }
}

void KonCamera::draw()
{

}

//カメラシェイクをスタートさせる
//引数 : シェイクの継続時間 , シェイクの強度
void KonCamera::StartShake(const float duration, const float magnitude)
{
    is_shaking_ = true;
    shake_duration_ = duration;
    shake_magnitude_ = magnitude;
    shake_elapsed_ = 0;
}

void KonCamera::CameraShake()
{
    // シェイクの経過時間が継続時間より短い場合にシェイクを行う
    if (shake_elapsed_ < shake_duration_) {

        // カメラの中心座標を計算
        tnl::Vector3 camera_center = tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

        // ランダムなX方向のオフセットを計算
        float offsetX = (rand() % 2 == 0 ? -1 : 1) * shake_magnitude_;
        // ターゲットの座標にオフセットを追加
        battle_camera_pos_.x += offsetX;
        // 経過時間をフレーム時間分だけ増加（ここでは16msを仮定）
        shake_elapsed_ += 0.016f; // Assume a frame time of 16ms

        // カメラの位置を制限
      // カメラの左上座標を計算
        tnl::Vector3 camera_top_left = battle_camera_pos_ - camera_center;

        // カメラの右下座標を計算
        tnl::Vector3 camera_bottom_right = camera_top_left + tnl::Vector3(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, 0);


        // 表示領域内に収める
        if (static_cast<int>(camera_top_left.x) < 0)
        {
            battle_camera_pos_.x += -static_cast<int>(camera_top_left.x);
        }
        else if (static_cast<int>(camera_bottom_right.x) > DXE_WINDOW_WIDTH + 50)
        {
            battle_camera_pos_.x -= static_cast<int>(camera_bottom_right.x) - DXE_WINDOW_WIDTH;
        }

    }
    else {
        // シェイクが終了したらシェイクフラグをオフにする
        is_shaking_ = false;
        battle_camera_pos_ = battle_camera_save_pos;
    }
}

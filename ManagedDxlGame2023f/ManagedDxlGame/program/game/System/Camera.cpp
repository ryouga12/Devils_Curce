#include "Camera.h"

KonCamera::KonCamera() : shake_duration_(0), shake_magnitude_(0), shake_elapsed_(0)
{
  
}

void KonCamera::update(tnl::Vector3 target_pos , int MAP_WIDTH ,int MAP_HEIGHT)
{
    //�J�����V�F�C�N���N����
    if (is_shaking_) {
        CameraShake();
    }
    //�ʏ�̃J�����̓v���C���[��ǐՂ���
    else {
        // �J�����̒��S���W���v�Z
        tnl::Vector3 camera_center = tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

        // �J�����̈ʒu���X�V
        target_ += (target_pos - target_) * 0.05f;

        // �J�����̈ʒu�𐧌�
        // �J�����̍�����W���v�Z
        tnl::Vector3 camera_top_left = target_ - camera_center;

        // �J�����̉E�����W���v�Z
        tnl::Vector3 camera_bottom_right = camera_top_left + tnl::Vector3(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, 0);

        // �\���̈���Ɏ��߂�
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

//�J�����V�F�C�N���X�^�[�g������
//���� : �V�F�C�N�̌p������ , �V�F�C�N�̋��x
void KonCamera::StartShake(const float duration, const float magnitude)
{
    is_shaking_ = true;
    shake_duration_ = duration;
    shake_magnitude_ = magnitude;
    shake_elapsed_ = 0;
}

void KonCamera::CameraShake()
{
    // �V�F�C�N�̌o�ߎ��Ԃ��p�����Ԃ��Z���ꍇ�ɃV�F�C�N���s��
    if (shake_elapsed_ < shake_duration_) {

        // �J�����̒��S���W���v�Z
        tnl::Vector3 camera_center = tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

        // �����_����X�����̃I�t�Z�b�g���v�Z
        float offsetX = (rand() % 2 == 0 ? -1 : 1) * shake_magnitude_;
        // �^�[�Q�b�g�̍��W�ɃI�t�Z�b�g��ǉ�
        battle_camera_pos_.x += offsetX;
        // �o�ߎ��Ԃ��t���[�����ԕ����������i�����ł�16ms������j
        shake_elapsed_ += 0.016f; // Assume a frame time of 16ms

        // �J�����̈ʒu�𐧌�
      // �J�����̍�����W���v�Z
        tnl::Vector3 camera_top_left = battle_camera_pos_ - camera_center;

        // �J�����̉E�����W���v�Z
        tnl::Vector3 camera_bottom_right = camera_top_left + tnl::Vector3(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, 0);


        // �\���̈���Ɏ��߂�
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
        // �V�F�C�N���I��������V�F�C�N�t���O���I�t�ɂ���
        is_shaking_ = false;
        battle_camera_pos_ = battle_camera_save_pos;
    }
}

//
//�J�����N���X
//


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Player;

class KonCamera {
public:
	KonCamera();

	void update(tnl::Vector3 target_pos, int MAP_WIDTH, int MAP_HEIGHT);
	void draw();

	//�^�[�Q�b�g�̍��W���擾����
	tnl::Vector3 getTargetPos()const{
		return target_;
	}

	//�^�[�Q�b�g�̍��W���Z�b�g����
	void SetTargetPos() {
		target_ = camera_trarget_savepos_;
	}

	//�J�����̕ۊǍ��W�Ɍ��݂̃J�������W��ۑ�����
	void SavePosition(const tnl::Vector3& target_pos) {
		camera_trarget_savepos_ = target_pos;
	}

private:

	//�v���C���[���^�[�Q�b�g�ɂ���
	tnl::Vector3 target_;

	//�J�����̍��W��ۊǂ��Ă���
	tnl::Vector3 camera_trarget_savepos_;
};

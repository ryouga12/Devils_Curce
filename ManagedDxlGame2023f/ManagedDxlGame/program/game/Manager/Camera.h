//
//�J�����N���X
//


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"


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
	void SetTargetPos(tnl::Vector3 newpos) {
		target_ = newpos;
	}

private:

	//�v���C���[���^�[�Q�b�g�ɂ���
	tnl::Vector3 target_;

};


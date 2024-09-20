//------------------------------------------------------------------------------------------------------------
//
//�J�����N���X
//
//-------------------------------------------------------------------------------------------------------------


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Player;

class KonCamera final{
public:
	KonCamera();

	//�J�����̓�������
	//arg_1 : �Ǐ]������W
	//arg_2 : �}�b�v�̕� 
	//arg_3 : �}�b�v�̍��� 
	void update(const tnl::Vector3& target_pos, const int& MAP_WIDTH, const int& MAP_HEIGHT);
	void draw();

	//�^�[�Q�b�g�̍��W���擾����
	tnl::Vector3 GetTargetPos()const{
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

	//---��Ƀ}�b�v�Ŏg���J����---//

	//�v���C���[���^�[�Q�b�g�ɂ���
	tnl::Vector3 target_;

	//�J�����̍��W��ۊǂ��Ă���
	tnl::Vector3 camera_trarget_savepos_;

	//�v���C���[�̒ǐՂ���X�s�[�h
	const float TRACKING_SPPED = 0.05f;

};

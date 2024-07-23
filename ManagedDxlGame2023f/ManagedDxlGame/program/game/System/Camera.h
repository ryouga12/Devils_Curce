//
//�J�����N���X
//


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Player;

class KonCamera {
public:
	KonCamera();

	//�J�����̓�������
	//���� : �Ǐ]������W , �}�b�v�̕� , �}�b�v�̍��� 
	void update(tnl::Vector3 target_pos = {0 , 0 , 0}, int MAP_WIDTH = 0, int MAP_HEIGHT = 0);
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

	//�J�����̏��
	//�J������Ԃ�؂�ւ��ăJ�����𑀍삷��
	enum class CameraState {
		INPLAY,			//�v���C��
		DEBUG			//�f�o�b�N
	};

	//�J�����V�F�C�N���X�^�[�g������
	void StartShake(const float duration, const float magnitude);

	// �o�g���p�J�������W���擾����
	tnl::Vector3 getBattleCameraPos() const {
		return battle_camera_pos_;
	}

private:

	//---��Ƀ}�b�v�Ŏg���J����---//

	//�v���C���[���^�[�Q�b�g�ɂ���
	tnl::Vector3 target_;

	//�J�����̍��W��ۊǂ��Ă���
	tnl::Vector3 camera_trarget_savepos_;

	//---�o�g�����Ɏg���J����---//

	//�o�g�����̃J����
	tnl::Vector3 battle_camera_pos_ = { DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2 ,  0 };

	//�ۑ��p�̃J����
	tnl::Vector3 battle_camera_save_pos = { DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2 ,  0 };

	//---�J�����V�F�C�N�p�̃p�����[�^---//

	// �J�������V�F�C�N�����ǂ����������t���O
	bool is_shaking_ = false;       
	// �V�F�C�N�̌p�����ԁi�b�j
	float shake_duration_ = 0;  
	// �V�F�C�N�̋��x�i�h��̕��j
	float shake_magnitude_ = 0; 
	// �V�F�C�N���J�n���Ă���̌o�ߎ��ԁi�b�j
	float shake_elapsed_ = 0;      

	//�J�����V�F�C�N
	void CameraShake();
};

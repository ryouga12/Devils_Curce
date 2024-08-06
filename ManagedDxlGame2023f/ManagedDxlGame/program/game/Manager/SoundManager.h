#pragma once
#include<unordered_map>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"


class SoundManager final {
public:
//------------------------------------------
// �V���O���g���݌v
//�t���C�E�F�C�g�p�^�[�����g��

	static SoundManager* getSoundManager();
	static void delateSoundManager() { delete getSoundManager(); }

private:

	SoundManager();
	~SoundManager();

	//BGM�̃p�X������z��
	std::unordered_map<std::string, int> sound_map;

//------------------------------------------------------------------------------------------------------------------------
//�֐�
public:

	//�T�E���h��ǂݍ���
	//arg_1 : �T�E���h��path
	int LoadSoundBGM(const std::string& bgmFile);

	//�T�E���h���Đ�������
	//arg_1 : �T�E���h��path
	//arg_2 : �ǂ̃^�C�v�ōĐ����邩
	void sound_Play(const std::string& filepath , const int& playType );
	
	//�T�E���h����������
	//arg_1 : �T�E���h��path
	bool daleteSound(const std::string& filepath);

	//�T�E���h���~�߂�
	//arg_1 : �T�E���h��path
	void StopSound(const std::string& filepath);

	//�T�E���h�̉��ʂ�ς���
	//arg_1 : �ǂ̉��ʂɕς���̂�
	//arg_2 : �T�E���h��path
	void ChangeSoundVolume(const int& sound , const std::string& ghpath);

};
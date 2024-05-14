#pragma once
#include<unordered_map>
#include "../../dxlib_ext/dxlib_ext.h"


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
	std::unordered_map<std::string, int> soundMap;

//------------------------------------------------------------------------------------------------------------------------
//�֐�
public:

	//�T�E���h��ǂݍ���
	int LoadSoundBGM(const std::string& bgmFile);

	//�T�E���h���Đ�������
	void sound_Play(const std::string& filepath , int playType );
	
	//�T�E���h����������
	bool daleteSound(const std::string& filepath);

	//�T�E���h���~�߂�
	void StopSound(const std::string& filepath);

	//�T�E���h�̉��ʂ�ς���
	void ChangeSoundVolume(int sound , const std::string& ghpath);

};
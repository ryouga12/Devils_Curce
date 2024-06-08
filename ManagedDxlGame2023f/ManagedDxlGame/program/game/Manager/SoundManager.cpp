#include "SoundManager.h"



SoundManager* SoundManager::getSoundManager()
{
	static SoundManager* p_instance = nullptr;

	if (!p_instance)p_instance = new SoundManager();
	return p_instance;

};

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
	//���̗v�f����������
	sound_map.clear();
}

int SoundManager::LoadSoundBGM(const std::string& bgmFile)
{
	auto it = sound_map.find(bgmFile);
	if (it != sound_map.end()) {
		return sound_map[bgmFile];
	}
	else {
		int bgmHandle = LoadSoundMem(bgmFile.c_str());
		sound_map.insert(std::make_pair(bgmFile, bgmHandle));
		return bgmHandle;
	}
}

bool SoundManager::daleteSound(const std::string& filepath)
{
	// sound�������Ă��邩�m�F
	if (auto it = sound_map.find(filepath); it != sound_map.end()) {
		if (DeleteSoundMem(it->second) != -1) {
			sound_map.erase(filepath);
			return true;
		}
		else {
			tnl::DebugTrace("\ ResourceManager���\�[�X��� => %s �̃���������Ɏ��s���܂��� \n", filepath.c_str());
			return false;
		}
	}

	// �����ł��Ȃ������ꍇ
	tnl::DebugTrace("\n ResourceManager���\�[�X��� => %s ��������܂���ł��� \n", filepath.c_str());
	return false;


}

void SoundManager::StopSound(const std::string& filepath)
{
	//�f�o�b�N�g���[�X
	/*tnl::DebugTrace("----------------------------------------------------------\n");
	auto c = tnl::BeginClock();*/
	int handle = LoadSoundBGM(filepath);
	StopSoundMem(handle);
	/*float time = tnl::EndClock(c);
	tnl::DebugTrace("sound_time %f\n", time);*/
}



void SoundManager::ChangeSoundVolume(int sound, const std::string& ghpath)
{
	int sound_ = LoadSoundBGM(ghpath);
	ChangeVolumeSoundMem((255 * sound) / 100, sound_);

}



void SoundManager::sound_Play(const std::string& filepath, int playType)
{
	int sound = LoadSoundBGM(filepath);
	ChangeVolumeSoundMem(255 *  30 / 100, sound);
	PlaySoundMem(sound, playType);

}


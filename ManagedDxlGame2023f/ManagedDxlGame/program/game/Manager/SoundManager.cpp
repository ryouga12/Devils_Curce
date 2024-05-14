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
	
}

int SoundManager::LoadSoundBGM(const std::string& bgmFile)
{
	auto it = soundMap.find(bgmFile);
	if (it != soundMap.end()) {
		return soundMap[bgmFile];
	}
	else {
		int bgmHandle = LoadSoundMem(bgmFile.c_str());
		soundMap.insert(std::make_pair(bgmFile, bgmHandle));
		return bgmHandle;
	}
}

bool SoundManager::daleteSound(const std::string& filepath)
{
	// sound�������Ă��邩�m�F
	if (auto it = soundMap.find(filepath); it != soundMap.end()) {
		if (DeleteSoundMem(it->second) != -1) {
			soundMap.erase(filepath);
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
	int handle = LoadSoundBGM(filepath);
	StopSoundMem(handle);

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


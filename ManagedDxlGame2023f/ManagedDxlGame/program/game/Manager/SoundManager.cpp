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
	// soundが入っているか確認
	if (auto it = soundMap.find(filepath); it != soundMap.end()) {
		if (DeleteSoundMem(it->second) != -1) {
			soundMap.erase(filepath);
			return true;
		}
		else {
			tnl::DebugTrace("\ ResourceManagerリソース解放 => %s のメモリ解放に失敗しました \n", filepath.c_str());
			return false;
		}
	}

	// 発見できなかった場合
	tnl::DebugTrace("\n ResourceManagerリソース解放 => %s が見つかりませんでした \n", filepath.c_str());
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


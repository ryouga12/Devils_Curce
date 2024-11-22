#include "SoundManager.h"



SoundManager* SoundManager::GetSoundManager()
{
	static SoundManager* p_instance = nullptr;

	if (!p_instance)p_instance = new SoundManager();
	return p_instance;

}
void SoundManager::DelateSoundManager()
{
	delete GetSoundManager();
}

SoundManager::~SoundManager()
{
	//中の要素を消去する
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

bool SoundManager::DeleteSound(const std::string& filepath)
{
	// soundが入っているか確認
	if (auto it = sound_map.find(filepath); it != sound_map.end()) {
		if (DeleteSoundMem(it->second) != -1) {
			sound_map.erase(filepath);
			return true;
		}
		else {
			tnl::DebugTrace("\nResourceManagerリソース解放 => %s のメモリ解放に失敗しました", filepath.c_str());
			return false;
		}
	}

	// 発見できなかった場合
	tnl::DebugTrace("\nResourceManagerリソース解放 => %s が見つかりませんでした", filepath.c_str());
	return false;


}

void SoundManager::StopSound(const std::string& filepath)
{
	int handle = LoadSoundBGM(filepath);
	StopSoundMem(handle);
}



void SoundManager::ChangeSoundVolume(const int& sound, const std::string& ghpath)
{
	int sound_ = LoadSoundBGM(ghpath);
	ChangeVolumeSoundMem((koni::Numeric::VOLUME_MAX_255 * sound) / koni::Numeric::VOLUME_MAX, sound_);

}



void SoundManager::Sound_Play(const std::string& filepath, const int& playType ,const int& top_position_flag)
{
	int sound = LoadSoundBGM(filepath);
	ChangeVolumeSoundMem(koni::Numeric::VOLUME_MAX_255 * koni::Numeric::VOLUME_30_PERCENT / koni::Numeric::VOLUME_MAX, sound);
	PlaySoundMem(sound, playType ,top_position_flag);

}


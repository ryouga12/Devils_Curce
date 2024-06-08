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

bool SoundManager::daleteSound(const std::string& filepath)
{
	// soundが入っているか確認
	if (auto it = sound_map.find(filepath); it != sound_map.end()) {
		if (DeleteSoundMem(it->second) != -1) {
			sound_map.erase(filepath);
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
	//デバックトレース
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


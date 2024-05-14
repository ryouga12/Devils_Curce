#pragma once
#include<unordered_map>
#include "../../dxlib_ext/dxlib_ext.h"


class SoundManager final {
public:
//------------------------------------------
// シングルトン設計
//フライウェイトパターンを使う

	static SoundManager* getSoundManager();
	static void delateSoundManager() { delete getSoundManager(); }

private:

	SoundManager();
	~SoundManager();

	//BGMのパスを入れる配列
	std::unordered_map<std::string, int> soundMap;

//------------------------------------------------------------------------------------------------------------------------
//関数
public:

	//サウンドを読み込む
	int LoadSoundBGM(const std::string& bgmFile);

	//サウンドを再生させる
	void sound_Play(const std::string& filepath , int playType );
	
	//サウンドを消去する
	bool daleteSound(const std::string& filepath);

	//サウンドを止める
	void StopSound(const std::string& filepath);

	//サウンドの音量を変える
	void ChangeSoundVolume(int sound , const std::string& ghpath);

};
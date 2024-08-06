#pragma once
#include<unordered_map>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"


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
	std::unordered_map<std::string, int> sound_map;

//------------------------------------------------------------------------------------------------------------------------
//関数
public:

	//サウンドを読み込む
	//arg_1 : サウンドのpath
	int LoadSoundBGM(const std::string& bgmFile);

	//サウンドを再生させる
	//arg_1 : サウンドのpath
	//arg_2 : どのタイプで再生するか
	void sound_Play(const std::string& filepath , const int& playType );
	
	//サウンドを消去する
	//arg_1 : サウンドのpath
	bool daleteSound(const std::string& filepath);

	//サウンドを止める
	//arg_1 : サウンドのpath
	void StopSound(const std::string& filepath);

	//サウンドの音量を変える
	//arg_1 : どの音量に変えるのか
	//arg_2 : サウンドのpath
	void ChangeSoundVolume(const int& sound , const std::string& ghpath);

};
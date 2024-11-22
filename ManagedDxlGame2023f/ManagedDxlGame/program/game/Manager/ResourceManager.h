//------------------------------------------------------------------------------------------------------------
//
//画像などを管理するマネージャー
//
//-------------------------------------------------------------------------------------------------------------


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Effect/animation.h"
#include<unordered_map>
#include<string>
#include<vector>
#include"../Map/Mapchip.h"

//------------------------------------------
// シングルトン設計

class ResourceManager final {

public:

	static ResourceManager* GetResourceManager();
	static void DelategetResourceManager();

private:

	ResourceManager() = default;
	~ResourceManager();

public:

	//何回も同じ画像をロードしない為の関数
	int LoadGraphEX(const std::string&  gh);

	//ロードした画像を消去する
	bool DeleteGraphEx(const std::string& ghpath);

	//配列内の画像を解放する
	//arg_1 : ハンドルの配列
	//arg_2 : ハンドルの数
	void DeleteGraphAryEx(int gh_ary[], const int& max_num);

	//ロードした画像を描画する
	//arg_1 : 画像のpath
	//arg_2 : X座標
	//arg_3 : Y座標
	//arg_4 : 画像の大きさ
	//arg_5 : 角度
	//arg_6 : 透明か不透明か
	void DrawRotaGraphEx(const std::string& ghpath , const int& x , const int& y , const double& Exlate, const double& Angle, const int& TransFlag);


//------------------------------------------------------------------------------------------------------------------------	
private:
	//一度読み込んだghを保存するmap
	std::unordered_map<std::string, int> ghmap;

};
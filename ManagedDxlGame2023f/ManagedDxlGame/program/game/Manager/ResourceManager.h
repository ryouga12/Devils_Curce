//
//まとめるクラス
//


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

	static ResourceManager* getResourceManager();
	static void delategetResourceManager() { delete getResourceManager(); }

private:

	ResourceManager();
	~ResourceManager();

public:

	void Update();
	void Draw();

	//何回も同じ画像をロードしない為の関数
	int LoadGraphEX(std::string gh);

	//ロードした画像を消去する
	bool deleteGraphEx(std::string ghpath);

	//ロードした画像を描画する
	void DrawRotaGraphEx(const std::string& ghpath , int x , int y , double Exlate, double Angle, int TransFlag);


//------------------------------------------------------------------------------------------------------------------------	
private:
	//一度読み込んだghを保存するmap
	std::unordered_map<std::string, int> ghmap;

};
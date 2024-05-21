#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include<string>

class BattleScene;

class Actor {
protected:
	
	//空のポインタを用意してそれぞれで設定して処理を変える
	Shared<BattleScene>battleScene = nullptr;

public:

	//空のポインタをセットする
	void SetBattleScene(Shared<BattleScene>battle_scene) {
		battleScene = battle_scene;
	}
};
#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include<string>

class BattleScene;

class Actor {
public:

	Actor() {};
	virtual~Actor() {};

	enum class ActorType {
		NONE,
		ENEMY,

	};

	ActorType actor_type = ActorType::NONE;

	//空のポインタをセットする
	void SetBattleScene(BattleScene* battle_scene) {
		battleScene = battle_scene;
	}

protected:
	
	//空のポインタを用意してそれぞれで設定して処理を変える
	//Sheadポインターにする場合、weakポインターにする必要がある
	BattleScene* battleScene = nullptr;

};
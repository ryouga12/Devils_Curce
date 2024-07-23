#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include<string>

class BattleScene;
class EventManager;

//アニメーションを切り替える秒数
static const float ChangeAnimationTime = 0.2f;
//フレーム
static const int animationFlame = 3;

class Actor {
public:

	Actor() {};
	virtual~Actor() {};

	virtual void Update(float delta_time) {};
	virtual void Draw(const KonCamera& camera) {};

	enum class ActorType {
		NONE,
		ENEMY,

	};

	//空のポインタをセットする
	void SetBattleScene(BattleScene* battle_scene) {
		battle_scene_ = battle_scene;
	}

	//バトルシーンのポインタを解放する
	/*void DeleteBattleScene() {
		delete battle_scene;
	}*/

protected:
	
	//空のポインタを用意してそれぞれで設定して処理を変える
	//Sheadポインターにする場合、weakポインターにする必要がある
	BattleScene* battle_scene_ = nullptr;

	//黒
	const int COLOR_BRACK = 0;

	//白色
	const int COLOR_WHITE = -1;

};
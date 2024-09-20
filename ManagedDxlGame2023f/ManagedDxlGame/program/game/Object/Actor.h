//------------------------------------------------------------------------------------------------------------
//
//ゲーム内のオブジェクトの基底クラス(PlayerやNpcなど)
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include<string>
#include"../../koni_name_space/common/common_value.h"

class BattleScene;
class EventManager;

class Actor {
public:

	Actor() = default;
	virtual~Actor()= default;

	virtual void Update(float delta_time) {};
	virtual void Draw(const KonCamera& camera) {};


	//空のポインタをセットする
	void SetBattleScene(BattleScene* battle_scene) {
		battle_scene_ = battle_scene;
	}


protected:
	
	//空のポインタを用意してそれぞれで設定して処理を変える
	//Sheadポインターにする場合、weakポインターにする必要がある
	BattleScene* battle_scene_ = nullptr;


	//---アニメーションで使うフレーム関連---//

	//総フレーム(3)
	const int TOTAL_FRAME = 3;							
	//横フレーム(3)
	const int HORIZONTAL_FRAME = 3;						
	//縦フレーム(1)
	const int VERTICAL_FRAME = 1;						
	//横方向へ 1フレームあたりの幅(48)
	const int HORIZONTAL_ONEFRAME = 48;					
	//縦方向へ 1フレームあたりの幅(48)
	const int VERTICAL_ONEFRAME = 48;					

	//アニメーションを切り替える秒数
	static constexpr float ChangeAnimationTime = 0.3f;
	//フレーム
	static constexpr int animationFlame = 3;


};
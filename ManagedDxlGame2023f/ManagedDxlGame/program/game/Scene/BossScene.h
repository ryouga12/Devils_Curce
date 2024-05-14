#pragma once
#include"../Scene/battleScene.h"
#include"../Manager/GameManager.h"
#include "../../dxlib_ext/dxlib_ext.h"

//-----------------------------------------------------------------------------------------
//
//ボスでの戦闘シーン
//
//------------------------------------------------------------------------------------------

class Player;
class Enemy;
class BattleLog;

class BossScene : public BattleScene {
public:

	BossScene();
	~BossScene()override;

	//更新処理
	void Update(float delta_time)override;

	//描画
	void Draw()override;

	//バトルシーン内で切り替える関数
	void MenuUpdate(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//Inventoryを表示させる
	/*void InventoryItemDraw()override;*/

private:

	//------------------------------------------------------------------------------------------------------------------
	//スマートポインタ

	//バトルログ
	Shared<BattleLog>battle_log = nullptr;
	//敵のポインタ
	Shared<Enemy>enemy = nullptr;



};
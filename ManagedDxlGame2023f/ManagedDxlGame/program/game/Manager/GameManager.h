#pragma once
///
///システム面を扱うマネージャー
/// 

#include"../Menu/MenuWindow.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Item/Inventory.h"
#include"../Skill/Skill.h"
#include"../Object/Player.h"


class GameManager {
public:
//------------------------------------------
// シングルトン設計
//フライウェイトパターンを使う

	static GameManager* getGameManager();
	static void delateGameManager() { delete getGameManager(); }

private:

	GameManager();
	~GameManager();

public:

	//秒数を数える
	bool TimeCount(float delta_time, float Time);

	//描画
	void draw();

private:

	//時間
	float count_Time = 0;


//------------------------------------------------------------------------------------------------------------------------
//---武器関連---//



//------------------------------------------------------------------------------------------------------------------------
//---アイテム関連---//
public:

	//ランダムな値をセットする（今後使う予定）
	int GetRandEx(int a, int b);

//------------------------------------------------------------------------------------------------------------------------
//---ポインタ---//
private:

	Shared<Inventory>inventory = nullptr;
	Shared<Player>player = nullptr;
	Shared<KonCamera>camera = nullptr;
	Shared<Skill>skill = nullptr;


//------------------------------------------------------------------------------------------------------------------------
//メニュー関連
public:
	
	//話すコマンドの処理
	void displayDialogue();

//------------------------------------------------------------------------------------------------------------------------
// 共通処理やインベントリの更新

public:

	//更新処理
	void UpdateCommon(float delta_time) {
		inventory->Update(delta_time);
		player->Update(delta_time);
	}

	//ポインタの取得

	//インベントリ
	const Shared<Inventory>& getInventory(){
		return inventory;
	}
	//プレイヤー
	const Shared<Player>& getPlayer() {
		return player;
	}
	//カメラ
	const Shared<KonCamera>& getCamera() {
		return camera;
	}
	//スキル
	const Shared<Skill>& getSkill() {
		return skill;
	}

};



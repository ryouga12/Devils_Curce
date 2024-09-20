#pragma once
//------------------------------------------------------------------------------------------------------------
//
//共通で使う物の管理する中間役の役割のマネージャー
//
//-------------------------------------------------------------------------------------------------------------


#include"../Menu/MenuWindow.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include "../Item/Inventory.h"

class Player;
class Skill;

class GameManager final{
public:
//------------------------------------------
// シングルトン設計

	static GameManager* GetGameManager();
	static void DelateGameManager();

private:

	GameManager()= default;
	~GameManager()= default;

public:

	//秒数を数える
	bool TimeCount(float delta_time, float Time);

	//ポインタを生成する
	void PoiterCreate();

	//ポインタをリセットする
	void Reset();

private:

	//時間
	float count_time = 0;

//------------------------------------------------------------------------------------------------------------------------
//---アイテム関連---//
public:

	//ランダムな値をセットする（今後使う予定）
	int GetRandEx(int a, int b);

//------------------------------------------------------------------------------------------------------------------------
//---ポインタ---//
private:

	//インベントリ
	Shared<Inventory>inventory = nullptr;
	//プレイヤー
	Shared<Player>player = nullptr;
	//カメラ
	Shared<KonCamera>camera = nullptr;

//------------------------------------------------------------------------------------------------------------------------
// 共通処理やインベントリの更新

public:

	//---ポインタの取得---//

	//インベントリ
	const Shared<Inventory>& GetInventory(){
		return inventory;
	}
	//プレイヤー
	const Shared<Player>& GetPlayer() {
		return player;
	}
	//カメラ
	const Shared<KonCamera>& GetCamera() {
		return camera;
	}

};



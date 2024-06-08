//------------------------------------------------------------------------------------------------------------
//
//イベントを管理するマネージャー(主にトリガーの役割)
// 
//
//-------------------------------------------------------------------------------------------------------------



#pragma once

#include"../Manager/ResourceManager.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/GameManager.h"
#include<unordered_map>

class UIManager;

class EventManager final {

public:

	static EventManager* getEventManager();
	static void delateEventManager() { delete getEventManager(); }

	//メニューの使い分け
	enum class MenuOpen {
		EMPTY,
		WEAPONSHOP,			//武器屋
		//HERDMAN,			//村長
		//PRIEST,				//神官
		BOSS,				//ボスモンスター
	};

	//Npcによって動作を変える
	enum class NpcMove {
		EMPTY,				//空
		HERDMAN,			//村長
		PRIEST,				//神官
		WEAPONSHOP,			//武器屋
		BOSS				//ボスモンスター
	};

private:

	EventManager();

	~EventManager();

	//メニュー画面で表示するアイコン的なもの
	int Armsdealer_icon_hdl = 0;
	int herdman_icon_hdl = 0;
	int Priest_icon_hdl = 0;
	int Player_icon_hdl = 0;

	//どのメニューを開いているか
	MenuOpen select_menu = MenuOpen::EMPTY;

	//誰が動作を行うか
	NpcMove npcMove = NpcMove::EMPTY;

	//---村長---//

	//村長のアイコンの座標
	const tnl::Vector2i HERDMAN_ICON_POS{ 110 , 600 };

	//村長のコメントの座標
	const tnl::Vector2i HERDMAN_COMENT_POS = { 150 , 580 };

	//村長のサイズ
	const float HERDMAN_SCALE = 1.5f;

	//ボスモンスターのコメント座標
	const tnl::Vector2i BOSS_COMENT_POS = { 150 , 580 };

	//ボスの表示するコメント数
	const int BOSS_COMENT_MAX_NUM = 4;

	//文字列アニメーションを何文字表記ずつ表示するか
	int add_num = 1;

	//行数
	int line = 0;
	
	//文字数を制限する
	int count = 0;

	//文字列を格納する配列
	std::string text[2];

	//文字列アニメーションを行ったかのフラグ
	bool coment_animation_flag = false;

public:

	//menu関連の初期化
	void InitMenu();

	//npcのDrawを切り替える
	void EventMenuWindow();

	//メニューの切替
	void SetMenuNpc(MenuOpen new_menu) {
		select_menu = new_menu;
	}

	//現在のメニューを取得する
	MenuOpen GetSelectMenu()const {
		return select_menu;
	}

	//Npcのコメントを表示する
	void NpcSwitchComent();

	//NPCのコメントの切り替え
	void SetNpcComent(NpcMove npc) {
		npcMove = npc;
	}

	//現在のNPCの状態をを取得する
	NpcMove GetNpcMove()const {
		return npcMove;
	}
	
	//コメントを流し切ってからEnterキーでシーンを遷移させる
	bool ComentBattleChange(const tnl::Vector2i& coment_pos, const int count_max);

private:

	//白色
	const int COLORWHITE = -1;

	//武器屋に話かけたときのコメント
	const int ARMSDEALER_SPEAK_COMENT = 0;

	//神官のコメント
	std::string priest_coment[2] = {"よく来たな","記録していくか？"};

	//神官のコメントの座標
	const tnl::Vector2i PRIEST_COMENT_POS = { 150, 570 };

	//ボスの背景
	int boss_background_hdl = 0;


//--------------------------------------------------------------------------------------------------------
//ポインタ
private:

	//選択メニューウィンドウ
	Weak<Menu>menu_window;

};
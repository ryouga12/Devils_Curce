//------------------------------------------------------------------------------------------------------------
//
//イベントを管理するマネージャー(主にトリガーの役割)
// NPCなどの会話が行われた際に処理や描画などを任せる
//
//-------------------------------------------------------------------------------------------------------------



#pragma once

#include"../Manager/ResourceManager.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/GameManager.h"
#include<unordered_map>


class UIManager;
class SceneManager;
class WeaponShop;
class EventNpc;

class EventManager final {

public:

	static EventManager* getEventManager();
	static void delateEventManager() { delete getEventManager(); }

	//メニューの使い分け
	enum class MenuOpen {
		EMPTY,
		WEAPONSHOP,			//武器屋
		//HERDMAN,			//村長
		//PRIEST,			//神官
		BOSS,				//ボスモンスター
	};

	//Npcによって動作を変える
	enum class NpcMove {
		EMPTY,				//空
		HERDMAN,			//村長
		PRIEST,				//神官
		WEAPONSHOP,			//武器屋
		TOWNHERDMAN,		//町長
		INN,				//宿屋
		ENEMY,				//敵
		BOSS,				//ボスモンスター
		KING,				//王
		QUEEN,				//王妃
		FEMALE_RESIDENT,	//女性村人
		RESIDENT_MALE		//男性村人
	};

private:

	EventManager();

	~EventManager();

	//メニュー画面で表示するアイコン的なもの

	//武器商人
	int Armsdealer_icon_hdl = 0;
	//村長
	int herdman_icon_hdl = 0;
	//神官
	int Priest_icon_hdl = 0;
	//町長
	int town_herdman_icon_hdl = 0;
	//宿屋
	int inn_icon_hdl = 0;
	//ボス
	int boss_icon_hdl = 0;
	//敵
	int enemy_icon_hdl = 0;
	//王
	int king_icon_hdl = 0;
	//王妃
	int queen_icon_hdl = 0;
	//女性村人
	int female_resident_icon_hdl = 0;
	//男性村人
	int male_villagers_icon_hdl = 0;

	//どのメニューを開いているか
	MenuOpen select_menu = MenuOpen::EMPTY;

	//誰が動作を行うか
	NpcMove npcMove = NpcMove::EMPTY;

	//ボスのID
	const int BOSS_ID = 20;

	//中ボスのID
	const int MEDIUM_BOSS = 21;

	//宿屋の費用
	const int INNMONEY = 5;

	//---コメント表示---//

	//村長のアイコンの座標
	const tnl::Vector2i COMENT_ICON_POS{ 110 , 600 };

	//村長のコメントの座標
	const tnl::Vector2i COMENT_POS = { 150 , 580 };

	//NPCのサイズ
	const float NPC_SCALE = 1.5f;

	//ボスの表示するコメント数
	const int BOSS_COMENT_MAX_NUM = 4;

	//文字列アニメーションを行ったかのフラグ
	bool coment_animation_flag = false;

public:

	//初期化
	void InitEventRelated(std::list<Shared<Actor>>& npc_list);

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
	void NpcSwitchComent(const Shared<WeaponShop>& weapon_shop);

	//Npcのそれぞれでの更新処理
	void NpcEventUpdate(float delta_time, InMapScene* inmap_scene, int curent_map_state, std::list<Shared<Actor>>& npc_list);

	//NPCのコメントの切り替え
	void SetNpcComent(NpcMove npc) {
		npcMove = npc;
	}

	//現在のNPCの状態をを取得する
	NpcMove GetNpcMove()const {
		return npcMove;
	}
	
	//コメントを流し切ってからEnterキーでシーンを遷移させる
	bool ComentEventChange(const int count_max);

	//プレイヤーに行動を選択させる
	void PlayerAnswerSelect(int curent_map_state , InMapScene* inmap_scene, const std::list<Shared<Actor>>& npc_list);

	//イベントフラグを切り替える
	void EventFlagChange() { event_npc_flag = !event_npc_flag; }

	//イベントフラグを取得する
	bool GetEventFlag()const { return event_npc_flag; }

	//敵のイベントフラグを取得する
	//フラグを管理して中ボスなどを生成する
	bool GetEnemyEventFlag()const { return enemy_event_flag; }

	//敵のフラグを切り替える
	//イベントを終了したらフラグを切り替えて終了させる
	void EnemyEventFlagChange() { enemy_event_flag = !enemy_event_flag; }

	//コメントを表示する
	void ComentEvent(InMapScene* inmap_scene, const int coment_max, const std::list<Shared<Actor>>& npc_list);

private:

	//白色
	const int COLORWHITE = -1;

	//武器屋に話かけたときのコメント
	const int ARMSDEALER_SPEAK_COMENT = 0;

	//神官のコメントの座標
	const tnl::Vector2i PRIEST_COMENT_POS = { 150, 570 };

	//ボスの背景
	int boss_background_hdl = 0;

	//選択コメント座標
	const tnl::Vector3 PLAYER_ANSWER_POS = { 80 , 100 , 0 };

	//Npcの処理毎の挙動を制限する
	//trueの場合のみイベントを発生させる
	bool event_npc_flag = true;

	//敵のイベント管理フラグ
	bool enemy_event_flag = true;

//--------------------------------------------------------------------------------------------------------
//ポインタ
private:

	//メニューウィンドウ
	Weak<Menu>menu_window;
	
	//プレイヤーの選択ウィンドウ
	Shared<MenuWindow>player_select_window = nullptr;
	//ウィンドウのコメント
	MenuWindow::MenuElement_t* player_select_coment = nullptr;

	//イベント用の関数
	Shared<EventNpc>function_npc = nullptr;

};
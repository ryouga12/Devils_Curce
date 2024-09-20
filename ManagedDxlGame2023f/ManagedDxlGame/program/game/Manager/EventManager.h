//------------------------------------------------------------------------------------------------------------
//
//イベントを管理するマネージャー(主にトリガーの役割)
//
//-------------------------------------------------------------------------------------------------------------



#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"
#include<unordered_map>
#include"../Menu/MenuWindow.h"

class WeaponShop;
class Event;
class BaseScene;
class Npc;

class EventManager final {

public:

	static EventManager* GetEventManager();
	static void DelateEventManager();

	//メニューの使い分け
	enum class MenuOpen {
		EMPTY,
		//武器屋
		WEAPONSHOP,			
		//ボスモンスター
		BOSS,				
	};

	//Npcによって動作を変える
	enum class NpcMove {
		//空
		EMPTY,				
		//村長
		HERDMAN,			
		//神官
		PRIEST,				
		//武器屋
		WEAPONSHOP,			
		//町長
		TOWNHERDMAN,		
		//宿屋
		INN,
		//敵
		ENEMY,				
		//影の番人
		SHADOWENEMY,		
		//ボスモンスター
		BOSS,
		//王
		KING,
		//王妃
		QUEEN,				
		//女性住人
		FEMALE_RESIDENT,	
		//男性住人
		RESIDENT_MALE,		
		//学者
		SCHOLAR,			
		//試練の番人
		ORDEAL				
	};

private:

	EventManager();

	~EventManager()= default;

	//メニュー関連の初期化
	void InitMenuEventConnection();

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
	//女性住人
	int female_resident_icon_hdl = 0;
	//男性住人
	int resident_male_icon_hdl = 0;
	//学者
	int scholar_icon_hdl = 0;
	//試練の番人
	int ordeal_icon_hdl = 0;
	//影の番人
	int shadow_icon_hdl = 0;

	//どのメニューを開いているか
	MenuOpen select_menu = MenuOpen::EMPTY;

	//誰が動作を行うか
	NpcMove npc_move = NpcMove::EMPTY;

	//ボスのID
	int boss_id = 0;

	//宿屋の費用
	const int INN_MONEY = 5;

	//必須アイテムID
	int ESSENTIAL_ITEM_ID = 31;

	//---コメント表示---//

	//アイコンの座標
	const tnl::Vector2i COMENT_ICON_POS{ 110 , 600 };

	//コメントの座標
	const tnl::Vector2i COMENT_POS = { 150 , 580 };

	//文字列アニメーションを行ったかのフラグ
	bool coment_animation_flag = false;

	//コメントウィンドウの座標
	const tnl::Vector2i COMENT_WINDOW_POS = { 50, 500 };

	//コメントウィンドウのサイズ
	const int COMENT_WINDOW_WIDTH = 700;
	const int COMENT_WONDOW_HEIGHT = 200;


public:

	//初期化
	//arg_1 : NPCクラスのリスト
	//NPCによってアイコンの初期化及び、std::functionによる処理の初期化、メニュー関連のUI初期化
	void InitEventRelated(std::list<Shared<Npc>>& npc_list);


	//メニューの切替
	void SetMenuNpc(const MenuOpen& new_menu) {
		select_menu = new_menu;
	}

	//現在のメニューを取得する
	MenuOpen GetSelectMenu()const {
		return select_menu;
	}

	//Npcのコメントを表示する
	//arg_1 : weapon_shopのポインタ
	//それぞれのNPCのコメント描画(weapon_shopのポインタはクラスの描画処理を呼ぶ出す為)
	void NpcSwitchComent(const Shared<WeaponShop>& weapon_shop = nullptr);

	//Npcのそれぞれでの更新処理
	//arg_1  : 　delta_time 
	//arg_2  :　NPCの配列 (配列はそれぞれのNPCの名前を参照して、処理を変える為に使用)
	//arg_3　:　BaseSceneのポインタ(それぞれのシーケンスに触る為、ポインタを使用する)
	//arg_4  :　InMapScene内の現在のシーン(今どのシーンにいるか確認する為に使用, セーブする為に使用し、InMapScene内のみで使用)
	void NpcEventUpdate(float delta_time, const std::list<Shared<Npc>>& npc_list , BaseScene* curent_map_scene, const int& curent_inmap_scene = 0);

	//NPCのコメントの切り替え
	void SetNpcComent(const NpcMove& npc) {
		npc_move = npc;
	}
	
	//プレイヤーに行動を選択させる
	//arg_1 : NPCクラスのリスト(配列はそれぞれのNPCの名前を参照して、処理を変える為に使用)
	//arg_2 : BaseSceneのポインタ(それぞれのシーケンスに触る為、ポインタを使用する)
	//arg_3 : InMapScene内の現在のシーン(今どのシーンにいるか確認する為に使用, セーブする為に使用し、InMapScene内のみで使用)
	void PlayerAnswerSelect(const std::list<Shared<Npc>>& npc_list ,BaseScene* curent_map_scene , const int& inmap_curent_state = 0);

	//イベントフラグを切り替える
	void EventFlagChange() { event_npc_flag = !event_npc_flag; }

	//イベントフラグを取得する
	bool GetEventFlag()const { return event_npc_flag; }

	//敵のイベントフラグを取得する
	//フラグを管理して中ボスなどを生成する
	//一体目のボス
	bool GetEnemyEventFlag_1()const { return enemy_event_flag_1; }

	//主にセーブを行うために使用
	void SetEnemyEventFlag_1(const bool& save_flag) { enemy_event_flag_1 = save_flag; }

	//敵のフラグを切り替える
	//イベントを終了したらフラグを切り替えて終了させる
	//一体目のボス
	void EnemyEventFlag_1Change() { enemy_event_flag_1 = !enemy_event_flag_1; }

	//敵のイベントフラグを取得する
	//フラグを管理して中ボスなどを生成する
	//二体目のボス
	bool GetEnemyEventFlag_2()const { return enemy_event_flag_2; }

	//主にセーブを行うために使用
	void SetEnemyEventFlag_2(const bool& save_flag) { enemy_event_flag_2 = save_flag; }

	//敵のフラグを切り替える
	//イベントを終了したらフラグを切り替えて終了させる
	//二体目のボス
	void EnemyEventFlag_2Change() { enemy_event_flag_2 = !enemy_event_flag_2; }

	//敵のイベントフラグを取得する
	//フラグを管理して中ボスなどを生成する
	//三体目のボス
	bool GetEnemyEventFlag_3()const { return enemy_event_flag_3; }

	//主にセーブを行うために使用
	void SetEnemyEventFlag_3(const bool& save_flag) { enemy_event_flag_3 = save_flag; }

	//敵のフラグを切り替える
	//イベントを終了したらフラグを切り替えて終了させる
	//三体目のボス
	void EnemyEventFlag_3Change() { enemy_event_flag_3 = !enemy_event_flag_3; }

	//コメントを表示する
	void ComentEvent(BaseScene* curent_scene, const int& coment_max, const std::list<Shared<Npc>>& npc_list);

	//コメントを流しきった後アイテムを所持しているかによってtrueかfalseを返す
	//シナリオで必須アイテムが存在するのでそのアイテムが存在するかを確認して処理を変える
	//arg_1 : 必須アイテムのID
	//arg_2 : 消去するアイテムかどうか
	bool CheckEventItem(const int& essential_items , const int& remove_need_flag = 1);

	//選択肢があるイベントの処理分け
	//NPCによって処理を変える
	void ProcessNpcEventByType(const std::list<Shared<Npc>>& npc_list, BaseScene* curent_map_scene , const int& inmap_curent_state);

	//イベントフラグのリセット
	void EventFlagReset() {
		enemy_event_flag_1 = true, enemy_event_flag_2 = true, enemy_event_flag_3 = true;
	}

	//試練のボスのコメントフラグのリセット
	void OrdealComentFlagReset() {
		ordeal_coment_flag = false;
	}

private:

	//ボスの背景
	int boss_background_hdl = 0;

	//Npcの処理毎の挙動を制限する
	//trueの場合のみイベントを発生させる
	bool event_npc_flag = true;

	//必須アイテムをチェックするボスのコメントフラグ
	bool ordeal_coment_flag = false;

	//敵のイベント管理フラグ
	//一体目のボス
	bool enemy_event_flag_1 = true;

	//敵のイベント管理フラグ
	//2体目のボス
	bool enemy_event_flag_2 = true;

	//敵のイベント管理フラグ
	//3体目のボス
	bool enemy_event_flag_3 = true;


//--------------------------------------------------------------------------------------------------------
//ポインタ
private:

	//プレイヤーの選択ウィンドウ
	Shared<MenuWindow>player_select_window = nullptr;
	//ウィンドウのコメント
	MenuWindow::MenuElement_t* player_select_coment = nullptr;

	//イベント用の関数
	Shared<Event>function_npc = nullptr;
};
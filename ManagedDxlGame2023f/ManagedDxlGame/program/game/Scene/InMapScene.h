//------------------------------------------------------------------------------------------------------------
//
//マップに設置してある村や町などのシーン
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include	<unordered_set>
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Object/NPC.h"
#include"../Manager/SoundManager.h"
#include"../Manager/EventManager.h"
#include"../../koni_name_space/common/common_value.h"

class WeaponShop;

class InMapScene final : public BaseScene {
public:
	
	//InMapSceneの状態
	enum class InMapState {
		//村
		VILLAGE,		
		//町
		TOWN,			
		//城下町
		CASTLETOWN,		
		//城
		CASTLE,			
		//ボスの城(ボスの場所までの道,主に階段など)
		BOSSCASTLE,     
		//ボスの間
		BOSSROOM,		
		//墓
		GRAVE,
		//船の中
		SHIP
	};

	InMapScene() = default;

	//InMapSceneのコンストラクタ
	//arg_1 : どの座標にプレイヤーをセットするか
	//arg_2 : どの村などに入ったのか
	InMapScene(const tnl::Vector3& firstPos, const InMapState& inmap_state);
	~InMapScene()override;

	//更新処理
	void Update(float delta_time)override;

	//描画処理
	void Draw()override;

	//InMapSceneでの初期化
	void InitInMapScene();

	//InMapSceneの更新処理
	void InMapSceneUpdate(float delta_time);

	//InMapSceneの描画処理
	void InMapSceneDraw();

	//村シーンでの初期化
	void InitVillageScene();

	//ボスの城シーンの初期化
	void InitBossCastleScene();

	//ボスの間シーンの初期化
	void InitBossRoomScene();

	//町シーンの初期化
	void InitTownScene();

	//城下町シーン初期化
	void InitCastleTownScene();

	//城シーンの初期化
	void InitCastleScene();

	//墓シーンの初期化
	void InitGraveScene();

	//船シーンの初期化
	void InitShipScene();
	
	//メニューウィンドウの生成
	void InitMenuWindow();

	//マップ外に出た際の処理
	void InMapSceneExitProcess(const float delta_time);
	
	//現在のシーンを取得する
	InMapState GetCurentMapState()const {
		return curent_inmap_scene;
	}

	//シーンをセットする
	void SetInMapScene(InMapState new_map_state) {
		curent_inmap_scene = new_map_state;
	}

private:

	//現在のシーン : 最初は村から始まる為VILLAGE
	InMapState curent_inmap_scene = InMapState::VILLAGE;

	//時間経過で切り替えるフラグ
	bool TimeCount = false;

	//遅延させる秒数
	float TimeSE = 0.2f;


//------------------------------------------------------------------------------------------------------------------------
//MapChip関連

private:

	//csvを格納しておく用のvector(床用)
	std::vector<std::vector<int>>map_csv_;

	//オブジェクト用
	std::vector<std::vector<int>>map_csv_objct_;

	//---mapChip関連の処理---//
	
	//mapChipの読み込み関数
	void mapChip_Loding(std::vector<std::vector<int>>map_csv, std::vector<std::vector<int>>map_object_csv );

	//当たり判定
	void mapChip_collision();																				
	
	//村の最初の座標
	tnl::Vector3 first_pos = {};	

	//worldMapでの最初の位置
	const tnl::Vector3 FIRST_WORLD_POS = { 334 , 1114 , 0 };

	//ボスの城から出たときにの座標
	const tnl::Vector3 BOSS_EXIT_POS = {3039 , 1145 , 0};													

	//ボスの間に入った時の座標
	const tnl::Vector3 BOSS_ROOM_FIRS_POS = { 805 , 895 , 0 };												

	//ボスの間から出たときの座標
	const tnl::Vector3 BOSS_ROOM_EXIT_POS = { 766 , 103 , 0 };		

	//町から出た時の座標
	const tnl::Vector3 TOWN_EXIT_POS = { 1536, 1195 , 0 };				

	//城下町から出た時の座標
	const tnl::Vector3 CASTLE_TOWN_EXIT_POS = { 1429, 155 , 0 };

	//城に入った時の最初の位置
	const tnl::Vector3 CASTLE_FIRST_POS = { 1111,1674 , 0 };	

	//城から城下町に出た時の座標
	const tnl::Vector3 CASTLE_EXIT_POS = { 1114 , 100 , 0 };	

	//勇者の墓から出た時の座標
	const tnl::Vector3 GRAVE_EXIT_POS = { 2625 , 98 , 0 };		

	//船から出たときの座標
	const tnl::Vector3 SHIP_EXIT_POS = { 138 , 1032 , 0 };

	//村シーンでの当たり判定
	void VillageCollision();

	//町シーンでの当たり判定
	void TownCollision();

	//ボスの城での当たり判定
	void BossCastleCollision();

	//ボスの間での当たり判定
	void BossRoomCollision();

	//城下町シーンの当たり判定
	void CastleTownCollision();

	//城シーンの当たり判定
	void CastleCollision();

	//勇者の墓シーンの当たり判定
	void GraveSceneCollision();

	//船に入った際の当たり判定
	void ShipCollision();

//------------------------------------------------------------------------------------------------------------------------
//---ポインタ---//

	//Npc関連
	
	//村長　||　町長
	Shared<Npc>herdman_npc = nullptr;

	//武器商人
	Shared<Npc>armsdealer_npc = nullptr;

	//神官
	Shared<Npc>priest_npc = nullptr;

	//影の番人
	Shared<Npc>shadow_npc = nullptr;

	//ボス
	Shared<Npc>boss_npc = nullptr;

	//武器屋
	Shared<WeaponShop>weapon_shop = nullptr;

	//宿屋
	Shared<Npc>inn_npc = nullptr;

	//住人(女性)
	Shared<Npc>female_resident_npc = nullptr;

	//住人(男性)
	Shared<Npc>resident_male_npc = nullptr;

	//兵士
	Shared<Npc>soldier_npc = nullptr;

	//兵士二人目
	Shared<Npc>second_soldier_npc = nullptr;

	//敵
	Shared<Npc>enemy_npc = nullptr;

	//王
	Shared<Npc>king_npc = nullptr;

	//王妃
	Shared<Npc>queen_npc = nullptr;

	//当たり判定のないNpc
	Shared<Npc>ordeal_npc = nullptr;

	//兵長
	Shared<Npc>corporal_npc = nullptr;

	//船長
	Shared<Npc>pirate_npc = nullptr;

	//僧侶(ラスボス後)
	Shared<Npc>monk_npc = nullptr;

	//---メニュー関連---//

	//話すコマンド
	Shared<MenuWindow>dia_log_coment_select = nullptr;
	MenuWindow::MenuElement_t* dia_log_coment = nullptr;

	//武器商人の購入コマンド
	Shared<MenuWindow>weapon_shop_coment_select = nullptr;
	MenuWindow::MenuElement_t* weapon_shop_coment = nullptr;

//------------------------------------------------------------------------------------------------------------------------
//---シーケンス---//

public:

	//シーケンスを管理するクラス
	enum class Sequence_num {
		//初期化
		INIT,
		//行動時
		MOVE,
		//武器屋
		WEAPONSHOP,
		//メニューを開いている時
		MENUOPEN,
		//npcと話している時
		MENUNPC
	};


	//他の所でシーケンスを切り替える
	void SequenceChangeMove()override;

	//どのシーケンスが選択されているか
	 Sequence_num select_num = Sequence_num::INIT;

	//シーケンスの定義
	tnl::Sequence<InMapScene> sequence_ = tnl::Sequence<InMapScene>(this, &InMapScene::seqInit);

	//マップを初期化する
	bool seqInit(float delta_time);

	//通常時のシーケンス
	bool seqMove(float delta_time);

	//武器屋のシーケンス
	bool seqWeaponShop(float delta_time);

	//メニューシーケンス
	bool seqMenuOpen(float delta_time);

	//シーンを変える為のシーケンス
	bool seqChangeScene(float delta_time);

	//Npcのシーケンス
	bool seqNpc(float delta_time);

	//シーケンスを切り替えた時の処理
	void switchSequenceDraw();

//------------------------------------------------------------------------------------------------------------------------
//---Npc関係---//	
private:

	//NPCのチップサイズ
	const int NPC_CHIP_SIZE = 48;

	//----NPCの座標系----//

	//---村長---//
	const tnl::Vector3 HERDMAN_POS = { 1375, 760, 0 };

	//---僧侶---//
	const tnl::Vector3 MONK_POS = { 909,235,0 };


	//---武器商人---//
	
	//村
	const tnl::Vector3 ARMSDEALER_VILLAGE_POS = { 1150, 140, 0 };
	//町
	const tnl::Vector3 ARMSDEALER_TOWN_POS = { 1730, 580, 0 };
	//城下町
	const tnl::Vector3 ARMSDEALER_CASTLE_TOWN_POS = { 495, 329, 0 };

	//---神官---//
	 
	//村
	const tnl::Vector3 PRIEST_VILLAGE_POS = { 239, 740, 0 };
	//城下町
	const tnl::Vector3 PRIEST_CASTLE_TOWN_POS = { 336, 682, 0 };

	//---村人---//

	//男性村人
	const tnl::Vector3 MALE_VILLAGERS_POS = { 1475, 360, 0 };
	//女性村人
	const tnl::Vector3 FEMALE_VILLAGERS_POS = { 585, 667, 0 };

	//---敵関係---//

	//中ボス
	const tnl::Vector3 ENEMY_POS = { 2032, 205, 0 };
	//中ボス3体目(影の番人)
	const tnl::Vector3 SHADOW_ENEMY_POS = { 769 , 591 , 0 };
	//ラスボス
	const tnl::Vector3 LAST_BOS_POS = { 801, 153, 0 };

	//---宿屋---//

	//町
	const tnl::Vector3 INN_TOWN_POS = { 780, 1062, 0 };
	//城下町
	const tnl::Vector3 INN_CASTLE_TOWN_POS = { 1814, 678, 0 };

	//---町長---//
	const tnl::Vector3 TOWN_HERDMAN_POS = { 220, 266, 0 };

	//---町民---//

	//男性町民
	const tnl::Vector3 TOWNS_PEOPLE_POS = { 1726, 1084, 0 };

	//---城下町の住人---//

	//男性住人
	const tnl::Vector3 MALE_RESIDENT_POS = { 1644, 914, 0 };

	//---城関連---//

	//王
	const tnl::Vector3 KING_POS = { 944, 315, 0 };
	//王妃
	const tnl::Vector3 QUEEN_POS = { 1261, 315, 0 };
	//学者
	const tnl::Vector3 SCHOLAR_POS = { 800, 1492, 0 };
	//兵士
	const tnl::Vector3 SOLDIER_POS = { 972, 1060, 0 };
	//兵士二人目
	const tnl::Vector3 SECOND_SOLDIER_POS = { 1265, 1063, 0 };
	//兵長
	const tnl::Vector3 CORPORAL_POS = { 1957 , 1531 , 0 };

//------------------------------------------------------------------------------------------------------------------------
//メニュー関連座標系
private:

	//ダイアログの座標
	const tnl::Vector2i DIALOG_POS{ 50 , 50 };

	//ダイアログのサイズ
	const tnl::Vector2i DIALOG_SIZE = { 250 , 200 };

	//武器屋コマンドのサイズ
	const tnl::Vector2i WEAPON_SHOP_WINDOW_SIZE = { 250 , 230 };

//------------------------------------------------------------------------------------------------------------------------
//プレイヤー関係
private:

	//プレイヤーの速度
	const float PLAYER_VELOCITY = 4.0f;

};
//
//マップに設置してある村や町などを管理するクラス
//


#pragma once
#include	<unordered_set>
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Object/NPC.h"
#include"../Manager/SoundManager.h"
#include"../Manager/EventManager.h"

class UIManager;
class CsvManager;
class WeaponShop;

class InMapScene : public BaseScene {
public:
	
	//InMapSceneの状態
	enum class InMapState {
		VILLAGE,		//村
		TOWN,			//町
		CASTLETOWN,		//城下町
		CASTLE,			//城
		BOSSCASTLE		//ボスの城
	};

	InMapScene(tnl::Vector3 firstPos , InMapState inmap_state);
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
	//ダンジョンシーンの初期化
	void InitDungeonScene();
	//町シーンの初期化
	void InitTownScene();
	//城下町シーン初期化
	void InitCastleTownScene();
	//城シーンの初期化
	void InitCastleScene();
	
	//メニューウィンドウの生成
	void InitMenuWindow();
	
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
public:

	//マップチップを格納する場所
	std::list<Shared<MapChip>> MapChips;
	//オブジェクト用のマップチップ
	std::list<Shared<MapChip>> MapChip_object;		

	//csvを格納しておく用のvector(床用)
	std::vector<std::vector<int>>map_csv_;

	//オブジェクト用
	std::vector<std::vector<int>>map_csv_objct_;
	
	//---描画する際のフラグ---//
	bool drawWindowFlag = true;
	bool drawSeqFlag = false;

private:
	/*TNL_CO_SEQUENCE(VillageScene , &VillageScene::seqIdle)*/

	//---mapChip関連の処理---//
	
	//mapChipの読み込み関数
	void mapChip_Loding(std::vector<std::vector<int>>map_csv, std::vector<std::vector<int>>map_object_csv );


	void mapChip_collision();																				//mapChipのあたり判定関数
	
	//---mapChip関連の変数---//
	std::string gpc_map_chip_hdls_pass_ = "graphics/image_(2024_0115_0231).png";							// 画像パス
	int map_chip_width_ = 32;																				// マップチップの幅
	int map_chip_height_ = 32;																				// マップチップの高さ
	int map_chip_all_size_ = 0;																				// マップチップの総フレーム数
	int map_chip_x_size_ = 8;																				// マップチップの横フレーム数
	int map_chip_y_size_ = 262;																				// マップチップの縦フレーム数
	int gpc_map_chip_hdls_[2096] = {};																		// 画像データ格納
	std::string map_data_csv_pass_;																			// マップCSVデータのパス
	int map_height = 945;																					//マップの高さ
	int map_width = 1600;																					//マップの幅

	tnl::Vector3 first_pos = {};																			//村の最初の座標
	const tnl::Vector3 FIRST_WORLD_POS = { 435 , 1790 , 0 };												//worldMapでの最初の位置
	const tnl::Vector3 BOSS_EXIT_POS = {5692 , 2046 , 0};													//ボスの城から出たときにの座標
	const tnl::Vector3 TOWN_EXIT_POS = { 2240, 2056 , 0 };													//町から出た時の座標
	const tnl::Vector3 CASTLE_TOWN_EXIT_POS = { 2683, 244 , 0 };											//城下町から出た時の座標
	const tnl::Vector3 CASTLE_FIRST_POS = { 1111,1674 , 0 };												//城に入った時の最初の位置
	const tnl::Vector3 CASTLE_EXIT_POS = { 1114 , 100 , 0 };												//城から城下町に出た時の座標

	const int map_Chip_WeaponShop = 510;																	//武器屋のマップチップ
	const int map_Chip_Herdman = 708;																		//村長のマップチップ
	
	//村シーンでの当たり判定
	void VillageCollision();

	//町シーンでの当たり判定
	void TownCollision();

	//ダンジョンシーンでの当たり判定
	void DungeonCollison();

	//城下町シーンの当たり判定
	void CastleTownCollision();

	//城シーンの当たり判定
	void CastleCollision();

//------------------------------------------------------------------------------------------------------------------------
//---ポインタ---//

	//メニューウィンドウ
	Weak<Menu>menuWindow;
	//Npc関連
	//村長　||　町長
	Shared<Npc>herdman_npc = nullptr;
	//武器商人
	Shared<Npc>armsdealer_npc = nullptr;
	//神官
	Shared<Npc>priest_npc = nullptr;
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
	//敵
	Shared<Npc>enemy_npc = nullptr;
	//王
	Shared<Npc>king_npc = nullptr;
	//王妃
	Shared<Npc>queen_npc = nullptr;
	//マップチップ
	Shared<MapChip>mapchip = nullptr;
	//話すコマンド
	Shared<MenuWindow>dia_log_coment_select = nullptr;
	MenuWindow::MenuElement_t* dia_log_coment = nullptr;

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
	void SequenceChange(Sequence_num secect_sequence);

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

	 //---画像配列---//
	 //村長、町長
	 std::string herdman_hdl_pass[4];
	 //武器商人
	 std::string Armsdealer_hdl_pass[4];
	 //神官
	 std::string Priest_hdl_pass[4] = { "graphics/Npc/Priest_left.png" , "graphics/Npc/Priest_right.png","graphics/Npc/Priest_Up.png" ,"graphics/Npc/Priest_npc.png"};
	 //宿屋
	 std::string inn_hdl_pass[4];
	 //魔物
	 std::string enemy_npc_pass[4];
	 //ボス
	 std::string boss_hdl_pass[4] = { "graphics/Npc/maou_left.png" ,"graphics/Npc/maou_right.png","graphics/Npc/maou_up.png","graphics/Npc/maou_idle.png"};
	 //王
	 std::string king_hdl_pass[4] = { "graphics/Npc/king_left.png" ,"graphics/Npc/king_right.png","graphics/Npc/king_up.png","graphics/Npc/king_idle.png" };
	 //王妃
	 std::string queen_hdl_pass[4] = { "graphics/Npc/queen_left.png" ,"graphics/Npc/queen_right.png","graphics/Npc/queen_up.png","graphics/Npc/queen_idle.png" };
	 //住人(女性)
	 std::string female_resident_hdl_pass[4];
	 //住人(男性)
	 std::string resident_male_hdl_pass[4];

	 //ボスシーンでの背景
	 int boss_background_hdl = 0;

	//武器屋のコメント
	int armsdealer_comet = 1;

	//NPCの大きさ
	const float NPC_SCALE = 1.5f;

public:


//------------------------------------------------------------------------------------------------------------------------
//インベントリ関係
private:

	//インベントリを開く
	int first_menu = 1;

//------------------------------------------------------------------------------------------------------------------------
//プレイヤー関係
private:

	//プレイヤーのサイズ
	float PlyerScale = 1.5f;

	//プレイヤーの速度
	float PlayerVelocity = 4.0f;

};
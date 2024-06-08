//
//マップに設置してある村や町などを管理するクラス
//


#pragma once
#include	<unordered_set>
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Object/NPC.h"
#include"../Item/WeaponShop.h"
#include"../Manager/SoundManager.h"
#include"../Manager/EventManager.h"

class UIManager;
class CsvManager;

class InMapScene : public BaseScene {
public:
	
	//InMapSceneの状態
	enum class InMapState {
		VILLAGE,		//村
		TOWN,			//町
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
	void InitInMapScene(InMapState inmap_state);
	//InMapSceneの更新処理
	void InMapSceneUpdate(float delta_time);
	//InMapSceneの描画処理
	void InMapSceneDraw();
	//村シーンでの初期化
	void InitVillageScene();
	//ダンジョンシーンの初期化
	void InitDungeonScene();
	
	//メニューウィンドウの生成
	void InitMenuWindow();


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
	
	//---描画する際のフラグ---//
	bool drawWindowFlag = true;
	bool drawSeqFlag = false;

private:
	/*TNL_CO_SEQUENCE(VillageScene , &VillageScene::seqIdle)*/

	//---mapChip関連の処理---//
	void mapChip_Loding(std::vector<std::vector<int>>map_csv);												//mapChipの読み込み関数
	void mapChip_collision(std::vector<std::vector<int>>map_csv);																				//mapChipのあたり判定関数
	
	//---mapChip関連の変数---//
	std::string gpc_map_chip_hdls_pass_;																	// 画像パス
	int map_chip_width_;																					// マップチップの幅
	int map_chip_height_;																					// マップチップの高さ
	int map_chip_all_size_;																					// マップチップの総フレーム数
	int map_chip_x_size_;																					// マップチップの横フレーム数
	int map_chip_y_size_;																					// マップチップの縦フレーム数
	int gpc_map_chip_hdls_[1992];																			// 画像データ格納
	std::string map_data_csv_pass_;																			// マップCSVデータのパス
	int map_height = 945;																					//マップの高さ
	int map_width = 1600;																					//マップの幅

	tnl::Vector3 first_pos = {};																			//村の最初の座標
	const tnl::Vector3 FIRST_WORLD_POS = { 435 , 1770 , 0 };	//worldMapでの最初の位置
	const tnl::Vector3 BOSS_EXIT_POS = {5692 , 1978 , 0};

	const int map_Chip_WeaponShop = 510;																	//武器屋のマップチップ
	const int map_Chip_Herdman = 708;																		//村長のマップチップ
	

	// ---この配列の中に入っている値を使ってあたり判定を決める---//
	//村シーン
	const std::unordered_set<int> village_colision_values = {-1, 324, 384, 392, 545, 528, 536, 464, 631, 708, 614, 630, 510, 400, 408, 657, 656, 395, 230, 231};

	//村シーンでの当たり判定
	void VillageCollision(std::vector<std::vector<int>>map_csv);

	//ダンジョンシーン	
	/*const std::unordered_set<int> doungen_collision_values = { };*/

	//ダンジョンシーンでの当たり判定
	void DungeonCollison();

//------------------------------------------------------------------------------------------------------------------------
//---ポインタ---//

	//メニューウィンドウ
	Weak<Menu>menuWindow;
	//Npc関連
	//村長
	Shared<Npc>herdman_npc = nullptr;
	//武器商人
	Shared<Npc>armsdealer_npc = nullptr;
	//神官
	Shared<Npc>priest_npc = nullptr;
	//ボス
	Shared<Npc>boss_npc = nullptr;
	//武器屋
	Shared<WeaponShop>weapon_shop = nullptr;
	//マップチップ
	Shared<MapChip>mapchip = nullptr;

//------------------------------------------------------------------------------------------------------------------------
//---シーケンス---//

	//シーケンスを管理するクラス
	enum class Sequence_num {
		Idle,
		WeaponShop,
		MenuOpen,
		MenuNpc
	};

	//どのシーケンスが選択されているか
	 Sequence_num select_num;

	//シーケンスの定義
	tnl::Sequence<InMapScene> sequence_ = tnl::Sequence<InMapScene>(this, &InMapScene::seqIdle);

	//通常時のシーケンス
	bool seqIdle(float delta_time);

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

	//マップをロードするシーケンス
	bool LoadMapSequence();

	
//------------------------------------------------------------------------------------------------------------------------
//---武器関係---//

//------------------------------------------------------------------------------------------------------------------------
//---Npc関係---//	

	//---村シーン---//
	 std::string herdman_hdl_pass[4] = {"graphics/Npc/herdman.png" ,"graphics/Npc/herdman_idle.png" ,"graphics/Npc/herdman_right.png","graphics/Npc/herdman_up.png" };
	 std::string Armsdealer_hdl_pass[1] = {"graphics/Npc/Armsdealer.png"};
	 std::string Priest_hdl_pass[3] = { "graphics/Npc/Priest_npc.png" , "graphics/Npc/Priest_left.png" , "graphics/Npc/Priest_right.png" };

	 //---ダンジョンシーン---//
	 std::string boss_hdl_pass[1] = {"graphics/Enemy/maou_idle.png"};

	 //ボスシーンでの背景
	 int boss_background_hdl = 0;

	//武器屋のコメント
	int armsdealer_comet = 1;

	//npcがコメント表示
	bool NpcComntTrue = true;

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
	float PlayerVelocity = 5.0f;

};
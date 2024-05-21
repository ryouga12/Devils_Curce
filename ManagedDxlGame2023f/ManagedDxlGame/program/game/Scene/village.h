//
//村クラス（主に何個かの村をこのクラスで管理する予定）
//


#pragma once
#include	<unordered_set>
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Object/NPC.h"
#include"../Item/WeaponShop.h"
#include"../Manager/SoundManager.h"
#include"../Manager/EventManager.h"

class VillageScene : public BaseScene {
public:
	
	VillageScene(tnl::Vector3 firstPos);
	~VillageScene()override;

	//更新処理
	void Update(float delta_time)override;
	//描画処理
	void Draw()override;
	//初期化
	void InitVillageScene();
	//メニューウィンドウの生成
	void InitMenuWindow();

private:

	//時間経過で切り替えるフラグ
	bool TimeCount = false;

	//遅延させる秒数
	float TimeSE = 0.2f;


//------------------------------------------------------------------------------------------------------------------------
//MapChip関連
public:

	//mapChipを格納する場所
	std::list<Shared<MapChip>> MapChips;
	std::vector<std::vector<int>> map_csv;								
	
	//---描画する際のフラグ---//
	bool drawWindowFlag = true;
	bool drawSeqFlag = false;

private:
	/*TNL_CO_SEQUENCE(VillageScene , &VillageScene::seqIdle)*/

	//---mapChip関連の処理---//
	void mapChip_Loding();												//mapChipの読み込み関数
	void mapChip_collision();											//mapChipのあたり判定関数
	
	//---mapChip関連の変数---//
	std::string gpc_map_chip_hdls_pass_;								// 画像パス
	int map_chip_width_;												// マップチップの幅
	int map_chip_height_;												// マップチップの高さ
	int map_chip_all_size_;												// マップチップの総フレーム数
	int map_chip_x_size_;												// マップチップの横フレーム数
	int map_chip_y_size_;												// マップチップの縦フレーム数
	int gpc_map_chip_hdls_[1992];										// 画像データ格納
	std::string map_data_csv_pass_;										// マップCSVデータのパス
	const int MAP_HEIGHT = 945;											//マップの高さ
	const int MAP_WIDTH = 1600;											//マップの幅

	tnl::Vector3 village_Pos = {};										//村の最初の座標
	tnl::Vector3 first_world_pos = { 435 , 1770 , 0 };					//worldMapでの最初の位置

	const int map_Chip_WeaponShop = 510;								//武器屋のマップチップ
	const int map_Chip_Herdman = 708;									//村長のマップチップ
	const int map_Chip_Object = 545;									//オブジェクトのマップチップ
	

	// ---この配列の中に入っている値を使ってあたり判定を決める---//
	const std::unordered_set<int> colisionValues = {-1, 324, 384, 392, 545, 528, 536, 464, 631, 708, 614, 630, 510, 400, 408, 657, 656, 395, 230, 231};
		
//------------------------------------------------------------------------------------------------------------------------
//---ポインタ---//

	//メニューウィンドウ
	Shared<Menu>menuWindow = nullptr;
	//選択メニューウィンドウ
	Shared<Menu>menuSelectWindow = nullptr;
	//メニューのコメントウィンドウ
	Shared<Menu>menuComentWindiow = nullptr;
	//Npc関連
	Shared<Npc>herdman_npc = nullptr;
	Shared<Npc>Armsdealer_npc = nullptr;
	Shared<Npc>Priest_npc = nullptr;
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
		MenuShopOpen,
		MenuNpc
	};

	//どのシーケンスが選択されているか
	 Sequence_num select_num;

	//シーケンスの定義
	tnl::Sequence<VillageScene> sequence_ = tnl::Sequence<VillageScene>(this, &VillageScene::seqIdle);

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
	
//------------------------------------------------------------------------------------------------------------------------
//---武器関係---//

//------------------------------------------------------------------------------------------------------------------------
//---Npc関係---//				
	std::string herdman_hdl_pass[4] = {"graphics/Npc/herdman.png" ,"graphics/Npc/herdman_idle.png" ,"graphics/Npc/herdman_right.png","graphics/Npc/herdman_up.png" };
	std::string Armsdealer_hdl_pass[1] = {"graphics/Npc/Armsdealer.png"};
	std::string Priest_hdl_pass[3] = { "graphics/Npc/Priest_npc.png" , "graphics/Npc/Priest_left.png" , "graphics/Npc/Priest_right.png" };

	//武器屋のコメント
	int armsdealer_comet = 1;

	//Npcの幅と高さを取得する為の変数
	int npcWidth = 1; int npcHeight = 2;

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

	//プレイヤーのコントロールを制御する
	int PlyControlTrue = 0; int PlyControlFalse = 1;

	//プレイヤーのサイズを取得する為のタイプ
	int PlyerWidth = 1; int PlyerHeight = 2;

	//プレイヤーの速度
	float PlayerVelocity = 5.0f;

};
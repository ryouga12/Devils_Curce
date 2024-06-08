///
///WorldMapクラス
//

#pragma once
#include	<unordered_set>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Scene/InMapScene.h"
#include"../Manager/GameManager.h"

class BaseScene;
class Enemy;
class MapChip;
class CsvManager;

class MapScene : public BaseScene {
public:
	MapScene();
	~MapScene()override;

	void Update(float delta_time)override;
	void Draw()override;

private:

	void worldMapLoad();

	tnl::Vector3 pos = { 0 , 0 , 0 };

	//村に入った時の座標
	const tnl::Vector3 village_pos = { 860 , 900 , 0 };

	//ボスの城に入った時の処理
	const tnl::Vector3 boss_castle_pos = { 805 , 895 , 0};

	//村に入るときのフラグ(trueになったら遷移させる)
	bool viilage_flag = false;

	//ボスの城に入るときのフラグ
	bool boss_castle_flag = false;

	//遅延させる秒数
	const float time = 0.5f;

	//遅延させるSEの秒数
	const float time_se = 0.8f;

	//プレイヤーのサイズ
	const float player_size = 1;

	//プレイヤーの速度
	float player_velocity = 2.5f;
	
	//プレイヤーのアニメーション
	int Animation_default = 0;

//------------------------------------------------------------------------------------------------------------------------
//シーケンス

	tnl::Sequence<MapScene> sequence_ = tnl::Sequence<MapScene>(this, &MapScene::seqIdle);
	bool seqIdle(float delta_time);
	//シーンを変える為のシーケンス
	bool seqChangeScene(float delta_time);
	
		
//------------------------------------------------------------------------------------------------------------------------
//---武器関係---//

//--------------------------------------------------------------------------------------------------------------------------
//ポインタ変数
//
private:

	Shared<MapChip>mapchip = nullptr;

//--------------------------------------------------------------------------------------------------------------------------
//mapchip関連
//
private:

	std::string map_chip_ghdl_pass;
	//建物や木などのオブジェクト用のcsvのdatapass
	std::string map_chip_csv_object_pass;

	//オブジェクト用のマップチップ
	std::list<Shared<MapChip>>MapChips_object;

	//平原用のマップチップ
	std::list<Shared<MapChip>>MapChip_continent;

	// マップチップの幅
	int map_chip_width_;	
	// マップチップの高さ
	int map_chip_height_;	
	// マップチップの総フレーム数
	int map_chip_all_size_;	
	// マップチップの横フレーム数
	int map_chip_x_size_;
	//mapchipの縦フレーム数
	int map_chip_y_size_;

	int gpc_map_chip_hdls_[2096];

	int MAP_HEIGHT = 3200;
	int MAP_WIDTH  = 6400;

	//この中の値で当たり判定をきめる
	std::unordered_set<int> colisionObjectValues = { 10 , 11 ,12, 13 , 18 , 19, 20 , 21 ,25, 36 , 37  ,41 , 234  ,335,432,440,442  , 827 , 835 ,1302,1303, 1336, 1337, 1363 ,1364 ,1365, 1476, 1477, 2024 };

	//町などの当たり判定に使う値
	std::unordered_set<int>villageValues = { 334 , 2032 , 2033  , 2034 , 2035 , 2069, 2089, 2090 };

	//地形で使う当たり判定の値
	std::unordered_set<int>worldCollisionValues = {2024 , 2027};

	//エンカウントで使う整数値
	std::unordered_set<int>encount_kind = { 0 , 1 , 2 , 4 , 5 };

	//村のマップチップ
	const int map_chip_village = 2035;

	//ボスの城のマップチップ
	const int map_chip_boss_castle = 2090;

	//当たり判定
	void WorldMapCollision();


//------------------------------------------------------------------------------------------------------------------------
//エンカウント
private:

	//ランダムエンカウントでバトルシーンで遷移する為の関数
	bool ChackEncount(int step);

	//何歩でエンカウントするか
	int encounterThreshold = 0;

	//平原
	int plain = 2;

	//平原2
	int plain_second = 0;

	//荒原
	int wilderness[2] = { 1 , 4 };

	//ボスエリア
	int boss_area = 5;

	//バトルシーンで使うハンドル
	int background_hdl = 0;

	//敵の配列のID
	int enemy_id = 0;

//------------------------------------------------------------------------------------------------------------------------
//インベントリ関係
private:
 
    //インベントリを開く
	int first_menu = 1;

};
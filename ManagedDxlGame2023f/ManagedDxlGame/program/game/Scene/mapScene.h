//------------------------------------------------------------------------------------------------------------
//
//WorldMapクラス
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include	<unordered_set>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Scene/InMapScene.h"
#include"../Manager/GameManager.h"

class BaseScene;
class Enemy;
class MapChip;

class MapScene final: public BaseScene {
public:
	MapScene();
	~MapScene()override;

	void Update(float delta_time)override;
	void Draw()override;

private:

	//マップチップの読み込み
	void worldMapLoad();

	//村に入った時の座標
	const tnl::Vector3 VILLAGE_POS = { 860 , 900 , 0 };

	//町に入った時の座標
	const tnl::Vector3 TOWN_POS = { 1180,1360,0 };

	//城下町に入った時の座標
	const tnl::Vector3 CASTLE_TOWN_POS = { 1950,950,0 };

	//ボスの城に入った時の座標
	const tnl::Vector3 BOSS_CASTLE_POS = { 769 , 895 , 0};

	//勇者の墓に入った時の座標
	const tnl::Vector3 GRAVE_POS = { 805 , 895 , 0 };

	//プレイヤーの速度
	float player_velocity = 2.5f;
	
	//プレイヤーのアニメーション
	int Animation_default = 0;

	//デバック用
	void DebugMapScene();

//------------------------------------------------------------------------------------------------------------------------
//シーケンス

	tnl::Sequence<MapScene> sequence_ = tnl::Sequence<MapScene>(this, &MapScene::seqMove);

	//プレイヤーの行動(移動など)
	bool seqMove(float delta_time);
	//イベント関連
	bool seqEvent(float delta_time);
	//シーンを変える為のシーケンス
	bool seqChangeScene(float delta_time);

	//他の所でシーケンスを切り替える
	void SequenceChangeMove()override;
	
//--------------------------------------------------------------------------------------------------------------------------
//mapchip関連
//

private:

	//この中の値で当たり判定をきめる
	std::unordered_set<int> colisionObjectValues = { 12 ,13 ,16 , 17 , 20 , 21 ,24 , 25 ,46 , 47, 97 ,432, 434 , 442  ,1244 , 1245 , 1328 , 1329 ,1336 , 1337,1670,1677, 2025 };

	//町などの当たり判定に使う値
	std::unordered_set<int>ObjectValues = { 334 ,335, 1247, 2032 , 2033  , 2034 , 2035 , 2069, 2089, 2090 };

	//地形で使う当たり判定の値
	std::unordered_set<int>worldCollisionValues = {2024 , 2027};

	//エンカウントで使う整数値
	std::unordered_set<int>encount_kind = { 0 , 1 , 2 ,3 , 5 ,6 ,1320};

	//当たり判定
	void WorldMapCollision();


//------------------------------------------------------------------------------------------------------------------------
//エンカウント
private:

	//ランダムエンカウントでバトルシーンで遷移する為の関数
	bool ChackEncount(const int& step);

	//何歩でエンカウントするか
	int encounterThreshold = 0;

	//バトルシーンで使うハンドル
	int background_hdl = 0;

	//敵の配列のID
	int enemy_id = 0;

};
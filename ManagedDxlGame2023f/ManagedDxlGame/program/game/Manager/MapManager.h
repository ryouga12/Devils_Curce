#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include	<unordered_set>
#include"../Map/Mapchip.h"


//-----------------------------------------------------------------------------------------
//
//シングルトンパターン
// マップ関係の変数などをここで扱う
//
//------------------------------------------------------------------------------------------

//class MapManager {
//
//public:
//
//	static MapManager* getMapManager();
//	static void deleteMapmanager() { delete getMapManager(); }
//
//private:
//
//	MapManager();
//	~MapManager() {};
//
////------------------------------------------------------------------------------------------------------------------------
//
//	//一個目の村に入った時の座標
//	tnl::Vector3 village_Pos = { 860 , 900 , 0 };
//
//	//遅延させる秒数
//	float BattleSceneTime = 0.5f;
//
//	//遅延させるSEの秒数
//	float VillageSceneTime = 0.8f;
//
//	//プレイヤーのサイズ
//	int PlayerSize = 1;
//
//	//プレイヤーの速度
//	float PlayerVelocity = 2.5f;
//
//	//プレイヤーのアニメーション
//	int Animation_default = 0;
//
//	//この中の値で当たり判定をきめる
//	std::unordered_set<int> colisionObjectValues = { 10 , 11 ,12, 13 , 18 , 19, 20 , 21 ,25, 36 , 37  ,41 , 234  ,335,432,440,442  , 827 , 835 ,1302,1303, 1336, 1337, 1363 ,1364 ,1365, 1476, 1477, 2024 };
//
//	//町などの当たり判定に使う値
//	std::unordered_set<int>villageValues = { 334 , 2032 , 2033  , 2034 , 2035 , 2069, 2089, 2090 };
//
//	//地形で使う当たり判定の値
//	std::unordered_set<int>worldCollisionValues = { 2024 , 2027 };
//
//	//エンカウントで使う整数値
//	std::unordered_set<int>encount_kind = { 0 , 1 , 2 , 4 , 5 };
//
//	//村のマップチップ
//	int mapChip_village = 2035;
//
//
////------------------------------------------------------------------------------------------------------------------------
////エンカウント
//private:
//
//	//平原
//	int plain = 2;
//
//	//平原2
//	int plain_second = 0;
//
//	//荒原
//	int wilderness[2] = { 1 , 4 };
//
//	//ボスエリア
//	int boss_area = 5;
//
//	//敵の配列のID
//	int enemy_1 = 1;
//	int enemy_2 = 2;
//	int enemy_3 = 3;
//	int enemy_4 = 4;
//
//
//public:
//
//	//一個目の村に入った時の座標
//	const tnl::Vector3& getVillageFirstPlayerPos()const {
//		return village_Pos;
//	}
//
//	//遅延させる秒数を取得する
//	float getTimeCount(int type) const{
//
//		//バトルシーンに遷移する用のタイム
//		if (type == 0) {
//			return BattleSceneTime;
//		}
//		//村に遷移する用の遅延タイム
//		else {
//			return VillageSceneTime;
//		}
//	}
//
//
//
//};
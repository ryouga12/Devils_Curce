///----------------------------------------------------------------------------------------------
///
/// csvをファイルのロードを一括で行う
/// 
/// ----------------------------------------------------------------------------------------------


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Scene/BaseScene.h"

class CsvManager final {

public:

	static CsvManager* getCsvManager();
	static void delateCsvManager() { delete getCsvManager(); }

	//ワールドマップ全体のマップチップのcsvを取得する
	std::vector<std::vector<int>> GetWorldMapCsv()const { return world_map_csv; }
	
	//ワールドマップ全体に配置されているオブジェクトのマップチップのcsvを取得する
	std::vector<std::vector<int>> GetWorldMapObjectCsv() const{ return world_object_csv; }

	//村のマップ用のcsvを取得する
	std::vector<std::vector<int>> GetVillagMapCsv()const { return village_map_csv; }

	//村マップ用のオブジェクトデータのcsvを取得する
	std::vector<std::vector<int>> GetVillagMapObjectCsv()const { return village_map_object_csv; }

	//町のマップ用のcsvを取得する
	std::vector<std::vector<int>> GetTownMapCsv()const { return town_map_csv; }

	//町のマップ用のオブジェクトデータのcsvを取得する
	std::vector<std::vector<int>> GetTownObjectMapCsv()const { return town_map_object_csv; }

	//城下町の床データを取得する
	std::vector<std::vector<int>> GetCastleTownMapCsv()const { return castle_town_map_csv; }

	//城下町のマップ用のオブジェクトデータのcsvを取得する
	std::vector<std::vector<int>> GetCastleTownObjectMapCsv()const { return castle_town_map_object_csv; }

	//城の床データを取得する
	std::vector<std::vector<int>> GetCastleMapCsv()const { return castle_map_csv; }

	//城のマップ用のオブジェクトデータのcsvを取得する
	std::vector<std::vector<int>> GetCastleObjectMapCsv()const { return castle_object_csv; }

	//ボスの城の床データのcsvを取得する　
	std::vector<std::vector<int>> GetBossCastleFloorCsv()const { return boss_castle_floor_csv; }

	//ボスの城のオブジェクトデータ用のcsvを取得する
	std::vector<std::vector<int>> GetBossCastleObjectCsv()const { return boss_castle_object_csv; }

	//勇者のお墓の床データ用のcsvを取得する
	std::vector<std::vector<int>> GetGraveFloorCsv()const { return grave_floor_csv; }

	//勇者のお墓のオブジェクトデータ用のcsvを取得する
	std::vector<std::vector<int>> GetGraveObjectCsv()const { return grave_object_csv; }

	//プレイヤーのステータス用のcsvを取得する
	std::vector<std::vector<tnl::CsvCell>>GetPlayerStatusCsv()const { return PlyerStatus_Csv_Info; }

	//プレイヤーのアニメーション用の画像を取得する
	std::vector<std::vector<std::string>>GetPlayerAnimationImage()const { return player_animation_csv; }

	//Npcの画像を取得する
	std::vector<std::vector<std::string>>GetNpcImageCsv()const { return npc_image_csv; }

	//エネミー用のステータスcsvを取得する
	std::vector<std::vector<std::string>>GetEnemyStatusCsv()const { return enemy_csv_array; }

	//コメント用のcsvを取得する
	std::vector<std::vector<std::string>>GetComentCsv()const { return coment_csv; }

	//アイテム用のcsvを取得する
	std::vector<std::vector<tnl::CsvCell>>GetItemCsv()const { return itemStatus_Csv_Info;}

	//ストーリー用のcsvを取得する
	std::vector<std::vector<std::string>>GetStoryCsv()const { return story_csv; }

	//csvをロードする
	void LoadCsv(const BaseScene::SceneState& curent_state);

private:

	CsvManager();
	~CsvManager() {};

	//平原マップデータのロード
	std::vector<std::vector<int>>world_map_csv;

	//ワールドマップのオブジェクトデータのロード
	std::vector<std::vector<int>>world_object_csv;

	//村のマップデータのロード
	std::vector<std::vector<int>>village_map_csv ;

	//村のオブジェクトデータのロード
	std::vector<std::vector<int>>village_map_object_csv ;

	//町の床データのロード
	std::vector<std::vector<int>>town_map_csv ;

	//町のマップオブジェクトデータのロード
	std::vector<std::vector<int>>town_map_object_csv;

	//城下町の床データのロード
	std::vector<std::vector<int>>castle_town_map_csv;

	//城下町のマップオブジェクトデータのロード
	std::vector<std::vector<int>>castle_town_map_object_csv;

	//城の床データのロード
	std::vector<std::vector<int>>castle_map_csv;

	//城のマップオブジェクトデータのロード
	std::vector<std::vector<int>>castle_object_csv ;

	//ボスの城の床データ
	std::vector<std::vector<int>>boss_castle_floor_csv ;

	//ボスの城のオブジェクトデータ
	std::vector<std::vector<int>>boss_castle_object_csv;

	//勇者の墓の床データ
	std::vector<std::vector<int>>grave_floor_csv;

	//勇者の墓のオブジェクトデータ
	std::vector<std::vector<int>>grave_object_csv;

	//プレイヤーのステータスデータ
	std::vector<std::vector<tnl::CsvCell>>PlyerStatus_Csv_Info;

	//プレイヤーのアニメーション画像データ
	std::vector<std::vector<std::string>>player_animation_csv;

	//Npcの画像データ
	std::vector<std::vector<std::string>>npc_image_csv;

	//エネミーのステータス
	std::vector<std::vector<std::string>>enemy_csv_array;

	//アクター用のコメント
	std::vector<std::vector<std::string>>coment_csv;

	//アイテム用のcsv
	std::vector<std::vector<tnl::CsvCell>>itemStatus_Csv_Info;

	//ストーリー用のcsv
	std::vector<std::vector<std::string>>story_csv;
};
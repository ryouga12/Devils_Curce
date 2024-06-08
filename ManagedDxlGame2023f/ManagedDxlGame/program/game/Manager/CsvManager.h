///----------------------------------------------------------------------------------------------
///
/// csvをファイルのロードを一括で行う
/// 
/// ----------------------------------------------------------------------------------------------


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

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

	//ボスの城の床データのcsvを取得する　
	std::vector<std::vector<int>> GetBossCastleFloorCsv()const { return boss_castle_floor_csv; }

	//ボスの城のオブジェクトデータ用のcsvを取得する
	std::vector<std::vector<int>> GetBossCastleObjectCsv()const { return boss_castle_object_csv; }

	//プレイヤーのステータス用のcsvを取得する
	std::vector<std::vector<tnl::CsvCell>>GetPlayerStatusCsv()const { return PlyerStatus_Csv_Info; }

	//プレイヤーのアニメーション用の画像を取得する
	std::vector<std::vector<std::string>>GetPlayerAnimationImage()const { return player_animation_csv; }

	//エネミー用のステータスcsvを取得する
	std::vector<std::vector<std::string>>GetEnemyStatusCsv()const { return enemy_csv_array; }

	//コメント用のcsvを取得する
	std::vector<std::vector<std::string>>GetComentCsv()const { return coment_csv; }

	//アイテム用のcsvを取得する
	std::vector<std::vector<tnl::CsvCell>>GetItemCsv()const { return itemStatus_Csv_Info;}

private:

	CsvManager();
	~CsvManager() {};

	//平原マップデータのロード
	std::vector<std::vector<int>>world_map_csv = tnl::LoadCsv<int>("csv/world_map_plain.csv");

	//ワールドマップのオブジェクトデータのロード
	std::vector<std::vector<int>>world_object_csv = tnl::LoadCsv<int>("csv/world_map_object.csv");

	//村のマップデータのロード
	std::vector<std::vector<int>>village_map_csv = tnl::LoadCsv<int>("csv/map_chip_first_village.csv");

	//ボスの城の床データ
	std::vector<std::vector<int>>boss_castle_floor_csv = tnl::LoadCsv<int>("csv/boss_castle__floor.csv");

	//ボスの城のオブジェクトデータ
	std::vector<std::vector<int>>boss_castle_object_csv = tnl::LoadCsv<int>("csv/boss_castle__object.csv");

	//プレイヤーのステータスデータのロード
	std::vector<std::vector<tnl::CsvCell>>PlyerStatus_Csv_Info = tnl::LoadCsv<tnl::CsvCell>("csv/PlyerStatus.csv");

	//プレイヤーのアニメーション画像データのロード
	std::vector<std::vector<std::string>>player_animation_csv = tnl::LoadCsv<std::string>("csv/player_image.csv");

	//エネミーのステータスをロード
	std::vector<std::vector<std::string>>enemy_csv_array = tnl::LoadCsv<std::string>("csv/Enemy.csv");

	//アクター用のコメントをロード
	std::vector<std::vector<std::string>>coment_csv = tnl::LoadCsv<std::string>("csv/actor_coment.csv");

	//アイテム用のcsvをロード
	std::vector<std::vector<tnl::CsvCell>>itemStatus_Csv_Info = tnl::LoadCsv<tnl::CsvCell>("csv/Item.csv");
};
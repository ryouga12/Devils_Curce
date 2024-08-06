#include "CsvManager.h"

CsvManager* CsvManager::getCsvManager()
{
	static CsvManager* p_instance = nullptr;

	if (!p_instance)p_instance = new CsvManager();
	return p_instance;
}

CsvManager::CsvManager()
{
	if (story_csv.empty()) {
		//ストーリー用のデータロード
		story_csv = tnl::LoadCsv<std::string>("csv/story.csv");
	}
	if (PlyerStatus_Csv_Info.empty()) {
		//プレイヤーのステータスデータのロード
		PlyerStatus_Csv_Info = tnl::LoadCsv<tnl::CsvCell>("csv/PlyerStatus.csv");
	}
	if (player_animation_csv.empty()) {
		//プレイヤーのアニメーション画像データのロード
		player_animation_csv = tnl::LoadCsv<std::string>("csv/player_image.csv");
	}
	if (itemStatus_Csv_Info.empty()) {
		//アイテム用のCSV
		itemStatus_Csv_Info = tnl::LoadCsv<tnl::CsvCell>("csv/Item.csv");
	}
	if (enemy_csv_array.empty()) {
		//エネミーのステータスをロード
		enemy_csv_array = tnl::LoadCsv<std::string>("csv/Enemy.csv");
	}
}

void CsvManager::LoadCsv(const BaseScene::SceneState& curent_state)
{
	//現在のシーンがInMapSceneだったら
	if (curent_state == BaseScene::SceneState::INMAP) {

		if (coment_csv.empty()) {
			coment_csv = tnl::LoadCsv<std::string>("csv/actor_coment.csv");
		}
		if (npc_image_csv.empty()) {
			npc_image_csv = tnl::LoadCsv<std::string>("csv/npc_image.csv");
		}
		if (village_map_csv.empty()) {
			//村のマップデータのロード
			village_map_csv = tnl::LoadCsv<int>("csv/map_chip_first_village_.csv");
		}
		if (village_map_object_csv.empty()) {
			//村のオブジェクトデータのロード
			village_map_object_csv = tnl::LoadCsv<int>("csv/map_chip_first_village_Object.csv");
		}
		if (town_map_csv.empty()) {
			//町の床データのロード
			town_map_csv = tnl::LoadCsv<int>("csv/town_.csv");
		}
		if (town_map_object_csv.empty()) {
			//町のマップオブジェクトデータのロード
			town_map_object_csv = tnl::LoadCsv<int>("csv/town_object.csv");
		}
		if (castle_town_map_csv.empty()) {
			//城下町の床データのロード
			castle_town_map_csv = tnl::LoadCsv<int>("csv/castle_town_.csv");
		}
		if (castle_town_map_object_csv.empty()) {
			//城下町のマップオブジェクトデータのロード
			castle_town_map_object_csv = tnl::LoadCsv<int>("csv/castle_town_object.csv");
		}
		if (castle_map_csv.empty()) {
			//城の床データのロード
			castle_map_csv = tnl::LoadCsv<int>("csv/castle_.csv");
		}
		if (castle_object_csv.empty()) {
			//城のマップオブジェクトデータのロード
			castle_object_csv = tnl::LoadCsv<int>("csv/castle_Object.csv");
		}
		if (boss_castle_floor_csv.empty()) {
			//ボスの城の床データ
			boss_castle_floor_csv = tnl::LoadCsv<int>("csv/boss_castle__floor.csv");
		}
		if (boss_castle_object_csv.empty()) {
			//ボスの城のオブジェクトデータ
			boss_castle_object_csv = tnl::LoadCsv<int>("csv/boss_castle__object.csv");
		}
		if (grave_floor_csv.empty()) {
			//勇者のお墓の床データ
			grave_floor_csv = tnl::LoadCsv<int>("csv/grave__Plain.csv");
		}
		if (grave_object_csv.empty()) {
			//勇者のお墓のオブジェクトのデータ
			grave_object_csv = tnl::LoadCsv<int>("csv/grave__Object.csv");
		}
	}
	//現在のシーンがワールドマップだったら
	else if (curent_state == BaseScene::SceneState::WORLDMAP) {

		if (world_map_csv.empty()) {
			//平原マップデータのロード
			world_map_csv = tnl::LoadCsv<int>("csv/world_map_plain.csv");
		}
		if (world_object_csv.empty()) {
			//ワールドマップのオブジェクトデータのロード
			world_object_csv = tnl::LoadCsv<int>("csv/world_map_object.csv");
		}
	}
}

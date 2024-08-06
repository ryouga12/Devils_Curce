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
		//�X�g�[���[�p�̃f�[�^���[�h
		story_csv = tnl::LoadCsv<std::string>("csv/story.csv");
	}
	if (PlyerStatus_Csv_Info.empty()) {
		//�v���C���[�̃X�e�[�^�X�f�[�^�̃��[�h
		PlyerStatus_Csv_Info = tnl::LoadCsv<tnl::CsvCell>("csv/PlyerStatus.csv");
	}
	if (player_animation_csv.empty()) {
		//�v���C���[�̃A�j���[�V�����摜�f�[�^�̃��[�h
		player_animation_csv = tnl::LoadCsv<std::string>("csv/player_image.csv");
	}
	if (itemStatus_Csv_Info.empty()) {
		//�A�C�e���p��CSV
		itemStatus_Csv_Info = tnl::LoadCsv<tnl::CsvCell>("csv/Item.csv");
	}
	if (enemy_csv_array.empty()) {
		//�G�l�~�[�̃X�e�[�^�X�����[�h
		enemy_csv_array = tnl::LoadCsv<std::string>("csv/Enemy.csv");
	}
}

void CsvManager::LoadCsv(const BaseScene::SceneState& curent_state)
{
	//���݂̃V�[����InMapScene��������
	if (curent_state == BaseScene::SceneState::INMAP) {

		if (coment_csv.empty()) {
			coment_csv = tnl::LoadCsv<std::string>("csv/actor_coment.csv");
		}
		if (npc_image_csv.empty()) {
			npc_image_csv = tnl::LoadCsv<std::string>("csv/npc_image.csv");
		}
		if (village_map_csv.empty()) {
			//���̃}�b�v�f�[�^�̃��[�h
			village_map_csv = tnl::LoadCsv<int>("csv/map_chip_first_village_.csv");
		}
		if (village_map_object_csv.empty()) {
			//���̃I�u�W�F�N�g�f�[�^�̃��[�h
			village_map_object_csv = tnl::LoadCsv<int>("csv/map_chip_first_village_Object.csv");
		}
		if (town_map_csv.empty()) {
			//���̏��f�[�^�̃��[�h
			town_map_csv = tnl::LoadCsv<int>("csv/town_.csv");
		}
		if (town_map_object_csv.empty()) {
			//���̃}�b�v�I�u�W�F�N�g�f�[�^�̃��[�h
			town_map_object_csv = tnl::LoadCsv<int>("csv/town_object.csv");
		}
		if (castle_town_map_csv.empty()) {
			//�鉺���̏��f�[�^�̃��[�h
			castle_town_map_csv = tnl::LoadCsv<int>("csv/castle_town_.csv");
		}
		if (castle_town_map_object_csv.empty()) {
			//�鉺���̃}�b�v�I�u�W�F�N�g�f�[�^�̃��[�h
			castle_town_map_object_csv = tnl::LoadCsv<int>("csv/castle_town_object.csv");
		}
		if (castle_map_csv.empty()) {
			//��̏��f�[�^�̃��[�h
			castle_map_csv = tnl::LoadCsv<int>("csv/castle_.csv");
		}
		if (castle_object_csv.empty()) {
			//��̃}�b�v�I�u�W�F�N�g�f�[�^�̃��[�h
			castle_object_csv = tnl::LoadCsv<int>("csv/castle_Object.csv");
		}
		if (boss_castle_floor_csv.empty()) {
			//�{�X�̏�̏��f�[�^
			boss_castle_floor_csv = tnl::LoadCsv<int>("csv/boss_castle__floor.csv");
		}
		if (boss_castle_object_csv.empty()) {
			//�{�X�̏�̃I�u�W�F�N�g�f�[�^
			boss_castle_object_csv = tnl::LoadCsv<int>("csv/boss_castle__object.csv");
		}
		if (grave_floor_csv.empty()) {
			//�E�҂̂���̏��f�[�^
			grave_floor_csv = tnl::LoadCsv<int>("csv/grave__Plain.csv");
		}
		if (grave_object_csv.empty()) {
			//�E�҂̂���̃I�u�W�F�N�g�̃f�[�^
			grave_object_csv = tnl::LoadCsv<int>("csv/grave__Object.csv");
		}
	}
	//���݂̃V�[�������[���h�}�b�v��������
	else if (curent_state == BaseScene::SceneState::WORLDMAP) {

		if (world_map_csv.empty()) {
			//�����}�b�v�f�[�^�̃��[�h
			world_map_csv = tnl::LoadCsv<int>("csv/world_map_plain.csv");
		}
		if (world_object_csv.empty()) {
			//���[���h�}�b�v�̃I�u�W�F�N�g�f�[�^�̃��[�h
			world_object_csv = tnl::LoadCsv<int>("csv/world_map_object.csv");
		}
	}
}

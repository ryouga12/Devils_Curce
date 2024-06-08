///----------------------------------------------------------------------------------------------
///
/// csv���t�@�C���̃��[�h���ꊇ�ōs��
/// 
/// ----------------------------------------------------------------------------------------------


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class CsvManager final {

public:

	static CsvManager* getCsvManager();
	static void delateCsvManager() { delete getCsvManager(); }

	//���[���h�}�b�v�S�̂̃}�b�v�`�b�v��csv���擾����
	std::vector<std::vector<int>> GetWorldMapCsv()const { return world_map_csv; }
	
	//���[���h�}�b�v�S�̂ɔz�u����Ă���I�u�W�F�N�g�̃}�b�v�`�b�v��csv���擾����
	std::vector<std::vector<int>> GetWorldMapObjectCsv() const{ return world_object_csv; }

	//���̃}�b�v�p��csv���擾����
	std::vector<std::vector<int>> GetVillagMapCsv()const { return village_map_csv; }

	//�{�X�̏�̏��f�[�^��csv���擾����@
	std::vector<std::vector<int>> GetBossCastleFloorCsv()const { return boss_castle_floor_csv; }

	//�{�X�̏�̃I�u�W�F�N�g�f�[�^�p��csv���擾����
	std::vector<std::vector<int>> GetBossCastleObjectCsv()const { return boss_castle_object_csv; }

	//�v���C���[�̃X�e�[�^�X�p��csv���擾����
	std::vector<std::vector<tnl::CsvCell>>GetPlayerStatusCsv()const { return PlyerStatus_Csv_Info; }

	//�v���C���[�̃A�j���[�V�����p�̉摜���擾����
	std::vector<std::vector<std::string>>GetPlayerAnimationImage()const { return player_animation_csv; }

	//�G�l�~�[�p�̃X�e�[�^�Xcsv���擾����
	std::vector<std::vector<std::string>>GetEnemyStatusCsv()const { return enemy_csv_array; }

	//�R�����g�p��csv���擾����
	std::vector<std::vector<std::string>>GetComentCsv()const { return coment_csv; }

	//�A�C�e���p��csv���擾����
	std::vector<std::vector<tnl::CsvCell>>GetItemCsv()const { return itemStatus_Csv_Info;}

private:

	CsvManager();
	~CsvManager() {};

	//�����}�b�v�f�[�^�̃��[�h
	std::vector<std::vector<int>>world_map_csv = tnl::LoadCsv<int>("csv/world_map_plain.csv");

	//���[���h�}�b�v�̃I�u�W�F�N�g�f�[�^�̃��[�h
	std::vector<std::vector<int>>world_object_csv = tnl::LoadCsv<int>("csv/world_map_object.csv");

	//���̃}�b�v�f�[�^�̃��[�h
	std::vector<std::vector<int>>village_map_csv = tnl::LoadCsv<int>("csv/map_chip_first_village.csv");

	//�{�X�̏�̏��f�[�^
	std::vector<std::vector<int>>boss_castle_floor_csv = tnl::LoadCsv<int>("csv/boss_castle__floor.csv");

	//�{�X�̏�̃I�u�W�F�N�g�f�[�^
	std::vector<std::vector<int>>boss_castle_object_csv = tnl::LoadCsv<int>("csv/boss_castle__object.csv");

	//�v���C���[�̃X�e�[�^�X�f�[�^�̃��[�h
	std::vector<std::vector<tnl::CsvCell>>PlyerStatus_Csv_Info = tnl::LoadCsv<tnl::CsvCell>("csv/PlyerStatus.csv");

	//�v���C���[�̃A�j���[�V�����摜�f�[�^�̃��[�h
	std::vector<std::vector<std::string>>player_animation_csv = tnl::LoadCsv<std::string>("csv/player_image.csv");

	//�G�l�~�[�̃X�e�[�^�X�����[�h
	std::vector<std::vector<std::string>>enemy_csv_array = tnl::LoadCsv<std::string>("csv/Enemy.csv");

	//�A�N�^�[�p�̃R�����g�����[�h
	std::vector<std::vector<std::string>>coment_csv = tnl::LoadCsv<std::string>("csv/actor_coment.csv");

	//�A�C�e���p��csv�����[�h
	std::vector<std::vector<tnl::CsvCell>>itemStatus_Csv_Info = tnl::LoadCsv<tnl::CsvCell>("csv/Item.csv");
};
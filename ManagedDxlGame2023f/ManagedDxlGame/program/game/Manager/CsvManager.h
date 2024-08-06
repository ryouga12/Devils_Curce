///----------------------------------------------------------------------------------------------
///
/// csv���t�@�C���̃��[�h���ꊇ�ōs��
/// 
/// ----------------------------------------------------------------------------------------------


#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Scene/BaseScene.h"

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

	//���}�b�v�p�̃I�u�W�F�N�g�f�[�^��csv���擾����
	std::vector<std::vector<int>> GetVillagMapObjectCsv()const { return village_map_object_csv; }

	//���̃}�b�v�p��csv���擾����
	std::vector<std::vector<int>> GetTownMapCsv()const { return town_map_csv; }

	//���̃}�b�v�p�̃I�u�W�F�N�g�f�[�^��csv���擾����
	std::vector<std::vector<int>> GetTownObjectMapCsv()const { return town_map_object_csv; }

	//�鉺���̏��f�[�^���擾����
	std::vector<std::vector<int>> GetCastleTownMapCsv()const { return castle_town_map_csv; }

	//�鉺���̃}�b�v�p�̃I�u�W�F�N�g�f�[�^��csv���擾����
	std::vector<std::vector<int>> GetCastleTownObjectMapCsv()const { return castle_town_map_object_csv; }

	//��̏��f�[�^���擾����
	std::vector<std::vector<int>> GetCastleMapCsv()const { return castle_map_csv; }

	//��̃}�b�v�p�̃I�u�W�F�N�g�f�[�^��csv���擾����
	std::vector<std::vector<int>> GetCastleObjectMapCsv()const { return castle_object_csv; }

	//�{�X�̏�̏��f�[�^��csv���擾����@
	std::vector<std::vector<int>> GetBossCastleFloorCsv()const { return boss_castle_floor_csv; }

	//�{�X�̏�̃I�u�W�F�N�g�f�[�^�p��csv���擾����
	std::vector<std::vector<int>> GetBossCastleObjectCsv()const { return boss_castle_object_csv; }

	//�E�҂̂���̏��f�[�^�p��csv���擾����
	std::vector<std::vector<int>> GetGraveFloorCsv()const { return grave_floor_csv; }

	//�E�҂̂���̃I�u�W�F�N�g�f�[�^�p��csv���擾����
	std::vector<std::vector<int>> GetGraveObjectCsv()const { return grave_object_csv; }

	//�v���C���[�̃X�e�[�^�X�p��csv���擾����
	std::vector<std::vector<tnl::CsvCell>>GetPlayerStatusCsv()const { return PlyerStatus_Csv_Info; }

	//�v���C���[�̃A�j���[�V�����p�̉摜���擾����
	std::vector<std::vector<std::string>>GetPlayerAnimationImage()const { return player_animation_csv; }

	//Npc�̉摜���擾����
	std::vector<std::vector<std::string>>GetNpcImageCsv()const { return npc_image_csv; }

	//�G�l�~�[�p�̃X�e�[�^�Xcsv���擾����
	std::vector<std::vector<std::string>>GetEnemyStatusCsv()const { return enemy_csv_array; }

	//�R�����g�p��csv���擾����
	std::vector<std::vector<std::string>>GetComentCsv()const { return coment_csv; }

	//�A�C�e���p��csv���擾����
	std::vector<std::vector<tnl::CsvCell>>GetItemCsv()const { return itemStatus_Csv_Info;}

	//�X�g�[���[�p��csv���擾����
	std::vector<std::vector<std::string>>GetStoryCsv()const { return story_csv; }

	//csv�����[�h����
	void LoadCsv(const BaseScene::SceneState& curent_state);

private:

	CsvManager();
	~CsvManager() {};

	//�����}�b�v�f�[�^�̃��[�h
	std::vector<std::vector<int>>world_map_csv;

	//���[���h�}�b�v�̃I�u�W�F�N�g�f�[�^�̃��[�h
	std::vector<std::vector<int>>world_object_csv;

	//���̃}�b�v�f�[�^�̃��[�h
	std::vector<std::vector<int>>village_map_csv ;

	//���̃I�u�W�F�N�g�f�[�^�̃��[�h
	std::vector<std::vector<int>>village_map_object_csv ;

	//���̏��f�[�^�̃��[�h
	std::vector<std::vector<int>>town_map_csv ;

	//���̃}�b�v�I�u�W�F�N�g�f�[�^�̃��[�h
	std::vector<std::vector<int>>town_map_object_csv;

	//�鉺���̏��f�[�^�̃��[�h
	std::vector<std::vector<int>>castle_town_map_csv;

	//�鉺���̃}�b�v�I�u�W�F�N�g�f�[�^�̃��[�h
	std::vector<std::vector<int>>castle_town_map_object_csv;

	//��̏��f�[�^�̃��[�h
	std::vector<std::vector<int>>castle_map_csv;

	//��̃}�b�v�I�u�W�F�N�g�f�[�^�̃��[�h
	std::vector<std::vector<int>>castle_object_csv ;

	//�{�X�̏�̏��f�[�^
	std::vector<std::vector<int>>boss_castle_floor_csv ;

	//�{�X�̏�̃I�u�W�F�N�g�f�[�^
	std::vector<std::vector<int>>boss_castle_object_csv;

	//�E�҂̕�̏��f�[�^
	std::vector<std::vector<int>>grave_floor_csv;

	//�E�҂̕�̃I�u�W�F�N�g�f�[�^
	std::vector<std::vector<int>>grave_object_csv;

	//�v���C���[�̃X�e�[�^�X�f�[�^
	std::vector<std::vector<tnl::CsvCell>>PlyerStatus_Csv_Info;

	//�v���C���[�̃A�j���[�V�����摜�f�[�^
	std::vector<std::vector<std::string>>player_animation_csv;

	//Npc�̉摜�f�[�^
	std::vector<std::vector<std::string>>npc_image_csv;

	//�G�l�~�[�̃X�e�[�^�X
	std::vector<std::vector<std::string>>enemy_csv_array;

	//�A�N�^�[�p�̃R�����g
	std::vector<std::vector<std::string>>coment_csv;

	//�A�C�e���p��csv
	std::vector<std::vector<tnl::CsvCell>>itemStatus_Csv_Info;

	//�X�g�[���[�p��csv
	std::vector<std::vector<std::string>>story_csv;
};
///
///WorldMap�N���X
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

	//���ɓ��������̍��W
	const tnl::Vector3 village_pos = { 860 , 900 , 0 };

	//�{�X�̏�ɓ��������̏���
	const tnl::Vector3 boss_castle_pos = { 805 , 895 , 0};

	//���ɓ���Ƃ��̃t���O(true�ɂȂ�����J�ڂ�����)
	bool viilage_flag = false;

	//�{�X�̏�ɓ���Ƃ��̃t���O
	bool boss_castle_flag = false;

	//�x��������b��
	const float time = 0.5f;

	//�x��������SE�̕b��
	const float time_se = 0.8f;

	//�v���C���[�̃T�C�Y
	const float player_size = 1;

	//�v���C���[�̑��x
	float player_velocity = 2.5f;
	
	//�v���C���[�̃A�j���[�V����
	int Animation_default = 0;

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X

	tnl::Sequence<MapScene> sequence_ = tnl::Sequence<MapScene>(this, &MapScene::seqIdle);
	bool seqIdle(float delta_time);
	//�V�[����ς���ׂ̃V�[�P���X
	bool seqChangeScene(float delta_time);
	
		
//------------------------------------------------------------------------------------------------------------------------
//---����֌W---//

//--------------------------------------------------------------------------------------------------------------------------
//�|�C���^�ϐ�
//
private:

	Shared<MapChip>mapchip = nullptr;

//--------------------------------------------------------------------------------------------------------------------------
//mapchip�֘A
//
private:

	std::string map_chip_ghdl_pass;
	//������؂Ȃǂ̃I�u�W�F�N�g�p��csv��datapass
	std::string map_chip_csv_object_pass;

	//�I�u�W�F�N�g�p�̃}�b�v�`�b�v
	std::list<Shared<MapChip>>MapChips_object;

	//�����p�̃}�b�v�`�b�v
	std::list<Shared<MapChip>>MapChip_continent;

	// �}�b�v�`�b�v�̕�
	int map_chip_width_;	
	// �}�b�v�`�b�v�̍���
	int map_chip_height_;	
	// �}�b�v�`�b�v�̑��t���[����
	int map_chip_all_size_;	
	// �}�b�v�`�b�v�̉��t���[����
	int map_chip_x_size_;
	//mapchip�̏c�t���[����
	int map_chip_y_size_;

	int gpc_map_chip_hdls_[2096];

	int MAP_HEIGHT = 3200;
	int MAP_WIDTH  = 6400;

	//���̒��̒l�œ����蔻������߂�
	std::unordered_set<int> colisionObjectValues = { 10 , 11 ,12, 13 , 18 , 19, 20 , 21 ,25, 36 , 37  ,41 , 234  ,335,432,440,442  , 827 , 835 ,1302,1303, 1336, 1337, 1363 ,1364 ,1365, 1476, 1477, 2024 };

	//���Ȃǂ̓����蔻��Ɏg���l
	std::unordered_set<int>villageValues = { 334 , 2032 , 2033  , 2034 , 2035 , 2069, 2089, 2090 };

	//�n�`�Ŏg�������蔻��̒l
	std::unordered_set<int>worldCollisionValues = {2024 , 2027};

	//�G���J�E���g�Ŏg�������l
	std::unordered_set<int>encount_kind = { 0 , 1 , 2 , 4 , 5 };

	//���̃}�b�v�`�b�v
	const int map_chip_village = 2035;

	//�{�X�̏�̃}�b�v�`�b�v
	const int map_chip_boss_castle = 2090;

	//�����蔻��
	void WorldMapCollision();


//------------------------------------------------------------------------------------------------------------------------
//�G���J�E���g
private:

	//�����_���G���J�E���g�Ńo�g���V�[���őJ�ڂ���ׂ̊֐�
	bool ChackEncount(int step);

	//�����ŃG���J�E���g���邩
	int encounterThreshold = 0;

	//����
	int plain = 2;

	//����2
	int plain_second = 0;

	//�r��
	int wilderness[2] = { 1 , 4 };

	//�{�X�G���A
	int boss_area = 5;

	//�o�g���V�[���Ŏg���n���h��
	int background_hdl = 0;

	//�G�̔z���ID
	int enemy_id = 0;

//------------------------------------------------------------------------------------------------------------------------
//�C���x���g���֌W
private:
 
    //�C���x���g�����J��
	int first_menu = 1;

};
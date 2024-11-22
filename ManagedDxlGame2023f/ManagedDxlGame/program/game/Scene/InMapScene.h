//------------------------------------------------------------------------------------------------------------
//
//�}�b�v�ɐݒu���Ă��鑺�⒬�Ȃǂ̃V�[��
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include	<unordered_set>
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Object/NPC.h"
#include"../Manager/SoundManager.h"
#include"../Manager/EventManager.h"
#include"../../koni_name_space/common/common_value.h"

class WeaponShop;

class InMapScene final : public BaseScene {
public:
	
	//InMapScene�̏��
	enum class InMapState {
		//��
		VILLAGE,		
		//��
		TOWN,			
		//�鉺��
		CASTLETOWN,		
		//��
		CASTLE,			
		//�{�X�̏�(�{�X�̏ꏊ�܂ł̓�,��ɊK�i�Ȃ�)
		BOSSCASTLE,     
		//�{�X�̊�
		BOSSROOM,		
		//��
		GRAVE,
		//�D�̒�
		SHIP
	};

	InMapScene() = default;

	//InMapScene�̃R���X�g���N�^
	//arg_1 : �ǂ̍��W�Ƀv���C���[���Z�b�g���邩
	//arg_2 : �ǂ̑��Ȃǂɓ������̂�
	InMapScene(const tnl::Vector3& firstPos, const InMapState& inmap_state);
	~InMapScene()override;

	//�X�V����
	void Update(float delta_time)override;

	//�`�揈��
	void Draw()override;

	//InMapScene�ł̏�����
	void InitInMapScene();

	//InMapScene�̍X�V����
	void InMapSceneUpdate(float delta_time);

	//InMapScene�̕`�揈��
	void InMapSceneDraw();

	//���V�[���ł̏�����
	void InitVillageScene();

	//�{�X�̏�V�[���̏�����
	void InitBossCastleScene();

	//�{�X�̊ԃV�[���̏�����
	void InitBossRoomScene();

	//���V�[���̏�����
	void InitTownScene();

	//�鉺���V�[��������
	void InitCastleTownScene();

	//��V�[���̏�����
	void InitCastleScene();

	//��V�[���̏�����
	void InitGraveScene();

	//�D�V�[���̏�����
	void InitShipScene();
	
	//���j���[�E�B���h�E�̐���
	void InitMenuWindow();

	//�}�b�v�O�ɏo���ۂ̏���
	void InMapSceneExitProcess(const float delta_time);
	
	//���݂̃V�[�����擾����
	InMapState GetCurentMapState()const {
		return curent_inmap_scene;
	}

	//�V�[�����Z�b�g����
	void SetInMapScene(InMapState new_map_state) {
		curent_inmap_scene = new_map_state;
	}

private:

	//���݂̃V�[�� : �ŏ��͑�����n�܂��VILLAGE
	InMapState curent_inmap_scene = InMapState::VILLAGE;

	//���Ԍo�߂Ő؂�ւ���t���O
	bool TimeCount = false;

	//�x��������b��
	float TimeSE = 0.2f;


//------------------------------------------------------------------------------------------------------------------------
//MapChip�֘A

private:

	//csv���i�[���Ă����p��vector(���p)
	std::vector<std::vector<int>>map_csv_;

	//�I�u�W�F�N�g�p
	std::vector<std::vector<int>>map_csv_objct_;

	//---mapChip�֘A�̏���---//
	
	//mapChip�̓ǂݍ��݊֐�
	void mapChip_Loding(std::vector<std::vector<int>>map_csv, std::vector<std::vector<int>>map_object_csv );

	//�����蔻��
	void mapChip_collision();																				
	
	//���̍ŏ��̍��W
	tnl::Vector3 first_pos = {};	

	//worldMap�ł̍ŏ��̈ʒu
	const tnl::Vector3 FIRST_WORLD_POS = { 334 , 1114 , 0 };

	//�{�X�̏邩��o���Ƃ��ɂ̍��W
	const tnl::Vector3 BOSS_EXIT_POS = {3039 , 1145 , 0};													

	//�{�X�̊Ԃɓ��������̍��W
	const tnl::Vector3 BOSS_ROOM_FIRS_POS = { 805 , 895 , 0 };												

	//�{�X�̊Ԃ���o���Ƃ��̍��W
	const tnl::Vector3 BOSS_ROOM_EXIT_POS = { 766 , 103 , 0 };		

	//������o�����̍��W
	const tnl::Vector3 TOWN_EXIT_POS = { 1536, 1195 , 0 };				

	//�鉺������o�����̍��W
	const tnl::Vector3 CASTLE_TOWN_EXIT_POS = { 1429, 155 , 0 };

	//��ɓ��������̍ŏ��̈ʒu
	const tnl::Vector3 CASTLE_FIRST_POS = { 1111,1674 , 0 };	

	//�邩��鉺���ɏo�����̍��W
	const tnl::Vector3 CASTLE_EXIT_POS = { 1114 , 100 , 0 };	

	//�E�҂̕悩��o�����̍��W
	const tnl::Vector3 GRAVE_EXIT_POS = { 2625 , 98 , 0 };		

	//�D����o���Ƃ��̍��W
	const tnl::Vector3 SHIP_EXIT_POS = { 138 , 1032 , 0 };

	//���V�[���ł̓����蔻��
	void VillageCollision();

	//���V�[���ł̓����蔻��
	void TownCollision();

	//�{�X�̏�ł̓����蔻��
	void BossCastleCollision();

	//�{�X�̊Ԃł̓����蔻��
	void BossRoomCollision();

	//�鉺���V�[���̓����蔻��
	void CastleTownCollision();

	//��V�[���̓����蔻��
	void CastleCollision();

	//�E�҂̕�V�[���̓����蔻��
	void GraveSceneCollision();

	//�D�ɓ������ۂ̓����蔻��
	void ShipCollision();

//------------------------------------------------------------------------------------------------------------------------
//---�|�C���^---//

	//Npc�֘A
	
	//�����@||�@����
	Shared<Npc>herdman_npc = nullptr;

	//���폤�l
	Shared<Npc>armsdealer_npc = nullptr;

	//�_��
	Shared<Npc>priest_npc = nullptr;

	//�e�̔Ԑl
	Shared<Npc>shadow_npc = nullptr;

	//�{�X
	Shared<Npc>boss_npc = nullptr;

	//���퉮
	Shared<WeaponShop>weapon_shop = nullptr;

	//�h��
	Shared<Npc>inn_npc = nullptr;

	//�Z�l(����)
	Shared<Npc>female_resident_npc = nullptr;

	//�Z�l(�j��)
	Shared<Npc>resident_male_npc = nullptr;

	//���m
	Shared<Npc>soldier_npc = nullptr;

	//���m��l��
	Shared<Npc>second_soldier_npc = nullptr;

	//�G
	Shared<Npc>enemy_npc = nullptr;

	//��
	Shared<Npc>king_npc = nullptr;

	//����
	Shared<Npc>queen_npc = nullptr;

	//�����蔻��̂Ȃ�Npc
	Shared<Npc>ordeal_npc = nullptr;

	//����
	Shared<Npc>corporal_npc = nullptr;

	//�D��
	Shared<Npc>pirate_npc = nullptr;

	//�m��(���X�{�X��)
	Shared<Npc>monk_npc = nullptr;

	//---���j���[�֘A---//

	//�b���R�}���h
	Shared<MenuWindow>dia_log_coment_select = nullptr;
	MenuWindow::MenuElement_t* dia_log_coment = nullptr;

	//���폤�l�̍w���R�}���h
	Shared<MenuWindow>weapon_shop_coment_select = nullptr;
	MenuWindow::MenuElement_t* weapon_shop_coment = nullptr;

//------------------------------------------------------------------------------------------------------------------------
//---�V�[�P���X---//

public:

	//�V�[�P���X���Ǘ�����N���X
	enum class Sequence_num {
		//������
		INIT,
		//�s����
		MOVE,
		//���퉮
		WEAPONSHOP,
		//���j���[���J���Ă��鎞
		MENUOPEN,
		//npc�Ƙb���Ă��鎞
		MENUNPC
	};


	//���̏��ŃV�[�P���X��؂�ւ���
	void SequenceChangeMove()override;

	//�ǂ̃V�[�P���X���I������Ă��邩
	 Sequence_num select_num = Sequence_num::INIT;

	//�V�[�P���X�̒�`
	tnl::Sequence<InMapScene> sequence_ = tnl::Sequence<InMapScene>(this, &InMapScene::seqInit);

	//�}�b�v������������
	bool seqInit(float delta_time);

	//�ʏ펞�̃V�[�P���X
	bool seqMove(float delta_time);

	//���퉮�̃V�[�P���X
	bool seqWeaponShop(float delta_time);

	//���j���[�V�[�P���X
	bool seqMenuOpen(float delta_time);

	//�V�[����ς���ׂ̃V�[�P���X
	bool seqChangeScene(float delta_time);

	//Npc�̃V�[�P���X
	bool seqNpc(float delta_time);

	//�V�[�P���X��؂�ւ������̏���
	void switchSequenceDraw();

//------------------------------------------------------------------------------------------------------------------------
//---Npc�֌W---//	
private:

	//NPC�̃`�b�v�T�C�Y
	const int NPC_CHIP_SIZE = 48;

	//----NPC�̍��W�n----//

	//---����---//
	const tnl::Vector3 HERDMAN_POS = { 1375, 760, 0 };

	//---�m��---//
	const tnl::Vector3 MONK_POS = { 909,235,0 };


	//---���폤�l---//
	
	//��
	const tnl::Vector3 ARMSDEALER_VILLAGE_POS = { 1150, 140, 0 };
	//��
	const tnl::Vector3 ARMSDEALER_TOWN_POS = { 1730, 580, 0 };
	//�鉺��
	const tnl::Vector3 ARMSDEALER_CASTLE_TOWN_POS = { 495, 329, 0 };

	//---�_��---//
	 
	//��
	const tnl::Vector3 PRIEST_VILLAGE_POS = { 239, 740, 0 };
	//�鉺��
	const tnl::Vector3 PRIEST_CASTLE_TOWN_POS = { 336, 682, 0 };

	//---���l---//

	//�j�����l
	const tnl::Vector3 MALE_VILLAGERS_POS = { 1475, 360, 0 };
	//�������l
	const tnl::Vector3 FEMALE_VILLAGERS_POS = { 585, 667, 0 };

	//---�G�֌W---//

	//���{�X
	const tnl::Vector3 ENEMY_POS = { 2032, 205, 0 };
	//���{�X3�̖�(�e�̔Ԑl)
	const tnl::Vector3 SHADOW_ENEMY_POS = { 769 , 591 , 0 };
	//���X�{�X
	const tnl::Vector3 LAST_BOS_POS = { 801, 153, 0 };

	//---�h��---//

	//��
	const tnl::Vector3 INN_TOWN_POS = { 780, 1062, 0 };
	//�鉺��
	const tnl::Vector3 INN_CASTLE_TOWN_POS = { 1814, 678, 0 };

	//---����---//
	const tnl::Vector3 TOWN_HERDMAN_POS = { 220, 266, 0 };

	//---����---//

	//�j������
	const tnl::Vector3 TOWNS_PEOPLE_POS = { 1726, 1084, 0 };

	//---�鉺���̏Z�l---//

	//�j���Z�l
	const tnl::Vector3 MALE_RESIDENT_POS = { 1644, 914, 0 };

	//---��֘A---//

	//��
	const tnl::Vector3 KING_POS = { 944, 315, 0 };
	//����
	const tnl::Vector3 QUEEN_POS = { 1261, 315, 0 };
	//�w��
	const tnl::Vector3 SCHOLAR_POS = { 800, 1492, 0 };
	//���m
	const tnl::Vector3 SOLDIER_POS = { 972, 1060, 0 };
	//���m��l��
	const tnl::Vector3 SECOND_SOLDIER_POS = { 1265, 1063, 0 };
	//����
	const tnl::Vector3 CORPORAL_POS = { 1957 , 1531 , 0 };

//------------------------------------------------------------------------------------------------------------------------
//���j���[�֘A���W�n
private:

	//�_�C�A���O�̍��W
	const tnl::Vector2i DIALOG_POS{ 50 , 50 };

	//�_�C�A���O�̃T�C�Y
	const tnl::Vector2i DIALOG_SIZE = { 250 , 200 };

	//���퉮�R�}���h�̃T�C�Y
	const tnl::Vector2i WEAPON_SHOP_WINDOW_SIZE = { 250 , 230 };

//------------------------------------------------------------------------------------------------------------------------
//�v���C���[�֌W
private:

	//�v���C���[�̑��x
	const float PLAYER_VELOCITY = 4.0f;

};
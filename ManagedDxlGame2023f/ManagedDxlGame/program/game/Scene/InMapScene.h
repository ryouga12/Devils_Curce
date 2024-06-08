//
//�}�b�v�ɐݒu���Ă��鑺�⒬�Ȃǂ��Ǘ�����N���X
//


#pragma once
#include	<unordered_set>
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Object/NPC.h"
#include"../Item/WeaponShop.h"
#include"../Manager/SoundManager.h"
#include"../Manager/EventManager.h"

class UIManager;
class CsvManager;

class InMapScene : public BaseScene {
public:
	
	//InMapScene�̏��
	enum class InMapState {
		VILLAGE,		//��
		TOWN,			//��
		CASTLE,			//��
		BOSSCASTLE		//�{�X�̏�
	};

	InMapScene(tnl::Vector3 firstPos , InMapState inmap_state);
	~InMapScene()override;

	//�X�V����
	void Update(float delta_time)override;
	//�`�揈��
	void Draw()override;
	//InMapScene�ł̏�����
	void InitInMapScene(InMapState inmap_state);
	//InMapScene�̍X�V����
	void InMapSceneUpdate(float delta_time);
	//InMapScene�̕`�揈��
	void InMapSceneDraw();
	//���V�[���ł̏�����
	void InitVillageScene();
	//�_���W�����V�[���̏�����
	void InitDungeonScene();
	
	//���j���[�E�B���h�E�̐���
	void InitMenuWindow();


private:

	//���݂̃V�[�� : �ŏ��͑�����n�܂��VILLAGE
	InMapState curent_inmap_scene = InMapState::VILLAGE;

	//���Ԍo�߂Ő؂�ւ���t���O
	bool TimeCount = false;

	//�x��������b��
	float TimeSE = 0.2f;


//------------------------------------------------------------------------------------------------------------------------
//MapChip�֘A
public:

	//�}�b�v�`�b�v���i�[����ꏊ
	std::list<Shared<MapChip>> MapChips;
	//�I�u�W�F�N�g�p�̃}�b�v�`�b�v
	std::list<Shared<MapChip>> MapChip_object;							
	
	//---�`�悷��ۂ̃t���O---//
	bool drawWindowFlag = true;
	bool drawSeqFlag = false;

private:
	/*TNL_CO_SEQUENCE(VillageScene , &VillageScene::seqIdle)*/

	//---mapChip�֘A�̏���---//
	void mapChip_Loding(std::vector<std::vector<int>>map_csv);												//mapChip�̓ǂݍ��݊֐�
	void mapChip_collision(std::vector<std::vector<int>>map_csv);																				//mapChip�̂����蔻��֐�
	
	//---mapChip�֘A�̕ϐ�---//
	std::string gpc_map_chip_hdls_pass_;																	// �摜�p�X
	int map_chip_width_;																					// �}�b�v�`�b�v�̕�
	int map_chip_height_;																					// �}�b�v�`�b�v�̍���
	int map_chip_all_size_;																					// �}�b�v�`�b�v�̑��t���[����
	int map_chip_x_size_;																					// �}�b�v�`�b�v�̉��t���[����
	int map_chip_y_size_;																					// �}�b�v�`�b�v�̏c�t���[����
	int gpc_map_chip_hdls_[1992];																			// �摜�f�[�^�i�[
	std::string map_data_csv_pass_;																			// �}�b�vCSV�f�[�^�̃p�X
	int map_height = 945;																					//�}�b�v�̍���
	int map_width = 1600;																					//�}�b�v�̕�

	tnl::Vector3 first_pos = {};																			//���̍ŏ��̍��W
	const tnl::Vector3 FIRST_WORLD_POS = { 435 , 1770 , 0 };	//worldMap�ł̍ŏ��̈ʒu
	const tnl::Vector3 BOSS_EXIT_POS = {5692 , 1978 , 0};

	const int map_Chip_WeaponShop = 510;																	//���퉮�̃}�b�v�`�b�v
	const int map_Chip_Herdman = 708;																		//�����̃}�b�v�`�b�v
	

	// ---���̔z��̒��ɓ����Ă���l���g���Ă����蔻������߂�---//
	//���V�[��
	const std::unordered_set<int> village_colision_values = {-1, 324, 384, 392, 545, 528, 536, 464, 631, 708, 614, 630, 510, 400, 408, 657, 656, 395, 230, 231};

	//���V�[���ł̓����蔻��
	void VillageCollision(std::vector<std::vector<int>>map_csv);

	//�_���W�����V�[��	
	/*const std::unordered_set<int> doungen_collision_values = { };*/

	//�_���W�����V�[���ł̓����蔻��
	void DungeonCollison();

//------------------------------------------------------------------------------------------------------------------------
//---�|�C���^---//

	//���j���[�E�B���h�E
	Weak<Menu>menuWindow;
	//Npc�֘A
	//����
	Shared<Npc>herdman_npc = nullptr;
	//���폤�l
	Shared<Npc>armsdealer_npc = nullptr;
	//�_��
	Shared<Npc>priest_npc = nullptr;
	//�{�X
	Shared<Npc>boss_npc = nullptr;
	//���퉮
	Shared<WeaponShop>weapon_shop = nullptr;
	//�}�b�v�`�b�v
	Shared<MapChip>mapchip = nullptr;

//------------------------------------------------------------------------------------------------------------------------
//---�V�[�P���X---//

	//�V�[�P���X���Ǘ�����N���X
	enum class Sequence_num {
		Idle,
		WeaponShop,
		MenuOpen,
		MenuNpc
	};

	//�ǂ̃V�[�P���X���I������Ă��邩
	 Sequence_num select_num;

	//�V�[�P���X�̒�`
	tnl::Sequence<InMapScene> sequence_ = tnl::Sequence<InMapScene>(this, &InMapScene::seqIdle);

	//�ʏ펞�̃V�[�P���X
	bool seqIdle(float delta_time);

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

	//�}�b�v�����[�h����V�[�P���X
	bool LoadMapSequence();

	
//------------------------------------------------------------------------------------------------------------------------
//---����֌W---//

//------------------------------------------------------------------------------------------------------------------------
//---Npc�֌W---//	

	//---���V�[��---//
	 std::string herdman_hdl_pass[4] = {"graphics/Npc/herdman.png" ,"graphics/Npc/herdman_idle.png" ,"graphics/Npc/herdman_right.png","graphics/Npc/herdman_up.png" };
	 std::string Armsdealer_hdl_pass[1] = {"graphics/Npc/Armsdealer.png"};
	 std::string Priest_hdl_pass[3] = { "graphics/Npc/Priest_npc.png" , "graphics/Npc/Priest_left.png" , "graphics/Npc/Priest_right.png" };

	 //---�_���W�����V�[��---//
	 std::string boss_hdl_pass[1] = {"graphics/Enemy/maou_idle.png"};

	 //�{�X�V�[���ł̔w�i
	 int boss_background_hdl = 0;

	//���퉮�̃R�����g
	int armsdealer_comet = 1;

	//npc���R�����g�\��
	bool NpcComntTrue = true;

//------------------------------------------------------------------------------------------------------------------------
//�C���x���g���֌W
private:

	//�C���x���g�����J��
	int first_menu = 1;

//------------------------------------------------------------------------------------------------------------------------
//�v���C���[�֌W
private:

	//�v���C���[�̃T�C�Y
	float PlyerScale = 1.5f;

	//�v���C���[�̑��x
	float PlayerVelocity = 5.0f;

};
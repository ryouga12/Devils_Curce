//
//�}�b�v�ɐݒu���Ă��鑺�⒬�Ȃǂ��Ǘ�����N���X
//


#pragma once
#include	<unordered_set>
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Object/NPC.h"
#include"../Manager/SoundManager.h"
#include"../Manager/EventManager.h"

class UIManager;
class CsvManager;
class WeaponShop;

class InMapScene : public BaseScene {
public:
	
	//InMapScene�̏��
	enum class InMapState {
		VILLAGE,		//��
		TOWN,			//��
		CASTLETOWN,		//�鉺��
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
	void InitInMapScene();
	//InMapScene�̍X�V����
	void InMapSceneUpdate(float delta_time);
	//InMapScene�̕`�揈��
	void InMapSceneDraw();
	//���V�[���ł̏�����
	void InitVillageScene();
	//�_���W�����V�[���̏�����
	void InitDungeonScene();
	//���V�[���̏�����
	void InitTownScene();
	//�鉺���V�[��������
	void InitCastleTownScene();
	//��V�[���̏�����
	void InitCastleScene();
	
	//���j���[�E�B���h�E�̐���
	void InitMenuWindow();
	
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
public:

	//�}�b�v�`�b�v���i�[����ꏊ
	std::list<Shared<MapChip>> MapChips;
	//�I�u�W�F�N�g�p�̃}�b�v�`�b�v
	std::list<Shared<MapChip>> MapChip_object;		

	//csv���i�[���Ă����p��vector(���p)
	std::vector<std::vector<int>>map_csv_;

	//�I�u�W�F�N�g�p
	std::vector<std::vector<int>>map_csv_objct_;
	
	//---�`�悷��ۂ̃t���O---//
	bool drawWindowFlag = true;
	bool drawSeqFlag = false;

private:
	/*TNL_CO_SEQUENCE(VillageScene , &VillageScene::seqIdle)*/

	//---mapChip�֘A�̏���---//
	
	//mapChip�̓ǂݍ��݊֐�
	void mapChip_Loding(std::vector<std::vector<int>>map_csv, std::vector<std::vector<int>>map_object_csv );


	void mapChip_collision();																				//mapChip�̂����蔻��֐�
	
	//---mapChip�֘A�̕ϐ�---//
	std::string gpc_map_chip_hdls_pass_ = "graphics/image_(2024_0115_0231).png";							// �摜�p�X
	int map_chip_width_ = 32;																				// �}�b�v�`�b�v�̕�
	int map_chip_height_ = 32;																				// �}�b�v�`�b�v�̍���
	int map_chip_all_size_ = 0;																				// �}�b�v�`�b�v�̑��t���[����
	int map_chip_x_size_ = 8;																				// �}�b�v�`�b�v�̉��t���[����
	int map_chip_y_size_ = 262;																				// �}�b�v�`�b�v�̏c�t���[����
	int gpc_map_chip_hdls_[2096] = {};																		// �摜�f�[�^�i�[
	std::string map_data_csv_pass_;																			// �}�b�vCSV�f�[�^�̃p�X
	int map_height = 945;																					//�}�b�v�̍���
	int map_width = 1600;																					//�}�b�v�̕�

	tnl::Vector3 first_pos = {};																			//���̍ŏ��̍��W
	const tnl::Vector3 FIRST_WORLD_POS = { 435 , 1790 , 0 };												//worldMap�ł̍ŏ��̈ʒu
	const tnl::Vector3 BOSS_EXIT_POS = {5692 , 2046 , 0};													//�{�X�̏邩��o���Ƃ��ɂ̍��W
	const tnl::Vector3 TOWN_EXIT_POS = { 2240, 2056 , 0 };													//������o�����̍��W
	const tnl::Vector3 CASTLE_TOWN_EXIT_POS = { 2683, 244 , 0 };											//�鉺������o�����̍��W
	const tnl::Vector3 CASTLE_FIRST_POS = { 1111,1674 , 0 };												//��ɓ��������̍ŏ��̈ʒu
	const tnl::Vector3 CASTLE_EXIT_POS = { 1114 , 100 , 0 };												//�邩��鉺���ɏo�����̍��W

	const int map_Chip_WeaponShop = 510;																	//���퉮�̃}�b�v�`�b�v
	const int map_Chip_Herdman = 708;																		//�����̃}�b�v�`�b�v
	
	//���V�[���ł̓����蔻��
	void VillageCollision();

	//���V�[���ł̓����蔻��
	void TownCollision();

	//�_���W�����V�[���ł̓����蔻��
	void DungeonCollison();

	//�鉺���V�[���̓����蔻��
	void CastleTownCollision();

	//��V�[���̓����蔻��
	void CastleCollision();

//------------------------------------------------------------------------------------------------------------------------
//---�|�C���^---//

	//���j���[�E�B���h�E
	Weak<Menu>menuWindow;
	//Npc�֘A
	//�����@||�@����
	Shared<Npc>herdman_npc = nullptr;
	//���폤�l
	Shared<Npc>armsdealer_npc = nullptr;
	//�_��
	Shared<Npc>priest_npc = nullptr;
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
	//�G
	Shared<Npc>enemy_npc = nullptr;
	//��
	Shared<Npc>king_npc = nullptr;
	//����
	Shared<Npc>queen_npc = nullptr;
	//�}�b�v�`�b�v
	Shared<MapChip>mapchip = nullptr;
	//�b���R�}���h
	Shared<MenuWindow>dia_log_coment_select = nullptr;
	MenuWindow::MenuElement_t* dia_log_coment = nullptr;

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
	void SequenceChange(Sequence_num secect_sequence);

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

	 //---�摜�z��---//
	 //�����A����
	 std::string herdman_hdl_pass[4];
	 //���폤�l
	 std::string Armsdealer_hdl_pass[4];
	 //�_��
	 std::string Priest_hdl_pass[4] = { "graphics/Npc/Priest_left.png" , "graphics/Npc/Priest_right.png","graphics/Npc/Priest_Up.png" ,"graphics/Npc/Priest_npc.png"};
	 //�h��
	 std::string inn_hdl_pass[4];
	 //����
	 std::string enemy_npc_pass[4];
	 //�{�X
	 std::string boss_hdl_pass[4] = { "graphics/Npc/maou_left.png" ,"graphics/Npc/maou_right.png","graphics/Npc/maou_up.png","graphics/Npc/maou_idle.png"};
	 //��
	 std::string king_hdl_pass[4] = { "graphics/Npc/king_left.png" ,"graphics/Npc/king_right.png","graphics/Npc/king_up.png","graphics/Npc/king_idle.png" };
	 //����
	 std::string queen_hdl_pass[4] = { "graphics/Npc/queen_left.png" ,"graphics/Npc/queen_right.png","graphics/Npc/queen_up.png","graphics/Npc/queen_idle.png" };
	 //�Z�l(����)
	 std::string female_resident_hdl_pass[4];
	 //�Z�l(�j��)
	 std::string resident_male_hdl_pass[4];

	 //�{�X�V�[���ł̔w�i
	 int boss_background_hdl = 0;

	//���퉮�̃R�����g
	int armsdealer_comet = 1;

	//NPC�̑傫��
	const float NPC_SCALE = 1.5f;

public:


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
	float PlayerVelocity = 4.0f;

};
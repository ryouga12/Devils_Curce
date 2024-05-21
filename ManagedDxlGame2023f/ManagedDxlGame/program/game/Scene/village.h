//
//���N���X�i��ɉ����̑������̃N���X�ŊǗ�����\��j
//


#pragma once
#include	<unordered_set>
#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Object/NPC.h"
#include"../Item/WeaponShop.h"
#include"../Manager/SoundManager.h"
#include"../Manager/EventManager.h"

class VillageScene : public BaseScene {
public:
	
	VillageScene(tnl::Vector3 firstPos);
	~VillageScene()override;

	//�X�V����
	void Update(float delta_time)override;
	//�`�揈��
	void Draw()override;
	//������
	void InitVillageScene();
	//���j���[�E�B���h�E�̐���
	void InitMenuWindow();

private:

	//���Ԍo�߂Ő؂�ւ���t���O
	bool TimeCount = false;

	//�x��������b��
	float TimeSE = 0.2f;


//------------------------------------------------------------------------------------------------------------------------
//MapChip�֘A
public:

	//mapChip���i�[����ꏊ
	std::list<Shared<MapChip>> MapChips;
	std::vector<std::vector<int>> map_csv;								
	
	//---�`�悷��ۂ̃t���O---//
	bool drawWindowFlag = true;
	bool drawSeqFlag = false;

private:
	/*TNL_CO_SEQUENCE(VillageScene , &VillageScene::seqIdle)*/

	//---mapChip�֘A�̏���---//
	void mapChip_Loding();												//mapChip�̓ǂݍ��݊֐�
	void mapChip_collision();											//mapChip�̂����蔻��֐�
	
	//---mapChip�֘A�̕ϐ�---//
	std::string gpc_map_chip_hdls_pass_;								// �摜�p�X
	int map_chip_width_;												// �}�b�v�`�b�v�̕�
	int map_chip_height_;												// �}�b�v�`�b�v�̍���
	int map_chip_all_size_;												// �}�b�v�`�b�v�̑��t���[����
	int map_chip_x_size_;												// �}�b�v�`�b�v�̉��t���[����
	int map_chip_y_size_;												// �}�b�v�`�b�v�̏c�t���[����
	int gpc_map_chip_hdls_[1992];										// �摜�f�[�^�i�[
	std::string map_data_csv_pass_;										// �}�b�vCSV�f�[�^�̃p�X
	const int MAP_HEIGHT = 945;											//�}�b�v�̍���
	const int MAP_WIDTH = 1600;											//�}�b�v�̕�

	tnl::Vector3 village_Pos = {};										//���̍ŏ��̍��W
	tnl::Vector3 first_world_pos = { 435 , 1770 , 0 };					//worldMap�ł̍ŏ��̈ʒu

	const int map_Chip_WeaponShop = 510;								//���퉮�̃}�b�v�`�b�v
	const int map_Chip_Herdman = 708;									//�����̃}�b�v�`�b�v
	const int map_Chip_Object = 545;									//�I�u�W�F�N�g�̃}�b�v�`�b�v
	

	// ---���̔z��̒��ɓ����Ă���l���g���Ă����蔻������߂�---//
	const std::unordered_set<int> colisionValues = {-1, 324, 384, 392, 545, 528, 536, 464, 631, 708, 614, 630, 510, 400, 408, 657, 656, 395, 230, 231};
		
//------------------------------------------------------------------------------------------------------------------------
//---�|�C���^---//

	//���j���[�E�B���h�E
	Shared<Menu>menuWindow = nullptr;
	//�I�����j���[�E�B���h�E
	Shared<Menu>menuSelectWindow = nullptr;
	//���j���[�̃R�����g�E�B���h�E
	Shared<Menu>menuComentWindiow = nullptr;
	//Npc�֘A
	Shared<Npc>herdman_npc = nullptr;
	Shared<Npc>Armsdealer_npc = nullptr;
	Shared<Npc>Priest_npc = nullptr;
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
		MenuShopOpen,
		MenuNpc
	};

	//�ǂ̃V�[�P���X���I������Ă��邩
	 Sequence_num select_num;

	//�V�[�P���X�̒�`
	tnl::Sequence<VillageScene> sequence_ = tnl::Sequence<VillageScene>(this, &VillageScene::seqIdle);

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
	
//------------------------------------------------------------------------------------------------------------------------
//---����֌W---//

//------------------------------------------------------------------------------------------------------------------------
//---Npc�֌W---//				
	std::string herdman_hdl_pass[4] = {"graphics/Npc/herdman.png" ,"graphics/Npc/herdman_idle.png" ,"graphics/Npc/herdman_right.png","graphics/Npc/herdman_up.png" };
	std::string Armsdealer_hdl_pass[1] = {"graphics/Npc/Armsdealer.png"};
	std::string Priest_hdl_pass[3] = { "graphics/Npc/Priest_npc.png" , "graphics/Npc/Priest_left.png" , "graphics/Npc/Priest_right.png" };

	//���퉮�̃R�����g
	int armsdealer_comet = 1;

	//Npc�̕��ƍ������擾����ׂ̕ϐ�
	int npcWidth = 1; int npcHeight = 2;

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

	//�v���C���[�̃R���g���[���𐧌䂷��
	int PlyControlTrue = 0; int PlyControlFalse = 1;

	//�v���C���[�̃T�C�Y���擾����ׂ̃^�C�v
	int PlyerWidth = 1; int PlyerHeight = 2;

	//�v���C���[�̑��x
	float PlayerVelocity = 5.0f;

};
#pragma once

//------------------------------------------
// �V���O���g���݌v


#include"../Manager/ResourceManager.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/GameManager.h"
#include<unordered_map>


class EventManager final {

public:

	static EventManager* getEventManager();
	static void delateEventManager() { delete getEventManager(); }

private:

	//���j���[��ʂŕ\������A�C�R���I�Ȃ���
	int Armsdealer_icon_hdl = 0;
	int herdman_icon_hdl = 0;
	int Priest_icon_hdl = 0;
	int Player_icon_hdl = 0;

	EventManager();

	~EventManager();

	//���j���[�̎g������
	enum class MenuOpenNum {
		Empty,
		WeaponShop,			//���퉮
		Herdman,			//����
		Priest,				//�_��
		Object				//�I�u�W�F�N�g
	};

	//�ǂ̃��j���[���J���Ă��邩
	MenuOpenNum select_menu_num = MenuOpenNum::Empty;

	//Npc�ɂ���ē����ς���
	enum class NpcMove {
		Empty,				//��
		Herdman,			//����
		Priest				//�_��
	};

	//�N��������s����
	NpcMove npcMove = NpcMove::Empty;

	//---����---//

	//�����̃A�C�R���̍��W
	tnl::Vector2i herdman_icon_pos{ 110 , 600 };

	//�����̃R�����g�̍��W
	tnl::Vector2i herdman_coment_pos = { 150 , 520 };

	//�����̃T�C�Y
	float herdman_scale = 1.5f;

	//�����̃R�����g
	std::string herdman_coment[3] = {"���܂ł��̑�n�͂��̈����Ɏx�z����Ă���","��������ȏ�ς�����Ȃ�","���̈����͉E�̌͂ꂽ��n��" };
	std::string herdman_coment_2[2] = {"������ĂďZ��ł���ƕ�����","�ǂ����͂����ē|���Ă���"};

	//�ǂ̃R�����g��\�����邩
	bool herdman_first_coment = true;
	bool herdman_second_coment = false;

	//������A�j���[�V�������������\�L���\�����邩
	int add_num = 1;

	//�s��
	int line = 0;
	
	//�������𐧌�����
	int count = 0;

	//��������i�[����z��
	std::string text[2];

	//������A�j���[�V�������s�������̃t���O
	bool ComentAnimation_flag = false;

public:

	//menu�֘A�̏�����
	void InitMenu();

	//npc��Draw��؂�ւ���
	void NpcComentDraw();

	//���퉮�̃R�����g�̕\��
	void armsdealerComentDraw(int type);

	//�I�u�W�F�N�g�̃R�����g�\��
	void objectComentDraw();

	// ���������`�����֐�
	bool isNpcMenuOpen();

	//���j���[�̐ؑ�
	MenuOpenNum SetMenuNpc(int npcType);

	//Npc�̃R�����g��\������
	void NpcSwitchComent();

	//NPC�̃R�����g�̐؂�ւ�
	NpcMove SetNpcComent(int npcType);

	//�R�����g�̃A�j���[�V����
	void NpcComent(int x , int y ,int max_coment_num , std::string npc_coment[]);

	//�R�����g��ǂݍ���
	void ComentLoad(const std::string& newComent);


private:

	//���폤�l�̃R�����g�̍��W
	tnl::Vector2i armsdealerComent = { 150, 550 };

	//���퉮�̘b���������Ƃ��̃R�����g
	int armsdealer_first_comet = 0;

	//���퉮�̕����̍��W
	tnl::Vector2i weaponShop = { 60 , 520 };

	//���퉮�̍w���R�����g�̍��W
	tnl::Vector2i WeaponShop_buy = { 70, 100 };

	//���퉮�̃��j���[�����R�����g�̍��W
	tnl::Vector2i menu_close = { 70, 150 };

	//���F
	int Color_White = -1;

	//���퉮�̃R�����g
	int armsdealer_comet = 1;

	//���퉮�ɘb�������Ƃ��̃R�����g
	int armsdealer_speak_coment = 0;

	//�I�u�W�F�N�g��G�������̃R�����g�̍��W
	tnl::Vector2i object_coment = { 150 , 550 };

	//���j���[�����R�����g�̕`����W
	tnl::Vector2i object_menu_close = { 150 ,650 };

	//�_���̃R�����g
	std::string priest_coment[2] = {"�悭������","�L�^���Ă������H"};

	//�_���̃R�����g�̍��W
	tnl::Vector2i priest_coment_pos = { 150, 570 };

	//�R�����g�̍ő吔
	int coment_max_num = 3;

	//�R�����g���Ǘ����邽�߂̃}�b�v
	std::vector<std::string>Npc_coment_Array;


//--------------------------------------------------------------------------------------------------------
//���ꂼ��̃t���O

private:

	//---�o�g���V�[���Ŏg���t���O---//

//--------------------------------------------------------------------------------------------------------
//�|�C���^
private:

	//�I�����j���[�E�B���h�E
	Shared<Menu>menuSelectWindow = nullptr;
	//���j���[�̃R�����g�E�B���h�E
	Shared<Menu>menuComentWindiow = nullptr;

};
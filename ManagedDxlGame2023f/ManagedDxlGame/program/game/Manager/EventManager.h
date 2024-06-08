//------------------------------------------------------------------------------------------------------------
//
//�C�x���g���Ǘ�����}�l�[�W���[(��Ƀg���K�[�̖���)
// 
//
//-------------------------------------------------------------------------------------------------------------



#pragma once

#include"../Manager/ResourceManager.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/GameManager.h"
#include<unordered_map>

class UIManager;

class EventManager final {

public:

	static EventManager* getEventManager();
	static void delateEventManager() { delete getEventManager(); }

	//���j���[�̎g������
	enum class MenuOpen {
		EMPTY,
		WEAPONSHOP,			//���퉮
		//HERDMAN,			//����
		//PRIEST,				//�_��
		BOSS,				//�{�X�����X�^�[
	};

	//Npc�ɂ���ē����ς���
	enum class NpcMove {
		EMPTY,				//��
		HERDMAN,			//����
		PRIEST,				//�_��
		WEAPONSHOP,			//���퉮
		BOSS				//�{�X�����X�^�[
	};

private:

	EventManager();

	~EventManager();

	//���j���[��ʂŕ\������A�C�R���I�Ȃ���
	int Armsdealer_icon_hdl = 0;
	int herdman_icon_hdl = 0;
	int Priest_icon_hdl = 0;
	int Player_icon_hdl = 0;

	//�ǂ̃��j���[���J���Ă��邩
	MenuOpen select_menu = MenuOpen::EMPTY;

	//�N��������s����
	NpcMove npcMove = NpcMove::EMPTY;

	//---����---//

	//�����̃A�C�R���̍��W
	const tnl::Vector2i HERDMAN_ICON_POS{ 110 , 600 };

	//�����̃R�����g�̍��W
	const tnl::Vector2i HERDMAN_COMENT_POS = { 150 , 580 };

	//�����̃T�C�Y
	const float HERDMAN_SCALE = 1.5f;

	//�{�X�����X�^�[�̃R�����g���W
	const tnl::Vector2i BOSS_COMENT_POS = { 150 , 580 };

	//�{�X�̕\������R�����g��
	const int BOSS_COMENT_MAX_NUM = 4;

	//������A�j���[�V�������������\�L���\�����邩
	int add_num = 1;

	//�s��
	int line = 0;
	
	//�������𐧌�����
	int count = 0;

	//��������i�[����z��
	std::string text[2];

	//������A�j���[�V�������s�������̃t���O
	bool coment_animation_flag = false;

public:

	//menu�֘A�̏�����
	void InitMenu();

	//npc��Draw��؂�ւ���
	void EventMenuWindow();

	//���j���[�̐ؑ�
	void SetMenuNpc(MenuOpen new_menu) {
		select_menu = new_menu;
	}

	//���݂̃��j���[���擾����
	MenuOpen GetSelectMenu()const {
		return select_menu;
	}

	//Npc�̃R�����g��\������
	void NpcSwitchComent();

	//NPC�̃R�����g�̐؂�ւ�
	void SetNpcComent(NpcMove npc) {
		npcMove = npc;
	}

	//���݂�NPC�̏�Ԃ����擾����
	NpcMove GetNpcMove()const {
		return npcMove;
	}
	
	//�R�����g�𗬂��؂��Ă���Enter�L�[�ŃV�[����J�ڂ�����
	bool ComentBattleChange(const tnl::Vector2i& coment_pos, const int count_max);

private:

	//���F
	const int COLORWHITE = -1;

	//���퉮�ɘb�������Ƃ��̃R�����g
	const int ARMSDEALER_SPEAK_COMENT = 0;

	//�_���̃R�����g
	std::string priest_coment[2] = {"�悭������","�L�^���Ă������H"};

	//�_���̃R�����g�̍��W
	const tnl::Vector2i PRIEST_COMENT_POS = { 150, 570 };

	//�{�X�̔w�i
	int boss_background_hdl = 0;


//--------------------------------------------------------------------------------------------------------
//�|�C���^
private:

	//�I�����j���[�E�B���h�E
	Weak<Menu>menu_window;

};
//------------------------------------------------------------------------------------------------------------
//
//�C�x���g���Ǘ�����}�l�[�W���[(��Ƀg���K�[�̖���)
// NPC�Ȃǂ̉�b���s��ꂽ�ۂɏ�����`��Ȃǂ�C����
//
//-------------------------------------------------------------------------------------------------------------



#pragma once

#include"../Manager/ResourceManager.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/GameManager.h"
#include<unordered_map>


class UIManager;
class SceneManager;
class WeaponShop;
class EventNpc;

class EventManager final {

public:

	static EventManager* getEventManager();
	static void delateEventManager() { delete getEventManager(); }

	//���j���[�̎g������
	enum class MenuOpen {
		EMPTY,
		WEAPONSHOP,			//���퉮
		//HERDMAN,			//����
		//PRIEST,			//�_��
		BOSS,				//�{�X�����X�^�[
	};

	//Npc�ɂ���ē����ς���
	enum class NpcMove {
		EMPTY,				//��
		HERDMAN,			//����
		PRIEST,				//�_��
		WEAPONSHOP,			//���퉮
		TOWNHERDMAN,		//����
		INN,				//�h��
		ENEMY,				//�G
		BOSS,				//�{�X�����X�^�[
		KING,				//��
		QUEEN,				//����
		FEMALE_RESIDENT,	//�������l
		RESIDENT_MALE		//�j�����l
	};

private:

	EventManager();

	~EventManager();

	//���j���[��ʂŕ\������A�C�R���I�Ȃ���

	//���폤�l
	int Armsdealer_icon_hdl = 0;
	//����
	int herdman_icon_hdl = 0;
	//�_��
	int Priest_icon_hdl = 0;
	//����
	int town_herdman_icon_hdl = 0;
	//�h��
	int inn_icon_hdl = 0;
	//�{�X
	int boss_icon_hdl = 0;
	//�G
	int enemy_icon_hdl = 0;
	//��
	int king_icon_hdl = 0;
	//����
	int queen_icon_hdl = 0;
	//�������l
	int female_resident_icon_hdl = 0;
	//�j�����l
	int male_villagers_icon_hdl = 0;

	//�ǂ̃��j���[���J���Ă��邩
	MenuOpen select_menu = MenuOpen::EMPTY;

	//�N��������s����
	NpcMove npcMove = NpcMove::EMPTY;

	//�{�X��ID
	const int BOSS_ID = 20;

	//���{�X��ID
	const int MEDIUM_BOSS = 21;

	//�h���̔�p
	const int INNMONEY = 5;

	//---�R�����g�\��---//

	//�����̃A�C�R���̍��W
	const tnl::Vector2i COMENT_ICON_POS{ 110 , 600 };

	//�����̃R�����g�̍��W
	const tnl::Vector2i COMENT_POS = { 150 , 580 };

	//NPC�̃T�C�Y
	const float NPC_SCALE = 1.5f;

	//�{�X�̕\������R�����g��
	const int BOSS_COMENT_MAX_NUM = 4;

	//������A�j���[�V�������s�������̃t���O
	bool coment_animation_flag = false;

public:

	//������
	void InitEventRelated(std::list<Shared<Actor>>& npc_list);

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
	void NpcSwitchComent(const Shared<WeaponShop>& weapon_shop);

	//Npc�̂��ꂼ��ł̍X�V����
	void NpcEventUpdate(float delta_time, InMapScene* inmap_scene, int curent_map_state, std::list<Shared<Actor>>& npc_list);

	//NPC�̃R�����g�̐؂�ւ�
	void SetNpcComent(NpcMove npc) {
		npcMove = npc;
	}

	//���݂�NPC�̏�Ԃ����擾����
	NpcMove GetNpcMove()const {
		return npcMove;
	}
	
	//�R�����g�𗬂��؂��Ă���Enter�L�[�ŃV�[����J�ڂ�����
	bool ComentEventChange(const int count_max);

	//�v���C���[�ɍs����I��������
	void PlayerAnswerSelect(int curent_map_state , InMapScene* inmap_scene, const std::list<Shared<Actor>>& npc_list);

	//�C�x���g�t���O��؂�ւ���
	void EventFlagChange() { event_npc_flag = !event_npc_flag; }

	//�C�x���g�t���O���擾����
	bool GetEventFlag()const { return event_npc_flag; }

	//�G�̃C�x���g�t���O���擾����
	//�t���O���Ǘ����Ē��{�X�Ȃǂ𐶐�����
	bool GetEnemyEventFlag()const { return enemy_event_flag; }

	//�G�̃t���O��؂�ւ���
	//�C�x���g���I��������t���O��؂�ւ��ďI��������
	void EnemyEventFlagChange() { enemy_event_flag = !enemy_event_flag; }

	//�R�����g��\������
	void ComentEvent(InMapScene* inmap_scene, const int coment_max, const std::list<Shared<Actor>>& npc_list);

private:

	//���F
	const int COLORWHITE = -1;

	//���퉮�ɘb�������Ƃ��̃R�����g
	const int ARMSDEALER_SPEAK_COMENT = 0;

	//�_���̃R�����g�̍��W
	const tnl::Vector2i PRIEST_COMENT_POS = { 150, 570 };

	//�{�X�̔w�i
	int boss_background_hdl = 0;

	//�I���R�����g���W
	const tnl::Vector3 PLAYER_ANSWER_POS = { 80 , 100 , 0 };

	//Npc�̏������̋����𐧌�����
	//true�̏ꍇ�̂݃C�x���g�𔭐�������
	bool event_npc_flag = true;

	//�G�̃C�x���g�Ǘ��t���O
	bool enemy_event_flag = true;

//--------------------------------------------------------------------------------------------------------
//�|�C���^
private:

	//���j���[�E�B���h�E
	Weak<Menu>menu_window;
	
	//�v���C���[�̑I���E�B���h�E
	Shared<MenuWindow>player_select_window = nullptr;
	//�E�B���h�E�̃R�����g
	MenuWindow::MenuElement_t* player_select_coment = nullptr;

	//�C�x���g�p�̊֐�
	Shared<EventNpc>function_npc = nullptr;

};
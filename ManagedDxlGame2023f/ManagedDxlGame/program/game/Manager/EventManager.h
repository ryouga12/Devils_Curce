//------------------------------------------------------------------------------------------------------------
//
//�C�x���g���Ǘ�����}�l�[�W���[(��Ƀg���K�[�̖���)
// NPC�Ȃǂ̉�b���s��ꂽ�ۂɏ�����`��Ȃǂ�C����
//
//-------------------------------------------------------------------------------------------------------------



#pragma once

#include"../Manager/ResourceManager.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"
#include"../Manager/GameManager.h"
#include<unordered_map>


class UIManager;
class SceneManager;
class WeaponShop;
class Event;
class BaseScene;
class Npc;

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
		FEMALE_RESIDENT,	//�����Z�l
		RESIDENT_MALE,		//�j���Z�l
		SCHOLAR,			//�w��
		ORDEAL				//�����̔Ԑl
	};

private:

	EventManager();

	~EventManager();

	//���j���[�֘A�̏�����
	void InitMenuEventConnection();

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
	//�����Z�l
	int female_resident_icon_hdl = 0;
	//�j���Z�l
	int resident_male_icon_hdl = 0;
	//�w��
	int scholar_icon_hdl = 0;
	//�����̔Ԑl
	int ordeal_icon_hdl = 0;

	//�ǂ̃��j���[���J���Ă��邩
	MenuOpen select_menu = MenuOpen::EMPTY;

	//�N��������s����
	NpcMove npc_move = NpcMove::EMPTY;

	//�{�X��ID
	int boss_id = 0;

	//�h���̔�p
	const int INNMONEY = 5;

	//�K�{�A�C�e��ID
	int ESSENTIAL_ITEM_ID = 31;

	//---�R�����g�\��---//

	//�A�C�R���̍��W
	const tnl::Vector2i COMENT_ICON_POS{ 110 , 600 };

	//�R�����g�̍��W
	const tnl::Vector2i COMENT_POS = { 150 , 580 };

	//�{�X�̕\������R�����g��
	const int BOSS_COMENT_MAX_NUM = 4;

	//������A�j���[�V�������s�������̃t���O
	bool coment_animation_flag = false;

	//�R�����g�E�B���h�E�̍��W
	const tnl::Vector2i COMENT_WINDOW_POS = { 50, 500 };

	//�R�����g�E�B���h�E�̃T�C�Y
	const int COMENT_WINDOW_WIDTH = 700;
	const int COMENT_WONDOW_HEIGHT = 200;


public:

	//������
	//arg_1 : NPC�N���X�̃��X�g
	//NPC�ɂ���ăA�C�R���̏������y�сAstd::function�ɂ�鏈���̏������A���j���[�֘A��UI������
	void InitEventRelated(std::list<Shared<Npc>>& npc_list);

	//npc��Draw��؂�ւ���
	void EventMenuWindow();

	//���j���[�̐ؑ�
	void SetMenuNpc(const MenuOpen& new_menu) {
		select_menu = new_menu;
	}

	//���݂̃��j���[���擾����
	MenuOpen GetSelectMenu()const {
		return select_menu;
	}

	//Npc�̃R�����g��\������
	//arg_1 : weapon_shop�̃|�C���^
	//���ꂼ���NPC�̃R�����g�`��(weapon_shop�̃|�C���^�̓N���X�̕`�揈�����Ăԏo����)
	void NpcSwitchComent(const Shared<WeaponShop>& weapon_shop = nullptr);

	//Npc�̂��ꂼ��ł̍X�V����
	//arg_1 : �@delta_time 
	//arg_ 2 :�@NPC�̔z�� (�z��͂��ꂼ���NPC�̖��O���Q�Ƃ��āA������ς���ׂɎg�p)
	//arg_3�@:�@BaseScene�̃|�C���^(���ꂼ��̃V�[�P���X�ɐG��ׁA�|�C���^���g�p����)
	//arg_4  :�@InMapScene���̌��݂̃V�[��(���ǂ̃V�[���ɂ��邩�m�F����ׂɎg�p, �Z�[�u����ׂɎg�p���AInMapScene���݂̂Ŏg�p)
	void NpcEventUpdate(float delta_time, const std::list<Shared<Npc>>& npc_list , BaseScene* curent_map_scene, const int& curent_inmap_scene = 0);

	//NPC�̃R�����g�̐؂�ւ�
	void SetNpcComent(const NpcMove& npc) {
		npc_move = npc;
	}

	//���݂�NPC�̏�Ԃ����擾����
	NpcMove GetNpcMove()const {
		return npc_move;
	}
	
	//�R�����g�𗬂��؂��Ă���Enter�L�[�ŃV�[����J�ڂ�����
	bool ComentEventChange(const int count_max);

	//�v���C���[�ɍs����I��������
	//arg_1 : NPC�N���X�̃��X�g(�z��͂��ꂼ���NPC�̖��O���Q�Ƃ��āA������ς���ׂɎg�p)
	//arg_2 : BaseScene�̃|�C���^(���ꂼ��̃V�[�P���X�ɐG��ׁA�|�C���^���g�p����)
	//arg_3 : InMapScene���̌��݂̃V�[��(���ǂ̃V�[���ɂ��邩�m�F����ׂɎg�p, �Z�[�u����ׂɎg�p���AInMapScene���݂̂Ŏg�p)
	void PlayerAnswerSelect(const std::list<Shared<Npc>>& npc_list ,BaseScene* curent_map_scene , const int& inmap_curent_state = 0);

	//�C�x���g�t���O��؂�ւ���
	void EventFlagChange() { event_npc_flag = !event_npc_flag; }

	//�C�x���g�t���O���擾����
	bool GetEventFlag()const { return event_npc_flag; }

	//�G�̃C�x���g�t���O���擾����
	//�t���O���Ǘ����Ē��{�X�Ȃǂ𐶐�����
	//��̖ڂ̃{�X
	bool GetEnemyEventFlag_1()const { return enemy_event_flag_1; }

	//��ɃZ�[�u���s�����߂Ɏg�p
	void SetEnemyEventFlag_1(const bool& save_flag) { enemy_event_flag_1 = save_flag; }

	//�G�̃t���O��؂�ւ���
	//�C�x���g���I��������t���O��؂�ւ��ďI��������
	//��̖ڂ̃{�X
	void EnemyEventFlag_1Change() { enemy_event_flag_1 = !enemy_event_flag_1; }

	//�G�̃C�x���g�t���O���擾����
	//�t���O���Ǘ����Ē��{�X�Ȃǂ𐶐�����
	//��̖ڂ̃{�X
	bool GetEnemyEventFlag_2()const { return enemy_event_flag_2; }

	//��ɃZ�[�u���s�����߂Ɏg�p
	void SetEnemyEventFlag_2(const bool& save_flag) { enemy_event_flag_2 = save_flag; }

	//�G�̃t���O��؂�ւ���
	//�C�x���g���I��������t���O��؂�ւ��ďI��������
	//��̖ڂ̃{�X
	void EnemyEventFlag_2Change() { enemy_event_flag_2 = !enemy_event_flag_2; }

	//�R�����g��\������
	void ComentEvent(BaseScene* curent_scene, const int& coment_max, const std::list<Shared<Npc>>& npc_list);

	//�R�����g�𗬂���������A�C�e�����������Ă��邩�ɂ����true��false��Ԃ�
	//�V�i���I�ŕK�{�A�C�e�������݂���̂ł��̃A�C�e�������݂��邩���m�F���ď�����ς���
	bool CheckEventItem(const int& essential_items);

	//�I����������C�x���g�̏�������
	//NPC�ɂ���ď�����ς���
	void ProcessNpcEventByType(const std::list<Shared<Npc>>& npc_list, BaseScene* curent_map_scene , const int& inmap_curent_state);

private:

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
	//��̖ڂ̃{�X
	bool enemy_event_flag_1 = true;

	//�G�̃C�x���g�Ǘ��t���O
	//2�̖ڂ̃{�X
	bool enemy_event_flag_2 = true;


//--------------------------------------------------------------------------------------------------------
//�|�C���^
private:

	//�v���C���[�̑I���E�B���h�E
	Shared<MenuWindow>player_select_window = nullptr;
	//�E�B���h�E�̃R�����g
	MenuWindow::MenuElement_t* player_select_coment = nullptr;

	//�C�x���g�p�̊֐�
	Shared<Event>function_npc = nullptr;

	//�A�C�e���̃|�C���^
	Shared<Item>item_ = nullptr;

};
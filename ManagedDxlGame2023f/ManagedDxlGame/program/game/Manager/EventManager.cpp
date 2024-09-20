#include "EventManager.h"
#include"../Manager/UiManager.h"
#include"../Manager/SceneManager.h"
#include"../Scene/BaseScene.h"
#include"../Scene/battleScene.h"
#include"../Item/WeaponShop.h"
#include"../System/Event.h"
#include"../Object/NPC.h"
#include"../Manager/GameManager.h"
#include"../Manager/ResourceManager.h"


EventManager* EventManager::GetEventManager()
{
	static EventManager* p_instance = nullptr;

	if (!p_instance)p_instance = new EventManager();
	return p_instance;
}

void EventManager::DelateEventManager()
{
	delete GetEventManager();
}


EventManager::EventManager()
{
	//�E�B���h�E�֘A�̏�����
	InitMenuEventConnection();
}

void EventManager::InitEventRelated(std::list<Shared<Npc>>& npc_list)
{
	npc_move = NpcMove::EMPTY;

	//null�������Ă����ꍇ��������
	if (!function_npc) {
		function_npc = std::make_shared<Event>();
	}

	//�󂶂ᖳ�����
	if (!npc_list.empty()) {

		//����������������
		function_npc->InitEventFunction(npc_list);

		//�C�x���g���N������悤�ɂ���
		event_npc_flag = true;

		//�A�C�R���̉摜�n���h��������������
		//���̃}�b�v�ł��������O�����݂���ׁA�}�b�v�ɂ���ăA�C�R����؂�ւ���
		for (auto& npc : npc_list) {

			if (npc->GetNpcName() == "���폤�l") {

				//���j���[�Ŏg���A�C�R��
				Armsdealer_icon_hdl = npc->GetNpcIconImage();
			}
			else if (npc->GetNpcName() == "����") {

				//���j���[�Ŏg���A�C�R��
				herdman_icon_hdl = npc->GetNpcIconImage();
			}
			else if (npc->GetNpcName() == "�_��") {

				//���j���[�Ŏg���A�C�R��
				Priest_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "����") {

				town_herdman_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "�h��") {

				inn_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "����") {

				boss_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "����") {

				enemy_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "��") {

				king_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "����") {

				queen_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "�������l") {

				female_resident_icon_hdl = npc->GetNpcIconImage();


			}
			else if (npc->GetNpcName() == "�j�����l") {

				resident_male_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "�j������") {

				resident_male_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "�j���Z��") {

				resident_male_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "�w��") {

				scholar_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "���m") {

				resident_male_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "�����̔Ԑl") {

				ordeal_icon_hdl = npc->GetNpcIconImage();
			}
			else if (npc->GetNpcName() == "�e�̔Ԑl") {
				shadow_icon_hdl = npc->GetNpcIconImage();
			}
		}
	}
}

void EventManager::InitMenuEventConnection()
{
	//�ŏ��̈�񂾂����[�h����
	if (!UIManager::GetUIManager()->getMenu("select_answer_menu")) {

		//�v���C���[�̑I���E�B���h�E���̕����̍��W
		const tnl::Vector2i PLAYER_SELECT_WINDOW_ANSWER_OK_POS = { 100 , 100 };
		const tnl::Vector2i PLAYER_SELECT_WINDOW_ANSWER_NO_POS = { 100 , 150 };

		//�v���C���[�ɑI��������E�B���h�E
		player_select_coment = new MenuWindow::MenuElement_t[]{
			{PLAYER_SELECT_WINDOW_ANSWER_OK_POS.x , PLAYER_SELECT_WINDOW_ANSWER_OK_POS.y , "�͂�" , MenuWindow::Elements::FIRST_ELEMENT},
			{PLAYER_SELECT_WINDOW_ANSWER_NO_POS.x , PLAYER_SELECT_WINDOW_ANSWER_NO_POS.y , "������", MenuWindow::Elements::SECOND_ELEMENT}
		};

		//���j���[�E�B���h�E�̍ő�v�f��
		const int MENU_ELEMENTS_MAX = 2;

		//�v���C���[�ɑ΂���s���I���E�B���h�E
		player_select_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", player_select_coment, MENU_ELEMENTS_MAX);
		player_select_window->Open();

		//map�Ɋi�[����
		UIManager::GetUIManager()->addMenu("select_answer_menu", player_select_window);
	}
}

//Npc�̃R�����g�̐؂�ւ�
void EventManager::NpcSwitchComent(const Shared<WeaponShop>& weapon_shop)
{
	//���퉮�������W
	const tnl::Vector2i WEAPON_SHOP_STRING_POS = { 60 , 520 };

	//�Z�[�u�e�L�X�g�������W
	const tnl::Vector2i SAVE_TEXT_STRING_POS = { 150, 630 };

	//�v���C���[�ɑI��������E�B���h�E�̍��W
	const tnl::Vector2i PLAYER_ANSWER_SELECT_WINDOW_POS = { 50, 50 };

	//�v���C���[�ɑI��������E�B���h�E�̃T�C�Y
	const int  PLAYER_ANSWER_SELECT_WINDOW_WIDTH  =  250;
	const int  PLAYER_ANSWER_SELECT_WINDOW_HEIGHT =  200;

	//���݂̏�������\�����镶��
	const tnl::Vector2i POSSESSION_STRING_POS = { 135, 100 };

	//�C�x���g���������Ȃ��������̕����̍��W
	const tnl::Vector2i EVENT_NOT_STRING_POS = { 150, 630 };

	switch (npc_move)
	{
	case EventManager::NpcMove::EMPTY:

		break;

	//����
	case EventManager::NpcMove::HERDMAN:

		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, herdman_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);
	
		break;

	//�_��
	case EventManager::NpcMove::PRIEST:

		//���j���[�̃E�B���h�E��\������
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, Priest_icon_hdl, true);
		
		//�Z�[�u�e�L�X�g
		UIManager::GetUIManager()->SaveText(SAVE_TEXT_STRING_POS);

		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y , PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);
		
	
		break;

	//���퉮
	case EventManager::NpcMove::WEAPONSHOP:

		//�E�B���h�E�̕`��
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//���퉮�֘A�̕\��
		DrawString(WEAPON_SHOP_STRING_POS.x, WEAPON_SHOP_STRING_POS.y, "���퉮", -1);

		//�R�����g�ƃA�C�R��
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, Armsdealer_icon_hdl , true);

		//���퉮�̃R�����g
		weapon_shop->Draw();

		//�w����I�����ꂽ��
		if (weapon_shop->GetWeapoShopTransaction() == WeaponShop::TransactionType::BUY) {

			//�v���C���[�̏�������`��
			DrawStringEx(POSSESSION_STRING_POS.x, POSSESSION_STRING_POS.y, koni::Color::WHITE, "������: %d", GameManager::GetGameManager()->GetPlayer()->GetPlayerMoney());
		}

		break;

	//�e�̔Ԑl
	case EventManager::NpcMove::SHADOWENEMY:

		//���j���[�̃E�B���h�E��\������
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�R�����g�ƃA�C�R��
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, shadow_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//�I���E�B���h�E��\������
		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;


	//�{�X�����X�^�[
	case EventManager::NpcMove::BOSS:

		//���j���[�̃E�B���h�E��\������
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�R�����g�ƃA�C�R��
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, boss_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//�I���E�B���h�E��\������
		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;


	//�h��
	case  EventManager::NpcMove::INN:

		//���j���[�̃E�B���h�E��\������
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�R�����g�ƃA�C�R��
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, inn_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;

	//����
	case EventManager::NpcMove::TOWNHERDMAN:

		UIManager::GetUIManager()->Menu_Draw("menu_window",COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, town_herdman_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//�G
	case EventManager::NpcMove::ENEMY:

		//�E�B���h�E��\������
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, enemy_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//�I���E�B���h�E��\������
		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;

	//��
	case  EventManager::NpcMove::KING:

		//�E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, king_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//����
	case  EventManager::NpcMove::QUEEN:

		//�E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, queen_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//�������l
	case  EventManager::NpcMove::FEMALE_RESIDENT:

		//�E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, female_resident_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//�j���Z�l
	case  EventManager::NpcMove::RESIDENT_MALE:

		//�E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, resident_male_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//�w��
	case  EventManager::NpcMove::SCHOLAR:
		
		//�E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, scholar_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//�����̔Ԑl
	case EventManager::NpcMove::ORDEAL:

		//�E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, ordeal_icon_hdl, true);

		//�R�����g��\������
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//�I���E�B���h�E��\������
		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		//�K�v�A�C�e���������Ă��Ȃ��ꍇ�A������\������
		if (ordeal_coment_flag) {
			DrawStringEx(EVENT_NOT_STRING_POS.x, EVENT_NOT_STRING_POS.y, koni::Color::WHITE, "���O�ɂ͎��i�������悤��");
		}

		break;

	default:

		break;
	}
}

//Npc�Ȃǂ̂��ꂼ��̃C�x���g���̍X�V����
//�킩��₷���悤�ɕ�����
void EventManager::NpcEventUpdate(float delta_time, const std::list<Shared<Npc>>& npc_list,BaseScene* curent_map_scene, const int& curent_inmap_scene )
{
	//---�\������R�����g�̐�---//

	//����
	const int HERDMAN_COMENT_MAX = 7;
	//�_��
	const int PRIEST_COMENT_MAX = 1;
	//�h��
	const int INN_COMENT_MAX = 1;
	//����
	const int TOWN_HERDMAN_COMENT_MAX = 2;
	//����
	const int ENEMY_COMENT_MAX = 3;
	//��
	const int KING_COMENT_MAX = 4;
	//����
	const int QUEEN_COMENT_MAX = 1;
	//�����Z�l
	const int FEMALE_RESIDENT_COMENT_MAX = 1;
	//�j���Z�l
	const int MALE_VILLAGERS_COMENT_MAX = 1;
	//�w��
	const int SCHOLAR_COMENT_MAX = 5;
	//�����̔Ԑl
	const int ORDEAL_COMENT_MAX = 3;
	//���X�{�X
	const int BOSS_COMENT_MAX_NUM = 4;
	//�e�̔Ԑl
	const int SHADOW_ENEMY_COMENT_MAX = 2;
		
	//�V�[���}�l�[�W���[�̃|�C���^
	auto mgr = SceneManager::GetSceneManager();

	switch (npc_move)
	{
	case EventManager::NpcMove::EMPTY:


		break;

	//����
	case EventManager::NpcMove::HERDMAN:

		//�R�����g��\������
		ComentEvent(curent_map_scene, HERDMAN_COMENT_MAX, npc_list);

		break;


	//�_��
	case EventManager::NpcMove::PRIEST:

		//�R�����g��\������
		if (function_npc->ComentEventChange(PRIEST_COMENT_MAX) && event_npc_flag) {

			//�I������
			//�Z�[�u�@�\
			PlayerAnswerSelect(npc_list , curent_map_scene ,curent_inmap_scene);

			//�t���O��false�̏ꍇ
			if (!player_select_window->getAcrionFlag() && event_npc_flag) {
				//�E�B���h�E�̏������󂯕t����悤�ɂ���
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//�e�̔Ԑl
	case EventManager::NpcMove::SHADOWENEMY:

		//�R�����g��\������
		if (function_npc->ComentEventChange(SHADOW_ENEMY_COMENT_MAX) && event_npc_flag) {

			//�I������
			PlayerAnswerSelect(npc_list, curent_map_scene);

			//�t���O��false�̏ꍇ
			if (!player_select_window->getAcrionFlag() && event_npc_flag) {
				//�E�B���h�E�̏������󂯕t����悤�ɂ���
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//�{�X
	case EventManager::NpcMove::BOSS:

		//�t�H���g�̑傫����ς���
		SetFontSize(20);

		//�R�����g��\������
		if (function_npc->ComentEventChange(BOSS_COMENT_MAX_NUM)) {

			//�I������ & �G�Ƃ̉�b����
			PlayerAnswerSelect(npc_list , curent_map_scene);

			//�t���O��false�̏ꍇ
			if (!player_select_window->getAcrionFlag() && event_npc_flag) {
				//�E�B���h�E�̏������󂯕t����悤�ɂ���
				player_select_window->IsActiveChangeFlag();
			}
		}

		//�t�H���g�̑傫����߂�
		SetFontSize(16);

		break;

	//�h��
	case EventManager::NpcMove::INN:

		//�R�����g��\������
		if (function_npc->ComentEventChange(INN_COMENT_MAX)) {

			//�I������ & �h������
			//�v���C���[��Hp��Mp�̉񕜏���
			PlayerAnswerSelect(npc_list , curent_map_scene);

			//�t���O��false�̏ꍇ
			if (!player_select_window->getAcrionFlag()&& event_npc_flag) {
				//�E�B���h�E�̏������󂯕t����悤�ɂ���
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//����
	case EventManager::NpcMove::TOWNHERDMAN:

		//�R�����g��\������
		ComentEvent(curent_map_scene, TOWN_HERDMAN_COMENT_MAX, npc_list);

		break;

	//�G
	case EventManager::NpcMove::ENEMY:

		//�R�����g��\������
		if (function_npc->ComentEventChange(ENEMY_COMENT_MAX)){

			//�I������ & �G�Ƃ̉�b����
			PlayerAnswerSelect(npc_list , curent_map_scene);

			//�t���O��false�̏ꍇ
			if (!player_select_window->getAcrionFlag() && event_npc_flag) {
				//�E�B���h�E�̏������󂯕t����悤�ɂ���
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//��
	case EventManager::NpcMove::KING:

		//�R�����g��\������
		ComentEvent(curent_map_scene, KING_COMENT_MAX, npc_list);

		break;

	//����
	case EventManager::NpcMove::QUEEN:

		//�R�����g��\������
		ComentEvent(curent_map_scene, QUEEN_COMENT_MAX, npc_list);

		break;

	//�����Z�l
	case EventManager::NpcMove::FEMALE_RESIDENT:

		//�R�����g��\������
		ComentEvent(curent_map_scene, FEMALE_RESIDENT_COMENT_MAX, npc_list);

		break;

	//�j���Z�l
	case EventManager::NpcMove::RESIDENT_MALE:

		//�R�����g��\������
		ComentEvent(curent_map_scene, MALE_VILLAGERS_COMENT_MAX, npc_list);

		break;
	
	//�w��
	case EventManager::NpcMove::SCHOLAR:

		//�R�����g��\������
		ComentEvent(curent_map_scene, SCHOLAR_COMENT_MAX, npc_list);

		break;

	case EventManager::NpcMove::ORDEAL:

		//�R�����g��\������
		if (function_npc->ComentEventChange(ORDEAL_COMENT_MAX)) {

			//�I������ & �G�Ƃ̉�b����
			PlayerAnswerSelect(npc_list, curent_map_scene);

			//�t���O��false�̏ꍇ
			if (!player_select_window->getAcrionFlag() && event_npc_flag) {
				//�E�B���h�E�̏������󂯕t����悤�ɂ���
				player_select_window->IsActiveChangeFlag();
			}
			
		}

		break;

	default:
		break;
	}
}

//�v���C���[�ɍs����I��������
void EventManager::PlayerAnswerSelect(const std::list<Shared<Npc>>& npc_list , BaseScene* curent_map_scene , const int& inmap_curent_state)
{
	//�v���C���[���͂���I��������
	if (player_select_window->getSelectNum() == MenuWindow::Elements::FIRST_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		&& player_select_window->getAcrionFlag())
	{
		//NPC�̂ɂ���Ă��ꂼ�ꏈ����ς���
		ProcessNpcEventByType(npc_list , curent_map_scene , inmap_curent_state);
	}
	//�v���C���[����������I��������
	else if (player_select_window->getSelectNum() == MenuWindow::Elements::SECOND_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		&& player_select_window->getAcrionFlag())
	{
		player_select_window->IsActiveChangeFlag();
		event_npc_flag = false;
		npc_move = NpcMove::EMPTY;
		curent_map_scene->SequenceChangeMove();
	}
}

//NPC�ɂ���ď�����ς���
void EventManager::ProcessNpcEventByType(const std::list<Shared<Npc>>& npc_list, BaseScene* curent_map_scene , const int& inmap_curent_state)
{
	for (auto& npc_list_ : npc_list) {

		//�_���ɘb�������ꍇ
		if (npc_list_->GetNpcName() == ("�_��") && npc_move == NpcMove::PRIEST) {

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), inmap_curent_state);

			//�������󂯕t���Ȃ��悤�ɂ���
			player_select_window->IsActiveChangeFlag();

			event_npc_flag = false;

			break;

		}
		//�h���ɘb�������ꍇ
		else if (npc_list_->GetNpcName() == "�h��" && npc_move == NpcMove::INN) {

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), INN_MONEY);

			//�������󂯕t���Ȃ��悤�ɂ���
			player_select_window->IsActiveChangeFlag();

			event_npc_flag = false;

			//�v���C���[�𓮂���悤�ɂ���
			curent_map_scene->SequenceChangeMove();

			break;
		}
		//�G�ɘb�������ꍇ
		else if (npc_list_->GetNpcName() == "����" && npc_move == NpcMove::ENEMY) {

			//���{�X��ID
			boss_id = 21;

			//�{�X�V�[���ł̔w�i�����[�h����
			boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/town.png");

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/town_bgm.mp3", "sound/BGM/tyubosu.mp3", static_cast<int>(InMapScene::InMapState::TOWN));

			//�������󂯕t���Ȃ��悤�ɂ���
			if (player_select_window->getAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

			event_npc_flag = false;

			break;
		}
		//�e�̔Ԑl
		else if (npc_list_->GetNpcName() == "�e�̔Ԑl") {

			boss_id = 28;

			//�{�X�V�[���ł̔w�i�����[�h����
			boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/boss_castle.png");

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/maou_bgm_castle.mp3", "sound/BGM/battle-dark.mp3", static_cast<int>(InMapScene::InMapState::BOSSCASTLE));

			//�������󂯕t���Ȃ��悤�ɂ���
			if (player_select_window->getAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

			event_npc_flag = false;

			break;
		}
		else if (npc_list_->GetNpcName() == "����") {

			//�{�X��ID
			boss_id = 20;

			//�{�X�V�[���ł̔w�i�����[�h����
			boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/boss_castle.png");

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/maou_bgm_castle.mp3", "sound/BGM/maou_sentou_bgm.mp3" , static_cast<int>(InMapScene::InMapState::BOSSROOM));

			//�������󂯕t���Ȃ��悤�ɂ���
			if (player_select_window->getAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

			event_npc_flag = false;

			break;

		}
		else if (npc_list_->GetNpcName() == "�����̔Ԑl") {

			if (CheckEventItem(ESSENTIAL_ITEM_ID , FALSE)) {

				//�{�X��ID
				boss_id = 27;

				//�{�X�V�[���ł̔w�i�����[�h����
				boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/setugen_2.png");

				function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/grave.mp3", "sound/BGM/ordeal_battle.mp3",static_cast<int>(InMapScene::InMapState::GRAVE));

				//�������󂯕t���Ȃ��悤�ɂ���
				if (player_select_window->getAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

				event_npc_flag = false;

				break;
			}
			else {

				//���s�s�̕�����\������
				ordeal_coment_flag = true;

				break;
			}
		}
	}

}

//�R�����g��\������
void EventManager::ComentEvent(BaseScene* curent_scene, const int& coment_max, const std::list<Shared<Npc>>& npc_list)
{
	std::vector<std::string> npc_names = { "����", "����", "��", "����", "�������l","�j�����l","�j������","�j���Z��","�w��" };

	//�󂶂ᖳ�����
	if (!npc_list.empty()) {

		for (auto& npc_list_ : npc_list) {


			//�����J�E���g
			int coment_execution_count = 0;

			for (const auto& name : npc_names) {
				if (npc_list_->GetNpcName() == name) {
					// �R�����g��\������
					function_npc->GetComentEvent(npc_list_->GetNpcName())(curent_scene, coment_max);
					coment_execution_count++;
					break;
				}
			}

			if (coment_execution_count > 0) {

				//NPC�͈��̂݃R�����g���o���̂ň�񏈗����s�����炱���Ŕ�����
				break;
			}
		}
	}
}

bool EventManager::CheckEventItem(const int& essential_items , const int& remove_need_flag)
{
	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto item_find = std::find_if(GameManager::GetGameManager()->GetInventory()->GetInventoryList().begin(), GameManager::GetGameManager()->GetInventory()->GetInventoryList().end(),
		[&](const ItemBase& item) { return item.GetItemId() == essential_items; });

	// �A�C�e�������������ꍇ�̓C�x���g�𔭐�������
	if (item_find != GameManager::GetGameManager()->GetInventory()->GetInventoryList().end()) {

		if (remove_need_flag) {
			GameManager::GetGameManager()->GetInventory()->GetInventoryList().erase(item_find);
			//�A�C�e���̐������炷
			GameManager::GetGameManager()->GetInventory()->DelateItemNum();
		}

		return true;
	}
	//������Ȃ�������false��Ԃ�
	else {
		return false;
	}
	
}


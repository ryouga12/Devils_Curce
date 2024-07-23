#include "EventManager.h"
#include"../Manager/UiManager.h"
#include"../Manager/SceneManager.h"
#include"../Scene/InMapScene.h"
#include"../Scene/battleScene.h"
#include"../Item/WeaponShop.h"
#include"../System/EventNpc.h"


EventManager* EventManager::getEventManager()
{
	static EventManager* p_instance = nullptr;

	if (!p_instance)p_instance = new EventManager();
	return p_instance;
}


EventManager::EventManager()
{
	
}

void EventManager::InitEventRelated(std::list<Shared<Actor>>& npc_list)
{
	//null�������Ă����ꍇ
	if (!function_npc) {
		function_npc = std::make_shared<EventNpc>();
	}

	//�A�C�R���̉摜�n���h��������������
	//���̃}�b�v�ł��������O�����݂���ׁA�}�b�v�ɂ���ăA�C�R����؂�ւ���
	for (auto& npc : npc_list) {

		auto itr = npc_list.begin();
		
		if (itr != npc_list.end()) {

			auto npc_ = std::dynamic_pointer_cast<Npc>(npc);

			if (npc_->GetNpcName() == "���폤�l") {
				//���j���[�Ŏg���A�C�R��
				Armsdealer_icon_hdl = npc_->GetNpcIconImage();
			}
			else if (npc_->GetNpcName() == "����") {
				//���j���[�Ŏg���A�C�R��
				herdman_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "�_��") {
				//���j���[�Ŏg���A�C�R��
				Priest_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "����") {
				town_herdman_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "�h��") {
				inn_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "����") {
				boss_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "����") {
				enemy_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "��") {
				king_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "����") {
				queen_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "�������l") {
				female_resident_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "�j�����l") {
				male_villagers_icon_hdl = npc_->GetNpcIconImage();
				//����������������
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
		}
	}

	//���j���[�̑I���E�B���h�E
	menu_window = UIManager::getUIManager()->getMenu("menu_window");

	//�ŏ��̈�񂾂����[�h����
	if (!UIManager::getUIManager()->getMenu("select_answer_menu")) {

		//�v���C���[�ɑI��������E�B���h�E
		player_select_coment = new MenuWindow::MenuElement_t[]{
			{100 , 100 , "�͂�" , 0},
			{100 , 150 , "������", 1}
		};

		//�v���C���[�ɑ΂���s���I���E�B���h�E
		player_select_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", player_select_coment, 2, 1);
		player_select_window->Open();

		//map�Ɋi�[����
		UIManager::getUIManager()->addMenu("select_answer_menu", player_select_window);
	}
}


EventManager::~EventManager()
{

}

//Npc�̃R�����g�̐؂�ւ�
void EventManager::NpcSwitchComent(const Shared<WeaponShop>& weapon_shop)
{
	//�R�����g�p
	auto menuComentWindiow = menu_window.lock();


	//�v���C���[�ւ̑I�������鎞�̃E�B���h�E
	auto menuSelectWindow = menu_window.lock();

	//���퉮�������W
	const tnl::Vector2i WEAPON_SHOP_STRING_POS = { 60 , 520 };

	switch (npcMove)
	{
	case EventManager::NpcMove::EMPTY:

		break;

	//����
	case EventManager::NpcMove::HERDMAN:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, herdman_icon_hdl, true);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);
	
		break;

	//�_��
	case EventManager::NpcMove::PRIEST:

		//���j���[�̃E�B���h�E��\������
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�A�C�R����\������
		DrawRotaGraph(110, 600, 1.5f, 0, Priest_icon_hdl, true);
		
		//�Z�[�u�e�L�X�g
		UIManager::getUIManager()->SaveText(tnl::Vector3{ 150,630,0 });

		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(50, 50, 250, 200);
		}

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);
		
	
		break;

	//���퉮
	case EventManager::NpcMove::WEAPONSHOP:

		if (auto menu_coment_window = menu_window.lock()) {
			menu_coment_window->Menu_draw(50, 500, 700, 200);
		}

		//���퉮�֘A�̕\��
		DrawString(WEAPON_SHOP_STRING_POS.x, WEAPON_SHOP_STRING_POS.y, "���퉮", -1);

		//�R�����g�ƃA�C�R��
		DrawRotaGraph(90, 600, 1.5f, 0, Armsdealer_icon_hdl , true);

		//���퉮�̃R�����g��\������
		UIManager::getUIManager()->armsdealerComentDraw(1);

		//���퉮�̃R�����g
		weapon_shop->Draw();

		//�v���C���[�̏�������`��
		DrawStringEx(135, 100, -1, "������: %d", GameManager::getGameManager()->getPlayer()->getPlayerMoney());

		break;

	//�{�X�����X�^�[
	case EventManager::NpcMove::BOSS:

		//���j���[�̃E�B���h�E��\������
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		//�I���E�B���h�E��\������
		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(50, 50, 250, 200);
		}

		break;


	//�h��
	case  EventManager::NpcMove::INN:

		//���j���[�̃E�B���h�E��\������
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�R�����g�ƃA�C�R��
		DrawRotaGraph(90, 600, 1.5f, 0, inn_icon_hdl, true);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(50, 50, 250, 200);
		}

		break;

	//����
	case EventManager::NpcMove::TOWNHERDMAN:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, town_herdman_icon_hdl, true);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	//�G
	case EventManager::NpcMove::ENEMY:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, enemy_icon_hdl, true);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		//�I���E�B���h�E��\������
		if (player_select_window->getAcrionFlag()) {
			//�I���E�B���h�E��\������
			player_select_window->All(50, 50, 250, 200);
		}

		break;

	//��
	case  EventManager::NpcMove::KING:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, king_icon_hdl, true);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	//����
	case  EventManager::NpcMove::QUEEN:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, queen_icon_hdl, true);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	//�������l
	case  EventManager::NpcMove::FEMALE_RESIDENT:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, female_resident_icon_hdl, true);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	//�j�����l
	case  EventManager::NpcMove::RESIDENT_MALE:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�A�C�R����\������
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, male_villagers_icon_hdl, true);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	default:

		break;
	}
}

//Npc�Ȃǂ̂��ꂼ��̃C�x���g���̍X�V����
void EventManager::NpcEventUpdate(float delta_time, InMapScene* inmap_scene, int curent_map_state, std::list<Shared<Actor>>& npc_list)
{
	//�\������R�����g�̐�
	const int HERDMAN_COMENT_MAX = 7;
	const int PRIEST_COMENT_MAX = 1;
	const int INN_COMENT_MAX = 1;
	const int TOWN_HERDMAN_COMENT_MAX = 2;
	const int ENEMY_COMENT_MAX = 3;
	const int KING_COMENT_MAX = 4;
	const int QUEEN_COMENT_MAX = 1;
	const int FEMALE_RESIDENT_COMENT_MAX = 1;
	const int MALE_VILLAGERS_COMENT_MAX = 1;


	//�V�[���}�l�[�W���[�̃|�C���^
	auto mgr = SceneManager::GetInstance();

	switch (npcMove)
	{
	case EventManager::NpcMove::EMPTY:


		break;

	//����
	case EventManager::NpcMove::HERDMAN:

		//�R�����g��\������
		ComentEvent(inmap_scene, HERDMAN_COMENT_MAX, npc_list);

		break;


	//�_��
	case EventManager::NpcMove::PRIEST:

		//�R�����g��\������
		if (ComentEventChange(PRIEST_COMENT_MAX) && event_npc_flag) {

			//�I������
			//�Z�[�u�@�\
			PlayerAnswerSelect(curent_map_state, inmap_scene, npc_list);

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
		if (ComentEventChange(BOSS_COMENT_MAX_NUM)) {

			//�I������ & �G�Ƃ̉�b����
			PlayerAnswerSelect(curent_map_state, inmap_scene, npc_list);

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
		if (ComentEventChange(INN_COMENT_MAX)) {

			//�I������ & �h������
			//�v���C���[��Hp��Mp�̉񕜏���
			PlayerAnswerSelect(curent_map_state, inmap_scene, npc_list);

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
		ComentEvent(inmap_scene, TOWN_HERDMAN_COMENT_MAX, npc_list);

		break;

	//�G
	case EventManager::NpcMove::ENEMY:

		//�R�����g��\������
		if (ComentEventChange(ENEMY_COMENT_MAX)){

			//�I������ & �G�Ƃ̉�b����
			PlayerAnswerSelect(curent_map_state, inmap_scene, npc_list);

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
		ComentEvent(inmap_scene, KING_COMENT_MAX, npc_list);

		break;

	//����
	case EventManager::NpcMove::QUEEN:

		//�R�����g��\������
		ComentEvent(inmap_scene, QUEEN_COMENT_MAX, npc_list);

		break;

	//�������l
	case EventManager::NpcMove::FEMALE_RESIDENT:

		//�R�����g��\������
		ComentEvent(inmap_scene, FEMALE_RESIDENT_COMENT_MAX, npc_list);

		break;

	//�������l
	case EventManager::NpcMove::RESIDENT_MALE:

		//�R�����g��\������
		ComentEvent(inmap_scene, MALE_VILLAGERS_COMENT_MAX, npc_list);

		break;


	default:
		break;
	}
}

//�R�����g�𗬂��؂��Ă���Enter�L�[�ŃV�[����J�ڂ�����
bool EventManager::ComentEventChange(const int count_max)
{
	//�R�����g���R�}����ŕ\������
	UIManager::getUIManager()->ComentNextByInput(count_max);

	//�w��̃R�����g�ȏ�ɂȂ�����true��Ԃ�
	if (UIManager::getUIManager()->GetCount() >= count_max) {
		return true;
	}

	return false;
}

//�v���C���[�ɍs����I��������
void EventManager::PlayerAnswerSelect(int curent_map_state , InMapScene* inmap_scene , const std::list<Shared<Actor>>& npc_list)
{

	for (auto& npc_list_ : npc_list) {

		auto itr = npc_list.begin();

		if (itr != npc_list.end()) {

			auto npc_ = std::dynamic_pointer_cast<Npc>(npc_list_);

			//�v���C���[���͂���I��������
			if (player_select_window->getSelectNum() == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
				&& player_select_window->getAcrionFlag())
			{
				//�_���ɘb�������ꍇ
				if (npc_->GetNpcName() == ("�_��")) {

					function_npc->EventSelectProcces(npc_->GetNpcName(), curent_map_state);

					//�������󂯕t���Ȃ��悤�ɂ���
					player_select_window->IsActiveChangeFlag();

					event_npc_flag = false;

				}
				//�h���ɘb�������ꍇ
				else if (npc_->GetNpcName() == "�h��" && npcMove == NpcMove::INN) {

					function_npc->EventSelectProcces(npc_->GetNpcName(), INNMONEY);

					//�������󂯕t���Ȃ��悤�ɂ���
					player_select_window->IsActiveChangeFlag();

					event_npc_flag = false;

					//�v���C���[�𓮂���悤�ɂ���
					inmap_scene->SequenceChange(InMapScene::Sequence_num::MOVE);
				}
				//�G�ɘb�������ꍇ
				else if (npc_->GetNpcName() == "����" && npcMove == NpcMove::ENEMY) {

					//�{�X�V�[���ł̔w�i�����[�h����
					boss_background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg013b.jpg");

					function_npc->EventSelectProcces(npc_->GetNpcName(), boss_background_hdl , MEDIUM_BOSS, "sound/BGM/town_bgm.mp3", "sound/BGM/maou_sentou_bgm.mp3");

					//�������󂯕t���Ȃ��悤�ɂ���
					player_select_window->IsActiveChangeFlag();

					event_npc_flag = false;
				}
				else if (npc_->GetNpcName() == "����") {

					//�{�X�V�[���ł̔w�i�����[�h����
					boss_background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg010b.jpg");

					function_npc->EventSelectProcces(npc_->GetNpcName(), boss_background_hdl, BOSS_ID, "sound/BGM/maou_bgm_castle.mp3", "sound/BGM/maou_sentou_bgm.mp3");

					//�������󂯕t���Ȃ��悤�ɂ���
					player_select_window->IsActiveChangeFlag();

					event_npc_flag = false;

				}

			}
			//�v���C���[����������I��������
			else if (player_select_window->getSelectNum() == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
				&& player_select_window->getAcrionFlag())
			{
				inmap_scene->SequenceChange(InMapScene::Sequence_num::MOVE);
				player_select_window->IsActiveChangeFlag();
				event_npc_flag = false;
			}
		}
		itr++;
	}
}

//�R�����g��\������
void EventManager::ComentEvent(InMapScene* inmap_scene, const int coment_max ,const std::list<Shared<Actor>>& npc_list )
{
	std::vector<std::string> npc_names = { "����", "����", "��", "����", "�������l","�j�����l"};

	for (auto& npc_list_ : npc_list) {

		auto npc_ = std::dynamic_pointer_cast<Npc>(npc_list_);

		if (npc_) {

			int coment_execution_count = 0;

			for (const auto& name : npc_names) {
				if (npc_->GetNpcName() == name) {
					// �R�����g��\������
					function_npc->GetComentEvent(npc_->GetNpcName())(inmap_scene, coment_max);
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

//Npc�̃��j���[�I�����
void EventManager::EventMenuWindow()
{
	//�R�����g�p
	auto menuComentWindiow = menu_window.lock();

	//�v���C���[�ւ̑I�������鎞�̃E�B���h�E
	auto menuSelectWindow = menu_window.lock();

	switch (select_menu)
	{

		//���퉮�̍ۂ̕`��
	case EventManager::MenuOpen::WEAPONSHOP:

		//�E�B���h�E�̕\��
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//���j���[�̃o�b�N�O���E���h��`�悷��
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//���폤�l�̃A�C�R����\������
		DrawRotaGraph(90, 600, 1.5f, 0, Armsdealer_icon_hdl, true);

		//���폤�l�̃R�����g��\������
		UIManager::getUIManager()->armsdealerComentDraw(ARMSDEALER_SPEAK_COMENT);

		break;


	case EventManager::MenuOpen::BOSS:

		//�E�B���h�E�̕\��
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//�b���R�}���h
		GameManager::getGameManager()->displayDialogue();


		break;

	default:

		//�E�B���h�E�̕\��
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//�b���R�}���h
		GameManager::getGameManager()->displayDialogue();

		break;
	}


}

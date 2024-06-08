#include "EventManager.h"
#include"../Manager/UiManager.h"


EventManager* EventManager::getEventManager()
{
	static EventManager* p_instance = nullptr;

	if (!p_instance)p_instance = new EventManager();
	return p_instance;
}

EventManager::EventManager()
{
	InitMenu();

}

void EventManager::InitMenu()
{
	//���j���[�Ŏg���A�C�R��
	Armsdealer_icon_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/Npc/Armsdealer_icon.png");
	herdman_icon_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/Npc/herdman_icon.png");
	Priest_icon_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/Npc/Priest_icon.png");
	Player_icon_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/Player/Player_Icon.png");


	//���j���[�̑I���E�B���h�E
	menu_window = UIManager::getUIManager()->getMenu("menu_window");

}

EventManager::~EventManager()
{
}

//Npc�̃R�����g�̐؂�ւ�
void EventManager::NpcSwitchComent()
{
	//�R�����g�p
	auto menuComentWindiow = menu_window.lock();

	//�v���C���[�ւ̑I�������鎞�̃E�B���h�E
	auto menuSelectWindow = menu_window.lock();

	switch (npcMove)
	{
	case EventManager::NpcMove::EMPTY:

		break;

	//����
	case EventManager::NpcMove::HERDMAN:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�A�C�R����\������
		DrawRotaGraph(HERDMAN_ICON_POS.x, HERDMAN_ICON_POS.y, HERDMAN_SCALE, 0, herdman_icon_hdl, true);

		//�R�����g��\������
		UIManager::getUIManager()->ComentDraw(HERDMAN_COMENT_POS, 7);

	
		break;

	//�_��
	case EventManager::NpcMove::PRIEST:

		//���j���[�̃E�B���h�E��\������
		menuComentWindiow->Menu_draw(50, 500, 700, 200);
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//�A�C�R����\������
		DrawRotaGraph(110, 600, 1.5f, 0, Priest_icon_hdl, true);

		//�R�����g��\������
		/*NpcComent(priest_coment_pos.x, priest_coment_pos.y, priest_coment, 2);*/

		break;

	//�{�X�����X�^�[
	case EventManager::NpcMove::BOSS:

		//���j���[�̃E�B���h�E��\������
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//�t�H���g�̑傫����ς���
		SetFontSize(20);

		//�R�����g��\������
		if (ComentBattleChange(BOSS_COMENT_POS, BOSS_COMENT_MAX_NUM) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//BGM���~�߂�
			SoundManager::getSoundManager()->StopSound("sound/BGM/maou_bgm_castle.mp3");
			//�o�g���p��BGM�𗬂�
			SoundManager::getSoundManager()->sound_Play("sound/BGM/maou_sentou_bgm.mp3", DX_PLAYTYPE_LOOP);

			auto mgr = SceneManager::GetInstance();
			//�V�[����J�ڂ�����(�v���C���[�̍��W��n��,�G��ID,�G�̃|�C���^��n��)
			mgr->changeScene(new BattleScene(GameManager::getGameManager()->getPlayer()->getPlayerPos(), boss_background_hdl, std::make_shared<BossMonster>()));
		}

		//�t�H���g�̑傫����߂�
		SetFontSize(16);

		break;

	default:

		break;
	}
}

//�R�����g�𗬂��؂��Ă���Enter�L�[�ŃV�[����J�ڂ�����
bool EventManager::ComentBattleChange(const tnl::Vector2i& coment_pos , const int count_max)
{
	//�R�����g��\������
	UIManager::getUIManager()->ComentDraw(coment_pos, count_max);

	if (UIManager::getUIManager()->GetCount() >= count_max) {
		return true;
	}

	return false;
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

		//�{�X�V�[���ł̔w�i�����[�h����
		boss_background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg010b.jpg");

		break;

	default:

		//�E�B���h�E�̕\��
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//�b���R�}���h
		GameManager::getGameManager()->displayDialogue();

		break;
	}


}

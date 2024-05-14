#include "EventManager.h"

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
	menuSelectWindow = std::make_shared<Menu>(50, 50, 250, 200, "graphics/WindowBase_01.png");

	//�R�����g�\���̍ۂ̃��j���[�E�B���h�E
	menuComentWindiow = std::make_shared<Menu>(50, 500, 700, 200, "graphics/WindowBase_01.png");
}

EventManager::~EventManager()
{
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Npc/Armsdealer_icon.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Npc/herdman_icon.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Npc/Priest_icon.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Player/Player_Icon.png");
}

bool EventManager::isNpcMenuOpen()
{
	switch (select_menu_num)
	{
	case EventManager::MenuOpenNum::WeaponShop:
		
		return select_menu_num == MenuOpenNum::WeaponShop;

		break;

	case EventManager::MenuOpenNum::Herdman:

		return select_menu_num == MenuOpenNum::Herdman;

		break;

	case EventManager::MenuOpenNum::Priest:

		return select_menu_num == MenuOpenNum::Priest;

		break;
	case EventManager::MenuOpenNum::Object:

		return select_menu_num == MenuOpenNum::Object;

		break;

	default:
		break;
	}
	
	

}

//���j���[�E�B���h�E�̐؂�ւ�
EventManager::MenuOpenNum EventManager::SetMenuNpc(int npcType)
{
	switch (npcType)
	{
	case 0:
		select_menu_num = MenuOpenNum::Herdman;
		return select_menu_num;
		break;

	case 1:
		select_menu_num = MenuOpenNum::Priest;
		return select_menu_num;
		break;

	case 2:
		select_menu_num = MenuOpenNum::WeaponShop;
		return select_menu_num;
		break;

	case 3:
		select_menu_num = MenuOpenNum::Object;
		return select_menu_num;
		break;


	default:

		return MenuOpenNum();

		break;
	}
	
}

//Npc�̃R�����g�̐؂�ւ�
void EventManager::NpcSwitchComent()
{
	switch (npcMove)
	{
	case EventManager::NpcMove::Empty:

		break;

	case EventManager::NpcMove::Herdman:

		menuComentWindiow->Menu_draw();

		//�A�C�R����\������
		DrawRotaGraph(herdman_icon_pos.x, herdman_icon_pos.y, herdman_scale, 0, herdman_icon_hdl, true);

		if (herdman_first_coment) {

			////��x�R�����g��������
			//Npc_coment_Array.clear();

			//�R�����g��\������
			NpcComent(herdman_coment_pos.x, herdman_coment_pos.y, 3, herdman_coment);

			//���̃R�����g��\���ł���悤�ɂ���
			herdman_first_coment = false;
			herdman_second_coment = true;

		}
		//Enter�L�[����������R�����g��؂�ւ���
		if (herdman_second_coment) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				Npc_coment_Array.clear();
				NpcComent(herdman_coment_pos.x, herdman_coment_pos.y, 2, herdman_coment_2);
				herdman_first_coment = true;
				herdman_second_coment = false;
			}
		}
	
		break;

	case EventManager::NpcMove::Priest:

		//���j���[�̃E�B���h�E��\������
		menuComentWindiow->Menu_draw();
		menuSelectWindow->Menu_draw();

		//�A�C�R����\������
		DrawRotaGraph(110, 600, 1.5f, 0, Priest_icon_hdl, true);

		//�R�����g��\������
		/*NpcComent(priest_coment_pos.x, priest_coment_pos.y, priest_coment, 2);*/

		break;

	default:

		break;
	}
}

//Npc�̃R�����g���Z�b�g����
EventManager::NpcMove EventManager::SetNpcComent(int npcType)
{
	switch (npcType)
	{
	case 0:
		npcMove = NpcMove::Empty;
		return npcMove;
		break;

	case 1:
		npcMove = NpcMove::Herdman;
		return npcMove;
		break;

	case 2:
		npcMove = NpcMove::Priest;
		return npcMove;
		break;

	default:
		return NpcMove();
		break;
	}
	
}

//�����͂������� : ���������s������Ƃ������s���Ŕ��ʂ��A�`�悷��
//�R�����g���A�j���[�V�����ŕ\������
void EventManager::NpcComent(int x, int y, int max_coment_num ,std::string npc_coment[])
{
	int LINE_HEIGHT = 50;
	int index = 0;

	for (int i = 0; i < max_coment_num; i++) {
		ComentLoad(npc_coment[i]);
	}

	if (!Npc_coment_Array.empty()) {
		// �R�����g�̐������R�����g��\������
		for (int i = 0; index < max_coment_num; i++, index++) {
			DrawStringEx(x, y + i * LINE_HEIGHT, -1, "%s", Npc_coment_Array[i].c_str());
		}
	}

}

//�R�����g��ǂݍ���
void EventManager::ComentLoad(const std::string& newComent)
{
	Npc_coment_Array.push_back(newComent);
}


void EventManager::NpcComentDraw()
{
	switch (select_menu_num)
	{
		//���퉮�̍ۂ̕`��
	case EventManager::MenuOpenNum::WeaponShop:

		//�E�B���h�E�̕\��
		menuSelectWindow->Menu_draw();

		//���j���[�̃o�b�N�O���E���h��`�悷��
		menuComentWindiow->Menu_draw();

		//���폤�l�̃A�C�R����\������
		DrawRotaGraph(90, 600, 1.5f, 0, Armsdealer_icon_hdl, true);

		//���폤�l�̃R�����g��\������
		armsdealerComentDraw(armsdealer_speak_coment);

		/*DrawStringEx(menuSelectWindow->menu_x + 50, menuSelectWindow->menu_y + 250, -1, "seqMenuOpen");*/

		break;

		//�����̎��̕`��
	case EventManager::MenuOpenNum::Herdman:

		//�E�B���h�E�̕\��
		menuSelectWindow->Menu_draw();

		//�b���R�}���h
		GameManager::getGameManager()->displayDialogue();

		break;

		//�_���̎��̕`��
	case EventManager::MenuOpenNum::Priest:

		//�E�B���h�E�̕\��
		menuSelectWindow->Menu_draw();

		//�b���R�}���h
		GameManager::getGameManager()->displayDialogue();

		break;

		//�I�u�W�F�N�g�̍ۂ̕`��
	case EventManager::MenuOpenNum::Object:

		//�E�B���h�E��\������
		menuComentWindiow->Menu_draw();

		//�v���C���[�̃A�C�R����\������
		DrawRotaGraph(100, 590, 1.5f, 0, Player_icon_hdl, true);

		//�I�u�W�F�N�g�̃R�����g��\������
		objectComentDraw();

		break;

	default:
		break;
	}


}

void EventManager::armsdealerComentDraw(int type)
{
	//���퉮�ɍŏ��ɘb�������Ƃ��̃R�����g
	if (type == armsdealer_first_comet) {
		DrawString(weaponShop.x, weaponShop.y, "���퉮", Color_White);
		DrawStringEx(armsdealerComent.x, armsdealerComent.y, Color_White, "�u ��������Ⴂ�I�v");
		DrawStringEx(WeaponShop_buy.x, WeaponShop_buy.y, Color_White, "(1 : ������w������)");
		DrawStringEx(menu_close.x, menu_close.y, Color_White, "(0 : ���j���[�����)");
	}

	//�Ȃɂ������Ƃ��̃R�����g
	else {
		DrawStringEx(armsdealerComent.x, armsdealerComent.y, Color_White, "�u ���𔃂��񂾂� �v");
	}
}

void EventManager::objectComentDraw()
{
	//���j���[�̃o�b�N�O���E���h��`�悷��
	DrawStringEx(object_coment.x, object_coment.y, Color_White, "�u ���͎g���Ă��Ȃ��B�݂�����\n   �K�тĂ���                 �v");
	DrawStringEx(object_menu_close.x, object_menu_close.y, Color_White, "(0 : ���j���[�����)");

}

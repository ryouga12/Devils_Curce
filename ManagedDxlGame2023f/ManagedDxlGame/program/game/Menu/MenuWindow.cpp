#include "MenuWindow.h"


Menu::Menu(const std::string& gh_path)
{
	LoadDivGraph(gh_path.c_str(), MENU_GHDL_ARRAY_MAX_SIZE, GHDL_WIDTH_NUM, GHDL_HEIGHT_NUM, GHDL_WIDTH_SIZE, GHDL_HEIGHT_SIZE, menu_gh);
	menu_live = false;
}

Menu::~Menu()
{
	tnl::DebugTrace("\n------------------------------------------------------------");
	tnl::DebugTrace("\n Menu���������܂���");
	tnl::DebugTrace("\n------------------------------------------------------------");

}

void Menu::Menu_draw(const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height)
{
	DrawGraph(menu_x, menu_y, menu_gh[0], TRUE);
	DrawExtendGraph(menu_x + ADD_OFSET_POS, menu_y, menu_x + menu_width - ADD_OFSET_POS, menu_y + ADD_OFSET_POS, menu_gh[1], TRUE);
	DrawGraph(menu_x + menu_width - 16, menu_y, menu_gh[2], TRUE);

	DrawExtendGraph(menu_x, menu_y + ADD_OFSET_POS, menu_x + ADD_OFSET_POS, menu_y + menu_height - ADD_OFSET_POS, menu_gh[3], TRUE);
	DrawExtendGraph(menu_x + ADD_OFSET_POS, menu_y + ADD_OFSET_POS, menu_x + menu_width - 16, menu_y + menu_height - ADD_OFSET_POS, menu_gh[4], TRUE);
	DrawExtendGraph(menu_x + menu_width - ADD_OFSET_POS, menu_y + ADD_OFSET_POS, menu_x + menu_width, menu_y + menu_height - ADD_OFSET_POS, menu_gh[5], TRUE);

	DrawGraph(menu_x, menu_y + menu_height - ADD_OFSET_POS, menu_gh[6], TRUE);
	DrawExtendGraph(menu_x + ADD_OFSET_POS, menu_y + menu_height - ADD_OFSET_POS, menu_x + menu_width - ADD_OFSET_POS, menu_y + menu_height, menu_gh[7], TRUE);
	DrawGraph(menu_x + menu_width - ADD_OFSET_POS, menu_y + menu_height - ADD_OFSET_POS, menu_gh[8], TRUE);
}

MenuWindow::MenuWindow(const std::string& gh_path, MenuElement_t* elements, const int& elements_num)

	: Menu(gh_path)

	, Select_Num(0)
	, m_IsStartOpend(false) // �R���X�g���N�^�ł́A�����o�ϐ��̏������͂����������Ƃ��ł���
	, elements_num_(elements_num)
	, MenuElement(elements)
{

	MenuElement = elements;
	coursorX = MenuElement[0].x - MINUS_OFFSET_COURSOR_POS;
}

MenuWindow::~MenuWindow()
{
	if (MenuElement) {
		delete[]MenuElement;	//�z���p������ꍇ�̏�����
	}

	tnl::DebugTrace("\n------------------------------------------------------------");
	tnl::DebugTrace("\n Menu���������܂���");
	tnl::DebugTrace("\n------------------------------------------------------------");

}

void MenuWindow::Open()
{
	menu_live = true;
	m_IsStartOpend = true;
	Select_Num = 0;
	menuInit = false;

}

void MenuWindow::Read()
{
	if (!menuInit) {
		coursorY = MenuElement[0].y + ADJUSTMENT_COURSOR_POS;
		menuInit = true;
	}
	if (m_IsStartOpend)return;
	if (manage_select_flag) {
		//����ɂ����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
			Select_Num = (Select_Num + 1) % elements_num_;
		}
		//���ɂ����
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
			Select_Num = (Select_Num + (elements_num_ - 1)) % elements_num_;

		}
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S) || tnl::Input::IsKeyDownTrigger(eKeys::KB_W))
	{
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		for (int i = 0; i < elements_num_; i++) {
			if (i == Select_Num) {
				coursorY = MenuElement[i].y + ADJUSTMENT_COURSOR_POS;
			}
		}
	}

	//�\���̂̔z��̗v�f���̕�����\������
	for (int i = 0; i < elements_num_; i++) {
		DrawFormatString(MenuElement[i].x, MenuElement[i].y, koni::Color::WHITE, MenuElement[i].name.c_str());
	}

	//�J�[�\���̕`��
	ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", coursorX, coursorY, 0.2, 0, true);

}

void MenuWindow::All(const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height)
{
	if (menu_live) {
		Menu_draw(menu_x, menu_y, menu_width, menu_height);
		Read();
		m_IsStartOpend = false;
	}

}

//�J�[�\������
void MenuWindow::SetSelectCousourMove()
{
	//�t���O�𔽓]����
	manage_select_flag = !manage_select_flag;
}



#include "MenuWindow.h"


Menu::Menu(const std::string& gh_path)
{
	int n = LoadDivGraph(gh_path.c_str(), 9, 3, 3, 16, 16, menu_gh);
	menu_live = false;


}

void Menu::Menu_draw(const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height)
{
	DrawGraph(menu_x, menu_y, menu_gh[0], TRUE);
	DrawExtendGraph(menu_x + 16, menu_y, menu_x + menu_width - 16, menu_y + 16, menu_gh[1], TRUE);
	DrawGraph(menu_x + menu_width - 16, menu_y, menu_gh[2], TRUE);

	DrawExtendGraph(menu_x, menu_y + 16, menu_x + 16, menu_y + menu_height - 16, menu_gh[3], TRUE);
	DrawExtendGraph(menu_x + 16, menu_y + 16, menu_x + menu_width - 16, menu_y + menu_height - 16, menu_gh[4], TRUE);
	DrawExtendGraph(menu_x + menu_width - 16, menu_y + 16, menu_x + menu_width, menu_y + menu_height - 16, menu_gh[5], TRUE);

	DrawGraph(menu_x, menu_y + menu_height - 16, menu_gh[6], TRUE);
	DrawExtendGraph(menu_x + 16, menu_y + menu_height - 16, menu_x + menu_width - 16, menu_y + menu_height, menu_gh[7], TRUE);
	DrawGraph(menu_x + menu_width - 16, menu_y + menu_height - 16, menu_gh[8], TRUE);
}

MenuWindow::MenuWindow(const std::string& gh_path, MenuElement_t* elements, int elements_num, double BackWidth)

	: Menu(gh_path)//���j���[�̑傫�������߂�(�J�n���W:����,����,�c��)

	, Select_Num(0)
	, m_IsStartOpend(false) // �R���X�g���N�^�ł́A�����o�ϐ��̏������͂����������Ƃ��ł���
	, read_menu_element_num(elements_num)
	, MenuElement(elements)
	, back_ghdl_width(BackWidth)
{

	MenuElement = elements;
	elements_num_ = elements_num;
	string_color_black = GetColor(0, 0, 0);
	coursorX = MenuElement[0].x - 20;

	/*selectItemBackGh = gManager->LoadGraphEX("graphics/selectItemBack.png");*/
}

MenuWindow::~MenuWindow()
{
	if (MenuElement) {
		delete[]MenuElement;	//�z���p������ꍇ�̏�����
	}

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
	if (menuInit == false) {
		coursorY = MenuElement[0].y + 8;
		menuInit = true;
	}
	if (m_IsStartOpend)return;
	if (manage_select_flag) {
		//����ɂ����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
			Select_Num = (Select_Num + 1) % read_menu_element_num;
		}
		//���ɂ����
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
			Select_Num = (Select_Num + (read_menu_element_num - 1)) % read_menu_element_num;

		}
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN) || tnl::Input::IsKeyDownTrigger(eKeys::KB_UP))
	{
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		for (int i = 0; i < elements_num_; i++) {
			if (i == Select_Num) {
				coursorY = MenuElement[i].y + 8;
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	for (int i = 0; i < elements_num_; i++) {
		DrawFormatString(MenuElement[i].x, MenuElement[i].y, -1, MenuElement[i].name.c_str());
	}
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", coursorX, coursorY, 0.2, 0, true);

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



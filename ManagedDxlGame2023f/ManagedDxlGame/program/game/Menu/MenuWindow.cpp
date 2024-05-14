#include "MenuWindow.h"


Menu::Menu(int menu_window_x, int menu_window_y, int menu_window_width, int menu_window_height, std::string gh_path)
{
	menu_x = menu_window_x;
	menu_y = menu_window_y;
	menu_width = menu_window_width;
	menu_height = menu_window_height;
	int n = LoadDivGraph(gh_path.c_str(), 9, 3, 3, 16, 16, menu_gh);
	menu_live = false;


}

void Menu::Menu_draw()
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

MenuWindow::MenuWindow(int menu_window_x, int menu_window_y, int menu_window_width, int menu_window_height, std::string gh_path, MenuElement_t* elements, int elements_num, double BackWidth)

	: Menu(menu_window_x, menu_window_y, menu_window_width, menu_window_height, gh_path)//メニューの大きさを決める(開始座標:左上,横幅,縦幅)

	, Select_Num(0)
	, m_IsStartOpend(false) // コンストラクタでは、メンバ変数の初期化はこう書くこともできる
	, read_menu_x(menu_window_x)
	, read_menu_y(menu_window_y)
	, read_menu_element_num(elements_num)
	, MenuElement(elements)
	, backGhwidth(BackWidth)
{
	
	MenuElement = elements;
	elements_num_ = elements_num;
	string_Color_Black = GetColor(0, 0, 0);
	coursorX = MenuElement[0].x - 20;

	/*selectItemBackGh = gManager->LoadGraphEX("graphics/selectItemBack.png");*/
}

MenuWindow::~MenuWindow()
{
	if (MenuElement) {
		delete[]MenuElement;	//配列を廃棄する場合の書き方
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
	if (manageSelectFlag != false) {
		//一つ下にずれる
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
			Select_Num = (Select_Num + 1) % read_menu_element_num;			
		}
		//一つ上にずれる
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
		DrawFormatString(MenuElement[i].x, MenuElement[i].y,-1, MenuElement[i].name.c_str());
	}
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", coursorX, coursorY, 0.2, 0, true);
	
}

void MenuWindow::All()
{
	if (menu_live) {
		Menu_draw();
		Read();
		m_IsStartOpend = false;
	}
	
}

//カーソル制御
void MenuWindow::SetSelectCousourMove(int flag)
{
	if (flag == 0) {
		manageSelectFlag = true;
	}
	else if (flag == 1) {
		manageSelectFlag = false;
	}

}



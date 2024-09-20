#include "MenuWindow.h"


Menu::Menu(const std::string& gh_path)
{
	LoadDivGraph(gh_path.c_str(), MENU_GHDL_ARRAY_MAX_SIZE, GHDL_WIDTH_NUM, GHDL_HEIGHT_NUM, GHDL_WIDTH_SIZE, GHDL_HEIGHT_SIZE, menu_gh);
	menu_live = false;
}

Menu::~Menu()
{
	tnl::DebugTrace("\n------------------------------------------------------------");
	tnl::DebugTrace("\n Menuが解放されました");
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
	, m_IsStartOpend(false) // コンストラクタでは、メンバ変数の初期化はこう書くこともできる
	, elements_num_(elements_num)
	, MenuElement(elements)
{

	MenuElement = elements;
	coursorX = MenuElement[0].x - MINUS_OFFSET_COURSOR_POS;
}

MenuWindow::~MenuWindow()
{
	if (MenuElement) {
		delete[]MenuElement;	//配列を廃棄する場合の書き方
	}

	tnl::DebugTrace("\n------------------------------------------------------------");
	tnl::DebugTrace("\n Menuが解放されました");
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
		//一つ下にずれる
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
			Select_Num = (Select_Num + 1) % elements_num_;
		}
		//一つ上にずれる
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

	//構造体の配列の要素内の文字を表示する
	for (int i = 0; i < elements_num_; i++) {
		DrawFormatString(MenuElement[i].x, MenuElement[i].y, koni::Color::WHITE, MenuElement[i].name.c_str());
	}

	//カーソルの描画
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

//カーソル制御
void MenuWindow::SetSelectCousourMove()
{
	//フラグを反転する
	manage_select_flag = !manage_select_flag;
}



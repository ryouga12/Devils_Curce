
///UIに関するクラス
///選択肢を持たないMenuと
///選択肢を持つMenuWindowがある



#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include<string>
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"

class Menu {
public:

	Menu();
	int menu_x, menu_y, menu_width, menu_height = 0;
	int menu_gh[9] = { 0 };
	bool menu_live;

	//newするときに大きさとghのpathを引数で渡す
	Menu(int menu_window_x, int menu_window_y, int menu_window_width, int menu_window_height, std::string gh_path);

	void Menu_draw();

};

class MenuWindow : public Menu {
public:

	//メニューの要素の構造体
	typedef struct {
		int x;
		int y;
		std::string name;
		int menu_num;
	}MenuElement_t;
	
	MenuWindow(int menu_window_x, int menu_window_y, int menu_window_width, int menu_window_height, std::string gh_path, MenuElement_t* elements, int elements_num, double BackWidth);
	~MenuWindow();

	//メニューを開く
	void Open();

	void Read();

	void All();

	int getSelectNum() { return Select_Num;}

	void SetSelectCousourMove(int flag);

private:

	//現在の選択番号
	int Select_Num;

	bool menuInit = false;

	int read_menu_x = 0;
	int read_menu_y = 0;
	int read_menu_element_num = 0;

	int string_Color_Black = 0;
	int elements_num_;
	bool m_IsStartOpend;									//ESCキーで開いた1フレーム目

	//Menuの構造体のポインタ
	MenuElement_t*  MenuElement = nullptr;

	int cursor_gh = 0;
	int coursorX = 0;
	int coursorY = 0;

	int selectItemBackGh = 0;
	double backGhwidth = 0;

	bool secondMenuFlag = false;
	bool manageSelectFlag = true;//Read関数のSelectNumを動かすかどうか trueなら動かす
};
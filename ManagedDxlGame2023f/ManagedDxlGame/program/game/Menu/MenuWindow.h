
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

	Menu() = default;

	//newするときにghのpathを引数で渡す
	Menu(const std::string& gh_path);

	virtual~Menu();

	//メニューの描画を行う
	//arg_1 : X座標
	//arg_2 : Y座標
	//arg_3 : 幅のサイズ
	//arg_4 : 縦のサイズ
	void Menu_draw(const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height);

private:

	int menu_x, menu_y, menu_width, menu_height = 0;
	int menu_gh[9] = { 0 };
	
protected:

	bool menu_live;

};

class MenuWindow final: public Menu {
public:

	//メニューの要素の構造体
	typedef struct {
		int x;
		int y;
		std::string name;
		int menu_num;
	}MenuElement_t;
	
	//選択肢のあるメニューのウィンドウ
	//arg_1 : 画像のpath
	//arg_2 : メニューの構造体のポインタ
	//arg_3 : 要素の数
	MenuWindow(const std::string& gh_path, MenuElement_t* elements, const int& elements_num);
	~MenuWindow()override;

	//メニューを開く
	void Open();

	//カーソル操作
	void Read();

	//メニューの描画とカーソル操作を行えるようにする
	void All(const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height);

	int getSelectNum() { return Select_Num;}

	//カーソルを制御する
	void SetSelectCousourMove();

	//カーソルの制御フラグを取得する
	bool getSelectCursour(){
		return manage_select_flag;
	}

	bool getAcrionFlag() { return is_active; }

	//処理を受け付けるようにする
	void IsActiveChangeFlag() { is_active = !is_active; }

	//メニューを使う際の定数関連

	//要素
	enum Elements {

	//一要素目
	FIRST_ELEMENT,
	//二要素目
	SECOND_ELEMENT,
	//三要素目
	THERD_ELEMENT,
	//四要素目
	FOUR_ELEMENT,
	//五要素目
	FIVE_ELEMENT,
	//最大値
	ELEMENTS_MAX
	};


private:

	//現在の選択番号
	int Select_Num;

	bool menuInit = false;

	int read_menu_element_num = 0;

	int string_color_black = 0;
	int elements_num_;
	bool m_IsStartOpend;									//ESCキーで開いた1フレーム目

	//Menuの構造体のポインタ
	MenuElement_t*  MenuElement = nullptr;

	int cursor_gh = 0;
	int coursorX = 0;
	int coursorY = 0;

	bool second_menu_flag = false;
	bool manage_select_flag = true;//Read関数のSelectNumを動かすかどうか trueなら動かす
	bool is_active = false;		   //プレイヤーからEnterキーなどを受け付けるかどうかのフラグ
};
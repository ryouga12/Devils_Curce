
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

	//---定数系---//

	//メニュー配列のサイズ
	static const int MENU_GHDL_ARRAY_MAX_SIZE = 9;

	//横幅の数
	const int GHDL_WIDTH_NUM = 3;
	//縦の数
	const int GHDL_HEIGHT_NUM = 3;
	//幅のサイズ
	const int GHDL_WIDTH_SIZE = 16;
	//縦のサイズ
	const int GHDL_HEIGHT_SIZE = 16;

	//メニューのグラフィックのハンドル
	int menu_gh[MENU_GHDL_ARRAY_MAX_SIZE] = { 0 };

	//追加座標
	const int ADD_OFSET_POS = 16;
	
protected:

	//メニュー関連の呼び出しフラグ
	bool menu_live = false;

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

	//選択番号を取得する
	int GetSelectNum() { return Select_Num;}

	//カーソルを制御する
	void SelectCousourMoveFlagChange();

	//カーソルの制御フラグを取得する
	bool GetSelectCursour(){
		return manage_select_flag;
	}

	bool GetAcrionFlag() { return is_active; }

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

	//初期化フラグ
	bool menuInit = false;

	//要素の数
	int elements_num_;

	//ESCキーで開いた1フレーム目
	bool m_IsStartOpend;									

	//Menuの構造体のポインタ
	MenuElement_t*  MenuElement = nullptr;

	//メニューウィンドウのX座標
	int coursorX = 0;
	//メニューウィンドウのY座標
	int coursorY = 0;

	//Read関数のSelectNumを動かすかどうか trueなら動かす
	bool manage_select_flag = true;

	//プレイヤーからEnterキーなどを受け付けるかどうかのフラグ
	bool is_active = false;		  

	//---定数系---//

	//カーソルのX座標の位置を設定する為の引く値
	const int MINUS_OFFSET_COURSOR_POS = 20;

	//カーソルの調整座標
	const int ADJUSTMENT_COURSOR_POS = 8;

};
#pragma once
//------------------------------------------------------------------------------------------------------------
//
//UIをまとめるマネージャー
//
//-------------------------------------------------------------------------------------------------------------

#include "../../dxlib_ext/dxlib_ext.h"
#include"../Menu/MenuWindow.h"
#include <unordered_map>

class CsvManager;
class GameManager;

class UIManager final {

public:

	static UIManager* getUIManager();
	static void delategetUIManager() { delete getUIManager(); }

	//メニューを追加する
	void addMenu(const std::string& menuName, std::shared_ptr<Menu> menu);

	// メニューを取得する
	std::shared_ptr<Menu> getMenu(const std::string& menuName) {

		auto it = menu_map.find(menuName);

		if (it != menu_map.end()) {
			return it->second;
		}

		return nullptr;
	}

	//メニューを描画する
	void Menu_Draw(const std::string& menuName, const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height);

	//武器屋のコメント
	void armsdealerComentDraw(int type);

	//コメントをロードする
	void ComentLoad(const int max_num ,const std::string& name);

	//コメントを表示する
	void ComentDraw(const tnl::Vector2i& coment_pos, const int max_draw_num);

	//コメントを初期化して１から表示させる
	void ComentClear() { curent_num = 0; }

	//カウンターを取得する
	int GetCount()const { return count; }

	//カウンターをリセットする
	void CountReset() { count = 0; }

	//プレイヤーのステータスをHPバーで表示する
	void PlayerStatusDrawWindow();
	
private:

	UIManager(){}
	~UIManager() {};

	//メニューを管理するマップ
	std::unordered_map<std::string, Shared<Menu>>menu_map;

	//コメントを格納する配列
	std::vector<std::string>coment_;

	//---それぞれのコメント---//

	//武器商人のコメントの座標
	const tnl::Vector2i ARMSDEALERCOMENT = { 150, 550 };

	//武器屋の話しかけたときのコメント
	const int ARMSDEALER_FIRST_COMET = 0;

	//武器屋の文字の座標
	const tnl::Vector2i WEAPONSHOP = { 60 , 520 };

	//武器屋の購入コメントの座標
	const tnl::Vector2i WEAPONSHOP_BUY = { 70, 100 };

	//武器屋のメニューを閉じるコメントの座標
	const tnl::Vector2i MENU_CLOSE = { 70, 150 };

	//白色
	const int COLOR_WHITE = -1;

	//現在のコメント
	int curent_num = 0;

	//カウンター(主にカウントを使ってコメント後のシーンを遷移させたりする)
	int count = 0;
};
#include "UiManager.h"
#include"CsvManager.h"
#include"GameManager.h"

UIManager* UIManager::getUIManager()
{
	static UIManager* p_instance = nullptr;

	if (!p_instance)p_instance = new UIManager();
	return p_instance;
}

void UIManager::addMenu(const std::string& menuName, std::shared_ptr<Menu> menu)
{
	//メニューマップにあるかを確認して
	auto it = menu_map.find(menuName);

	//すでにあれば処理を飛ばす
	if (it != menu_map.end()) {
		return;
	}
	//もし無ければ要素を追加する
	else {
		menu_map[menuName] = menu;
	}

}

void UIManager::Menu_Draw(const std::string& menuName, const int&  menu_x, const int& menu_y, const int&  menu_width, const int& menu_height)
{
	//名前を検索する
	auto it = menu_map.find(menuName);

	//見つかったら描画する
	if (it != menu_map.end()) {
		(*it).second->Menu_draw(menu_x, menu_y, menu_width, menu_height);
	}
	//見つからなかったら処理を飛ばす
	else {
		return;
	}

}

//武器屋のコメント表示
void UIManager::armsdealerComentDraw(int type)
{
	//武器屋に最初に話かけたときのコメント
	if (type == ARMSDEALER_FIRST_COMET) {
		DrawString(WEAPONSHOP.x, WEAPONSHOP.y, "武器屋", COLOR_WHITE);
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, COLOR_WHITE, "「 いらっしゃい！」");
		DrawStringEx(WEAPONSHOP_BUY.x, WEAPONSHOP_BUY.y, -1, "(1 : 武器を購入する)");
		DrawStringEx(MENU_CLOSE.x, MENU_CLOSE.y, COLOR_WHITE, "(0 : メニューを閉じる)");
	}
	//なにか買うときのコメント
	else {
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, COLOR_WHITE, "「 何を買うんだい 」");
	}
}

//コメントをロードする
void UIManager::ComentLoad(const int max_num , const std::string& name)
{
	//一度コメントを初期化する
	coment_.clear();

	auto coment_csv = CsvManager::getCsvManager()->GetComentCsv();

	for (int i = 1; i < coment_csv.size(); i++) {

		//名前がcsvの1行目の為、0
		if (name == coment_csv[i][0].c_str()) {

			for (int k = 1; k < max_num; k++) {

				coment_.emplace_back(coment_csv[i][k]);

			}
		}

	}
}

//コメントを描画する
void UIManager::ComentDraw(const tnl::Vector2i& coment_pos,const int max_draw_num)
{
	//描画の最大数を超えてなければ
	if (curent_num < max_draw_num) {
		//Enterキーを受け付ける
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//数を増やす
			curent_num++;
			//カウントを増やす
			count++;
		}
	}

	DrawStringEx(coment_pos.x, coment_pos.y, COLOR_WHITE, "%s", coment_[curent_num].c_str());


}

//プレイヤーのステータスをHPバーで表示する
void UIManager::PlayerStatusDrawWindow()
{
	//プレイヤーのステータスを取得する
	auto& plyerstatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

	//プレイヤーの画像
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/Player/Player_Icon_sentou.png", 90, 120, 1.3f, 0, true);

	//hpバーの画像
	int hpber_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/hpbar.png");
	//mpバーの画像
	int mpber_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/mpbar.png");
	//hpバー最大の長さ
	int maxHpBar = 200;
	//mpバーの最大の長さ
	int maxMpBer = 200;

	// HPの割合を計算する
	float hp_ratio = plyerstatus.GetcurentHp() / plyerstatus.GetMaxHp();
	// HPバーの長さを計算する
	int hpbar = static_cast<int>(maxHpBar * hp_ratio);

	// MPの割合を計算する
	float mp_ratio = plyerstatus.GetCurentMp() / plyerstatus.GetMaxMp();
	// MPバーの長さを計算する
	int mpbar = static_cast<int>(maxMpBer * mp_ratio);

	DrawExtendGraph(75, 155, (75 + hpbar), 185, hpber_hdl, true);
	DrawExtendGraph(75, 215, (75 + mpbar), 245, mpber_hdl, true);

	//プレイヤーのHPを表示する
	DrawStringEx(75, 70, -1, "Player");
	DrawStringEx(120, 110, -1, " Lv %d", plyerstatus.GetLevel());
	DrawStringEx(75, 190, -1, " Hp : %d / %d", static_cast<int>(plyerstatus.GetcurentHp()), static_cast<int>(plyerstatus.GetMaxHp()));
	DrawStringEx(75, 250, -1, " Mp : %d / %d", static_cast<int>(plyerstatus.GetCurentMp()), static_cast<int>(plyerstatus.GetMaxMp()));

}

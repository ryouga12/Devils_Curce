#include"EventManager.h"
#include"../Manager/UiManager.h"
#include"../Manager/SceneManager.h"
#include"../Scene/BaseScene.h"
#include"../Scene/battleScene.h"
#include"../Item/WeaponShop.h"
#include"../System/Event.h"
#include"../Object/NPC.h"
#include"../Manager/GameManager.h"
#include"../Manager/ResourceManager.h"
#include"../Item/Inventory.h"


EventManager* EventManager::GetEventManager()
{
	static EventManager* p_instance = nullptr;

	if (!p_instance)p_instance = new EventManager();
	return p_instance;
}

void EventManager::DelateEventManager()
{
	delete GetEventManager();
}


EventManager::EventManager()
{
	//ウィンドウ関連の初期化
	InitMenuEventConnection();
}

void EventManager::InitEventRelated(std::list<Shared<Npc>>& npc_list)
{
	npc_move = NpcMove::EMPTY;

	//nullが入っていた場合生成する
	if (!function_npc) {
		function_npc = std::make_shared<Event>();
	}

	//空じゃ無ければ
	if (!npc_list.empty()) {

		//処理を初期化する
		function_npc->InitEventFunction(npc_list);

		//イベントを起こせるようにする
		event_npc_flag = true;

		//アイコンの画像ハンドルを初期化する
		//他のマップでも同じ名前が存在する為、マップによってアイコンを切り替える
		for (auto& npc : npc_list) {

			//ポインタが存在しない場合配列を抜ける
			if (!npc) { break; }

			if (npc->GetNpcName() == "武器商人") {

				//メニューで使うアイコン
				Armsdealer_icon_hdl = npc->GetNpcIconImage();
			}
			else if (npc->GetNpcName() == "村長") {

				//メニューで使うアイコン
				herdman_icon_hdl = npc->GetNpcIconImage();
			}
			else if (npc->GetNpcName() == "神官") {

				//メニューで使うアイコン
				Priest_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "町長") {

				//メニューで使うアイコン
				town_herdman_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "宿屋") {

				//メニューで使うアイコン
				inn_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "魔王") {

				//メニューで使うアイコン
				boss_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "魔物") {

				//メニューで使うアイコン
				enemy_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "王") {

				//メニューで使うアイコン
				king_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "王妃") {

				//メニューで使うアイコン
				queen_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "女性村人") {

				//メニューで使うアイコン
				female_resident_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "男性村人") {

				//メニューで使うアイコン
				resident_male_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "男性町民") {

				//メニューで使うアイコン
				resident_male_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "男性住民") {

				//メニューで使うアイコン
				resident_male_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "学者") {

				//メニューで使うアイコン
				scholar_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "兵士") {

				//メニューで使うアイコン
				resident_male_icon_hdl = npc->GetNpcIconImage();

			}
			else if (npc->GetNpcName() == "試練の番人") {

				//メニューで使うアイコン
				ordeal_icon_hdl = npc->GetNpcIconImage();
			}
			else if (npc->GetNpcName() == "影の番人") {

				//メニューで使うアイコン
				shadow_icon_hdl = npc->GetNpcIconImage();
			}
			else if (npc->GetNpcName() == "兵長") {

				//メニューで使うアイコン
				corporal_icon_hdl = npc->GetNpcIconImage();
			}
			else if (npc->GetNpcName() == "船長") {
				pirate_icon_hdl = npc->GetNpcIconImage();
			}
			else if (npc->GetNpcName() == "僧侶") {
				monk_icon_hdl = npc->GetNpcIconImage();
			}
		}
	}
}

void EventManager::InitMenuEventConnection()
{
	//最初の一回だけロードする
	if (!UIManager::GetUIManager()->GetMenu("select_answer_menu")) {

		//プレイヤーの選択ウィンドウ内の文字の座標
		const tnl::Vector2i PLAYER_SELECT_WINDOW_ANSWER_OK_POS = { 100 , 100 };
		const tnl::Vector2i PLAYER_SELECT_WINDOW_ANSWER_NO_POS = { 100 , 150 };

		//プレイヤーに選択させるウィンドウ
		player_select_coment = new MenuWindow::MenuElement_t[]{
			{PLAYER_SELECT_WINDOW_ANSWER_OK_POS.x , PLAYER_SELECT_WINDOW_ANSWER_OK_POS.y , "はい" , MenuWindow::Elements::FIRST_ELEMENT},
			{PLAYER_SELECT_WINDOW_ANSWER_NO_POS.x , PLAYER_SELECT_WINDOW_ANSWER_NO_POS.y , "いいえ", MenuWindow::Elements::SECOND_ELEMENT}
		};

		//メニューウィンドウの最大要素数
		const int MENU_ELEMENTS_MAX = 2;

		//プレイヤーに対する行動選択ウィンドウ
		player_select_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", player_select_coment, MENU_ELEMENTS_MAX);
		player_select_window->Open();

		//mapに格納する
		UIManager::GetUIManager()->addMenu("select_answer_menu", player_select_window);
	}
}

//Npcのコメントの切り替え
void EventManager::NpcSwitchComent(const Shared<WeaponShop>& weapon_shop)
{
	//武器屋文字座標
	const tnl::Vector2i WEAPON_SHOP_STRING_POS = { 60 , 520 };

	//セーブテキスト文字座標
	const tnl::Vector2i SAVE_TEXT_STRING_POS = { 150, 630 };

	//プレイヤーに選択させるウィンドウの座標
	const tnl::Vector2i PLAYER_ANSWER_SELECT_WINDOW_POS = { 50, 50 };

	//プレイヤーに選択させるウィンドウのサイズ
	const int  PLAYER_ANSWER_SELECT_WINDOW_WIDTH  =  250;
	const int  PLAYER_ANSWER_SELECT_WINDOW_HEIGHT =  200;

	//現在の所持金を表示する文字
	const tnl::Vector2i POSSESSION_STRING_POS = { 135, 100 };

	//イベントが発生しなかった時の文字の座標
	const tnl::Vector2i EVENT_NOT_STRING_POS = { 150, 630 };

	switch (npc_move)
	{
	case EventManager::NpcMove::EMPTY:

		break;

	//村長
	case EventManager::NpcMove::HERDMAN:

		//コメントのウィンドウを表示する
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, herdman_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);
	
		break;

	//神官
	case EventManager::NpcMove::PRIEST:

		//コメントのウィンドウを表示する
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, Priest_icon_hdl, true);
		
		//セーブテキスト
		UIManager::GetUIManager()->SaveText(SAVE_TEXT_STRING_POS);

		if (player_select_window->GetAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y , PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);
		
	
		break;

	//武器屋
	case EventManager::NpcMove::WEAPONSHOP:

		//コメントのウィンドウの描画
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//武器屋関連の表示
		DrawString(WEAPON_SHOP_STRING_POS.x, WEAPON_SHOP_STRING_POS.y, "武器屋", -1);

		//コメントとアイコン
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, Armsdealer_icon_hdl , true);

		//武器屋のコメント
		weapon_shop->Draw();

		//購入を選択されたら
		if (weapon_shop->GetWeapoShopTransaction() == WeaponShop::TransactionType::BUY) {

			//プレイヤーの所持金を描画
			DrawStringEx(POSSESSION_STRING_POS.x, POSSESSION_STRING_POS.y, koni::Color::WHITE, "所持金: %d", GameManager::GetGameManager()->GetPlayer()->GetPlayerMoney());
		}

		break;

	//影の番人
	case EventManager::NpcMove::SHADOWENEMY:

		//コメントのウィンドウを表示する
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//コメントとアイコン
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, shadow_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//選択ウィンドウを表示する
		if (player_select_window->GetAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;


	//ボスモンスター
	case EventManager::NpcMove::BOSS:

		//コメントのウィンドウを表示する
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//コメントとアイコン
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, boss_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//選択ウィンドウを表示する
		if (player_select_window->GetAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;


	//宿屋
	case  EventManager::NpcMove::INN:

		//コメントのウィンドウを表示する
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//コメントとアイコン
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, inn_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		if (player_select_window->GetAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;

	//町長
	case EventManager::NpcMove::TOWNHERDMAN:

		//コメントのウィンドウを表示する
		UIManager::GetUIManager()->Menu_Draw("menu_window",COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, town_herdman_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//敵
	case EventManager::NpcMove::ENEMY:

		//コメントのウィンドウを表示する
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, enemy_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//選択ウィンドウを表示する
		if (player_select_window->GetAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;

	//王
	case  EventManager::NpcMove::KING:

		//コメントのウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, king_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//王妃
	case  EventManager::NpcMove::QUEEN:

		//コメントのウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, queen_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//女性村人
	case  EventManager::NpcMove::FEMALE_RESIDENT:

		//コメントのウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, female_resident_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//男性住人
	case  EventManager::NpcMove::RESIDENT_MALE:

		//コメントのウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, resident_male_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//学者
	case  EventManager::NpcMove::SCHOLAR:
		
		//コメントのウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, scholar_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	//兵長
	case EventManager::NpcMove::CORPORAL:

		//コメントのウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, corporal_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//選択ウィンドウを表示する
		if (player_select_window->GetAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;

	//試練の番人
	case EventManager::NpcMove::ORDEAL:

		//コメントのウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, ordeal_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//選択ウィンドウを表示する
		if (player_select_window->GetAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		//必要アイテムをもっていない場合、文字を表示する
		if (ordeal_coment_flag) {
			DrawStringEx(EVENT_NOT_STRING_POS.x, EVENT_NOT_STRING_POS.y, koni::Color::WHITE, "お前には資格が無いようだ");
		}

		break;

	//船長
	case EventManager::NpcMove::PIRATE:

		//コメントのウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, pirate_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		//選択ウィンドウを表示する
		if (player_select_window->GetAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(PLAYER_ANSWER_SELECT_WINDOW_POS.x, PLAYER_ANSWER_SELECT_WINDOW_POS.y, PLAYER_ANSWER_SELECT_WINDOW_WIDTH, PLAYER_ANSWER_SELECT_WINDOW_HEIGHT);
		}

		break;

	//僧侶
	case  EventManager::NpcMove::MONK:

		//コメントのウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", COMENT_WINDOW_POS.x, COMENT_WINDOW_POS.y, COMENT_WINDOW_WIDTH, COMENT_WONDOW_HEIGHT);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, koni::Numeric::SCALE_ONE_AND_HALF, 0, monk_icon_hdl, true);

		//コメントを表示する
		UIManager::GetUIManager()->ComentDraw(COMENT_POS);

		break;

	default:

		break;
	}
}

//Npcなどのそれぞれのイベント時の更新処理
//わかりやすいように分ける
void EventManager::NpcEventUpdate(float delta_time, const std::list<Shared<Npc>>& npc_list,BaseScene* curent_map_scene, const int& curent_inmap_scene )
{
	//---表示するコメントの数---//

	//村長
	const int HERDMAN_COMENT_MAX = 7;
	//神官
	const int PRIEST_COMENT_MAX = 1;
	//宿屋
	const int INN_COMENT_MAX = 1;
	//町長
	const int TOWN_HERDMAN_COMENT_MAX = 2;
	//魔物
	const int ENEMY_COMENT_MAX = 3;
	//王
	const int KING_COMENT_MAX = 4;
	//王妃
	const int QUEEN_COMENT_MAX = 1;
	//女性住人
	const int FEMALE_RESIDENT_COMENT_MAX = 1;
	//男性住人
	const int MALE_VILLAGERS_COMENT_MAX = 1;
	//学者
	const int SCHOLAR_COMENT_MAX = 5;
	//試練の番人
	const int ORDEAL_COMENT_MAX = 3;
	//ラスボス
	const int BOSS_COMENT_MAX_NUM = 4;
	//影の番人
	const int SHADOW_ENEMY_COMENT_MAX = 2;
	//兵長
	const int CORPORAL_COMENT_MAX = 2;
	//船長
	const int PIRATE_COMENT_MAX = 4;
	//僧侶
	const int MONK_COMENT_MAX = 1;
		
	//シーンマネージャーのポインタ
	auto mgr = SceneManager::GetSceneManager();

	switch (npc_move)
	{
	case EventManager::NpcMove::EMPTY:


		break;

	//村長
	case EventManager::NpcMove::HERDMAN:

		//コメントを表示する
		ComentEvent(curent_map_scene, HERDMAN_COMENT_MAX, npc_list);

		break;


	//神官
	case EventManager::NpcMove::PRIEST:

		//コメントを表示する
		if (function_npc->ComentEventChange(PRIEST_COMENT_MAX) && event_npc_flag) {

			//選択処理
			//セーブ機能
			PlayerAnswerSelect(npc_list , curent_map_scene ,curent_inmap_scene);

			//フラグがfalseの場合
			if (!player_select_window->GetAcrionFlag() && event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//影の番人
	case EventManager::NpcMove::SHADOWENEMY:

		//コメントを表示する
		if (function_npc->ComentEventChange(SHADOW_ENEMY_COMENT_MAX) && event_npc_flag) {

			//選択処理
			PlayerAnswerSelect(npc_list, curent_map_scene);

			//フラグがfalseの場合
			if (!player_select_window->GetAcrionFlag() && event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//ボス
	case EventManager::NpcMove::BOSS:

		//フォントの大きさを変える
		SetFontSize(20);

		//コメントを表示する
		if (function_npc->ComentEventChange(BOSS_COMENT_MAX_NUM)) {

			//選択処理 & 敵との会話処理
			PlayerAnswerSelect(npc_list , curent_map_scene);

			//フラグがfalseの場合
			if (!player_select_window->GetAcrionFlag() && event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}
		}

		//フォントの大きさを戻す
		SetFontSize(16);

		break;

	//宿屋
	case EventManager::NpcMove::INN:

		//コメントを表示する
		if (function_npc->ComentEventChange(INN_COMENT_MAX)) {

			//選択処理 & 宿屋処理
			//プレイヤーのHpとMpの回復処理
			PlayerAnswerSelect(npc_list , curent_map_scene);

			//フラグがfalseの場合
			if (!player_select_window->GetAcrionFlag()&& event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//町長
	case EventManager::NpcMove::TOWNHERDMAN:

		//コメントを表示する
		ComentEvent(curent_map_scene, TOWN_HERDMAN_COMENT_MAX, npc_list);

		break;

	//敵
	case EventManager::NpcMove::ENEMY:

		//コメントを表示する
		if (function_npc->ComentEventChange(ENEMY_COMENT_MAX)){

			//選択処理 & 敵との会話処理
			PlayerAnswerSelect(npc_list , curent_map_scene);

			//フラグがfalseの場合
			if (!player_select_window->GetAcrionFlag() && event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//王
	case EventManager::NpcMove::KING:

		//コメントを表示する
		ComentEvent(curent_map_scene, KING_COMENT_MAX, npc_list);

		break;

	//王妃
	case EventManager::NpcMove::QUEEN:

		//コメントを表示する
		ComentEvent(curent_map_scene, QUEEN_COMENT_MAX, npc_list);

		break;

	//女性住人
	case EventManager::NpcMove::FEMALE_RESIDENT:

		//コメントを表示する
		ComentEvent(curent_map_scene, FEMALE_RESIDENT_COMENT_MAX, npc_list);

		break;

	//男性住人
	case EventManager::NpcMove::RESIDENT_MALE:

		//コメントを表示する
		ComentEvent(curent_map_scene, MALE_VILLAGERS_COMENT_MAX, npc_list);

		break;
	
	//学者
	case EventManager::NpcMove::SCHOLAR:

		//コメントを表示する
		ComentEvent(curent_map_scene, SCHOLAR_COMENT_MAX, npc_list);

		break;

	//墓の番人
	case EventManager::NpcMove::ORDEAL:

		//コメントを表示する
		if (function_npc->ComentEventChange(ORDEAL_COMENT_MAX)) {

			//選択処理 & 敵との会話処理
			PlayerAnswerSelect(npc_list, curent_map_scene);

			//フラグがfalseの場合
			if (!player_select_window->GetAcrionFlag() && event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}
			
		}

		break;

	//兵長
	case EventManager::NpcMove::CORPORAL:

		//コメントを表示する
		if (function_npc->ComentEventChange(CORPORAL_COMENT_MAX)) {

			//選択処理 & 敵との会話処理
			PlayerAnswerSelect(npc_list, curent_map_scene);

			//フラグがfalseの場合
			if (!player_select_window->GetAcrionFlag() && event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}

		}

		break;

	//船長
	case EventManager::NpcMove::PIRATE:

		//コメントを表示する
		if (function_npc->ComentEventChange(PIRATE_COMENT_MAX)) {

			//選択処理 & 敵との会話処理
			PlayerAnswerSelect(npc_list, curent_map_scene);

			//フラグがfalseの場合
			if (!player_select_window->GetAcrionFlag() && event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}

		}

		break;

	//僧侶
	case EventManager::NpcMove::MONK:

		//コメントを表示する
		ComentEvent(curent_map_scene, MONK_COMENT_MAX, npc_list);

		break;

	default:
		break;
	}
}

//プレイヤーに行動を選択させる
void EventManager::PlayerAnswerSelect(const std::list<Shared<Npc>>& npc_list , BaseScene* curent_map_scene , const int& inmap_curent_state)
{
	//プレイヤーがはいを選択したら
	if (player_select_window->GetSelectNum() == MenuWindow::Elements::FIRST_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		&& player_select_window->GetAcrionFlag())
	{
		//NPCのによってそれぞれ処理を変える
		ProcessNpcEventByType(npc_list , curent_map_scene , inmap_curent_state);
	}
	//プレイヤーがいいえを選択したら
	else if (player_select_window->GetSelectNum() == MenuWindow::Elements::SECOND_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		&& player_select_window->GetAcrionFlag())
	{
		player_select_window->IsActiveChangeFlag();
		event_npc_flag = false;
		npc_move = NpcMove::EMPTY;
		curent_map_scene->SequenceChangeMove();
	}
}

//NPCによって処理を変える
void EventManager::ProcessNpcEventByType(const std::list<Shared<Npc>>& npc_list, BaseScene* curent_map_scene , const int& inmap_curent_state)
{
	for (auto& npc_list_ : npc_list) {

		//神官に話かけた場合
		if (npc_list_->GetNpcName() == ("神官") && npc_move == NpcMove::PRIEST) {

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), inmap_curent_state);

			//処理を受け付けないようにする
			player_select_window->IsActiveChangeFlag();

			event_npc_flag = false;

			break;

		}
		//宿屋に話かけた場合
		else if (npc_list_->GetNpcName() == "宿屋" && npc_move == NpcMove::INN) {

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), INN_MONEY);

			//処理を受け付けないようにする
			player_select_window->IsActiveChangeFlag();

			event_npc_flag = false;

			//プレイヤーを動けるようにする
			curent_map_scene->SequenceChangeMove();

			break;
		}
		//敵に話かけた場合
		else if (npc_list_->GetNpcName() == "魔物" && npc_move == NpcMove::ENEMY) {

			//中ボスのID
			boss_id = 21;

			//ボスシーンでの背景をロードする
			boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/town.png");

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/town_bgm.mp3", "sound/BGM/tyubosu.mp3", static_cast<int>(InMapScene::InMapState::TOWN));

			//処理を受け付けないようにする
			if (player_select_window->GetAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

			event_npc_flag = false;

			break;
		}
		//影の番人
		else if (npc_list_->GetNpcName() == "影の番人") {

			boss_id = 28;

			//ボスシーンでの背景をロードする
			boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/boss_castle.png");

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/maou_bgm_castle.mp3", "sound/BGM/battle-dark.mp3", static_cast<int>(InMapScene::InMapState::BOSSCASTLE));

			//処理を受け付けないようにする
			if (player_select_window->GetAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

			event_npc_flag = false;

			break;
		}
		else if (npc_list_->GetNpcName() == "魔王") {

			//ボスのID
			boss_id = 20;

			//ボスシーンでの背景をロードする
			boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/boss_castle.png");

			function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/maou_bgm_castle.mp3", "sound/BGM/maou_sentou_bgm.mp3" , static_cast<int>(InMapScene::InMapState::BOSSROOM));

			//処理を受け付けないようにする
			if (player_select_window->GetAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

			event_npc_flag = false;

			break;

		}
		else if (npc_list_->GetNpcName() == "試練の番人") {

			if (CheckEventItem(ESSENTIAL_ITEM_ID , FALSE)) {

				//ボスのID
				boss_id = 27;

				//ボスシーンでの背景をロードする
				boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/setugen_2.png");

				function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/grave.mp3", "sound/BGM/ordeal_battle.mp3",static_cast<int>(InMapScene::InMapState::GRAVE));

				//処理を受け付けないようにする
				if (player_select_window->GetAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

				event_npc_flag = false;

				break;
			}
			else {

				//実行不可の文字を表示する
				ordeal_coment_flag = true;

				break;
			}
		}
		//兵長
		else if (npc_list_->GetNpcName() == "兵長") {

			//ボスのID
			boss_id = 29;

			//ボスシーンでの背景をロードする
			boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/castle_back.png");

			//バトル処理を呼び出す
			function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/castle.mp3", "sound/BGM/corporal_battle.mp3", static_cast<int>(InMapScene::InMapState::CASTLE));

			//処理を受け付けないようにする
			if (player_select_window->GetAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

			event_npc_flag = false;
		}
		//船長
		else if (npc_list_->GetNpcName() == "船長") {

			//ボスのID
			boss_id = 30;

			//ボスシーンでの背景をロードする
			boss_background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/sea.jpg");

			//バトル処理を呼び出す
			function_npc->EventSelectProcces(npc_list_->GetNpcName(), boss_background_hdl, boss_id, "sound/BGM/ship.mp3", "sound/BGM/pirate.mp3", static_cast<int>(InMapScene::InMapState::SHIP));

			//処理を受け付けないようにする
			if (player_select_window->GetAcrionFlag()) { player_select_window->IsActiveChangeFlag(); }

			event_npc_flag = false;

		}
	}

}

//イベントを通知する
void EventManager::EventWindowNotification(const std::string& event_name , const std::string& event_coment)
{
	// 通知済みならリターン
	if (event_flag_map[event_name]) { return; }

	auto curent_inventry_state = GameManager::GetGameManager()->GetInventory()->GetSelectMenuNum();
		
	// 通知ウィンドウが立っていてかつ、インベントリが開かれていない時
	if (UIManager::GetUIManager()->GetIsNotificationVisible() &&
		curent_inventry_state == Inventory::MenuWindow_I::EMPTY) {

		//プレイヤーを取得する
		auto player = GameManager::GetGameManager()->GetPlayer();

		//プレイヤーを動けなくする
		if (player->GetPlayerControl()) {
			player->PlayerControlChangeFlag();
		}
		//プレイヤーが動けない状態でBackSpaceを押すと通知ウィンドウを閉じてプレイヤーを動けるようにする
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)&& !player->GetPlayerControl()) {
			UIManager::GetUIManager()->ToggleAcquisitionWindow();
			player->PlayerControlChangeFlag();
		}

		//通知ウィンドウを表示する
		UIManager::GetUIManager()->SetDisplayEventMessage(event_coment);
		if (UIManager::GetUIManager()->GetIsNotificationVisible()) {
			UIManager::GetUIManager()->ToggleAcquisitionWindow();
		}
	}
}

//イベント用のフラグを追加する
bool EventManager::AddEventFlagMap(const std::string& new_event_flag)
{
	//イベント名を探す
	auto it = event_flag_map.find(new_event_flag);

	//マップ内にすでに存在していれば
	if (it != event_flag_map.end()) {
		return event_flag_map[new_event_flag];
	}
	//なければ新しく追加して返す
	else {
		bool event_flag = false;
		event_flag_map.insert(std::make_pair(new_event_flag, event_flag));
	}
}

//コメントを表示する
void EventManager::ComentEvent(BaseScene* curent_scene, const int& coment_max, const std::list<Shared<Npc>>& npc_list)
{
	std::vector<std::string> npc_names = { "村長", "町長", "王", "王妃", "女性村人","男性村人","男性町民","男性住民","学者","僧侶"};

	//空じゃ無ければ
	if (!npc_list.empty()) {

		for (auto& npc_list_ : npc_list) {


			//処理カウント
			int coment_execution_count = 0;

			for (const auto& name : npc_names) {
				if (npc_list_->GetNpcName() == name) {
					// コメントを表示する
					function_npc->GetComentEvent(npc_list_->GetNpcName())(curent_scene, coment_max);
					coment_execution_count++;
					break;
				}
			}

			if (coment_execution_count > 0) {

				//NPCは一回のみコメントを出すので一回処理を行ったらここで抜ける
				break;
			}
		}
	}
}

bool EventManager::CheckEventItem(const int& essential_items , const int& remove_need_flag)
{
	// 選択したアイテムの ID と一致する要素を特定する
	auto item_find = std::find_if(GameManager::GetGameManager()->GetInventory()->GetInventoryList().begin(), GameManager::GetGameManager()->GetInventory()->GetInventoryList().end(),
		[&](const ItemBase& item) { return item.GetItemId() == essential_items; });

	// アイテムが見つかった場合はイベントを発生させる
	if (item_find != GameManager::GetGameManager()->GetInventory()->GetInventoryList().end()) {

		if (remove_need_flag) {
			GameManager::GetGameManager()->GetInventory()->GetInventoryList().erase(item_find);
			//アイテムの数を減らす
			GameManager::GetGameManager()->GetInventory()->DelateItemNum();
		}

		return true;
	}
	//見つからなかったらfalseを返す
	else {
		return false;
	}
	
}


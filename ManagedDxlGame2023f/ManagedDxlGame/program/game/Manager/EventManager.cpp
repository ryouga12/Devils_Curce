#include "EventManager.h"
#include"../Manager/UiManager.h"
#include"../Manager/SceneManager.h"
#include"../Scene/InMapScene.h"
#include"../Scene/battleScene.h"
#include"../Item/WeaponShop.h"
#include"../System/EventNpc.h"


EventManager* EventManager::getEventManager()
{
	static EventManager* p_instance = nullptr;

	if (!p_instance)p_instance = new EventManager();
	return p_instance;
}


EventManager::EventManager()
{
	
}

void EventManager::InitEventRelated(std::list<Shared<Actor>>& npc_list)
{
	//nullが入っていた場合
	if (!function_npc) {
		function_npc = std::make_shared<EventNpc>();
	}

	//アイコンの画像ハンドルを初期化する
	//他のマップでも同じ名前が存在する為、マップによってアイコンを切り替える
	for (auto& npc : npc_list) {

		auto itr = npc_list.begin();
		
		if (itr != npc_list.end()) {

			auto npc_ = std::dynamic_pointer_cast<Npc>(npc);

			if (npc_->GetNpcName() == "武器商人") {
				//メニューで使うアイコン
				Armsdealer_icon_hdl = npc_->GetNpcIconImage();
			}
			else if (npc_->GetNpcName() == "村長") {
				//メニューで使うアイコン
				herdman_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "神官") {
				//メニューで使うアイコン
				Priest_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "町長") {
				town_herdman_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "宿屋") {
				inn_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "魔王") {
				boss_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "魔物") {
				enemy_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "王") {
				king_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "王妃") {
				queen_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "女性村人") {
				female_resident_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
			else if (npc_->GetNpcName() == "男性村人") {
				male_villagers_icon_hdl = npc_->GetNpcIconImage();
				//処理を初期化する
				function_npc->InitEventFunction(npc_->GetNpcName());
			}
		}
	}

	//メニューの選択ウィンドウ
	menu_window = UIManager::getUIManager()->getMenu("menu_window");

	//最初の一回だけロードする
	if (!UIManager::getUIManager()->getMenu("select_answer_menu")) {

		//プレイヤーに選択させるウィンドウ
		player_select_coment = new MenuWindow::MenuElement_t[]{
			{100 , 100 , "はい" , 0},
			{100 , 150 , "いいえ", 1}
		};

		//プレイヤーに対する行動選択ウィンドウ
		player_select_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", player_select_coment, 2, 1);
		player_select_window->Open();

		//mapに格納する
		UIManager::getUIManager()->addMenu("select_answer_menu", player_select_window);
	}
}


EventManager::~EventManager()
{

}

//Npcのコメントの切り替え
void EventManager::NpcSwitchComent(const Shared<WeaponShop>& weapon_shop)
{
	//コメント用
	auto menuComentWindiow = menu_window.lock();


	//プレイヤーへの選択させる時のウィンドウ
	auto menuSelectWindow = menu_window.lock();

	//武器屋文字座標
	const tnl::Vector2i WEAPON_SHOP_STRING_POS = { 60 , 520 };

	switch (npcMove)
	{
	case EventManager::NpcMove::EMPTY:

		break;

	//村長
	case EventManager::NpcMove::HERDMAN:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, herdman_icon_hdl, true);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);
	
		break;

	//神官
	case EventManager::NpcMove::PRIEST:

		//メニューのウィンドウを表示する
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//アイコンを表示する
		DrawRotaGraph(110, 600, 1.5f, 0, Priest_icon_hdl, true);
		
		//セーブテキスト
		UIManager::getUIManager()->SaveText(tnl::Vector3{ 150,630,0 });

		if (player_select_window->getAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(50, 50, 250, 200);
		}

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);
		
	
		break;

	//武器屋
	case EventManager::NpcMove::WEAPONSHOP:

		if (auto menu_coment_window = menu_window.lock()) {
			menu_coment_window->Menu_draw(50, 500, 700, 200);
		}

		//武器屋関連の表示
		DrawString(WEAPON_SHOP_STRING_POS.x, WEAPON_SHOP_STRING_POS.y, "武器屋", -1);

		//コメントとアイコン
		DrawRotaGraph(90, 600, 1.5f, 0, Armsdealer_icon_hdl , true);

		//武器屋のコメントを表示する
		UIManager::getUIManager()->armsdealerComentDraw(1);

		//武器屋のコメント
		weapon_shop->Draw();

		//プレイヤーの所持金を描画
		DrawStringEx(135, 100, -1, "所持金: %d", GameManager::getGameManager()->getPlayer()->getPlayerMoney());

		break;

	//ボスモンスター
	case EventManager::NpcMove::BOSS:

		//メニューのウィンドウを表示する
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		//選択ウィンドウを表示する
		if (player_select_window->getAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(50, 50, 250, 200);
		}

		break;


	//宿屋
	case  EventManager::NpcMove::INN:

		//メニューのウィンドウを表示する
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//コメントとアイコン
		DrawRotaGraph(90, 600, 1.5f, 0, inn_icon_hdl, true);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		if (player_select_window->getAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(50, 50, 250, 200);
		}

		break;

	//町長
	case EventManager::NpcMove::TOWNHERDMAN:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, town_herdman_icon_hdl, true);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	//敵
	case EventManager::NpcMove::ENEMY:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, enemy_icon_hdl, true);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		//選択ウィンドウを表示する
		if (player_select_window->getAcrionFlag()) {
			//選択ウィンドウを表示する
			player_select_window->All(50, 50, 250, 200);
		}

		break;

	//王
	case  EventManager::NpcMove::KING:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, king_icon_hdl, true);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	//王妃
	case  EventManager::NpcMove::QUEEN:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, queen_icon_hdl, true);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	//女性村人
	case  EventManager::NpcMove::FEMALE_RESIDENT:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, female_resident_icon_hdl, true);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	//男性村人
	case  EventManager::NpcMove::RESIDENT_MALE:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//アイコンを表示する
		DrawRotaGraph(COMENT_ICON_POS.x, COMENT_ICON_POS.y, NPC_SCALE, 0, male_villagers_icon_hdl, true);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(COMENT_POS);

		break;

	default:

		break;
	}
}

//Npcなどのそれぞれのイベント時の更新処理
void EventManager::NpcEventUpdate(float delta_time, InMapScene* inmap_scene, int curent_map_state, std::list<Shared<Actor>>& npc_list)
{
	//表示するコメントの数
	const int HERDMAN_COMENT_MAX = 7;
	const int PRIEST_COMENT_MAX = 1;
	const int INN_COMENT_MAX = 1;
	const int TOWN_HERDMAN_COMENT_MAX = 2;
	const int ENEMY_COMENT_MAX = 3;
	const int KING_COMENT_MAX = 4;
	const int QUEEN_COMENT_MAX = 1;
	const int FEMALE_RESIDENT_COMENT_MAX = 1;
	const int MALE_VILLAGERS_COMENT_MAX = 1;


	//シーンマネージャーのポインタ
	auto mgr = SceneManager::GetInstance();

	switch (npcMove)
	{
	case EventManager::NpcMove::EMPTY:


		break;

	//村長
	case EventManager::NpcMove::HERDMAN:

		//コメントを表示する
		ComentEvent(inmap_scene, HERDMAN_COMENT_MAX, npc_list);

		break;


	//神官
	case EventManager::NpcMove::PRIEST:

		//コメントを表示する
		if (ComentEventChange(PRIEST_COMENT_MAX) && event_npc_flag) {

			//選択処理
			//セーブ機能
			PlayerAnswerSelect(curent_map_state, inmap_scene, npc_list);

			//フラグがfalseの場合
			if (!player_select_window->getAcrionFlag() && event_npc_flag) {
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
		if (ComentEventChange(BOSS_COMENT_MAX_NUM)) {

			//選択処理 & 敵との会話処理
			PlayerAnswerSelect(curent_map_state, inmap_scene, npc_list);

			//フラグがfalseの場合
			if (!player_select_window->getAcrionFlag() && event_npc_flag) {
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
		if (ComentEventChange(INN_COMENT_MAX)) {

			//選択処理 & 宿屋処理
			//プレイヤーのHpとMpの回復処理
			PlayerAnswerSelect(curent_map_state, inmap_scene, npc_list);

			//フラグがfalseの場合
			if (!player_select_window->getAcrionFlag()&& event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//町長
	case EventManager::NpcMove::TOWNHERDMAN:

		//コメントを表示する
		ComentEvent(inmap_scene, TOWN_HERDMAN_COMENT_MAX, npc_list);

		break;

	//敵
	case EventManager::NpcMove::ENEMY:

		//コメントを表示する
		if (ComentEventChange(ENEMY_COMENT_MAX)){

			//選択処理 & 敵との会話処理
			PlayerAnswerSelect(curent_map_state, inmap_scene, npc_list);

			//フラグがfalseの場合
			if (!player_select_window->getAcrionFlag() && event_npc_flag) {
				//ウィンドウの処理を受け付けるようにする
				player_select_window->IsActiveChangeFlag();
			}
		}

		break;

	//王
	case EventManager::NpcMove::KING:

		//コメントを表示する
		ComentEvent(inmap_scene, KING_COMENT_MAX, npc_list);

		break;

	//王妃
	case EventManager::NpcMove::QUEEN:

		//コメントを表示する
		ComentEvent(inmap_scene, QUEEN_COMENT_MAX, npc_list);

		break;

	//女性村人
	case EventManager::NpcMove::FEMALE_RESIDENT:

		//コメントを表示する
		ComentEvent(inmap_scene, FEMALE_RESIDENT_COMENT_MAX, npc_list);

		break;

	//女性村人
	case EventManager::NpcMove::RESIDENT_MALE:

		//コメントを表示する
		ComentEvent(inmap_scene, MALE_VILLAGERS_COMENT_MAX, npc_list);

		break;


	default:
		break;
	}
}

//コメントを流し切ってからEnterキーでシーンを遷移させる
bool EventManager::ComentEventChange(const int count_max)
{
	//コメントをコマ送りで表示する
	UIManager::getUIManager()->ComentNextByInput(count_max);

	//指定のコメント以上になったらtrueを返す
	if (UIManager::getUIManager()->GetCount() >= count_max) {
		return true;
	}

	return false;
}

//プレイヤーに行動を選択させる
void EventManager::PlayerAnswerSelect(int curent_map_state , InMapScene* inmap_scene , const std::list<Shared<Actor>>& npc_list)
{

	for (auto& npc_list_ : npc_list) {

		auto itr = npc_list.begin();

		if (itr != npc_list.end()) {

			auto npc_ = std::dynamic_pointer_cast<Npc>(npc_list_);

			//プレイヤーがはいを選択したら
			if (player_select_window->getSelectNum() == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
				&& player_select_window->getAcrionFlag())
			{
				//神官に話かけた場合
				if (npc_->GetNpcName() == ("神官")) {

					function_npc->EventSelectProcces(npc_->GetNpcName(), curent_map_state);

					//処理を受け付けないようにする
					player_select_window->IsActiveChangeFlag();

					event_npc_flag = false;

				}
				//宿屋に話かけた場合
				else if (npc_->GetNpcName() == "宿屋" && npcMove == NpcMove::INN) {

					function_npc->EventSelectProcces(npc_->GetNpcName(), INNMONEY);

					//処理を受け付けないようにする
					player_select_window->IsActiveChangeFlag();

					event_npc_flag = false;

					//プレイヤーを動けるようにする
					inmap_scene->SequenceChange(InMapScene::Sequence_num::MOVE);
				}
				//敵に話かけた場合
				else if (npc_->GetNpcName() == "魔物" && npcMove == NpcMove::ENEMY) {

					//ボスシーンでの背景をロードする
					boss_background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg013b.jpg");

					function_npc->EventSelectProcces(npc_->GetNpcName(), boss_background_hdl , MEDIUM_BOSS, "sound/BGM/town_bgm.mp3", "sound/BGM/maou_sentou_bgm.mp3");

					//処理を受け付けないようにする
					player_select_window->IsActiveChangeFlag();

					event_npc_flag = false;
				}
				else if (npc_->GetNpcName() == "魔王") {

					//ボスシーンでの背景をロードする
					boss_background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg010b.jpg");

					function_npc->EventSelectProcces(npc_->GetNpcName(), boss_background_hdl, BOSS_ID, "sound/BGM/maou_bgm_castle.mp3", "sound/BGM/maou_sentou_bgm.mp3");

					//処理を受け付けないようにする
					player_select_window->IsActiveChangeFlag();

					event_npc_flag = false;

				}

			}
			//プレイヤーがいいえを選択したら
			else if (player_select_window->getSelectNum() == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
				&& player_select_window->getAcrionFlag())
			{
				inmap_scene->SequenceChange(InMapScene::Sequence_num::MOVE);
				player_select_window->IsActiveChangeFlag();
				event_npc_flag = false;
			}
		}
		itr++;
	}
}

//コメントを表示する
void EventManager::ComentEvent(InMapScene* inmap_scene, const int coment_max ,const std::list<Shared<Actor>>& npc_list )
{
	std::vector<std::string> npc_names = { "村長", "町長", "王", "王妃", "女性村人","男性村人"};

	for (auto& npc_list_ : npc_list) {

		auto npc_ = std::dynamic_pointer_cast<Npc>(npc_list_);

		if (npc_) {

			int coment_execution_count = 0;

			for (const auto& name : npc_names) {
				if (npc_->GetNpcName() == name) {
					// コメントを表示する
					function_npc->GetComentEvent(npc_->GetNpcName())(inmap_scene, coment_max);
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

//Npcのメニュー選択画面
void EventManager::EventMenuWindow()
{
	//コメント用
	auto menuComentWindiow = menu_window.lock();

	//プレイヤーへの選択させる時のウィンドウ
	auto menuSelectWindow = menu_window.lock();

	switch (select_menu)
	{

		//武器屋の際の描画
	case EventManager::MenuOpen::WEAPONSHOP:

		//ウィンドウの表示
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//メニューのバックグラウンドを描画する
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//武器商人のアイコンを表示する
		DrawRotaGraph(90, 600, 1.5f, 0, Armsdealer_icon_hdl, true);

		//武器商人のコメントを表示する
		UIManager::getUIManager()->armsdealerComentDraw(ARMSDEALER_SPEAK_COMENT);

		break;


	case EventManager::MenuOpen::BOSS:

		//ウィンドウの表示
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//話すコマンド
		GameManager::getGameManager()->displayDialogue();


		break;

	default:

		//ウィンドウの表示
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//話すコマンド
		GameManager::getGameManager()->displayDialogue();

		break;
	}


}

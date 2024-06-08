#include "EventManager.h"
#include"../Manager/UiManager.h"


EventManager* EventManager::getEventManager()
{
	static EventManager* p_instance = nullptr;

	if (!p_instance)p_instance = new EventManager();
	return p_instance;
}

EventManager::EventManager()
{
	InitMenu();

}

void EventManager::InitMenu()
{
	//メニューで使うアイコン
	Armsdealer_icon_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/Npc/Armsdealer_icon.png");
	herdman_icon_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/Npc/herdman_icon.png");
	Priest_icon_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/Npc/Priest_icon.png");
	Player_icon_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/Player/Player_Icon.png");


	//メニューの選択ウィンドウ
	menu_window = UIManager::getUIManager()->getMenu("menu_window");

}

EventManager::~EventManager()
{
}

//Npcのコメントの切り替え
void EventManager::NpcSwitchComent()
{
	//コメント用
	auto menuComentWindiow = menu_window.lock();

	//プレイヤーへの選択させる時のウィンドウ
	auto menuSelectWindow = menu_window.lock();

	switch (npcMove)
	{
	case EventManager::NpcMove::EMPTY:

		break;

	//村長
	case EventManager::NpcMove::HERDMAN:

		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//アイコンを表示する
		DrawRotaGraph(HERDMAN_ICON_POS.x, HERDMAN_ICON_POS.y, HERDMAN_SCALE, 0, herdman_icon_hdl, true);

		//コメントを表示する
		UIManager::getUIManager()->ComentDraw(HERDMAN_COMENT_POS, 7);

	
		break;

	//神官
	case EventManager::NpcMove::PRIEST:

		//メニューのウィンドウを表示する
		menuComentWindiow->Menu_draw(50, 500, 700, 200);
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//アイコンを表示する
		DrawRotaGraph(110, 600, 1.5f, 0, Priest_icon_hdl, true);

		//コメントを表示する
		/*NpcComent(priest_coment_pos.x, priest_coment_pos.y, priest_coment, 2);*/

		break;

	//ボスモンスター
	case EventManager::NpcMove::BOSS:

		//メニューのウィンドウを表示する
		menuComentWindiow->Menu_draw(50, 500, 700, 200);

		//フォントの大きさを変える
		SetFontSize(20);

		//コメントを表示する
		if (ComentBattleChange(BOSS_COMENT_POS, BOSS_COMENT_MAX_NUM) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//BGMを止める
			SoundManager::getSoundManager()->StopSound("sound/BGM/maou_bgm_castle.mp3");
			//バトル用のBGMを流す
			SoundManager::getSoundManager()->sound_Play("sound/BGM/maou_sentou_bgm.mp3", DX_PLAYTYPE_LOOP);

			auto mgr = SceneManager::GetInstance();
			//シーンを遷移させる(プレイヤーの座標を渡す,敵のID,敵のポインタを渡す)
			mgr->changeScene(new BattleScene(GameManager::getGameManager()->getPlayer()->getPlayerPos(), boss_background_hdl, std::make_shared<BossMonster>()));
		}

		//フォントの大きさを戻す
		SetFontSize(16);

		break;

	default:

		break;
	}
}

//コメントを流し切ってからEnterキーでシーンを遷移させる
bool EventManager::ComentBattleChange(const tnl::Vector2i& coment_pos , const int count_max)
{
	//コメントを表示する
	UIManager::getUIManager()->ComentDraw(coment_pos, count_max);

	if (UIManager::getUIManager()->GetCount() >= count_max) {
		return true;
	}

	return false;
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

		//ボスシーンでの背景をロードする
		boss_background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg010b.jpg");

		break;

	default:

		//ウィンドウの表示
		menuSelectWindow->Menu_draw(50, 50, 250, 200);

		//話すコマンド
		GameManager::getGameManager()->displayDialogue();

		break;
	}


}

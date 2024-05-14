#include "EventManager.h"

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
	menuSelectWindow = std::make_shared<Menu>(50, 50, 250, 200, "graphics/WindowBase_01.png");

	//コメント表示の際のメニューウィンドウ
	menuComentWindiow = std::make_shared<Menu>(50, 500, 700, 200, "graphics/WindowBase_01.png");
}

EventManager::~EventManager()
{
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Npc/Armsdealer_icon.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Npc/herdman_icon.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Npc/Priest_icon.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Player/Player_Icon.png");
}

bool EventManager::isNpcMenuOpen()
{
	switch (select_menu_num)
	{
	case EventManager::MenuOpenNum::WeaponShop:
		
		return select_menu_num == MenuOpenNum::WeaponShop;

		break;

	case EventManager::MenuOpenNum::Herdman:

		return select_menu_num == MenuOpenNum::Herdman;

		break;

	case EventManager::MenuOpenNum::Priest:

		return select_menu_num == MenuOpenNum::Priest;

		break;
	case EventManager::MenuOpenNum::Object:

		return select_menu_num == MenuOpenNum::Object;

		break;

	default:
		break;
	}
	
	

}

//メニューウィンドウの切り替え
EventManager::MenuOpenNum EventManager::SetMenuNpc(int npcType)
{
	switch (npcType)
	{
	case 0:
		select_menu_num = MenuOpenNum::Herdman;
		return select_menu_num;
		break;

	case 1:
		select_menu_num = MenuOpenNum::Priest;
		return select_menu_num;
		break;

	case 2:
		select_menu_num = MenuOpenNum::WeaponShop;
		return select_menu_num;
		break;

	case 3:
		select_menu_num = MenuOpenNum::Object;
		return select_menu_num;
		break;


	default:

		return MenuOpenNum();

		break;
	}
	
}

//Npcのコメントの切り替え
void EventManager::NpcSwitchComent()
{
	switch (npcMove)
	{
	case EventManager::NpcMove::Empty:

		break;

	case EventManager::NpcMove::Herdman:

		menuComentWindiow->Menu_draw();

		//アイコンを表示する
		DrawRotaGraph(herdman_icon_pos.x, herdman_icon_pos.y, herdman_scale, 0, herdman_icon_hdl, true);

		if (herdman_first_coment) {

			////一度コメントを消して
			//Npc_coment_Array.clear();

			//コメントを表示する
			NpcComent(herdman_coment_pos.x, herdman_coment_pos.y, 3, herdman_coment);

			//次のコメントを表示できるようにする
			herdman_first_coment = false;
			herdman_second_coment = true;

		}
		//Enterキーを押したらコメントを切り替える
		if (herdman_second_coment) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				Npc_coment_Array.clear();
				NpcComent(herdman_coment_pos.x, herdman_coment_pos.y, 2, herdman_coment_2);
				herdman_first_coment = true;
				herdman_second_coment = false;
			}
		}
	
		break;

	case EventManager::NpcMove::Priest:

		//メニューのウィンドウを表示する
		menuComentWindiow->Menu_draw();
		menuSelectWindow->Menu_draw();

		//アイコンを表示する
		DrawRotaGraph(110, 600, 1.5f, 0, Priest_icon_hdl, true);

		//コメントを表示する
		/*NpcComent(priest_coment_pos.x, priest_coment_pos.y, priest_coment, 2);*/

		break;

	default:

		break;
	}
}

//Npcのコメントをセットする
EventManager::NpcMove EventManager::SetNpcComent(int npcType)
{
	switch (npcType)
	{
	case 0:
		npcMove = NpcMove::Empty;
		return npcMove;
		break;

	case 1:
		npcMove = NpcMove::Herdman;
		return npcMove;
		break;

	case 2:
		npcMove = NpcMove::Priest;
		return npcMove;
		break;

	default:
		return NpcMove();
		break;
	}
	
}

//明日はここから : 文字列を一行かそれとも複数行かで判別し、描画する
//コメントをアニメーションで表示する
void EventManager::NpcComent(int x, int y, int max_coment_num ,std::string npc_coment[])
{
	int LINE_HEIGHT = 50;
	int index = 0;

	for (int i = 0; i < max_coment_num; i++) {
		ComentLoad(npc_coment[i]);
	}

	if (!Npc_coment_Array.empty()) {
		// コメントの数だけコメントを表示する
		for (int i = 0; index < max_coment_num; i++, index++) {
			DrawStringEx(x, y + i * LINE_HEIGHT, -1, "%s", Npc_coment_Array[i].c_str());
		}
	}

}

//コメントを読み込む
void EventManager::ComentLoad(const std::string& newComent)
{
	Npc_coment_Array.push_back(newComent);
}


void EventManager::NpcComentDraw()
{
	switch (select_menu_num)
	{
		//武器屋の際の描画
	case EventManager::MenuOpenNum::WeaponShop:

		//ウィンドウの表示
		menuSelectWindow->Menu_draw();

		//メニューのバックグラウンドを描画する
		menuComentWindiow->Menu_draw();

		//武器商人のアイコンを表示する
		DrawRotaGraph(90, 600, 1.5f, 0, Armsdealer_icon_hdl, true);

		//武器商人のコメントを表示する
		armsdealerComentDraw(armsdealer_speak_coment);

		/*DrawStringEx(menuSelectWindow->menu_x + 50, menuSelectWindow->menu_y + 250, -1, "seqMenuOpen");*/

		break;

		//村長の時の描画
	case EventManager::MenuOpenNum::Herdman:

		//ウィンドウの表示
		menuSelectWindow->Menu_draw();

		//話すコマンド
		GameManager::getGameManager()->displayDialogue();

		break;

		//神官の時の描画
	case EventManager::MenuOpenNum::Priest:

		//ウィンドウの表示
		menuSelectWindow->Menu_draw();

		//話すコマンド
		GameManager::getGameManager()->displayDialogue();

		break;

		//オブジェクトの際の描画
	case EventManager::MenuOpenNum::Object:

		//ウィンドウを表示する
		menuComentWindiow->Menu_draw();

		//プレイヤーのアイコンを表示する
		DrawRotaGraph(100, 590, 1.5f, 0, Player_icon_hdl, true);

		//オブジェクトのコメントを表示する
		objectComentDraw();

		break;

	default:
		break;
	}


}

void EventManager::armsdealerComentDraw(int type)
{
	//武器屋に最初に話かけたときのコメント
	if (type == armsdealer_first_comet) {
		DrawString(weaponShop.x, weaponShop.y, "武器屋", Color_White);
		DrawStringEx(armsdealerComent.x, armsdealerComent.y, Color_White, "「 いらっしゃい！」");
		DrawStringEx(WeaponShop_buy.x, WeaponShop_buy.y, Color_White, "(1 : 武器を購入する)");
		DrawStringEx(menu_close.x, menu_close.y, Color_White, "(0 : メニューを閉じる)");
	}

	//なにか買うときのコメント
	else {
		DrawStringEx(armsdealerComent.x, armsdealerComent.y, Color_White, "「 何を買うんだい 」");
	}
}

void EventManager::objectComentDraw()
{
	//メニューのバックグラウンドを描画する
	DrawStringEx(object_coment.x, object_coment.y, Color_White, "「 今は使われていない檻みたいだ\n   錆びている                 」");
	DrawStringEx(object_menu_close.x, object_menu_close.y, Color_White, "(0 : メニューを閉じる)");

}

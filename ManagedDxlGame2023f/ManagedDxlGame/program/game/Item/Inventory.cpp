#include "Inventory.h"
#include"../Manager/GameManager.h"
#include"../Manager/UiManager.h"

Inventory::Inventory()
{
	item = std::make_shared<Item>();

	//メニューの初期化
	InitMenuWinodow();
}

Inventory::~Inventory()
{
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/cousour_bgm.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/kaihuku.mp3");

}

//更新処理
void Inventory::Update(float delta_time)
{
	//インベントリの更新
	swichInventoryUpdate(delta_time);

}

//描画処理
void Inventory::draw()
{
	//アイテムを表示する為のバックウィンドウ
	auto item_coment_window = menu_window.lock();

	if (select_menu == MenuWindow_I::EMPTY) {
		Game_Menu();
	}
	//選択されているのが最初のメニューだったら
	else if (select_menu == MenuWindow_I::FIRSTMENU) {
		First_Menu();
	}
	//選択されたのがアイテムメニューだったら
	else if (select_menu == MenuWindow_I::ITEMMENU) {

		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x , MENU_WINDOW_POS.y , MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(DRAWPOS, CURENTPAGETEXT, CURSORX,ITEMPERPAGE);
	}
	//選択されたのがプレイヤーの強さメニューだったら
	else if (select_menu == MenuWindow_I::STATUSMENU) {
		PlyStatusMenu();
	}
	//選択されたのがアイテムの詳細メニューだったら
	else if (select_menu == MenuWindow_I::ITEMUSEMENU) {
		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(DRAWPOS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		ItemUseMenu();
		if (!can_used_item) {
			if (item_coment_window) {
				item_coment_window->Menu_draw(600, 50, 300, 200);
				DrawStringEx(ITEMCOMENTPOS.x, ITEMCOMENTPOS.y, COLORWHITE, "このアイテムは使用できません");
			}
		}
	}
	else if (select_menu == MenuWindow_I::ITEMDETAILMENU) {
		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(DRAWPOS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		ItemUseMenu();
		ItemDetail();
	}
	else if (select_menu == MenuWindow_I::SKILLMENU) {
		// ウィンドウの表示
		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		InventorySkill(DRAWPOS , CURENTPAGETEXT , CURSORX ,ITEMPERPAGE);

	}
	else if (select_menu == MenuWindow_I::SKILLDATAILMENU) {
		// ウィンドウの表示
		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		InventorySkill(DRAWPOS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		SkillDetailDraw(skill_selected_index);
	}
	
}

//アイテムを追加する
void Inventory::AddInventory(int id)
{
	if (inventory_list.size() == inventory_max_size_) return;
	ItemBase newItem = item->GetItemById(id);
	inventory_list.emplace_back(newItem);
	item_num++;
}

//武器を装備する
void Inventory::EquipWeapon(int weaponIndex)
{
	//プレイヤーを取得する
	auto& player = GameManager::getGameManager()->getPlayer();

	// アイテムがない場合やインデックスが不正な場合は何もせずに関数を終了
	if (item_num == 0 || weaponIndex < 0 || weaponIndex >= item_num) {
		return;
	}

	// 選択されたアイテムを取得
	ItemBase selectedItem = inventory_list[weaponIndex];

	// 同じアイテムが既に装備されているかチェック
	bool isAlreadyEquipped = false;
	for (const auto& equipped : equipped_weapon) {
		if (equipped.getItemId() == selectedItem.getItemId()) {
			// 同じアイテムが装備されている場合、何もせずに関数を終了
			isAlreadyEquipped = true;
			break;
		}
	}

	for (const auto& equipped : equipped_armor) {
		if (equipped.getItemId() == selectedItem.getItemId()) {
			// 同じアイテムが装備されている場合、何もせずに関数を終了
			isAlreadyEquipped = true;
			break;
		}
	}


	// 同じアイテムが既に装備されている場合は何もせずに関数を終了
	if (isAlreadyEquipped) {
		return;
	}

	// 新しいアイテムのタイプに応じて装備を切り替える
	if (selectedItem.getItemType() == WEAPON) {
		auto curentAttack = player->getPlayerStatusSave().GetAttack();
		// 装備中の武器を外す
		if (!equipped_weapon.empty()) {
			// 装備中の武器の効果を反映から削除する
			ItemBase equippedWeaponItem = equipped_weapon.back();
			//装備中の武器のステータスを削除する
			curentAttack -= equippedWeaponItem.getItemDamage();
			// 装備中の武器を削除する
			equipped_weapon.pop_back();
		}
		
		// 新しい武器を装備
		equipped_weapon.push_back(selectedItem);
		//装備した際にプレイヤーの画像を変える
		player->SetPlayerAnimationHdl(selectedItem.getItemWeapontype());
		// プレイヤーのステータスに新しい武器の効果を反映
		equip_weapon = true;
		equip_attack = selectedItem.getItemDamage();
		player->getPlayerStatusSave().SetPlayerAttack(curentAttack + equip_attack);
	}
	else if (selectedItem.getItemType() == ARMOR) {
		auto curentDefance = player->getPlayerStatusSave().GetDefance();
		// 装備中の防具を外す
		if (!equipped_armor.empty()) {
			// 装備中の防具の効果を反映から削除する
			ItemBase equippedArmorItem = equipped_armor.back();
			//装備中の防具のステータスを削除する
			curentDefance -= equippedArmorItem.getItemDefance();
			// 装備中の防具を削除する
			equipped_armor.pop_back();
		}
		// 新しい防具を装備
		equipped_armor.push_back(selectedItem);
		// プレイヤーのステータスに新しい防具の効果を反映
		equip_armor = true;
		equip_defance = selectedItem.getItemDefance();
		player->getPlayerStatusSave().SetPlayerDefance(curentDefance + equip_defance);
	}
}

//アイテムの詳細を開いた時の処理
void Inventory::ItemDetail()
{
	//アイテムの説明を出すためのバックウィンドウ
	auto item_detail_window = menu_window.lock();


	item_detail_window->Menu_draw(ITEM_DETAIL_WINDOW_POS.x, ITEM_DETAIL_WINDOW_POS.y, ITEM_DETAIL_WINDOW_WIDTH, ITEM_DETAIL_WINDOW_HEIGHT);

	DrawStringEx(630, 70, -1, "===アイテム説明===");

	//アイテム説明を取得する
	std::string ItemDetail = inventory_list[selected_index].getItemDetail();

	//インベントリが空じゃない場合
	if (!inventory_list.empty()) {
	//アイテムの詳細を表示する
		DrawStringEx(ITEM_DETAIL_POS.x, ITEM_DETAIL_POS.y, COLORWHITE, "%s" ,ItemDetail.c_str());
	}

}

//カーソル移動時のインデックス操作と取得
void Inventory::ItemCurourIndex(int ItemPerPage)
{
	//---選択したアイテムにをIDとして取得する---//

	// 上キーが押されたときの処理
	//一番上にカーソルがいた場合それ以上にいかないようにする
	if (curent_page == 0 && selected_index == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// 何もしない
		return;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selected_index % ItemPerPage == 0) {
			// カーソルがページ内の最上部にいる場合は、一つ上のページの最後の要素を選択
			if (curent_page > 0) {
				--curent_page;
				// インデックスを更新してページの最後の要素を選択する
				selected_index = (curent_page + 1) * ItemPerPage - 1;
			}
		}
		else {
			// それ以外の場合は、一つ上の要素を選択
			--selected_index;
		}
	}

	// 下キーが押されたときの処理
	if (selected_index == inventory_list.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN))return;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selected_index + 1) % ItemPerPage == 0) {
			// カーソルがページ内の最下部にいる場合は、一つ下のページの最初の要素を選択
			//今の所プレイヤーが持てるインベントリは20で考えている為4ページ分まで開けるようにする
			if (curent_page < ITEM_MAX_PAGE) {
				++curent_page;
				// インデックスを更新してページの最初の要素を選択する
				selected_index = curent_page * ItemPerPage;
			}
		}
		else {
			// それ以外の場合は、一つ下の要素を選択
			++selected_index;
		}
	}

	// selectedIndexが有効な範囲内にあるか確認して、選択されたアイテムのIDを取得する
	if (selected_index >= 0 && selected_index < inventory_list.size()) {
		ItemBase selectedItem = inventory_list[selected_index];
		selected_item_id = selectedItem.getItemId();
	}
}

void Inventory::CusorMove()
{
	//アイテムがない場合、何もしない
	if (inventory_list.empty())
	{
		return;
	}
	//カーソルが一番上にいる場合カーソルをさらに上に押してもカーソルを動かさないようにする
	if (select_cursor == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) 
	{
		return;
	}
	//カーソルの動き
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		item_num = static_cast<int>(inventory_list.size());
		select_cursor = (select_cursor + (item_num - 1)) % item_num;

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		item_num = static_cast<int>(inventory_list.size());
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// カーソルが最後の要素よりも下に移動しようとする場合は、何もせずに処理を終了する
		if (select_cursor == item_num - 1) {
			return;
		}
		select_cursor = (select_cursor + 1) % item_num;
	}
}

//カーソルをリセットする
void Inventory::CursorReset()
{
	select_cursor = 0;
}

//メニューの初期化
void Inventory::InitMenuWinodow()
{
	//最初のESCを押したときに出る最初のメニュー
	MenuWindow::MenuElement_t* first_menu_coment = new MenuWindow::MenuElement_t[]
	{
		{MENU_TOOL_POS.x, MENU_TOOL_POS.y , "道具", 0},
		{MENU_SKILL_POS.x, MENU_SKILL_POS.y , "特技", 1},
		{MENU_STATUS_POS.x, MENU_STATUS_POS.y , "強さ", 2},
		{MENU_CLOSE_POS.x, MENU_CLOSE_POS.y , "閉じる" ,3}
	};

	first_menu = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", first_menu_coment, 4 , 1);
	first_menu->Open();

	//mapに追加する
	UIManager::getUIManager()->addMenu("select_answer_window", select_action_menu);

	//アイテムウィンドウ内で選択するメニュー
	MenuWindow::MenuElement_t* select_detail = new MenuWindow::MenuElement_t[]
	{
		{MENU_ITEM_USE_POS.x , MENU_ITEM_USE_POS.y , "使う" , 0},
		{MENU_ITEM_DETAIL_POS.x , MENU_ITEM_DETAIL_POS.y , "詳細" , 1},
		{MENU_ITEM_EQUIP_POS.x , MENU_ITEM_EQUIP_POS.y , "装備する",2},
		{MENU_ITEM_DISPOSE_POS.x , MENU_ITEM_DISPOSE_POS.y , "捨てる", 3},
		{MENU_ITEM_CLOSE_POS.x , MENU_ITEM_CLOSE_POS.y , "閉じる" , 4}
	};

	select_detail_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_detail, 5, 1);
	select_detail_window->Open();


	//メニューウィンドウ
	menu_window = UIManager::getUIManager()->getMenu("menu_window");
}

//プレイヤーが動いている時のメニュー表示
void Inventory::Game_Menu()
{
	//プレイヤーを取得する
	auto& player = GameManager::getGameManager()->getPlayer();

	//ゴールドを表示する為のバックウィンドウ
	auto gold_display_window = menu_window.lock();

	if (gold_display_window) {
		gold_display_window->Menu_draw(GOLD_DISPLAY_WINDOW_POS.x, GOLD_DISPLAY_WINDOW_POS.y, GOLD_DISPLAY_WINDOW_WIDTH, GOLD_DISPLAY_WINDOW_HEIGHT);
	}

	DrawStringEx(1030, 65, -1, "G : %d", player->getPlayerMoney());

	//プレイヤーのHpなどを表示するウィンドウ
	//auto player_status_window_ = menu_window.lock();

	//if (player_status_window_) {
	//	player_status_window_->Menu_draw(50, 50, 250, 240);
	//}

	//UIManager::getUIManager()->PlayerStatusDrawWindow();

	auto esc_key_string_window = menu_window.lock();

	if (esc_key_string_window) {
		esc_key_string_window->Menu_draw(50, 50, 250, 50);
	}

	DrawString(60, 65,"ESCキーでインベントリを表示" ,-1);
}

//一番最初のメニュー
void Inventory::First_Menu()
{
	first_menu->All(50, 50, 250, 250);
}

//アイテムの表示
void Inventory::ItemMenu(const tnl::Vector2i& itemDrawPos, const tnl::Vector2i& curentPageText, int CousourX , int itemParPage)
{
	int i = 0;
	const int Y = 34;
	const int EQUIP_Y_POS = 30;
	const int EQUIP_X_POS = 30;


		//現在のページに基づいて表示するアイテムの範囲を計算
		start_index = curent_page * itemParPage;
		end_index = min((start_index + itemParPage) , static_cast<int>(inventory_list.size()));

		for (int index = start_index; index < end_index; ++index) {
			//
			auto& InventoryData = inventory_list[index];
			//アイテムの名前を取得する
			std::string ItemName = InventoryData.getItemName();

			// アイテムの描画
			DrawStringEx(itemDrawPos.x, itemDrawPos.y + Y * i, COLORWHITE, "%s", ItemName.c_str());

			// カーソルを描画する位置を決定
			int cursorY = itemDrawPos.y + Y * (select_cursor % itemParPage);
			ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CousourX, cursorY, CURSORSIZE, 0, true);
		
			//装備できるアイテムだった場合
			if (InventoryData.getItemType() == 1 || InventoryData.getItemType() == 2) {
				// アイテムが装備されているか、選択されたアイテムかを確認
				if ((equip_weapon && InventoryData.getItemId() == equipped_weapon.back().getItemId())) {
					// 装備中のアイテムまたは選択されたアイテムの場合、"[E]" を表示
					//アイテムの名前から左に30ずれた座標に表示
					DrawStringEx(itemDrawPos.x - EQUIP_X_POS, itemDrawPos.y + EQUIP_Y_POS * i, COLORWHITE, "[E]");
				}
				if ((equip_armor && InventoryData.getItemId() == equipped_armor.back().getItemId())) {
					// 装備中のアイテムまたは選択されたアイテムの場合、"[E]" を表示
					//アイテムの名前から左に30ずれた座標に表示
					DrawStringEx(itemDrawPos.x - EQUIP_X_POS, itemDrawPos.y + EQUIP_Y_POS * i, COLORWHITE, "[E]");
				}
				++i;
			}
			//使用アイテムだった場合
			else
			{
				++i;
			}
			
		}
		//ページの表示
		DrawStringEx(curentPageText.x, curentPageText.y, COLORWHITE , "%d / 4", curent_page + 1);

}

//プレイヤーのステータス表示
void Inventory::PlyStatusMenu()
{

	//プレイヤーを取得する
	auto& player = GameManager::getGameManager()->getPlayer();

	//プレイヤーのステータスを表示する為のバックウィンドウ
	auto plyer_status_window = menu_window.lock();

	if (plyer_status_window) {
		plyer_status_window->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, PLAYER_STATUS_WINDOW_WIDTH, PLAYER_STATUS_WINDOW_HEIGHT);
	}

	//強さ文字の座標
	const tnl::Vector2i PLAYER_STATUS_STRING = { 150 , 70 };
	
	//強さの文字を表示する
	DrawStringEx(PLAYER_STATUS_STRING.x, PLAYER_STATUS_STRING.y, -1, "===強さ===");

	//追加していく座標
	const int Y = 50;

	// Player クラスのポインターが有効かどうかを確認する
	if (player) {
		// Player クラスのポインターを介してステータスを取得する
		int hp = static_cast<int>(player->getPlayerStatusSave().GetcurentHp());
		int mp = static_cast<int>(player->getPlayerStatusSave().GetCurentMp());
		int Attack = player->getPlayerStatusSave().GetAttack();
		int Defense = player->getPlayerStatusSave().GetDefance();
		int Level = player->getPlayerStatusSave().GetLevel();
		int Speed = player->getPlayerStatusSave().GetSpeed();
		int R_expoint = player->getPlayerStatusSave().GetExpoint();

		// 取得したステータスを描画する
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y, COLORWHITE, "HP : %d", hp);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + Y, COLORWHITE, "MP : %d", mp);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 2), COLORWHITE, "攻撃力 : %d", Attack);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 3), COLORWHITE, "防御力 : %d", Defense);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 4), COLORWHITE, "レベル : %d", Level);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 5), COLORWHITE, "すばやさ : %d", Speed);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 6), COLORWHITE, "レベルアップに必要な経験値 : %d", R_expoint);
	}

}

void Inventory::ItemUseMenu()
{
	select_detail_window->All(350, 50, 250, 280);

}

//インベントリの処理
void Inventory::swichInventoryUpdate(float deltatime)
{
	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	switch (select_menu)
	{
	case Inventory::MenuWindow_I::EMPTY:

		break;

		//最初のメニューでの処理
	case Inventory::MenuWindow_I::FIRSTMENU:
	
		//1個目が選択された場合、アイテムを表示する
		if (first_menu->getSelectNum() == TOOL && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ITEMMENU;
			CursorReset();
			curent_page = 0;
			selected_index = 0;
		}
			//2個目が選択された場合、特技を表示する
		else if (first_menu->getSelectNum() == SKILL && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::SKILLMENU;

		}
		//3個目が選択された場合、プレイヤーの強さを表示する
		else if (first_menu->getSelectNum() == STATUS && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::STATUSMENU;
		}
		//4個目が選択された場合、閉じる
		else if (first_menu->getSelectNum() == CLOSE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::EMPTY;
			//plyerを動けないようにする
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
		
			}

		break;

		//アイテムメニューでの処理
	case Inventory::MenuWindow_I::ITEMMENU:

		//Enterキーを押したらItemUseMenuに変更する
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ITEMUSEMENU;
		}
		//左キーを押すとアイテム使用メニューに戻す
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		//インデックス操作
		ItemCurourIndex(ITEMPERPAGE);
		

		CusorMove();

		break;

	//アイテム詳細メニューの場合
	case Inventory::MenuWindow_I::ITEMDETAILMENU:

		//左キーを押すとアイテム使用メニューに戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::ITEMUSEMENU;
		}

		break;

		//アイテム使用メニュー
	case Inventory::MenuWindow_I::ITEMUSEMENU:

		//使うが選択された時
		if (select_detail_window->getSelectNum() == USE) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				//itemを使用する
				item->ItemUse(selected_item_id);
			}
		}
		//詳細が選択された時
		else if (select_detail_window->getSelectNum() == DETAILE) {

			//インベントリが空の時は処理をとばす
			if (inventory_list.empty()) {
				return;
			}
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ITEMDETAILMENU;
			}
		}
		//装備が選択された時
		else if (select_detail_window->getSelectNum() == EQUIP) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				EquipWeapon(selected_index);
			}
		}
		//捨てるが選択された時
		else if (select_detail_window->getSelectNum() == DISPOSE) {
			
			//アイテムが空の場合処理をとばす
			if (inventory_list.empty()) {
				return;
			}
			//選択してEnterキーを押したら
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

				// 選択したアイテムの ID と一致する要素を特定する
				auto itemid = inventory_list[selected_index].getItemId();

				auto itemToRemove = std::find_if(inventory_list.begin(), inventory_list.end(),
					[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

				// アイテムが見つかった場合は削除する
				if (itemToRemove != inventory_list.end()) {
					inventory_list.erase(itemToRemove);
					item_num--;
				}
			}
		}
		//閉じるが選択された時
		else if (select_detail_window->getSelectNum() == ITEMCLOSE) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ITEMMENU;
			}
		}
		
		break;

	case MenuWindow_I::STATUSMENU:

		//左キーを押すとアイテム使用メニューに戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		break;

	//スキル表示画面
	case MenuWindow_I::SKILLMENU:

		//Enterキーを押したらItemUseMenuに変更する
		//要素が空じゃ無ければ
		if (!SkillList.empty()) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::SKILLDATAILMENU;
			}

		}

		//スキルのカーソル移動時のインデックス操作
		SkillCurourIndex();


		//特技のカーソル移動
		SkillCousorMove();

		//左キーを押すとアイテム使用メニューに戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		break;

	//スキル説明
	case MenuWindow_I::SKILLDATAILMENU:

		//スキル選択に戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::SKILLMENU;
		}

		break;


	default:
		break;
	}

}

//スキルの描画
void Inventory::InventorySkill(const tnl::Vector2i& SKILLDRAWPOS, const tnl::Vector2i& CURENTPAGETEXT, const int& COUSOURX, const int& ITEMPARPAGE)
{
	int i = 0;
	int y = 34;

	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	//現在のページに基づいて表示するアイテムの範囲を計算
	start_index = skill_curent_page * ITEMPARPAGE;
	end_index = min((start_index + ITEMPARPAGE), static_cast<int>(SkillList.size()));

	for (int index = start_index; index < end_index; ++index) {
		//
		auto& SkillData = SkillList[index];

		//アイテムの名前を取得する
		std::string skillName = SkillData->getName();

		// アイテムの描画
		DrawStringEx(SKILLDRAWPOS.x, SKILLDRAWPOS.y + y * i, COLORWHITE, "%s", skillName.c_str());

		// カーソルを描画する位置を決定
		int cursorY = SKILLDRAWPOS.y + y * (skill_cousour % ITEMPARPAGE);
		ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", COUSOURX, cursorY, CURSORSIZE, 0, true);

		++i;

	}

	//ページの表示
	DrawStringEx(CURENTPAGETEXT.x, CURENTPAGETEXT.y, COLORWHITE, "%d / 4", skill_curent_page + 1);

}

//スキル説明の描画
void Inventory::SkillDetailDraw(int skill_index)
{
	//スキル配列
	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	//スキル説明用のバックウィンドウ
	auto skill_detail_window = menu_window.lock();
	
	//特技説明
	skill_detail_window->Menu_draw(SKILL_DETAIL_WINDOW_POS.x, SKILL_DETAIL_WINDOW_POS.y, SKILL_DETAIL_WINDOW_WIDTH, SKILL_DETAIL_WINDOW_HEIGHT);

	//スキル説明の文字用座標
	const tnl::Vector2i SUKILL_STRING_POS = { 400, 70 };

	DrawStringEx(SUKILL_STRING_POS.x, SUKILL_STRING_POS.y, COLORWHITE, "==スキル説明==");

	//スキルデータを取得
	auto& skillData = SkillList[skill_index];

	//スキルデータのスキル説明を取得
	std::string skillDatail = skillData->getDescription();
	
	//スキル説明を描画
	DrawStringEx(380,100, COLORWHITE, "%s", skillDatail.c_str());

}

//スキルのカーソル操作
void Inventory::SkillCousorMove()
{

	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	//スキルがない場合、何もしない
	if (SkillList.empty())
	{
		return;
	}
	//カーソルが一番上にいる場合カーソルをさらに上に押してもカーソルを動かさないようにする
	if (skill_cousour == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP))
	{
		return;
	}
	//カーソルの動き
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		skill_cousour = (skill_cousour + (SkillList.size() - 1)) % static_cast<int>(SkillList.size());

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// カーソルが最後の要素よりも下に移動しようとする場合は、何もせずに処理を終了する
		if (skill_cousour == SkillList.size() - 1) {
			return;
		}

		skill_cousour = (skill_cousour + 1) % SkillList.size();
	}

}

//スキルカーソルの移動時のインデックス操作
void Inventory::SkillCurourIndex()
{

	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	// 上キーが押されたときの処理
	if (skill_curent_page == 0 && skill_selected_index == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// 何もしない
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (skill_selected_index % ITEMPERPAGE == 0) {
			// カーソルがページ内の最上部にいる場合は、一つ上のページの最後の要素を選択
			if (skill_curent_page > 0) {
				--skill_curent_page;
				// インデックスを更新してページの最後の要素を選択する
				skill_selected_index = (skill_curent_page + 1) * ITEMPERPAGE - 1;
			}
		}
		else {
			// それ以外の場合は、一つ上の要素を選択
			--skill_selected_index;
		}
	}

	// 下キーが押されたときの処理
	if (skill_selected_index == SkillList.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		// 何もしない
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((skill_selected_index + 1) % ITEMPERPAGE == 0) {
			// カーソルがページ内の最下部にいる場合は、一つ下のページの最初の要素を選択
			// スキルが持てるインベントリは 20 で考えている為 4 ページ分まで開けるようにする
			if (skill_curent_page < ITEM_MAX_PAGE) {
				++skill_curent_page;
				// インデックスを更新してページの最初の要素を選択する
				skill_selected_index = curent_page * ITEMPERPAGE;
			}
		}
		else {
			// それ以外の場合は、一つ下の要素を選択
			++skill_selected_index;
		}
	}
}


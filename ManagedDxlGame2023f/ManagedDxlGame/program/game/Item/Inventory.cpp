#include "Inventory.h"
#include"../Manager/GameManager.h"
#include"../Manager/UiManager.h"
#include"../Object/Enemy.h"


Inventory::Inventory()
{
	item = std::make_shared<Item>();

	//メニューの初期化
	InitMenuWinodow();

	//AddInventory(item->GetItemById(16).GetItemId());
	//AddInventory(item->GetItemById(39).GetItemId());

}
//更新処理
void Inventory::Update(const float& delta_time)
{
	//インベントリの更新
	swichInventoryUpdate();

	//プレイヤーのステータスの更新処理
	UIManager::GetUIManager()->PlayerStatusBarUpdate(delta_time);

}

//描画処理
void Inventory::draw()
{
	
	if (select_menu == MenuWindow_I::EMPTY) {
		Game_Menu();
	}
	//選択されているのが最初のメニューの描画
	else if (select_menu == MenuWindow_I::FIRSTMENU) {
		First_Menu();
	}
	//選択されたのがアイテムメニューの描画
	else if (select_menu == MenuWindow_I::ITEMMENU) {

		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(ITEMES_DRAW_POS, CURENTPAGETEXT, CURSORX,ITEMPERPAGE);
	}
	//選択されたのがプレイヤーの強さメニューの描画
	else if (select_menu == MenuWindow_I::STATUSMENU) {
		PlyStatusMenu();
	}
	//選択されたのがアイテムの詳細メニューの描画
	else if (select_menu == MenuWindow_I::ITEMUSEMENU) {
		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(ITEMES_DRAW_POS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		ItemUseMenu();
	}
	//アイテムを使用する際のウィンドウのメニューの描画
	else if (select_menu == MenuWindow_I::ITEMDETAILMENU) {
		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(ITEMES_DRAW_POS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		ItemUseMenu();
		ItemDetail();
	}
	//スキル表示用のウィンドウの描画
	else if (select_menu == MenuWindow_I::SKILLMENU) {
		// ウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		InventorySkill(SKILL_DRAW_POS, CURENTPAGETEXT , CURSORX ,ITEMPERPAGE);

		//空じゃない場合のみ説明を表示
		if (!GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {
			SkillDetailDraw(skill_selected_index, SKILL_DETAIL_WINDOW_POS , BaseScene::SceneState::WORLDMAP);
		}

	}
	//インベントリ使用時に操作説明用の描画
	if (select_menu != MenuWindow_I::EMPTY) {

		UIManager::GetUIManager()->Menu_Draw("menu_window", BUTTON_DETAIL_WINDOW_POS.x, BUTTON_DETAIL_WINDOW_POS.y, BUTTON_DETAIL_WINDOW_WIDTH, BUTTON_DETAIL_WINDOW_HEIGHT);

		DrawStringEx(BUTTON_BACK_STRING_POS.x, BUTTON_BACK_STRING_POS.y, koni::Color::WHITE, "back space : 戻る");
	}
	
}

//アイテムを追加する
void Inventory::AddInventory(const int& id)
{
	if (inventory_list.size() == INVENTORY_MAX_SIZE_) return;
	ItemBase newItem = item->GetItemById(id);
	inventory_list.emplace_back(newItem);
	item_num++;
}

//武器を装備する
void Inventory::EquipWeapon(const int& weaponIndex)
{
	//プレイヤーを取得する
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//イベントメッセージ
	std::string event_message_equip;

	// アイテムがない場合やインデックスが不正な場合は何もせずに関数を終了
	if (item_num == 0 || weaponIndex < 0 || weaponIndex >= item_num) {
		return;
	}

	// 選択されたアイテムを取得
	ItemBase selectedItem = inventory_list[weaponIndex];

	//選択したアイテムが武具じゃなければ飛ばす
	if (selectedItem.GetItemType() == EMPTY) 
	{
		// UIマネージャーの取得
		auto ui_manager = UIManager::GetUIManager();

		//イベント通知用の文字
		event_message_equip = "装備できるアイテムじゃありません";

		//通知用の文字をセットする
		ui_manager->SetDisplayEventMessage(event_message_equip);

		//ウィンドウを表示させて取得文字を出す
		if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

		return; 
	}

	// 同じアイテムが既に装備されているかチェック
	bool isAlreadyEquipped = false;
	for (const auto& equipped : equipped_weapon) {
		if (equipped.GetItemId() == selectedItem.GetItemId()) {
			// 同じアイテムが装備されている場合、何もせずに関数を終了
			isAlreadyEquipped = true;
			break;
		}
	}

	for (const auto& equipped : equipped_armor) {
		if (equipped.GetItemId() == selectedItem.GetItemId()) {
			// 同じアイテムが装備されている場合、何もせずに関数を終了
			isAlreadyEquipped = true;
			break;
		}
	}


	// 同じアイテムが既に装備されている場合は何もせずに関数を終了
	if (isAlreadyEquipped) {
		return;
	}

	//イベント通知用のステータス
	std::string event_status_message;

	// 新しいアイテムのタイプに応じて装備を切り替える
	if (selectedItem.GetItemType() == WEAPON) {
		auto base_attack = player->GetPlayerStatusSave().GetBaseAttack();
		// 装備中の武器を外す
		if (!equipped_weapon.empty()) {
			// 装備中の武器を削除する
			equipped_weapon.pop_back();
		}
		
		// 新しい武器を装備
		equipped_weapon.push_back(selectedItem);
		//装備した際にプレイヤーの画像を変える
		player->SetPlayerAnimationHdl(selectedItem.GetItemWeapontype() , player->GetPlayerID());
		// プレイヤーのステータスに新しい武器の効果を反映
		equip_weapon = true;
		equip_attack = selectedItem.GetItemDamage();
		player->GetPlayerStatusSave().SetPlayerAttack(base_attack + equip_attack);

		//ステータスの上昇値を表示する
		event_status_message = player->GetPlayerName() + " は攻撃力が" + std::to_string(equip_attack) + "上昇した";
	}
	else if (selectedItem.GetItemType() == ARMOR) {
		auto base_defance = player->GetPlayerStatusSave().GetBaseDefance();
		// 装備中の防具を外す
		if (!equipped_armor.empty()) {
			// 装備中の防具を削除する
			equipped_armor.pop_back();
		}
		// 新しい防具を装備
		equipped_armor.push_back(selectedItem);
		// プレイヤーのステータスに新しい防具の効果を反映
		equip_armor = true;
		equip_defance = selectedItem.GetItemDefance();
		player->GetPlayerStatusSave().SetPlayerDefance(base_defance + equip_defance);

		//ステータスの上昇値を表示する
		event_status_message = player->GetPlayerName() + " は防御力が" + std::to_string(equip_defance) + "上昇した";
	}

	// UIマネージャーの取得
	auto ui_manager = UIManager::GetUIManager();

	//イベント通知用の文字
	event_message_equip = selectedItem.GetItemName() + "を装備した\n\n" + event_status_message;

	//通知用の文字をセットする
	ui_manager->SetDisplayEventMessage(event_message_equip);

	//ウィンドウを表示させて取得文字を出す
	if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }
}

//アイテムの詳細を開いた時の処理
void Inventory::ItemDetail()
{
	//ウィンドウの描画
	UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_DETAIL_WINDOW_POS.x, ITEM_DETAIL_WINDOW_POS.y, ITEM_DETAIL_WINDOW_WIDTH, ITEM_DETAIL_WINDOW_HEIGHT);

	//アイテム説明の文字を表示する座標
	const tnl::Vector2i ITEM_DETAIL = { 630 , 70 };

	DrawStringEx(ITEM_DETAIL.x, ITEM_DETAIL.y, koni::Color::WHITE, "===アイテム説明===");

	//アイテム説明を取得する
	auto& inventory_item = inventory_list[selected_index];

	//インベントリが空じゃない場合
	if (!inventory_list.empty()) {

		//アイテムの詳細を表示する
		DrawStringEx(ITEM_DETAIL_POS.x, ITEM_DETAIL_POS.y, koni::Color::WHITE, "%s" , inventory_item.GetItemDetail().c_str());
		//アイテムのステータスを表示する
		DrawStringEx(ITEM_STAUS_POS.x, ITEM_STAUS_POS.y, koni::Color::WHITE, "攻撃力 : %d", inventory_item.GetItemDamage());
		DrawStringEx(ITEM_STAUS_POS.x + ITEM_ADD_OFSET_, ITEM_STAUS_POS.y, koni::Color::WHITE, "防御力 : %d", inventory_item.GetItemDefance());

	}

}

//カーソル移動時のインデックス操作と取得
void Inventory::ItemCurourIndex(const int& ItemPerPage)
{
	//---選択したアイテムにをIDとして取得する---//

	// 上キーが押されたときの処理
	//一番上にカーソルがいた場合それ以上にいかないようにする
	if (curent_page == 0 && selected_index == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		// 何もしない
		return;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
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
	if (selected_index == inventory_list.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) { return; }
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
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
		selected_item_id = selectedItem.GetItemId();
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
	if (select_cursor == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) 
	{
		return;
	}
	//カーソルの動き
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		item_num = static_cast<int>(inventory_list.size());
		select_cursor = (select_cursor + (item_num - 1)) % item_num;

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		item_num = static_cast<int>(inventory_list.size());
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
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
		{MENU_TOOL_POS.x,   MENU_TOOL_POS.y ,	 "道具",    MenuWindow::Elements::FIRST_ELEMENT },
		{MENU_SKILL_POS.x,  MENU_SKILL_POS.y ,   "特技",    MenuWindow::Elements::SECOND_ELEMENT},
		{MENU_STATUS_POS.x, MENU_STATUS_POS.y ,  "強さ",    MenuWindow::Elements::THERD_ELEMENT},
		{MENU_CLOSE_POS.x,  MENU_CLOSE_POS.y ,   "閉じる" , MenuWindow::Elements::FOUR_ELEMENT}
	};

	first_menu = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", first_menu_coment, 4);
	first_menu->Open();

	//アイテムウィンドウ内で選択するメニュー
	MenuWindow::MenuElement_t* select_detail = new MenuWindow::MenuElement_t[]
	{
		{MENU_ITEM_USE_POS.x ,    MENU_ITEM_USE_POS.y ,     "使う" ,    MenuWindow::Elements::FIRST_ELEMENT},
		{MENU_ITEM_DETAIL_POS.x , MENU_ITEM_DETAIL_POS.y ,  "詳細" ,    MenuWindow::Elements::SECOND_ELEMENT},
		{MENU_ITEM_EQUIP_POS.x ,  MENU_ITEM_EQUIP_POS.y ,   "装備する" ,MenuWindow::Elements::THERD_ELEMENT},
		{MENU_ITEM_DISPOSE_POS.x ,MENU_ITEM_DISPOSE_POS.y , "捨てる",   MenuWindow::Elements::FOUR_ELEMENT},
		{MENU_ITEM_CLOSE_POS.x ,  MENU_ITEM_CLOSE_POS.y ,   "閉じる" ,  MenuWindow::Elements::FIVE_ELEMENT}
	};

	select_action_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_detail, 5);
	select_action_window->Open();
}

//プレイヤーが動いている時のメニュー表示
void Inventory::Game_Menu()
{
	//半透明にする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_70_PERCENT);
	UIManager::GetUIManager()->Menu_Draw("menu_window", PLAYER_STATUS_WINDOW_POS.x, PLAYER_STATUS_WINDOW_POS.y, PLAYER_STATUS_WINDOW_WIDTH, PLAYER_STATUS_WINDOW_HEIGHT);
	//アルファ値を戻す
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_OPAQUE);

	//プレイヤーのステータスの描画
	UIManager::GetUIManager()->PlayerStatusDrawWindow();

	//プレイヤーを取得する
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//ウィンドウの描画
	UIManager::GetUIManager()->Menu_Draw("menu_window", GOLD_DISPLAY_WINDOW_POS.x, GOLD_DISPLAY_WINDOW_POS.y, GOLD_DISPLAY_WINDOW_WIDTH, GOLD_DISPLAY_WINDOW_HEIGHT);
	

	DrawStringEx(GOLD_STRING_POS.x, GOLD_STRING_POS.y, koni::Color::WHITE, "G : %d", player->GetPlayerMoney());

}

//一番最初のメニュー
void Inventory::First_Menu()
{
	first_menu->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, FIRST_MENU_WINDOW_WIDTH, FIRST_MENU_WINDOW_HEIGHT);
}

//指定したIDのアイテムを削除する
void Inventory::InventoryItemRemove(const int& item_id)
{
	// 選択したアイテムの ID と一致する要素を特定する
	auto itemToRemove = std::find_if(inventory_list.begin(), inventory_list.end(),
		[&](const ItemBase& item) { return item.GetItemId() == item_id; });

	// アイテムが見つかった場合は削除する
	if (itemToRemove != inventory_list.end()) {

		inventory_list.erase(itemToRemove);
		//アイテムの数を減らす
		--item_num;
	}
}

//アイテムを削除する際の武器を装備していた際の処理
void Inventory::DeleteEquipItemProcess()
{
	//アイテムが空の場合処理をとばす
	if (inventory_list.empty()) { return; }

	//武器を装備していた場合の処理
	auto itr = equipped_weapon.begin();

	if (itr != equipped_weapon.end()) {
		//選択されたアイテムを装備していた場合
		if (inventory_list[selected_index].GetItemId() == (*itr).GetItemId()) {

			auto& player = GameManager::GetGameManager()->GetPlayer();

			//武器の装備を外す
			equipped_weapon.erase(itr);

			//フラグを切り替える(E文字の表示を行えないようにする為)
			equip_weapon = false;

			//プレイヤーのステータスを調整する
			player->GetPlayerStatusSave().SetPlayerAttack(player->GetPlayerStatusSave().GetBaseAttack());

			//プレイヤーの見た目を変更する
			player->SetPlayerAnimationHdl(Player::PlayerEquipState::BARE_HANDS , player->GetPlayerID());
		}
	}

	//防具を装備していた場合の処理
	auto it = equipped_armor.begin();

	if (it != equipped_armor.end()) {
		//選択されたアイテムを装備していた場合
		if (inventory_list[selected_index].GetItemId() == (*it).GetItemId()) {

			auto& player = GameManager::GetGameManager()->GetPlayer();

			//武器の装備を外す
			equipped_armor.erase(it);

			//フラグを切り替える(E文字の表示を行えないようにする為)
			equip_armor = false;

			//プレイヤーのステータスを調整する
			player->GetPlayerStatusSave().SetPlayerDefance(player->GetPlayerStatusSave().GetBaseDefance());
		}
	}
}

//アイテムを捨てるが選択された時の処理
void Inventory::DisposeItemProcess()
{
	//アイテムが空の場合処理をとばす
	if (inventory_list.empty()) { return; }

	//決定音を鳴らす
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

	// 選択したアイテムの ID と一致する要素を特定する
	auto& selected_item = inventory_list[selected_index];

	//アイテムが削除可能であれば
	if (selected_item.GetEssentialItemsFlag()) {

		//消去したいアイテムを装備していた時の処理
		DeleteEquipItemProcess();

		//アイテムの選択メニューに戻す
		//select_menu = MenuWindow_I::ITEMMENU;

		// 選択されたアイテムを削除する
		inventory_list.erase(inventory_list.begin() + selected_index);
		item_num--;

		//カーソルとインデックスを最初に戻す
		SelectedIndexClear();
		CurentPageReset();
		CursorReset();
	}
}

//アイテムの表示
void Inventory::ItemMenu(const tnl::Vector2i& itemDrawPos, const tnl::Vector2i& curentPageText, const int& CousourX, const int& itemParPage /*,const tnl::Vector2i& item_icon_pos*/)
{
	//インデックス
	int i = 0;
	//アイテム間の垂直方向の間隔
	const int ITEMVERTICALSPACING = 34;
	//武器装備時のEquipの文字のオフセット表示座標
	const int EQUIP_TEXT_OFFSET_Y = 30;
	const int EQUIP_TEXT_OFFSET_X = 58;
	//アイコンの表示時のオフセット座標
	const int ICON_HORIZONTAL_OFFSET = 18;
	//文字の調整地値
	const int TEXT_VERTICAL_ADJUSTMENT = 6;


		//現在のページに基づいて表示するアイテムの範囲を計算
		start_index = curent_page * itemParPage;
		end_index = min((start_index + itemParPage) , static_cast<int>(inventory_list.size()));

		for (int index = start_index; index < end_index; ++index) {

			//
			auto& InventoryData = inventory_list[index];
			//アイテムの名前を取得する
			std::string ItemName = InventoryData.GetItemName();

			//アイテムのアイコンを取得する
			int item_icon = ResourceManager::GetResourceManager()->LoadGraphEX(InventoryData.GetItemIcon().c_str());

			//アイテムアイコンの描画
			DrawRotaGraph(itemDrawPos.x - ICON_HORIZONTAL_OFFSET, (itemDrawPos.y + ITEMVERTICALSPACING * i)+ TEXT_VERTICAL_ADJUSTMENT, static_cast<double>(koni::Numeric::SCALE_ONE_F), 0 , item_icon , true);

			// アイテムの描画
			DrawStringEx(itemDrawPos.x, itemDrawPos.y + ITEMVERTICALSPACING * i, koni::Color::WHITE, "%s", ItemName.c_str());

			// カーソルを描画する位置を決定
			int cursorY = itemDrawPos.y + ITEMVERTICALSPACING * (select_cursor % itemParPage);
			ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CousourX, cursorY, CURSORSIZE, 0, true);
		
			//装備できるアイテムだった場合
			if (InventoryData.GetItemType() == 1 || InventoryData.GetItemType() == 2) {
				// アイテムが装備されているか、選択されたアイテムかを確認
				if ((equip_weapon && InventoryData.GetItemId() == equipped_weapon.back().GetItemId())) {
					// 装備中のアイテムまたは選択されたアイテムの場合、"[E]" を表示
					//アイテムの名前から左に55ずれた座標に表示
					DrawStringEx(itemDrawPos.x - EQUIP_TEXT_OFFSET_X, itemDrawPos.y + EQUIP_TEXT_OFFSET_Y * i + TEXT_VERTICAL_ADJUSTMENT, koni::Color::WHITE, "[E]");
				}
				if ((equip_armor && InventoryData.GetItemId() == equipped_armor.back().GetItemId())) {
					// 装備中のアイテムまたは選択されたアイテムの場合、"[E]" を表示
					//アイテムの名前から左に55ずれた座標に表示
					DrawStringEx(itemDrawPos.x - EQUIP_TEXT_OFFSET_X, itemDrawPos.y + EQUIP_TEXT_OFFSET_Y * i + TEXT_VERTICAL_ADJUSTMENT, koni::Color::WHITE, "[E]");
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
		DrawStringEx(curentPageText.x, curentPageText.y, koni::Color::WHITE, "%d / 4", curent_page + 1);

}

//プレイヤーのステータス表示
void Inventory::PlyStatusMenu()
{

	//プレイヤーを取得する
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//ウィンドウの描画
	UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, PLAYER_STATUS_GOLD_WINDOW_WIDTH, PLAYER_STATUS_GOLD_WINDOW_HEIGHT);

	//強さ文字の座標
	const tnl::Vector2i PLAYER_STATUS_STRING = { 150 , 70 };
	
	//強さの文字を表示する
	DrawStringEx(PLAYER_STATUS_STRING.x, PLAYER_STATUS_STRING.y, koni::Color::WHITE, "===強さ===");

	//追加していく座標
	const int Y = 50;

	// Player クラスのポインターが有効かどうかを確認する
	if (player) {
		// Player クラスのポインターを介してステータスを取得する
		int hp = static_cast<int>(player->GetPlayerStatusSave().GetcurentHp());
		int mp = static_cast<int>(player->GetPlayerStatusSave().GetCurentMp());
		int Attack = player->GetPlayerStatusSave().GetAttack();
		int Defense = player->GetPlayerStatusSave().GetDefance();
		int Level = player->GetPlayerStatusSave().GetLevel();
		int Speed = player->GetPlayerStatusSave().GetSpeed();
		int R_expoint = player->GetPlayerStatusSave().GetExpoint();

		// 取得したステータスを描画する
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y, koni::Color::WHITE, "HP : %d", hp);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + Y, koni::Color::WHITE, "MP : %d", mp);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_DOUBLE_I),	koni::Color::WHITE, "攻撃力 : %d", Attack);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_TRIPLE_I),	koni::Color::WHITE, "防御力 : %d", Defense);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_QUADRUPLE_I), koni::Color::WHITE, "レベル : %d", Level);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_QUINTUPLE_I), koni::Color::WHITE, "すばやさ : %d", Speed);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_SIXFOLD_I),   koni::Color::WHITE, "レベルアップに必要な経験値 : %d", R_expoint);
	}

}

//アイテムを使用するメニュー
void Inventory::ItemUseMenu()
{
	select_action_window->All(ITEM_USE_POS.x, ITEM_USE_POS.y, ITEM_USE_WINDOW_WIDTH, ITEM_USE_WINDOW_HEIGHT);

}

//インベントリの処理
void Inventory::swichInventoryUpdate()
{
	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	switch (select_menu)
	{
	case Inventory::MenuWindow_I::EMPTY:

		break;

		//最初のメニューでの処理
	case Inventory::MenuWindow_I::FIRSTMENU:
	
		//1個目が選択された場合、アイテムを表示する
		if (first_menu->GetSelectNum() == TOOL && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ITEMMENU;
			CursorReset();
			curent_page = 0;
			selected_index = 0;
		}
			//2個目が選択された場合、特技を表示する
		else if (first_menu->GetSelectNum() == SKILL && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::SKILLMENU;

		}
		//3個目が選択された場合、プレイヤーの強さを表示する
		else if (first_menu->GetSelectNum() == STATUS && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::STATUSMENU;
		}
		//4個目が選択された場合、閉じる
		else if (first_menu->GetSelectNum() == CLOSE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::EMPTY;
			//plyerを動けるようにする
			if (!GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
				GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
			}
		
			}

		//バックスペースを押すとアイテム使用メニューに戻す
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::EMPTY;
			//plyerを動けるようにする
			if (!GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
				GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
			}
		}

		break;

		//アイテムメニューでの処理
	case Inventory::MenuWindow_I::ITEMMENU:

		//Enterキーを押したらItemUseMenuに変更する
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ITEMUSEMENU;
		}
		//バックスペースを押すとアイテム使用メニューに戻す
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		//インデックス操作
		ItemCurourIndex(ITEMPERPAGE);
		
		//カーソルの動き
		CusorMove();

		break;

	//アイテム詳細メニューの場合
	case Inventory::MenuWindow_I::ITEMDETAILMENU:

		//バックスペースを押すとアイテム使用メニューに戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::ITEMUSEMENU;
		}

		break;

		//アイテム使用メニュー
	case Inventory::MenuWindow_I::ITEMUSEMENU:

		//イベント通知文字が表示されていたらカーソルを動けないようにする
		if (UIManager::GetUIManager()->GetIsNotificationVisible()&& select_action_window->GetSelectCursour()) 
		{
			//カーソルを動けないようにする
			select_action_window->SelectCousourMoveFlagChange(); 

			return;

		}
		//イベント通知フラグがtrueの状態でカーソルフラグがfalseの場合、backSpaceを押したらウィンドウを消して動けるようにする
		else if (UIManager::GetUIManager()->GetIsNotificationVisible() && !select_action_window->GetSelectCursour()) {

			//backSpaceを押したら
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {

				//通知ウィンドウを消去する
				UIManager::GetUIManager()->ToggleAcquisitionWindow();

				//カーソルを動けるようにする
				select_action_window->SelectCousourMoveFlagChange();

				return;
			}
		}

		//使うが選択された時
		if (select_action_window->GetSelectNum() == USE) {

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				//itemを使用する
				//アイテムが使用できたら
				if (item->ItemUse(selected_item_id)) {
					//カーソルとインデックスを最初に戻す
					SelectedIndexClear();
					CurentPageReset();
					CursorReset();
				}
				//アイテムが使用出来なかったら
				else {

					//デバックログを流す
					tnl::DebugTrace("\n------------------------------------------------------------");
					tnl::DebugTrace("\n アイテムが使用できませんでした。");
					tnl::DebugTrace("\n------------------------------------------------------------");

				}
			}
		}
		//詳細が選択された時
		else if (select_action_window->GetSelectNum() == DETAILE) {

			//インベントリが空の時は処理をとばす
			if (inventory_list.empty()) {
				return;
			}
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ITEMDETAILMENU;
			}
		}
		//装備が選択された時
		else if (select_action_window->GetSelectNum() == EQUIP) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				EquipWeapon(selected_index);
			}
		}
		//捨てるが選択された時
		else if (select_action_window->GetSelectNum() == DISPOSE) {
			
			//エンターキーが押された時
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//捨てる処理
				DisposeItemProcess();
			}
		}
		//閉じるが選択された時
		else if (select_action_window->GetSelectNum() == ITEMCLOSE) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ITEMMENU;
			}
		}

		//バックスペースを押すとアイテム使用メニューに戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::ITEMMENU;
		}
		
		break;

	//プレイヤーのステータス表示
	case MenuWindow_I::STATUSMENU:

		//バックスペースを押すとアイテム使用メニューに戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		break;

	//スキル表示画面
	case MenuWindow_I::SKILLMENU:

		//スキルのカーソル移動時のインデックス操作
		SkillCurourIndex(ITEMPERPAGE);


		//特技のカーソル移動
		SkillCousorMove();

		//バックスペースを押すと最初のメニューに戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::FIRSTMENU;
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

	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	//現在のページに基づいて表示するアイテムの範囲を計算
	start_index = skill_curent_page * ITEMPARPAGE;
	end_index = min((start_index + ITEMPARPAGE), static_cast<int>(SkillList.size()));

	for (int index = start_index; index < end_index; ++index) {
		//
		auto& SkillData = SkillList[index];

		//アイテムの名前を取得する
		std::string skillName = SkillData->GetName();

		// アイテムの描画
		DrawStringEx(SKILLDRAWPOS.x, SKILLDRAWPOS.y + y * i, koni::Color::WHITE, "%s", skillName.c_str());

		// カーソルを描画する位置を決定
		int cursorY = SKILLDRAWPOS.y + y * (skill_cousour % ITEMPARPAGE);
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", COUSOURX, cursorY, CURSORSIZE, 0, true);

		++i;

	}

	//ページの表示
	DrawStringEx(CURENTPAGETEXT.x, CURENTPAGETEXT.y, koni::Color::WHITE, "%d / 4", skill_curent_page + 1);


}

//スキル説明の描画
void Inventory::SkillDetailDraw(const int& skill_index, const tnl::Vector2i& SKILL_WINDOW_POS, const BaseScene::SceneState& curent_scene, const int enemy_id)
{
	//文字のインデント
	const int INDENT_X = 30;
	//スキル説明文字のオフセット座標
	const int SKILL_TITLE_OFFSET = 20;
	//スキル説明の文字を表示する為のオフセット座標
	const int DESCRIPTION_Y = 50;
	//必要なMPを表示する為のオフセット座標
	const int MP_Y = DESCRIPTION_Y + 130;

	// スキル配列
	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	// 特技説明
	UIManager::GetUIManager()->Menu_Draw("menu_window", SKILL_WINDOW_POS.x, SKILL_WINDOW_POS.y, SKILL_DETAIL_WINDOW_WIDTH, SKILL_DETAIL_WINDOW_HEIGHT);

	// スキル説明文字の表示
	DrawStringEx(SKILL_WINDOW_POS.x + INDENT_X + SKILL_TITLE_OFFSET, SKILL_WINDOW_POS.y + SKILL_TITLE_OFFSET, koni::Color::WHITE, "==スキル説明==");

	// スキルデータを取得
	auto& skillData = SkillList[skill_index];

	// スキルが入っていれば
	if (!SkillList.empty()) {

		// スキルデータのスキル説明を取得
		const std::string& skillDatail = skillData->GetDescription();
		const int skill_mp = skillData->GetSkillConsumeMp();

		// スキル説明を描画
		DrawStringEx(SKILL_WINDOW_POS.x + INDENT_X, SKILL_WINDOW_POS.y + DESCRIPTION_Y, koni::Color::WHITE, "%s", skillDatail.c_str());
		
		//バトルシーンでかつ攻撃スキルの場合
		if (curent_scene == BaseScene::SceneState::BATTLE && skillData->GetSkillType() == AttackType) {

			//スキルのサポート機能による座標のオフセット
			const int SUPPORT_MESSAGE_OFFSET = 50;

			// スキルの弱点によるサポート機能
			DrawStringEx(SKILL_WINDOW_POS.x + INDENT_X, SKILL_WINDOW_POS.y + DESCRIPTION_Y + SUPPORT_MESSAGE_OFFSET, koni::Color::WHITE, "%s", SkillSupportExplanation(skillData->GetSkillAttribute(), enemy_id).c_str());

		}
		
		// スキルの必要Mpの描画
		DrawStringEx(SKILL_WINDOW_POS.x + INDENT_X, SKILL_WINDOW_POS.y + MP_Y, koni::Color::WHITE, "必要なMP : %d", skill_mp);
	}

}

//スキルの属性値によるサポート説明
const std::string& Inventory::SkillSupportExplanation(const Skill::SkillAttribute& curent_attribute , const int& enemy_id)
{
	//弱点属性値
	const float WEAKNESS_ATTRIBUTE = 1.5f;
	//強耐性
	const float STRONG_RESISTANCE = 0.5f;
	//通常耐性
	const float NORMAL_RESISTANCE = 1.0f;

	//炎属性だった場合
	if (curent_attribute == Skill::SkillAttribute::FIRE) {

		//敵のポインタを生成する
		auto enemy = std::make_shared<Enemy>();

		//敵のIDから敵のステータスを取得する
		auto curent_enemy = enemy->GetEnemyStatus(enemy_id);

		//敵の属性値が等倍だったら
		if (curent_enemy.GetFireResist() == WEAKNESS_ATTRIBUTE) {
			return EXCELLENT_EFFECT;
		}
		else if (curent_enemy.GetFireResist() == NORMAL_RESISTANCE) {
			return NORMAL_RESISTANCE_STRING;
		}
		else if (curent_enemy.GetFireResist() == STRONG_RESISTANCE) {
			return STRONG_RESISTANCE_STRING;
		}
	}
	//水・氷属性の場合
	else if (curent_attribute == Skill::SkillAttribute::WATER) {

		//敵のポインタを生成する
		auto enemy = std::make_shared<Enemy>();

		//敵のIDから敵のステータスを取得する
		auto curent_enemy = enemy->GetEnemyStatus(enemy_id);

		//敵の属性値が等倍だったら
		if (curent_enemy.GetIceResist() == WEAKNESS_ATTRIBUTE) {
			return EXCELLENT_EFFECT;
		}
		else if (curent_enemy.GetIceResist() == NORMAL_RESISTANCE) {
			return NORMAL_RESISTANCE_STRING;
		}
		else if (curent_enemy.GetIceResist() == STRONG_RESISTANCE) {
			return STRONG_RESISTANCE_STRING;
		}
	}
	//雷属性の場合
	else if (curent_attribute == Skill::SkillAttribute::THUNDER) {

		//敵のポインタを生成する
		auto enemy = std::make_shared<Enemy>();

		//敵のIDから敵のステータスを取得する
		auto curent_enemy = enemy->GetEnemyStatus(enemy_id);

		//敵の属性値が等倍だったら
		if (curent_enemy.GetThunderResist() == WEAKNESS_ATTRIBUTE) {
			return EXCELLENT_EFFECT;
		}
		else if (curent_enemy.GetThunderResist() == NORMAL_RESISTANCE) {
			return NORMAL_RESISTANCE_STRING;
		}
		else if (curent_enemy.GetThunderResist() == STRONG_RESISTANCE) {
			return STRONG_RESISTANCE_STRING;
		}
	}
	//無属性の場合安定したダメージを与えられる
	else if (curent_attribute == Skill::SkillAttribute::NON_ATTRIBUTE) {
		return NORMAL_RESISTANCE_STRING;
	}

	//エラー時
	return ERROR_MESSAGE;
}

//スキルのカーソル操作
void Inventory::SkillCousorMove()
{

	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	//スキルがない場合、何もしない
	if (SkillList.empty())
	{
		return;
	}
	//カーソルが一番上にいる場合カーソルをさらに上に押してもカーソルを動かさないようにする
	if (skill_cousour == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_W))
	{
		return;
	}
	//カーソルの動き
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		skill_cousour = (skill_cousour + (SkillList.size() - 1)) % static_cast<int>(SkillList.size());

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// カーソルが最後の要素よりも下に移動しようとする場合は、何もせずに処理を終了する
		if (skill_cousour == SkillList.size() - 1) {
			return;
		}

		skill_cousour = (skill_cousour + 1) % SkillList.size();
	}

}

//スキルカーソルの移動時のインデックス操作
void Inventory::SkillCurourIndex(const int& skil_list_perpage)
{

	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	// 上キーが押されたときの処理
	if (skill_curent_page == 0 && skill_selected_index == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		// 何もしない
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		if (skill_selected_index % skil_list_perpage == 0) {
			// カーソルがページ内の最上部にいる場合は、一つ上のページの最後の要素を選択
			if (skill_curent_page > 0) {
				--skill_curent_page;
				// インデックスを更新してページの最後の要素を選択する
				skill_selected_index = (skill_curent_page + 1) * skil_list_perpage - 1;
			}
		}
		else {
			// それ以外の場合は、一つ上の要素を選択
			--skill_selected_index;
		}
	}

	// 下キーが押されたときの処理
	if (skill_selected_index == SkillList.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		// 何もしない
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		if ((skill_selected_index + 1) % skil_list_perpage == 0) {
			// カーソルがページ内の最下部にいる場合は、一つ下のページの最初の要素を選択
			// スキルが持てるインベントリは 20 で考えている為 4 ページ分まで開けるようにする
			if (skill_curent_page < ITEM_MAX_PAGE) {
				++skill_curent_page;
				// インデックスを更新してページの最初の要素を選択する
				skill_selected_index = skill_curent_page * skil_list_perpage;
			}
		}
		else {
			// それ以外の場合は、一つ下の要素を選択
			++skill_selected_index;
		}
	}
}


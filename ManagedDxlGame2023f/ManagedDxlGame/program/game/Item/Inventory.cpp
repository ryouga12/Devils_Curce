#include "Inventory.h"

using namespace std;

Inventory::Inventory()
{
	item = make_shared<Item>();

	//メニューの初期化
	InitMenuWinodow();

	//デバック
	AddInventory(item->GetItemById(21).getItemId());

}

Inventory::~Inventory()
{
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/cousour_bgm.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/kaihuku.mp3");

	//画像を消去する
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/cur_sannkaku2.png");

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
	
	//選択されているのが最初のメニューだったら
	if (select_menu == MenuWindow_I::First_Menu) {
		First_Menu();
	}
	//選択されたのがアイテムメニューだったら
	else if (select_menu == MenuWindow_I::ItemMenu) {
		menu_window->Menu_draw();
		ItemMenu(DrawPos, CurentPageText, cursorX,item_per_page);
	}
	//選択されたのがプレイヤーの強さメニューだったら
	else if (select_menu == MenuWindow_I::StatusMenu) {
		PlyStatusMenu();
	}
	//選択されたのがアイテムの詳細メニューだったら
	else if (select_menu == MenuWindow_I::ItemUseMenu) {
		menu_window->Menu_draw();
		ItemMenu(DrawPos, CurentPageText, cursorX, item_per_page);
		ItemUseMenu();
		if (item_coment == ItemComent::No) {
			item_coment_window->Menu_draw();
			DrawStringEx(ItemComentPos.x, ItemComentPos.y, Color_White, "このアイテムは使用できません");
		}
	}
	else if (select_menu == MenuWindow_I::ItemDetailMenu) {
		menu_window->Menu_draw();
		ItemMenu(DrawPos, CurentPageText, cursorX, item_per_page);
		ItemUseMenu();
		ItemDetail();
	}
	else if (select_menu == MenuWindow_I::SkillMenu) {
		InventorySkill();

	}
	else if (select_menu == MenuWindow_I::SkillDatailMenu) {
		InventorySkill();
		SkillDetailDraw(selectedSkillIndex);
	}
	
}

//アイテムを追加する
void Inventory::AddInventory(int id)
{
	if (InventoryList.size() == InventoryMaxSize) return;
	ItemBase newItem = item->GetItemById(id);
	InventoryList.emplace_back(newItem);
	itemNum++;
}

//武器を装備する
void Inventory::EquipWeapon(int weaponIndex)
{
	// アイテムがない場合やインデックスが不正な場合は何もせずに関数を終了
	if (itemNum == 0 || weaponIndex < 0 || weaponIndex >= itemNum) {
		return;
	}

	// 選択されたアイテムを取得
	ItemBase selectedItem = InventoryList[weaponIndex];

	// 同じアイテムが既に装備されているかチェック
	bool isAlreadyEquipped = false;
	for (const auto& equipped : equippedWeapon) {
		if (equipped.getItemId() == selectedItem.getItemId()) {
			// 同じアイテムが装備されている場合、何もせずに関数を終了
			isAlreadyEquipped = true;
			break;
		}
	}

	for (const auto& equipped : equippedArmor) {
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
		auto curentAttack = plyer->getPlayerStatusSave().getAttack();
		// 装備中の武器を外す
		if (!equippedWeapon.empty()) {
			// 装備中の武器の効果を反映から削除する
			ItemBase equippedWeaponItem = equippedWeapon.back();
			//装備中の武器のステータスを削除する
			curentAttack -= equippedWeaponItem.getItemDamage();
			// 装備中の武器を削除する
			equippedWeapon.pop_back();
		}
		
		// 新しい武器を装備
		equippedWeapon.push_back(selectedItem);
		// プレイヤーのステータスに新しい武器の効果を反映
		equipWeapon = true;
		equipAttack = selectedItem.getItemDamage();
		plyer->getPlayerStatusSave().SetPlayerAttack(curentAttack + equipAttack);
	}
	else if (selectedItem.getItemType() == ARMOR) {
		auto curentDefance = plyer->getPlayerStatusSave().getDefance();
		// 装備中の防具を外す
		if (!equippedArmor.empty()) {
			// 装備中の防具の効果を反映から削除する
			ItemBase equippedArmorItem = equippedArmor.back();
			//装備中の防具のステータスを削除する
			curentDefance -= equippedArmorItem.getItemDefance();
			// 装備中の防具を削除する
			equippedArmor.pop_back();
		}
		// 新しい防具を装備
		equippedArmor.push_back(selectedItem);
		// プレイヤーのステータスに新しい防具の効果を反映
		equipArmor = true;
		equipDefance = selectedItem.getItemDefance();
		plyer->getPlayerStatusSave().SetPlayerDefance(curentDefance + equipDefance);
	}
}

//アイテムの詳細を開いた時の処理
void Inventory::ItemDetail()
{
	ItemDetailWindow->Menu_draw();

	DrawStringEx(630, 70, -1, "===アイテム説明===");

	//アイテム説明を取得する
	std::string ItemDetail = InventoryList[selectedIndex].getItemDetail();

	//インベントリが空じゃない場合
	if (!InventoryList.empty()) {
	//アイテムの詳細を表示する
		DrawStringEx(630 , 100 , -1 , "%s" ,ItemDetail.c_str());
	}

}

void Inventory::InventoryItemUse(int itemid)
{
	//プレイヤーの最大HPを取得する
	max_hp = static_cast<int>(plyer->getPlayerStatusSave().getMaxHp());
	auto max_mp = static_cast<int>(plyer->getPlayerStatusSave().getMaxMp());

	//武器関係は使用できませんと表示させる
	if (itemid == 0 || itemid == 1 || itemid == 2 || itemid == 4 || itemid == 5 || itemid == 6 || itemid == 7
		|| itemid == 9 || itemid == 10 || itemid == 11 || itemid == 12 || itemid == 13 || itemid == 15 || itemid == 18
		|| itemid == 19 || itemid == 25|| itemid == 26 || itemid == 28 || itemid == 29)
	{
		//使えませんとウィンドウに描画する
		item_coment = ItemComent::No;
		return;
	}
	//ポーション
	else if (itemid == 3) {
		
		//回復量(Hpの3割分回復させる)
		float percentage = 0.3;

		//Hpを回復する
		ItemHpHeal(percentage, itemid);

		//バトルシーンのみでログを流す
		if (BattleFlag) {
			battleLog->addRecoveryLog("Player", item->GetItemById(3).getItemName().c_str(), healAmount);
		}

	}
	//ハイポーション
	else if (itemid == 8) {
		
		//回復量(Hpの半分を回復させる)
		float percentage = 0.5;

		//Hpを回復する
		ItemHpHeal(percentage, itemid);

		if (BattleFlag) {
			battleLog->addRecoveryLog("Player", item->GetItemById(8).getItemName().c_str(), healAmount);
		}
	}
	else if (itemid == 14) {
		//プレイヤーのMpを取得する
		auto curentMp = plyer->getPlayerStatusSave().getCurentMp();

		// アイテムIDが14でかつプレイヤーのMPがMAXでない場合に処理を実行
		if (curentMp < max_mp) {

			// MPを最大MPの30％回復させる
			float percentage = 0.3;
			int healAmount = static_cast<int>(max_hp * percentage);
			plyer->getPlayerStatusSave().SetPlayerCurentMp(curentMp + healAmount);

			//回復音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

			// MPがMAX_Pを超える場合はMAX_HPにクリップする
			if (plyer->getPlayerStatusSave().getCurentMp() > max_mp) {
				plyer->getPlayerStatusSave().SetPlayerCurentMp(max_mp);
			}

			// 選択したアイテムの ID と一致する要素を特定する
			auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
				[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

			// アイテムが見つかった場合は削除する
			if (itemToRemove != InventoryList.end()) {
				InventoryList.erase(itemToRemove);
				itemNum--;
			}
			//バトルシーンのみでログを流す
			if (BattleFlag) {
				battleLog->addRecoveryLog("Player", item->GetItemById(14).getItemName().c_str(), healAmount);
			}
		}
		//MpがmaxMpと同じ場合処理を飛ばす
		else {
			return;
		}
	}
	if (itemid == 16 && BattleFlag) {

		//プレイヤーの攻撃力を取得する
		auto PlayerAttack = plyer->getPlayerStatusSave().getAttack();

		//プレイヤーの攻撃力の半分の値をプラスさせる(調整対象)
		float percentage = 0.5;

		AttackAmount = static_cast<int>(PlayerAttack * percentage);
		plyer->getPlayerStatusSave().SetPlayerAttack(PlayerAttack + AttackAmount);


		// 選択したアイテムの ID と一致する要素を特定する
		auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
			[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

		// アイテムが見つかった場合は削除する
		if (itemToRemove != InventoryList.end()) {
			InventoryList.erase(itemToRemove);
			itemNum--;
		}

		//ログを流す
		battleLog->addBuffLog("Player", item->GetItemById(16).getItemName().c_str(), "攻撃力", AttackAmount);
	}
	//プラントエキス(一番回復力が高いアイテム)
	if (itemid == 17) {

		//回復量(Hpの7割を回復させる)
		float percentage = 0.7;

		//hpを回復する
		ItemHpHeal(percentage, itemid);

		//バトルシーンのみでログを流す
		if (BattleFlag) {
			battleLog->addRecoveryLog("Player", item->GetItemById(17).getItemName().c_str(), healAmount);
		}
	}
	//スライムの鈴
	if (itemid == 20 && !BattleFlag) {

		//スキルを追加する処理
		AddSkillItem(itemid, Shared<SlimBell>());
	}
	//
	if (itemid == 21 && !BattleFlag) {

		//スキルを追加する処理
		AddSkillItem(itemid, Shared<SnakeBell>());
	}

}

//カーソル移動時のインデックス操作と取得
void Inventory::ItemCurourIndex(int ItemPerPage)
{
	//---選択したアイテムにをIDとして取得する---//

	// 上キーが押されたときの処理
	//一番上にカーソルがいた場合それ以上にいかないようにする
	if (currentPage == 0 && selectedIndex == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// 何もしない
		return;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selectedIndex % ItemPerPage == 0) {
			// カーソルがページ内の最上部にいる場合は、一つ上のページの最後の要素を選択
			if (currentPage > 0) {
				--currentPage;
				// インデックスを更新してページの最後の要素を選択する
				selectedIndex = (currentPage + 1) * ItemPerPage - 1;
			}
		}
		else {
			// それ以外の場合は、一つ上の要素を選択
			--selectedIndex;
		}
	}

	// 下キーが押されたときの処理
	if (selectedIndex == InventoryList.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN))return;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selectedIndex + 1) % ItemPerPage == 0) {
			// カーソルがページ内の最下部にいる場合は、一つ下のページの最初の要素を選択
			//今の所プレイヤーが持てるインベントリは20で考えている為4ページ分まで開けるようにする
			if (currentPage < 4) {
				++currentPage;
				// インデックスを更新してページの最初の要素を選択する
				selectedIndex = currentPage * ItemPerPage;
			}
		}
		else {
			// それ以外の場合は、一つ下の要素を選択
			++selectedIndex;
		}
	}

	// selectedIndexが有効な範囲内にあるか確認して、選択されたアイテムのIDを取得する
	if (selectedIndex >= 0 && selectedIndex < InventoryList.size()) {
		ItemBase selectedItem = InventoryList[selectedIndex];
		selectedItemId = selectedItem.getItemId();
	}
}

//インベントリのメニューを切り替える
void Inventory::SetSelect_num(int select_menu_)
{
	switch (select_menu_)
	{
	case 0 :
		select_menu = MenuWindow_I::Empty;

		break;

	case 1:
		select_menu = MenuWindow_I::First_Menu;


		break;

	case 2:

		select_menu = MenuWindow_I::ItemMenu;

		break;

	case 3:
		select_menu = MenuWindow_I::StatusMenu;

		break;

	case 4:
		select_menu = MenuWindow_I::ItemUseMenu;

		break;

	default:
		break;
	}
}

void Inventory::CusorMove()
{
	//アイテムがない場合、何もしない
	if (InventoryList.empty())
	{
		return;
	}
	//カーソルが一番上にいる場合カーソルをさらに上に押してもカーソルを動かさないようにする
	if (selectCursor == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) 
	{
		return;
	}
	//カーソルの動き
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		itemNum = GetItemCount();
		selectCursor = (selectCursor + (itemNum - 1)) % itemNum;

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		itemNum = GetItemCount();
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// カーソルが最後の要素よりも下に移動しようとする場合は、何もせずに処理を終了する
		if (selectCursor == itemNum - 1) {
			return;
		}
		selectCursor = (selectCursor + 1) % itemNum;
	}
}

void Inventory::CursorReset()
{
	selectCursor = 0;
}

//Hp回復系のアイテム
void Inventory::ItemHpHeal(float percentage , int itemid)
{
	auto curentHp = plyer->getPlayerStatusSave().getcurentHp();

	// アイテムIDが回復アイテムでかつ最大Hpより低ければ実行できる
	if (curentHp < max_hp) {

		healAmount = static_cast<int>(max_hp * percentage);
		plyer->getPlayerStatusSave().SetPlayerCurentHp(curentHp + healAmount);

		//回復音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		// HPがMAX_HPを超える場合はMAX_HPにクリップする
		if (plyer->getPlayerStatusSave().getcurentHp() > max_hp) {
			plyer->getPlayerStatusSave().SetPlayerCurentHp(max_hp);
		}

		// 選択したアイテムの ID と一致する要素を特定する
		auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
			[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

		// アイテムが見つかった場合は削除する
		if (itemToRemove != InventoryList.end()) {
			InventoryList.erase(itemToRemove);
			itemNum--;
		}

	}
	//HpがmaxHpと同じ場合処理を飛ばす
	else {
		return;
	}
}

//メニューの初期化
void Inventory::InitMenuWinodow()
{
	//最初のESCを押したときに出る最初のメニュー
	MenuWindow::MenuElement_t* first_menu_coment = new MenuWindow::MenuElement_t[]
	{
		{100, 100 , "道具", 0},
		{100, 150 , "特技", 1},
		{100, 200 , "強さ", 2},
		{100, 250 , "閉じる" ,3}
	};

	first_menu = std::make_shared<MenuWindow>(50, 50, 250, 250, "graphics/WindowBase_01.png", first_menu_coment, 4 , 1);
	first_menu->Open();

	//プレイヤーの選択してもらうメニュー
	MenuWindow::MenuElement_t* select_menu_coment = new MenuWindow::MenuElement_t[]
	{
		{ 100 , 100 , "はい" , 0},
		{ 100 , 150 , "いいえ" , 1}
	};

	select_action_menu = std::make_shared<MenuWindow>(50, 50, 250, 200, "graphics/WindowBase_01.png", select_menu_coment, 2, 1);

	//アイテムウィンドウ内で選択するメニュー
	MenuWindow::MenuElement_t* select_detail = new MenuWindow::MenuElement_t[]
	{
		{400 , 90 , "使う" , 0},
		{400 , 140 , "詳細" , 1},
		{400 , 190 , "装備する",2},
		{400 , 240 , "捨てる", 3},
		{400 , 290 , "閉じる" , 4}
	};

	select_detail_window = std::make_shared<MenuWindow>(350, 50, 250, 280, "graphics/WindowBase_01.png", select_detail, 5, 1);
	select_detail_window->Open();

	//メニューウィンドウ
	menu_window = std::make_shared<Menu>(50, 50, 300, 270, "graphics/WindowBase_01.png");
	item_coment_window = std::make_shared<Menu>(600, 50, 300, 200, "graphics/WindowBase_01.png");
	plyer_status_window = std::make_shared<Menu>(50, 50, 300, 400, "graphics/WindowBase_01.png");
	SkilldetailWinodow = std::make_shared<Menu>(350, 50, 300, 250, "graphics/WindowBase_01.png");
	ItemDetailWindow = std::make_shared<Menu>(600, 50, 400, 250, "graphics/WindowBase_01.png");

}

//一番最初のメニュー
void Inventory::First_Menu()
{
	first_menu->All();
}

//アイテムの表示
void Inventory::ItemMenu(const tnl::Vector2i& itemDrawPos, tnl::Vector2i& curentPageText, int CousourX , int itemParPage)
{
	int i = 0;
	int y = 34;
	int equipY = 30;


		//現在のページに基づいて表示するアイテムの範囲を計算
		start_index = currentPage * itemParPage;
		end_index = min((start_index + itemParPage) , InventoryList.size());

		for (int index = start_index; index < end_index; ++index) {
			//
			auto& InventoryData = InventoryList[index];
			//アイテムの名前を取得する
			std::string ItemName = InventoryData.getItemName();

			// アイテムの描画
			DrawStringEx(itemDrawPos.x, itemDrawPos.y + y * i, Color_White, "%s", ItemName.c_str());

			// カーソルを描画する位置を決定
			int cursorY = itemDrawPos.y + y * (selectCursor % itemParPage);
			ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CousourX, cursorY, cursorSize, 0, true);
		
			//装備できるアイテムだった場合
			if (InventoryData.getItemType() == 1 || InventoryData.getItemType() == 2) {
				// アイテムが装備されているか、選択されたアイテムかを確認
				if ((equipWeapon && InventoryData.getItemId() == equippedWeapon.back().getItemId())) {
					// 装備中のアイテムまたは選択されたアイテムの場合、"[E]" を表示
					DrawStringEx(itemDrawPos.x - 30, itemDrawPos.y + equipY * i, Color_White, "[E]");
				}
				if ((equipArmor && InventoryData.getItemId() == equippedArmor.back().getItemId())) {
					// 装備中のアイテムまたは選択されたアイテムの場合、"[E]" を表示
					DrawStringEx(itemDrawPos.x - 30 , itemDrawPos.y + equipY * i, Color_White, "[E]");
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
		DrawStringEx(curentPageText.x, curentPageText.y, Color_White , "%d / 4", currentPage + 1);

}

//プレイヤーのステータス表示
void Inventory::PlyStatusMenu()
{
	plyer_status_window->Menu_draw();

	DrawStringEx(150, 70, -1, "===強さ===");

	// Player クラスのポインターが有効かどうかを確認する
	if (plyer) {
		// Player クラスのポインターを介してステータスを取得する
		int hp = plyer->getPlayerStatusSave().getcurentHp();
		int mp = plyer->getPlayerStatusSave().getCurentMp();
		int Attack = plyer->getPlayerStatusSave().getAttack();
		int Defense = plyer->getPlayerStatusSave().getDefance();
		int Level = plyer->getPlayerStatusSave().getLevel();
		int Speed = plyer->getPlayerStatusSave().getSpeed();
		int R_expoint = plyer->getPlayerStatusSave().getExpoint();

		// 取得したステータスを描画する
		DrawStringEx(70, 100, Color_White, "HP : %d", hp);
		DrawStringEx(70, 150, Color_White, "MP : %d", mp);
		DrawStringEx(70, 200, Color_White, "攻撃力 : %d", Attack);
		DrawStringEx(70, 250, Color_White, "防御力 : %d", Defense);
		DrawStringEx(70, 300, Color_White, "レベル : %d", Level);
		DrawStringEx(70, 350, Color_White, "すばやさ : %d", Speed);
		DrawStringEx(70, 400, Color_White, "レベルアップに必要な経験値 : %d", R_expoint);
	}

}

void Inventory::ItemUseMenu()
{
	select_detail_window->All();
}

//インベントリの処理
void Inventory::swichInventoryUpdate(float deltatime)
{
	switch (select_menu)
	{
	case Inventory::MenuWindow_I::Empty:


		break;

		//最初のメニューでの処理
	case Inventory::MenuWindow_I::First_Menu:

		//plyerを動けないようにする
		plyer->setPlayerControl(PlyControlFalse);
			
		//1個目が選択された場合、アイテムを表示する
		if (first_menu->getSelectNum() == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ItemMenu;
			CursorReset();
			currentPage = 0;
			selectedIndex = 0;
		}
			//2個目が選択された場合、特技を表示する
		else if (first_menu->getSelectNum() == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::SkillMenu;

		}
		//3個目が選択された場合、プレイヤーの強さを表示する
		else if (first_menu->getSelectNum() == 2 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::StatusMenu;
		}
		//4個目が選択された場合、閉じる
		else if (first_menu->getSelectNum() == 3 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::Empty;
			//plyerを動けるようにする
			plyer->setPlayerControl(PlyControlTrue);
			}

		break;

		//アイテムメニューでの処理
	case Inventory::MenuWindow_I::ItemMenu:

		//アイテムのコメントを初期化する
		item_coment = ItemComent::Empty;

		//Enterキーを押したらItemUseMenuに変更する
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ItemUseMenu;
		}

		//インデックス操作
		ItemCurourIndex(item_per_page);
		

		CusorMove();

		break;

	//アイテム詳細メニューの場合
	case Inventory::MenuWindow_I::ItemDetailMenu:

		//左キーを押すとアイテム使用メニューに戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::ItemUseMenu;
		}

		break;

	case Inventory::MenuWindow_I::StatusMenu:

		break;

		//アイテム使用メニュー
	case Inventory::MenuWindow_I::ItemUseMenu:

		//使うが選択された時
		if (select_detail_window->getSelectNum() == 0) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				//30%のHpを回復させる(ポーションが選択された時)
				InventoryItemUse(selectedItemId);
			}
		}
		//詳細が選択された時
		else if (select_detail_window->getSelectNum() == 1) {

			//インベントリが空の時は処理をとばす
			if (InventoryList.empty()) {
				return;
			}
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ItemDetailMenu;
			}
		}
		//装備が選択された時
		else if (select_detail_window->getSelectNum() == 2) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				EquipWeapon(selectedIndex);
			}
		}
		//捨てるが選択された時
		else if (select_detail_window->getSelectNum() == 3) {
			
			//アイテムが空の場合処理をとばす
			if (InventoryList.empty()) {
				return;
			}
			//選択してEnterキーを押したら
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

				// 選択したアイテムの ID と一致する要素を特定する
				auto itemid = InventoryList[selectedIndex].getItemId();

				auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
					[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

				// アイテムが見つかった場合は削除する
				if (itemToRemove != InventoryList.end()) {
					InventoryList.erase(itemToRemove);
					itemNum--;
				}
			}
		}
		//閉じるが選択された時
		else if (select_detail_window->getSelectNum() == 4) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ItemMenu;
			}
		}
		
		break;

	//スキル表示画面
	case MenuWindow_I::SkillMenu:

		//Enterキーを押したらItemUseMenuに変更する
		if (SkillNum > 0) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//決定音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::SkillDatailMenu;
			}
		}

		//スキルのカーソル移動時のインデックス操作
		SkillCurourIndex();


		//特技のカーソル移動
		SkillCousorMove();

		break;

	//スキル説明
	case MenuWindow_I::SkillDatailMenu:

		//スキル選択に戻す
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::SkillMenu;
		}

		break;


	default:
		break;
	}

}

//装備時のステータスを取得する
int Inventory::getEquipStatus(int type)
{
	if (type == 0) {
		return equipAttack;
	}
	if (type == 1) {
		return equipDefance;
	}
}

//スキルを追加する
void Inventory::AddSkill(Shared<Skill>skill)
{
	SkillList.push_back(skill);
	SkillNum++;

}

//スキルの描画
void Inventory::InventorySkill()
{
	// ウィンドウの表示
	menu_window->Menu_draw();

	int i = 0;
	int y = 34;

	//1ページ当たりのアイテム数
	item_per_page = 5;

	//現在のページに基づいて表示するアイテムの範囲を計算
	start_index = SkillCurentPage * item_per_page;
	end_index = min((start_index + item_per_page), SkillList.size());

	for (int index = start_index; index < end_index; ++index) {
		//
		auto& SkillData = SkillList[index];
		//アイテムの名前を取得する
		string skillName = SkillData->getName();

		// アイテムの描画
		DrawStringEx(DrawPos.x, DrawPos.y + y * i, Color_White, "%s", skillName.c_str());

		// カーソルを描画する位置を決定
		int cursorY = DrawPos.y + y * (SkillCousour % item_per_page);
		ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", cursorX, cursorY, cursorSize, 0, true);

		++i;

	}

	////ページの表示
	DrawStringEx(CurentPageText.x, CurentPageText.y, Color_White, "%d / 4", SkillCurentPage + 1);

}

//スキル説明の描画
void Inventory::SkillDetailDraw(int skill_index)
{

	//特技説明
	SkilldetailWinodow->Menu_draw();

	DrawStringEx(SkilldetailWinodow->menu_x + 50, SkilldetailWinodow->menu_y + 20, Color_White, "==スキル説明==");

	//スキルデータを取得
	auto& skillData = SkillList[skill_index];

	//スキルデータのスキル説明を取得
	string skillDatail = skillData->getDescription();
	
	//スキル説明を描画
	DrawStringEx(SkilldetailWinodow->menu_x + 30, SkilldetailWinodow->menu_y + 50, Color_White, "%s", skillDatail.c_str());

}

//スキルのカーソル操作
void Inventory::SkillCousorMove()
{

	//スキルがない場合、何もしない
	if (SkillNum == 0)
	{
		return;
	}
	//カーソルが一番上にいる場合カーソルをさらに上に押してもカーソルを動かさないようにする
	if (SkillCousour == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP))
	{
		return;
	}
	//カーソルの動き
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		SkillNum = SkillList.size();
		SkillCousour = (SkillCousour + (SkillNum - 1)) % SkillNum;

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		SkillNum = SkillList.size();
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// カーソルが最後の要素よりも下に移動しようとする場合は、何もせずに処理を終了する
		if (SkillCousour == SkillNum - 1) {
			return;
		}

		SkillCousour = (SkillCousour + 1) % SkillNum;
	}

}

//スキルカーソルの移動時のインデックス操作
void Inventory::SkillCurourIndex()
{
	// 上キーが押されたときの処理
	if (SkillCurentPage == 0 && selectedSkillIndex == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// 何もしない
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selectedSkillIndex % item_per_page == 0) {
			// カーソルがページ内の最上部にいる場合は、一つ上のページの最後の要素を選択
			if (SkillCurentPage > 0) {
				--SkillCurentPage;
				// インデックスを更新してページの最後の要素を選択する
				selectedSkillIndex = (SkillCurentPage + 1) * item_per_page - 1;
			}
		}
		else {
			// それ以外の場合は、一つ上の要素を選択
			--selectedSkillIndex;
		}
	}

	// 下キーが押されたときの処理
	if (selectedSkillIndex == SkillList.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		// 何もしない
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selectedSkillIndex + 1) % item_per_page == 0) {
			// カーソルがページ内の最下部にいる場合は、一つ下のページの最初の要素を選択
			// スキルが持てるインベントリは 20 で考えている為 4 ページ分まで開けるようにする
			if (SkillCurentPage < 4) {
				++SkillCurentPage;
				// インデックスを更新してページの最初の要素を選択する
				selectedSkillIndex = currentPage * item_per_page;
			}
		}
		else {
			// それ以外の場合は、一つ下の要素を選択
			++selectedSkillIndex;
		}
	}
}

//スキルをセットする
void Inventory::SkillSet()
{
	switch (plyer->getPlayerStatusSave().getLevel())
	{
	//レベル1の時は何も覚えない
	case 1:
		break;
	//レベル2の時に火炎斬りを覚える
	case 2:
		AddSkill(std::make_shared<FlameSlash>());

		//ログを流す
		battleLog->addLog("火炎斬りを覚えた!");

		break;
	//レベル3の時にヒールを覚える
	case 3:
		AddSkill(std::make_shared<Heal>());

		battleLog->addLog("ヒールを覚えた!");

		break;
	//レベル4の時にアイスブラストを覚える
	case 4:
		AddSkill(std::make_shared<IceBlast>());

		battleLog->addLog("アイスブラストを覚えた!");

		break;
	//レベル5の時にサンダーボルトを覚える
	case 5:
		AddSkill(std::make_shared<ThunderBolt>());

		battleLog->addLog("サンダーボルトを覚えた!");

	default:
		break;
	}
}

//スキルを追加する系のアイテム
template <class T>
void Inventory::AddSkillItem(int itemid ,Shared<T>skill)
{
	//---スキルを追加する---//

	// 選択したアイテムの ID と一致する要素を特定する
	auto SkillToRemove = std::find_if(SkillList.begin(), SkillList.end(),
		[itemid](const Shared<Skill>& skill) { return skill->getId() == itemid; });

	// アイテムが見つかった場合は処理をとばす
	if (SkillToRemove != SkillList.end()) {
		return;
	}

	// 見つからなかった場合スキルを追加する
	AddSkill(std::make_shared<T>());

	//---アイテムを消去する---//

	// 選択したアイテムの ID と一致する要素を特定する
	auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
		[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

	// アイテムが見つかった場合は削除する
	if (itemToRemove != InventoryList.end()) {
		InventoryList.erase(itemToRemove);
		itemNum--;
	}
}

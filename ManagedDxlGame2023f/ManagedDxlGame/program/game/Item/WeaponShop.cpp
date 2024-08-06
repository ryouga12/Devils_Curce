#include "WeaponShop.h"
#include"../Manager/UiManager.h"
#include"../Manager/GameManager.h"


WeaponShop::WeaponShop(const InMapScene::InMapState& curent_inmap_scene)
{
	item_ = std::make_shared<Item>();

	//武器屋のラインナップを決める
	WeaponShopInit(curent_inmap_scene);
}

WeaponShop::~WeaponShop()
{
}

void WeaponShop::WeaponShopInit(const InMapScene::InMapState& curent_inmap_scene)
{
	weponList.clear();
	
	switch (curent_inmap_scene)
	{
	case InMapScene::InMapState::VILLAGE:

		//一個目の店

		//木の棒
		weponList.emplace_back(item_->GetItemById(6));
		//木の剣
		weponList.emplace_back(item_->GetItemById(4));
		//木のハンマー
		weponList.emplace_back(item_->GetItemById(5));
		//革の盾
		weponList.emplace_back(item_->GetItemById(2));
		//ポーション
		weponList.emplace_back(item_->GetItemById(3));

		break;

	case InMapScene::InMapState::TOWN:

		//冒険者の刀
		weponList.emplace_back(item_->GetItemById(10));
		//銅の剣
		weponList.emplace_back(item_->GetItemById(0));
		//革の鎧
		weponList.emplace_back(item_->GetItemById(1));
		//冒険者の鎧
		weponList.emplace_back(item_->GetItemById(13));
		//ハイポーション
		weponList.emplace_back(item_->GetItemById(8));

		break;

	//城下町
	case InMapScene::InMapState::CASTLETOWN:

		//甲羅の鎧
		weponList.emplace_back(item_->GetItemById(7));
		//プラントエキス(回復アイテム)
		weponList.emplace_back(item_->GetItemById(17));
		//鋼の剣
		weponList.emplace_back(item_->GetItemById(12));
		//毛皮のコート
		weponList.emplace_back(item_->GetItemById(28));
		//鉄のハンマー
		weponList.emplace_back(item_->GetItemById(34));


		break;

	default:
		break;
	}


	//値段が低い順にソート
	std::sort(weponList.begin(), weponList.end(), [](const ItemBase& left, const ItemBase& right) {
		return left.GetItemPrice() < right.GetItemPrice();
		});
}


void WeaponShop::Draw()
{
	//メニューの表示
	UIManager::getUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);

	DrawString(WEAPON_SHOP_STRING_POS.x, WEAPON_SHOP_STRING_POS.y, "===== 武器屋 =====", koni::Color::WHITE);

	for (int i = 0; i < weponList.size(); ++i) {
		DrawStringEx(WEAPON_CONNECTION_POS.x, WEAPON_CONNECTION_POS.y + i * SPACING, koni::Color::WHITE, "%d. %s \n(威力: %d, 防御力: %d,価格: %d)", i + 1, weponList[i].GetItemName().c_str(), weponList[i].GetItemDamage(), weponList[i].GetItemDefance(), weponList[i].GetItemPrice());
	}
	DrawString(PROMPT_POS.x, PROMPT_POS.y, "購入したい武器の番号を\n選択してください(0: 終了)", koni::Color::WHITE);


	//武器商人のコメント
	if (buyWeapon == Buy_Weapon::BUY) {
		DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "「毎度あり!ありがとね！」", koni::Color::WHITE);
	}
	else if (buyWeapon == Buy_Weapon::NO) {
		DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "「お金が足りないよ!出直してきな!」", koni::Color::WHITE);
	}

	//カーソルの描画
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CURSOR_POS.x, CURSOR_POS.y + select_cousor * SPACING, CURSOR_SIZE, koni::Color::BLACK, true);

}

//カーソルの動き
void WeaponShop::WeaponShopCursorMove()
{
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		weapon_num = static_cast<int>(weponList.size());
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3" , DX_PLAYTYPE_BACK);
		select_cousor = (select_cousor + (weapon_num - 1)) % weapon_num;
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		weapon_num = static_cast<int>(weponList.size());
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		select_cousor = (select_cousor + 1) % weapon_num;
	}
}


void WeaponShop::BuyWeapon(const Shared<Player>& player)
{
	//インベントリを取得する
	auto& inventory = GameManager::getGameManager()->getInventory();

	if (select_cousor == MenuWindow::Elements::FIRST_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[0].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[0].GetItemPrice());
			inventory->AddInventory(weponList[0].GetItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else
		{
			buyWeapon = Buy_Weapon::NO;
		}

	}
	if (select_cousor == MenuWindow::Elements::SECOND_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[1].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[1].GetItemPrice());
			inventory->AddInventory(weponList[1].GetItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else {
			buyWeapon = Buy_Weapon::NO;
		}

	}
	if (select_cousor == MenuWindow::Elements::THERD_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[2].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[2].GetItemPrice());
			inventory->AddInventory(weponList[2].GetItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else {
			buyWeapon = Buy_Weapon::NO;
		}
	}
	if (select_cousor == MenuWindow::Elements::FOUR_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[3].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[3].GetItemPrice());
			inventory->AddInventory(weponList[3].GetItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else {
			buyWeapon = Buy_Weapon::NO;
		}
	}
	if (select_cousor == MenuWindow::Elements::FIVE_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[4].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[4].GetItemPrice());
			inventory->AddInventory(weponList[4].GetItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else {
			buyWeapon = Buy_Weapon::NO;
		}
	}
}

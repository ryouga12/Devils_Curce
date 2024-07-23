#include "WeaponShop.h"
#include"../Manager/UiManager.h"
#include"../Manager/GameManager.h"


WeaponShop::WeaponShop(const InMapScene::InMapState& curent_inmap_scene)
{
	menuWindow = UIManager::getUIManager()->getMenu("menu_window");

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

	//城
	//今後追加予定
	case InMapScene::InMapState::CASTLE:

		break;

	default:
		break;
	}


	//値段が低い順にソート
	std::sort(weponList.begin(), weponList.end(), [](const ItemBase& left, const ItemBase& right) {
		return left.getItemPrice() < right.getItemPrice();
		});
}


void WeaponShop::Update(float delta_time)
{
	
}

void WeaponShop::Draw()
{

	//メニューの表示
	menuWindow.lock()->Menu_draw(MENU_WINDOW_POS.x,MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);

	DrawString(WEAPON_SHOP_STRING_POS.x, WEAPON_SHOP_STRING_POS.y, "===== 武器屋 =====", COLOR_WHITE);

	for (int i = 0; i < weponList.size(); ++i) {
		DrawStringEx(WEAPON_CONNECTION_POS.x, WEAPON_CONNECTION_POS.y + i * SPACING, -1, "%d. %s \n(威力: %d, 防御力: %d,価格: %d)", i + 1, weponList[i].getItemName().c_str(), weponList[i].getItemDamage(), weponList[i].getItemDefance(), weponList[i].getItemPrice());
	}
	DrawString(PROMPT_POS.x, PROMPT_POS.y, "購入したい武器の番号を\n選択してください(0: 終了)", COLOR_WHITE);


	//武器商人のコメント
	if (buyWeapon == Buy_Weapon::BUY) {
		DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "「毎度あり!ありがとね！」", COLOR_WHITE);
	}
	else if (buyWeapon == Buy_Weapon::NO) {
		DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "「お金が足りないよ!出直してきな!」", COLOR_WHITE);
	}

	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CURSOR_POS.x, CURSOR_POS.y + select_cousor * SPACING, CURSOR_SIZE, 0, true);
	//DrawStringEx(100, 700, -1, "%d", weponList.size());


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

	if (select_cousor == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[0].getItemPrice()) {
			player->ReducePlayerMoney(weponList[0].getItemPrice());
			inventory->AddInventory(weponList[0].getItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else
		{
			buyWeapon = Buy_Weapon::NO;
		}

	}
	if (select_cousor == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[1].getItemPrice()) {
			player->ReducePlayerMoney(weponList[1].getItemPrice());
			inventory->AddInventory(weponList[1].getItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else {
			buyWeapon = Buy_Weapon::NO;
		}

	}
	if (select_cousor == 2 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[2].getItemPrice()) {
			player->ReducePlayerMoney(weponList[2].getItemPrice());
			inventory->AddInventory(weponList[2].getItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else {
			buyWeapon = Buy_Weapon::NO;
		}
	}
	if (select_cousor == 3 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[3].getItemPrice()) {
			player->ReducePlayerMoney(weponList[3].getItemPrice());
			inventory->AddInventory(weponList[3].getItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else {
			buyWeapon = Buy_Weapon::NO;
		}
	}
	if (select_cousor == 4 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weponList[4].getItemPrice()) {
			player->ReducePlayerMoney(weponList[4].getItemPrice());
			inventory->AddInventory(weponList[4].getItemId());
			buyWeapon = Buy_Weapon::BUY;
		}
		else {
			buyWeapon = Buy_Weapon::NO;
		}
	}
}

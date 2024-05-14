#include "WeaponShop.h"

WeaponShop::WeaponShop()
{
	weapon_ = std::make_shared<Weapon>();
	// inventory = std::make_shared<Inventory>();
	myplayer = std::make_shared<Player>();
	/*sound = std::make_shared<SoundManager>();*/
	menuWindow = std::make_shared<Menu>(50, 50, 350, 400, "graphics/WindowBase_01.png");

}

WeaponShop::~WeaponShop()
{
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/cousour_bgm.mp3");
}

void WeaponShop::Update(float delta_time)
{
	
}

void WeaponShop::Draw()
{
	//メニューの表示
	menuWindow->Menu_draw();

	//武器屋関連の表示
	DrawString(60, 520, "武器屋", -1);

	DrawString(110, 70, "===== 武器屋 =====", -1);

	for (int i = 0; i < weapon_->GetWeaponValue().size(); ++i) {
		DrawStringEx(85, 130 + i * 40, -1, "%d. %s \n(威力: %d, 防御力: %d,価格: %d)", i + 1, weapon_->GetWeaponValue()[i].getItemName().c_str(), weapon_->GetWeaponValue()[i].getItemDamage(), weapon_->GetWeaponValue()[i].getItemDefance(), weapon_->GetWeaponValue()[i].getItemPrice());
	}
	DrawString(85, 380, "購入したい武器の番号を\n選択してください(0: 終了)", -1);


	if (buyWeapon == B_Weapon::buy) {
		DrawString(150, 620, "「毎度あり!ありがとね！」", -1);
	}
	else if (buyWeapon == B_Weapon::no) {
		DrawString(150, 620, "「お金が足りないよ!出直してきな!」", -1);
	}

	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", 70, 140 + SelectCousor * 40, 0.3f, 0, true);
	DrawStringEx(100, 700, -1, "%d", weapon_->GetWeaponListCount());


}

//カーソルの動き
void WeaponShop::WeaponShopCursorMove()
{
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		WeaponNum = weapon_->GetWeaponListCount();
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3" , DX_PLAYTYPE_BACK);
		SelectCousor = (SelectCousor + (WeaponNum - 1)) % WeaponNum;
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		WeaponNum = weapon_->GetWeaponListCount();
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		SelectCousor = (SelectCousor + 1) % WeaponNum;
	}

	

}

void WeaponShop::BuyWeapon(const std::shared_ptr<Player>& player)
{
	if (SelectCousor == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == 20) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weapon_->GetWeaponValue()[0].getItemPrice()) {
			player->ReducePlayerMoney(weapon_->GetWeaponValue()[0].getItemPrice());
			inventory->AddInventory(weapon_->GetWeaponValue()[0].getItemId());
			buyWeapon = B_Weapon::buy;
		}
		else
		{
			buyWeapon = B_Weapon::no;
		}

	}
	if (SelectCousor == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == 20) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weapon_->GetWeaponValue()[1].getItemPrice()) {
			player->ReducePlayerMoney(weapon_->GetWeaponValue()[1].getItemPrice());
			inventory->AddInventory(weapon_->GetWeaponValue()[1].getItemId());
			buyWeapon = B_Weapon::buy;
		}
		else {
			buyWeapon = B_Weapon::no;
		}

	}
	if (SelectCousor == 2 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == 20) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weapon_->GetWeaponValue()[2].getItemPrice()) {
			player->ReducePlayerMoney(weapon_->GetWeaponValue()[2].getItemPrice());
			inventory->AddInventory(weapon_->GetWeaponValue()[2].getItemId());
			buyWeapon = B_Weapon::buy;
		}
		else {
			buyWeapon = B_Weapon::no;
		}
	}
	if (SelectCousor == 3 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == 20) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weapon_->GetWeaponValue()[3].getItemPrice()) {
			player->ReducePlayerMoney(weapon_->GetWeaponValue()[3].getItemPrice());
			inventory->AddInventory(weapon_->GetWeaponValue()[3].getItemId());
			buyWeapon = B_Weapon::buy;
		}
		else {
			buyWeapon = B_Weapon::no;
		}
	}
	if (SelectCousor == 4 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == 20) return;
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->getPlayerMoney() >= weapon_->GetWeaponValue()[4].getItemPrice()) {
			player->ReducePlayerMoney(weapon_->GetWeaponValue()[4].getItemPrice());
			inventory->AddInventory(weapon_->GetWeaponValue()[4].getItemId());
			buyWeapon = B_Weapon::buy;
		}
		else {
			buyWeapon = B_Weapon::no;
		}
	}
}

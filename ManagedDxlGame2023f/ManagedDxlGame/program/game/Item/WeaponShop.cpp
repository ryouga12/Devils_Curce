#include "WeaponShop.h"
#include"../Manager/UiManager.h"
#include"../Manager/GameManager.h"


WeaponShop::WeaponShop(const InMapScene::InMapState& curent_inmap_scene)
{
	item_ = std::make_shared<Item>();

	//武器屋のラインナップを決める
	WeaponShopInit(curent_inmap_scene);

	//売却確認コマンド
	if (!sell_coment) {

		//---座標---//

		//売却
		const tnl::Vector2i SELL_STRING_POS = { 350 , 100 };

		//やめる
		const tnl::Vector2i CLOASE_STRING_POS = { 350 , 150 };

		sell_coment = new MenuWindow::MenuElement_t[]{
			{SELL_STRING_POS.x ,   SELL_STRING_POS.y ,   "売却する" ,	MenuWindow::Elements::FIRST_ELEMENT},
			{CLOASE_STRING_POS.x , CLOASE_STRING_POS.y , "やめる"   ,   MenuWindow::Elements::SECOND_ELEMENT}
		};
	}

	if (!sell_coment_select) {

		//売却確認コマンド
		const int SELL_COMENT_NUM = 2;

		sell_coment_select = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", sell_coment, SELL_COMENT_NUM);
		sell_coment_select->Open();
	}
}

//更新処理
void WeaponShop::Update(const float delta_time)
{
	//シーケンスの更新処理
	tnl_sequence_.update(delta_time);
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

//待機状態(主に購入なのか、売却なのかで処理を切り替える)
bool WeaponShop::seqIdle(float delta_time)
{
	//購入が選択されたら購入シーケンスに移行させる
	if (curent_transaction_type == TransactionType::BUY) {
		tnl_sequence_.change(&WeaponShop::seqBuy);
	}
	//売るが選択されたら売却シーケンスに移行させる
	else if (curent_transaction_type == TransactionType::SELL) {
		tnl_sequence_.change(&WeaponShop::seqSell);
	}
	else {

		tnl::DebugTrace("\n------------------------------------------------------------");
		tnl::DebugTrace("\n シーケンスを切り替えませんでした。");
		tnl::DebugTrace("\n------------------------------------------------------------");

	}


	return false;
}

//購入シーケンス
bool WeaponShop::seqBuy(float delta_time)
{
	//武器屋のカーソル操作
	WeaponShopCursorMove();
	//購入処理
	BuyWeapon();

	return false;
}

//売却シーケンス
bool WeaponShop::seqSell(float delta_time)
{
	//売却処理
	SellItemProcess();

	return false;
}


void WeaponShop::Draw()
{
	//購入が選択された場合
	if (curent_transaction_type == TransactionType::BUY) {

		//メニューの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);

		DrawString(WEAPON_SHOP_STRING_POS.x, WEAPON_SHOP_STRING_POS.y, "===== 武器屋 =====", koni::Color::WHITE);

		for (int i = 0; i < weponList.size(); ++i) {
			DrawStringEx(WEAPON_CONNECTION_POS.x, WEAPON_CONNECTION_POS.y + i * SPACING, koni::Color::WHITE, "%d. %s \n(威力: %d, 防御力: %d,価格: %d)", i + 1, weponList[i].GetItemName().c_str(), weponList[i].GetItemDamage(), weponList[i].GetItemDefance(), weponList[i].GetItemPrice());
		}
		DrawString(PROMPT_POS.x, PROMPT_POS.y, "購入したい武器を\n選択してください\n(back space : 閉じる)", koni::Color::WHITE);

		//なにか買うときのコメント
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, koni::Color::WHITE, "「 何を買うんだい 」");

		//武器商人のコメント
		if (curent_weapon_shop_coment == WeaponshopComent::BUY) {
			DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "「毎度あり!ありがとね！」", koni::Color::WHITE);
		}
		else if (curent_weapon_shop_coment == WeaponshopComent::NO) {
			DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "「お金が足りないよ!出直してきな!」", koni::Color::WHITE);
		}

		//カーソルの描画
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CURSOR_POS.x, CURSOR_POS.y + select_cousor * SPACING, CURSOR_SIZE, koni::Color::BLACK, true);
	}
	//売却が選択された場合
	else if (curent_transaction_type == TransactionType::SELL) {

		//インベントリのアイテムを表示するウィンドウの座標
		const tnl::Vector2i ITEM_WINDOW_POS = { 50 ,50 };

		//インベントリのアイテムを表示するウィンドウのサイズ
		const tnl::Vector2i ITEM_WINDOW_SIZE = { 280, 400 };

		//なにか買うときのコメント
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, koni::Color::WHITE, "「 何を売るんだい 」");

		//アイテム選択画面の場合
		if (curent_sell_action == SellAction::SELLITEMSELECT) {

			//ウィンドウを表示する
			UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_WINDOW_POS.x, ITEM_WINDOW_POS.y, ITEM_WINDOW_SIZE.x, ITEM_WINDOW_SIZE.y);

			//アイテムを表示する
			GameManager::GetGameManager()->GetInventory()->ItemMenu(ITEM_DRAW_POS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE_);

		}
		//アイテムの売却確認ウィンドウの場合
		else if (curent_sell_action == SellAction::SELLITEMCHECK) {

			//売却選択コマンドのウィンドウの座標
			const tnl::Vector2i SELL_COMENT_WINDOW_POS = { 300 , 50 };

			//売却選択コマンドのウィンドウのサイズ
			const tnl::Vector2i SELL_COMENT_WINDOW_SIZE = { 200 , 250 };

			//売却価格を表示する座標
			const tnl::Vector2i SELL_PRICE_STRING_POS = { 330, 220 };

			//インベントリ
			auto& Inventory = GameManager::GetGameManager()->GetInventory();

			//ウィンドウを表示する
			UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_WINDOW_POS.x, ITEM_WINDOW_POS.y, ITEM_WINDOW_SIZE.x, ITEM_WINDOW_SIZE.y);

			//アイテムを表示する
			Inventory->ItemMenu(ITEM_DRAW_POS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE_);

			//売却選択コマンドの表示を行う
			sell_coment_select->All(SELL_COMENT_WINDOW_POS.x, SELL_COMENT_WINDOW_POS.y, SELL_COMENT_WINDOW_SIZE.x, SELL_COMENT_WINDOW_SIZE.y);

			//インベントリが空じゃ無ければ
			if (!GameManager::GetGameManager()->GetInventory()->GetInventoryList().empty()) {

				//売却価格を半分の価格に設定する
				auto sell_price = Inventory->GetInventoryList()[Inventory->GetSelectedIndex()].GetItemPrice() / koni::Numeric::DIVIDER_TWO;

				//売却時の値段表示
				DrawStringEx(SELL_PRICE_STRING_POS.x, SELL_PRICE_STRING_POS.y, koni::Color::WHITE, "売却価格 : %dG", sell_price);
			}
		}

		//武器商人のコメント
		if (curent_weapon_shop_coment == WeaponshopComent::SELL) {
			DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "「毎度あり!ありがとね！」", koni::Color::WHITE);
		}

		DrawString(PROMPT_POS.x, PROMPT_POS.y, "売却するアイテムを\n選択してください\n(back space : 閉じる)", koni::Color::WHITE);
	}


}

//カーソルの動き
void WeaponShop::WeaponShopCursorMove()
{
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		weapon_num = static_cast<int>(weponList.size());
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3" , DX_PLAYTYPE_BACK);
		select_cousor = (select_cousor + (weapon_num - 1)) % weapon_num;
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		weapon_num = static_cast<int>(weponList.size());
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		select_cousor = (select_cousor + 1) % weapon_num;
	}
}


void WeaponShop::BuyWeapon()
{
	//インベントリを取得する
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//プレイヤーを取得する
	auto& player = GameManager::GetGameManager()->GetPlayer();

	if (select_cousor == MenuWindow::Elements::FIRST_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->GetPlayerMoney() >= weponList[0].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[0].GetItemPrice());
			inventory->AddInventory(weponList[0].GetItemId());
			curent_weapon_shop_coment = WeaponshopComent::BUY;
		}
		else
		{
			curent_weapon_shop_coment = WeaponshopComent::NO;
		}

	}
	if (select_cousor == MenuWindow::Elements::SECOND_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->GetPlayerMoney() >= weponList[1].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[1].GetItemPrice());
			inventory->AddInventory(weponList[1].GetItemId());
			curent_weapon_shop_coment = WeaponshopComent::BUY;
		}
		else {
			curent_weapon_shop_coment = WeaponshopComent::NO;
		}

	}
	if (select_cousor == MenuWindow::Elements::THERD_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->GetPlayerMoney() >= weponList[2].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[2].GetItemPrice());
			inventory->AddInventory(weponList[2].GetItemId());
			curent_weapon_shop_coment = WeaponshopComent::BUY;
		}
		else {
			curent_weapon_shop_coment = WeaponshopComent::NO;
		}
	}
	if (select_cousor == MenuWindow::Elements::FOUR_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->GetPlayerMoney() >= weponList[3].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[3].GetItemPrice());
			inventory->AddInventory(weponList[3].GetItemId());
			curent_weapon_shop_coment = WeaponshopComent::BUY;
		}
		else {
			curent_weapon_shop_coment = WeaponshopComent::NO;
		}
	}
	if (select_cousor == MenuWindow::Elements::FIVE_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//決定音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		if (player->GetPlayerMoney() >= weponList[4].GetItemPrice()) {
			player->ReducePlayerMoney(weponList[4].GetItemPrice());
			inventory->AddInventory(weponList[4].GetItemId());
			curent_weapon_shop_coment = WeaponshopComent::BUY;
		}
		else {
			curent_weapon_shop_coment = WeaponshopComent::NO;
		}
	}
}

//武器やアイテムを売却する
void WeaponShop::SellItemProcess()
{
	//プレイヤーを取得する
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//インベントリを取得する
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//売却の選択
	if (curent_sell_action == SellAction::SELLITEMCHECK) {

		//売却が選択されたら
		if (sell_coment_select->GetSelectNum() == MenuWindow::FIRST_ELEMENT
			&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//インベントリが空でないかつ、アイテムが売却可能であれば
			if (!inventory->GetInventoryList().empty() && inventory->GetInventoryList()[inventory->GetSelectedIndex()].GetEssentialItemsFlag()) {

				// 選択されたアイテムを参照
				auto& selectedItem = inventory->GetInventoryList()[inventory->GetSelectedIndex()];

				// 売却価格を半分に設定し、所持金に追加
				player->AddPlayerMoney(selectedItem.GetItemPrice() / koni::Numeric::DIVIDER_TWO);

				//もし装備している武器を売却した時の処理
				inventory->DeleteEquipItemProcess();

				// インベントリからアイテムを削除
				inventory->GetInventoryList().erase(inventory->GetInventoryList().begin() + inventory->GetSelectedIndex());

				//カーソルとインデックスをリセットする
				inventory->CursorReset();
				inventory->CurentPageReset();
				inventory->SelectedIndexClear();

				//武器屋のコメントを切り替える
				curent_weapon_shop_coment = WeaponshopComent::SELL;

				//状態をアイテムの選択画面に戻す
				curent_sell_action = SellAction::SELLITEMSELECT;
			}
		}
		else if (sell_coment_select->GetSelectNum() == MenuWindow::SECOND_ELEMENT
			&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//アイテムの選択画面に戻す
			curent_sell_action = SellAction::SELLITEMSELECT;
		}
	}
	//アイテムを選択する場合
	else if (curent_sell_action == SellAction::SELLITEMSELECT) {

		//インデックスの操作と取得
		GameManager::GetGameManager()->GetInventory()->ItemCurourIndex(ITEMPERPAGE_);

		//アイテムのカーソル処理
		GameManager::GetGameManager()->GetInventory()->CusorMove();

		//アイテムを選択状態でEnterキーを押したら確認ウィンドウに移動させる
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			curent_sell_action = SellAction::SELLITEMCHECK;
		}
	}
	
}

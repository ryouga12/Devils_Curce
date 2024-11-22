#include "Item.h"
#include <vector>
#include <algorithm>
#include"../Manager/GameManager.h"
#include"../Skill/Skill.h"
#include"../Menu/BattleLog.h"
#include"../Manager/CsvManager.h"
#include"../Manager/UiManager.h"
#include"../System/Camera.h"
#include"../Manager/ResourceManager.h"

Item::Item()
{
	//アイテムのロード
	ItemLoadTypeInfo();
}

void Item::ItemLoadTypeInfo()
{
	int max_num = static_cast<int>(CsvManager::GetCsvManager()->GetItemCsv().size());

	item_status_total_num = max_num;

	for (int y = 1; y < max_num; y++) {
		ItemBase itemStatus_info;
		
		//csv
		auto m_itemStatus_Csv_Info = CsvManager::GetCsvManager()->GetItemCsv();

		//csvからnameの取得
		itemStatus_info.SetItemName(m_itemStatus_Csv_Info[y][0].getString());
		//csvからidの取得
		itemStatus_info.SetItemId(m_itemStatus_Csv_Info[y][1].getInt());
		//csvからtypeの取得
		itemStatus_info.SetItemType(m_itemStatus_Csv_Info[y][2].getInt());
		//csvからDamageの取得
		itemStatus_info.SetItemDamage(m_itemStatus_Csv_Info[y][3].getInt());
		//csvからEpicを取得する
		itemStatus_info.SetItemEpic(m_itemStatus_Csv_Info[y][4].getInt());
		//csvからdefanceの取得
		itemStatus_info.SetItemDefance(m_itemStatus_Csv_Info[y][5].getInt());
		//csvからpriceを取得
		itemStatus_info.SetItemPrice(m_itemStatus_Csv_Info[y][6].getInt());
		//csvからWeaponTypeを取得する
		itemStatus_info.SetWeaponType(m_itemStatus_Csv_Info[y][7].getInt());
		//csvからItemDetailを取得する
		itemStatus_info.SetItemDetail(m_itemStatus_Csv_Info[y][8].getString());
		//csvから武器の補正をかけるモンスターの種族の取得する
		itemStatus_info.SetEpicCorrection(m_itemStatus_Csv_Info[y][9].getString());
		//csvから特別アイテムかの情報を取得する
		itemStatus_info.SetEssentialItemsFlag(m_itemStatus_Csv_Info[y][10].getInt());
		//csvからアイコンを取得する
		itemStatus_info.SetItemIcon(m_itemStatus_Csv_Info[y][11].getString());

		item_status_type.emplace_back(itemStatus_info);
	}

	//ダメージ順にソート
	item_status_type.sort([](const Item::ItemBase& left, const Item::ItemBase& right){
		return left.GetItemDamage() > right.GetItemDamage();
	});

}

ItemBase Item::GetItemById(const int& id) const
{
	auto it = std::find_if(item_status_type.begin(), item_status_type.end(),
		[id](const ItemBase& item) { return item.GetItemId() == id; });

	if (it != item_status_type.end()) {
		return *it;
	}
	else {
		
		return ItemBase{};
	}
}

//アイテムを使用した時の処理
bool Item::ItemUse(const int& itemid)
{

	//回復量(HPを20回復させる)
	const int PERCENTAGE = 20;

	//回復量(Hpを40回復させる)
	const int HI_PERCENTAGE = 40;

	//プレイヤーの攻撃力の半分の値をプラスさせる(調整対象)
	const float ATTACK_PERCENTAGE = 0.5f;

	//回復量(HPを60回復させる)
	const int PLANT_PERCENTAGE = 60;

	//プレイヤーの攻撃力を取得する
	auto PlayerAttack = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetAttack();

	//プレイヤーのMpを取得する
	auto curentMp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetCurentMp();

	//プレイヤーの最大Mpを取得する
	auto max_mp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxMp();

	//ゲームマネージャーのインスタンス
	auto game_manager = GameManager::GetGameManager();

	switch (itemid)
	{

	//ポーション
	case static_cast<int>(Item::UsedItem::PORTION):

		//バトルシーンのみでログを流す
		//バトルシーンでかつ効果が出来ようできた場合
		if (BattleFlag && ItemHpHeal(PERCENTAGE, itemid)) {

			//ログを流す
			battle_log_->AddRecoveryLog("Player", GetItemById(3).GetItemName().c_str(), healAmount);

			return true;
		}
		//バトルシーンの際に効果が無効だった場合
		else if (BattleFlag && !ItemHpHeal(PERCENTAGE, itemid)) {

			return false;
		}
		//バトルシーン以外で使ってかつ、回復出来たら
		else if(!BattleFlag && ItemHpHeal(PERCENTAGE, itemid)) {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("HPを20回復した！");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;

		}
		//バトルシーン以外でかつ、回復できなかった場合
		else if (!BattleFlag && !ItemHpHeal(PERCENTAGE, itemid)) {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("今使っても効果がありません");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return false;
		}

		break;

	//ハイポーション
	case static_cast<int>(Item::UsedItem::HIPORTION):

		//バトルシーンのみでログを流す
		//バトルシーンでかつ効果が出来ようできた場合
		if (BattleFlag && ItemHpHeal(HI_PERCENTAGE, itemid)) {

			//ログを流す
			battle_log_->AddRecoveryLog("Player", GetItemById(8).GetItemName().c_str(), healAmount);

			return true;
		}
		//バトルシーンの際に効果が無効だった場合
		else if (BattleFlag && !ItemHpHeal(HI_PERCENTAGE, itemid)) {

			return false;
		}
		//バトルシーン以外で使ってかつ、回復出来たら
		else if (!BattleFlag && ItemHpHeal(HI_PERCENTAGE, itemid)) {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("HPを40回復した！");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;

		}
		//バトルシーン以外でかつ、回復できなかった場合
		else if (!BattleFlag && !ItemHpHeal(HI_PERCENTAGE, itemid)) {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("今使っても効果がありません");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return false;
		}

		break;

	//マジックポーション
	case static_cast<int>(Item::UsedItem::MAGICPORTION):

		// アイテムIDが14でかつプレイヤーのMPがMAXでない場合に処理を実行
		if (curentMp < max_mp) {

			int healAmount = static_cast<int>(max_mp * PERCENTAGE);
			GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentMp(curentMp + healAmount);

			//回復音を鳴らす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

			// MPがMAX_MPを超える場合はMAX_MPにクリップする
			if (GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetCurentMp() > max_mp) {
				GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentMp(max_mp);
			}

			//アイテムがインベントリ内に存在したらインベントリから消去する
			game_manager->GetInventory()->InventoryItemRemove(itemid);

			//バトルシーンのみでログを流す
			if (BattleFlag) {
				battle_log_->AddRecoveryLog("Player", GetItemById(14).GetItemName().c_str(), healAmount);
			}

			return true;
		}
		//MpがmaxMpと同じ場合処理を飛ばす
		else {
			return false;
		}

		break;

	//アタックポーション
	case static_cast<int>(Item::UsedItem::ATTACKPORTION):

		//バトル時のみ使用
		if (!BattleFlag) { return false; }

		//SEを流す
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/status_up.mp3", DX_PLAYTYPE_BACK);

		//ボリュームを変える
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/status_up.mp3");

		//攻撃力を上昇させる
		AttackAmount = static_cast<int>(PlayerAttack * ATTACK_PERCENTAGE);
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerAttack(PlayerAttack + AttackAmount);

		//アイテムがインベントリ内に存在したらインベントリから消去する
		game_manager->GetInventory()->InventoryItemRemove(itemid);

		//ログを流す
		battle_log_->AddBuffLog("Player", GetItemById(16).GetItemName().c_str(), "攻撃力", AttackAmount);

		return true;

		break;

	//プラントエキス(回復アイテム)
	case static_cast<int>(Item::UsedItem::PLANTEXTRACT):

		//バトルシーンのみでログを流す
		//バトルシーンでかつ効果が出来ようできた場合
		if (BattleFlag && ItemHpHeal(PLANT_PERCENTAGE, itemid)) {

			//ログを流す
			battle_log_->AddRecoveryLog("Player", GetItemById(17).GetItemName().c_str(), healAmount);

			return true;
		}
		//バトルシーンの際に効果が無効だった場合
		else if (BattleFlag && !ItemHpHeal(PLANT_PERCENTAGE, itemid)) {

			return false;
		}
		//バトルシーン以外で使ってかつ、回復出来たら
		else if (!BattleFlag && ItemHpHeal(PLANT_PERCENTAGE, itemid)) {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("HPを60回復した！");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;

		}
		//バトルシーン以外でかつ、回復できなかった場合
		else if (!BattleFlag && !ItemHpHeal(PLANT_PERCENTAGE, itemid)) {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("今使っても効果がありません");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return false;
		}

		break;

	//スライムの鈴
	case static_cast<int>(Item::UsedItem::SLIMBELL):

		//バトル中は使えないようにする
		if (BattleFlag) { return false; }

		//スキルを追加する処理
		//処理が通った場合
		if (AddSkillItem(itemid, std::make_shared<SlimBell>())) {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("スキル : スライムを呼ぶを覚えた！！");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;
		}
		else {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("すでにスキルを取得している様だ");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			//デバックログを流す
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("\n スキルがすでに存在するか、エラーが起きました");
			tnl::DebugTrace("\n------------------------------------------------------------");

			return false;
		}

		break;

	//アースワームの鈴
	case static_cast<int>(Item::UsedItem::SNAKEBELL):

		//バトル中は使えないようにする
		if (BattleFlag) { return false; }

		//スキルを追加する処理
		//処理が通った場合
		if (AddSkillItem(itemid, std::make_shared<SnakeBell>())) {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("スキル : アースワームを呼ぶを覚えた！！");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;
		}
		else {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("すでにスキルを取得している様だ");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			//デバックログを流す
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("\n スキルがすでに存在するか、エラーが起きました");
			tnl::DebugTrace("\n------------------------------------------------------------");

			return false;
		}

		break;

	//大火炎斬りの秘伝書
	case static_cast<int>(Item::UsedItem::GREATFLAMESLASH):

		//バトル中は使えないようにする
		if (BattleFlag) { return false; }

		//スキルを進化させる処理
		//処理がとおった場合
		if (EvolutionSkillItem(itemid, std::make_shared<GreatFlameSlash>())) {
			
			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("火炎斬りが大火炎斬りに進化した！！");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;
		}
		else {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("進化元のスキルを取得していない様だ");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			//デバックログを流す
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("\n スキルがすでに存在するか、エラーが起きました");
			tnl::DebugTrace("\n------------------------------------------------------------");

			return false;
		}

		break;

	//ウォーターフォージ 
	case static_cast<int>(Item::UsedItem::WATERFORGE):

		//バトル中は使えないようにする
		if (BattleFlag) { return false; }

		//スキルを進化させる処理
		//処理がとおった場合
		if (EvolutionSkillItem(itemid, std::make_shared<WaterForge>())) {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("ウォーターブレードがウォーターフォージに進化した！！");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;
		}
		else {

			//UIマネージャーの取得
			auto ui_manager = UIManager::GetUIManager();

			//通知用の文字をセットする
			ui_manager->SetDisplayEventMessage("進化元のスキルを取得していない様だ");

			//ウィンドウを表示させて取得文字を出す
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			//デバックログを流す
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("\n スキルがすでに存在するか、エラーが起きました");
			tnl::DebugTrace("\n------------------------------------------------------------");

			return false;
		}

		break;

	default:

		//UIマネージャーの取得
		auto ui_manager = UIManager::GetUIManager();

		//通知用の文字をセットする
		ui_manager->SetDisplayEventMessage("使用できるアイテムではありません");

		//ウィンドウを表示させて取得文字を出す
		if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }


		return false;

		break;
	}

	return false;
}

//使用時Hp回復系
bool Item::ItemHpHeal(const int& percentage, const int& itemid)
{
	//現在のHpを取得する
	int curent_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp();

	//最大値のHpを取得する
	int max_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxHp();

	// アイテムIDが回復アイテムでかつ最大Hpより低ければ実行できる
	if (curent_hp < max_hp) {

		healAmount = percentage;
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentHp(curent_hp + healAmount);

		//回復音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		// HPがMAX_HPを超える場合はMAX_HPにクリップする
		if (GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp() > max_hp) {
			GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentHp(max_hp);
		}

		// 選択したアイテムの ID と一致する要素を特定する
		auto itemToRemove = std::find_if(GameManager::GetGameManager()->GetInventory()->GetInventoryList().begin(), GameManager::GetGameManager()->GetInventory()->GetInventoryList().end(),
			[itemid](const ItemBase& item) { return item.GetItemId() == itemid; });

		// アイテムが見つかった場合は削除する
		if (itemToRemove != GameManager::GetGameManager()->GetInventory()->GetInventoryList().end()) {
			GameManager::GetGameManager()->GetInventory()->GetInventoryList().erase(itemToRemove);
			//アイテムの数を減らす
			GameManager::GetGameManager()->GetInventory()->DelateItemNum();
		}

		return true;
	}
	//HpがmaxHpと同じ場合処理を飛ばす
	else {
		return false;
	}
}

//秘伝書アイテムに応じてスキルIDを返す
int Item::EvolutionSkillID(const int& evolution_item_id)const
{
	//大火炎斬り
	//火炎斬りの進化技
	if (evolution_item_id == static_cast<int>(UsedItem::GREATFLAMESLASH)) {

		//一時的にポインタを生成する
		Shared flame_slash = std::make_shared<FlameSlash>();

		//火炎斬りのIDを返す
		return flame_slash->GetId();
	}
	//ウォーターフォージ 
	//ウォーターブレードの進化技
	else if(evolution_item_id == static_cast<int>(UsedItem::WATERFORGE)){

		//一時的にポインタを生成する
		Shared water_blade = std::make_shared<WaterBlade>();

		//ウォーターブレードのIDを返す
		return water_blade->GetId();
	}

	//なにも取得出来なかった場合エラーを出す
	return -1;
}

//スキルを追加する系のアイテム
bool Item::AddSkillItem(const int& itemid, const Shared<Skill>& skill)
{
	//---スキルを追加する---//

	// 選択したアイテムの ID と一致する要素を特定する
	auto SkillAddID = std::find_if(GameManager::GetGameManager()->GetPlayer()->getSkillList().begin(), GameManager::GetGameManager()->GetPlayer()->getSkillList().end(),
		[itemid](const Shared<Skill>& skill) { return skill->GetId() == itemid; });

	// スキルが見つかった場合は処理をとばす
	if (SkillAddID != GameManager::GetGameManager()->GetPlayer()->getSkillList().end()) {
		return false;
	}

	// 見つからなかった場合スキルを追加する
	GameManager::GetGameManager()->GetPlayer()->AddSkill(skill);

	//---アイテムを消去する---//
	GameManager::GetGameManager()->GetInventory()->InventoryItemRemove(itemid);

	return true;
}

//スキルを進化させるアイテム(秘伝書)
bool Item::EvolutionSkillItem(const int& item_id, const Shared<Skill>& skill_)
{
	// 進化元のスキルを消去する
	auto& skill_list = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	// イテレータを使用してスキルリストを走査
	for (auto itr = skill_list.begin(); itr != skill_list.end(); ) {

		// スキルが見つかった場合、消去する
		if ((*itr)->GetId() == EvolutionSkillID(item_id)) {

			// スキルを消去
			GameManager::GetGameManager()->GetPlayer()->SkillRemove(EvolutionSkillID(item_id));

			//---スキルを追加する---//

			// 見つからなかった場合スキルを追加する
			GameManager::GetGameManager()->GetPlayer()->AddSkill(skill_);

			//---アイテムを消去する---//
			GameManager::GetGameManager()->GetInventory()->InventoryItemRemove(item_id);

			// 1つ見つかったら終了
			return true;
		}
		else {

			// スキルが見つからなかった場合、イテレータを進める
			++itr;  
		}
	}

	//見つからなかった場合処理を行わない
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//当たり判定のあるアイテム
CollisonItem::CollisonItem(const int& id, const float& pos_x, const float& pos_y, const int& CHIPSIZE, const float& scale, const int& ghdl, const bool canBeAcquired) : item_id(id), chip_size(CHIPSIZE), item_scale(scale), isObtainable(canBeAcquired)
{
	//アイテムハンドルのロード
	item_hdl = ghdl;

	//座標を決定する
	item_pos = { pos_x ,pos_y ,0 };
}

//描画処理
void CollisonItem::Draw(const KonCamera& camera)
{
	tnl::Vector3 draw_pos = item_pos - camera.GetTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), item_scale, 0, item_hdl, true);
}

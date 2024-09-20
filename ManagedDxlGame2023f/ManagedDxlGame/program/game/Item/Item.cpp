#include "Item.h"
#include <vector>
#include <algorithm>
#include"../Manager/GameManager.h"
#include"../Skill/Skill.h"
#include"../Menu/BattleLog.h"
#include"../Manager/CsvManager.h"

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
void Item::ItemUse(const int& itemid)
{

	//回復量(Hpの3割分回復させる)
	const float PERCENTAGE = 0.3f;

	//回復量(Hpの半分を回復させる)
	const float HI_PERCENTAGE = 0.5f;

	//プレイヤーの攻撃力の半分の値をプラスさせる(調整対象)
	const float ATTACK_PERCENTAGE = 0.5f;

	//回復量(Hpの7割を回復させる)
	const float PLANT_PERCENTAGE = 0.7f;

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

		//Hpを回復する
		ItemHpHeal(PERCENTAGE, itemid);

		//バトルシーンのみでログを流す
		if (BattleFlag) {
			battle_log_->AddRecoveryLog("Player", GetItemById(3).GetItemName().c_str(), healAmount);
		}

		break;

	//ハイポーション
	case static_cast<int>(Item::UsedItem::HIPORTION):

		//Hpを回復する
		ItemHpHeal(HI_PERCENTAGE, itemid);

		if (BattleFlag) {
			battle_log_->AddRecoveryLog("Player", GetItemById(8).GetItemName().c_str(), healAmount);
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
		}
		//MpがmaxMpと同じ場合処理を飛ばす
		else {
			return;
		}

		break;

	case static_cast<int>(Item::UsedItem::ATTACKPORTION):

		//バトル時のみ使用
		if (!BattleFlag) { return; }

		AttackAmount = static_cast<int>(PlayerAttack * ATTACK_PERCENTAGE);
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerAttack(PlayerAttack + AttackAmount);

		//アイテムがインベントリ内に存在したらインベントリから消去する
		game_manager->GetInventory()->InventoryItemRemove(itemid);

		//ログを流す
		battle_log_->AddBuffLog("Player", GetItemById(16).GetItemName().c_str(), "攻撃力", AttackAmount);

		break;

	case static_cast<int>(Item::UsedItem::PLANTEXTRACT):

		//hpを回復する
		ItemHpHeal(PLANT_PERCENTAGE, itemid);

		//バトルシーンのみでログを流す
		if (BattleFlag) {
			battle_log_->AddRecoveryLog("Player", GetItemById(17).GetItemName().c_str(), healAmount);
		}

		break;

	//スライムの鈴
	case static_cast<int>(Item::UsedItem::SLIMBELL):

		//バトル中は使えないようにする
		if (BattleFlag) { break; }

		//スキルを追加する処理
		AddSkillItem(itemid, Shared<SlimBell>());

		break;

	//スネークの鈴
	case static_cast<int>(Item::UsedItem::SNAKEBELL):

		//バトル中は使えないようにする
		if (BattleFlag) { break; }

		//スキルを追加する処理
		AddSkillItem(itemid, Shared<SnakeBell>());

		break;

	default:
		break;
	}
}

//使用時Hp回復系
void Item::ItemHpHeal(const float& percentage, const int& itemid)
{
	//現在のHpを取得する
	int curent_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp();

	//最大値のHpを取得する
	int max_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxHp();

	// アイテムIDが回復アイテムでかつ最大Hpより低ければ実行できる
	if (curent_hp < max_hp) {

		healAmount = static_cast<int>(max_hp * percentage);
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

	}
	//HpがmaxHpと同じ場合処理を飛ばす
	else {
		return;
	}
}

//スキルを追加する系のアイテム
template <class T>
void Item::AddSkillItem(const int& itemid, Shared<T> skill)
{
	//---スキルを追加する---//

	// 選択したアイテムの ID と一致する要素を特定する
	auto SkillToRemove = std::find_if(GameManager::GetGameManager()->GetPlayer()->getSkillList().begin(), GameManager::GetGameManager()->GetPlayer()->getSkillList().end(),
		[itemid](const Shared<Skill>& skill) { return skill->getId() == itemid; });

	// アイテムが見つかった場合は処理をとばす
	if (SkillToRemove != GameManager::GetGameManager()->GetPlayer()->getSkillList().end()) {
		return;
	}

	// 見つからなかった場合スキルを追加する
	GameManager::GetGameManager()->GetPlayer()->AddSkill(std::make_shared<T>());

	//---アイテムを消去する---//

	// 選択したアイテムの ID と一致する要素を特定する
	auto itemToRemove = std::find_if(GameManager::GetGameManager()->GetInventory()->GetInventoryList().begin(), GameManager::GetGameManager()->GetInventory()->GetInventoryList().end(),
		[itemid](const ItemBase& item) { return item.GetItemId() == itemid; });

	// アイテムが見つかった場合は削除する
	if (itemToRemove != GameManager::GetGameManager()->GetInventory()->GetInventoryList().end()) {
		GameManager::GetGameManager()->GetInventory()->GetInventoryList().erase(itemToRemove);
		//アイテムの数を減らす
		GameManager::GetGameManager()->GetInventory()->DelateItemNum();
	}
}


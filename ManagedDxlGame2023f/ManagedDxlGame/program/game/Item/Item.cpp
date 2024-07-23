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

Item::~Item()
{

}

void Item::update(float delta_time)
{

}



void Item::ItemLoadTypeInfo()
{
	int max_num = static_cast<int>(CsvManager::getCsvManager()->GetItemCsv().size());

	m_item_Status_Total_num = max_num;

	for (int y = 1; y < max_num; y++) {
		ItemBase itemStatus_info;
		
		//csv
		auto m_itemStatus_Csv_Info = CsvManager::getCsvManager()->GetItemCsv();

		//csvからnameの取得
		itemStatus_info.SetItemName(m_itemStatus_Csv_Info[y][0].getString());
		//csvからidの取得
		itemStatus_info.SetItemId(m_itemStatus_Csv_Info[y][1].getInt());
		//csvからtypeの取得
		itemStatus_info.SetItemType(m_itemStatus_Csv_Info[y][2].getInt());
		//csvからDamageの取得
		itemStatus_info.SetItemDamage(m_itemStatus_Csv_Info[y][3].getInt());
		//csvからdefanceの取得
		itemStatus_info.SetItemDefance(m_itemStatus_Csv_Info[y][5].getInt());
		//csvからpriceを取得
		itemStatus_info.SetItemPrice(m_itemStatus_Csv_Info[y][6].getInt());
		//csvからWeaponTypeを取得する
		itemStatus_info.SetWeaponType(m_itemStatus_Csv_Info[y][7].getInt());
		//csvからItemDetailを取得する
		itemStatus_info.SetItemDetail(m_itemStatus_Csv_Info[y][8].getString());

		m_Item_Status_Type.emplace_back(itemStatus_info);
	}

	//ダメージ順にソート
	m_Item_Status_Type.sort([](const Item::ItemBase& left, const Item::ItemBase& right){
		return left.getItemDamage() > right.getItemDamage();
	});

}

ItemBase Item::GetItemById(int id) const
{
	auto it = std::find_if(m_Item_Status_Type.begin(), m_Item_Status_Type.end(),
		[id](const ItemBase& item) { return item.getItemId() == id; });

	if (it != m_Item_Status_Type.end()) {
		return *it;
	}
	else {
		
		return ItemBase{};
	}
}

//アイテムを使用した時の処理
void Item::ItemUse(int itemid)
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
	auto PlayerAttack = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetAttack();

	//プレイヤーのMpを取得する
	auto curentMp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetCurentMp();

	//プレイヤーの最大Mpを取得する
	auto max_mp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetMaxMp();



	switch (itemid)
	{

	//ポーション
	case static_cast<int>(Item::UsedItem::PORTION):

		//Hpを回復する
		ItemHpHeal(PERCENTAGE, itemid);

		//バトルシーンのみでログを流す
		if (BattleFlag) {
			battle_log_->addRecoveryLog("Player", GetItemById(3).getItemName().c_str(), healAmount);
		}

		break;

	//ハイポーション
	case static_cast<int>(Item::UsedItem::HIPORTION):

		//Hpを回復する
		ItemHpHeal(HI_PERCENTAGE, itemid);

		if (BattleFlag) {
			battle_log_->addRecoveryLog("Player", GetItemById(8).getItemName().c_str(), healAmount);
		}

		break;

	//マジックポーション
	case static_cast<int>(Item::UsedItem::MAGICPORTION):

		// アイテムIDが14でかつプレイヤーのMPがMAXでない場合に処理を実行
		if (curentMp < max_mp) {

			int healAmount = static_cast<int>(max_mp * PERCENTAGE);
			GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentMp(curentMp + healAmount);

			//回復音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

			// MPがMAX_Pを超える場合はMAX_HPにクリップする
			if (GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetCurentMp() > max_mp) {
				GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentMp(max_mp);
			}

			//アイテムがインベントリ内に存在したらインベントリから消去する
			ItemRemove(itemid);

			//バトルシーンのみでログを流す
			if (BattleFlag) {
				battle_log_->addRecoveryLog("Player", GetItemById(14).getItemName().c_str(), healAmount);
			}
		}
		//MpがmaxMpと同じ場合処理を飛ばす
		else {
			return;
		}

		break;

	case static_cast<int>(Item::UsedItem::ATTACKPORTION):

		AttackAmount = static_cast<int>(PlayerAttack * ATTACK_PERCENTAGE);
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerAttack(PlayerAttack + AttackAmount);

		//アイテムがインベントリ内に存在したらインベントリから消去する
		ItemRemove(itemid);

		//ログを流す
		battle_log_->addBuffLog("Player", GetItemById(16).getItemName().c_str(), "攻撃力", AttackAmount);

		break;

	case static_cast<int>(Item::UsedItem::PLANTEXTRACT):

		//hpを回復する
		ItemHpHeal(PLANT_PERCENTAGE, itemid);

		//バトルシーンのみでログを流す
		if (BattleFlag) {
			battle_log_->addRecoveryLog("Player", GetItemById(17).getItemName().c_str(), healAmount);
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
void Item::ItemHpHeal(const float& percentage, int itemid)
{
	//現在のHpを取得する
	auto curent_hp = static_cast<int>(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetcurentHp());

	//最大値のHpを取得する
	float max_hp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetMaxHp();

	// アイテムIDが回復アイテムでかつ最大Hpより低ければ実行できる
	if (curent_hp < max_hp) {

		healAmount = static_cast<int>(max_hp * percentage);
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentHp(static_cast<float>(curent_hp + healAmount));

		//回復音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		// HPがMAX_HPを超える場合はMAX_HPにクリップする
		if (GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetcurentHp() > max_hp) {
			GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentHp(max_hp);
		}

		// 選択したアイテムの ID と一致する要素を特定する
		auto itemToRemove = std::find_if(GameManager::getGameManager()->getInventory()->GetInventoryList().begin(), GameManager::getGameManager()->getInventory()->GetInventoryList().end(),
			[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

		// アイテムが見つかった場合は削除する
		if (itemToRemove != GameManager::getGameManager()->getInventory()->GetInventoryList().end()) {
			GameManager::getGameManager()->getInventory()->GetInventoryList().erase(itemToRemove);
			//アイテムの数を減らす
			GameManager::getGameManager()->getInventory()->DelateItemNum();
		}

	}
	//HpがmaxHpと同じ場合処理を飛ばす
	else {
		return;
	}
}

//アイテムがインベントリ内に存在したらインベントリから消去する
void Item::ItemRemove(int itemid)
{
	// 選択したアイテムの ID と一致する要素を特定する
	auto itemToRemove = std::find_if(GameManager::getGameManager()->getInventory()->GetInventoryList().begin(), GameManager::getGameManager()->getInventory()->GetInventoryList().end(),
		[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

	// アイテムが見つかった場合は削除する
	if (itemToRemove != GameManager::getGameManager()->getInventory()->GetInventoryList().end()) {
		GameManager::getGameManager()->getInventory()->GetInventoryList().erase(itemToRemove);
		//アイテムの数を減らす
		GameManager::getGameManager()->getInventory()->DelateItemNum();
	}
}

void Item::draw()
{
	
}

//スキルを追加する系のアイテム
template <class T>
void Item::AddSkillItem(int itemid, Shared<T>skill)
{
	//---スキルを追加する---//

	// 選択したアイテムの ID と一致する要素を特定する
	auto SkillToRemove = std::find_if(GameManager::getGameManager()->getPlayer()->getSkillList().begin(), GameManager::getGameManager()->getPlayer()->getSkillList().end(),
		[itemid](const Shared<Skill>& skill) { return skill->getId() == itemid; });

	// アイテムが見つかった場合は処理をとばす
	if (SkillToRemove != GameManager::getGameManager()->getPlayer()->getSkillList().end()) {
		return;
	}

	// 見つからなかった場合スキルを追加する
	GameManager::getGameManager()->getPlayer()->AddSkill(std::make_shared<T>());

	//---アイテムを消去する---//

	// 選択したアイテムの ID と一致する要素を特定する
	auto itemToRemove = std::find_if(GameManager::getGameManager()->getInventory()->GetInventoryList().begin(), GameManager::getGameManager()->getInventory()->GetInventoryList().end(),
		[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

	// アイテムが見つかった場合は削除する
	if (itemToRemove != GameManager::getGameManager()->getInventory()->GetInventoryList().end()) {
		GameManager::getGameManager()->getInventory()->GetInventoryList().erase(itemToRemove);
		//アイテムの数を減らす
		GameManager::getGameManager()->getInventory()->DelateItemNum();
	}
}


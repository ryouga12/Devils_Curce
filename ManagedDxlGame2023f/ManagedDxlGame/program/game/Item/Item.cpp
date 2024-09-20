#include "Item.h"
#include <vector>
#include <algorithm>
#include"../Manager/GameManager.h"
#include"../Skill/Skill.h"
#include"../Menu/BattleLog.h"
#include"../Manager/CsvManager.h"

Item::Item()
{
	//�A�C�e���̃��[�h
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

		//csv����name�̎擾
		itemStatus_info.SetItemName(m_itemStatus_Csv_Info[y][0].getString());
		//csv����id�̎擾
		itemStatus_info.SetItemId(m_itemStatus_Csv_Info[y][1].getInt());
		//csv����type�̎擾
		itemStatus_info.SetItemType(m_itemStatus_Csv_Info[y][2].getInt());
		//csv����Damage�̎擾
		itemStatus_info.SetItemDamage(m_itemStatus_Csv_Info[y][3].getInt());
		//csv����Epic���擾����
		itemStatus_info.SetItemEpic(m_itemStatus_Csv_Info[y][4].getInt());
		//csv����defance�̎擾
		itemStatus_info.SetItemDefance(m_itemStatus_Csv_Info[y][5].getInt());
		//csv����price���擾
		itemStatus_info.SetItemPrice(m_itemStatus_Csv_Info[y][6].getInt());
		//csv����WeaponType���擾����
		itemStatus_info.SetWeaponType(m_itemStatus_Csv_Info[y][7].getInt());
		//csv����ItemDetail���擾����
		itemStatus_info.SetItemDetail(m_itemStatus_Csv_Info[y][8].getString());
		//csv���畐��̕␳�������郂���X�^�[�̎푰�̎擾����
		itemStatus_info.SetEpicCorrection(m_itemStatus_Csv_Info[y][9].getString());
		//csv������ʃA�C�e�����̏����擾����
		itemStatus_info.SetEssentialItemsFlag(m_itemStatus_Csv_Info[y][10].getInt());

		item_status_type.emplace_back(itemStatus_info);
	}

	//�_���[�W���Ƀ\�[�g
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

//�A�C�e�����g�p�������̏���
void Item::ItemUse(const int& itemid)
{

	//�񕜗�(Hp��3�����񕜂�����)
	const float PERCENTAGE = 0.3f;

	//�񕜗�(Hp�̔������񕜂�����)
	const float HI_PERCENTAGE = 0.5f;

	//�v���C���[�̍U���͂̔����̒l���v���X������(�����Ώ�)
	const float ATTACK_PERCENTAGE = 0.5f;

	//�񕜗�(Hp��7�����񕜂�����)
	const float PLANT_PERCENTAGE = 0.7f;

	//�v���C���[�̍U���͂��擾����
	auto PlayerAttack = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetAttack();

	//�v���C���[��Mp���擾����
	auto curentMp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetCurentMp();

	//�v���C���[�̍ő�Mp���擾����
	auto max_mp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxMp();

	//�Q�[���}�l�[�W���[�̃C���X�^���X
	auto game_manager = GameManager::GetGameManager();

	switch (itemid)
	{

	//�|�[�V����
	case static_cast<int>(Item::UsedItem::PORTION):

		//Hp���񕜂���
		ItemHpHeal(PERCENTAGE, itemid);

		//�o�g���V�[���݂̂Ń��O�𗬂�
		if (BattleFlag) {
			battle_log_->AddRecoveryLog("Player", GetItemById(3).GetItemName().c_str(), healAmount);
		}

		break;

	//�n�C�|�[�V����
	case static_cast<int>(Item::UsedItem::HIPORTION):

		//Hp���񕜂���
		ItemHpHeal(HI_PERCENTAGE, itemid);

		if (BattleFlag) {
			battle_log_->AddRecoveryLog("Player", GetItemById(8).GetItemName().c_str(), healAmount);
		}

		break;

	//�}�W�b�N�|�[�V����
	case static_cast<int>(Item::UsedItem::MAGICPORTION):

		// �A�C�e��ID��14�ł��v���C���[��MP��MAX�łȂ��ꍇ�ɏ��������s
		if (curentMp < max_mp) {

			int healAmount = static_cast<int>(max_mp * PERCENTAGE);
			GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentMp(curentMp + healAmount);

			//�񕜉���炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

			// MP��MAX_MP�𒴂���ꍇ��MAX_MP�ɃN���b�v����
			if (GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetCurentMp() > max_mp) {
				GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentMp(max_mp);
			}

			//�A�C�e�����C���x���g�����ɑ��݂�����C���x���g�������������
			game_manager->GetInventory()->InventoryItemRemove(itemid);

			//�o�g���V�[���݂̂Ń��O�𗬂�
			if (BattleFlag) {
				battle_log_->AddRecoveryLog("Player", GetItemById(14).GetItemName().c_str(), healAmount);
			}
		}
		//Mp��maxMp�Ɠ����ꍇ�������΂�
		else {
			return;
		}

		break;

	case static_cast<int>(Item::UsedItem::ATTACKPORTION):

		//�o�g�����̂ݎg�p
		if (!BattleFlag) { return; }

		AttackAmount = static_cast<int>(PlayerAttack * ATTACK_PERCENTAGE);
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerAttack(PlayerAttack + AttackAmount);

		//�A�C�e�����C���x���g�����ɑ��݂�����C���x���g�������������
		game_manager->GetInventory()->InventoryItemRemove(itemid);

		//���O�𗬂�
		battle_log_->AddBuffLog("Player", GetItemById(16).GetItemName().c_str(), "�U����", AttackAmount);

		break;

	case static_cast<int>(Item::UsedItem::PLANTEXTRACT):

		//hp���񕜂���
		ItemHpHeal(PLANT_PERCENTAGE, itemid);

		//�o�g���V�[���݂̂Ń��O�𗬂�
		if (BattleFlag) {
			battle_log_->AddRecoveryLog("Player", GetItemById(17).GetItemName().c_str(), healAmount);
		}

		break;

	//�X���C���̗�
	case static_cast<int>(Item::UsedItem::SLIMBELL):

		//�o�g�����͎g���Ȃ��悤�ɂ���
		if (BattleFlag) { break; }

		//�X�L����ǉ����鏈��
		AddSkillItem(itemid, Shared<SlimBell>());

		break;

	//�X�l�[�N�̗�
	case static_cast<int>(Item::UsedItem::SNAKEBELL):

		//�o�g�����͎g���Ȃ��悤�ɂ���
		if (BattleFlag) { break; }

		//�X�L����ǉ����鏈��
		AddSkillItem(itemid, Shared<SnakeBell>());

		break;

	default:
		break;
	}
}

//�g�p��Hp�񕜌n
void Item::ItemHpHeal(const float& percentage, const int& itemid)
{
	//���݂�Hp���擾����
	int curent_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp();

	//�ő�l��Hp���擾����
	int max_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxHp();

	// �A�C�e��ID���񕜃A�C�e���ł��ő�Hp���Ⴏ��Ύ��s�ł���
	if (curent_hp < max_hp) {

		healAmount = static_cast<int>(max_hp * percentage);
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentHp(curent_hp + healAmount);

		//�񕜉���炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		// HP��MAX_HP�𒴂���ꍇ��MAX_HP�ɃN���b�v����
		if (GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp() > max_hp) {
			GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentHp(max_hp);
		}

		// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
		auto itemToRemove = std::find_if(GameManager::GetGameManager()->GetInventory()->GetInventoryList().begin(), GameManager::GetGameManager()->GetInventory()->GetInventoryList().end(),
			[itemid](const ItemBase& item) { return item.GetItemId() == itemid; });

		// �A�C�e�������������ꍇ�͍폜����
		if (itemToRemove != GameManager::GetGameManager()->GetInventory()->GetInventoryList().end()) {
			GameManager::GetGameManager()->GetInventory()->GetInventoryList().erase(itemToRemove);
			//�A�C�e���̐������炷
			GameManager::GetGameManager()->GetInventory()->DelateItemNum();
		}

	}
	//Hp��maxHp�Ɠ����ꍇ�������΂�
	else {
		return;
	}
}

//�X�L����ǉ�����n�̃A�C�e��
template <class T>
void Item::AddSkillItem(const int& itemid, Shared<T> skill)
{
	//---�X�L����ǉ�����---//

	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto SkillToRemove = std::find_if(GameManager::GetGameManager()->GetPlayer()->getSkillList().begin(), GameManager::GetGameManager()->GetPlayer()->getSkillList().end(),
		[itemid](const Shared<Skill>& skill) { return skill->getId() == itemid; });

	// �A�C�e�������������ꍇ�͏������Ƃ΂�
	if (SkillToRemove != GameManager::GetGameManager()->GetPlayer()->getSkillList().end()) {
		return;
	}

	// ������Ȃ������ꍇ�X�L����ǉ�����
	GameManager::GetGameManager()->GetPlayer()->AddSkill(std::make_shared<T>());

	//---�A�C�e������������---//

	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto itemToRemove = std::find_if(GameManager::GetGameManager()->GetInventory()->GetInventoryList().begin(), GameManager::GetGameManager()->GetInventory()->GetInventoryList().end(),
		[itemid](const ItemBase& item) { return item.GetItemId() == itemid; });

	// �A�C�e�������������ꍇ�͍폜����
	if (itemToRemove != GameManager::GetGameManager()->GetInventory()->GetInventoryList().end()) {
		GameManager::GetGameManager()->GetInventory()->GetInventoryList().erase(itemToRemove);
		//�A�C�e���̐������炷
		GameManager::GetGameManager()->GetInventory()->DelateItemNum();
	}
}


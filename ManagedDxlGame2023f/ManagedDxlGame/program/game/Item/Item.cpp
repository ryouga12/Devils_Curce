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

		//csv����name�̎擾
		itemStatus_info.SetItemName(m_itemStatus_Csv_Info[y][0].getString());
		//csv����id�̎擾
		itemStatus_info.SetItemId(m_itemStatus_Csv_Info[y][1].getInt());
		//csv����type�̎擾
		itemStatus_info.SetItemType(m_itemStatus_Csv_Info[y][2].getInt());
		//csv����Damage�̎擾
		itemStatus_info.SetItemDamage(m_itemStatus_Csv_Info[y][3].getInt());
		//csv����defance�̎擾
		itemStatus_info.SetItemDefance(m_itemStatus_Csv_Info[y][5].getInt());
		//csv����price���擾
		itemStatus_info.SetItemPrice(m_itemStatus_Csv_Info[y][6].getInt());
		//csv����WeaponType���擾����
		itemStatus_info.SetWeaponType(m_itemStatus_Csv_Info[y][7].getInt());
		//csv����ItemDetail���擾����
		itemStatus_info.SetItemDetail(m_itemStatus_Csv_Info[y][8].getString());

		m_Item_Status_Type.emplace_back(itemStatus_info);
	}

	//�_���[�W���Ƀ\�[�g
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

//�A�C�e�����g�p�������̏���
void Item::ItemUse(int itemid)
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
	auto PlayerAttack = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetAttack();

	//�v���C���[��Mp���擾����
	auto curentMp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetCurentMp();

	//�v���C���[�̍ő�Mp���擾����
	auto max_mp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetMaxMp();



	switch (itemid)
	{

	//�|�[�V����
	case static_cast<int>(Item::UsedItem::PORTION):

		//Hp���񕜂���
		ItemHpHeal(PERCENTAGE, itemid);

		//�o�g���V�[���݂̂Ń��O�𗬂�
		if (BattleFlag) {
			battle_log_->addRecoveryLog("Player", GetItemById(3).getItemName().c_str(), healAmount);
		}

		break;

	//�n�C�|�[�V����
	case static_cast<int>(Item::UsedItem::HIPORTION):

		//Hp���񕜂���
		ItemHpHeal(HI_PERCENTAGE, itemid);

		if (BattleFlag) {
			battle_log_->addRecoveryLog("Player", GetItemById(8).getItemName().c_str(), healAmount);
		}

		break;

	//�}�W�b�N�|�[�V����
	case static_cast<int>(Item::UsedItem::MAGICPORTION):

		// �A�C�e��ID��14�ł��v���C���[��MP��MAX�łȂ��ꍇ�ɏ��������s
		if (curentMp < max_mp) {

			int healAmount = static_cast<int>(max_mp * PERCENTAGE);
			GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentMp(curentMp + healAmount);

			//�񕜉���炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

			// MP��MAX_P�𒴂���ꍇ��MAX_HP�ɃN���b�v����
			if (GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetCurentMp() > max_mp) {
				GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentMp(max_mp);
			}

			//�A�C�e�����C���x���g�����ɑ��݂�����C���x���g�������������
			ItemRemove(itemid);

			//�o�g���V�[���݂̂Ń��O�𗬂�
			if (BattleFlag) {
				battle_log_->addRecoveryLog("Player", GetItemById(14).getItemName().c_str(), healAmount);
			}
		}
		//Mp��maxMp�Ɠ����ꍇ�������΂�
		else {
			return;
		}

		break;

	case static_cast<int>(Item::UsedItem::ATTACKPORTION):

		AttackAmount = static_cast<int>(PlayerAttack * ATTACK_PERCENTAGE);
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerAttack(PlayerAttack + AttackAmount);

		//�A�C�e�����C���x���g�����ɑ��݂�����C���x���g�������������
		ItemRemove(itemid);

		//���O�𗬂�
		battle_log_->addBuffLog("Player", GetItemById(16).getItemName().c_str(), "�U����", AttackAmount);

		break;

	case static_cast<int>(Item::UsedItem::PLANTEXTRACT):

		//hp���񕜂���
		ItemHpHeal(PLANT_PERCENTAGE, itemid);

		//�o�g���V�[���݂̂Ń��O�𗬂�
		if (BattleFlag) {
			battle_log_->addRecoveryLog("Player", GetItemById(17).getItemName().c_str(), healAmount);
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
void Item::ItemHpHeal(const float& percentage, int itemid)
{
	//���݂�Hp���擾����
	auto curent_hp = static_cast<int>(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetcurentHp());

	//�ő�l��Hp���擾����
	float max_hp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetMaxHp();

	// �A�C�e��ID���񕜃A�C�e���ł��ő�Hp���Ⴏ��Ύ��s�ł���
	if (curent_hp < max_hp) {

		healAmount = static_cast<int>(max_hp * percentage);
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentHp(static_cast<float>(curent_hp + healAmount));

		//�񕜉���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		// HP��MAX_HP�𒴂���ꍇ��MAX_HP�ɃN���b�v����
		if (GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetcurentHp() > max_hp) {
			GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentHp(max_hp);
		}

		// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
		auto itemToRemove = std::find_if(GameManager::getGameManager()->getInventory()->GetInventoryList().begin(), GameManager::getGameManager()->getInventory()->GetInventoryList().end(),
			[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

		// �A�C�e�������������ꍇ�͍폜����
		if (itemToRemove != GameManager::getGameManager()->getInventory()->GetInventoryList().end()) {
			GameManager::getGameManager()->getInventory()->GetInventoryList().erase(itemToRemove);
			//�A�C�e���̐������炷
			GameManager::getGameManager()->getInventory()->DelateItemNum();
		}

	}
	//Hp��maxHp�Ɠ����ꍇ�������΂�
	else {
		return;
	}
}

//�A�C�e�����C���x���g�����ɑ��݂�����C���x���g�������������
void Item::ItemRemove(int itemid)
{
	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto itemToRemove = std::find_if(GameManager::getGameManager()->getInventory()->GetInventoryList().begin(), GameManager::getGameManager()->getInventory()->GetInventoryList().end(),
		[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

	// �A�C�e�������������ꍇ�͍폜����
	if (itemToRemove != GameManager::getGameManager()->getInventory()->GetInventoryList().end()) {
		GameManager::getGameManager()->getInventory()->GetInventoryList().erase(itemToRemove);
		//�A�C�e���̐������炷
		GameManager::getGameManager()->getInventory()->DelateItemNum();
	}
}

void Item::draw()
{
	
}

//�X�L����ǉ�����n�̃A�C�e��
template <class T>
void Item::AddSkillItem(int itemid, Shared<T>skill)
{
	//---�X�L����ǉ�����---//

	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto SkillToRemove = std::find_if(GameManager::getGameManager()->getPlayer()->getSkillList().begin(), GameManager::getGameManager()->getPlayer()->getSkillList().end(),
		[itemid](const Shared<Skill>& skill) { return skill->getId() == itemid; });

	// �A�C�e�������������ꍇ�͏������Ƃ΂�
	if (SkillToRemove != GameManager::getGameManager()->getPlayer()->getSkillList().end()) {
		return;
	}

	// ������Ȃ������ꍇ�X�L����ǉ�����
	GameManager::getGameManager()->getPlayer()->AddSkill(std::make_shared<T>());

	//---�A�C�e������������---//

	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto itemToRemove = std::find_if(GameManager::getGameManager()->getInventory()->GetInventoryList().begin(), GameManager::getGameManager()->getInventory()->GetInventoryList().end(),
		[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

	// �A�C�e�������������ꍇ�͍폜����
	if (itemToRemove != GameManager::getGameManager()->getInventory()->GetInventoryList().end()) {
		GameManager::getGameManager()->getInventory()->GetInventoryList().erase(itemToRemove);
		//�A�C�e���̐������炷
		GameManager::getGameManager()->getInventory()->DelateItemNum();
	}
}


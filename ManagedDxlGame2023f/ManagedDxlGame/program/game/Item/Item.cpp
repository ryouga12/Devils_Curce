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
		//csv����A�C�R�����擾����
		itemStatus_info.SetItemIcon(m_itemStatus_Csv_Info[y][11].getString());

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
bool Item::ItemUse(const int& itemid)
{

	//�񕜗�(HP��20�񕜂�����)
	const int PERCENTAGE = 20;

	//�񕜗�(Hp��40�񕜂�����)
	const int HI_PERCENTAGE = 40;

	//�v���C���[�̍U���͂̔����̒l���v���X������(�����Ώ�)
	const float ATTACK_PERCENTAGE = 0.5f;

	//�񕜗�(HP��60�񕜂�����)
	const int PLANT_PERCENTAGE = 60;

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

		//�o�g���V�[���݂̂Ń��O�𗬂�
		//�o�g���V�[���ł����ʂ��o���悤�ł����ꍇ
		if (BattleFlag && ItemHpHeal(PERCENTAGE, itemid)) {

			//���O�𗬂�
			battle_log_->AddRecoveryLog("Player", GetItemById(3).GetItemName().c_str(), healAmount);

			return true;
		}
		//�o�g���V�[���̍ۂɌ��ʂ������������ꍇ
		else if (BattleFlag && !ItemHpHeal(PERCENTAGE, itemid)) {

			return false;
		}
		//�o�g���V�[���ȊO�Ŏg���Ă��A�񕜏o������
		else if(!BattleFlag && ItemHpHeal(PERCENTAGE, itemid)) {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("HP��20�񕜂����I");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;

		}
		//�o�g���V�[���ȊO�ł��A�񕜂ł��Ȃ������ꍇ
		else if (!BattleFlag && !ItemHpHeal(PERCENTAGE, itemid)) {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("���g���Ă����ʂ�����܂���");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return false;
		}

		break;

	//�n�C�|�[�V����
	case static_cast<int>(Item::UsedItem::HIPORTION):

		//�o�g���V�[���݂̂Ń��O�𗬂�
		//�o�g���V�[���ł����ʂ��o���悤�ł����ꍇ
		if (BattleFlag && ItemHpHeal(HI_PERCENTAGE, itemid)) {

			//���O�𗬂�
			battle_log_->AddRecoveryLog("Player", GetItemById(8).GetItemName().c_str(), healAmount);

			return true;
		}
		//�o�g���V�[���̍ۂɌ��ʂ������������ꍇ
		else if (BattleFlag && !ItemHpHeal(HI_PERCENTAGE, itemid)) {

			return false;
		}
		//�o�g���V�[���ȊO�Ŏg���Ă��A�񕜏o������
		else if (!BattleFlag && ItemHpHeal(HI_PERCENTAGE, itemid)) {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("HP��40�񕜂����I");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;

		}
		//�o�g���V�[���ȊO�ł��A�񕜂ł��Ȃ������ꍇ
		else if (!BattleFlag && !ItemHpHeal(HI_PERCENTAGE, itemid)) {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("���g���Ă����ʂ�����܂���");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return false;
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

			return true;
		}
		//Mp��maxMp�Ɠ����ꍇ�������΂�
		else {
			return false;
		}

		break;

	//�A�^�b�N�|�[�V����
	case static_cast<int>(Item::UsedItem::ATTACKPORTION):

		//�o�g�����̂ݎg�p
		if (!BattleFlag) { return false; }

		//SE�𗬂�
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/status_up.mp3", DX_PLAYTYPE_BACK);

		//�{�����[����ς���
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/status_up.mp3");

		//�U���͂��㏸������
		AttackAmount = static_cast<int>(PlayerAttack * ATTACK_PERCENTAGE);
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerAttack(PlayerAttack + AttackAmount);

		//�A�C�e�����C���x���g�����ɑ��݂�����C���x���g�������������
		game_manager->GetInventory()->InventoryItemRemove(itemid);

		//���O�𗬂�
		battle_log_->AddBuffLog("Player", GetItemById(16).GetItemName().c_str(), "�U����", AttackAmount);

		return true;

		break;

	//�v�����g�G�L�X(�񕜃A�C�e��)
	case static_cast<int>(Item::UsedItem::PLANTEXTRACT):

		//�o�g���V�[���݂̂Ń��O�𗬂�
		//�o�g���V�[���ł����ʂ��o���悤�ł����ꍇ
		if (BattleFlag && ItemHpHeal(PLANT_PERCENTAGE, itemid)) {

			//���O�𗬂�
			battle_log_->AddRecoveryLog("Player", GetItemById(17).GetItemName().c_str(), healAmount);

			return true;
		}
		//�o�g���V�[���̍ۂɌ��ʂ������������ꍇ
		else if (BattleFlag && !ItemHpHeal(PLANT_PERCENTAGE, itemid)) {

			return false;
		}
		//�o�g���V�[���ȊO�Ŏg���Ă��A�񕜏o������
		else if (!BattleFlag && ItemHpHeal(PLANT_PERCENTAGE, itemid)) {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("HP��60�񕜂����I");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;

		}
		//�o�g���V�[���ȊO�ł��A�񕜂ł��Ȃ������ꍇ
		else if (!BattleFlag && !ItemHpHeal(PLANT_PERCENTAGE, itemid)) {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("���g���Ă����ʂ�����܂���");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return false;
		}

		break;

	//�X���C���̗�
	case static_cast<int>(Item::UsedItem::SLIMBELL):

		//�o�g�����͎g���Ȃ��悤�ɂ���
		if (BattleFlag) { return false; }

		//�X�L����ǉ����鏈��
		//�������ʂ����ꍇ
		if (AddSkillItem(itemid, std::make_shared<SlimBell>())) {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("�X�L�� : �X���C�����ĂԂ��o�����I�I");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;
		}
		else {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("���łɃX�L�����擾���Ă���l��");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			//�f�o�b�N���O�𗬂�
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("\n �X�L�������łɑ��݂��邩�A�G���[���N���܂���");
			tnl::DebugTrace("\n------------------------------------------------------------");

			return false;
		}

		break;

	//�A�[�X���[���̗�
	case static_cast<int>(Item::UsedItem::SNAKEBELL):

		//�o�g�����͎g���Ȃ��悤�ɂ���
		if (BattleFlag) { return false; }

		//�X�L����ǉ����鏈��
		//�������ʂ����ꍇ
		if (AddSkillItem(itemid, std::make_shared<SnakeBell>())) {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("�X�L�� : �A�[�X���[�����ĂԂ��o�����I�I");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;
		}
		else {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("���łɃX�L�����擾���Ă���l��");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			//�f�o�b�N���O�𗬂�
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("\n �X�L�������łɑ��݂��邩�A�G���[���N���܂���");
			tnl::DebugTrace("\n------------------------------------------------------------");

			return false;
		}

		break;

	//��Ή��a��̔�`��
	case static_cast<int>(Item::UsedItem::GREATFLAMESLASH):

		//�o�g�����͎g���Ȃ��悤�ɂ���
		if (BattleFlag) { return false; }

		//�X�L����i�������鏈��
		//�������Ƃ������ꍇ
		if (EvolutionSkillItem(itemid, std::make_shared<GreatFlameSlash>())) {
			
			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("�Ή��a�肪��Ή��a��ɐi�������I�I");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;
		}
		else {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("�i�����̃X�L�����擾���Ă��Ȃ��l��");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			//�f�o�b�N���O�𗬂�
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("\n �X�L�������łɑ��݂��邩�A�G���[���N���܂���");
			tnl::DebugTrace("\n------------------------------------------------------------");

			return false;
		}

		break;

	//�E�H�[�^�[�t�H�[�W 
	case static_cast<int>(Item::UsedItem::WATERFORGE):

		//�o�g�����͎g���Ȃ��悤�ɂ���
		if (BattleFlag) { return false; }

		//�X�L����i�������鏈��
		//�������Ƃ������ꍇ
		if (EvolutionSkillItem(itemid, std::make_shared<WaterForge>())) {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("�E�H�[�^�[�u���[�h���E�H�[�^�[�t�H�[�W�ɐi�������I�I");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			return true;
		}
		else {

			//UI�}�l�[�W���[�̎擾
			auto ui_manager = UIManager::GetUIManager();

			//�ʒm�p�̕������Z�b�g����
			ui_manager->SetDisplayEventMessage("�i�����̃X�L�����擾���Ă��Ȃ��l��");

			//�E�B���h�E��\�������Ď擾�������o��
			if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }

			//�f�o�b�N���O�𗬂�
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("\n �X�L�������łɑ��݂��邩�A�G���[���N���܂���");
			tnl::DebugTrace("\n------------------------------------------------------------");

			return false;
		}

		break;

	default:

		//UI�}�l�[�W���[�̎擾
		auto ui_manager = UIManager::GetUIManager();

		//�ʒm�p�̕������Z�b�g����
		ui_manager->SetDisplayEventMessage("�g�p�ł���A�C�e���ł͂���܂���");

		//�E�B���h�E��\�������Ď擾�������o��
		if (!ui_manager->GetIsNotificationVisible()) { ui_manager->ToggleAcquisitionWindow(); }


		return false;

		break;
	}

	return false;
}

//�g�p��Hp�񕜌n
bool Item::ItemHpHeal(const int& percentage, const int& itemid)
{
	//���݂�Hp���擾����
	int curent_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp();

	//�ő�l��Hp���擾����
	int max_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxHp();

	// �A�C�e��ID���񕜃A�C�e���ł��ő�Hp���Ⴏ��Ύ��s�ł���
	if (curent_hp < max_hp) {

		healAmount = percentage;
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

		return true;
	}
	//Hp��maxHp�Ɠ����ꍇ�������΂�
	else {
		return false;
	}
}

//��`���A�C�e���ɉ����ăX�L��ID��Ԃ�
int Item::EvolutionSkillID(const int& evolution_item_id)const
{
	//��Ή��a��
	//�Ή��a��̐i���Z
	if (evolution_item_id == static_cast<int>(UsedItem::GREATFLAMESLASH)) {

		//�ꎞ�I�Ƀ|�C���^�𐶐�����
		Shared flame_slash = std::make_shared<FlameSlash>();

		//�Ή��a���ID��Ԃ�
		return flame_slash->GetId();
	}
	//�E�H�[�^�[�t�H�[�W 
	//�E�H�[�^�[�u���[�h�̐i���Z
	else if(evolution_item_id == static_cast<int>(UsedItem::WATERFORGE)){

		//�ꎞ�I�Ƀ|�C���^�𐶐�����
		Shared water_blade = std::make_shared<WaterBlade>();

		//�E�H�[�^�[�u���[�h��ID��Ԃ�
		return water_blade->GetId();
	}

	//�Ȃɂ��擾�o���Ȃ������ꍇ�G���[���o��
	return -1;
}

//�X�L����ǉ�����n�̃A�C�e��
bool Item::AddSkillItem(const int& itemid, const Shared<Skill>& skill)
{
	//---�X�L����ǉ�����---//

	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto SkillAddID = std::find_if(GameManager::GetGameManager()->GetPlayer()->getSkillList().begin(), GameManager::GetGameManager()->GetPlayer()->getSkillList().end(),
		[itemid](const Shared<Skill>& skill) { return skill->GetId() == itemid; });

	// �X�L�������������ꍇ�͏������Ƃ΂�
	if (SkillAddID != GameManager::GetGameManager()->GetPlayer()->getSkillList().end()) {
		return false;
	}

	// ������Ȃ������ꍇ�X�L����ǉ�����
	GameManager::GetGameManager()->GetPlayer()->AddSkill(skill);

	//---�A�C�e������������---//
	GameManager::GetGameManager()->GetInventory()->InventoryItemRemove(itemid);

	return true;
}

//�X�L����i��������A�C�e��(��`��)
bool Item::EvolutionSkillItem(const int& item_id, const Shared<Skill>& skill_)
{
	// �i�����̃X�L������������
	auto& skill_list = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	// �C�e���[�^���g�p���ăX�L�����X�g�𑖍�
	for (auto itr = skill_list.begin(); itr != skill_list.end(); ) {

		// �X�L�������������ꍇ�A��������
		if ((*itr)->GetId() == EvolutionSkillID(item_id)) {

			// �X�L��������
			GameManager::GetGameManager()->GetPlayer()->SkillRemove(EvolutionSkillID(item_id));

			//---�X�L����ǉ�����---//

			// ������Ȃ������ꍇ�X�L����ǉ�����
			GameManager::GetGameManager()->GetPlayer()->AddSkill(skill_);

			//---�A�C�e������������---//
			GameManager::GetGameManager()->GetInventory()->InventoryItemRemove(item_id);

			// 1����������I��
			return true;
		}
		else {

			// �X�L����������Ȃ������ꍇ�A�C�e���[�^��i�߂�
			++itr;  
		}
	}

	//������Ȃ������ꍇ�������s��Ȃ�
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//�����蔻��̂���A�C�e��
CollisonItem::CollisonItem(const int& id, const float& pos_x, const float& pos_y, const int& CHIPSIZE, const float& scale, const int& ghdl, const bool canBeAcquired) : item_id(id), chip_size(CHIPSIZE), item_scale(scale), isObtainable(canBeAcquired)
{
	//�A�C�e���n���h���̃��[�h
	item_hdl = ghdl;

	//���W�����肷��
	item_pos = { pos_x ,pos_y ,0 };
}

//�`�揈��
void CollisonItem::Draw(const KonCamera& camera)
{
	tnl::Vector3 draw_pos = item_pos - camera.GetTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), item_scale, 0, item_hdl, true);
}

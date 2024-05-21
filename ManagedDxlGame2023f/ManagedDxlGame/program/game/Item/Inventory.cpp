#include "Inventory.h"

using namespace std;

Inventory::Inventory()
{
	item = make_shared<Item>();

	//���j���[�̏�����
	InitMenuWinodow();

	//�f�o�b�N
	AddInventory(item->GetItemById(21).getItemId());

}

Inventory::~Inventory()
{
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/cousour_bgm.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/kaihuku.mp3");

	//�摜����������
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/cur_sannkaku2.png");

}

//�X�V����
void Inventory::Update(float delta_time)
{
	//�C���x���g���̍X�V
	swichInventoryUpdate(delta_time);

}

//�`�揈��
void Inventory::draw()
{
	
	//�I������Ă���̂��ŏ��̃��j���[��������
	if (select_menu == MenuWindow_I::First_Menu) {
		First_Menu();
	}
	//�I�����ꂽ�̂��A�C�e�����j���[��������
	else if (select_menu == MenuWindow_I::ItemMenu) {
		menu_window->Menu_draw();
		ItemMenu(DrawPos, CurentPageText, cursorX,item_per_page);
	}
	//�I�����ꂽ�̂��v���C���[�̋������j���[��������
	else if (select_menu == MenuWindow_I::StatusMenu) {
		PlyStatusMenu();
	}
	//�I�����ꂽ�̂��A�C�e���̏ڍ׃��j���[��������
	else if (select_menu == MenuWindow_I::ItemUseMenu) {
		menu_window->Menu_draw();
		ItemMenu(DrawPos, CurentPageText, cursorX, item_per_page);
		ItemUseMenu();
		if (item_coment == ItemComent::No) {
			item_coment_window->Menu_draw();
			DrawStringEx(ItemComentPos.x, ItemComentPos.y, Color_White, "���̃A�C�e���͎g�p�ł��܂���");
		}
	}
	else if (select_menu == MenuWindow_I::ItemDetailMenu) {
		menu_window->Menu_draw();
		ItemMenu(DrawPos, CurentPageText, cursorX, item_per_page);
		ItemUseMenu();
		ItemDetail();
	}
	else if (select_menu == MenuWindow_I::SkillMenu) {
		InventorySkill();

	}
	else if (select_menu == MenuWindow_I::SkillDatailMenu) {
		InventorySkill();
		SkillDetailDraw(selectedSkillIndex);
	}
	
}

//�A�C�e����ǉ�����
void Inventory::AddInventory(int id)
{
	if (InventoryList.size() == InventoryMaxSize) return;
	ItemBase newItem = item->GetItemById(id);
	InventoryList.emplace_back(newItem);
	itemNum++;
}

//����𑕔�����
void Inventory::EquipWeapon(int weaponIndex)
{
	// �A�C�e�����Ȃ��ꍇ��C���f�b�N�X���s���ȏꍇ�͉��������Ɋ֐����I��
	if (itemNum == 0 || weaponIndex < 0 || weaponIndex >= itemNum) {
		return;
	}

	// �I�����ꂽ�A�C�e�����擾
	ItemBase selectedItem = InventoryList[weaponIndex];

	// �����A�C�e�������ɑ�������Ă��邩�`�F�b�N
	bool isAlreadyEquipped = false;
	for (const auto& equipped : equippedWeapon) {
		if (equipped.getItemId() == selectedItem.getItemId()) {
			// �����A�C�e������������Ă���ꍇ�A���������Ɋ֐����I��
			isAlreadyEquipped = true;
			break;
		}
	}

	for (const auto& equipped : equippedArmor) {
		if (equipped.getItemId() == selectedItem.getItemId()) {
			// �����A�C�e������������Ă���ꍇ�A���������Ɋ֐����I��
			isAlreadyEquipped = true;
			break;
		}
	}


	// �����A�C�e�������ɑ�������Ă���ꍇ�͉��������Ɋ֐����I��
	if (isAlreadyEquipped) {
		return;
	}

	// �V�����A�C�e���̃^�C�v�ɉ����đ�����؂�ւ���
	if (selectedItem.getItemType() == WEAPON) {
		auto curentAttack = plyer->getPlayerStatusSave().getAttack();
		// �������̕�����O��
		if (!equippedWeapon.empty()) {
			// �������̕���̌��ʂ𔽉f����폜����
			ItemBase equippedWeaponItem = equippedWeapon.back();
			//�������̕���̃X�e�[�^�X���폜����
			curentAttack -= equippedWeaponItem.getItemDamage();
			// �������̕�����폜����
			equippedWeapon.pop_back();
		}
		
		// �V��������𑕔�
		equippedWeapon.push_back(selectedItem);
		// �v���C���[�̃X�e�[�^�X�ɐV��������̌��ʂ𔽉f
		equipWeapon = true;
		equipAttack = selectedItem.getItemDamage();
		plyer->getPlayerStatusSave().SetPlayerAttack(curentAttack + equipAttack);
	}
	else if (selectedItem.getItemType() == ARMOR) {
		auto curentDefance = plyer->getPlayerStatusSave().getDefance();
		// �������̖h����O��
		if (!equippedArmor.empty()) {
			// �������̖h��̌��ʂ𔽉f����폜����
			ItemBase equippedArmorItem = equippedArmor.back();
			//�������̖h��̃X�e�[�^�X���폜����
			curentDefance -= equippedArmorItem.getItemDefance();
			// �������̖h����폜����
			equippedArmor.pop_back();
		}
		// �V�����h��𑕔�
		equippedArmor.push_back(selectedItem);
		// �v���C���[�̃X�e�[�^�X�ɐV�����h��̌��ʂ𔽉f
		equipArmor = true;
		equipDefance = selectedItem.getItemDefance();
		plyer->getPlayerStatusSave().SetPlayerDefance(curentDefance + equipDefance);
	}
}

//�A�C�e���̏ڍׂ��J�������̏���
void Inventory::ItemDetail()
{
	ItemDetailWindow->Menu_draw();

	DrawStringEx(630, 70, -1, "===�A�C�e������===");

	//�A�C�e���������擾����
	std::string ItemDetail = InventoryList[selectedIndex].getItemDetail();

	//�C���x���g�����󂶂�Ȃ��ꍇ
	if (!InventoryList.empty()) {
	//�A�C�e���̏ڍׂ�\������
		DrawStringEx(630 , 100 , -1 , "%s" ,ItemDetail.c_str());
	}

}

void Inventory::InventoryItemUse(int itemid)
{
	//�v���C���[�̍ő�HP���擾����
	max_hp = static_cast<int>(plyer->getPlayerStatusSave().getMaxHp());
	auto max_mp = static_cast<int>(plyer->getPlayerStatusSave().getMaxMp());

	//����֌W�͎g�p�ł��܂���ƕ\��������
	if (itemid == 0 || itemid == 1 || itemid == 2 || itemid == 4 || itemid == 5 || itemid == 6 || itemid == 7
		|| itemid == 9 || itemid == 10 || itemid == 11 || itemid == 12 || itemid == 13 || itemid == 15 || itemid == 18
		|| itemid == 19 || itemid == 25|| itemid == 26 || itemid == 28 || itemid == 29)
	{
		//�g���܂���ƃE�B���h�E�ɕ`�悷��
		item_coment = ItemComent::No;
		return;
	}
	//�|�[�V����
	else if (itemid == 3) {
		
		//�񕜗�(Hp��3�����񕜂�����)
		float percentage = 0.3;

		//Hp���񕜂���
		ItemHpHeal(percentage, itemid);

		//�o�g���V�[���݂̂Ń��O�𗬂�
		if (BattleFlag) {
			battleLog->addRecoveryLog("Player", item->GetItemById(3).getItemName().c_str(), healAmount);
		}

	}
	//�n�C�|�[�V����
	else if (itemid == 8) {
		
		//�񕜗�(Hp�̔������񕜂�����)
		float percentage = 0.5;

		//Hp���񕜂���
		ItemHpHeal(percentage, itemid);

		if (BattleFlag) {
			battleLog->addRecoveryLog("Player", item->GetItemById(8).getItemName().c_str(), healAmount);
		}
	}
	else if (itemid == 14) {
		//�v���C���[��Mp���擾����
		auto curentMp = plyer->getPlayerStatusSave().getCurentMp();

		// �A�C�e��ID��14�ł��v���C���[��MP��MAX�łȂ��ꍇ�ɏ��������s
		if (curentMp < max_mp) {

			// MP���ő�MP��30���񕜂�����
			float percentage = 0.3;
			int healAmount = static_cast<int>(max_hp * percentage);
			plyer->getPlayerStatusSave().SetPlayerCurentMp(curentMp + healAmount);

			//�񕜉���炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

			// MP��MAX_P�𒴂���ꍇ��MAX_HP�ɃN���b�v����
			if (plyer->getPlayerStatusSave().getCurentMp() > max_mp) {
				plyer->getPlayerStatusSave().SetPlayerCurentMp(max_mp);
			}

			// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
			auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
				[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

			// �A�C�e�������������ꍇ�͍폜����
			if (itemToRemove != InventoryList.end()) {
				InventoryList.erase(itemToRemove);
				itemNum--;
			}
			//�o�g���V�[���݂̂Ń��O�𗬂�
			if (BattleFlag) {
				battleLog->addRecoveryLog("Player", item->GetItemById(14).getItemName().c_str(), healAmount);
			}
		}
		//Mp��maxMp�Ɠ����ꍇ�������΂�
		else {
			return;
		}
	}
	if (itemid == 16 && BattleFlag) {

		//�v���C���[�̍U���͂��擾����
		auto PlayerAttack = plyer->getPlayerStatusSave().getAttack();

		//�v���C���[�̍U���͂̔����̒l���v���X������(�����Ώ�)
		float percentage = 0.5;

		AttackAmount = static_cast<int>(PlayerAttack * percentage);
		plyer->getPlayerStatusSave().SetPlayerAttack(PlayerAttack + AttackAmount);


		// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
		auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
			[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

		// �A�C�e�������������ꍇ�͍폜����
		if (itemToRemove != InventoryList.end()) {
			InventoryList.erase(itemToRemove);
			itemNum--;
		}

		//���O�𗬂�
		battleLog->addBuffLog("Player", item->GetItemById(16).getItemName().c_str(), "�U����", AttackAmount);
	}
	//�v�����g�G�L�X(��ԉ񕜗͂������A�C�e��)
	if (itemid == 17) {

		//�񕜗�(Hp��7�����񕜂�����)
		float percentage = 0.7;

		//hp���񕜂���
		ItemHpHeal(percentage, itemid);

		//�o�g���V�[���݂̂Ń��O�𗬂�
		if (BattleFlag) {
			battleLog->addRecoveryLog("Player", item->GetItemById(17).getItemName().c_str(), healAmount);
		}
	}
	//�X���C���̗�
	if (itemid == 20 && !BattleFlag) {

		//�X�L����ǉ����鏈��
		AddSkillItem(itemid, Shared<SlimBell>());
	}
	//
	if (itemid == 21 && !BattleFlag) {

		//�X�L����ǉ����鏈��
		AddSkillItem(itemid, Shared<SnakeBell>());
	}

}

//�J�[�\���ړ����̃C���f�b�N�X����Ǝ擾
void Inventory::ItemCurourIndex(int ItemPerPage)
{
	//---�I�������A�C�e���ɂ�ID�Ƃ��Ď擾����---//

	// ��L�[�������ꂽ�Ƃ��̏���
	//��ԏ�ɃJ�[�\���������ꍇ����ȏ�ɂ����Ȃ��悤�ɂ���
	if (currentPage == 0 && selectedIndex == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// �������Ȃ�
		return;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selectedIndex % ItemPerPage == 0) {
			// �J�[�\�����y�[�W���̍ŏ㕔�ɂ���ꍇ�́A���̃y�[�W�̍Ō�̗v�f��I��
			if (currentPage > 0) {
				--currentPage;
				// �C���f�b�N�X���X�V���ăy�[�W�̍Ō�̗v�f��I������
				selectedIndex = (currentPage + 1) * ItemPerPage - 1;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A���̗v�f��I��
			--selectedIndex;
		}
	}

	// ���L�[�������ꂽ�Ƃ��̏���
	if (selectedIndex == InventoryList.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN))return;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selectedIndex + 1) % ItemPerPage == 0) {
			// �J�[�\�����y�[�W���̍ŉ����ɂ���ꍇ�́A����̃y�[�W�̍ŏ��̗v�f��I��
			//���̏��v���C���[�����Ă�C���x���g����20�ōl���Ă����4�y�[�W���܂ŊJ����悤�ɂ���
			if (currentPage < 4) {
				++currentPage;
				// �C���f�b�N�X���X�V���ăy�[�W�̍ŏ��̗v�f��I������
				selectedIndex = currentPage * ItemPerPage;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A����̗v�f��I��
			++selectedIndex;
		}
	}

	// selectedIndex���L���Ȕ͈͓��ɂ��邩�m�F���āA�I�����ꂽ�A�C�e����ID���擾����
	if (selectedIndex >= 0 && selectedIndex < InventoryList.size()) {
		ItemBase selectedItem = InventoryList[selectedIndex];
		selectedItemId = selectedItem.getItemId();
	}
}

//�C���x���g���̃��j���[��؂�ւ���
void Inventory::SetSelect_num(int select_menu_)
{
	switch (select_menu_)
	{
	case 0 :
		select_menu = MenuWindow_I::Empty;

		break;

	case 1:
		select_menu = MenuWindow_I::First_Menu;


		break;

	case 2:

		select_menu = MenuWindow_I::ItemMenu;

		break;

	case 3:
		select_menu = MenuWindow_I::StatusMenu;

		break;

	case 4:
		select_menu = MenuWindow_I::ItemUseMenu;

		break;

	default:
		break;
	}
}

void Inventory::CusorMove()
{
	//�A�C�e�����Ȃ��ꍇ�A�������Ȃ�
	if (InventoryList.empty())
	{
		return;
	}
	//�J�[�\������ԏ�ɂ���ꍇ�J�[�\��������ɏ�ɉ����Ă��J�[�\���𓮂����Ȃ��悤�ɂ���
	if (selectCursor == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) 
	{
		return;
	}
	//�J�[�\���̓���
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		itemNum = GetItemCount();
		selectCursor = (selectCursor + (itemNum - 1)) % itemNum;

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		itemNum = GetItemCount();
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// �J�[�\�����Ō�̗v�f�������Ɉړ����悤�Ƃ���ꍇ�́A���������ɏ������I������
		if (selectCursor == itemNum - 1) {
			return;
		}
		selectCursor = (selectCursor + 1) % itemNum;
	}
}

void Inventory::CursorReset()
{
	selectCursor = 0;
}

//Hp�񕜌n�̃A�C�e��
void Inventory::ItemHpHeal(float percentage , int itemid)
{
	auto curentHp = plyer->getPlayerStatusSave().getcurentHp();

	// �A�C�e��ID���񕜃A�C�e���ł��ő�Hp���Ⴏ��Ύ��s�ł���
	if (curentHp < max_hp) {

		healAmount = static_cast<int>(max_hp * percentage);
		plyer->getPlayerStatusSave().SetPlayerCurentHp(curentHp + healAmount);

		//�񕜉���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		// HP��MAX_HP�𒴂���ꍇ��MAX_HP�ɃN���b�v����
		if (plyer->getPlayerStatusSave().getcurentHp() > max_hp) {
			plyer->getPlayerStatusSave().SetPlayerCurentHp(max_hp);
		}

		// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
		auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
			[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

		// �A�C�e�������������ꍇ�͍폜����
		if (itemToRemove != InventoryList.end()) {
			InventoryList.erase(itemToRemove);
			itemNum--;
		}

	}
	//Hp��maxHp�Ɠ����ꍇ�������΂�
	else {
		return;
	}
}

//���j���[�̏�����
void Inventory::InitMenuWinodow()
{
	//�ŏ���ESC���������Ƃ��ɏo��ŏ��̃��j���[
	MenuWindow::MenuElement_t* first_menu_coment = new MenuWindow::MenuElement_t[]
	{
		{100, 100 , "����", 0},
		{100, 150 , "���Z", 1},
		{100, 200 , "����", 2},
		{100, 250 , "����" ,3}
	};

	first_menu = std::make_shared<MenuWindow>(50, 50, 250, 250, "graphics/WindowBase_01.png", first_menu_coment, 4 , 1);
	first_menu->Open();

	//�v���C���[�̑I�����Ă��炤���j���[
	MenuWindow::MenuElement_t* select_menu_coment = new MenuWindow::MenuElement_t[]
	{
		{ 100 , 100 , "�͂�" , 0},
		{ 100 , 150 , "������" , 1}
	};

	select_action_menu = std::make_shared<MenuWindow>(50, 50, 250, 200, "graphics/WindowBase_01.png", select_menu_coment, 2, 1);

	//�A�C�e���E�B���h�E���őI�����郁�j���[
	MenuWindow::MenuElement_t* select_detail = new MenuWindow::MenuElement_t[]
	{
		{400 , 90 , "�g��" , 0},
		{400 , 140 , "�ڍ�" , 1},
		{400 , 190 , "��������",2},
		{400 , 240 , "�̂Ă�", 3},
		{400 , 290 , "����" , 4}
	};

	select_detail_window = std::make_shared<MenuWindow>(350, 50, 250, 280, "graphics/WindowBase_01.png", select_detail, 5, 1);
	select_detail_window->Open();

	//���j���[�E�B���h�E
	menu_window = std::make_shared<Menu>(50, 50, 300, 270, "graphics/WindowBase_01.png");
	item_coment_window = std::make_shared<Menu>(600, 50, 300, 200, "graphics/WindowBase_01.png");
	plyer_status_window = std::make_shared<Menu>(50, 50, 300, 400, "graphics/WindowBase_01.png");
	SkilldetailWinodow = std::make_shared<Menu>(350, 50, 300, 250, "graphics/WindowBase_01.png");
	ItemDetailWindow = std::make_shared<Menu>(600, 50, 400, 250, "graphics/WindowBase_01.png");

}

//��ԍŏ��̃��j���[
void Inventory::First_Menu()
{
	first_menu->All();
}

//�A�C�e���̕\��
void Inventory::ItemMenu(const tnl::Vector2i& itemDrawPos, tnl::Vector2i& curentPageText, int CousourX , int itemParPage)
{
	int i = 0;
	int y = 34;
	int equipY = 30;


		//���݂̃y�[�W�Ɋ�Â��ĕ\������A�C�e���͈̔͂��v�Z
		start_index = currentPage * itemParPage;
		end_index = min((start_index + itemParPage) , InventoryList.size());

		for (int index = start_index; index < end_index; ++index) {
			//
			auto& InventoryData = InventoryList[index];
			//�A�C�e���̖��O���擾����
			std::string ItemName = InventoryData.getItemName();

			// �A�C�e���̕`��
			DrawStringEx(itemDrawPos.x, itemDrawPos.y + y * i, Color_White, "%s", ItemName.c_str());

			// �J�[�\����`�悷��ʒu������
			int cursorY = itemDrawPos.y + y * (selectCursor % itemParPage);
			ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CousourX, cursorY, cursorSize, 0, true);
		
			//�����ł���A�C�e���������ꍇ
			if (InventoryData.getItemType() == 1 || InventoryData.getItemType() == 2) {
				// �A�C�e������������Ă��邩�A�I�����ꂽ�A�C�e�������m�F
				if ((equipWeapon && InventoryData.getItemId() == equippedWeapon.back().getItemId())) {
					// �������̃A�C�e���܂��͑I�����ꂽ�A�C�e���̏ꍇ�A"[E]" ��\��
					DrawStringEx(itemDrawPos.x - 30, itemDrawPos.y + equipY * i, Color_White, "[E]");
				}
				if ((equipArmor && InventoryData.getItemId() == equippedArmor.back().getItemId())) {
					// �������̃A�C�e���܂��͑I�����ꂽ�A�C�e���̏ꍇ�A"[E]" ��\��
					DrawStringEx(itemDrawPos.x - 30 , itemDrawPos.y + equipY * i, Color_White, "[E]");
				}
				++i;
			}
			//�g�p�A�C�e���������ꍇ
			else
			{
				++i;
			}
			
		}
		//�y�[�W�̕\��
		DrawStringEx(curentPageText.x, curentPageText.y, Color_White , "%d / 4", currentPage + 1);

}

//�v���C���[�̃X�e�[�^�X�\��
void Inventory::PlyStatusMenu()
{
	plyer_status_window->Menu_draw();

	DrawStringEx(150, 70, -1, "===����===");

	// Player �N���X�̃|�C���^�[���L�����ǂ������m�F����
	if (plyer) {
		// Player �N���X�̃|�C���^�[����ăX�e�[�^�X���擾����
		int hp = plyer->getPlayerStatusSave().getcurentHp();
		int mp = plyer->getPlayerStatusSave().getCurentMp();
		int Attack = plyer->getPlayerStatusSave().getAttack();
		int Defense = plyer->getPlayerStatusSave().getDefance();
		int Level = plyer->getPlayerStatusSave().getLevel();
		int Speed = plyer->getPlayerStatusSave().getSpeed();
		int R_expoint = plyer->getPlayerStatusSave().getExpoint();

		// �擾�����X�e�[�^�X��`�悷��
		DrawStringEx(70, 100, Color_White, "HP : %d", hp);
		DrawStringEx(70, 150, Color_White, "MP : %d", mp);
		DrawStringEx(70, 200, Color_White, "�U���� : %d", Attack);
		DrawStringEx(70, 250, Color_White, "�h��� : %d", Defense);
		DrawStringEx(70, 300, Color_White, "���x�� : %d", Level);
		DrawStringEx(70, 350, Color_White, "���΂₳ : %d", Speed);
		DrawStringEx(70, 400, Color_White, "���x���A�b�v�ɕK�v�Ȍo���l : %d", R_expoint);
	}

}

void Inventory::ItemUseMenu()
{
	select_detail_window->All();
}

//�C���x���g���̏���
void Inventory::swichInventoryUpdate(float deltatime)
{
	switch (select_menu)
	{
	case Inventory::MenuWindow_I::Empty:


		break;

		//�ŏ��̃��j���[�ł̏���
	case Inventory::MenuWindow_I::First_Menu:

		//plyer�𓮂��Ȃ��悤�ɂ���
		plyer->setPlayerControl(PlyControlFalse);
			
		//1�ڂ��I�����ꂽ�ꍇ�A�A�C�e����\������
		if (first_menu->getSelectNum() == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ItemMenu;
			CursorReset();
			currentPage = 0;
			selectedIndex = 0;
		}
			//2�ڂ��I�����ꂽ�ꍇ�A���Z��\������
		else if (first_menu->getSelectNum() == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::SkillMenu;

		}
		//3�ڂ��I�����ꂽ�ꍇ�A�v���C���[�̋�����\������
		else if (first_menu->getSelectNum() == 2 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::StatusMenu;
		}
		//4�ڂ��I�����ꂽ�ꍇ�A����
		else if (first_menu->getSelectNum() == 3 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::Empty;
			//plyer�𓮂���悤�ɂ���
			plyer->setPlayerControl(PlyControlTrue);
			}

		break;

		//�A�C�e�����j���[�ł̏���
	case Inventory::MenuWindow_I::ItemMenu:

		//�A�C�e���̃R�����g������������
		item_coment = ItemComent::Empty;

		//Enter�L�[����������ItemUseMenu�ɕύX����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ItemUseMenu;
		}

		//�C���f�b�N�X����
		ItemCurourIndex(item_per_page);
		

		CusorMove();

		break;

	//�A�C�e���ڍ׃��j���[�̏ꍇ
	case Inventory::MenuWindow_I::ItemDetailMenu:

		//���L�[�������ƃA�C�e���g�p���j���[�ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::ItemUseMenu;
		}

		break;

	case Inventory::MenuWindow_I::StatusMenu:

		break;

		//�A�C�e���g�p���j���[
	case Inventory::MenuWindow_I::ItemUseMenu:

		//�g�����I�����ꂽ��
		if (select_detail_window->getSelectNum() == 0) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				//30%��Hp���񕜂�����(�|�[�V�������I�����ꂽ��)
				InventoryItemUse(selectedItemId);
			}
		}
		//�ڍׂ��I�����ꂽ��
		else if (select_detail_window->getSelectNum() == 1) {

			//�C���x���g������̎��͏������Ƃ΂�
			if (InventoryList.empty()) {
				return;
			}
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ItemDetailMenu;
			}
		}
		//�������I�����ꂽ��
		else if (select_detail_window->getSelectNum() == 2) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				EquipWeapon(selectedIndex);
			}
		}
		//�̂Ă邪�I�����ꂽ��
		else if (select_detail_window->getSelectNum() == 3) {
			
			//�A�C�e������̏ꍇ�������Ƃ΂�
			if (InventoryList.empty()) {
				return;
			}
			//�I������Enter�L�[����������
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

				// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
				auto itemid = InventoryList[selectedIndex].getItemId();

				auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
					[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

				// �A�C�e�������������ꍇ�͍폜����
				if (itemToRemove != InventoryList.end()) {
					InventoryList.erase(itemToRemove);
					itemNum--;
				}
			}
		}
		//���邪�I�����ꂽ��
		else if (select_detail_window->getSelectNum() == 4) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ItemMenu;
			}
		}
		
		break;

	//�X�L���\�����
	case MenuWindow_I::SkillMenu:

		//Enter�L�[����������ItemUseMenu�ɕύX����
		if (SkillNum > 0) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::SkillDatailMenu;
			}
		}

		//�X�L���̃J�[�\���ړ����̃C���f�b�N�X����
		SkillCurourIndex();


		//���Z�̃J�[�\���ړ�
		SkillCousorMove();

		break;

	//�X�L������
	case MenuWindow_I::SkillDatailMenu:

		//�X�L���I���ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::SkillMenu;
		}

		break;


	default:
		break;
	}

}

//�������̃X�e�[�^�X���擾����
int Inventory::getEquipStatus(int type)
{
	if (type == 0) {
		return equipAttack;
	}
	if (type == 1) {
		return equipDefance;
	}
}

//�X�L����ǉ�����
void Inventory::AddSkill(Shared<Skill>skill)
{
	SkillList.push_back(skill);
	SkillNum++;

}

//�X�L���̕`��
void Inventory::InventorySkill()
{
	// �E�B���h�E�̕\��
	menu_window->Menu_draw();

	int i = 0;
	int y = 34;

	//1�y�[�W������̃A�C�e����
	item_per_page = 5;

	//���݂̃y�[�W�Ɋ�Â��ĕ\������A�C�e���͈̔͂��v�Z
	start_index = SkillCurentPage * item_per_page;
	end_index = min((start_index + item_per_page), SkillList.size());

	for (int index = start_index; index < end_index; ++index) {
		//
		auto& SkillData = SkillList[index];
		//�A�C�e���̖��O���擾����
		string skillName = SkillData->getName();

		// �A�C�e���̕`��
		DrawStringEx(DrawPos.x, DrawPos.y + y * i, Color_White, "%s", skillName.c_str());

		// �J�[�\����`�悷��ʒu������
		int cursorY = DrawPos.y + y * (SkillCousour % item_per_page);
		ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", cursorX, cursorY, cursorSize, 0, true);

		++i;

	}

	////�y�[�W�̕\��
	DrawStringEx(CurentPageText.x, CurentPageText.y, Color_White, "%d / 4", SkillCurentPage + 1);

}

//�X�L�������̕`��
void Inventory::SkillDetailDraw(int skill_index)
{

	//���Z����
	SkilldetailWinodow->Menu_draw();

	DrawStringEx(SkilldetailWinodow->menu_x + 50, SkilldetailWinodow->menu_y + 20, Color_White, "==�X�L������==");

	//�X�L���f�[�^���擾
	auto& skillData = SkillList[skill_index];

	//�X�L���f�[�^�̃X�L���������擾
	string skillDatail = skillData->getDescription();
	
	//�X�L��������`��
	DrawStringEx(SkilldetailWinodow->menu_x + 30, SkilldetailWinodow->menu_y + 50, Color_White, "%s", skillDatail.c_str());

}

//�X�L���̃J�[�\������
void Inventory::SkillCousorMove()
{

	//�X�L�����Ȃ��ꍇ�A�������Ȃ�
	if (SkillNum == 0)
	{
		return;
	}
	//�J�[�\������ԏ�ɂ���ꍇ�J�[�\��������ɏ�ɉ����Ă��J�[�\���𓮂����Ȃ��悤�ɂ���
	if (SkillCousour == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP))
	{
		return;
	}
	//�J�[�\���̓���
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		SkillNum = SkillList.size();
		SkillCousour = (SkillCousour + (SkillNum - 1)) % SkillNum;

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		SkillNum = SkillList.size();
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// �J�[�\�����Ō�̗v�f�������Ɉړ����悤�Ƃ���ꍇ�́A���������ɏ������I������
		if (SkillCousour == SkillNum - 1) {
			return;
		}

		SkillCousour = (SkillCousour + 1) % SkillNum;
	}

}

//�X�L���J�[�\���̈ړ����̃C���f�b�N�X����
void Inventory::SkillCurourIndex()
{
	// ��L�[�������ꂽ�Ƃ��̏���
	if (SkillCurentPage == 0 && selectedSkillIndex == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// �������Ȃ�
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selectedSkillIndex % item_per_page == 0) {
			// �J�[�\�����y�[�W���̍ŏ㕔�ɂ���ꍇ�́A���̃y�[�W�̍Ō�̗v�f��I��
			if (SkillCurentPage > 0) {
				--SkillCurentPage;
				// �C���f�b�N�X���X�V���ăy�[�W�̍Ō�̗v�f��I������
				selectedSkillIndex = (SkillCurentPage + 1) * item_per_page - 1;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A���̗v�f��I��
			--selectedSkillIndex;
		}
	}

	// ���L�[�������ꂽ�Ƃ��̏���
	if (selectedSkillIndex == SkillList.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		// �������Ȃ�
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selectedSkillIndex + 1) % item_per_page == 0) {
			// �J�[�\�����y�[�W���̍ŉ����ɂ���ꍇ�́A����̃y�[�W�̍ŏ��̗v�f��I��
			// �X�L�������Ă�C���x���g���� 20 �ōl���Ă���� 4 �y�[�W���܂ŊJ����悤�ɂ���
			if (SkillCurentPage < 4) {
				++SkillCurentPage;
				// �C���f�b�N�X���X�V���ăy�[�W�̍ŏ��̗v�f��I������
				selectedSkillIndex = currentPage * item_per_page;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A����̗v�f��I��
			++selectedSkillIndex;
		}
	}
}

//�X�L�����Z�b�g����
void Inventory::SkillSet()
{
	switch (plyer->getPlayerStatusSave().getLevel())
	{
	//���x��1�̎��͉����o���Ȃ�
	case 1:
		break;
	//���x��2�̎��ɉΉ��a����o����
	case 2:
		AddSkill(std::make_shared<FlameSlash>());

		//���O�𗬂�
		battleLog->addLog("�Ή��a����o����!");

		break;
	//���x��3�̎��Ƀq�[�����o����
	case 3:
		AddSkill(std::make_shared<Heal>());

		battleLog->addLog("�q�[�����o����!");

		break;
	//���x��4�̎��ɃA�C�X�u���X�g���o����
	case 4:
		AddSkill(std::make_shared<IceBlast>());

		battleLog->addLog("�A�C�X�u���X�g���o����!");

		break;
	//���x��5�̎��ɃT���_�[�{���g���o����
	case 5:
		AddSkill(std::make_shared<ThunderBolt>());

		battleLog->addLog("�T���_�[�{���g���o����!");

	default:
		break;
	}
}

//�X�L����ǉ�����n�̃A�C�e��
template <class T>
void Inventory::AddSkillItem(int itemid ,Shared<T>skill)
{
	//---�X�L����ǉ�����---//

	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto SkillToRemove = std::find_if(SkillList.begin(), SkillList.end(),
		[itemid](const Shared<Skill>& skill) { return skill->getId() == itemid; });

	// �A�C�e�������������ꍇ�͏������Ƃ΂�
	if (SkillToRemove != SkillList.end()) {
		return;
	}

	// ������Ȃ������ꍇ�X�L����ǉ�����
	AddSkill(std::make_shared<T>());

	//---�A�C�e������������---//

	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto itemToRemove = std::find_if(InventoryList.begin(), InventoryList.end(),
		[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

	// �A�C�e�������������ꍇ�͍폜����
	if (itemToRemove != InventoryList.end()) {
		InventoryList.erase(itemToRemove);
		itemNum--;
	}
}

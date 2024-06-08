#include "Inventory.h"
#include"../Manager/GameManager.h"
#include"../Manager/UiManager.h"

Inventory::Inventory()
{
	item = std::make_shared<Item>();

	//���j���[�̏�����
	InitMenuWinodow();
}

Inventory::~Inventory()
{
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/cousour_bgm.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/kaihuku.mp3");

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
	//�A�C�e����\������ׂ̃o�b�N�E�B���h�E
	auto item_coment_window = menu_window.lock();

	if (select_menu == MenuWindow_I::EMPTY) {
		Game_Menu();
	}
	//�I������Ă���̂��ŏ��̃��j���[��������
	else if (select_menu == MenuWindow_I::FIRSTMENU) {
		First_Menu();
	}
	//�I�����ꂽ�̂��A�C�e�����j���[��������
	else if (select_menu == MenuWindow_I::ITEMMENU) {

		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x , MENU_WINDOW_POS.y , MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(DRAWPOS, CURENTPAGETEXT, CURSORX,ITEMPERPAGE);
	}
	//�I�����ꂽ�̂��v���C���[�̋������j���[��������
	else if (select_menu == MenuWindow_I::STATUSMENU) {
		PlyStatusMenu();
	}
	//�I�����ꂽ�̂��A�C�e���̏ڍ׃��j���[��������
	else if (select_menu == MenuWindow_I::ITEMUSEMENU) {
		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(DRAWPOS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		ItemUseMenu();
		if (!can_used_item) {
			if (item_coment_window) {
				item_coment_window->Menu_draw(600, 50, 300, 200);
				DrawStringEx(ITEMCOMENTPOS.x, ITEMCOMENTPOS.y, COLORWHITE, "���̃A�C�e���͎g�p�ł��܂���");
			}
		}
	}
	else if (select_menu == MenuWindow_I::ITEMDETAILMENU) {
		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(DRAWPOS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		ItemUseMenu();
		ItemDetail();
	}
	else if (select_menu == MenuWindow_I::SKILLMENU) {
		// �E�B���h�E�̕\��
		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		InventorySkill(DRAWPOS , CURENTPAGETEXT , CURSORX ,ITEMPERPAGE);

	}
	else if (select_menu == MenuWindow_I::SKILLDATAILMENU) {
		// �E�B���h�E�̕\��
		menu_window.lock()->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		InventorySkill(DRAWPOS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		SkillDetailDraw(skill_selected_index);
	}
	
}

//�A�C�e����ǉ�����
void Inventory::AddInventory(int id)
{
	if (inventory_list.size() == inventory_max_size_) return;
	ItemBase newItem = item->GetItemById(id);
	inventory_list.emplace_back(newItem);
	item_num++;
}

//����𑕔�����
void Inventory::EquipWeapon(int weaponIndex)
{
	//�v���C���[���擾����
	auto& player = GameManager::getGameManager()->getPlayer();

	// �A�C�e�����Ȃ��ꍇ��C���f�b�N�X���s���ȏꍇ�͉��������Ɋ֐����I��
	if (item_num == 0 || weaponIndex < 0 || weaponIndex >= item_num) {
		return;
	}

	// �I�����ꂽ�A�C�e�����擾
	ItemBase selectedItem = inventory_list[weaponIndex];

	// �����A�C�e�������ɑ�������Ă��邩�`�F�b�N
	bool isAlreadyEquipped = false;
	for (const auto& equipped : equipped_weapon) {
		if (equipped.getItemId() == selectedItem.getItemId()) {
			// �����A�C�e������������Ă���ꍇ�A���������Ɋ֐����I��
			isAlreadyEquipped = true;
			break;
		}
	}

	for (const auto& equipped : equipped_armor) {
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
		auto curentAttack = player->getPlayerStatusSave().GetAttack();
		// �������̕�����O��
		if (!equipped_weapon.empty()) {
			// �������̕���̌��ʂ𔽉f����폜����
			ItemBase equippedWeaponItem = equipped_weapon.back();
			//�������̕���̃X�e�[�^�X���폜����
			curentAttack -= equippedWeaponItem.getItemDamage();
			// �������̕�����폜����
			equipped_weapon.pop_back();
		}
		
		// �V��������𑕔�
		equipped_weapon.push_back(selectedItem);
		//���������ۂɃv���C���[�̉摜��ς���
		player->SetPlayerAnimationHdl(selectedItem.getItemWeapontype());
		// �v���C���[�̃X�e�[�^�X�ɐV��������̌��ʂ𔽉f
		equip_weapon = true;
		equip_attack = selectedItem.getItemDamage();
		player->getPlayerStatusSave().SetPlayerAttack(curentAttack + equip_attack);
	}
	else if (selectedItem.getItemType() == ARMOR) {
		auto curentDefance = player->getPlayerStatusSave().GetDefance();
		// �������̖h����O��
		if (!equipped_armor.empty()) {
			// �������̖h��̌��ʂ𔽉f����폜����
			ItemBase equippedArmorItem = equipped_armor.back();
			//�������̖h��̃X�e�[�^�X���폜����
			curentDefance -= equippedArmorItem.getItemDefance();
			// �������̖h����폜����
			equipped_armor.pop_back();
		}
		// �V�����h��𑕔�
		equipped_armor.push_back(selectedItem);
		// �v���C���[�̃X�e�[�^�X�ɐV�����h��̌��ʂ𔽉f
		equip_armor = true;
		equip_defance = selectedItem.getItemDefance();
		player->getPlayerStatusSave().SetPlayerDefance(curentDefance + equip_defance);
	}
}

//�A�C�e���̏ڍׂ��J�������̏���
void Inventory::ItemDetail()
{
	//�A�C�e���̐������o�����߂̃o�b�N�E�B���h�E
	auto item_detail_window = menu_window.lock();


	item_detail_window->Menu_draw(ITEM_DETAIL_WINDOW_POS.x, ITEM_DETAIL_WINDOW_POS.y, ITEM_DETAIL_WINDOW_WIDTH, ITEM_DETAIL_WINDOW_HEIGHT);

	DrawStringEx(630, 70, -1, "===�A�C�e������===");

	//�A�C�e���������擾����
	std::string ItemDetail = inventory_list[selected_index].getItemDetail();

	//�C���x���g�����󂶂�Ȃ��ꍇ
	if (!inventory_list.empty()) {
	//�A�C�e���̏ڍׂ�\������
		DrawStringEx(ITEM_DETAIL_POS.x, ITEM_DETAIL_POS.y, COLORWHITE, "%s" ,ItemDetail.c_str());
	}

}

//�J�[�\���ړ����̃C���f�b�N�X����Ǝ擾
void Inventory::ItemCurourIndex(int ItemPerPage)
{
	//---�I�������A�C�e���ɂ�ID�Ƃ��Ď擾����---//

	// ��L�[�������ꂽ�Ƃ��̏���
	//��ԏ�ɃJ�[�\���������ꍇ����ȏ�ɂ����Ȃ��悤�ɂ���
	if (curent_page == 0 && selected_index == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// �������Ȃ�
		return;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selected_index % ItemPerPage == 0) {
			// �J�[�\�����y�[�W���̍ŏ㕔�ɂ���ꍇ�́A���̃y�[�W�̍Ō�̗v�f��I��
			if (curent_page > 0) {
				--curent_page;
				// �C���f�b�N�X���X�V���ăy�[�W�̍Ō�̗v�f��I������
				selected_index = (curent_page + 1) * ItemPerPage - 1;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A���̗v�f��I��
			--selected_index;
		}
	}

	// ���L�[�������ꂽ�Ƃ��̏���
	if (selected_index == inventory_list.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN))return;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selected_index + 1) % ItemPerPage == 0) {
			// �J�[�\�����y�[�W���̍ŉ����ɂ���ꍇ�́A����̃y�[�W�̍ŏ��̗v�f��I��
			//���̏��v���C���[�����Ă�C���x���g����20�ōl���Ă����4�y�[�W���܂ŊJ����悤�ɂ���
			if (curent_page < ITEM_MAX_PAGE) {
				++curent_page;
				// �C���f�b�N�X���X�V���ăy�[�W�̍ŏ��̗v�f��I������
				selected_index = curent_page * ItemPerPage;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A����̗v�f��I��
			++selected_index;
		}
	}

	// selectedIndex���L���Ȕ͈͓��ɂ��邩�m�F���āA�I�����ꂽ�A�C�e����ID���擾����
	if (selected_index >= 0 && selected_index < inventory_list.size()) {
		ItemBase selectedItem = inventory_list[selected_index];
		selected_item_id = selectedItem.getItemId();
	}
}

void Inventory::CusorMove()
{
	//�A�C�e�����Ȃ��ꍇ�A�������Ȃ�
	if (inventory_list.empty())
	{
		return;
	}
	//�J�[�\������ԏ�ɂ���ꍇ�J�[�\��������ɏ�ɉ����Ă��J�[�\���𓮂����Ȃ��悤�ɂ���
	if (select_cursor == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) 
	{
		return;
	}
	//�J�[�\���̓���
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		item_num = static_cast<int>(inventory_list.size());
		select_cursor = (select_cursor + (item_num - 1)) % item_num;

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		item_num = static_cast<int>(inventory_list.size());
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// �J�[�\�����Ō�̗v�f�������Ɉړ����悤�Ƃ���ꍇ�́A���������ɏ������I������
		if (select_cursor == item_num - 1) {
			return;
		}
		select_cursor = (select_cursor + 1) % item_num;
	}
}

//�J�[�\�������Z�b�g����
void Inventory::CursorReset()
{
	select_cursor = 0;
}

//���j���[�̏�����
void Inventory::InitMenuWinodow()
{
	//�ŏ���ESC���������Ƃ��ɏo��ŏ��̃��j���[
	MenuWindow::MenuElement_t* first_menu_coment = new MenuWindow::MenuElement_t[]
	{
		{MENU_TOOL_POS.x, MENU_TOOL_POS.y , "����", 0},
		{MENU_SKILL_POS.x, MENU_SKILL_POS.y , "���Z", 1},
		{MENU_STATUS_POS.x, MENU_STATUS_POS.y , "����", 2},
		{MENU_CLOSE_POS.x, MENU_CLOSE_POS.y , "����" ,3}
	};

	first_menu = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", first_menu_coment, 4 , 1);
	first_menu->Open();

	//map�ɒǉ�����
	UIManager::getUIManager()->addMenu("select_answer_window", select_action_menu);

	//�A�C�e���E�B���h�E���őI�����郁�j���[
	MenuWindow::MenuElement_t* select_detail = new MenuWindow::MenuElement_t[]
	{
		{MENU_ITEM_USE_POS.x , MENU_ITEM_USE_POS.y , "�g��" , 0},
		{MENU_ITEM_DETAIL_POS.x , MENU_ITEM_DETAIL_POS.y , "�ڍ�" , 1},
		{MENU_ITEM_EQUIP_POS.x , MENU_ITEM_EQUIP_POS.y , "��������",2},
		{MENU_ITEM_DISPOSE_POS.x , MENU_ITEM_DISPOSE_POS.y , "�̂Ă�", 3},
		{MENU_ITEM_CLOSE_POS.x , MENU_ITEM_CLOSE_POS.y , "����" , 4}
	};

	select_detail_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_detail, 5, 1);
	select_detail_window->Open();


	//���j���[�E�B���h�E
	menu_window = UIManager::getUIManager()->getMenu("menu_window");
}

//�v���C���[�������Ă��鎞�̃��j���[�\��
void Inventory::Game_Menu()
{
	//�v���C���[���擾����
	auto& player = GameManager::getGameManager()->getPlayer();

	//�S�[���h��\������ׂ̃o�b�N�E�B���h�E
	auto gold_display_window = menu_window.lock();

	if (gold_display_window) {
		gold_display_window->Menu_draw(GOLD_DISPLAY_WINDOW_POS.x, GOLD_DISPLAY_WINDOW_POS.y, GOLD_DISPLAY_WINDOW_WIDTH, GOLD_DISPLAY_WINDOW_HEIGHT);
	}

	DrawStringEx(1030, 65, -1, "G : %d", player->getPlayerMoney());

	//�v���C���[��Hp�Ȃǂ�\������E�B���h�E
	//auto player_status_window_ = menu_window.lock();

	//if (player_status_window_) {
	//	player_status_window_->Menu_draw(50, 50, 250, 240);
	//}

	//UIManager::getUIManager()->PlayerStatusDrawWindow();

	auto esc_key_string_window = menu_window.lock();

	if (esc_key_string_window) {
		esc_key_string_window->Menu_draw(50, 50, 250, 50);
	}

	DrawString(60, 65,"ESC�L�[�ŃC���x���g����\��" ,-1);
}

//��ԍŏ��̃��j���[
void Inventory::First_Menu()
{
	first_menu->All(50, 50, 250, 250);
}

//�A�C�e���̕\��
void Inventory::ItemMenu(const tnl::Vector2i& itemDrawPos, const tnl::Vector2i& curentPageText, int CousourX , int itemParPage)
{
	int i = 0;
	const int Y = 34;
	const int EQUIP_Y_POS = 30;
	const int EQUIP_X_POS = 30;


		//���݂̃y�[�W�Ɋ�Â��ĕ\������A�C�e���͈̔͂��v�Z
		start_index = curent_page * itemParPage;
		end_index = min((start_index + itemParPage) , static_cast<int>(inventory_list.size()));

		for (int index = start_index; index < end_index; ++index) {
			//
			auto& InventoryData = inventory_list[index];
			//�A�C�e���̖��O���擾����
			std::string ItemName = InventoryData.getItemName();

			// �A�C�e���̕`��
			DrawStringEx(itemDrawPos.x, itemDrawPos.y + Y * i, COLORWHITE, "%s", ItemName.c_str());

			// �J�[�\����`�悷��ʒu������
			int cursorY = itemDrawPos.y + Y * (select_cursor % itemParPage);
			ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CousourX, cursorY, CURSORSIZE, 0, true);
		
			//�����ł���A�C�e���������ꍇ
			if (InventoryData.getItemType() == 1 || InventoryData.getItemType() == 2) {
				// �A�C�e������������Ă��邩�A�I�����ꂽ�A�C�e�������m�F
				if ((equip_weapon && InventoryData.getItemId() == equipped_weapon.back().getItemId())) {
					// �������̃A�C�e���܂��͑I�����ꂽ�A�C�e���̏ꍇ�A"[E]" ��\��
					//�A�C�e���̖��O���獶��30���ꂽ���W�ɕ\��
					DrawStringEx(itemDrawPos.x - EQUIP_X_POS, itemDrawPos.y + EQUIP_Y_POS * i, COLORWHITE, "[E]");
				}
				if ((equip_armor && InventoryData.getItemId() == equipped_armor.back().getItemId())) {
					// �������̃A�C�e���܂��͑I�����ꂽ�A�C�e���̏ꍇ�A"[E]" ��\��
					//�A�C�e���̖��O���獶��30���ꂽ���W�ɕ\��
					DrawStringEx(itemDrawPos.x - EQUIP_X_POS, itemDrawPos.y + EQUIP_Y_POS * i, COLORWHITE, "[E]");
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
		DrawStringEx(curentPageText.x, curentPageText.y, COLORWHITE , "%d / 4", curent_page + 1);

}

//�v���C���[�̃X�e�[�^�X�\��
void Inventory::PlyStatusMenu()
{

	//�v���C���[���擾����
	auto& player = GameManager::getGameManager()->getPlayer();

	//�v���C���[�̃X�e�[�^�X��\������ׂ̃o�b�N�E�B���h�E
	auto plyer_status_window = menu_window.lock();

	if (plyer_status_window) {
		plyer_status_window->Menu_draw(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, PLAYER_STATUS_WINDOW_WIDTH, PLAYER_STATUS_WINDOW_HEIGHT);
	}

	//���������̍��W
	const tnl::Vector2i PLAYER_STATUS_STRING = { 150 , 70 };
	
	//�����̕�����\������
	DrawStringEx(PLAYER_STATUS_STRING.x, PLAYER_STATUS_STRING.y, -1, "===����===");

	//�ǉ����Ă������W
	const int Y = 50;

	// Player �N���X�̃|�C���^�[���L�����ǂ������m�F����
	if (player) {
		// Player �N���X�̃|�C���^�[����ăX�e�[�^�X���擾����
		int hp = static_cast<int>(player->getPlayerStatusSave().GetcurentHp());
		int mp = static_cast<int>(player->getPlayerStatusSave().GetCurentMp());
		int Attack = player->getPlayerStatusSave().GetAttack();
		int Defense = player->getPlayerStatusSave().GetDefance();
		int Level = player->getPlayerStatusSave().GetLevel();
		int Speed = player->getPlayerStatusSave().GetSpeed();
		int R_expoint = player->getPlayerStatusSave().GetExpoint();

		// �擾�����X�e�[�^�X��`�悷��
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y, COLORWHITE, "HP : %d", hp);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + Y, COLORWHITE, "MP : %d", mp);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 2), COLORWHITE, "�U���� : %d", Attack);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 3), COLORWHITE, "�h��� : %d", Defense);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 4), COLORWHITE, "���x�� : %d", Level);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 5), COLORWHITE, "���΂₳ : %d", Speed);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * 6), COLORWHITE, "���x���A�b�v�ɕK�v�Ȍo���l : %d", R_expoint);
	}

}

void Inventory::ItemUseMenu()
{
	select_detail_window->All(350, 50, 250, 280);

}

//�C���x���g���̏���
void Inventory::swichInventoryUpdate(float deltatime)
{
	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	switch (select_menu)
	{
	case Inventory::MenuWindow_I::EMPTY:

		break;

		//�ŏ��̃��j���[�ł̏���
	case Inventory::MenuWindow_I::FIRSTMENU:
	
		//1�ڂ��I�����ꂽ�ꍇ�A�A�C�e����\������
		if (first_menu->getSelectNum() == TOOL && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ITEMMENU;
			CursorReset();
			curent_page = 0;
			selected_index = 0;
		}
			//2�ڂ��I�����ꂽ�ꍇ�A���Z��\������
		else if (first_menu->getSelectNum() == SKILL && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::SKILLMENU;

		}
		//3�ڂ��I�����ꂽ�ꍇ�A�v���C���[�̋�����\������
		else if (first_menu->getSelectNum() == STATUS && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::STATUSMENU;
		}
		//4�ڂ��I�����ꂽ�ꍇ�A����
		else if (first_menu->getSelectNum() == CLOSE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::EMPTY;
			//plyer�𓮂��Ȃ��悤�ɂ���
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
		
			}

		break;

		//�A�C�e�����j���[�ł̏���
	case Inventory::MenuWindow_I::ITEMMENU:

		//Enter�L�[����������ItemUseMenu�ɕύX����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ITEMUSEMENU;
		}
		//���L�[�������ƃA�C�e���g�p���j���[�ɖ߂�
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		//�C���f�b�N�X����
		ItemCurourIndex(ITEMPERPAGE);
		

		CusorMove();

		break;

	//�A�C�e���ڍ׃��j���[�̏ꍇ
	case Inventory::MenuWindow_I::ITEMDETAILMENU:

		//���L�[�������ƃA�C�e���g�p���j���[�ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::ITEMUSEMENU;
		}

		break;

		//�A�C�e���g�p���j���[
	case Inventory::MenuWindow_I::ITEMUSEMENU:

		//�g�����I�����ꂽ��
		if (select_detail_window->getSelectNum() == USE) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				//item���g�p����
				item->ItemUse(selected_item_id);
			}
		}
		//�ڍׂ��I�����ꂽ��
		else if (select_detail_window->getSelectNum() == DETAILE) {

			//�C���x���g������̎��͏������Ƃ΂�
			if (inventory_list.empty()) {
				return;
			}
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ITEMDETAILMENU;
			}
		}
		//�������I�����ꂽ��
		else if (select_detail_window->getSelectNum() == EQUIP) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				EquipWeapon(selected_index);
			}
		}
		//�̂Ă邪�I�����ꂽ��
		else if (select_detail_window->getSelectNum() == DISPOSE) {
			
			//�A�C�e������̏ꍇ�������Ƃ΂�
			if (inventory_list.empty()) {
				return;
			}
			//�I������Enter�L�[����������
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

				// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
				auto itemid = inventory_list[selected_index].getItemId();

				auto itemToRemove = std::find_if(inventory_list.begin(), inventory_list.end(),
					[itemid](const ItemBase& item) { return item.getItemId() == itemid; });

				// �A�C�e�������������ꍇ�͍폜����
				if (itemToRemove != inventory_list.end()) {
					inventory_list.erase(itemToRemove);
					item_num--;
				}
			}
		}
		//���邪�I�����ꂽ��
		else if (select_detail_window->getSelectNum() == ITEMCLOSE) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ITEMMENU;
			}
		}
		
		break;

	case MenuWindow_I::STATUSMENU:

		//���L�[�������ƃA�C�e���g�p���j���[�ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		break;

	//�X�L���\�����
	case MenuWindow_I::SKILLMENU:

		//Enter�L�[����������ItemUseMenu�ɕύX����
		//�v�f���󂶂ᖳ�����
		if (!SkillList.empty()) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::SKILLDATAILMENU;
			}

		}

		//�X�L���̃J�[�\���ړ����̃C���f�b�N�X����
		SkillCurourIndex();


		//���Z�̃J�[�\���ړ�
		SkillCousorMove();

		//���L�[�������ƃA�C�e���g�p���j���[�ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		break;

	//�X�L������
	case MenuWindow_I::SKILLDATAILMENU:

		//�X�L���I���ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_menu = MenuWindow_I::SKILLMENU;
		}

		break;


	default:
		break;
	}

}

//�X�L���̕`��
void Inventory::InventorySkill(const tnl::Vector2i& SKILLDRAWPOS, const tnl::Vector2i& CURENTPAGETEXT, const int& COUSOURX, const int& ITEMPARPAGE)
{
	int i = 0;
	int y = 34;

	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	//���݂̃y�[�W�Ɋ�Â��ĕ\������A�C�e���͈̔͂��v�Z
	start_index = skill_curent_page * ITEMPARPAGE;
	end_index = min((start_index + ITEMPARPAGE), static_cast<int>(SkillList.size()));

	for (int index = start_index; index < end_index; ++index) {
		//
		auto& SkillData = SkillList[index];

		//�A�C�e���̖��O���擾����
		std::string skillName = SkillData->getName();

		// �A�C�e���̕`��
		DrawStringEx(SKILLDRAWPOS.x, SKILLDRAWPOS.y + y * i, COLORWHITE, "%s", skillName.c_str());

		// �J�[�\����`�悷��ʒu������
		int cursorY = SKILLDRAWPOS.y + y * (skill_cousour % ITEMPARPAGE);
		ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", COUSOURX, cursorY, CURSORSIZE, 0, true);

		++i;

	}

	//�y�[�W�̕\��
	DrawStringEx(CURENTPAGETEXT.x, CURENTPAGETEXT.y, COLORWHITE, "%d / 4", skill_curent_page + 1);

}

//�X�L�������̕`��
void Inventory::SkillDetailDraw(int skill_index)
{
	//�X�L���z��
	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	//�X�L�������p�̃o�b�N�E�B���h�E
	auto skill_detail_window = menu_window.lock();
	
	//���Z����
	skill_detail_window->Menu_draw(SKILL_DETAIL_WINDOW_POS.x, SKILL_DETAIL_WINDOW_POS.y, SKILL_DETAIL_WINDOW_WIDTH, SKILL_DETAIL_WINDOW_HEIGHT);

	//�X�L�������̕����p���W
	const tnl::Vector2i SUKILL_STRING_POS = { 400, 70 };

	DrawStringEx(SUKILL_STRING_POS.x, SUKILL_STRING_POS.y, COLORWHITE, "==�X�L������==");

	//�X�L���f�[�^���擾
	auto& skillData = SkillList[skill_index];

	//�X�L���f�[�^�̃X�L���������擾
	std::string skillDatail = skillData->getDescription();
	
	//�X�L��������`��
	DrawStringEx(380,100, COLORWHITE, "%s", skillDatail.c_str());

}

//�X�L���̃J�[�\������
void Inventory::SkillCousorMove()
{

	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	//�X�L�����Ȃ��ꍇ�A�������Ȃ�
	if (SkillList.empty())
	{
		return;
	}
	//�J�[�\������ԏ�ɂ���ꍇ�J�[�\��������ɏ�ɉ����Ă��J�[�\���𓮂����Ȃ��悤�ɂ���
	if (skill_cousour == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP))
	{
		return;
	}
	//�J�[�\���̓���
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		skill_cousour = (skill_cousour + (SkillList.size() - 1)) % static_cast<int>(SkillList.size());

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// �J�[�\�����Ō�̗v�f�������Ɉړ����悤�Ƃ���ꍇ�́A���������ɏ������I������
		if (skill_cousour == SkillList.size() - 1) {
			return;
		}

		skill_cousour = (skill_cousour + 1) % SkillList.size();
	}

}

//�X�L���J�[�\���̈ړ����̃C���f�b�N�X����
void Inventory::SkillCurourIndex()
{

	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList();

	// ��L�[�������ꂽ�Ƃ��̏���
	if (skill_curent_page == 0 && skill_selected_index == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// �������Ȃ�
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (skill_selected_index % ITEMPERPAGE == 0) {
			// �J�[�\�����y�[�W���̍ŏ㕔�ɂ���ꍇ�́A���̃y�[�W�̍Ō�̗v�f��I��
			if (skill_curent_page > 0) {
				--skill_curent_page;
				// �C���f�b�N�X���X�V���ăy�[�W�̍Ō�̗v�f��I������
				skill_selected_index = (skill_curent_page + 1) * ITEMPERPAGE - 1;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A���̗v�f��I��
			--skill_selected_index;
		}
	}

	// ���L�[�������ꂽ�Ƃ��̏���
	if (skill_selected_index == SkillList.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		// �������Ȃ�
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((skill_selected_index + 1) % ITEMPERPAGE == 0) {
			// �J�[�\�����y�[�W���̍ŉ����ɂ���ꍇ�́A����̃y�[�W�̍ŏ��̗v�f��I��
			// �X�L�������Ă�C���x���g���� 20 �ōl���Ă���� 4 �y�[�W���܂ŊJ����悤�ɂ���
			if (skill_curent_page < ITEM_MAX_PAGE) {
				++skill_curent_page;
				// �C���f�b�N�X���X�V���ăy�[�W�̍ŏ��̗v�f��I������
				skill_selected_index = curent_page * ITEMPERPAGE;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A����̗v�f��I��
			++skill_selected_index;
		}
	}
}


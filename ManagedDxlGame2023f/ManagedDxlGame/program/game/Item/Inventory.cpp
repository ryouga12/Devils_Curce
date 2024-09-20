#include "Inventory.h"
#include"../Manager/GameManager.h"
#include"../Manager/UiManager.h"

Inventory::Inventory()
{
	item = std::make_shared<Item>();

	//���j���[�̏�����
	InitMenuWinodow();

}
//�X�V����
void Inventory::Update(const float& delta_time)
{
	//�C���x���g���̍X�V
	swichInventoryUpdate();

	//�v���C���[�̃X�e�[�^�X�̍X�V����
	UIManager::GetUIManager()->PlayerStatusBarUpdate(delta_time);

}

//�`�揈��
void Inventory::draw()
{
	
	if (select_menu == MenuWindow_I::EMPTY) {
		Game_Menu();
	}
	//�I������Ă���̂��ŏ��̃��j���[�̕`��
	else if (select_menu == MenuWindow_I::FIRSTMENU) {
		First_Menu();
	}
	//�I�����ꂽ�̂��A�C�e�����j���[�̕`��
	else if (select_menu == MenuWindow_I::ITEMMENU) {

		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(DRAWPOS, CURENTPAGETEXT, CURSORX,ITEMPERPAGE);
	}
	//�I�����ꂽ�̂��v���C���[�̋������j���[�̕`��
	else if (select_menu == MenuWindow_I::STATUSMENU) {
		PlyStatusMenu();
	}
	//�I�����ꂽ�̂��A�C�e���̏ڍ׃��j���[�̕`��
	else if (select_menu == MenuWindow_I::ITEMUSEMENU) {
		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(DRAWPOS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		ItemUseMenu();
	}
	//�A�C�e�����g�p����ۂ̃E�B���h�E�̃��j���[�̕`��
	else if (select_menu == MenuWindow_I::ITEMDETAILMENU) {
		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		ItemMenu(DRAWPOS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		ItemUseMenu();
		ItemDetail();
	}
	//�X�L���\���p�̃E�B���h�E�̕`��
	else if (select_menu == MenuWindow_I::SKILLMENU) {
		// �E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		InventorySkill(DRAWPOS , CURENTPAGETEXT , CURSORX ,ITEMPERPAGE);

	}
	//�X�L�������p�̃E�B���h�E�̕`��
	else if (select_menu == MenuWindow_I::SKILLDATAILMENU) {
		// �E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window",MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		InventorySkill(DRAWPOS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE);
		SkillDetailDraw(skill_selected_index);
	}
	//�C���x���g���g�p���ɑ�������p�̕`��
	if (select_menu != MenuWindow_I::EMPTY) {

		UIManager::GetUIManager()->Menu_Draw("menu_window", BUTTON_DETAIL_WINDOW_POS.x, BUTTON_DETAIL_WINDOW_POS.y, BUTTON_DETAIL_WINDOW_WIDTH, BUTTON_DETAIL_WINDOW_HEIGHT);

		DrawStringEx(BUTTON_BACK_STRING_POS.x, BUTTON_BACK_STRING_POS.y, koni::Color::WHITE, "back space : �߂�");
	}
	
}

//�A�C�e����ǉ�����
void Inventory::AddInventory(const int& id)
{
	if (inventory_list.size() == INVENTORY_MAX_SIZE_) return;
	ItemBase newItem = item->GetItemById(id);
	inventory_list.emplace_back(newItem);
	item_num++;
}

//����𑕔�����
void Inventory::EquipWeapon(const int& weaponIndex)
{
	//�v���C���[���擾����
	auto& player = GameManager::GetGameManager()->GetPlayer();

	// �A�C�e�����Ȃ��ꍇ��C���f�b�N�X���s���ȏꍇ�͉��������Ɋ֐����I��
	if (item_num == 0 || weaponIndex < 0 || weaponIndex >= item_num) {
		return;
	}

	// �I�����ꂽ�A�C�e�����擾
	ItemBase selectedItem = inventory_list[weaponIndex];

	// �����A�C�e�������ɑ�������Ă��邩�`�F�b�N
	bool isAlreadyEquipped = false;
	for (const auto& equipped : equipped_weapon) {
		if (equipped.GetItemId() == selectedItem.GetItemId()) {
			// �����A�C�e������������Ă���ꍇ�A���������Ɋ֐����I��
			isAlreadyEquipped = true;
			break;
		}
	}

	for (const auto& equipped : equipped_armor) {
		if (equipped.GetItemId() == selectedItem.GetItemId()) {
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
	if (selectedItem.GetItemType() == WEAPON) {
		auto base_attack = player->GetPlayerStatusSave().GetBaseAttack();
		// �������̕�����O��
		if (!equipped_weapon.empty()) {
			// �������̕�����폜����
			equipped_weapon.pop_back();
		}
		
		// �V��������𑕔�
		equipped_weapon.push_back(selectedItem);
		//���������ۂɃv���C���[�̉摜��ς���
		player->SetPlayerAnimationHdl(selectedItem.GetItemWeapontype() , player->GetPlayerID());
		// �v���C���[�̃X�e�[�^�X�ɐV��������̌��ʂ𔽉f
		equip_weapon = true;
		equip_attack = selectedItem.GetItemDamage();
		player->GetPlayerStatusSave().SetPlayerAttack(base_attack + equip_attack);
	}
	else if (selectedItem.GetItemType() == ARMOR) {
		auto base_defance = player->GetPlayerStatusSave().GetBaseDefance();
		// �������̖h����O��
		if (!equipped_armor.empty()) {
			// �������̖h����폜����
			equipped_armor.pop_back();
		}
		// �V�����h��𑕔�
		equipped_armor.push_back(selectedItem);
		// �v���C���[�̃X�e�[�^�X�ɐV�����h��̌��ʂ𔽉f
		equip_armor = true;
		equip_defance = selectedItem.GetItemDefance();
		player->GetPlayerStatusSave().SetPlayerDefance(base_defance + equip_defance);
	}
}

//�A�C�e���̏ڍׂ��J�������̏���
void Inventory::ItemDetail()
{
	//�E�B���h�E�̕`��
	UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_DETAIL_WINDOW_POS.x, ITEM_DETAIL_WINDOW_POS.y, ITEM_DETAIL_WINDOW_WIDTH, ITEM_DETAIL_WINDOW_HEIGHT);

	//�A�C�e�������̕�����\��������W
	const tnl::Vector2i ITEM_DETAIL = { 630 , 70 };

	DrawStringEx(ITEM_DETAIL.x, ITEM_DETAIL.y, koni::Color::WHITE, "===�A�C�e������===");

	//�A�C�e���������擾����
	auto& inventory_item = inventory_list[selected_index];

	//�C���x���g�����󂶂�Ȃ��ꍇ
	if (!inventory_list.empty()) {

		//�A�C�e���̏ڍׂ�\������
		DrawStringEx(ITEM_DETAIL_POS.x, ITEM_DETAIL_POS.y, koni::Color::WHITE, "%s" , inventory_item.GetItemDetail().c_str());
		//�A�C�e���̃X�e�[�^�X��\������
		DrawStringEx(ITEM_STAUS_POS.x, ITEM_STAUS_POS.y, koni::Color::WHITE, "�U���� : %d", inventory_item.GetItemDamage());
		DrawStringEx(ITEM_STAUS_POS.x + ITEM_ADD_OFSET_, ITEM_STAUS_POS.y, koni::Color::WHITE, "�h��� : %d", inventory_item.GetItemDefance());

	}

}

//�J�[�\���ړ����̃C���f�b�N�X����Ǝ擾
void Inventory::ItemCurourIndex(const int& ItemPerPage)
{
	//---�I�������A�C�e���ɂ�ID�Ƃ��Ď擾����---//

	// ��L�[�������ꂽ�Ƃ��̏���
	//��ԏ�ɃJ�[�\���������ꍇ����ȏ�ɂ����Ȃ��悤�ɂ���
	if (curent_page == 0 && selected_index == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		// �������Ȃ�
		return;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
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
	if (selected_index == inventory_list.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) { return; }
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
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
		selected_item_id = selectedItem.GetItemId();
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
	if (select_cursor == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) 
	{
		return;
	}
	//�J�[�\���̓���
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		item_num = static_cast<int>(inventory_list.size());
		select_cursor = (select_cursor + (item_num - 1)) % item_num;

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		item_num = static_cast<int>(inventory_list.size());
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
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
		{MENU_TOOL_POS.x,   MENU_TOOL_POS.y ,	 "����",    MenuWindow::Elements::FIRST_ELEMENT },
		{MENU_SKILL_POS.x,  MENU_SKILL_POS.y ,   "���Z",    MenuWindow::Elements::SECOND_ELEMENT},
		{MENU_STATUS_POS.x, MENU_STATUS_POS.y ,  "����",    MenuWindow::Elements::THERD_ELEMENT},
		{MENU_CLOSE_POS.x,  MENU_CLOSE_POS.y ,   "����" , MenuWindow::Elements::FOUR_ELEMENT}
	};

	first_menu = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", first_menu_coment, 4);
	first_menu->Open();

	//map�ɒǉ�����
	UIManager::GetUIManager()->addMenu("select_answer_window", select_action_menu);

	//�A�C�e���E�B���h�E���őI�����郁�j���[
	MenuWindow::MenuElement_t* select_detail = new MenuWindow::MenuElement_t[]
	{
		{MENU_ITEM_USE_POS.x ,    MENU_ITEM_USE_POS.y ,     "�g��" ,    MenuWindow::Elements::FIRST_ELEMENT},
		{MENU_ITEM_DETAIL_POS.x , MENU_ITEM_DETAIL_POS.y ,  "�ڍ�" ,    MenuWindow::Elements::SECOND_ELEMENT},
		{MENU_ITEM_EQUIP_POS.x ,  MENU_ITEM_EQUIP_POS.y ,   "��������" ,MenuWindow::Elements::THERD_ELEMENT},
		{MENU_ITEM_DISPOSE_POS.x ,MENU_ITEM_DISPOSE_POS.y , "�̂Ă�",   MenuWindow::Elements::FOUR_ELEMENT},
		{MENU_ITEM_CLOSE_POS.x ,  MENU_ITEM_CLOSE_POS.y ,   "����" ,  MenuWindow::Elements::FIVE_ELEMENT}
	};

	select_detail_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_detail, 5);
	select_detail_window->Open();
}

//�v���C���[�������Ă��鎞�̃��j���[�\��
void Inventory::Game_Menu()
{
	//�������ɂ���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_70_PERCENT);
	UIManager::GetUIManager()->Menu_Draw("menu_window", PLAYER_STATUS_WINDOW_POS.x, PLAYER_STATUS_WINDOW_POS.y, PLAYER_STATUS_WINDOW_WIDTH, PLAYER_STATUS_WINDOW_HEIGHT);
	//�A���t�@�l��߂�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_OPAQUE);

	//�v���C���[�̃X�e�[�^�X�̕`��
	UIManager::GetUIManager()->PlayerStatusDrawWindow();

	//�v���C���[���擾����
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//�E�B���h�E�̕`��
	UIManager::GetUIManager()->Menu_Draw("menu_window", GOLD_DISPLAY_WINDOW_POS.x, GOLD_DISPLAY_WINDOW_POS.y, GOLD_DISPLAY_WINDOW_WIDTH, GOLD_DISPLAY_WINDOW_HEIGHT);
	

	DrawStringEx(GOLD_STRING_POS.x, GOLD_STRING_POS.y, koni::Color::WHITE, "G : %d", player->GetPlayerMoney());

}

//��ԍŏ��̃��j���[
void Inventory::First_Menu()
{
	first_menu->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, FIRST_MENU_WINDOW_WIDTH, FIRST_MENU_WINDOW_HEIGHT);
}

//�w�肵���A�C�e����ID���폜����
void Inventory::InventoryItemRemove(const int& item_id)
{
	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto itemToRemove = std::find_if(inventory_list.begin(), inventory_list.end(),
		[&](const ItemBase& item) { return item.GetItemId() == item_id; });

	// �A�C�e�������������ꍇ�͍폜����
	if (itemToRemove != inventory_list.end()) {

		inventory_list.erase(itemToRemove);
		//�A�C�e���̐������炷
		--item_num;
	}
}

//�A�C�e�����폜����ۂ̕���𑕔����Ă����ۂ̏���
void Inventory::DeleteEquipItemProcess()
{
	//�A�C�e������̏ꍇ�������Ƃ΂�
	if (inventory_list.empty()) { return; }

	//����𑕔����Ă����ꍇ�̏���
	auto itr = equipped_weapon.begin();

	if (itr != equipped_weapon.end()) {
		//�I�����ꂽ�A�C�e���𑕔����Ă����ꍇ
		if (inventory_list[selected_index].GetItemId() == (*itr).GetItemId()) {

			auto& player = GameManager::GetGameManager()->GetPlayer();

			//����̑������O��
			equipped_weapon.erase(itr);

			//�t���O��؂�ւ���(E�����̕\�����s���Ȃ��悤�ɂ����)
			equip_weapon = false;

			//�v���C���[�̃X�e�[�^�X�𒲐�����
			player->GetPlayerStatusSave().SetPlayerAttack(player->GetPlayerStatusSave().GetBaseAttack());

			//�v���C���[�̌����ڂ�ύX����
			player->SetPlayerAnimationHdl(Player::PlayerEquipState::BARE_HANDS , player->GetPlayerID());
		}
	}

	//�h��𑕔����Ă����ꍇ�̏���
	auto it = equipped_armor.begin();

	if (it != equipped_armor.end()) {
		//�I�����ꂽ�A�C�e���𑕔����Ă����ꍇ
		if (inventory_list[selected_index].GetItemId() == (*it).GetItemId()) {

			auto& player = GameManager::GetGameManager()->GetPlayer();

			//����̑������O��
			equipped_armor.erase(it);

			//�t���O��؂�ւ���(E�����̕\�����s���Ȃ��悤�ɂ����)
			equip_armor = false;

			//�v���C���[�̃X�e�[�^�X�𒲐�����
			player->GetPlayerStatusSave().SetPlayerDefance(player->GetPlayerStatusSave().GetBaseDefance());
		}
	}
}

//�A�C�e�����̂Ă邪�I�����ꂽ���̏���
void Inventory::DisposeItemProcess()
{
	//�A�C�e������̏ꍇ�������Ƃ΂�
	if (inventory_list.empty()) { return; }

	//���艹��炷
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

	// �I�������A�C�e���� ID �ƈ�v����v�f����肷��
	auto& selected_item = inventory_list[selected_index];

	//�A�C�e�����폜�\�ł����
	if (selected_item.GetEssentialItemsFlag()) {

		//�����������A�C�e���𑕔����Ă������̏���
		DeleteEquipItemProcess();

		//�A�C�e���̑I�����j���[�ɖ߂�
		//select_menu = MenuWindow_I::ITEMMENU;

		// �I�����ꂽ�A�C�e�����폜����
		inventory_list.erase(inventory_list.begin() + selected_index);
		item_num--;

		//�J�[�\���ƃC���f�b�N�X���ŏ��ɖ߂�
		SelectedIndexClear();
		CurentPageReset();
		CursorReset();
	}
}

//�A�C�e���̕\��
void Inventory::ItemMenu(const tnl::Vector2i& itemDrawPos, const tnl::Vector2i& curentPageText, const int& CousourX, const int& itemParPage)
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
			std::string ItemName = InventoryData.GetItemName();

			// �A�C�e���̕`��
			DrawStringEx(itemDrawPos.x, itemDrawPos.y + Y * i, koni::Color::WHITE, "%s", ItemName.c_str());

			// �J�[�\����`�悷��ʒu������
			int cursorY = itemDrawPos.y + Y * (select_cursor % itemParPage);
			ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CousourX, cursorY, CURSORSIZE, 0, true);
		
			//�����ł���A�C�e���������ꍇ
			if (InventoryData.GetItemType() == 1 || InventoryData.GetItemType() == 2) {
				// �A�C�e������������Ă��邩�A�I�����ꂽ�A�C�e�������m�F
				if ((equip_weapon && InventoryData.GetItemId() == equipped_weapon.back().GetItemId())) {
					// �������̃A�C�e���܂��͑I�����ꂽ�A�C�e���̏ꍇ�A"[E]" ��\��
					//�A�C�e���̖��O���獶��30���ꂽ���W�ɕ\��
					DrawStringEx(itemDrawPos.x - EQUIP_X_POS, itemDrawPos.y + EQUIP_Y_POS * i, koni::Color::WHITE, "[E]");
				}
				if ((equip_armor && InventoryData.GetItemId() == equipped_armor.back().GetItemId())) {
					// �������̃A�C�e���܂��͑I�����ꂽ�A�C�e���̏ꍇ�A"[E]" ��\��
					//�A�C�e���̖��O���獶��30���ꂽ���W�ɕ\��
					DrawStringEx(itemDrawPos.x - EQUIP_X_POS, itemDrawPos.y + EQUIP_Y_POS * i, koni::Color::WHITE, "[E]");
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
		DrawStringEx(curentPageText.x, curentPageText.y, koni::Color::WHITE, "%d / 4", curent_page + 1);

}

//�v���C���[�̃X�e�[�^�X�\��
void Inventory::PlyStatusMenu()
{

	//�v���C���[���擾����
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//�E�B���h�E�̕`��
	UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, PLAYER_STATUS_GOLD_WINDOW_WIDTH, PLAYER_STATUS_GOLD_WINDOW_HEIGHT);

	//���������̍��W
	const tnl::Vector2i PLAYER_STATUS_STRING = { 150 , 70 };
	
	//�����̕�����\������
	DrawStringEx(PLAYER_STATUS_STRING.x, PLAYER_STATUS_STRING.y, koni::Color::WHITE, "===����===");

	//�ǉ����Ă������W
	const int Y = 50;

	// Player �N���X�̃|�C���^�[���L�����ǂ������m�F����
	if (player) {
		// Player �N���X�̃|�C���^�[����ăX�e�[�^�X���擾����
		int hp = static_cast<int>(player->GetPlayerStatusSave().GetcurentHp());
		int mp = static_cast<int>(player->GetPlayerStatusSave().GetCurentMp());
		int Attack = player->GetPlayerStatusSave().GetAttack();
		int Defense = player->GetPlayerStatusSave().GetDefance();
		int Level = player->GetPlayerStatusSave().GetLevel();
		int Speed = player->GetPlayerStatusSave().GetSpeed();
		int R_expoint = player->GetPlayerStatusSave().GetExpoint();

		// �擾�����X�e�[�^�X��`�悷��
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y, koni::Color::WHITE, "HP : %d", hp);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + Y, koni::Color::WHITE, "MP : %d", mp);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_DOUBLE_I),	koni::Color::WHITE, "�U���� : %d", Attack);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_TRIPLE_I),	koni::Color::WHITE, "�h��� : %d", Defense);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_QUADRUPLE_I), koni::Color::WHITE, "���x�� : %d", Level);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_QUINTUPLE_I), koni::Color::WHITE, "���΂₳ : %d", Speed);
		DrawStringEx(PLAYER_STATUS_X, PLAYER_STATUS_Y + (Y * koni::Numeric::SCALE_SIXFOLD_I),   koni::Color::WHITE, "���x���A�b�v�ɕK�v�Ȍo���l : %d", R_expoint);
	}

}

//�A�C�e�����g�p���郁�j���[
void Inventory::ItemUseMenu()
{
	select_detail_window->All(ITEM_USE_POS.x, ITEM_USE_POS.y, ITEM_USE_WINDOW_WIDTH, ITEM_USE_WINDOW_HEIGHT);

}

//�C���x���g���̏���
void Inventory::swichInventoryUpdate()
{
	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	switch (select_menu)
	{
	case Inventory::MenuWindow_I::EMPTY:

		break;

		//�ŏ��̃��j���[�ł̏���
	case Inventory::MenuWindow_I::FIRSTMENU:
	
		//1�ڂ��I�����ꂽ�ꍇ�A�A�C�e����\������
		if (first_menu->getSelectNum() == TOOL && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ITEMMENU;
			CursorReset();
			curent_page = 0;
			selected_index = 0;
		}
			//2�ڂ��I�����ꂽ�ꍇ�A���Z��\������
		else if (first_menu->getSelectNum() == SKILL && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::SKILLMENU;

		}
		//3�ڂ��I�����ꂽ�ꍇ�A�v���C���[�̋�����\������
		else if (first_menu->getSelectNum() == STATUS && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::STATUSMENU;
		}
		//4�ڂ��I�����ꂽ�ꍇ�A����
		else if (first_menu->getSelectNum() == CLOSE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::EMPTY;
			//plyer�𓮂���悤�ɂ���
			if (!GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
				GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
			}
		
			}

		//�o�b�N�X�y�[�X�������ƃA�C�e���g�p���j���[�ɖ߂�
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::EMPTY;
			//plyer�𓮂���悤�ɂ���
			if (!GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
				GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
			}
		}

		break;

		//�A�C�e�����j���[�ł̏���
	case Inventory::MenuWindow_I::ITEMMENU:

		//Enter�L�[����������ItemUseMenu�ɕύX����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_menu = MenuWindow_I::ITEMUSEMENU;
		}
		//�o�b�N�X�y�[�X�������ƃA�C�e���g�p���j���[�ɖ߂�
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		//�C���f�b�N�X����
		ItemCurourIndex(ITEMPERPAGE);
		
		//�J�[�\���̓���
		CusorMove();

		break;

	//�A�C�e���ڍ׃��j���[�̏ꍇ
	case Inventory::MenuWindow_I::ITEMDETAILMENU:

		//�o�b�N�X�y�[�X�������ƃA�C�e���g�p���j���[�ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::ITEMUSEMENU;
		}

		break;

		//�A�C�e���g�p���j���[
	case Inventory::MenuWindow_I::ITEMUSEMENU:

		//�g�����I�����ꂽ��
		if (select_detail_window->getSelectNum() == USE) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				//item���g�p����
				item->ItemUse(selected_item_id);
				//�J�[�\���ƃC���f�b�N�X���ŏ��ɖ߂�
				SelectedIndexClear();
				CurentPageReset();
				CursorReset();
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
				SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ITEMDETAILMENU;
			}
		}
		//�������I�����ꂽ��
		else if (select_detail_window->getSelectNum() == EQUIP) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				EquipWeapon(selected_index);
			}
		}
		//�̂Ă邪�I�����ꂽ��
		else if (select_detail_window->getSelectNum() == DISPOSE) {
			
			//�G���^�[�L�[�������ꂽ��
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//�̂Ă鏈��
				DisposeItemProcess();
			}
		}
		//���邪�I�����ꂽ��
		else if (select_detail_window->getSelectNum() == ITEMCLOSE) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//���艹��炷
				SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::ITEMMENU;
			}
		}

		//�o�b�N�X�y�[�X�������ƃA�C�e���g�p���j���[�ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::ITEMMENU;
		}
		
		break;

	//�v���C���[�̃X�e�[�^�X�\��
	case MenuWindow_I::STATUSMENU:

		//�o�b�N�X�y�[�X�������ƃA�C�e���g�p���j���[�ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
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
				SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
				select_menu = MenuWindow_I::SKILLDATAILMENU;
			}

		}

		//�X�L���̃J�[�\���ړ����̃C���f�b�N�X����
		SkillCurourIndex(ITEMPERPAGE);


		//���Z�̃J�[�\���ړ�
		SkillCousorMove();

		//�o�b�N�X�y�[�X�������ƍŏ��̃��j���[�ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_menu = MenuWindow_I::FIRSTMENU;
		}

		break;

	//�X�L������
	case MenuWindow_I::SKILLDATAILMENU:

		//�X�L���I���ɖ߂�
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
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

	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	//���݂̃y�[�W�Ɋ�Â��ĕ\������A�C�e���͈̔͂��v�Z
	start_index = skill_curent_page * ITEMPARPAGE;
	end_index = min((start_index + ITEMPARPAGE), static_cast<int>(SkillList.size()));

	for (int index = start_index; index < end_index; ++index) {
		//
		auto& SkillData = SkillList[index];

		//�A�C�e���̖��O���擾����
		std::string skillName = SkillData->getName();

		// �A�C�e���̕`��
		DrawStringEx(SKILLDRAWPOS.x, SKILLDRAWPOS.y + y * i, koni::Color::WHITE, "%s", skillName.c_str());

		// �J�[�\����`�悷��ʒu������
		int cursorY = SKILLDRAWPOS.y + y * (skill_cousour % ITEMPARPAGE);
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", COUSOURX, cursorY, CURSORSIZE, 0, true);

		++i;

	}

	//�y�[�W�̕\��
	DrawStringEx(CURENTPAGETEXT.x, CURENTPAGETEXT.y, koni::Color::WHITE, "%d / 4", skill_curent_page + 1);


}

//�X�L�������̕`��
void Inventory::SkillDetailDraw(const int& skill_index)
{
	//�X�L���z��
	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	//���Z����
	UIManager::GetUIManager()->Menu_Draw("menu_window", SKILL_DETAIL_WINDOW_POS.x, SKILL_DETAIL_WINDOW_POS.y, SKILL_DETAIL_WINDOW_WIDTH, SKILL_DETAIL_WINDOW_HEIGHT);

	//�X�L�������̕����p���W
	const tnl::Vector2i SUKILL_STRING_POS = { 400, 70 };

	DrawStringEx(SUKILL_STRING_POS.x, SUKILL_STRING_POS.y, koni::Color::WHITE, "==�X�L������==");

	//�X�L���f�[�^���擾
	auto& skillData = SkillList[skill_index];

	//�X�L���������Ă����
	if (!SkillList.empty()) {

		//�X�L���f�[�^�̃X�L���������擾
		const std::string& skillDatail = skillData->getDescription();
		const int skill_mp = skillData->getSkillConsumeMp();

		//�X�L��������`��
		DrawStringEx(SKILL_DETAIL_POS.x, SKILL_DETAIL_POS.y, koni::Color::WHITE, "%s", skillDatail.c_str());

		//�X�L���̕K�vMp�̕`��
		DrawStringEx(SKILL_DETAIL_POS.x, SKILL_CONSME_POS_Y, koni::Color::WHITE, "�K�v��MP : %d", skill_mp);
	}

}

//�X�L���̃J�[�\������
void Inventory::SkillCousorMove()
{

	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	//�X�L�����Ȃ��ꍇ�A�������Ȃ�
	if (SkillList.empty())
	{
		return;
	}
	//�J�[�\������ԏ�ɂ���ꍇ�J�[�\��������ɏ�ɉ����Ă��J�[�\���𓮂����Ȃ��悤�ɂ���
	if (skill_cousour == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_W))
	{
		return;
	}
	//�J�[�\���̓���
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		skill_cousour = (skill_cousour + (SkillList.size() - 1)) % static_cast<int>(SkillList.size());

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
		// �J�[�\�����Ō�̗v�f�������Ɉړ����悤�Ƃ���ꍇ�́A���������ɏ������I������
		if (skill_cousour == SkillList.size() - 1) {
			return;
		}

		skill_cousour = (skill_cousour + 1) % SkillList.size();
	}

}

//�X�L���J�[�\���̈ړ����̃C���f�b�N�X����
void Inventory::SkillCurourIndex(const int& skil_list_perpage)
{

	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList();

	// ��L�[�������ꂽ�Ƃ��̏���
	if (skill_curent_page == 0 && skill_selected_index == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		// �������Ȃ�
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		if (skill_selected_index % skil_list_perpage == 0) {
			// �J�[�\�����y�[�W���̍ŏ㕔�ɂ���ꍇ�́A���̃y�[�W�̍Ō�̗v�f��I��
			if (skill_curent_page > 0) {
				--skill_curent_page;
				// �C���f�b�N�X���X�V���ăy�[�W�̍Ō�̗v�f��I������
				skill_selected_index = (skill_curent_page + 1) * skil_list_perpage - 1;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A���̗v�f��I��
			--skill_selected_index;
		}
	}

	// ���L�[�������ꂽ�Ƃ��̏���
	if (skill_selected_index == SkillList.size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		// �������Ȃ�
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		if ((skill_selected_index + 1) % skil_list_perpage == 0) {
			// �J�[�\�����y�[�W���̍ŉ����ɂ���ꍇ�́A����̃y�[�W�̍ŏ��̗v�f��I��
			// �X�L�������Ă�C���x���g���� 20 �ōl���Ă���� 4 �y�[�W���܂ŊJ����悤�ɂ���
			if (skill_curent_page < ITEM_MAX_PAGE) {
				++skill_curent_page;
				// �C���f�b�N�X���X�V���ăy�[�W�̍ŏ��̗v�f��I������
				skill_selected_index = skill_curent_page * skil_list_perpage;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A����̗v�f��I��
			++skill_selected_index;
		}
	}
}


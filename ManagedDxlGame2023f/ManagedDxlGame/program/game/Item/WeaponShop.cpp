#include "WeaponShop.h"
#include"../Manager/UiManager.h"
#include"../Manager/GameManager.h"


WeaponShop::WeaponShop(const InMapScene::InMapState& curent_inmap_scene)
{
	item_ = std::make_shared<Item>();

	//���퉮�̃��C���i�b�v�����߂�
	WeaponShopInit(curent_inmap_scene);

	//���p�m�F�R�}���h
	if (!sell_coment) {

		//---���W---//

		//���p
		const tnl::Vector2i SELL_STRING_POS = { 350 , 100 };

		//��߂�
		const tnl::Vector2i CLOASE_STRING_POS = { 350 , 150 };

		sell_coment = new MenuWindow::MenuElement_t[]{
			{SELL_STRING_POS.x ,   SELL_STRING_POS.y ,   "���p����" ,	MenuWindow::Elements::FIRST_ELEMENT},
			{CLOASE_STRING_POS.x , CLOASE_STRING_POS.y , "��߂�"   ,   MenuWindow::Elements::SECOND_ELEMENT}
		};
	}

	if (!sell_coment_select) {

		//���p�m�F�R�}���h
		const int SELL_COMENT_NUM = 2;

		sell_coment_select = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", sell_coment, SELL_COMENT_NUM);
		sell_coment_select->Open();
	}
}

//�X�V����
void WeaponShop::Update(const float delta_time)
{
	//�V�[�P���X�̍X�V����
	tnl_sequence_.update(delta_time);
}

void WeaponShop::WeaponShopInit(const InMapScene::InMapState& curent_inmap_scene)
{
	weponList.clear();
	
	switch (curent_inmap_scene)
	{
	case InMapScene::InMapState::VILLAGE:

		//��ڂ̓X

		//�؂̖_
		weponList.emplace_back(item_->GetItemById(6));
		//�؂̌�
		weponList.emplace_back(item_->GetItemById(4));
		//�؂̃n���}�[
		weponList.emplace_back(item_->GetItemById(5));
		//�v�̏�
		weponList.emplace_back(item_->GetItemById(2));
		//�|�[�V����
		weponList.emplace_back(item_->GetItemById(3));

		break;

	case InMapScene::InMapState::TOWN:

		//�`���҂̓�
		weponList.emplace_back(item_->GetItemById(10));
		//���̌�
		weponList.emplace_back(item_->GetItemById(0));
		//�v�̊Z
		weponList.emplace_back(item_->GetItemById(1));
		//�`���҂̊Z
		weponList.emplace_back(item_->GetItemById(13));
		//�n�C�|�[�V����
		weponList.emplace_back(item_->GetItemById(8));

		break;

	//�鉺��
	case InMapScene::InMapState::CASTLETOWN:

		//�b���̊Z
		weponList.emplace_back(item_->GetItemById(7));
		//�v�����g�G�L�X(�񕜃A�C�e��)
		weponList.emplace_back(item_->GetItemById(17));
		//�|�̌�
		weponList.emplace_back(item_->GetItemById(12));
		//�є�̃R�[�g
		weponList.emplace_back(item_->GetItemById(28));
		//�S�̃n���}�[
		weponList.emplace_back(item_->GetItemById(34));


		break;

	default:
		break;
	}


	//�l�i���Ⴂ���Ƀ\�[�g
	std::sort(weponList.begin(), weponList.end(), [](const ItemBase& left, const ItemBase& right) {
		return left.GetItemPrice() < right.GetItemPrice();
		});
}

//�ҋ@���(��ɍw���Ȃ̂��A���p�Ȃ̂��ŏ�����؂�ւ���)
bool WeaponShop::seqIdle(float delta_time)
{
	//�w�����I�����ꂽ��w���V�[�P���X�Ɉڍs������
	if (curent_transaction_type == TransactionType::BUY) {
		tnl_sequence_.change(&WeaponShop::seqBuy);
	}
	//���邪�I�����ꂽ�甄�p�V�[�P���X�Ɉڍs������
	else if (curent_transaction_type == TransactionType::SELL) {
		tnl_sequence_.change(&WeaponShop::seqSell);
	}
	else {

		tnl::DebugTrace("\n------------------------------------------------------------");
		tnl::DebugTrace("\n �V�[�P���X��؂�ւ��܂���ł����B");
		tnl::DebugTrace("\n------------------------------------------------------------");

	}


	return false;
}

//�w���V�[�P���X
bool WeaponShop::seqBuy(float delta_time)
{
	//���퉮�̃J�[�\������
	WeaponShopCursorMove();
	//�w������
	BuyWeapon();

	return false;
}

//���p�V�[�P���X
bool WeaponShop::seqSell(float delta_time)
{
	//���p����
	SellItemProcess();

	return false;
}


void WeaponShop::Draw()
{
	//�w�����I�����ꂽ�ꍇ
	if (curent_transaction_type == TransactionType::BUY) {

		//���j���[�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);

		DrawString(WEAPON_SHOP_STRING_POS.x, WEAPON_SHOP_STRING_POS.y, "===== ���퉮 =====", koni::Color::WHITE);

		for (int i = 0; i < weponList.size(); ++i) {
			DrawStringEx(WEAPON_CONNECTION_POS.x, WEAPON_CONNECTION_POS.y + i * SPACING, koni::Color::WHITE, "%d. %s \n(�З�: %d, �h���: %d,���i: %d)", i + 1, weponList[i].GetItemName().c_str(), weponList[i].GetItemDamage(), weponList[i].GetItemDefance(), weponList[i].GetItemPrice());
		}
		DrawString(PROMPT_POS.x, PROMPT_POS.y, "�w�������������\n�I�����Ă�������\n(back space : ����)", koni::Color::WHITE);

		//�Ȃɂ������Ƃ��̃R�����g
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, koni::Color::WHITE, "�u ���𔃂��񂾂� �v");

		//���폤�l�̃R�����g
		if (curent_weapon_shop_coment == WeaponshopComent::BUY) {
			DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "�u���x����!���肪�ƂˁI�v", koni::Color::WHITE);
		}
		else if (curent_weapon_shop_coment == WeaponshopComent::NO) {
			DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "�u����������Ȃ���!�o�����Ă���!�v", koni::Color::WHITE);
		}

		//�J�[�\���̕`��
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", CURSOR_POS.x, CURSOR_POS.y + select_cousor * SPACING, CURSOR_SIZE, koni::Color::BLACK, true);
	}
	//���p���I�����ꂽ�ꍇ
	else if (curent_transaction_type == TransactionType::SELL) {

		//�C���x���g���̃A�C�e����\������E�B���h�E�̍��W
		const tnl::Vector2i ITEM_WINDOW_POS = { 50 ,50 };

		//�C���x���g���̃A�C�e����\������E�B���h�E�̃T�C�Y
		const tnl::Vector2i ITEM_WINDOW_SIZE = { 280, 400 };

		//�Ȃɂ������Ƃ��̃R�����g
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, koni::Color::WHITE, "�u ���𔄂�񂾂� �v");

		//�A�C�e���I����ʂ̏ꍇ
		if (curent_sell_action == SellAction::SELLITEMSELECT) {

			//�E�B���h�E��\������
			UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_WINDOW_POS.x, ITEM_WINDOW_POS.y, ITEM_WINDOW_SIZE.x, ITEM_WINDOW_SIZE.y);

			//�A�C�e����\������
			GameManager::GetGameManager()->GetInventory()->ItemMenu(ITEM_DRAW_POS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE_);

		}
		//�A�C�e���̔��p�m�F�E�B���h�E�̏ꍇ
		else if (curent_sell_action == SellAction::SELLITEMCHECK) {

			//���p�I���R�}���h�̃E�B���h�E�̍��W
			const tnl::Vector2i SELL_COMENT_WINDOW_POS = { 300 , 50 };

			//���p�I���R�}���h�̃E�B���h�E�̃T�C�Y
			const tnl::Vector2i SELL_COMENT_WINDOW_SIZE = { 200 , 250 };

			//���p���i��\��������W
			const tnl::Vector2i SELL_PRICE_STRING_POS = { 330, 220 };

			//�C���x���g��
			auto& Inventory = GameManager::GetGameManager()->GetInventory();

			//�E�B���h�E��\������
			UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_WINDOW_POS.x, ITEM_WINDOW_POS.y, ITEM_WINDOW_SIZE.x, ITEM_WINDOW_SIZE.y);

			//�A�C�e����\������
			Inventory->ItemMenu(ITEM_DRAW_POS, CURENTPAGETEXT, CURSORX, ITEMPERPAGE_);

			//���p�I���R�}���h�̕\�����s��
			sell_coment_select->All(SELL_COMENT_WINDOW_POS.x, SELL_COMENT_WINDOW_POS.y, SELL_COMENT_WINDOW_SIZE.x, SELL_COMENT_WINDOW_SIZE.y);

			//�C���x���g�����󂶂ᖳ�����
			if (!GameManager::GetGameManager()->GetInventory()->GetInventoryList().empty()) {

				//���p���i�𔼕��̉��i�ɐݒ肷��
				auto sell_price = Inventory->GetInventoryList()[Inventory->GetSelectedIndex()].GetItemPrice() / koni::Numeric::DIVIDER_TWO;

				//���p���̒l�i�\��
				DrawStringEx(SELL_PRICE_STRING_POS.x, SELL_PRICE_STRING_POS.y, koni::Color::WHITE, "���p���i : %dG", sell_price);
			}
		}

		//���폤�l�̃R�����g
		if (curent_weapon_shop_coment == WeaponshopComent::SELL) {
			DrawString(MESSAGE_POS.x, MESSAGE_POS.y, "�u���x����!���肪�ƂˁI�v", koni::Color::WHITE);
		}

		DrawString(PROMPT_POS.x, PROMPT_POS.y, "���p����A�C�e����\n�I�����Ă�������\n(back space : ����)", koni::Color::WHITE);
	}


}

//�J�[�\���̓���
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
	//�C���x���g�����擾����
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//�v���C���[���擾����
	auto& player = GameManager::GetGameManager()->GetPlayer();

	if (select_cousor == MenuWindow::Elements::FIRST_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (inventory->GetInventoryList().size() == INVENTORY_MAX_SIZE) return;
		//���艹��炷
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
		//���艹��炷
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
		//���艹��炷
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
		//���艹��炷
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
		//���艹��炷
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

//�����A�C�e���𔄋p����
void WeaponShop::SellItemProcess()
{
	//�v���C���[���擾����
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//�C���x���g�����擾����
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//���p�̑I��
	if (curent_sell_action == SellAction::SELLITEMCHECK) {

		//���p���I�����ꂽ��
		if (sell_coment_select->GetSelectNum() == MenuWindow::FIRST_ELEMENT
			&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�C���x���g������łȂ����A�A�C�e�������p�\�ł����
			if (!inventory->GetInventoryList().empty() && inventory->GetInventoryList()[inventory->GetSelectedIndex()].GetEssentialItemsFlag()) {

				// �I�����ꂽ�A�C�e�����Q��
				auto& selectedItem = inventory->GetInventoryList()[inventory->GetSelectedIndex()];

				// ���p���i�𔼕��ɐݒ肵�A�������ɒǉ�
				player->AddPlayerMoney(selectedItem.GetItemPrice() / koni::Numeric::DIVIDER_TWO);

				//�����������Ă��镐��𔄋p�������̏���
				inventory->DeleteEquipItemProcess();

				// �C���x���g������A�C�e�����폜
				inventory->GetInventoryList().erase(inventory->GetInventoryList().begin() + inventory->GetSelectedIndex());

				//�J�[�\���ƃC���f�b�N�X�����Z�b�g����
				inventory->CursorReset();
				inventory->CurentPageReset();
				inventory->SelectedIndexClear();

				//���퉮�̃R�����g��؂�ւ���
				curent_weapon_shop_coment = WeaponshopComent::SELL;

				//��Ԃ��A�C�e���̑I����ʂɖ߂�
				curent_sell_action = SellAction::SELLITEMSELECT;
			}
		}
		else if (sell_coment_select->GetSelectNum() == MenuWindow::SECOND_ELEMENT
			&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�A�C�e���̑I����ʂɖ߂�
			curent_sell_action = SellAction::SELLITEMSELECT;
		}
	}
	//�A�C�e����I������ꍇ
	else if (curent_sell_action == SellAction::SELLITEMSELECT) {

		//�C���f�b�N�X�̑���Ǝ擾
		GameManager::GetGameManager()->GetInventory()->ItemCurourIndex(ITEMPERPAGE_);

		//�A�C�e���̃J�[�\������
		GameManager::GetGameManager()->GetInventory()->CusorMove();

		//�A�C�e����I����Ԃ�Enter�L�[����������m�F�E�B���h�E�Ɉړ�������
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			curent_sell_action = SellAction::SELLITEMCHECK;
		}
	}
	
}

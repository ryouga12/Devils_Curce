#include "battleScene.h"
#include"../Manager/SceneManager.h"
#include"mapScene.h"
#include"resultScene.h"
#include "../Object/Player.h"
#include"../Object/Enemy.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

BattleScene::BattleScene(tnl::Vector3 pos,std::vector<Enemy::EnemyStatus>enemy_array, int background ) : enemy_Array(enemy_array), background_(background), map_pos(pos)
{
	enemy = std::make_shared<Enemy>();
	battle_log = std::make_shared<BattleLog>();

	//�C���f�b�N�X��1�`4�̃����_���Ȑ����ɐݒ肷��
	enemy_index = rand() % 5;

	//�V�����z��𐶐�����
	auto& enemy_ = enemy_Array[enemy_index];

	//�G�̃O���t�B�b�N�n���h����ǂݍ���
	Enemy_ghdl = ResourceManager::getResourceManager()->LoadGraphEX(enemy_.getEnemyGhdl().c_str());

	enemy->InitEnemyItemDrop(enemy_index);

	//���j���[�̏�����
	InitMenuWindow();

	//����̃^�C�v���擾����
	SetWeaponType();

	//�o�g���t���O��؂�ւ���(�o�g���݂̂ł���A�C�e���̈�)
	GameManager::getGameManager()->getInventory()->BattleFlagSwitch(0);

	//�o�g�����O���Z�b�g����
	GameManager::getGameManager()->getInventory()->SetBattleLog(battle_log);

	//�ʏ�U��
	nomal_attack = std::make_shared<Nomal_Attack>(WeaponType);
}

//�f�X�g���N�^
BattleScene::~BattleScene()
{
	auto& enemy = enemy_Array[enemy_index];

	//�T�E���h����������
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/levelup.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/BGM/sentou.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/dageki_2.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/nigeru.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/syouri.mp3");

	//�摜����������
	ResourceManager::getResourceManager()->deleteGraphEx(enemy.getEnemyGhdl().c_str());
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/haikei/battle_field_01.jpg");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/haikei/battle_field_02.jpg");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/haikei/battle_field_03.jpg");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/cur_sannkaku2.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/mpbar.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/hpbar.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Player/Player_Icon_sentou.png");

}

//���j���[�̏�����
void BattleScene::InitMenuWindow()
{
	//�ŏ��̍s�������߂�window
	select_action_coment = new MenuWindow::MenuElement_t[]
	{
		{100 , 550 , "��������" , 0},
		{100 , 600 , "����" , 1},
		{100 , 650 , "������" , 2}

	};

	Select_Action_Menu = std::make_shared<MenuWindow>(50, 510, 250, 200, "graphics/WindowBase_01.png", select_action_coment, 3, 1);
	Select_Action_Menu->Open();

	//�U�������߂�E�B���h�E
	select_action_attack = new MenuWindow::MenuElement_t[]
	{
		{100 , 550 ,"�U��" , 0},
		{100 , 600, "���Z" , 1},
		{100 , 650 ,"�߂�" , 2}
	};

	attack_window = std::make_shared<MenuWindow>(50, 510, 250, 200, "graphics/WindowBase_01.png", select_action_attack, 3, 1);
	attack_window->Open();

	//�A�C�e�����g�p�̃E�B���h�E
	select_detail = new MenuWindow::MenuElement_t[]{
		{ 600 , 550 , "�g��", 0},
		{ 600 , 600 , "����", 1}
	};

	select_use_window = std::make_shared<MenuWindow>(550, 510, 250, 200, "graphics/WindowBase_01.png", select_detail, 2, 1);
	select_use_window->Open();

	//�o�g�����O�̃E�B���h�E
	BattleLogMenu = std::make_shared<Menu>(670, 30, 600, 180, "graphics/WindowBase_01.png");
	//�v���C���[�̃E�B���h�E
	playerStatus_window = std::make_shared<Menu>(50, 50, 250, 240, "graphics/WindowBase_01.png");
	//�A�C�e���̃E�B���h�E
	item_window = std::make_shared<Menu>(300, 510, 250, 200, "graphics/WindowBase_01.png");
	//�X�L���E�B���h�E
	skill_menu_window = std::make_shared<Menu>(300, 510, 250, 200, "graphics/WindowBase_01.png");

}

//����̃^�C�v���Z�b�g����
void BattleScene::SetWeaponType()
{
	//���킪��������Ă�����
	if (!GameManager::getGameManager()->getInventory()->getEquipArray().empty()) {

		auto it = GameManager::getGameManager()->getInventory()->getEquipArray().begin();

		//����̃^�C�v���擾����
		WeaponType = (*it).getItemWeapontype();

	}
	//��������Ă��Ȃ�������0�ɂ���
	else {
		WeaponType = 0;
	}

}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void BattleScene::Update(float delta_time)
{
	//�V�[�P���X�̍X�V����
	sequence_.update(delta_time);

	//�A�j���[�V�����̍X�V����
	nomal_attack->SkillAnimationUpdate(delta_time);
	//�A�j���[�V�����z�񂪖������͏������΂�
	if (GameManager::getGameManager()->getInventory()->getSkillNum() == 0) {
		return;
	}
	//�A�j���[�V�����̍X�V����
	GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillAnimationUpdate(delta_time);
	
}

//�v���C���[�̃^�[�����̍X�V����
void BattleScene::PlayerUpdate()
{
	//�v���C���[�̃X�e�[�^�X���擾����
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	//�G�̃X�e�[�^�X���擾����
	auto& enemyStatus = enemy_Array[enemy_index];

	//�v���C���[�̃A�N�V�����̎�
	if (select_sequence == Sequence::PlayerAction) {
		//���j���[�̍X�V����
		MenuUpdate(PlayerStatus, enemyStatus);
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�`��
void BattleScene::Draw()
{
	//�w�i�摜
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, background_, TRUE);

	//�o�g�����O�̃E�B���h�E�̕`��
	BattleLogMenu->Menu_draw();


	//�o�g���E�B���h�E�̕`��
	BattleDraw();
	//�o�g�����O�̕����̕`��
	battle_log->drawLogs();

	//�v���C���[�̃X�e�[�^�X�̕`��
	PlayerStatusDraw();

	/*DrawStringEx(10, 10, -1, "�o�g�����");*/

	//�G�̉摜�̕`��
	if (!enemy_annihilation_flag) {
		DrawRotaGraph(600, 320, 0.8f, 0, Enemy_ghdl, true);
	}
	
	//�A�j���[�V�����̕`��
	nomal_attack->SkillAnimationDraw();

	DrawStringEx(360, 200, -1, "�U����\n%d", GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().getAttack());

	//�A�j���[�V�����z�񂪖������͏������΂�
	if (GameManager::getGameManager()->getInventory()->getSkillNum() == 0) {
		return;
	}

	//���Z�̃A�j���[�V�����̕`��
	GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillAnimationDraw();

}

//�o�g���E�B���h�E�̕`��
void BattleScene::BattleDraw()
{
	//�ŏ��̍s���̎��̃E�B���h�E
	if (select_action_menu == MenuAction::first_action) {
		Select_Action_Menu->All();
	}
	//�U���E�B���h�E
	else if (select_action_menu == MenuAction::fight_action) {
		attack_window->All();
	}
	//�A�C�e���\���̃E�B���h�E
	else if (select_action_menu == MenuAction::item_action) {
		Select_Action_Menu->All();
		Select_Action_Menu->SetSelectCousourMove(False);
		item_window->Menu_draw();
		InventoryItemDraw();
	}
	//�A�C�e�����g�����̃E�B���h�E
	else if (select_action_menu == MenuAction::item_use_action) {
		Select_Action_Menu->All();
		Select_Action_Menu->SetSelectCousourMove(False);
		item_window->Menu_draw();
		InventoryItemDraw();
		select_use_window->All();
	}
	else if (select_action_menu == MenuAction::skill_action) {
		attack_window->All();
		attack_window->SetSelectCousourMove(False);
		skill_menu_window->Menu_draw();
		InventorySkillDraw();
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�A�C�e���֘A


//�A�C�e���̕`��
void BattleScene::InventoryItemDraw()
{
	//�C���x���g���̃A�C�e����2�s���\������
	for (int i = 0; i < itemPerPage; ++i) {
		int itemIndex = currentPage * itemPerPage + i;
		if (itemIndex >= GameManager::getGameManager()->getInventory()->GetInventoryList().size()) {

			break;//�C���x���g���͈̔͊O��������I������

		}

		auto& InventoryData = GameManager::getGameManager()->getInventory()->GetInventoryList()[itemIndex];
		std::string itemName = InventoryData.getItemName();

		//�A�C�e���̕`��
		DrawStringEx(350, StartY + i * lineHeight, Color_White, "%s", itemName.c_str());

		// �I�����ꂽ�A�C�e���̃C���f�b�N�X�Ɋ�Â��ăJ�[�\����`��
		if (itemIndex == selectedIndex) {
			// �J�[�\����`�悷��ʒu���v�Z
			int cursorY = StartY + i * lineHeight;
			ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", 330, cursorY, cursourSize, Color_Black, true);
		}
	}

}

//�A�C�e���̑I������
void BattleScene::ItemSelectProcess()
{
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		select_action_menu = MenuAction::item_use_action;
	}
	// ��L�[�������ꂽ�Ƃ��̏���
	//��ԏ�ɃJ�[�\���������ꍇ����ȏ�ɂ����Ȃ��悤�ɂ���
	if (currentPage == 0 && selectedIndex == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// �������Ȃ�
		return;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selectedIndex % itemPerPage == 0) {
			// �J�[�\�����y�[�W���̍ŏ㕔�ɂ���ꍇ�́A���̃y�[�W�̍Ō�̗v�f��I��
			if (currentPage > 0) {
				--currentPage;
				// �C���f�b�N�X���X�V���ăy�[�W�̍Ō�̗v�f��I������
				selectedIndex = (currentPage + 1) * itemPerPage - 1;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A���̗v�f��I��
			--selectedIndex;
		}
	}

	// ���L�[�������ꂽ�Ƃ��̏���
	if (selectedIndex == GameManager::getGameManager()->getInventory()->GetInventoryList().size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN))return;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selectedIndex + 1) % itemPerPage == 0) {
			// �J�[�\�����y�[�W���̍ŉ����ɂ���ꍇ�́A����̃y�[�W�̍ŏ��̗v�f��I��
			//���̏��v���C���[�����Ă�C���x���g����20�ōl���Ă����4�y�[�W���܂ŊJ����悤�ɂ���
			if (currentPage < 4) {
				++currentPage;
				// �C���f�b�N�X���X�V���ăy�[�W�̍ŏ��̗v�f��I������
				selectedIndex = currentPage * itemPerPage;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A����̗v�f��I��
			++selectedIndex;
		}
	}

	// selectedIndex���L���Ȕ͈͓��ɂ��邩�m�F���āA�I�����ꂽ�A�C�e����ID���擾����
	if (selectedIndex >= 0 && selectedIndex < GameManager::getGameManager()->getInventory()->InventoryList.size()) {
		ItemBase selectedItem = GameManager::getGameManager()->getInventory()->GetInventoryList()[selectedIndex];
		selectedItemId = selectedItem.getItemId();
	}

	//�A�C�e���̃J�[�\������
	GameManager::getGameManager()->getInventory()->CusorMove();

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		//���ʉ���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�s�����ڂ�
		select_action_menu = MenuAction::first_action;
		//���j���[��؂�ւ���
		Select_Action_Menu->SetSelectCousourMove(True);
	}

}

//�A�C�e�����g�����ۂ̏���
void BattleScene::ItemUseProcess()
{
	if (select_use_window->getSelectNum() == ItemUse && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//���ʉ���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�A�C�e�����g�������̏���
		GameManager::getGameManager()->getInventory()->InventoryItemUse(selectedItemId);
		//�s�����ڂ�
		select_action_menu = MenuAction::first_action;
		//�J�[�\���𓮂���悤�ɂ���
		Select_Action_Menu->SetSelectCousourMove(True);
		//�V�[�P���X��؂�ւ���
		sequence_.change(&BattleScene::seqChangeScene);
		PlayerTurn = false;
	}
	//���邪�����ꂽ��
	else if (select_use_window->getSelectNum() == ItemUseMenuClose && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		select_action_menu = MenuAction::item_action;
	}
}

//�G��|�������̃A�C�e���h���b�v����
void BattleScene::ItemDropProcess()
{
	//�����_���ȃV�[�h�l�𐶐�����ׂ̕ϐ�
	std::random_device rd;

	//�^�������𐶐�����(�����Z���k�c�C�X�^�[)
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 100);

	//0����1�̈�l�����𐶐�����
	int rand_val = dis(gen);

	//�G�̃A�C�e���z����擾����
	auto& EnemyItemDrop = enemy->getItemDropArray();

	//�ʏ�h���b�v
	if (rand_val < NomalDrop) {
		//�G�̃A�C�e�����C���x���g���Ɋi�[����
		GameManager::getGameManager()->getInventory()->AddInventory(EnemyItemDrop[0].getItemId());

		//���O���Ȃ���
		//�������������ς�����Ȃ�������
		if (GameManager::getGameManager()->getInventory()->GetInventoryList().size() != 20) {
			battle_log->addItemDrop("�v���C���[", EnemyItemDrop[0].getItemName());
		}
		//�������������ς���������
		else {
			battle_log->addLog("�������������ς��̈׃A�C�e������肷�鎖���ł��܂���ł���");
		}
	}

	//���A�h���b�v
	else if (rand_val < RareDrop) {
		//�G�̃A�C�e�����C���x���g���Ɋi�[����
		GameManager::getGameManager()->getInventory()->AddInventory(EnemyItemDrop[1].getItemId());

		//���O���Ȃ���
		//�������������ς�����Ȃ�������
		if (GameManager::getGameManager()->getInventory()->GetInventoryList().size() != 20) {
			battle_log->addItemDrop("�v���C���[", EnemyItemDrop[1].getItemName());
		}
		//�������������ς���������
		else {
			battle_log->addLog("�������������ς��̈׃A�C�e������肷�鎖���ł��܂���ł���");
		}
	}
}

//�ꎞ�I�ɏオ�����U���͂Ȃǂ̃o�t�����Z�b�g����
void BattleScene::BattleBuffResetProcess()
{
	//�U���͂Ȃǂ��オ���Ă����ꍇ��������Z�b�g����
	if (GameManager::getGameManager()->getInventory()->getAttackAmount() != 0) {

		int AttackAmoument = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().getAttack() - GameManager::getGameManager()->getInventory()->getAttackAmount();
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerAttack(AttackAmoument);
	}
	//�オ���ċ��Ȃ���Ώ������΂�
	else {
		return;
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�X�L���֘A

//�X�L�����I�����ꂽ���̏���
void BattleScene::SkillSelectProcess()
{
	//Enter�L�[�������ꂽ����
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//�X�L���������Ȃ��ꍇ�ɂ͏������s��Ȃ�
		if (GameManager::getGameManager()->getInventory()->getSkillNum() <= 0) {
			return;
		}
		//�U������
		SkillUseFlag = true;
	}

	// ��L�[�������ꂽ�Ƃ��̏���
	if (SkillCurentPage == 0 && selectedSkillIndex == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// �������Ȃ�
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selectedSkillIndex % itemPerPage == 0) {
			// �J�[�\�����y�[�W���̍ŏ㕔�ɂ���ꍇ�́A���̃y�[�W�̍Ō�̗v�f��I��
			if (SkillCurentPage > 0) {
				--SkillCurentPage;
				// �C���f�b�N�X���X�V���ăy�[�W�̍Ō�̗v�f��I������
				selectedSkillIndex = (SkillCurentPage + 1) * itemPerPage - 1;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A���̗v�f��I��
			--selectedSkillIndex;
		}
	}

	// ���L�[�������ꂽ�Ƃ��̏���
	if (selectedSkillIndex == GameManager::getGameManager()->getInventory()->getSkillList().size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		// �������Ȃ�
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selectedSkillIndex + 1) % itemPerPage == 0) {
			// �J�[�\�����y�[�W���̍ŉ����ɂ���ꍇ�́A����̃y�[�W�̍ŏ��̗v�f��I��
			// �X�L�������Ă�C���x���g���� 20 �ōl���Ă���� 4 �y�[�W���܂ŊJ����悤�ɂ���
			if (SkillCurentPage < 4) {
				++SkillCurentPage;
				// �C���f�b�N�X���X�V���ăy�[�W�̍ŏ��̗v�f��I������
				selectedSkillIndex = currentPage * itemPerPage;
			}
		}
		else {
			// ����ȊO�̏ꍇ�́A����̗v�f��I��
			++selectedSkillIndex;
		}
	}

	// selectedSkillIndex ���L���Ȕ͈͓��ɂ��邩�m�F���āA�I�����ꂽ�X�L���� ID ���擾����
	if (selectedSkillIndex >= 0 && selectedSkillIndex < GameManager::getGameManager()->getInventory()->getSkillList().size()) {
		Shared<Skill> selectedSkill = GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex];
		selectedSkillId = selectedSkill->getId();
	}

	//���Z�̃J�[�\���ړ�
	GameManager::getGameManager()->getInventory()->SkillCousorMove();

	//�����������烁�j���[��߂�
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		//���ʉ���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�s�����ڂ�
		select_action_menu = MenuAction::fight_action;
		//���j���[��؂�ւ���
		attack_window->SetSelectCousourMove(True);
	}
}

//�X�L���̕`��
void BattleScene::InventorySkillDraw() {

	//�X�L����0�̎��͕`�悵�Ȃ�
	if (GameManager::getGameManager()->getInventory()->getSkillNum() > 0) {

		// �X�L���C���x���g���̃X�L����2�s���\������
		for (int i = 0; i < itemPerPage; ++i) {
			int skillIndex = SkillCurentPage * itemPerPage + i;
			if (skillIndex >= GameManager::getGameManager()->getInventory()->getSkillList().size()) {
				break; // �X�L���C���x���g���͈̔͊O��������I������
			}

			//�X�L���z����擾����
			auto& skillDataPtr = GameManager::getGameManager()->getInventory()->getSkillList()[skillIndex];

			// �X�L���̕`��
			DrawStringEx(350, StartY + i * lineHeight, Color_White, "%s", skillDataPtr->getName().c_str());

			// �I�����ꂽ�X�L���̃C���f�b�N�X�Ɋ�Â��ăJ�[�\����`��
			if (skillIndex == selectedSkillIndex) {
				// �J�[�\����`�悷��ʒu���v�Z
				int cursorY = StartY + i * lineHeight;
				ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", 330, cursorY, cursourSize, 0, true);
			}
		}
	}
}

//�X�L�����g���邩�ǂ����̃`�F�b�N
void BattleScene::SkillUseMpChack(Player::PlayerStatus& playerStatus)
{
	if (playerStatus.getCurentMp() < GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getSkillConsumeMp()) {
		battle_log->addLog("Mp������܂���");
		Skill_Chaeck_Flag = false;
		SkillUseFlag = false;
		return;
	}
	GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillMpConsume(playerStatus);
	Skill_Chaeck_Flag = true;
}

//�X�L�����g�p�����ۂ̏���
void BattleScene::SkillUseProcess(Player::PlayerStatus& playerStatus,Enemy::EnemyStatus& enemyStatus_)
{
	auto enemyHp = enemyStatus_.getEnemyHp();

	if (SkillUseFlag) {
		//�X�L�����g���邩�ǂ����̃`�F�b�N���s��
		SkillUseMpChack(playerStatus);
		//�X�L�����g���Ȃ����珈�����΂�
		if (!Skill_Chaeck_Flag) {
			select_action_menu = MenuAction::fight_action;
			return;
		}
		//�U���n�̃X�L���̏ꍇ
		if (GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getSkillType() == 0) {
			if (GameManager::getGameManager()->getInventory()->getSkillNum() > 0) {
				//�X�L���̃A�j���[�V�����𗬂�
				GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUseAnimation();
				//���Z�̃A�j���[�V�������Đ�����
				Skill_Attack_Flag = true;
			}
			//�_���[�W���v�Z����
			SkillDamage = GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUse(playerStatus, enemyStatus_);
			enemyStatus_.SetEnemyHp(enemyHp - SkillDamage);
			battle_log->addSkillUseLog("Player", GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getName().c_str(), "Enemy", SkillDamage);
		}
		//�o�t�n�̃X�L���̏ꍇ
		else if(GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getSkillType() == 1)  {
			Skill_Buff_Flag = true;
			//�o�t���ʂ��i�[����
			int buff = GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUse(playerStatus);
			//����炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);
			//���O�𗬂�
			battle_log->addRecoveryLog("Player", GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getName(), buff);
		}
		//�A�C�e���n�̏ꍇ
		else {
			if (GameManager::getGameManager()->getInventory()->getSkillNum() > 0) {
				//�X�L���̃A�j���[�V�����𗬂�
				GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUseAnimation();
				//���Z�̃A�j���[�V�������Đ�����
				Skill_Attack_Flag = true;
			}
			//�_���[�W���v�Z����
			SkillDamage = GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUse(enemyStatus_);
			enemyStatus_.SetEnemyHp(enemyHp - SkillDamage);
			battle_log->addSkillUseLog("Player", GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getName().c_str(), "Enemy", SkillDamage);
		}

		// �G�l�~�[�̗̑͂�0�ȉ��ɂȂ�����A���S���o + ���x���A�b�v����
		if (enemyStatus_.getEnemyHp() <= 0) {
			deadEnemyProcess(playerStatus, enemyStatus_);
			levelUpProcess(playerStatus);
		}

		//�t���O��؂�ւ���
		SkillUseFlag = false;
		PlayerTurn = false;

		//�X�L���I���E�B���h�E�ɖ߂�
		select_action_menu = MenuAction::fight_action;
		//�J�[�\���𓮂���悤�ɂ���
		attack_window->SetSelectCousourMove(True);

	}
}

//------------------------------------------------------------------------------------------------------------------------
//���j���[�֘A 

//�������I�������ۂ̏���
void BattleScene::FleeProcess(float delta_time)
{
	if (flee_flag) {
		//�����鎞��SE�𗬂�
		if (GameManager::getGameManager()->TimeCount(delta_time, flee_Time)) {
			auto mgr = SceneManager::GetInstance();
			//�V�[����؂�ւ���
			mgr->changeScene(new MapScene(map_pos));
			//�T�E���h���~�߂�
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		}
	}
}

//���j���[���̏���
void BattleScene::MenuUpdate(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerAttack = playerStatus.getAttack();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyHp = enemyStatus_.getEnemyHp();

	switch (select_action_menu)
	{
		//��ԍŏ��̃��j���[�̎��̏���
	case BattleScene::MenuAction::first_action:

		//�U��
		if (Select_Action_Menu->getSelectNum() == Attack && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::fight_action;

		}
		//����
		else if (Select_Action_Menu->getSelectNum() == Item && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::item_action;
		}
		//������
		else if (Select_Action_Menu->getSelectNum() == Flee && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�v���C���[�ƓG�̑f�������擾����
			auto playerSpeed = playerStatus.getSpeed();
			auto enemySpeed = enemyStatus_.getEnemySpeed();

			// �v���C���[�̑f�������G���������ꍇ�A���������������߂�
			if (playerSpeed > enemySpeed) {
				// �������������㏸�����鏈��
				probability += 20; // 20%�̃{�[�i�X��^����
			}
			// �G�̑f�������v���C���[���������ꍇ�A������������ቺ������
			else if (playerSpeed < enemySpeed) {
				// ������������ቺ�����鏈��
				probability -= 20; // 20%�̃y�i���e�B��^����
			}

			// �����̐����m���𔻒肷��
			if (rand() % 100 < probability) {
				// �����ɐ��������ꍇ�̏���
				battle_log->addLog("���܂������ꂽ");

				//�t���O�𗧂Ă�
				flee_flag = true;
				//�퓬BGM���~�߂�
				SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
				//������̌��ʉ��𗬂�
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/nigeru.mp3", DX_PLAYTYPE_BACK);
				//���ʂ�ς���
				SoundManager::getSoundManager()->ChangeSoundVolume(50, "sound/SoundEffect/nigeru.mp3");
			}
			else {
				// �����Ɏ��s�����ꍇ�̏���
				battle_log->addLog("�������Ȃ�����");
				flee_flag = false;
				sequence_.change(&BattleScene::seqChangeScene);
				PlayerTurn = false;
			}
		}
		break;

		//�U���I���̎���
	case BattleScene::MenuAction::fight_action:

		//�U���������ꂽ����
		if (attack_window->getSelectNum() == Attack && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�U���������̃A�j���[�V����
			Nomal_Attack_Flag = true;

			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

			// �_���[�W���v�Z���A�_���[�W�� 0 �ȉ��Ȃ�΁A1�ɂ��� ( �Œ�ł�1�_���[�W�͗^����
			int damage = nomal_attack->SkillUse(playerStatus, enemyStatus_);

			// �G��HP�����炷
			enemyStatus_.SetEnemyHp(EnemyHp - damage);

			// �퓬���O�Ƀ_���[�W���ʂ��o��
			battle_log->addDamageLog("Player", "Enemy", damage);

			// �G�l�~�[�̗̑͂�0�ȉ��ɂȂ�����A���S���o + ���x���A�b�v����
			if (enemyStatus_.getEnemyHp() <= 0) {
				deadEnemyProcess(playerStatus, enemyStatus_);
				levelUpProcess(playerStatus);
			}
			// �����łȂ��Ȃ�A���ʂɃA�j���[�V�������s
			nomal_attack->SkillUseAnimation();
			PlayerTurn = false;
		}
		//���Z���I�����ꂽ��
		else if (attack_window->getSelectNum() == SpecialSkill && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			select_action_menu = MenuAction::skill_action;
		}
		//���邪�I�����ꂽ��
		else if (attack_window->getSelectNum() == MenuClose && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			select_action_menu = MenuAction::first_action;

		}

		break;

	case BattleScene::MenuAction::item_action:

		//�A�C�e���I������
		ItemSelectProcess();

		break;

		//�g�����I�����ꂽ��
	case MenuAction::item_use_action:

		//�A�C�e�����g�����ۂ̏���
		ItemUseProcess();

		break;

		//�X�L������
	case MenuAction::skill_action:

		//�X�L���I�����̏���
		SkillSelectProcess();

		break;

	default:
		break;
	}

}

//------------------------------------------------------------------------------------------------------------------------
//�v���C���[�֘A


//�A�j���[�V�����g�p���̃^�[���`�F���W
void BattleScene::AnimationTurnChange(int enemyhp , float delta_time)
{
	//�X�L�����g���Ȃ����珈�����΂�
	if (!Skill_Chaeck_Flag) {
		Skill_Attack_Flag = false;
		return;
	}
	//�A�j���[�V�����𗬂������Ă���^�[�������ւ���
	if (Nomal_Attack_Flag && enemyhp > 0 || Skill_Attack_Flag && enemyhp > 0) {
		//�A�j���[�V������1�b�Ԃ𗬂�
		if (GameManager::getGameManager()->TimeCount(delta_time, Animation_Time)) {
			//�^�[����؂�ւ���
			sequence_.change(&BattleScene::seqChangeScene);
		}
	}
	//�o�t�n�̃X�L�����g�����ꍇ�̓^�[�������ւ���
	else if (Skill_Buff_Flag) {
		//�^�[����؂�ւ���
		sequence_.change(&BattleScene::seqChangeScene);
	}
}

//���x���A�b�v
void BattleScene::levelUpProcess(Player::PlayerStatus& playerStatus) {

	//���x�����擾����
	auto Playerlevel = playerStatus.getLevel();

	//���x��10�ȏ�ł���΃��x���A�b�v���Ȃ�(���㑝�₵�čs���\��)
	if (Playerlevel >= 10) {
		return;
	}

	//�v���C���[�̕K�v�Ȍo���l��0�ȉ��ɂȂ������Ƀ��x����������
	if (playerStatus.getExpoint() <= 0)
	{
		//���x����������
		levelUp_flag = true;
		DeadEnemy_flag = true;
	}
	//�K�v�Ȍo���l��0�ɂȂ�Ȃ�������
	else {
		DeadEnemy_flag = true;
	}

}

//�v���C���[��HP��MP�o�[�̏���
void BattleScene::PlayerStatusDraw()
{
	//�v���C���[�̃X�e�[�^�X���擾����
	auto plyerstatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

	//�E�B���h�E��`�悷��
	playerStatus_window->Menu_draw();

	//�v���C���[�̉摜
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/Player/Player_Icon_sentou.png" , 90 , 120  ,1.3f, 0 , true);

	//hp�o�[�̉摜
	int hpber_hdl =  ResourceManager::getResourceManager()->LoadGraphEX("graphics/hpbar.png");
	//mp�o�[�̉摜
	int mpber_hdl =  ResourceManager::getResourceManager()->LoadGraphEX("graphics/mpbar.png");
	//hp�o�[�ő�̒���
	int maxHpBar = 200;
	//mp�o�[�̍ő�̒���
	int maxMpBer = 200;

	// HP�̊������v�Z����
	float hp_ratio = plyerstatus.getcurentHp() / plyerstatus.getMaxHp();
	// HP�o�[�̒������v�Z����
	float hpbar = static_cast<int>(maxHpBar * hp_ratio);

	// MP�̊������v�Z����
	float mp_ratio = plyerstatus.getCurentMp() / plyerstatus.getMaxMp();
	// MP�o�[�̒������v�Z����
	float mpbar = static_cast<int>(maxMpBer * mp_ratio);

	DrawExtendGraph(75, 155, (75 + hpbar), 185, hpber_hdl, true);
	DrawExtendGraph(75, 215, (75 + mpbar), 245, mpber_hdl, true);

	//�v���C���[��HP��\������
	DrawStringEx(playerStatus_window->menu_x + 25, playerStatus_window->menu_y + 20, -1, "Player");
	DrawStringEx(120, 110, -1, " Lv %d" , plyerstatus.getLevel());
	DrawStringEx(playerStatus_window->menu_x + 25, playerStatus_window->menu_y + 140, -1, " Hp : %d / %d", static_cast<int>(plyerstatus.getcurentHp()),static_cast<int>(plyerstatus.getMaxHp()));
	DrawStringEx(playerStatus_window->menu_x + 25, playerStatus_window->menu_y + 200, -1, " Mp : %d / %d", static_cast<int>(plyerstatus.getCurentMp()), static_cast<int>(plyerstatus.getMaxMp()));



}

//------------------------------------------------------------------------------------------------------------------------
//�G�֘A

//�G��HP��0�ɂȂ������̏���
void BattleScene::deadEnemyProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_) {

	//�K�v�Ȍo���l���擾����
	auto R_point = playerStatus.getExpoint();

	//�G�̌o���l���擾����
	auto Enemy_expoint = enemyStatus_.getEnemyExpoint();

	// �f�[�^����
	playerStatus.SetPlayerExpoint(R_point - Enemy_expoint);

	//��������ǉ�����
	GameManager::getGameManager()->getPlayer()->AddPlayerMoney(enemyStatus_.getEnemyMoney());
}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A(��Ƀ^�[������)

//�ŏ��̑f�������r����
bool BattleScene::seqIdle(float delta_time)
{
	//�X�e�[�^�X���擾����
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy_Array[enemy_index];

	//�V�[�P���X�̏�����
	if (sequence_.isStart()) {
		//�f�������r����
		//�v���C���[�������ꍇ
		if (PlayerStatus.getSpeed() >= enemyStatus.getEnemySpeed()) {
			//�v���C���[�̃V�[�P���X��
			sequence_.change(&BattleScene::seqPlayerAction);
			PlayerTurn = true;
		}
		//�G�l�~�[�������ꍇ
		else {

			//�G�l�~�[�̃V�[�P���X��
			sequence_.change(&BattleScene::seqEnemyAction);

		}
	}

	//�f�o�b�N
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new ResultScene());
	}

	return true;
}

//�v���C���[�̃^�[��
bool BattleScene::seqPlayerAction(float delta_time)
{
	auto& playerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy_Array[enemy_index];
	auto enemyHp = enemyStatus.getEnemyHp();

	if (sequence_.isStart()) {
		select_sequence = Sequence::PlayerAction;

		//���O�����Z�b�g����
		battle_log->LogReset();

		//���O�𗬂�
		battle_log->addLog("[ �v���C���[�̃^�[�� ]");

	}

	if (PlayerTurn) {
		//�v���C���[�̍X�V����
		PlayerUpdate();
	}

	//�G��|������1�b�ԑ҂��Ă���A�j���[�V�������~�߂�ׂ̃t���O
	if (DeadEnemy_flag) {

		if (GameManager::getGameManager()->TimeCount(delta_time, 1)) {
			//�V�[����J�ڂ�����ׂ̃V�[�P���X
			sequence_.change(&BattleScene::seqChangeScene);
		}
	}

	//�X�L�����g�p�����ۂ̏���
	SkillUseProcess(playerStatus, enemyStatus);

	//�A�j���[�V�������g�p���ɃA�j���[�V�����𗬂��Ă���^�[�����`�F���W������
	AnimationTurnChange(enemyHp, delta_time);

	//������
	FleeProcess(delta_time);

	return true;

}

//�G�̃^�[��
bool BattleScene::seqEnemyAction(float delta_time)
{
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy_Array[enemy_index];
	auto enemyAttack = enemyStatus.getEnemyAttack();
	auto PlayerDefance = PlayerStatus.getDefance();
	auto PlayerHp = PlayerStatus.getcurentHp();

	//�V�[�P���X���n�܂����ŏ��̈��̂ݏ������s��
	if (sequence_.isStart()) {

		//�A�j���[�V�������~�߂�
		if (Nomal_Attack_Flag) {
			nomal_attack->SkillAnimationStop();
			Nomal_Attack_Flag = false;
		}
		else if (Skill_Attack_Flag) {
			GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillAnimationStop();
			Skill_Attack_Flag = false;
		}
		//�o�t�X�L���̃t���O��؂�ւ���
		else if (Skill_Buff_Flag) {
			Skill_Buff_Flag = false;
		}

		select_sequence = Sequence::EnemyAction;

		//���O�����Z�b�g����
		battle_log->LogReset();

		//���O�𗬂�
		battle_log->addLog("[ �G�̃^�[�� ]");

		//�_���[�W���v�Z����u
		int damage = enemyAttack - PlayerDefance;

		//�_���[�W��0�������Ƃ���0�ɐݒ肷��
		if (damage <= 0) { damage = 0; }

		//�v���C���[��Hp�����炷
		PlayerStatus.SetPlayerCurentHp(PlayerHp - damage);

		//�o�g�����O�𗬂�
		battle_log->addDamagePlayerLog("Enemy", "Player", damage);

		//SE�𗬂�
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/dageki_2.mp3", DX_PLAYTYPE_BACK);

		//�{�����[����ς���
		SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/dageki_2.mp3");

		//�v���C���[��Hp��0�ɂȂ�����S�ł̃V�[���ɐ؂�ւ���
		if (PlayerStatus.getcurentHp() <= 0) {

			//���O�𗬂�
			battle_log->addLog("�S�ł���");

			//�v���C���[��Hp��0�ɂ���
			PlayerStatus.SetPlayerCurentHp(0);

			//BGM���~�߂�
			SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/zenmetu.mp3", DX_PLAYTYPE_BACK);

			//�V�[����J�ڂ�����
			annihilation_flag = true;
		}
	}
	//�S�ł�����
	if (annihilation_flag) {
		if (GameManager::getGameManager()->TimeCount(delta_time, annihilation_Time)) {
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new ResultScene());
			annihilation_flag = false;
		}
	}
	//Hp��0�ɂȂ�Ȃ�������^�[����؂�ւ���
	else if(/*SystemManager::getSystemManager()->TimeCount(delta_time, Change_Turn_Time)*/tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		sequence_.change(&BattleScene::seqPlayerAction);
		select_sequence = Sequence::PlayerAction;
		PlayerTurn = true;
	}


	return true;
}

//�V�[����J�ڂ�����ׂ̃V�[�P���X
bool BattleScene::seqChangeScene(float delta_time)
{
	//�G�̃X�e�[�^�X
	auto& enemyStatus = enemy_Array[enemy_index];
	//�v���C���[�̃X�e�[�^�X
	auto& playerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

	if (sequence_.isStart()) {
		// ���o
		//�A�j���[�V�������Ƃ߂�
		if (Nomal_Attack_Flag) {
			nomal_attack->SkillAnimationStop();
		}
		else if (Skill_Attack_Flag) {
			GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillAnimationStop();
		}

		//�G��|���Ă����ꍇ
		if (DeadEnemy_flag) {

			//���O�𗬂�
			battle_log->addLog("�G��|�����I");

			//�G�̉摜������
			enemy_annihilation_flag = true;

			//�A�C�e���̃h���b�v����
			ItemDropProcess();

			//���x���A�b�v���Ă����ꍇ
			if (levelUp_flag) {

			//���ʉ���炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/levelup.mp3", DX_PLAYTYPE_BACK);

			//���O���o��
			battle_log->addLog("���x���A�b�v�����I");

			//�V�������x�����擾����
			auto Playerlevel = playerStatus.getLevel() + 1;

			//�v���C���[�̃X�e�[�^�X���Z�b�g����
			GameManager::getGameManager()->getPlayer()->SetPlayerStatus(Playerlevel);

			//�v���C���[������𑕔����Ă�����
			if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack) != 0) {
				auto playerAttack = playerStatus.getAttack();
				playerStatus.SetPlayerAttack(playerAttack + GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack));
			}

			//�v���C���[���h��𑕔����Ă�����
			if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance) != 0) {
				auto playerDefance = playerStatus.getDefance();
				playerStatus.SetPlayerDefance(playerDefance + GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance));
			}

			//�X�L�����Z�b�g����
			GameManager::getGameManager()->getInventory()->SkillSet();
				
		    }
			else {
				//��������炷
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/syouri.mp3", DX_PLAYTYPE_BACK);
				SoundManager::getSoundManager()->ChangeSoundVolume(60, "sound/SoundEffect/syouri.mp3");
			}

			//�����ꎞ�I�ɍU���͂��オ���Ă����炻������Z�b�g����
			BattleBuffResetProcess();
			
			//�o�g���t���O��؂�ւ���
			GameManager::getGameManager()->getInventory()->BattleFlagSwitch(1);

			//�S�[���h����ɓ��ꂽ���̃��O�𗬂�
			battle_log->addMoney("�v���C���[", enemyStatus.getEnemyMoney());

		}
	}
	//�G������Ă�����Enter�L�[�������đJ�ڂ�����
	if (DeadEnemy_flag) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new MapScene(map_pos));
		}
	}
	//�G������ĂȂ��ꍇ�G�Ƀ^�[����ύX����
	else {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			sequence_.change(&BattleScene::seqEnemyAction);
			select_sequence = Sequence::EnemyAction;
		}
	}

	return false;
}
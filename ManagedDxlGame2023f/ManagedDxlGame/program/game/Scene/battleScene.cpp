#include "battleScene.h"
#include"../Object/Actor.h"


//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

BattleScene::BattleScene(tnl::Vector3 pos, int background, int EnemyID, Shared<Enemy>enemy_pointer) : background_(background), map_pos(pos)
{
	//�G�̃|�C���^��������
	enemy = enemy_pointer;

	//�G�̃|�C���^�𐶐�����
	enemy->InitEnemyPointer(enemy, EnemyID);

	//�o�g�����O������������
	battle_log = std::make_shared<BattleLog>();

	//���j���[�̏�����
	InitMenuWindow();

	//�v���C���[�̏�Ԃ�IDLE�ɂ���(�R�}���h���͎�t�̈�)
	GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

	//����̃^�C�v�Z�b�g����
	SetWeaponType();

	//�ʏ�U���̃|�C���^���Z�b�g����
	nomal_attack = std::make_shared<Nomal_Attack>(weapon_type);

	//�o�g���t���O��؂�ւ���(�o�g���݂̂ł���A�C�e���̈�)
	GameManager::getGameManager()->getInventory()->BattleFlagSwitch(0);

	//�o�g�����O���Z�b�g����
	GameManager::getGameManager()->getInventory()->SetBattleLog(battle_log);

	//�C���f�b�N�X������������
	GameManager::getGameManager()->getInventory()->SelectedIndexClear();

	//�J�[�\�������Z�b�g����
	GameManager::getGameManager()->getInventory()->CursorReset();

	//�o�g���V�[�����Z�b�g����
	/*for (auto actor : actor_list) {
		actor->SetBattleScene(this);
	}*/
	enemy->SetBattleScene(this);


	//�v���C���[�̃o�g���V�[�����Z�b�g����
	GameManager::getGameManager()->getPlayer()->SetBattleScene(this);

	//�o�g���V�[���̏�Ԃ��o�g���ɕύX����
	battle_state = BattleState::BATTLE;

}

//�f�X�g���N�^
BattleScene::~BattleScene()
{
	auto& enemyArray = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];

	//�T�E���h����������
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/levelup.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/BGM/sentou.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/dageki_2.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/nigeru.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/syouri.mp3");

	//�摜����������
	ResourceManager::getResourceManager()->deleteGraphEx(enemyArray.getEnemyGhdl().c_str());
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

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void BattleScene::Update(float delta_time)
{
	//�퓬���s���Ă���Ԃ̓V�[�P���X�𓮂���
	if (battle_state == BattleState::BATTLE) {
		//�V�[�P���X�̍X�V����
		tnl_sequence_.update(delta_time);

		//�v���C���[�̃^�[���̂�
		if (PlayerTurn) {
			//�v���C���[�̍X�V����
			GameManager::getGameManager()->getPlayer()->PlayerMoveProcess(delta_time, battle_log, enemy, nomal_attack, map_pos);
		}

	}
	//�ҋ@��Ԃ̏ꍇ��Enter�L�[�̂ݎ�t���A�����ꂽ��V�[����J�ڂ�����
	else if (battle_state == BattleState::IDLE) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new MapScene());
		}
	}

	//�A�j���[�V�����̍X�V����
	nomal_attack->SkillAnimationUpdate(delta_time);
	//�A�j���[�V�����z�񂪖������͏������΂�
	if (GameManager::getGameManager()->getInventory()->getSkillNum() == 0) {
		return;
	}
	//�A�j���[�V�����̍X�V����
	GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationUpdate(delta_time);

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
	enemy->Draw();
	
	//�A�j���[�V�����̕`��
	nomal_attack->SkillAnimationDraw();

	DrawStringEx(360, 200, -1, "�U����\n%d", GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().getAttack());

	//�A�j���[�V�����z�񂪖������͏������΂�
	if (GameManager::getGameManager()->getInventory()->getSkillNum() == 0) {
		return;
	}
	else {
		//���Z�̃A�j���[�V�����̕`��
		GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationDraw();
	}

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
		GameManager::getGameManager()->getInventory()->ItemMenu(item_draw_pos, item_curent_page , curourY, itemPerPage);
	}
	//�A�C�e�����g�����̃E�B���h�E
	else if (select_action_menu == MenuAction::item_use_action) {
		Select_Action_Menu->All();
		Select_Action_Menu->SetSelectCousourMove(False);
		item_window->Menu_draw();
		GameManager::getGameManager()->getInventory()->ItemMenu(item_draw_pos, item_curent_page , curourY, itemPerPage);
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


//�A�C�e���̑I������
void BattleScene::ItemSelectProcess()
{

	//�C���f�b�N�X�̑���Ǝ擾
	GameManager::getGameManager()->getInventory()->ItemCurourIndex(itemPerPage);

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
		GameManager::getGameManager()->getInventory()->InventoryItemUse(GameManager::getGameManager()->getInventory()->GetSelectedItemId());
		//�s�����ڂ�
		select_action_menu = MenuAction::first_action;
		//�J�[�\���𓮂���悤�ɂ���
		Select_Action_Menu->SetSelectCousourMove(True);
		//�V�[�P���X��؂�ւ���
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
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

	//���A�h���b�v
	if (rand_val < RareDrop ) {
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
	//�ʏ�h���b�v
	else if (rand_val > RareDrop && rand_val < (NomalDrop + RareDrop)) {
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

//������Z�b�g����
void BattleScene::SetWeaponType()
{
	//���킪��������Ă�����
	if (!GameManager::getGameManager()->getInventory()->getEquipArray().empty()) {

		auto it = GameManager::getGameManager()->getInventory()->getEquipArray().begin();

		//����̃^�C�v���擾����
		weapon_type = (*it).getItemWeapontype();

	}
	//��������Ă��Ȃ�������0�ɂ���
	else {
		weapon_type = 0;
	}
}

//������ۂ̏���
void BattleScene::FleeProcess(Player::PlayerStatus& playerStatus , Enemy::EnemyStatus& enemyStatus , float delta_time)
{
	//�v���C���[�ƓG�̑f�������擾����
	auto playerSpeed = playerStatus.getSpeed();
	auto enemySpeed = enemyStatus.getEnemySpeed();

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
		//�o�g�����I��������
		battle_state = BattleState::IDLE;
	}
	else {
		// �����Ɏ��s�����ꍇ�̏���
		battle_log->addLog("�������Ȃ�����");
		flee_flag = false;
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);
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

	//�X�L���̃J�[�\���ړ��������̃C���f�b�N�X����
	GameManager::getGameManager()->getInventory()->SkillCurourIndex();

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
		//�v���C���[�̏�Ԃ�؂�ւ���
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);
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
			if (skillIndex == GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()) {
				// �J�[�\����`�悷��ʒu���v�Z
				int cursorY = StartY + i * lineHeight;
				ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", 330, cursorY, cursourSize, 0, true);
			}
		}
	}
}

//�X�L�����g���邩�ǂ����̃`�F�b�N
bool BattleScene::SkillUseMpChack(Player::PlayerStatus& playerStatus)
{
	if (playerStatus.getCurentMp() < GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getSkillConsumeMp()) {
		battle_log->addLog("Mp������܂���");
		SkillUseFlag = false;
		return false;
	}
	GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillMpConsume(playerStatus);
	return true;
}

//�X�L�����g�p�����ۂ̏���
void BattleScene::SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
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
		if (GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getSkillType() == 0 && GameManager::getGameManager()->getInventory()->getSkillNum() > 0) {
			//���Z�̃A�j���[�V�������Đ�����
			Skill_Attack_Flag = true;

			//�_���[�W���v�Z����
			SkillDamage = GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus, enemyStatus_);
			enemyStatus_.SetEnemyHp(enemyHp - SkillDamage);
			battle_log->addSkillUseLog("Player", GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getName().c_str(), "Enemy", SkillDamage);
		}
		//�o�t�n�̃X�L���̏ꍇ
		else if (GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getSkillType() == 1) {
			Skill_Buff_Flag = true;
			//�o�t���ʂ��i�[����
			int buff = GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus);
			//����炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);
			//���O�𗬂�
			battle_log->addRecoveryLog("Player", GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getName(), buff);
		}
		//�A�C�e���n�̏ꍇ
		else {
			if (GameManager::getGameManager()->getInventory()->getSkillNum() > 0) {
				//�X�L���̃A�j���[�V�����𗬂�
				GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUseAnimation();
				//���Z�̃A�j���[�V�������Đ�����
				Skill_Attack_Flag = true;
			}
			//�_���[�W���v�Z����
			SkillDamage = GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(enemyStatus_);
			enemyStatus_.SetEnemyHp(enemyHp - SkillDamage);
			battle_log->addSkillUseLog("Player", GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getName().c_str(), "Enemy", SkillDamage);
		}

		//�G��HP��0�ɂȂ������̏���
		enemy->ChackDeadEnemy();

		//�v���C���[�̏�Ԃ�؂�ւ���
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);


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
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::FLEE);
		}
		break;

		//�U���I���̎���
	case BattleScene::MenuAction::fight_action:

		//�U���������ꂽ����
		if (attack_window->getSelectNum() == Attack && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�U���������̃A�j���[�V����
			Nomal_Attack_Flag = true;

			//�v���C���[�̏�Ԃ��U���ɕς���
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::NOMALATTACK);
		
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

		//Enter�L�[����������ItemUseMenu�ɕύX����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::item_use_action;
		}

		break;

		//�g�����I�����ꂽ��
	case MenuAction::item_use_action:

		//�A�C�e�����g�����ۂ̏���
		ItemUseProcess();

		break;

		//�X�L������
	case MenuAction::skill_action:

		//�X�L���I�����̏���
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::SKILLATTACK);

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
			//�^�[����؂�ւ���
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}
	//�o�t�n�̃X�L�����g�����ꍇ�̓^�[�������ւ���
	else if (Skill_Buff_Flag) {
		//�^�[����؂�ւ���
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
	}
}

//���x���A�b�v����
void BattleScene::ChackPlayerLevelUp(Player::PlayerStatus& player_status) {

	//���x�����擾����
	auto Playerlevel = player_status.getLevel();

	//���x��10�ȏ�ł���΃��x���A�b�v���Ȃ�(���㑝�₵�čs���\��)
	if (Playerlevel >= 10) {
		return;
	}

	//�v���C���[�̕K�v�Ȍo���l��0�ȉ��ɂȂ������Ƀ��x����������
	if (player_status.getExpoint() <= 0)
	{
		//���x����������
		//���ʉ���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/levelup.mp3", DX_PLAYTYPE_BACK);

		//���O���o��
		battle_log->addLog("���x���A�b�v�����I");

		//�V�������x�����擾����
		auto Playerlevel = player_status.getLevel() + 1;

		//�v���C���[�̃X�e�[�^�X���Z�b�g����
		GameManager::getGameManager()->getPlayer()->SetPlayerStatus(Playerlevel);

		//�v���C���[������𑕔����Ă�����
		if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack) != 0) {
			auto playerAttack = player_status.getAttack();
			player_status.SetPlayerAttack(playerAttack + GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack));
		}

		//�v���C���[���h��𑕔����Ă�����
		if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance) != 0) {
			auto playerDefance = player_status.getDefance();
			player_status.SetPlayerDefance(playerDefance + GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance));
		}

		//�X�L�����Z�b�g����
		GameManager::getGameManager()->getInventory()->SkillSet();
	}
	//�K�v�Ȍo���l��0�ɂȂ�Ȃ�������
	else {
		//��������炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/syouri.mp3", DX_PLAYTYPE_BACK);
		SoundManager::getSoundManager()->ChangeSoundVolume(40, "sound/SoundEffect/syouri.mp3");
	}

}

//���x���A�b�v���̏���
void BattleScene::LevelUpProcess(Player::PlayerStatus& player_status)
{
	////���x���A�b�v���Ă����ꍇ
	//if (levelUp_flag) {

	//	//���ʉ���炷
	//	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/levelup.mp3", DX_PLAYTYPE_BACK);

	//	//���O���o��
	//	battle_log->addLog("���x���A�b�v�����I");

	//	//�V�������x�����擾����
	//	auto Playerlevel = player_status.getLevel() + 1;

	//	//�v���C���[�̃X�e�[�^�X���Z�b�g����
	//	GameManager::getGameManager()->getPlayer()->SetPlayerStatus(Playerlevel);

	//	//�v���C���[������𑕔����Ă�����
	//	if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack) != 0) {
	//		auto playerAttack = player_status.getAttack();
	//		player_status.SetPlayerAttack(playerAttack + GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack));
	//	}

	//	//�v���C���[���h��𑕔����Ă�����
	//	if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance) != 0) {
	//		auto playerDefance = player_status.getDefance();
	//		player_status.SetPlayerDefance(playerDefance + GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance));
	//	}

	//	//�X�L�����Z�b�g����
	//	GameManager::getGameManager()->getInventory()->SkillSet();
	//}
	//else if(DeadEnemy_flag){
	//	//��������炷
	//	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/syouri.mp3", DX_PLAYTYPE_BACK);
	//	SoundManager::getSoundManager()->ChangeSoundVolume(60, "sound/SoundEffect/syouri.mp3");
	//}
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


//�G�̎��S�����A���o
void BattleScene::DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyStatus& enemy_status)
{
	
	//���O�𗬂�
	battle_log->addLog("�G��|�����I");

	//�A�C�e���̃h���b�v����
	ItemDropProcess();

	//�����ꎞ�I�ɍU���͂��オ���Ă����炻������Z�b�g����
	BattleBuffResetProcess();

	//�o�g���t���O��؂�ւ���
	GameManager::getGameManager()->getInventory()->BattleFlagSwitch(1);

	//�K�v�Ȍo���l���擾����
	auto R_point = playerStatus.getExpoint();

	//�G�̌o���l���擾����
	auto Enemy_expoint = enemy_status.getEnemyExpoint();

	// �f�[�^����
	playerStatus.SetPlayerExpoint(R_point - Enemy_expoint);

	//�S�[���h����ɓ��ꂽ���̃��O�𗬂�
	battle_log->addMoney("�v���C���[", enemy_status.getEnemyMoney());
	
	//��������ǉ�����
	GameManager::getGameManager()->getPlayer()->AddPlayerMoney(enemy_status.getEnemyMoney());
}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A(��Ƀ^�[������)

//�ŏ��̑f�������r����
bool BattleScene::seqIdle(float delta_time)
{
	//�X�e�[�^�X���擾����
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];

	//�V�[�P���X�̏�����
	if (tnl_sequence_.isStart()) {
		//�f�������r����
		//�v���C���[�������ꍇ
		if (PlayerStatus.getSpeed() >= enemyStatus.getEnemySpeed()) {
			//�v���C���[�̃V�[�P���X��
			tnl_sequence_.change(&BattleScene::seqPlayerAction);
			PlayerTurn = true;
		}
		//�G�l�~�[�������ꍇ
		else {

			//�G�l�~�[�̃V�[�P���X��
			tnl_sequence_.change(&BattleScene::seqEnemyAction);

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
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];
	auto enemyHp = enemyStatus.getEnemyHp();

	if (tnl_sequence_.isStart()) {

		PlayerTurn = true;

		select_sequence = Sequence::PlayerAction;

		//���O�����Z�b�g����
		battle_log->LogReset();

		//���O�𗬂�
		battle_log->addLog("[ �v���C���[�̃^�[�� ]");
	
	}

	//�X�L�����g�p�����ۂ̏���
	SkillUseProcess(playerStatus, enemyStatus);

	//�A�j���[�V�������g�p���ɃA�j���[�V�����𗬂��Ă���^�[�����`�F���W������
	AnimationTurnChange(enemyHp, delta_time);

	return false;

}

//�G�̃^�[��
bool BattleScene::seqEnemyAction(float delta_time)
{
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];

	//�V�[�P���X���n�܂����ŏ��̈��̂ݏ������s��
	if (tnl_sequence_.isStart()) {

		PlayerTurn = false;

		select_sequence = Sequence::EnemyAction;

		//���O�����Z�b�g����
		battle_log->LogReset();

		//���O�𗬂�
		battle_log->addLog("[ �G�̃^�[�� ]");

		//�G�̍s�����s��
		enemy->EnemyAction(battle_log);

		//SE�𗬂�
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/dageki_2.mp3", DX_PLAYTYPE_BACK);

		//�{�����[����ς���
		SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/dageki_2.mp3");

		//�v���C���[��Hp��0�ɂȂ�����S�ł̃V�[���ɐ؂�ւ���
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::DEAD);
		PlayerTurn = true;
		
	}
	//Hp��0�ɂȂ�Ȃ�������^�[����؂�ւ���
	if (PlayerStatus.getcurentHp() >= 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//��Ԃ�ʏ�ɖ߂��ăR�}���h�o�g�����ēx�s��
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

		tnl_sequence_.change(&BattleScene::seqPlayerAction);
		select_sequence = Sequence::PlayerAction;
	}

	return false;
}

//�^�[����؂�ւ���V�[�P���X
bool BattleScene::seqChangeTurn(float delta_time)
{
	//Enter�L�[����������V�[�P���X��J�ڂ�����
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		tnl_sequence_.change(&BattleScene::seqEnemyAction);
		select_sequence = Sequence::EnemyAction;
	}


	return false;
}

//�A�j���[�V�����𗬂��V�[�P���X
bool BattleScene::seqAnimation(float delta_time)
{
	if (Nomal_Attack_Flag) {

		if (tnl_sequence_.isStart()) {
			//�X�L���̃A�j���[�V�����𗬂�
			nomal_attack->SkillUseAnimation();
		}

		//�v���C���[����̓��͂��󂯕t���Ȃ��悤�ɂ���
		PlayerTurn = false;

		if (tnl_sequence_.getProgressTime() > 1.0f) {

			//�̃A�j���[�V�������~�߂�
			nomal_attack->SkillAnimationStop();

			//�t���O�����
			Nomal_Attack_Flag = false;

			//�G������ł��邩�m�F���Ă�������ł�����o�g�����I��������
			if (enemy->ChackDeadEnemy()) {

				//�G�̉摜������
				enemy->DeadEnemyFlag();

				//�o�g�����I��������
				battle_state = BattleState::IDLE;

				return false;

			}

			//�G������ł��Ȃ�������V�[�P���X��؂�ւ���
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}
	}
	else if (Skill_Attack_Flag) {

		if (tnl_sequence_.isStart()) {
			//�X�L���̃A�j���[�V�����𗬂�
			GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUseAnimation();
		}

		//�v���C���[����̓��͂��󂯕t���Ȃ��悤�ɂ���
		PlayerTurn = false;

		if (tnl_sequence_.getProgressTime() > 0.5f) {

			//�X�L���̃A�j���[�V�������~�߂�
			GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationStop();

			//�t���O�����
			Skill_Attack_Flag = false;

			//�V�[�P���X��؂�ւ���
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}


	}

	return false;

}

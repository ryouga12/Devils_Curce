#include "battleScene.h"
#include"../Object/Actor.h"
#include"../Item/Item.h"
#include"../Menu/BattleLog.h"
#include"../Manager/GameManager.h"
#include"../Manager/UiManager.h"
#include"mapScene.h"
#include"resultScene.h"
#include"../System/EventNpc.h"
#include"../Scene/tittleScene.h"


//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

BattleScene::BattleScene(tnl::Vector3 pos, int background, Shared<Enemy> enemy_pointer, int inmap_state) : back_ground_(background), map_pos(pos), inmap_state_save(inmap_state)
{
	//���݂̃V�[����BattleScene�ɐݒ�
	curent_scene = SceneState::BATTLE;

	//�o�g���V�[���̏�Ԃ��o�g���ɕύX����
	battle_state = BattleState::BATTLE;

	//�G�̃|�C���^��������
	enemy = enemy_pointer;

	//�o�g�����O������������
	battle_log = std::make_shared<BattleLog>();

	//�A�C�e��������������
	item_ = std::make_shared<Item>();

	//���j���[�̏�����
	InitMenuWindow();

	//�v���C���[�̏�Ԃ�IDLE�ɂ���(�R�}���h���͎�t�̈�)
	GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

	//����̃^�C�v�Z�b�g����
	SetWeaponType();

	//�ʏ�U���̃|�C���^���Z�b�g����
	nomal_attack = std::make_shared<Nomal_Attack>(weapon_type);

	//�o�g���t���O��؂�ւ���(�o�g���݂̂ł���A�C�e���̈�)
	item_->BattleFlagSwitch();

	//�o�g�����O���Z�b�g����
	item_->SetBattleLog(battle_log);

	//�J�[�\�������Z�b�g����
	GameManager::getGameManager()->getInventory()->CursorReset();

	//�C���f�b�N�X������������
	GameManager::getGameManager()->getInventory()->SelectedIndexClear();

	/*for (auto actor : actor_list) {
		actor->SetBattleScene(this);
	}*/

	//�o�g���V�[�����Z�b�g����
	enemy->SetBattleScene(this);

	//�v���C���[�̃o�g���V�[�����Z�b�g����
	auto& PlayerPtr = GameManager::getGameManager()->getPlayer();
	GameManager::getGameManager()->getPlayer()->SetBattleScene(this);

	//�J�����̕ۊǍ��W���Z�b�g����
	GameManager::getGameManager()->getCamera()->SavePosition(map_pos);

}

//�f�X�g���N�^
BattleScene::~BattleScene()
{
	//�T�E���h���~�߂�
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {

		SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
	}
	else {
		SoundManager::getSoundManager()->StopSound("sound/BGM/maou_sentou_bgm.mp3");
	}
}

//���j���[�̏�����
void BattleScene::InitMenuWindow()
{
	//�ŏ��̍s�������߂�window
	select_action_coment = new MenuWindow::MenuElement_t[]
	{
		{MENU_COMENT_POS_FIRST.x ,MENU_COMENT_POS_FIRST.y , "��������" , 0},
		{MENU_COMENT_POS_SECOND.x , MENU_COMENT_POS_SECOND.y , "����" , 1},
		{MENU_COMENT_POS_THIRD.x , MENU_COMENT_POS_THIRD.y , "������" , 2}

	};

	select_comand_menu = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_action_coment, 3, 1);
	select_comand_menu->Open();

	//�U�������߂�E�B���h�E
	select_action_attack = new MenuWindow::MenuElement_t[]
	{
		{MENU_COMENT_POS_FIRST.x , MENU_COMENT_POS_FIRST.y ,"�U��" , 0},
		{MENU_COMENT_POS_SECOND.x , MENU_COMENT_POS_SECOND.y, "���Z" , 1},
		{MENU_COMENT_POS_THIRD.x , MENU_COMENT_POS_THIRD.y ,"�߂�" , 2}
	};

	attack_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_action_attack, 3, 1);
	attack_window->Open();

	//�A�C�e�����g�p�̃E�B���h�E
	select_detail = new MenuWindow::MenuElement_t[]{
		{ MENU_ITEM_USE_POS.x , MENU_ITEM_USE_POS.y , "�g��", 0},
		{ MENU_ITEM_CLOSE.x , MENU_ITEM_CLOSE.y , "����", 1}
	};

	select_itemuse_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_detail, 2, 1);
	select_itemuse_window->Open();

	//map�Ɋi�[����
	UIManager::getUIManager()->addMenu("select_use_window", select_itemuse_window);

	//�E�B���h�E
	window_log = UIManager::getUIManager()->getMenu("menu_window");
}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void BattleScene::Update(float delta_time)
{
	//���ʏ���
	__super::Update(delta_time);

	//�J��������
	GameManager::getGameManager()->getCamera()->update();

	//�퓬���s���Ă���Ԃ̓V�[�P���X�𓮂���
	if (battle_state == BattleState::BATTLE) {
		//�V�[�P���X�̍X�V����
		tnl_sequence_.update(delta_time);

		//�v���C���[�̃^�[���̂�
		if (PlayerTurn) {
			//�v���C���[�̍X�V����
			GameManager::getGameManager()->getPlayer()->PlayerMoveProcess(delta_time, battle_log, enemy, nomal_attack);
		}

	}
	//�ҋ@��Ԃ��v���C���[�̏�Ԃ�DEAD�������ꍇ�A�S�ŃV�[���ɐ؂�ւ���
	else if (battle_state == BattleState::IDLE && GameManager::getGameManager()->getPlayer()->GetPlayerState() == Player::PlayerState::DEAD) {
		//���Ԃ�҂��Ă���J�ڂ�����
		if (GameManager::getGameManager()->TimeCount(delta_time, ANNIHILATION_TIME)) {
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new ResultScene());
		}
	}
	//�{�X�̏ꍇ�A�}�b�v���Ő퓬���n�܂�ׁA�퓬�I��������InMapScene�ɖ߂�
	else if (battle_state == BattleState::IDLE && enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId() != LAST_BOSS_ID) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new InMapScene(GameManager::getGameManager()->getPlayer()->getPlayerPos(), static_cast<InMapScene::InMapState>(inmap_state_save)));
		}
	}
	//�Ō�̃{�X�̏ꍇ�A�T�[�u���s���^�C�g����ʂɖ߂�
	//����N���A�V�[���Ȃǂ�������
	else if(battle_state == BattleState::IDLE && enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId() == LAST_BOSS_ID) {

		Weak save_event = std::make_shared <EventNpc>();

		//�Z�[�u���s��
		save_event.lock()->SaveProcces();

		//�^�C�g���V�[���Ɉړ�����
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new TittleScene());

	}
	//�ҋ@��Ԃ̏ꍇ��Enter�L�[�̂ݎ�t���A�����ꂽ��V�[����J�ڂ�����
	else if (battle_state == BattleState::IDLE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new MapScene());
		
	}
	

	//�G�̃G�t�F�N�g�̍X�V����
	if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//�_�E���L���X�g����
		auto boss_monster = std::dynamic_pointer_cast<BossMonster>(enemy);

		//�G�t�F�N�g�̕`��
		boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationUpdate(delta_time);
	}

	//�G�t�F�N�g�̍X�V����
	nomal_attack->SkillAnimationUpdate(delta_time);
	//�G�t�F�N�g�̔z�񂪖������͏������΂�
	if (GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {
		return;
	}
	//�G�t�F�N�g�̍X�V����
	GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationUpdate(delta_time);

}

//------------------------------------------------------------------------------------------------------------------------
//�`��
void BattleScene::Draw()
{
	//�w�i�摜
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, back_ground_, TRUE);

	//�o�g���E�B���h�E�̕`��
	BattleDraw();

	//�v���C���[�̃X�e�[�^�X�̕`��
	PlayerStatusDraw();

	//�G�̉摜�̕`��
	enemy->Draw();

	//�o�g�����O�E�B���h�E�̕`��
	if (auto battle_log_window = window_log.lock()) {
		battle_log_window->Menu_draw(670, 30, 600, 180);
	}

	//�o�g�����O�̕����̕`��
	battle_log->drawLogs();

	//���ʕ`��
	__super::Draw();

	//�G�̃G�t�F�N�g�̍X�V����
	if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//�_�E���L���X�g����
		auto boss_monster = std::dynamic_pointer_cast<BossMonster>(enemy);

		//�G�t�F�N�g�̍X�V����
		boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationDraw();
	}

	//�v���C���[�̃G�t�F�N�g�̕`��
	nomal_attack->SkillAnimationDraw();

	//�G�t�F�N�g�̔z�񂪖������͏������΂�
	if (GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {
		return;
	}
	else {
		//���Z�̃G�t�F�N�g�̕`��
		GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationDraw();
	}
}

//�o�g���E�B���h�E�̕`��
void BattleScene::BattleDraw()
{
	//�A�C�e����`�悷��E�B���h�E
	auto item_window = window_log.lock();


	//�ŏ��̍s���̎��̃E�B���h�E
	if (select_action_menu == MenuAction::FIRST_ACTION) {
		select_comand_menu->All(50, 500, 250, 200);
	}
	//�U���E�B���h�E
	else if (select_action_menu == MenuAction::FIGHT_ACTION) {
		attack_window->All(50, 500, 250, 200);
	}
	//�A�C�e���\���̃E�B���h�E
	else if (select_action_menu == MenuAction::ITEM_ACTION) {
		select_comand_menu->All(50, 500, 250, 200);
		item_window->Menu_draw(300, 500, 250, 215);
		GameManager::getGameManager()->getInventory()->ItemMenu(ITEM_DRAW_POS, ITEM_CURENT_PAGE, CUROURY, ITEMPERPAGE_);
	}
	//�A�C�e�����g�����̃E�B���h�E
	else if (select_action_menu == MenuAction::ITEM_USE_ACTION) {
		select_comand_menu->All(50, 500, 250, 200);
		item_window->Menu_draw(300, 500, 250, 215);
		GameManager::getGameManager()->getInventory()->ItemMenu(ITEM_DRAW_POS, ITEM_CURENT_PAGE, CUROURY, ITEMPERPAGE_);
		select_itemuse_window->All(550, 500, 250, 215);
	}
	//�X�L���E�B���h�E
	else if (select_action_menu == MenuAction::SKILL_ACTION) {
		attack_window->All(50, 500, 250, 200);
		InventorySkillDraw();
	}
}


//------------------------------------------------------------------------------------------------------------------------
//�A�C�e���֘A


//�A�C�e���̑I������
void BattleScene::ItemSelectProcess()
{
	//�C���f�b�N�X�̑���Ǝ擾
	GameManager::getGameManager()->getInventory()->ItemCurourIndex(ITEMPERPAGE_);

	//�A�C�e���̃J�[�\������
	GameManager::getGameManager()->getInventory()->CusorMove();

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		//���ʉ���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�s�����ڂ�
		select_action_menu = MenuAction::FIRST_ACTION;
		//���j���[��؂�ւ���
		select_comand_menu->SetSelectCousourMove();
	}

}

//�A�C�e�����g�����ۂ̏���
void BattleScene::ItemUseProcess()
{
	if (select_itemuse_window->getSelectNum() == ITEMUSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//���ʉ���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�A�C�e�����g�������̏���
		item_->ItemUse(GameManager::getGameManager()->getInventory()->GetSelectedItemId());
		//�s�����ڂ�
		select_action_menu = MenuAction::FIRST_ACTION;
		//�J�[�\���𓮂���悤�ɂ���
		select_comand_menu->SetSelectCousourMove();
		//�V�[�P���X��؂�ւ���
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
	}
	//���邪�����ꂽ��
	else if (select_itemuse_window->getSelectNum() == ITEMUSEMENUCLOSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		select_action_menu = MenuAction::ITEM_ACTION;
	}
	//���L�[�������ƍŏ��̑I�����j���[�ɖ߂�
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		select_action_menu = MenuAction::ITEM_ACTION;
	}
}

//�G��|�������̃A�C�e���h���b�v����
void BattleScene::ItemDropProcess(Enemy::EnemyConnection& enemy_)
{
	//�����_���ȃV�[�h�l�𐶐�����ׂ̕ϐ�
	std::random_device rd;

	//�^�������𐶐�����(�����Z���k�c�C�X�^�[)
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 100);

	//0����100�̈�l�����𐶐�����
	int rand_val = static_cast<int>(dis(gen));

	//�G�̃A�C�e���z����擾����
	auto& EnemyItemDrop = enemy->getItemDropArray();

	//�m�[�}���h���b�v�̊m��
	int nomal_drop = enemy_.GetNomalProbability();

	//���A�h���b�v�̊m��
	int rare_drop = enemy_.GetRareProbability();

	//���A�h���b�v
	if (rand_val < rare_drop) {
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
	else if (rand_val > rare_drop && rand_val < (nomal_drop + rare_drop)) {
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
	if (item_->getAttackAmount() != 0) {

		int AttackAmoument = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetAttack() - item_->getAttackAmount();
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
void BattleScene::FleeProcess(Player::PlayerStatus& playerStatus , Enemy::EnemyConnection& enemy_ , float delta_time)
{
	//�v���C���[�ƓG�̑f�������擾����
	auto playerSpeed = playerStatus.GetSpeed();
	auto enemySpeed = enemy_.GetEnemySpeed();

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
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�X�L���֘A

//�X�L�����I�����ꂽ���̏���
void BattleScene::SkillSelectProcess()
{
	//�X�L���̃J�[�\���ړ��������̃C���f�b�N�X����
	GameManager::getGameManager()->getInventory()->SkillCurourIndex();

	//���Z�̃J�[�\���ړ�
	GameManager::getGameManager()->getInventory()->SkillCousorMove();

	//Enter�L�[�������ꂽ����
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//�X�L���������Ȃ��ꍇ�ɂ͏������s��Ȃ�
		if (GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {
			return;
		}
		//�v���C���[�̏�Ԃ��U���ɕς��čU��
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::SKILL);
	}
	//�����������烁�j���[��߂�
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		//���ʉ���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�s�����ڂ�
		select_action_menu = MenuAction::FIGHT_ACTION;
		//�J�[�\���𓮂���悤�ɂ���
		attack_window->SetSelectCousourMove();
	}
}

//�X�L���̕`��
void BattleScene::InventorySkillDraw() {

	//�E�B���h�E�̕\��
	//�|�C���^���������ė�����E�B���h�E��`�悷��
	if (auto skill_window = window_log.lock()) {
		skill_window->Menu_draw(300, 500, 250, 215);
	}

	//�C���x���g�����̃X�L����`�悷��
	GameManager::getGameManager()->getInventory()->InventorySkill(ITEM_DRAW_POS , ITEM_CURENT_PAGE , CUROURY, ITEMPERPAGE_);
}

//�X�L�����g���邩�ǂ����̃`�F�b�N
bool BattleScene::SkillUseMpChack(Player::PlayerStatus& playerStatus)
{
	//�X�L����������Ώ������΂�
	if (GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {
		return false;
	}

	//MP������Ȃ�������
	if (playerStatus.GetCurentMp() < GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getSkillConsumeMp()) {
		battle_log->addLog("Mp������܂���");
		return false;
	}
	GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillMpConsume(playerStatus);
	return true;
}

//�X�L�����g�p�����ۂ̏���
void BattleScene::SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_)
{
	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()];

	//�X�L�����g���Ȃ����珈�����΂�
	if (!SkillUseMpChack(playerStatus)) {
		select_action_menu = MenuAction::FIGHT_ACTION;
		return;
	}
	//�U���n�̃X�L���̏ꍇ
	if (SkillList->getSkillType() == 0 && !GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {

		//�_���[�W��^����
		GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus, enemy_, battle_log);

		//�G�t�F�N�g�𗬂�
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}
	//�o�t�n�̃X�L���̏ꍇ
	else if (SkillList->getSkillType() == 1 && !GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {

		//�o�t���ʂ��v���C���[�ɗ^����
		GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus , battle_log);

		//�G�t�F�N�g�𗬂�
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}
	//�A�C�e���n�̃X�L��
	else {
		//�_���[�W��^����
		GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(enemy_, battle_log);

		//�G�t�F�N�g�𗬂�
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}

	//�v���C���[����̓��͂��󂯕t���Ȃ��悤�ɂ���
	PlayerTurn = false;
	
	//�X�L���I���E�B���h�E�ɖ߂�
	select_action_menu = MenuAction::FIGHT_ACTION;
	//�J�[�\���𓮂���悤�ɂ���
	attack_window->SetSelectCousourMove();

}

//------------------------------------------------------------------------------------------------------------------------
//���j���[�֘A 


//���j���[���̏���
void BattleScene::MenuUpdate(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemy_.GetEnemyDefance();
	auto EnemyHp = enemy_.GetEnemyHp();

	switch (select_action_menu)
	{
		//��ԍŏ��̃��j���[�̎��̏���
	case BattleScene::MenuAction::FIRST_ACTION:

		//��������
		if (select_comand_menu->getSelectNum() == ATTACK_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::FIGHT_ACTION;
			//�J�[�\���𓮂��Ȃ��悤�ɂ���
			select_comand_menu->SetSelectCousourMove();

		}
		//����
		else if (select_comand_menu->getSelectNum() == ITEM_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::ITEM_ACTION;
			//�ŏ��̉�ʂ̃J�[�\���𓮂��Ȃ�����
			select_comand_menu->SetSelectCousourMove();
		}
		//������
		else if (select_comand_menu->getSelectNum() == FLEE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::FLEE);
		}



		break;

		//�U���I���̎���
	case BattleScene::MenuAction::FIGHT_ACTION:

		//�U���������ꂽ����
		if (attack_window->getSelectNum() == ATTACK_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�v���C���[�̏�Ԃ��U���ɕς���
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::NOMALATTACK);

			//�V�[�P���X��ς���
			tnl_sequence_.change(&BattleScene::seqAnimation);
		}
		//���Z���I�����ꂽ��
		else if (attack_window->getSelectNum() == SPECIAL_SKILL_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�����Ă���X�L���̑I����������
			select_action_menu = MenuAction::SKILL_ACTION;

			//�s�������߂�ׂ̑I����ʂ̃J�[�\���𓮂��Ȃ��悤�ɂ���
			attack_window->SetSelectCousourMove();
		}
		//���邪�I�����ꂽ��
		else if (attack_window->getSelectNum() == MENU_CLOSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			select_action_menu = MenuAction::FIRST_ACTION;

			//�J�[�\���𓮂���悤�ɂ���
			select_comand_menu->SetSelectCousourMove();

		}
		
		//���L�[�������ƍŏ��̑I�����j���[�ɖ߂�
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_action_menu = MenuAction::FIRST_ACTION;
			
			//�J�[�\���𓮂���悤�ɂ���
			select_comand_menu->SetSelectCousourMove();
		}

		break;

	case BattleScene::MenuAction::ITEM_ACTION:

		//�A�C�e���I������
		ItemSelectProcess();

		//Enter�L�[����������ItemUseMenu�ɕύX����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::ITEM_USE_ACTION;
		}

		break;

		//�g�����I�����ꂽ��
	case MenuAction::ITEM_USE_ACTION:

		//�A�C�e�����g�����ۂ̏���
		ItemUseProcess();

		break;

		//�X�L������
	case MenuAction::SKILL_ACTION:

		//�X�L���I�����̏���
		SkillSelectProcess();


		break;

	default:
		break;
	}

}

//------------------------------------------------------------------------------------------------------------------------
//�v���C���[�֘A

//���x���A�b�v���� & ����
void BattleScene::ChackPlayerLevelUp(Player::PlayerStatus& player_status) {

	//���x�����擾����
	auto Playerlevel = player_status.GetLevel();

	//���x��10�ȏ�ł���΃��x���A�b�v���Ȃ�(���㑝�₵�čs���\��)
	if (Playerlevel >= 20) {
		return;
	}

	//�v���C���[�̕K�v�Ȍo���l��0�ȉ��ɂȂ������Ƀ��x����������
	if (player_status.GetExpoint() <= 0)
	{
		//���x����������
		//���ʉ���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/levelup.mp3", DX_PLAYTYPE_BACK);

		//���O���o��
		battle_log->addLog("���x���A�b�v�����I");

		//�V�������x�����擾����
		auto Playerlevel = player_status.GetLevel() + 1;

		//�v���C���[�̃X�e�[�^�X���Z�b�g����
		GameManager::getGameManager()->getPlayer()->SetPlayerStatus(Playerlevel);

		//�v���C���[������𑕔����Ă�����
		if (GameManager::getGameManager()->getInventory()->GetEquipAttack() != 0) {
			auto playerAttack = player_status.GetAttack();
			player_status.SetPlayerAttack(playerAttack + GameManager::getGameManager()->getInventory()->GetEquipAttack());
		}

		//�v���C���[���h��𑕔����Ă�����
		if (GameManager::getGameManager()->getInventory()->GetEquipDefance() != 0) {
			auto playerDefance = player_status.GetDefance();
			player_status.SetPlayerDefance(playerDefance + GameManager::getGameManager()->getInventory()->GetEquipDefance());
		}

		//�X�L�����Z�b�g����
		GameManager::getGameManager()->getPlayer()->SkillSet(battle_log);
	}
	//�K�v�Ȍo���l��0�ɂȂ�Ȃ�������
	else {
		//��������炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/syouri.mp3", DX_PLAYTYPE_BACK);
		SoundManager::getSoundManager()->ChangeSoundVolume(40, "sound/SoundEffect/syouri.mp3");
	}

}


//�v���C���[��HP��MP�o�[�̏���
void BattleScene::PlayerStatusDraw()
{
	if (auto playerStatus_window = window_log.lock()) {
		//�E�B���h�E��`�悷��
		playerStatus_window->Menu_draw(50, 50, 250, 240);
	}

	//�X�e�[�^�X��\������
	UIManager::getUIManager()->PlayerStatusDrawWindow();

}

//------------------------------------------------------------------------------------------------------------------------
//�G�֘A


//�G�̎��S�����A���o
void BattleScene::DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyConnection& enemy_)
{
	
	//���O�𗬂�
	battle_log->addLog("�G��|�����I");

	//�A�C�e���̃h���b�v����
	ItemDropProcess(enemy_);

	//�����ꎞ�I�ɍU���͂��オ���Ă����炻������Z�b�g����
	BattleBuffResetProcess();

	//�o�g���t���O��؂�ւ���
	item_->BattleFlagSwitch();

	//�K�v�Ȍo���l���擾����
	auto R_point = playerStatus.GetExpoint();

	//�G�̌o���l���擾����
	auto Enemy_expoint = enemy_.GetEnemyExpoint();

	// �f�[�^����
	playerStatus.SetPlayerExpoint(R_point - Enemy_expoint);

	//�S�[���h����ɓ��ꂽ���̃��O�𗬂�
	battle_log->addMoney("�v���C���[", enemy_.GetEnemyMoney());
	
	//��������ǉ�����
	GameManager::getGameManager()->getPlayer()->AddPlayerMoney(enemy_.GetEnemyMoney());

	//�G���{�X�������ꍇ�C�x���g���I��������
	if (enemy_.GetEnemyId() != 20 && enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		EventManager::getEventManager()->EnemyEventFlagChange();
	}
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
		if (PlayerStatus.GetSpeed() >= enemyStatus.GetEnemySpeed()) {
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
	auto enemyHp = enemyStatus.GetEnemyHp();

	if (tnl_sequence_.isStart()) {

		PlayerTurn = true;

		select_sequence = Sequence::PLAYERACTION;

		//���O�����Z�b�g����
		battle_log->LogReset();

		//���O�𗬂�
		battle_log->addLog("[ �v���C���[�̃^�[�� ]");
	
	}

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

		select_sequence = Sequence::ENEMYACTION;

		//���O�����Z�b�g����
		battle_log->LogReset();

		//���O�𗬂�
		battle_log->addLog("[ �G�̃^�[�� ]");

		//�G�̍s�����s��
		enemy->EnemyAction(battle_log);
		
		//�G���G���I�������炻�̂܂܃v���C���[������ł��邩�̃`�F�b�N���s��
		if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {

			//�v���C���[��Hp��0�ɂȂ�����S�ł̃V�[���ɐ؂�ւ���
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::DEAD);

			PlayerTurn = true;
		}
		//�G���{�X��������{�X�̍U���G�t�F�N�g�𗬂�
		else if(enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

			tnl_sequence_.change(&BattleScene::seqAnimation);

			//�v���C���[��Hp��0�ɂȂ�����S�ł̃V�[���ɐ؂�ւ���
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::DEAD);

			PlayerTurn = true;
		}
		
	}
	//Hp��0�ɂȂ�Ȃ�������^�[����؂�ւ���
	if (PlayerStatus.GetcurentHp() >= 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//��Ԃ�ʏ�ɖ߂��ăR�}���h�o�g�����ēx�s��
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

		tnl_sequence_.change(&BattleScene::seqPlayerAction);
	}

	return false;
}

//�^�[����؂�ւ���V�[�P���X
bool BattleScene::seqChangeTurn(float delta_time)
{
	//Enter�L�[����������V�[�P���X��J�ڂ�����
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		tnl_sequence_.change(&BattleScene::seqEnemyAction);
	}

	return false;
}

//�G�t�F�N�g�𗬂��V�[�P���X
bool BattleScene::seqAnimation(float delta_time)
{

	//�v���C���[���U����I�����Ă�����
	if (GameManager::getGameManager()->getPlayer()->GetPlayerState() == Player::PlayerState::NOMALATTACK) {
		
		if (tnl_sequence_.isStart()) {
			//�X�L���̃G�t�F�N�g����
			nomal_attack->SkillUseAnimation();
		}

		//�v���C���[����̓��͂��󂯕t���Ȃ��悤�ɂ���
		PlayerTurn = false;

		if (tnl_sequence_.getProgressTime() > 1.0f) {

			//�̃G�t�F�N�g�~�߂�
			nomal_attack->SkillAnimationStop();

			//�G������ł��邩�m�F���Ă�������ł�����o�g�����I��������
			if (enemy->ChackDeadEnemy()) {

				//�G�̉摜������
				enemy->DeadEnemyFlag();

				//�o�g�����I��������
				battle_state = BattleState::IDLE;

				//�v���C���[�̏�Ԃ�ҋ@��Ԃɂ�����
				GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

				return false; 

			}

			//�G������ł��Ȃ�������V�[�P���X��؂�ւ���
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}
	}
	else if (GameManager::getGameManager()->getPlayer()->GetPlayerState() == Player::PlayerState::SKILL) {

		if (tnl_sequence_.isStart()) {
			//�X�L���̃G�t�F�N�g����
			GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUseAnimation();

		}

		if (tnl_sequence_.getProgressTime() > 1.0f) {

			//�X�L���̃G�t�F�N�g�~�߂�
			GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationStop();

			//�G������ł��邩�m�F���Ă�������ł�����o�g�����I��������
			if (enemy->ChackDeadEnemy()) {

				//�G�̉摜������
				enemy->DeadEnemyFlag();

				//�o�g�����I��������
				battle_state = BattleState::IDLE;

				//�v���C���[�̏�Ԃ�ҋ@��Ԃɂ�����
				GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

				return false;

			}

			//�v���C���[�̏�Ԃ�ҋ@��Ԃɂ�����
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

			//�V�[�P���X��؂�ւ���
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}


	}
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//�{�X�����X�^�[���_�E���L���X�g�ŌĂяo��
		auto boss_monster = std::dynamic_pointer_cast<BossMonster>(enemy);


		if (tnl_sequence_.isStart()) {

			//�X�L���̃G�t�F�N�g����
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillUseAnimation();

		}

		if (tnl_sequence_.getProgressTime() > 1.0f) {

			//�X�L���̃G�t�F�N�g�~�߂�
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationStop();

			//�v���C���[�̃X�e�[�^�X���擾����
			auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

			//�v���C���[��HP��0�ɂȂ��ċ��Ȃ�������
			if (PlayerStatus.GetcurentHp() > 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//��Ԃ�ʏ�ɖ߂��ăR�}���h�o�g�����ēx�s��
				GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

				tnl_sequence_.change(&BattleScene::seqPlayerAction);

				return false;
			}

		}

	}

	return false;

}

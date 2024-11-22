#include "battleScene.h"
#include"../Object/Actor.h"
#include"../Item/Item.h"
#include"../Menu/BattleLog.h"
#include"../Manager/GameManager.h"
#include"../Manager/UiManager.h"
#include"mapScene.h"
#include"resultScene.h"
#include"../System/Event.h"
#include"../Scene/tittleScene.h"
#include"../Manager/CsvManager.h"


//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

BattleScene::BattleScene(const tnl::Vector3& pos, const int& background, Shared<Enemy> enemy_pointer, const int& inmap_state) : back_ground_(background), map_pos(pos), inmap_state_save(inmap_state)
{
	//���݂̃V�[����BattleScene�ɐݒ�
	curent_scene = SceneState::BATTLE;

	//�o�g���V�[���̏�Ԃ��o�g���ɕύX����
	battle_state = BattleState::BATTLE;

	//�G�̃|�C���^��������
	enemy = enemy_pointer;

	//�o�g���V�[���̏�����
	InitBattleScene();
}

//�o�g�����I�������ۂɓG�̎�ނɉ��y���~�߂���t���O���~������
void BattleScene::BattleEndProcces(const int& enemy_id)
{
	//�T�E���h���~�߂�
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {

		SoundManager::GetSoundManager()->StopSound("sound/BGM/sentou.mp3");
	}
	//�{�X�����X�{�X�̏ꍇ
	else if(enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::ZERAHKIEL)){
		SoundManager::GetSoundManager()->StopSound("sound/BGM/maou_sentou_bgm.mp3");
	}
	//�{�X�����{�X1�̖ڂ̏ꍇ
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::GROVEGUARDIAN)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/tyubosu.mp3");
	}
	//�{�X�����{�X2�̖ڂ̏ꍇ
	else  if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::SOULWARRIOR)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/ordeal_battle.mp3");
	}
	//�{�X�����{�X3�̖ڂ̏ꍇ
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::SHADOWENEMY)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/battle-dark.mp3");
	}
	//�T�u�{�X��1�̖�
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::CORPORAL)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/corporal_battle.mp3");
	}
	//���{�X
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::PIRATE)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/pirate.mp3");
	}
}

//�f�X�g���N�^
BattleScene::~BattleScene()
{
	//�G�ɂ���Ď~�߂鏈����ς���
	BattleEndProcces(enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId());
}

//�o�g���V�[���̏�����
void BattleScene::InitBattleScene()
{
	//�o�g�����O������������
	battle_log = std::make_shared<BattleLog>();

	//�A�C�e��������������
	item_ = std::make_shared<Item>();

	//���j���[�̏�����
	InitMenuWindow();

	//�v���C���[�̏�Ԃ�IDLE�ɂ���(�R�}���h���͎�t�̈�)
	GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

	//����̃^�C�v�Z�b�g����
	SetWeaponType();

	//�ʏ�U���̃|�C���^���Z�b�g����
	nomal_attack = std::make_shared<Nomal_Attack>(weapon_type);

	//�o�g���t���O��؂�ւ���(�o�g���݂̂ł���A�C�e���̈�)
	item_->BattleFlagSwitch();

	//�o�g�����O���Z�b�g����
	item_->SetBattleLog(battle_log);

	//�J�[�\�������Z�b�g����
	GameManager::GetGameManager()->GetInventory()->CursorReset();

	//���݂̃y�[�W����ԍŏ��̃y�[�W�ɂ���
	GameManager::GetGameManager()->GetInventory()->CurentPageReset();

	//�C���f�b�N�X������������
	GameManager::GetGameManager()->GetInventory()->SelectedIndexClear();

	//�o�g���V�[�����Z�b�g����
	enemy->SetBattleScene(this);

	//�v���C���[�̃o�g���V�[�����Z�b�g����
	auto& PlayerPtr = GameManager::GetGameManager()->GetPlayer();
	GameManager::GetGameManager()->GetPlayer()->SetBattleScene(this);

	//�J�����̕ۊǍ��W���Z�b�g����
	GameManager::GetGameManager()->GetCamera()->SavePosition(map_pos);

	//�G�����u�̏ꍇ�A�ʏ�퓬���y�𗬂�
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {
		//�o�g���p��BGM�𗬂�
		SoundManager::GetSoundManager()->Sound_Play("sound/BGM/sentou.mp3", DX_PLAYTYPE_LOOP);

	}
	//�G���{�X�̏ꍇ�_�E���L���X�g���ăG�t�F�N�g�p�̃|�C���^��p�ӂ���
	//�_�E���L���X�g�͏������d�����߈�ԍŏ��ɃL���X�g����
	else {
		boss_monster_ = std::dynamic_pointer_cast<BossMonster>(enemy);
	}
}

//���j���[�̏�����
void BattleScene::InitMenuWindow()
{
	//�ŏ��̍s�������߂�window
	select_action_coment = new MenuWindow::MenuElement_t[]
	{
		{MENU_COMENT_POS_FIRST.x ,MENU_COMENT_POS_FIRST.y ,   "��������" , MenuWindow::Elements::FIRST_ELEMENT},
		{MENU_COMENT_POS_SECOND.x , MENU_COMENT_POS_SECOND.y , "����" ,    MenuWindow::Elements::SECOND_ELEMENT},
		{MENU_COMENT_POS_THIRD.x , MENU_COMENT_POS_THIRD.y ,   "������" ,  MenuWindow::Elements::THERD_ELEMENT}

	};

	//�v�f�̐�
	const int SELECT_ACTION_MENU_ELEMENTS_NUM = 3;

	select_comand_menu = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_action_coment, SELECT_ACTION_MENU_ELEMENTS_NUM);
	select_comand_menu->Open();

	//�U�������߂�E�B���h�E
	select_action_attack = new MenuWindow::MenuElement_t[]
	{
		{MENU_COMENT_POS_FIRST.x , MENU_COMENT_POS_FIRST.y ,  "�U��" ,  MenuWindow::Elements::FIRST_ELEMENT},
		{MENU_COMENT_POS_SECOND.x , MENU_COMENT_POS_SECOND.y, "���Z"   ,MenuWindow::Elements::SECOND_ELEMENT},
		{MENU_COMENT_POS_THIRD.x , MENU_COMENT_POS_THIRD.y ,  "�߂�" ,  MenuWindow::Elements::THERD_ELEMENT}
	};

	//�v�f�̐�
	const int ATTACK_WINDOW_ELEMENTS_NUM = 3;

	attack_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_action_attack, ATTACK_WINDOW_ELEMENTS_NUM);
	attack_window->Open();

	

	//�A�C�e�����g�p�̃E�B���h�E
	select_detail = new MenuWindow::MenuElement_t[]{
		{ MENU_ITEM_USE_POS.x , MENU_ITEM_USE_POS.y ,  "�g��",    MenuWindow::Elements::FIRST_ELEMENT},
		{ MENU_ITEM_CLOSE.x ,   MENU_ITEM_CLOSE.y ,    "����",  MenuWindow::Elements::SECOND_ELEMENT}
	};

	//�v�f�̐�
	const int SELECT_ITEM_USE_WINDOW_ELEMENTS_NUM = 2;

	select_itemuse_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_detail, SELECT_ITEM_USE_WINDOW_ELEMENTS_NUM);
	select_itemuse_window->Open();

	//map�Ɋi�[����
	UIManager::GetUIManager()->addMenu("select_use_window", select_itemuse_window);
}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void BattleScene::Update(float delta_time)
{
	//���ʏ���
	__super::Update(delta_time);

	//�o�g���V�[���̍X�V����(�؂�ւ��Ȃ�)
	BattleSceneUpdate(delta_time);
	
	//�v���C���[��HP��MP�o�[�̍X�V����
	UIManager::GetUIManager()->PlayerStatusBarUpdate(delta_time);

	//�G�̃G�t�F�N�g�̍X�V����
	if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//���b�N���ă|�C���^���g����悤�ɂ���
		if (auto boss_monster = boss_monster_.lock()) {

			//�G�t�F�N�g�̕`��
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationUpdate(delta_time);
		}
	}

	//�G�t�F�N�g�̍X�V����
	nomal_attack->SkillAnimationUpdate(delta_time);

	//�G�t�F�N�g�̔z�񂪖������͏������΂�
	if (!GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {

		//�G�t�F�N�g�̍X�V����
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillAnimationUpdate(delta_time);

	}
}

//�o�g���V�[���̐؂�ւ�
void BattleScene::BattleSceneUpdate(const float delta_time)
{
	//�퓬���s���Ă���Ԃ̓V�[�P���X�𓮂���
	if (battle_state == BattleState::BATTLE) {

		//�V�[�P���X�̍X�V����
		tnl_sequence_.update(delta_time);

		//�v���C���[�̃^�[���̂�
		if (PlayerTurn) {
			//�v���C���[�̍X�V����
			GameManager::GetGameManager()->GetPlayer()->PlayerMoveProcess(delta_time, battle_log, enemy, nomal_attack);
		}

	}
	//�ҋ@��Ԃ��v���C���[�̏�Ԃ�DEAD�������ꍇ�A�����𔼕��ɂ��ă}�b�v�V�[���ɖ߂�
	else if (battle_state == BattleState::BATTLE_END && GameManager::GetGameManager()->GetPlayer()->GetPlayerState() == Player::PlayerState::DEAD
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//�v���C���[�̃|�C���^���擾����
		auto& player = GameManager::GetGameManager()->GetPlayer();

		//�v���C���[�̂����𔼕�����
		player->ReducePlayerMoney(player->GetPlayerMoney() / koni::Numeric::DIVIDER_TWO);

		//�v���C���[��Hp��MAX�ɂ���
		player->GetPlayerStatusSave().SetPlayerCurentHp(player->GetPlayerStatusSave().GetMaxHp());

		//�v���C���[�̃o�t�֘A�����Z�b�g����
		BattleBuffResetProcess();

		//BattleState��ҋ@��Ԃɂ���
		battle_state = BattleState::IDLE;

		//�V�[���}�l�[�W���[�̃C���X�^���X���擾����
		auto mgr = SceneManager::GetSceneManager();

		//�G���G���G�̏ꍇ
		if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {
			mgr->changeScene(new MapScene());
		}
		//�G���{�X�̏ꍇ
		else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
			mgr->changeScene(new InMapScene(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), static_cast<InMapScene::InMapState>(inmap_state_save)));
		}

	}
	//�{�X�̏ꍇ�A�}�b�v���Ő퓬���n�܂�ׁA�퓬�I��������InMapScene�ɖ߂�
	else if (battle_state == BattleState::BATTLE_END && enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId() != static_cast<int>(BossMonster::BossType::ZERAHKIEL)
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		auto mgr = SceneManager::GetSceneManager();

		//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), static_cast<InMapScene::InMapState>(inmap_state_save)));

	}
	//�Ō�̃{�X�̏ꍇ�A�Z�[�u���s���G�s���[�O�𗬂�
	else if (battle_state == BattleState::BATTLE_END && enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId() == static_cast<int>(BossMonster::BossType::ZERAHKIEL)
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		Weak save_event = std::make_shared <Event>();

		//�Z�[�u���s��
		save_event.lock()->SaveProcces(static_cast<int>(InMapScene::InMapState::VILLAGE));

		//�Z�[�u�̃e�L�X�g���Ă΂Ȃ��悤�ɂ���
		UIManager::GetUIManager()->SaveTextFlagChange();

		//BattleState��ҋ@��Ԃɂ���
		battle_state = BattleState::IDLE;

		//�G�s���[�O�𗬂�
		auto mgr = SceneManager::GetSceneManager();

		//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new ResultScene());

	}
	//�ҋ@��Ԃ̏ꍇ��Enter�L�[�̂ݎ�t���A�����ꂽ��V�[����J�ڂ�����
	else if (battle_state == BattleState::BATTLE_END && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//BattleState��ҋ@��Ԃɂ���
		battle_state = BattleState::IDLE;

		//�V�[����J�ڂ�����
		auto mgr = SceneManager::GetSceneManager();
		mgr->changeScene(new MapScene());

	}
}

//------------------------------------------------------------------------------------------------------------------------
//�`��
void BattleScene::Draw()
{
	//�w�i�摜
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, back_ground_, TRUE);

	//�v���C���[�̃X�e�[�^�X�̕`��
	PlayerStatusDraw();

	//�G�̉摜�̕`��
	enemy->Draw();

	//�o�g���E�B���h�E�̕`��
	BattleDraw();

	//�o�g�����O�E�B���h�E�̕`��
	UIManager::GetUIManager()->Menu_Draw("menu_window" , BATTLE_LOG_WINDOW_POS.x, BATTLE_LOG_WINDOW_POS.y, BATTLE_LOG_WINDOW_WIDTH, BATTLE_LOG_WINDOW_HEIGHT);

	//�o�g�����O�̕����̕`��
	battle_log->DrawLogs();

	//���ʕ`��
	__super::Draw();

	//�G�̃G�t�F�N�g�̍X�V����
	if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//���b�N���ă|�C���^���g����悤�ɂ���
		if (auto boss_monster = boss_monster_.lock()) {
			//�G�t�F�N�g�̍X�V����
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationDraw();
		}
	}

	//�v���C���[�̃G�t�F�N�g�̕`��
	nomal_attack->SkillAnimationDraw();

	//�G�t�F�N�g�̔z�񂪖������͏������΂�
	if (GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {
		return;
	}
	else {
		//���Z�̃G�t�F�N�g�̕`��
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillAnimationDraw();
	}
}

//�o�g���E�B���h�E�̕`��
void BattleScene::BattleDraw()
{
	//�ŏ��̍s���̎��̃E�B���h�E
	if (select_action_menu == MenuAction::FIRST_ACTION) {
		//�R�}���h�E�B���h�E�̕\��
		select_comand_menu->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
	}
	//�U���E�B���h�E
	else if (select_action_menu == MenuAction::FIGHT_ACTION) {
		//�U���R�}���h�E�B���h�E�̕\��
		attack_window->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
	}
	//�A�C�e���\���̃E�B���h�E
	else if (select_action_menu == MenuAction::ITEM_ACTION) {
		//�R�}���h�E�B���h�E�̕\��
		select_comand_menu->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		//�A�C�e���E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_WINDOW_POS.x, ITEM_WINDOW_POS.y, ITEM_WINDOW_WIDTH, ITEM_WINDOW_HEIGHT);
		//�A�C�e���̖��O�̕\��
		GameManager::GetGameManager()->GetInventory()->ItemMenu(ITEM_DRAW_POS, ITEM_CURENT_PAGE, CUROURY, ITEMPERPAGE_);
	}
	//�A�C�e�����g�����̃E�B���h�E
	else if (select_action_menu == MenuAction::ITEM_USE_ACTION) {
		//�R�}���h�E�B���h�E�̕\��
		select_comand_menu->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		//�A�C�e���E�B���h�E�̕\��
		UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_WINDOW_POS.x, ITEM_WINDOW_POS.y, ITEM_WINDOW_WIDTH, ITEM_WINDOW_HEIGHT);
		//�A�C�e���̖��O�̕\��
		GameManager::GetGameManager()->GetInventory()->ItemMenu(ITEM_DRAW_POS, ITEM_CURENT_PAGE, CUROURY, ITEMPERPAGE_);
		//�g������߂邩�̑I���E�B���h�E
		select_itemuse_window->All(SELECT_ITEM_USE_WINDOW_POS.x, SELECT_ITEM_USE_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
	}
	//�X�L���E�B���h�E
	else if (select_action_menu == MenuAction::SKILL_ACTION) {
		//�U���̑I���E�B���h�E
		attack_window->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		//�X�L���̕\��
		InventorySkillDraw();
	}
}


//------------------------------------------------------------------------------------------------------------------------
//�A�C�e���֘A


//�A�C�e���̑I������
void BattleScene::ItemSelectProcess()
{
	//�C���f�b�N�X�̑���Ǝ擾
	GameManager::GetGameManager()->GetInventory()->ItemCurourIndex(ITEMPERPAGE_);

	//�A�C�e���̃J�[�\������
	GameManager::GetGameManager()->GetInventory()->CusorMove();

	//�o�b�N�X�y�[�X���������烁�j���[��߂�
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		//���ʉ���炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�s�����ڂ�
		select_action_menu = MenuAction::FIRST_ACTION;
		//���j���[��؂�ւ���
		select_comand_menu->SelectCousourMoveFlagChange();
	}

}

//�A�C�e�����g�����ۂ̏���
void BattleScene::ItemUseProcess()
{
	if (select_itemuse_window->GetSelectNum() == ITEMUSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//���ʉ���炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

		//�A�C�e�����g�������̏���
		//�A�C�e���̌��ʂ��K�p�ł�����
		if (item_->ItemUse(GameManager::GetGameManager()->GetInventory()->GetSelectedItemId())) {
			//�s�����ڂ�
			select_action_menu = MenuAction::FIRST_ACTION;
			//�J�[�\���𓮂���悤�ɂ���
			select_comand_menu->SelectCousourMoveFlagChange();
			//�v���C���[�̃^�[�����I��������
			PlayerTurn = false;
			//�V�[�P���X��؂�ւ���
			tnl_sequence_.immediatelyChange(&BattleScene::seqChangeTurn);
		}
		//�A�C�e�����̌��ʂ��K�p�o���Ȃ�������
		else {
			//�s�����A�C�e���I�����j���[�ɖ߂�
			select_action_menu = MenuAction::ITEM_ACTION;
			//�J�[�\���𓮂���悤�ɂ���
			//select_comand_menu->SelectCousourMoveFlagChange();
		}
	}
	//���邪�����ꂽ��
	else if (select_itemuse_window->GetSelectNum() == ITEMUSEMENUCLOSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		select_action_menu = MenuAction::ITEM_ACTION;
	}
	//�o�b�N�X�y�[�X�������ƍŏ��̑I�����j���[�ɖ߂�
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		select_action_menu = MenuAction::ITEM_ACTION;
	}
}

//�G��|�������̃A�C�e���h���b�v����
void BattleScene::ItemDropProcess(Enemy::EnemyConnection& enemy_)
{
	//�����_���ȃV�[�h�l�𐶐�����ׂ̕ϐ�
	std::random_device rd;

	//�����l�̍Œ�l
	const int RAND_PROBABILITY_MIN = 0;
	//�����l�̍ő�l
	const int RAND_PROBABILITY_MAX = 100;

	//�^�������𐶐�����(�����Z���k�c�C�X�^�[)
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(RAND_PROBABILITY_MIN, RAND_PROBABILITY_MAX);

	//0����100�̈�l�����𐶐�����
	int rand_val = static_cast<int>(dis(gen));

	//�G�̃A�C�e���z����擾����
	auto& EnemyItemDrop = enemy->getItemDropArray();

	//�m�[�}���h���b�v�̊m��
	int nomal_drop = enemy_.GetNomalProbability();

	//���A�h���b�v�̊m��
	int rare_drop = enemy_.GetRareProbability();

	//�v���C���[�̃|�C���^���擾����
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//�C���x���g���̃|�C���^���擾����
	auto& inventory_ = GameManager::GetGameManager()->GetInventory();

	//�h���b�v�A�C�e��
	enum DropItemes {
		NOMAL,
		RARE,
	};

	//���A�h���b�v
	if (rand_val < rare_drop) {

		//���O���Ȃ���
		//�������������ς�����Ȃ�������
		if (inventory_->GetInventoryList().size() != inventory_->GetInventoryMaxSize()) {
			battle_log->AddItemDrop(player->GetPlayerName(), EnemyItemDrop[RARE].GetItemName());
		}
		//�������������ς���������
		else {
			battle_log->AddLog("�������������ς��̈׃A�C�e������肷�鎖���ł��܂���ł���");
		}

		//�G�̃A�C�e�����C���x���g���Ɋi�[����
		inventory_->AddInventory(EnemyItemDrop[1].GetItemId());
	}
	//�ʏ�h���b�v
	else if (rand_val > rare_drop && rand_val < (nomal_drop + rare_drop)) {

		//���O���Ȃ���
		//�������������ς�����Ȃ�������
		if (inventory_->GetInventoryList().size() != inventory_->GetInventoryMaxSize()) {
			battle_log->AddItemDrop(player->GetPlayerName(), EnemyItemDrop[NOMAL].GetItemName());
		}
		//�������������ς���������
		else {
			battle_log->AddLog("�������������ς��̈׃A�C�e������肷�鎖���ł��܂���ł���");
		}
		
		//�G�̃A�C�e�����C���x���g���Ɋi�[����
		inventory_->AddInventory(EnemyItemDrop[NOMAL].GetItemId());
	}
}

//�ϓ������l�����Z�b�g����
void BattleScene::BattleBuffResetProcess()
{
	//�x�[�X�̍U����
	auto player_base_attck = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetBaseAttack();

	//�U����
	auto curent_player_attack = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetAttack();

	//�C���x���g��
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//����𑕔����Ă���ꍇ
	if (!inventory->GetEquipWeaponArray().empty()) {

		for (auto& equip_weqpon : inventory->GetEquipWeaponArray()) {

			//�v���C���[�̍U���͂��ω����Ă�����
			if ((player_base_attck + equip_weqpon.GetItemDamage()) != curent_player_attack) {

			//�������̃X�e�[�^�X�ɖ߂�
			GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerAttack(player_base_attck + equip_weqpon.GetItemDamage());

			}
		}
	}
	//�������Ă��Ȃ���Δ�r���ĕϓ����Ă���Βl��߂�
	else if(player_base_attck != curent_player_attack){

		//�������̃X�e�[�^�X�ɖ߂�
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerAttack(player_base_attck);

	}
	//�ϓ����Ă��Ȃ���Ώ������΂�
	else{
		return;
	}
	
}

//������Z�b�g���ăG�t�F�N�g��K�p������
void BattleScene::SetWeaponType()
{
	//���킪��������Ă�����
	if (!GameManager::GetGameManager()->GetInventory()->GetEquipWeaponArray().empty()) {

		auto it = GameManager::GetGameManager()->GetInventory()->GetEquipWeaponArray().begin();

		//����̃^�C�v���擾����
		weapon_type = (*it).GetItemWeapontype();

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

	//�G�����u�̏ꍇ�f�����ɂ���ē������𒲐�����
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {
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
	}
	//�G���{�X�̏ꍇ������Ȃ�����
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		// ������������ቺ�����鏈��
		probability = 0; 
	}

	// �����̐����m���𔻒肷��
	if (rand() % 100 < probability) {
		// �����ɐ��������ꍇ�̏���
		battle_log->AddLog("���܂������ꂽ");

		//�퓬BGM���~�߂�
		SoundManager::GetSoundManager()->StopSound("sound/BGM/sentou.mp3");
		//������̌��ʉ��𗬂�
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/nigeru.mp3", DX_PLAYTYPE_BACK);
		//���ʂ�ς���
		SoundManager::GetSoundManager()->ChangeSoundVolume(50, "sound/SoundEffect/nigeru.mp3");
		//�o�g�����I��������
		battle_state = BattleState::BATTLE_END;
	}
	else {
		// �����Ɏ��s�����ꍇ�̏���
		battle_log->AddLog("�������Ȃ�����");
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);
	}
}

//����̃{�X��ł̏���
bool BattleScene::CheckRequiredItemEnemyProcess()
{
	//�K�{�A�C�e����ID(���̋�)
	const int REQUIRED_ITEM_ID = 39;

	//�K�{�A�C�e���������Ă���ꍇ���̂܂ܐ퓬���s��
	if (EventManager::GetEventManager()->CheckEventItem(REQUIRED_ITEM_ID , FALSE)) {
		return true;
	}
	//���������Ă��Ȃ����
	else {

		//���O�𗬂�
		battle_log->AddLog("�U�����Ƃ���Ȃ�!?");

		//�^�[�����I��点��
		PlayerTurn = false;

		//�v���C���[��ҋ@��Ԃɂ�����
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::IDLE);

		//�V�[�P���X��؂�ւ���
		tnl_sequence_.immediatelyChange(&BattleScene::seqChangeTurn);

		return false;
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�X�L���֘A

//�X�L�����I�����ꂽ���̏���
void BattleScene::SkillSelectProcess()
{
	//�X�L���̃J�[�\���ړ��������̃C���f�b�N�X����
	GameManager::GetGameManager()->GetInventory()->SkillCurourIndex(ITEMPERPAGE_);

	//���Z�̃J�[�\���ړ�
	GameManager::GetGameManager()->GetInventory()->SkillCousorMove();

	//Enter�L�[�������ꂽ����
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//�X�L���������Ȃ��ꍇ�ɂ͏������s��Ȃ�
		if (GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {
			return;
		}

		//�v���C���[�̏�Ԃ��U���ɕς��čU��
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::SKILL);
	}
	//�o�b�N�X�y�[�X���������烁�j���[��߂�
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		//���ʉ���炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//�s�����ڂ�
		select_action_menu = MenuAction::FIGHT_ACTION;
		//�J�[�\���𓮂���悤�ɂ���
		attack_window->SelectCousourMoveFlagChange();
	}
}

//�X�L���̕`��
void BattleScene::InventorySkillDraw() {

	//UI�}�l�[�W���[���擾����
	auto ui_manager = UIManager::GetUIManager();

	//�E�B���h�E�̕\��
	ui_manager->Menu_Draw("menu_window", SKILL_WINDOW_POS.x, SKILL_WINDOW_POS.y, SKILL_WINDOW_WIDTH, SKILL_WINDOW_HEIGHT);
	
	//�C���x���g�����擾����
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//�C���x���g�����̃X�L����`�悷��
	inventory->InventorySkill(Skill_DRAW_POS, ITEM_CURENT_PAGE , CUROURY, ITEMPERPAGE_);
	
	//�X�L�����󂶂�Ȃ��ꍇ�A�X�L���������`�悷��
	if (!GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {

		//�X�L���E�B���h�E�̍��W
		const tnl::Vector2i SKILL_WINDOW_POS = { 570 , 500 };

		//�X�L��������`�悷��
		inventory->SkillDetailDraw(inventory->GetSkillSelectedIndex(), SKILL_WINDOW_POS , curent_scene , enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId());
	}

}

//�X�L�����g���邩�ǂ����̃`�F�b�N
bool BattleScene::SkillUseMpChack(Player::PlayerStatus& playerStatus)
{
	//�X�L����������Ώ������΂�
	if (GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {
		return false;
	}

	//MP������Ȃ�������
	if (playerStatus.GetCurentMp() < GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->GetSkillConsumeMp()) {
		battle_log->AddLog("Mp������܂���");
		return false;
	}
	else {
		return true;
	}
}

//�X�L�����g�p�����ۂ̏���
void BattleScene::SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_)
{
	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()];

	//�X�L�����g���Ȃ����珈�����΂�
	if (!SkillUseMpChack(playerStatus)) {
		//������x�I��������
		select_action_menu = MenuAction::SKILL_ACTION;
		//�v���C���[�̏�Ԃ�ҋ@��Ԃɂ�����
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

		return;
	}
	//�U���n�̃X�L���̏ꍇ
	if (SkillList->GetSkillType() == AttackType && !GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {

		//�{�X���e�̔Ԑl�̏ꍇ�̂ݕK�{�A�C�e�����K�v�ȈׁA�m�F���͂���
		//false���������Ă����ꍇ�A�������^�[��������
		if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::SHADOWENEMY) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS
			&& !CheckRequiredItemEnemyProcess()) {

			//��Ԃ�߂�
			select_action_menu = MenuAction::FIGHT_ACTION;

			//�J�[�\���̏�Ԃ�ύX����
			attack_window->SelectCousourMoveFlagChange();

			return;
		}

		//�_���[�W��^����
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus, enemy_, battle_log);

		//MP�����炷
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillMpConsume(playerStatus);

		//�G�t�F�N�g�𗬂�
		tnl_sequence_.immediatelyChange(&BattleScene::seqAnimation);
	}
	//�o�t�n�̃X�L���̏ꍇ
	else if (SkillList->GetSkillType() == BuffType && !GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {

		//�o�t���ʂ��v���C���[�ɗ^����
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus , battle_log);

		//MP�����炷
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillMpConsume(playerStatus);

		//�G�t�F�N�g�𗬂�
		tnl_sequence_.immediatelyChange(&BattleScene::seqAnimation);
	}
	

	//�v���C���[����̓��͂��󂯕t���Ȃ��悤�ɂ���
	PlayerTurn = false;
	
	//�X�L���I���E�B���h�E�ɖ߂�
	select_action_menu = MenuAction::FIGHT_ACTION;
	//�J�[�\���𓮂���悤�ɂ���
	attack_window->SelectCousourMoveFlagChange();

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
		if (select_comand_menu->GetSelectNum() == ATTACK_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::FIGHT_ACTION;
			//�J�[�\���𓮂��Ȃ��悤�ɂ���
			select_comand_menu->SelectCousourMoveFlagChange();

		}
		//����
		else if (select_comand_menu->GetSelectNum() == ITEM_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::ITEM_ACTION;
			//�ŏ��̉�ʂ̃J�[�\���𓮂��Ȃ�����
			select_comand_menu->SelectCousourMoveFlagChange();
		}
		//������
		else if (select_comand_menu->GetSelectNum() == FLEE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::FLEE);
		}



		break;

		//�U���I���̎���
	case BattleScene::MenuAction::FIGHT_ACTION:

		//�U���������ꂽ����
		if (attack_window->GetSelectNum() == ATTACK_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�{�X���e�̔Ԑl�̏ꍇ�̂ݕK�{�A�C�e�����K�v�ȈׁA�m�F���͂���
			//false���������Ă����ꍇ�A�������^�[��������
			if(enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::SHADOWENEMY) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS
				&& !CheckRequiredItemEnemyProcess()){
				return;
			}

			//�v���C���[�̏�Ԃ��U���ɕς���
			GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::NOMALATTACK);

			//�V�[�P���X��ς���
			tnl_sequence_.change(&BattleScene::seqAnimation);
		}
		//���Z���I�����ꂽ��
		else if (attack_window->GetSelectNum() == SPECIAL_SKILL_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�����Ă���X�L���̑I����������
			select_action_menu = MenuAction::SKILL_ACTION;

			//�s�������߂�ׂ̑I����ʂ̃J�[�\���𓮂��Ȃ��悤�ɂ���
			attack_window->SelectCousourMoveFlagChange();
		}
		//���邪�I�����ꂽ��
		else if (attack_window->GetSelectNum() == MENU_CLOSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			select_action_menu = MenuAction::FIRST_ACTION;

			//�J�[�\���𓮂���悤�ɂ���
			select_comand_menu->SelectCousourMoveFlagChange();

		}
		
		//�o�b�N�X�y�[�X�������ƍŏ��̑I�����j���[�ɖ߂�
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_action_menu = MenuAction::FIRST_ACTION;
			
			//�J�[�\���𓮂���悤�ɂ���
			select_comand_menu->SelectCousourMoveFlagChange();
		}

		break;

	case BattleScene::MenuAction::ITEM_ACTION:

		//�A�C�e���I������
		ItemSelectProcess();

		//Enter�L�[����������ItemUseMenu�ɕύX����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���艹��炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
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
	auto player_level = player_status.GetLevel();

	//�ő僌�x�����擾����
	auto player_max_level = GameManager::GetGameManager()->GetPlayer()->GetMaxLevel();

	//�v���C���[�̕K�v�Ȍo���l��0�ȉ��ɂȂ������Ƀ��x����������
	//���x��20�ȏ�ł���΃��x���A�b�v���Ȃ�(���㑝�₵�čs���\��)
	
	//���x�����܂�20���ᖳ����Όo���l��t�^����
	if (player_status.GetExpoint() <= 0 && player_level < player_max_level)
	{
		//���x����������
		//���ʉ���炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/levelup.mp3", DX_PLAYTYPE_BACK);

		//���O���o��
		battle_log->AddLog("���x���A�b�v�����I");

		//�V�������x�����擾����
		auto Playerlevel = player_status.GetLevel() + 1;

		//�v���C���[�̃X�e�[�^�X���Z�b�g����
		GameManager::GetGameManager()->GetPlayer()->SetPlayerStatus(Playerlevel);

		//�v���C���[������𑕔����Ă�����
		if (GameManager::GetGameManager()->GetInventory()->GetEquipAttack() != 0) {
			auto playerAttack = player_status.GetAttack();
			player_status.SetPlayerAttack(playerAttack + GameManager::GetGameManager()->GetInventory()->GetEquipAttack());
		}

		//�v���C���[���h��𑕔����Ă�����
		if (GameManager::GetGameManager()->GetInventory()->GetEquipDefance() != 0) {
			auto playerDefance = player_status.GetDefance();
			player_status.SetPlayerDefance(playerDefance + GameManager::GetGameManager()->GetInventory()->GetEquipDefance());
		}

		//�X�L�����Z�b�g����
		GameManager::GetGameManager()->GetPlayer()->SkillSet(battle_log);
	}
	//�K�v�Ȍo���l��0�ɂȂ�Ȃ�������
	//�������̓��x�����ő僌�x����������
	else {
		//��������炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/syouri.mp3", DX_PLAYTYPE_BACK);
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_40_PERCENT, "sound/SoundEffect/syouri.mp3");
	}

}


//�v���C���[��HP��MP�o�[�̏���
void BattleScene::PlayerStatusDraw()
{
	//�E�B���h�E��`�悷��
	UIManager::GetUIManager()->Menu_Draw("menu_window", PLAYER_STATUS_WINDOW_POS.x, PLAYER_STATUS_WINDOW_POS.y, PLAYER_STATUS_WINDOW_WIDTH, PLAYER_STATUS_WINDOW_HEIGHT);
	

	//�X�e�[�^�X��\������
	UIManager::GetUIManager()->PlayerStatusDrawWindow();

}

//------------------------------------------------------------------------------------------------------------------------
//�G�֘A

//�G�̍U��
void BattleScene::EnemyTurnProcess()
{
	//�G�̍s�����s��
	enemy->EnemyAction(battle_log);

	//�G���G���I�������炻�̂܂܃v���C���[������ł��邩�̃`�F�b�N���s��
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {

		//�v���C���[��Hp��0�ɂȂ�����S�ł̃V�[���ɐ؂�ւ���
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::DEAD);

		//��Ԋm�F�̈׃t���O��؂�ւ���
		PlayerTurn = true;

		//�^�[����؂�ւ���
		tnl_sequence_.immediatelyChange(&BattleScene::seqChangeTurn);

	}
	//�G���{�X��������{�X�̍U���G�t�F�N�g�𗬂�
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//�v���C���[��Hp��0�ɂȂ�����S�ł̃V�[���ɐ؂�ւ���
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::DEAD);

		//��Ԋm�F�̈׃t���O��؂�ւ���
		PlayerTurn = true;

		//�A�j���[�V�����𗬂�
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}
}

//�G�̎��S�����A���o
void BattleScene::DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyConnection& enemy_)
{
	
	//���O�𗬂�
	battle_log->AddLog("�G��|�����I");

	//�A�C�e���̃h���b�v����
	ItemDropProcess(enemy_);

	//�����U���͂��ϓ����Ă�������Ƃɖ߂�
	BattleBuffResetProcess();

	//�o�g���t���O��؂�ւ���
	item_->BattleFlagSwitch();

	//�K�v�Ȍo���l���擾����
	auto R_point = playerStatus.GetExpoint();

	//�G�̌o���l���擾����
	auto Enemy_expoint = enemy_.GetEnemyExpoint();

	//�v���C���[�̃|�C���^���擾����
	auto& player = GameManager::GetGameManager()->GetPlayer();

	// �f�[�^����
	playerStatus.SetPlayerExpoint(R_point - Enemy_expoint);

	//�S�[���h����ɓ��ꂽ���̃��O�𗬂�
	std::string log = player->GetPlayerName() + "��" + std::to_string(enemy_.GetEnemyMoney()) + "�S�[���h����ɓ��ꂽ�I";
	battle_log->AddLog(log);
	
	//��������ǉ�����
	GameManager::GetGameManager()->GetPlayer()->AddPlayerMoney(enemy_.GetEnemyMoney());

	//---�G���{�X�������ꍇ�C�x���g���I��������---//

	//���{�X1�̖�
	if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::GROVEGUARDIAN) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		EventManager::GetEventManager()->EnemyEventFlag_1Change();
	}
	//���{�X2�̖�
	else if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::SOULWARRIOR) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		EventManager::GetEventManager()->EnemyEventFlag_2Change();

		//�ł̋ʂ��C���x���g������������Č��̋ʂ��C���x���g���ɉ�����(�d�v�A�C�e��)
		if (item_) {

			//�ł̋ʂ�ID
			const int BALL_OF_DARKNESS_ID = 31;
			//���̋ʂ�ID
			const int BALL_OF_LIGHT_ID = 39;

			//�ł̋ʂ��C���x���g�����炩���������
			GameManager::GetGameManager()->GetInventory()->InventoryItemRemove(BALL_OF_DARKNESS_ID);
			//���̋ʂ��C���x���g���ɒǉ�����
			GameManager::GetGameManager()->GetInventory()->AddInventory(item_->GetItemById(BALL_OF_LIGHT_ID).GetItemId());
			//���O�𗬂�
			battle_log->AddLog(player->GetPlayerName() + "�͌��̋ʂ���ɓ��ꂽ�I");

		}
	}
	//���{�X3�̖�
	else if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::SHADOWENEMY) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		EventManager::GetEventManager()->EnemyEventFlag_3Change();
	}
	//���X�g�{�X
	else if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::ZERAHKIEL) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS
			&& EventManager::GetEventManager()->GetLastBossFlag()) {
		//�t���O��؂�ւ���
		//���{�X�o���̈�
		EventManager::GetEventManager()->LastBossFlagChange();
	}
}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A(��Ƀ^�[������)


//�ŏ��̑f�������r����
bool BattleScene::seqIdle(float delta_time)
{
	//�X�e�[�^�X���擾����
	auto& PlayerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];

	//�V�[�P���X�̏�����
	if (tnl_sequence_.isStart()) {
		//�f�������r����
		//�v���C���[�������ꍇ
		if (PlayerStatus.GetSpeed() >= enemyStatus.GetEnemySpeed()) {
			//�v���C���[�̃V�[�P���X��
			tnl_sequence_.change(&BattleScene::seqPlayerAction);
		}
		//�G�l�~�[�������ꍇ
		else {

			//�G�l�~�[�̃V�[�P���X��
			tnl_sequence_.change(&BattleScene::seqEnemyAction);

		}
	}

	return true;
}

//�v���C���[�̃^�[��
bool BattleScene::seqPlayerAction(float delta_time)
{
	auto& playerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];
	auto enemyHp = enemyStatus.GetEnemyHp();

	if (tnl_sequence_.isStart()) {

		select_sequence = Sequence::PLAYERACTION;

		//���O�����Z�b�g����
		battle_log->LogReset();

		//���O�𗬂�
		battle_log->AddLog("[ �v���C���[�̃^�[�� ]");
	
	}

	//���O�𗬂��Ă���s���ł���悤�ɂ���
	if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_0_2_F) {
		PlayerTurn = true;
	}


	return false;

}

//�G�̃^�[��
bool BattleScene::seqEnemyAction(float delta_time)
{
	auto& PlayerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()]; 

	//�V�[�P���X���n�܂����ŏ��̈��̂ݏ������s��
	if (tnl_sequence_.isStart()) {

		PlayerTurn = false;

		select_sequence = Sequence::ENEMYACTION;

		//���O�����Z�b�g����
		battle_log->LogReset();

		//���O�𗬂�
		battle_log->AddLog("[ �G�̃^�[�� ]");

		//�x�������Ȃ炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/warningsound.mp3", DX_PLAYTYPE_BACK);

		//�{�����[����ς���
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/warningsound.mp3");
		
	}

	//0.5�b�o���Ă���G�̍s�����s��
	if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_0_5_F) {
		//�G�̍s�����s��
		EnemyTurnProcess();
	}
	
	return false;
}

//�^�[����؂�ւ���V�[�P���X
bool BattleScene::seqChangeTurn(float delta_time)
{

	//Enter�L�[����������V�[�P���X��J�ڂ�����
	//�v���C���[�̃^�[���̏ꍇ
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		&& select_sequence == Sequence::PLAYERACTION) {
		tnl_sequence_.change(&BattleScene::seqEnemyAction);
	}
	//�G�l�~�[�^�[���̏ꍇ
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		&& select_sequence == Sequence::ENEMYACTION) {

		//��Ԃ�ʏ�ɖ߂��ăR�}���h�o�g�����ēx�s��
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

		//�I�𐧌�̈׈ꎞ�I�Ƀt���O��؂�ւ���
		PlayerTurn = false;

		tnl_sequence_.change(&BattleScene::seqPlayerAction);
	}

	return false;
}

//�G�t�F�N�g�𗬂��V�[�P���X
bool BattleScene::seqAnimation(float delta_time)
{
	//�v���C���[���U����I�����Ă�����
	if (GameManager::GetGameManager()->GetPlayer()->GetPlayerState() == Player::PlayerState::NOMALATTACK) {

		if (tnl_sequence_.isStart()) {
			//�X�L���̃G�t�F�N�g����
			nomal_attack->SkillUseAnimation();
		}

		//�v���C���[����̓��͂��󂯕t���Ȃ��悤�ɂ���
		PlayerTurn = false;

		if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_1_F) {

			//�̃G�t�F�N�g�~�߂�
			nomal_attack->SkillAnimationStop();

			//�G������ł��邩�m�F���Ă�������ł�����o�g�����I��������
			if (enemy->ChackDeadEnemy()) {

				//�G�̉摜������
				enemy->DeadEnemyFlag();

				//�o�g�����I��������
				battle_state = BattleState::BATTLE_END;

				//�v���C���[�̏�Ԃ�ҋ@��Ԃɂ�����
				GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

				return false;

			}

			//�G������ł��Ȃ�������V�[�P���X��؂�ւ���
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}
	}
	else if (GameManager::GetGameManager()->GetPlayer()->GetPlayerState() == Player::PlayerState::SKILL) {

		if (tnl_sequence_.isStart()) {
			//�X�L���̃G�t�F�N�g����
			GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillUseAnimation();

		}

		if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_1_F) {

			//�X�L���̃G�t�F�N�g�~�߂�
			GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillAnimationStop();

			//�G������ł��邩�m�F���Ă�������ł�����o�g�����I��������
			if (enemy->ChackDeadEnemy()) {

				//�G�̉摜������
				enemy->DeadEnemyFlag();

				//�o�g�����I��������
				battle_state = BattleState::BATTLE_END;

				//�v���C���[�̏�Ԃ�ҋ@��Ԃɂ�����
				GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

				return false;

			}

			//�v���C���[��ҋ@��Ԃɂ�����
			GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::IDLE);

			//�V�[�P���X��؂�ւ���
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}


	}
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//�{�X�����X�^�[�����b�N���ČĂяo��
		auto boss_monster = boss_monster_.lock();

		if (tnl_sequence_.isStart()) {

			//�X�L���̃G�t�F�N�g����
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillUseAnimation();

		}

		if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_1_F) {

			//�X�L���̃G�t�F�N�g�~�߂�
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationStop();

			//�v���C���[�̃X�e�[�^�X���擾����
			auto& PlayerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();

			//�v���C���[��HP��0�ɂȂ��ċ��Ȃ�������
			if (PlayerStatus.GetcurentHp() > 0) {

				tnl_sequence_.change(&BattleScene::seqChangeTurn);

				return false;
			}

		}

	}

	return false;

}

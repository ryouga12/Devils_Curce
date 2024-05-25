#include "battleScene.h"
#include"../Object/Actor.h"


//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

BattleScene::BattleScene(tnl::Vector3 pos, int background, int EnemyID, Shared<Enemy>enemy_pointer) : background_(background), map_pos(pos)
{
	//敵のポインタを代入する
	enemy = enemy_pointer;

	//敵のポインタを生成する
	enemy->InitEnemyPointer(enemy, EnemyID);

	//バトルログを初期化する
	battle_log = std::make_shared<BattleLog>();

	//メニューの初期化
	InitMenuWindow();

	//プレイヤーの状態をIDLEにする(コマンド入力受付の為)
	GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

	//武器のタイプセットする
	SetWeaponType();

	//通常攻撃のポインタをセットする
	nomal_attack = std::make_shared<Nomal_Attack>(weapon_type);

	//バトルフラグを切り替える(バトルのみでつかるアイテムの為)
	GameManager::getGameManager()->getInventory()->BattleFlagSwitch(0);

	//バトルログをセットする
	GameManager::getGameManager()->getInventory()->SetBattleLog(battle_log);

	//インデックスを初期化する
	GameManager::getGameManager()->getInventory()->SelectedIndexClear();

	//バトルシーンをセットする
	/*for (auto actor : actor_list) {
		actor->SetBattleScene(this);
	}*/
	enemy->SetBattleScene(this);


	//プレイヤーのバトルシーンをセットする
	GameManager::getGameManager()->getPlayer()->SetBattleScene(this);

	//バトルシーンの状態をバトルに変更する
	battle_state = BattleState::BATTLE;

}

//デストラクタ
BattleScene::~BattleScene()
{
	auto& enemyArray = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];

	//サウンドを消去する
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/levelup.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/BGM/sentou.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/dageki_2.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/nigeru.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/syouri.mp3");

	//画像を消去する
	ResourceManager::getResourceManager()->deleteGraphEx(enemyArray.getEnemyGhdl().c_str());
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/haikei/battle_field_01.jpg");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/haikei/battle_field_02.jpg");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/haikei/battle_field_03.jpg");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/cur_sannkaku2.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/mpbar.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/hpbar.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/Player/Player_Icon_sentou.png");

}

//メニューの初期化
void BattleScene::InitMenuWindow()
{
	//最初の行動を決めるwindow
	select_action_coment = new MenuWindow::MenuElement_t[]
	{
		{100 , 550 , "たたかう" , 0},
		{100 , 600 , "道具" , 1},
		{100 , 650 , "逃げる" , 2}

	};

	select_comand_menu = std::make_shared<MenuWindow>(50, 510, 250, 200, "graphics/WindowBase_01.png", select_action_coment, 3, 1);
	select_comand_menu->Open();

	//攻撃を決めるウィンドウ
	select_action_attack = new MenuWindow::MenuElement_t[]
	{
		{100 , 550 ,"攻撃" , 0},
		{100 , 600, "特技" , 1},
		{100 , 650 ,"戻る" , 2}
	};

	attack_window = std::make_shared<MenuWindow>(50, 510, 250, 200, "graphics/WindowBase_01.png", select_action_attack, 3, 1);
	attack_window->Open();

	//アイテムを使用のウィンドウ
	select_detail = new MenuWindow::MenuElement_t[]{
		{ 600 , 550 , "使う", 0},
		{ 600 , 600 , "閉じる", 1}
	};

	select_itemuse_window = std::make_shared<MenuWindow>(550, 510, 250, 200, "graphics/WindowBase_01.png", select_detail, 2, 1);
	select_itemuse_window->Open();

	//バトルログのウィンドウ
	battle_log_window = std::make_shared<Menu>(670, 30, 600, 180, "graphics/WindowBase_01.png");
	//プレイヤーのウィンドウ
	playerStatus_window = std::make_shared<Menu>(50, 50, 250, 240, "graphics/WindowBase_01.png");
	//アイテムのウィンドウ
	item_window = std::make_shared<Menu>(300, 510, 250, 200, "graphics/WindowBase_01.png");
	//スキルウィンドウ
	skill_menu_window = std::make_shared<Menu>(300, 510, 250, 200, "graphics/WindowBase_01.png");

}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void BattleScene::Update(float delta_time)
{
	//戦闘を行っている間はシーケンスを動かす
	if (battle_state == BattleState::BATTLE) {
		//シーケンスの更新処理
		tnl_sequence_.update(delta_time);

		//プレイヤーのターンのみ
		if (PlayerTurn) {
			//プレイヤーの更新処理
			GameManager::getGameManager()->getPlayer()->PlayerMoveProcess(delta_time, battle_log, enemy, nomal_attack, map_pos);
		}

	}
	//待機状態かつプレイヤーの状態がDEADだった場合、全滅シーンに切り替える
	else if (battle_state == BattleState::IDLE && GameManager::getGameManager()->getPlayer()->GetPlayerState() == Player::PlayerState::DEAD) {
		//時間を待ってから遷移させる
		if (GameManager::getGameManager()->TimeCount(delta_time, annihilation_Time)) {
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new ResultScene());
		}
	}
	//待機状態の場合はEnterキーのみ受付し、押されたらシーンを遷移させる
	else if (battle_state == BattleState::IDLE) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new MapScene());
		}
	}

	//アニメーションの更新処理
	nomal_attack->SkillAnimationUpdate(delta_time);
	//アニメーション配列が無い時は処理を飛ばす
	if (GameManager::getGameManager()->getInventory()->getSkillNum() == 0) {
		return;
	}
	//アニメーションの更新処理
	GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationUpdate(delta_time);

}

//------------------------------------------------------------------------------------------------------------------------
//描画
void BattleScene::Draw()
{
	//背景画像
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, background_, TRUE);

	//バトルログのウィンドウの描画
	battle_log_window->Menu_draw();

	//バトルウィンドウの描画
	BattleDraw();

	//バトルログの文字の描画
	battle_log->drawLogs();

	//プレイヤーのステータスの描画
	PlayerStatusDraw();

	/*DrawStringEx(10, 10, -1, "バトル画面");*/

	//敵の画像の描画
	enemy->Draw();
	
	//アニメーションの描画
	nomal_attack->SkillAnimationDraw();

	DrawStringEx(300, 250, -1, "プレイヤーの状態 : [%d]" , GameManager::getGameManager()->getPlayer()->GetPlayerState());
	DrawStringEx(300, 300, -1, "PlayerTurn : [%d]" ,PlayerTurn);

	//アニメーション配列が無い時は処理を飛ばす
	if (GameManager::getGameManager()->getInventory()->getSkillNum() == 0) {
		return;
	}
	else {
		//特技のアニメーションの描画
		GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationDraw();
	}

}



//バトルウィンドウの描画
void BattleScene::BattleDraw()
{
	//最初の行動の時のウィンドウ
	if (select_action_menu == MenuAction::first_action) {
		select_comand_menu->All();
	}
	//攻撃ウィンドウ
	else if (select_action_menu == MenuAction::fight_action) {
		attack_window->All();
	}
	//アイテム表示のウィンドウ
	else if (select_action_menu == MenuAction::item_action) {
		select_comand_menu->All();
		item_window->Menu_draw();
		GameManager::getGameManager()->getInventory()->ItemMenu(item_draw_pos, item_curent_page , curourY, itemPerPage);
	}
	//アイテムを使う時のウィンドウ
	else if (select_action_menu == MenuAction::item_use_action) {
		select_comand_menu->All();
		item_window->Menu_draw();
		GameManager::getGameManager()->getInventory()->ItemMenu(item_draw_pos, item_curent_page , curourY, itemPerPage);
		select_itemuse_window->All();
	}
	else if (select_action_menu == MenuAction::skill_action) {
		attack_window->All();
		skill_menu_window->Menu_draw();
		InventorySkillDraw();
	}
}

//------------------------------------------------------------------------------------------------------------------------
//アイテム関連


//アイテムの選択処理
void BattleScene::ItemSelectProcess()
{
	//インデックスの操作と取得
	GameManager::getGameManager()->getInventory()->ItemCurourIndex(itemPerPage);

	//アイテムのカーソル処理
	GameManager::getGameManager()->getInventory()->CusorMove();

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//行動を移す
		select_action_menu = MenuAction::first_action;
		//メニューを切り替える
		select_comand_menu->SetSelectCousourMove();
	}

}

//アイテムを使った際の処理
void BattleScene::ItemUseProcess()
{
	if (select_itemuse_window->getSelectNum() == ItemUse && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//アイテムを使った時の処理
		GameManager::getGameManager()->getInventory()->InventoryItemUse(GameManager::getGameManager()->getInventory()->GetSelectedItemId());
		//行動を移す
		select_action_menu = MenuAction::first_action;
		//カーソルを動けるようにする
		select_comand_menu->SetSelectCousourMove();
		//シーケンスを切り替える
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
	}
	//閉じるが押されたら
	else if (select_itemuse_window->getSelectNum() == ItemUseMenuClose && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		select_action_menu = MenuAction::item_action;
	}
}

//敵を倒した時のアイテムドロップ処理
void BattleScene::ItemDropProcess()
{
	//ランダムなシード値を生成する為の変数
	std::random_device rd;

	//疑似乱数を生成する(メルセンヌツイスター)
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 100);

	//0から100の一様乱数を生成する
	int rand_val = dis(gen);

	//敵のアイテム配列を取得する
	auto& EnemyItemDrop = enemy->getItemDropArray();

	//レアドロップ
	if (rand_val < RareDrop ) {
		//敵のアイテムをインベントリに格納する
		GameManager::getGameManager()->getInventory()->AddInventory(EnemyItemDrop[1].getItemId());

		//ログをながす
		//持ち物がいっぱいじゃなかったら
		if (GameManager::getGameManager()->getInventory()->GetInventoryList().size() != 20) {
			battle_log->addItemDrop("プレイヤー", EnemyItemDrop[1].getItemName());
		}
		//持ち物がいっぱいだったら
		else {
			battle_log->addLog("持ち物がいっぱいの為アイテムを入手する事ができませんでした");
		}
	}
	//通常ドロップ
	else if (rand_val > RareDrop && rand_val < (NomalDrop + RareDrop)) {
		//敵のアイテムをインベントリに格納する
		GameManager::getGameManager()->getInventory()->AddInventory(EnemyItemDrop[0].getItemId());

		//ログをながす
		//持ち物がいっぱいじゃなかったら
		if (GameManager::getGameManager()->getInventory()->GetInventoryList().size() != 20) {
			battle_log->addItemDrop("プレイヤー", EnemyItemDrop[0].getItemName());
		}
		//持ち物がいっぱいだったら
		else {
			battle_log->addLog("持ち物がいっぱいの為アイテムを入手する事ができませんでした");
		}
	}
}

//一時的に上がった攻撃力などのバフをリセットする
void BattleScene::BattleBuffResetProcess()
{
	//攻撃力などが上がっていた場合それをリセットする
	if (GameManager::getGameManager()->getInventory()->getAttackAmount() != 0) {

		int AttackAmoument = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().getAttack() - GameManager::getGameManager()->getInventory()->getAttackAmount();
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerAttack(AttackAmoument);
	}
	//上がって居なければ処理を飛ばす
	else {
		return;
	}
}

//武器をセットする
void BattleScene::SetWeaponType()
{
	//武器が装備されていたら
	if (!GameManager::getGameManager()->getInventory()->getEquipArray().empty()) {

		auto it = GameManager::getGameManager()->getInventory()->getEquipArray().begin();

		//武器のタイプを取得する
		weapon_type = (*it).getItemWeapontype();

	}
	//装備されていなかったら0にする
	else {
		weapon_type = 0;
	}
}

//逃げる際の処理
void BattleScene::FleeProcess(Player::PlayerStatus& playerStatus , Enemy::EnemyStatus& enemyStatus , float delta_time)
{
	//プレイヤーと敵の素早さを取得する
	auto playerSpeed = playerStatus.getSpeed();
	auto enemySpeed = enemyStatus.getEnemySpeed();

	// プレイヤーの素早さが敵よりも速い場合、逃走成功率を高める
	if (playerSpeed > enemySpeed) {
		// 逃走成功率を上昇させる処理
		probability += 20; // 20%のボーナスを与える
	}
	// 敵の素早さがプレイヤーよりも速い場合、逃走成功率を低下させる
	else if (playerSpeed < enemySpeed) {
		// 逃走成功率を低下させる処理
		probability -= 20; // 20%のペナルティを与える
	}

	// 逃走の成功確率を判定する
	if (rand() % 100 < probability) {
		// 逃走に成功した場合の処理
		battle_log->addLog("うまく逃げれた");

		//フラグを立てる
		flee_flag = true;
		//戦闘BGMを止める
		SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
		//逃げるの効果音を流す
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/nigeru.mp3", DX_PLAYTYPE_BACK);
		//音量を変える
		SoundManager::getSoundManager()->ChangeSoundVolume(50, "sound/SoundEffect/nigeru.mp3");
		//バトルを終了させる
		battle_state = BattleState::IDLE;
	}
	else {
		// 逃走に失敗した場合の処理
		battle_log->addLog("逃げられなかった");
		flee_flag = false;
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);
	}
}

//------------------------------------------------------------------------------------------------------------------------
//スキル関連

//スキルが選択された時の処理
void BattleScene::SkillSelectProcess()
{
	//スキルのカーソル移動処理時のインデックス操作
	GameManager::getGameManager()->getInventory()->SkillCurourIndex();

	//特技のカーソル移動
	GameManager::getGameManager()->getInventory()->SkillCousorMove();

	//Enterキーを押された時に
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//スキルが何もない場合には処理を行わない
		if (GameManager::getGameManager()->getInventory()->getSkillList().empty()) {
			return;
		}
		//プレイヤーの状態を攻撃に変えて攻撃
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::SKILL);
	}
	//左を押したらメニューを戻す
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//行動を移す
		select_action_menu = MenuAction::fight_action;
		//カーソルを動けなくする
		attack_window->SetSelectCousourMove();
		//行動を決める為の選択画面のカーソルを動けないようにする
		select_comand_menu->SetSelectCousourMove();
	}
}

//スキルの描画
void BattleScene::InventorySkillDraw() {

	//スキルが0の時は描画しない
	if (GameManager::getGameManager()->getInventory()->getSkillNum() > 0) {

		// スキルインベントリのスキルを2行ずつ表示する
		for (int i = 0; i < itemPerPage; ++i) {
			int skillIndex = SkillCurentPage * itemPerPage + i;
			if (skillIndex >= GameManager::getGameManager()->getInventory()->getSkillList().size()) {
				break; // スキルインベントリの範囲外だったら終了する
			}

			//スキル配列を取得する
			auto& skillDataPtr = GameManager::getGameManager()->getInventory()->getSkillList()[skillIndex];

			// スキルの描画
			DrawStringEx(350, StartY + i * lineHeight, Color_White, "%s", skillDataPtr->getName().c_str());

			// 選択されたスキルのインデックスに基づいてカーソルを描画
			if (skillIndex == GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()) {
				// カーソルを描画する位置を計算
				int cursorY = StartY + i * lineHeight;
				ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", 330, cursorY, cursourSize, 0, true);
			}
		}
	}
}

//スキルが使えるかどうかのチェック
bool BattleScene::SkillUseMpChack(Player::PlayerStatus& playerStatus)
{
	//スキルが無ければ処理を飛ばす
	if (GameManager::getGameManager()->getInventory()->getSkillList().empty()) {
		return false;
	}

	//MPが足りなかったら
	if (playerStatus.getCurentMp() < GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getSkillConsumeMp()) {
		battle_log->addLog("Mpが足りません");
		return false;
	}
	GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillMpConsume(playerStatus);
	return true;
}

//スキルを使用した際の処理
void BattleScene::SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	//スキルが使えなったら処理を飛ばす
	if (!SkillUseMpChack(playerStatus)) {
		select_action_menu = MenuAction::fight_action;
		return;
	}
	//攻撃系のスキルの場合
	if (GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getSkillType() == 0 && !GameManager::getGameManager()->getInventory()->getSkillList().empty()) {

		auto skillList = GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()];

		//ダメージを与える
		GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus, enemyStatus_, battle_log);

		//エフェクトを流す
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}
	//バフ系のスキルの場合
	else if (GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getSkillType() == 1) {
		Skill_Buff_Flag = true;

		//バフ効果をプレイヤーに与える
		GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus , battle_log);
	}
	//アイテム系のスキル
	else {
		//ダメージを与える
		GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(enemyStatus_, battle_log);

		//エフェクトを流す
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}

	//プレイヤーからの入力を受け付けないようにする
	PlayerTurn = false;
	
	//スキル選択ウィンドウに戻す
	select_action_menu = MenuAction::fight_action;
	//カーソルを動けるようにする
	attack_window->SetSelectCousourMove();

}

//------------------------------------------------------------------------------------------------------------------------
//メニュー関連 


//メニュー毎の処理
void BattleScene::MenuUpdate(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerAttack = playerStatus.getAttack();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyHp = enemyStatus_.getEnemyHp();

	switch (select_action_menu)
	{
		//一番最初のメニューの時の処理
	case BattleScene::MenuAction::first_action:

		//たたかう
		if (select_comand_menu->getSelectNum() == Attack && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::fight_action;

		}
		//道具
		else if (select_comand_menu->getSelectNum() == Item && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::item_action;
			//最初の画面のカーソルを動けなくする
			select_comand_menu->SetSelectCousourMove();
		}
		//逃げる
		else if (select_comand_menu->getSelectNum() == Flee && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::FLEE);
		}
		break;

		//攻撃選択の時に
	case BattleScene::MenuAction::fight_action:

		//攻撃が押された時に
		if (attack_window->getSelectNum() == Attack && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//プレイヤーの状態を攻撃に変える
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::NOMALATTACK);

			//シーケンスを変える
			tnl_sequence_.change(&BattleScene::seqAnimation);
		}
		//特技が選択されたら
		else if (attack_window->getSelectNum() == SpecialSkill && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//持っているスキルの選択をさせる
			select_action_menu = MenuAction::skill_action;

			//行動を決める為の選択画面のカーソルを動けないようにする
			select_comand_menu->SetSelectCousourMove();
		}
		//閉じるが選択されたら
		else if (attack_window->getSelectNum() == MenuClose && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			select_action_menu = MenuAction::first_action;

		}

		break;

	case BattleScene::MenuAction::item_action:

		//アイテム選択処理
		ItemSelectProcess();

		//Enterキーを押したらItemUseMenuに変更する
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::item_use_action;
		}

		break;

		//使うが選択されたら
	case MenuAction::item_use_action:

		//アイテムを使った際の処理
		ItemUseProcess();

		break;

		//スキル処理
	case MenuAction::skill_action:

		//スキル選択時の処理
		SkillSelectProcess();


		break;

	default:
		break;
	}

}

//------------------------------------------------------------------------------------------------------------------------
//プレイヤー関連

//アニメーション使用時のターンチェンジ
void BattleScene::AnimationTurnChange(int enemyhp , float delta_time)
{
	//バフ系のスキルを使った場合はターンを入れ替える
	if (Skill_Buff_Flag) {
		//ターンを切り替える
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
	}
}

//レベルアップ判定 & 処理
void BattleScene::ChackPlayerLevelUp(Player::PlayerStatus& player_status) {

	//レベルを取得する
	auto Playerlevel = player_status.getLevel();

	//レベル10以上であればレベルアップしない(今後増やして行く予定)
	if (Playerlevel >= 10) {
		return;
	}

	//プレイヤーの必要な経験値が0以下になった時にレベルをあげる
	if (player_status.getExpoint() <= 0)
	{
		//レベルをあげる
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/levelup.mp3", DX_PLAYTYPE_BACK);

		//ログを出す
		battle_log->addLog("レベルアップした！");

		//新しいレベルを取得する
		auto Playerlevel = player_status.getLevel() + 1;

		//プレイヤーのステータスをセットする
		GameManager::getGameManager()->getPlayer()->SetPlayerStatus(Playerlevel);

		//プレイヤーが武器を装備していたら
		if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack) != 0) {
			auto playerAttack = player_status.getAttack();
			player_status.SetPlayerAttack(playerAttack + GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack));
		}

		//プレイヤーが防具を装備していたら
		if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance) != 0) {
			auto playerDefance = player_status.getDefance();
			player_status.SetPlayerDefance(playerDefance + GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance));
		}

		//スキルをセットする
		GameManager::getGameManager()->getInventory()->SkillSet();
	}
	//必要な経験値が0にならなかったら
	else {
		//勝利音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/syouri.mp3", DX_PLAYTYPE_BACK);
		SoundManager::getSoundManager()->ChangeSoundVolume(40, "sound/SoundEffect/syouri.mp3");
	}

}


//プレイヤーのHPとMPバーの処理
void BattleScene::PlayerStatusDraw()
{
	//プレイヤーのステータスを取得する
	auto plyerstatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

	//ウィンドウを描画する
	playerStatus_window->Menu_draw();

	//プレイヤーの画像
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/Player/Player_Icon_sentou.png" , 90 , 120  ,1.3f, 0 , true);

	//hpバーの画像
	int hpber_hdl =  ResourceManager::getResourceManager()->LoadGraphEX("graphics/hpbar.png");
	//mpバーの画像
	int mpber_hdl =  ResourceManager::getResourceManager()->LoadGraphEX("graphics/mpbar.png");
	//hpバー最大の長さ
	int maxHpBar = 200;
	//mpバーの最大の長さ
	int maxMpBer = 200;

	// HPの割合を計算する
	float hp_ratio = plyerstatus.getcurentHp() / plyerstatus.getMaxHp();
	// HPバーの長さを計算する
	float hpbar = static_cast<int>(maxHpBar * hp_ratio);

	// MPの割合を計算する
	float mp_ratio = plyerstatus.getCurentMp() / plyerstatus.getMaxMp();
	// MPバーの長さを計算する
	float mpbar = static_cast<int>(maxMpBer * mp_ratio);

	DrawExtendGraph(75, 155, (75 + hpbar), 185, hpber_hdl, true);
	DrawExtendGraph(75, 215, (75 + mpbar), 245, mpber_hdl, true);

	//プレイヤーのHPを表示する
	DrawStringEx(playerStatus_window->menu_x + 25, playerStatus_window->menu_y + 20, -1, "Player");
	DrawStringEx(120, 110, -1, " Lv %d" , plyerstatus.getLevel());
	DrawStringEx(playerStatus_window->menu_x + 25, playerStatus_window->menu_y + 140, -1, " Hp : %d / %d", static_cast<int>(plyerstatus.getcurentHp()),static_cast<int>(plyerstatus.getMaxHp()));
	DrawStringEx(playerStatus_window->menu_x + 25, playerStatus_window->menu_y + 200, -1, " Mp : %d / %d", static_cast<int>(plyerstatus.getCurentMp()), static_cast<int>(plyerstatus.getMaxMp()));



}

//------------------------------------------------------------------------------------------------------------------------
//敵関連


//敵の死亡処理、演出
void BattleScene::DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyStatus& enemy_status)
{
	
	//ログを流す
	battle_log->addLog("敵を倒した！");

	//アイテムのドロップ処理
	ItemDropProcess();

	//もし一時的に攻撃力が上がっていたらそれをリセットする
	BattleBuffResetProcess();

	//バトルフラグを切り替える
	GameManager::getGameManager()->getInventory()->BattleFlagSwitch(1);

	//必要な経験値を取得する
	auto R_point = playerStatus.getExpoint();

	//敵の経験値を取得する
	auto Enemy_expoint = enemy_status.getEnemyExpoint();

	// データ処理
	playerStatus.SetPlayerExpoint(R_point - Enemy_expoint);

	//ゴールドを手に入れた時のログを流す
	battle_log->addMoney("プレイヤー", enemy_status.getEnemyMoney());
	
	//所持金を追加する
	GameManager::getGameManager()->getPlayer()->AddPlayerMoney(enemy_status.getEnemyMoney());
}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連(主にターン制御)

//最初の素早さを比較する
bool BattleScene::seqIdle(float delta_time)
{
	//ステータスを取得する
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];

	//シーケンスの初期化
	if (tnl_sequence_.isStart()) {
		//素早さを比較する
		//プレイヤーが早い場合
		if (PlayerStatus.getSpeed() >= enemyStatus.getEnemySpeed()) {
			//プレイヤーのシーケンスへ
			tnl_sequence_.change(&BattleScene::seqPlayerAction);
			PlayerTurn = true;
		}
		//エネミーが早い場合
		else {

			//エネミーのシーケンスへ
			tnl_sequence_.change(&BattleScene::seqEnemyAction);

		}
	}

	//デバック
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new ResultScene());
	}

	return true;
}

//プレイヤーのターン
bool BattleScene::seqPlayerAction(float delta_time)
{
	auto& playerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];
	auto enemyHp = enemyStatus.getEnemyHp();

	if (tnl_sequence_.isStart()) {

		PlayerTurn = true;

		select_sequence = Sequence::PlayerAction;

		//ログをリセットする
		battle_log->LogReset();

		//ログを流す
		battle_log->addLog("[ プレイヤーのターン ]");
	
	}

	//アニメーションを使用時にアニメーションを流してからターンをチェンジさせる
	AnimationTurnChange(enemyHp, delta_time);

	return false;

}

//敵のターン
bool BattleScene::seqEnemyAction(float delta_time)
{
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];

	//シーケンスが始まった最初の一回のみ処理を行う
	if (tnl_sequence_.isStart()) {

		PlayerTurn = false;

		select_sequence = Sequence::EnemyAction;

		//ログをリセットする
		battle_log->LogReset();

		//ログを流す
		battle_log->addLog("[ 敵のターン ]");

		//敵の行動を行う
		enemy->EnemyAction(battle_log);

		//SEを流す
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/dageki_2.mp3", DX_PLAYTYPE_BACK);

		//ボリュームを変える
		SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/dageki_2.mp3");

		//プレイヤーのHpが0になったら全滅のシーンに切り替える
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::DEAD);
		PlayerTurn = true;
		
	}
	//Hpが0にならなかったらターンを切り替える
	if (PlayerStatus.getcurentHp() >= 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//状態を通常に戻してコマンドバトルを再度行う
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

		tnl_sequence_.change(&BattleScene::seqPlayerAction);
		select_sequence = Sequence::PlayerAction;
	}

	return false;
}

//ターンを切り替えるシーケンス
bool BattleScene::seqChangeTurn(float delta_time)
{
	//Enterキーを押したらシーケンスを遷移させる
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		tnl_sequence_.change(&BattleScene::seqEnemyAction);
		select_sequence = Sequence::EnemyAction;
	}

	return false;
}

//アニメーションを流すシーケンス
bool BattleScene::seqAnimation(float delta_time)
{
	//プレイヤーが攻撃を選択していたら
	if (GameManager::getGameManager()->getPlayer()->GetPlayerState() == Player::PlayerState::NOMALATTACK) {
		
		if (tnl_sequence_.isStart()) {
			//スキルのアニメーションを流す
			nomal_attack->SkillUseAnimation();
		}

		//プレイヤーからの入力を受け付けないようにする
		PlayerTurn = false;

		if (tnl_sequence_.getProgressTime() > 1.0f) {

			//のアニメーションを止める
			nomal_attack->SkillAnimationStop();

			//敵が死んでいるか確認してもし死んでいたらバトルを終了させる
			if (enemy->ChackDeadEnemy()) {

				//敵の画像を消す
				enemy->DeadEnemyFlag();

				//バトルを終了させる
				battle_state = BattleState::IDLE;

				//プレイヤーの状態を待機状態にさせる
				GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

				return false; 

			}

			//敵が死んでいなかったらシーケンスを切り替える
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}
	}
	else if (GameManager::getGameManager()->getPlayer()->GetPlayerState() == Player::PlayerState::SKILL) {

		if (tnl_sequence_.isStart()) {
			//スキルのアニメーションを流す
			GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUseAnimation();

		}

		if (tnl_sequence_.getProgressTime() > 1.0f) {

			//スキルのアニメーションを止める
			GameManager::getGameManager()->getInventory()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationStop();

			//敵が死んでいるか確認してもし死んでいたらバトルを終了させる
			if (enemy->ChackDeadEnemy()) {

				//敵の画像を消す
				enemy->DeadEnemyFlag();

				//バトルを終了させる
				battle_state = BattleState::IDLE;

				//プレイヤーの状態を待機状態にさせる
				GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

				return false;

			}
			//シーケンスを切り替える
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}


	}

	return false;

}

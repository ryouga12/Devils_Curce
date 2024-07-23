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
//初期化　&  解放

BattleScene::BattleScene(tnl::Vector3 pos, int background, Shared<Enemy> enemy_pointer, int inmap_state) : back_ground_(background), map_pos(pos), inmap_state_save(inmap_state)
{
	//現在のシーンをBattleSceneに設定
	curent_scene = SceneState::BATTLE;

	//バトルシーンの状態をバトルに変更する
	battle_state = BattleState::BATTLE;

	//敵のポインタを代入する
	enemy = enemy_pointer;

	//バトルログを初期化する
	battle_log = std::make_shared<BattleLog>();

	//アイテムを初期化する
	item_ = std::make_shared<Item>();

	//メニューの初期化
	InitMenuWindow();

	//プレイヤーの状態をIDLEにする(コマンド入力受付の為)
	GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

	//武器のタイプセットする
	SetWeaponType();

	//通常攻撃のポインタをセットする
	nomal_attack = std::make_shared<Nomal_Attack>(weapon_type);

	//バトルフラグを切り替える(バトルのみでつかるアイテムの為)
	item_->BattleFlagSwitch();

	//バトルログをセットする
	item_->SetBattleLog(battle_log);

	//カーソルをリセットする
	GameManager::getGameManager()->getInventory()->CursorReset();

	//インデックスを初期化する
	GameManager::getGameManager()->getInventory()->SelectedIndexClear();

	/*for (auto actor : actor_list) {
		actor->SetBattleScene(this);
	}*/

	//バトルシーンをセットする
	enemy->SetBattleScene(this);

	//プレイヤーのバトルシーンをセットする
	auto& PlayerPtr = GameManager::getGameManager()->getPlayer();
	GameManager::getGameManager()->getPlayer()->SetBattleScene(this);

	//カメラの保管座標をセットする
	GameManager::getGameManager()->getCamera()->SavePosition(map_pos);

}

//デストラクタ
BattleScene::~BattleScene()
{
	//サウンドを止める
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {

		SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
	}
	else {
		SoundManager::getSoundManager()->StopSound("sound/BGM/maou_sentou_bgm.mp3");
	}
}

//メニューの初期化
void BattleScene::InitMenuWindow()
{
	//最初の行動を決めるwindow
	select_action_coment = new MenuWindow::MenuElement_t[]
	{
		{MENU_COMENT_POS_FIRST.x ,MENU_COMENT_POS_FIRST.y , "たたかう" , 0},
		{MENU_COMENT_POS_SECOND.x , MENU_COMENT_POS_SECOND.y , "道具" , 1},
		{MENU_COMENT_POS_THIRD.x , MENU_COMENT_POS_THIRD.y , "逃げる" , 2}

	};

	select_comand_menu = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_action_coment, 3, 1);
	select_comand_menu->Open();

	//攻撃を決めるウィンドウ
	select_action_attack = new MenuWindow::MenuElement_t[]
	{
		{MENU_COMENT_POS_FIRST.x , MENU_COMENT_POS_FIRST.y ,"攻撃" , 0},
		{MENU_COMENT_POS_SECOND.x , MENU_COMENT_POS_SECOND.y, "特技" , 1},
		{MENU_COMENT_POS_THIRD.x , MENU_COMENT_POS_THIRD.y ,"戻る" , 2}
	};

	attack_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_action_attack, 3, 1);
	attack_window->Open();

	//アイテムを使用のウィンドウ
	select_detail = new MenuWindow::MenuElement_t[]{
		{ MENU_ITEM_USE_POS.x , MENU_ITEM_USE_POS.y , "使う", 0},
		{ MENU_ITEM_CLOSE.x , MENU_ITEM_CLOSE.y , "閉じる", 1}
	};

	select_itemuse_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_detail, 2, 1);
	select_itemuse_window->Open();

	//mapに格納する
	UIManager::getUIManager()->addMenu("select_use_window", select_itemuse_window);

	//ウィンドウ
	window_log = UIManager::getUIManager()->getMenu("menu_window");
}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void BattleScene::Update(float delta_time)
{
	//共通処理
	__super::Update(delta_time);

	//カメラ処理
	GameManager::getGameManager()->getCamera()->update();

	//戦闘を行っている間はシーケンスを動かす
	if (battle_state == BattleState::BATTLE) {
		//シーケンスの更新処理
		tnl_sequence_.update(delta_time);

		//プレイヤーのターンのみ
		if (PlayerTurn) {
			//プレイヤーの更新処理
			GameManager::getGameManager()->getPlayer()->PlayerMoveProcess(delta_time, battle_log, enemy, nomal_attack);
		}

	}
	//待機状態かつプレイヤーの状態がDEADだった場合、全滅シーンに切り替える
	else if (battle_state == BattleState::IDLE && GameManager::getGameManager()->getPlayer()->GetPlayerState() == Player::PlayerState::DEAD) {
		//時間を待ってから遷移させる
		if (GameManager::getGameManager()->TimeCount(delta_time, ANNIHILATION_TIME)) {
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new ResultScene());
		}
	}
	//ボスの場合、マップ内で戦闘が始まる為、戦闘終了したらInMapSceneに戻る
	else if (battle_state == BattleState::IDLE && enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId() != LAST_BOSS_ID) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new InMapScene(GameManager::getGameManager()->getPlayer()->getPlayerPos(), static_cast<InMapScene::InMapState>(inmap_state_save)));
		}
	}
	//最後のボスの場合、サーブを行いタイトル画面に戻る
	//今後クリアシーンなども検討中
	else if(battle_state == BattleState::IDLE && enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId() == LAST_BOSS_ID) {

		Weak save_event = std::make_shared <EventNpc>();

		//セーブを行う
		save_event.lock()->SaveProcces();

		//タイトルシーンに移動する
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new TittleScene());

	}
	//待機状態の場合はEnterキーのみ受付し、押されたらシーンを遷移させる
	else if (battle_state == BattleState::IDLE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new MapScene());
		
	}
	

	//敵のエフェクトの更新処理
	if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//ダウンキャストする
		auto boss_monster = std::dynamic_pointer_cast<BossMonster>(enemy);

		//エフェクトの描画
		boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationUpdate(delta_time);
	}

	//エフェクトの更新処理
	nomal_attack->SkillAnimationUpdate(delta_time);
	//エフェクトの配列が無い時は処理を飛ばす
	if (GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {
		return;
	}
	//エフェクトの更新処理
	GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationUpdate(delta_time);

}

//------------------------------------------------------------------------------------------------------------------------
//描画
void BattleScene::Draw()
{
	//背景画像
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, back_ground_, TRUE);

	//バトルウィンドウの描画
	BattleDraw();

	//プレイヤーのステータスの描画
	PlayerStatusDraw();

	//敵の画像の描画
	enemy->Draw();

	//バトルログウィンドウの描画
	if (auto battle_log_window = window_log.lock()) {
		battle_log_window->Menu_draw(670, 30, 600, 180);
	}

	//バトルログの文字の描画
	battle_log->drawLogs();

	//共通描画
	__super::Draw();

	//敵のエフェクトの更新処理
	if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//ダウンキャストする
		auto boss_monster = std::dynamic_pointer_cast<BossMonster>(enemy);

		//エフェクトの更新処理
		boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationDraw();
	}

	//プレイヤーのエフェクトの描画
	nomal_attack->SkillAnimationDraw();

	//エフェクトの配列が無い時は処理を飛ばす
	if (GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {
		return;
	}
	else {
		//特技のエフェクトの描画
		GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationDraw();
	}
}

//バトルウィンドウの描画
void BattleScene::BattleDraw()
{
	//アイテムを描画するウィンドウ
	auto item_window = window_log.lock();


	//最初の行動の時のウィンドウ
	if (select_action_menu == MenuAction::FIRST_ACTION) {
		select_comand_menu->All(50, 500, 250, 200);
	}
	//攻撃ウィンドウ
	else if (select_action_menu == MenuAction::FIGHT_ACTION) {
		attack_window->All(50, 500, 250, 200);
	}
	//アイテム表示のウィンドウ
	else if (select_action_menu == MenuAction::ITEM_ACTION) {
		select_comand_menu->All(50, 500, 250, 200);
		item_window->Menu_draw(300, 500, 250, 215);
		GameManager::getGameManager()->getInventory()->ItemMenu(ITEM_DRAW_POS, ITEM_CURENT_PAGE, CUROURY, ITEMPERPAGE_);
	}
	//アイテムを使う時のウィンドウ
	else if (select_action_menu == MenuAction::ITEM_USE_ACTION) {
		select_comand_menu->All(50, 500, 250, 200);
		item_window->Menu_draw(300, 500, 250, 215);
		GameManager::getGameManager()->getInventory()->ItemMenu(ITEM_DRAW_POS, ITEM_CURENT_PAGE, CUROURY, ITEMPERPAGE_);
		select_itemuse_window->All(550, 500, 250, 215);
	}
	//スキルウィンドウ
	else if (select_action_menu == MenuAction::SKILL_ACTION) {
		attack_window->All(50, 500, 250, 200);
		InventorySkillDraw();
	}
}


//------------------------------------------------------------------------------------------------------------------------
//アイテム関連


//アイテムの選択処理
void BattleScene::ItemSelectProcess()
{
	//インデックスの操作と取得
	GameManager::getGameManager()->getInventory()->ItemCurourIndex(ITEMPERPAGE_);

	//アイテムのカーソル処理
	GameManager::getGameManager()->getInventory()->CusorMove();

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//行動を移す
		select_action_menu = MenuAction::FIRST_ACTION;
		//メニューを切り替える
		select_comand_menu->SetSelectCousourMove();
	}

}

//アイテムを使った際の処理
void BattleScene::ItemUseProcess()
{
	if (select_itemuse_window->getSelectNum() == ITEMUSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//アイテムを使った時の処理
		item_->ItemUse(GameManager::getGameManager()->getInventory()->GetSelectedItemId());
		//行動を移す
		select_action_menu = MenuAction::FIRST_ACTION;
		//カーソルを動けるようにする
		select_comand_menu->SetSelectCousourMove();
		//シーケンスを切り替える
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
	}
	//閉じるが押されたら
	else if (select_itemuse_window->getSelectNum() == ITEMUSEMENUCLOSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		select_action_menu = MenuAction::ITEM_ACTION;
	}
	//左キーを押すと最初の選択メニューに戻す
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		select_action_menu = MenuAction::ITEM_ACTION;
	}
}

//敵を倒した時のアイテムドロップ処理
void BattleScene::ItemDropProcess(Enemy::EnemyConnection& enemy_)
{
	//ランダムなシード値を生成する為の変数
	std::random_device rd;

	//疑似乱数を生成する(メルセンヌツイスター)
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 100);

	//0から100の一様乱数を生成する
	int rand_val = static_cast<int>(dis(gen));

	//敵のアイテム配列を取得する
	auto& EnemyItemDrop = enemy->getItemDropArray();

	//ノーマルドロップの確率
	int nomal_drop = enemy_.GetNomalProbability();

	//レアドロップの確率
	int rare_drop = enemy_.GetRareProbability();

	//レアドロップ
	if (rand_val < rare_drop) {
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
	else if (rand_val > rare_drop && rand_val < (nomal_drop + rare_drop)) {
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
	if (item_->getAttackAmount() != 0) {

		int AttackAmoument = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetAttack() - item_->getAttackAmount();
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
void BattleScene::FleeProcess(Player::PlayerStatus& playerStatus , Enemy::EnemyConnection& enemy_ , float delta_time)
{
	//プレイヤーと敵の素早さを取得する
	auto playerSpeed = playerStatus.GetSpeed();
	auto enemySpeed = enemy_.GetEnemySpeed();

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
		if (GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {
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
		select_action_menu = MenuAction::FIGHT_ACTION;
		//カーソルを動けるようにする
		attack_window->SetSelectCousourMove();
	}
}

//スキルの描画
void BattleScene::InventorySkillDraw() {

	//ウィンドウの表示
	//ポインタがかえって来たらウィンドウを描画する
	if (auto skill_window = window_log.lock()) {
		skill_window->Menu_draw(300, 500, 250, 215);
	}

	//インベントリ内のスキルを描画する
	GameManager::getGameManager()->getInventory()->InventorySkill(ITEM_DRAW_POS , ITEM_CURENT_PAGE , CUROURY, ITEMPERPAGE_);
}

//スキルが使えるかどうかのチェック
bool BattleScene::SkillUseMpChack(Player::PlayerStatus& playerStatus)
{
	//スキルが無ければ処理を飛ばす
	if (GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {
		return false;
	}

	//MPが足りなかったら
	if (playerStatus.GetCurentMp() < GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->getSkillConsumeMp()) {
		battle_log->addLog("Mpが足りません");
		return false;
	}
	GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillMpConsume(playerStatus);
	return true;
}

//スキルを使用した際の処理
void BattleScene::SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_)
{
	auto& SkillList = GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()];

	//スキルが使えなったら処理を飛ばす
	if (!SkillUseMpChack(playerStatus)) {
		select_action_menu = MenuAction::FIGHT_ACTION;
		return;
	}
	//攻撃系のスキルの場合
	if (SkillList->getSkillType() == 0 && !GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {

		//ダメージを与える
		GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus, enemy_, battle_log);

		//エフェクトを流す
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}
	//バフ系のスキルの場合
	else if (SkillList->getSkillType() == 1 && !GameManager::getGameManager()->getPlayer()->getSkillList().empty()) {

		//バフ効果をプレイヤーに与える
		GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus , battle_log);

		//エフェクトを流す
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}
	//アイテム系のスキル
	else {
		//ダメージを与える
		GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUse(enemy_, battle_log);

		//エフェクトを流す
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}

	//プレイヤーからの入力を受け付けないようにする
	PlayerTurn = false;
	
	//スキル選択ウィンドウに戻す
	select_action_menu = MenuAction::FIGHT_ACTION;
	//カーソルを動けるようにする
	attack_window->SetSelectCousourMove();

}

//------------------------------------------------------------------------------------------------------------------------
//メニュー関連 


//メニュー毎の処理
void BattleScene::MenuUpdate(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemy_.GetEnemyDefance();
	auto EnemyHp = enemy_.GetEnemyHp();

	switch (select_action_menu)
	{
		//一番最初のメニューの時の処理
	case BattleScene::MenuAction::FIRST_ACTION:

		//たたかう
		if (select_comand_menu->getSelectNum() == ATTACK_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::FIGHT_ACTION;
			//カーソルを動けないようにする
			select_comand_menu->SetSelectCousourMove();

		}
		//道具
		else if (select_comand_menu->getSelectNum() == ITEM_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::ITEM_ACTION;
			//最初の画面のカーソルを動けなくする
			select_comand_menu->SetSelectCousourMove();
		}
		//逃げる
		else if (select_comand_menu->getSelectNum() == FLEE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::FLEE);
		}



		break;

		//攻撃選択の時に
	case BattleScene::MenuAction::FIGHT_ACTION:

		//攻撃が押された時に
		if (attack_window->getSelectNum() == ATTACK_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//プレイヤーの状態を攻撃に変える
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::NOMALATTACK);

			//シーケンスを変える
			tnl_sequence_.change(&BattleScene::seqAnimation);
		}
		//特技が選択されたら
		else if (attack_window->getSelectNum() == SPECIAL_SKILL_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//持っているスキルの選択をさせる
			select_action_menu = MenuAction::SKILL_ACTION;

			//行動を決める為の選択画面のカーソルを動けないようにする
			attack_window->SetSelectCousourMove();
		}
		//閉じるが選択されたら
		else if (attack_window->getSelectNum() == MENU_CLOSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			select_action_menu = MenuAction::FIRST_ACTION;

			//カーソルを動けるようにする
			select_comand_menu->SetSelectCousourMove();

		}
		
		//左キーを押すと最初の選択メニューに戻す
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			select_action_menu = MenuAction::FIRST_ACTION;
			
			//カーソルを動けるようにする
			select_comand_menu->SetSelectCousourMove();
		}

		break;

	case BattleScene::MenuAction::ITEM_ACTION:

		//アイテム選択処理
		ItemSelectProcess();

		//Enterキーを押したらItemUseMenuに変更する
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::ITEM_USE_ACTION;
		}

		break;

		//使うが選択されたら
	case MenuAction::ITEM_USE_ACTION:

		//アイテムを使った際の処理
		ItemUseProcess();

		break;

		//スキル処理
	case MenuAction::SKILL_ACTION:

		//スキル選択時の処理
		SkillSelectProcess();


		break;

	default:
		break;
	}

}

//------------------------------------------------------------------------------------------------------------------------
//プレイヤー関連

//レベルアップ判定 & 処理
void BattleScene::ChackPlayerLevelUp(Player::PlayerStatus& player_status) {

	//レベルを取得する
	auto Playerlevel = player_status.GetLevel();

	//レベル10以上であればレベルアップしない(今後増やして行く予定)
	if (Playerlevel >= 20) {
		return;
	}

	//プレイヤーの必要な経験値が0以下になった時にレベルをあげる
	if (player_status.GetExpoint() <= 0)
	{
		//レベルをあげる
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/levelup.mp3", DX_PLAYTYPE_BACK);

		//ログを出す
		battle_log->addLog("レベルアップした！");

		//新しいレベルを取得する
		auto Playerlevel = player_status.GetLevel() + 1;

		//プレイヤーのステータスをセットする
		GameManager::getGameManager()->getPlayer()->SetPlayerStatus(Playerlevel);

		//プレイヤーが武器を装備していたら
		if (GameManager::getGameManager()->getInventory()->GetEquipAttack() != 0) {
			auto playerAttack = player_status.GetAttack();
			player_status.SetPlayerAttack(playerAttack + GameManager::getGameManager()->getInventory()->GetEquipAttack());
		}

		//プレイヤーが防具を装備していたら
		if (GameManager::getGameManager()->getInventory()->GetEquipDefance() != 0) {
			auto playerDefance = player_status.GetDefance();
			player_status.SetPlayerDefance(playerDefance + GameManager::getGameManager()->getInventory()->GetEquipDefance());
		}

		//スキルをセットする
		GameManager::getGameManager()->getPlayer()->SkillSet(battle_log);
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
	if (auto playerStatus_window = window_log.lock()) {
		//ウィンドウを描画する
		playerStatus_window->Menu_draw(50, 50, 250, 240);
	}

	//ステータスを表示する
	UIManager::getUIManager()->PlayerStatusDrawWindow();

}

//------------------------------------------------------------------------------------------------------------------------
//敵関連


//敵の死亡処理、演出
void BattleScene::DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyConnection& enemy_)
{
	
	//ログを流す
	battle_log->addLog("敵を倒した！");

	//アイテムのドロップ処理
	ItemDropProcess(enemy_);

	//もし一時的に攻撃力が上がっていたらそれをリセットする
	BattleBuffResetProcess();

	//バトルフラグを切り替える
	item_->BattleFlagSwitch();

	//必要な経験値を取得する
	auto R_point = playerStatus.GetExpoint();

	//敵の経験値を取得する
	auto Enemy_expoint = enemy_.GetEnemyExpoint();

	// データ処理
	playerStatus.SetPlayerExpoint(R_point - Enemy_expoint);

	//ゴールドを手に入れた時のログを流す
	battle_log->addMoney("プレイヤー", enemy_.GetEnemyMoney());
	
	//所持金を追加する
	GameManager::getGameManager()->getPlayer()->AddPlayerMoney(enemy_.GetEnemyMoney());

	//敵がボスだった場合イベントを終了させる
	if (enemy_.GetEnemyId() != 20 && enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		EventManager::getEventManager()->EnemyEventFlagChange();
	}
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
		if (PlayerStatus.GetSpeed() >= enemyStatus.GetEnemySpeed()) {
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
	auto enemyHp = enemyStatus.GetEnemyHp();

	if (tnl_sequence_.isStart()) {

		PlayerTurn = true;

		select_sequence = Sequence::PLAYERACTION;

		//ログをリセットする
		battle_log->LogReset();

		//ログを流す
		battle_log->addLog("[ プレイヤーのターン ]");
	
	}

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

		select_sequence = Sequence::ENEMYACTION;

		//ログをリセットする
		battle_log->LogReset();

		//ログを流す
		battle_log->addLog("[ 敵のターン ]");

		//敵の行動を行う
		enemy->EnemyAction(battle_log);
		
		//敵が雑魚的だったらそのままプレイヤーが死んでいるかのチェックを行う
		if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {

			//プレイヤーのHpが0になったら全滅のシーンに切り替える
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::DEAD);

			PlayerTurn = true;
		}
		//敵がボスだったらボスの攻撃エフェクトを流す
		else if(enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

			tnl_sequence_.change(&BattleScene::seqAnimation);

			//プレイヤーのHpが0になったら全滅のシーンに切り替える
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::DEAD);

			PlayerTurn = true;
		}
		
	}
	//Hpが0にならなかったらターンを切り替える
	if (PlayerStatus.GetcurentHp() >= 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//状態を通常に戻してコマンドバトルを再度行う
		GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

		tnl_sequence_.change(&BattleScene::seqPlayerAction);
	}

	return false;
}

//ターンを切り替えるシーケンス
bool BattleScene::seqChangeTurn(float delta_time)
{
	//Enterキーを押したらシーケンスを遷移させる
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		tnl_sequence_.change(&BattleScene::seqEnemyAction);
	}

	return false;
}

//エフェクトを流すシーケンス
bool BattleScene::seqAnimation(float delta_time)
{

	//プレイヤーが攻撃を選択していたら
	if (GameManager::getGameManager()->getPlayer()->GetPlayerState() == Player::PlayerState::NOMALATTACK) {
		
		if (tnl_sequence_.isStart()) {
			//スキルのエフェクト流す
			nomal_attack->SkillUseAnimation();
		}

		//プレイヤーからの入力を受け付けないようにする
		PlayerTurn = false;

		if (tnl_sequence_.getProgressTime() > 1.0f) {

			//のエフェクト止める
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
			//スキルのエフェクト流す
			GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillUseAnimation();

		}

		if (tnl_sequence_.getProgressTime() > 1.0f) {

			//スキルのエフェクト止める
			GameManager::getGameManager()->getPlayer()->getSkillList()[GameManager::getGameManager()->getInventory()->GetSkillSelectedIndex()]->SkillAnimationStop();

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

			//プレイヤーの状態を待機状態にさせる
			GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

			//シーケンスを切り替える
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}


	}
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//ボスモンスターをダウンキャストで呼び出す
		auto boss_monster = std::dynamic_pointer_cast<BossMonster>(enemy);


		if (tnl_sequence_.isStart()) {

			//スキルのエフェクト流す
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillUseAnimation();

		}

		if (tnl_sequence_.getProgressTime() > 1.0f) {

			//スキルのエフェクト止める
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationStop();

			//プレイヤーのステータスを取得する
			auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

			//プレイヤーのHPが0になって居なかったら
			if (PlayerStatus.GetcurentHp() > 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//状態を通常に戻してコマンドバトルを再度行う
				GameManager::getGameManager()->getPlayer()->SetPlayerState(Player::PlayerState::IDLE);

				tnl_sequence_.change(&BattleScene::seqPlayerAction);

				return false;
			}

		}

	}

	return false;

}

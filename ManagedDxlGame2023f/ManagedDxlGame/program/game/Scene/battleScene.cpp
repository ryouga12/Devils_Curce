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
//初期化　&  解放

BattleScene::BattleScene(const tnl::Vector3& pos, const int& background, Shared<Enemy> enemy_pointer, const int& inmap_state) : back_ground_(background), map_pos(pos), inmap_state_save(inmap_state)
{
	//現在のシーンをBattleSceneに設定
	curent_scene = SceneState::BATTLE;

	//バトルシーンの状態をバトルに変更する
	battle_state = BattleState::BATTLE;

	//敵のポインタを代入する
	enemy = enemy_pointer;

	//バトルシーンの初期化
	InitBattleScene();
}

//バトルが終了した際に敵の種類に音楽を止めたりフラグを停止させる
void BattleScene::BattleEndProcces(const int& enemy_id)
{
	//サウンドを止める
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {

		SoundManager::GetSoundManager()->StopSound("sound/BGM/sentou.mp3");
	}
	//ボスがラスボスの場合
	else if(enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::ZERAHKIEL)){
		SoundManager::GetSoundManager()->StopSound("sound/BGM/maou_sentou_bgm.mp3");
	}
	//ボスが中ボス1体目の場合
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::GROVEGUARDIAN)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/tyubosu.mp3");
	}
	//ボスが中ボス2体目の場合
	else  if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::SOULWARRIOR)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/ordeal_battle.mp3");
	}
	//ボスが中ボス3体目の場合
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::SHADOWENEMY)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/battle-dark.mp3");
	}
	//サブボスの1体目
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::CORPORAL)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/corporal_battle.mp3");
	}
	//裏ボス
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy_id == static_cast<int>(BossMonster::BossType::PIRATE)) {
		SoundManager::GetSoundManager()->StopSound("sound/BGM/pirate.mp3");
	}
}

//デストラクタ
BattleScene::~BattleScene()
{
	//敵によって止める処理を変える
	BattleEndProcces(enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId());
}

//バトルシーンの初期化
void BattleScene::InitBattleScene()
{
	//バトルログを初期化する
	battle_log = std::make_shared<BattleLog>();

	//アイテムを初期化する
	item_ = std::make_shared<Item>();

	//メニューの初期化
	InitMenuWindow();

	//プレイヤーの状態をIDLEにする(コマンド入力受付の為)
	GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

	//武器のタイプセットする
	SetWeaponType();

	//通常攻撃のポインタをセットする
	nomal_attack = std::make_shared<Nomal_Attack>(weapon_type);

	//バトルフラグを切り替える(バトルのみでつかるアイテムの為)
	item_->BattleFlagSwitch();

	//バトルログをセットする
	item_->SetBattleLog(battle_log);

	//カーソルをリセットする
	GameManager::GetGameManager()->GetInventory()->CursorReset();

	//現在のページを一番最初のページにする
	GameManager::GetGameManager()->GetInventory()->CurentPageReset();

	//インデックスを初期化する
	GameManager::GetGameManager()->GetInventory()->SelectedIndexClear();

	//バトルシーンをセットする
	enemy->SetBattleScene(this);

	//プレイヤーのバトルシーンをセットする
	auto& PlayerPtr = GameManager::GetGameManager()->GetPlayer();
	GameManager::GetGameManager()->GetPlayer()->SetBattleScene(this);

	//カメラの保管座標をセットする
	GameManager::GetGameManager()->GetCamera()->SavePosition(map_pos);

	//敵がモブの場合、通常戦闘音楽を流す
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {
		//バトル用のBGMを流す
		SoundManager::GetSoundManager()->Sound_Play("sound/BGM/sentou.mp3", DX_PLAYTYPE_LOOP);

	}
	//敵がボスの場合ダウンキャストしてエフェクト用のポインタを用意する
	//ダウンキャストは処理が重いため一番最初にキャストする
	else {
		boss_monster_ = std::dynamic_pointer_cast<BossMonster>(enemy);
	}
}

//メニューの初期化
void BattleScene::InitMenuWindow()
{
	//最初の行動を決めるwindow
	select_action_coment = new MenuWindow::MenuElement_t[]
	{
		{MENU_COMENT_POS_FIRST.x ,MENU_COMENT_POS_FIRST.y ,   "たたかう" , MenuWindow::Elements::FIRST_ELEMENT},
		{MENU_COMENT_POS_SECOND.x , MENU_COMENT_POS_SECOND.y , "道具" ,    MenuWindow::Elements::SECOND_ELEMENT},
		{MENU_COMENT_POS_THIRD.x , MENU_COMENT_POS_THIRD.y ,   "逃げる" ,  MenuWindow::Elements::THERD_ELEMENT}

	};

	//要素の数
	const int SELECT_ACTION_MENU_ELEMENTS_NUM = 3;

	select_comand_menu = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_action_coment, SELECT_ACTION_MENU_ELEMENTS_NUM);
	select_comand_menu->Open();

	//攻撃を決めるウィンドウ
	select_action_attack = new MenuWindow::MenuElement_t[]
	{
		{MENU_COMENT_POS_FIRST.x , MENU_COMENT_POS_FIRST.y ,  "攻撃" ,  MenuWindow::Elements::FIRST_ELEMENT},
		{MENU_COMENT_POS_SECOND.x , MENU_COMENT_POS_SECOND.y, "特技"   ,MenuWindow::Elements::SECOND_ELEMENT},
		{MENU_COMENT_POS_THIRD.x , MENU_COMENT_POS_THIRD.y ,  "戻る" ,  MenuWindow::Elements::THERD_ELEMENT}
	};

	//要素の数
	const int ATTACK_WINDOW_ELEMENTS_NUM = 3;

	attack_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_action_attack, ATTACK_WINDOW_ELEMENTS_NUM);
	attack_window->Open();

	

	//アイテムを使用のウィンドウ
	select_detail = new MenuWindow::MenuElement_t[]{
		{ MENU_ITEM_USE_POS.x , MENU_ITEM_USE_POS.y ,  "使う",    MenuWindow::Elements::FIRST_ELEMENT},
		{ MENU_ITEM_CLOSE.x ,   MENU_ITEM_CLOSE.y ,    "閉じる",  MenuWindow::Elements::SECOND_ELEMENT}
	};

	//要素の数
	const int SELECT_ITEM_USE_WINDOW_ELEMENTS_NUM = 2;

	select_itemuse_window = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", select_detail, SELECT_ITEM_USE_WINDOW_ELEMENTS_NUM);
	select_itemuse_window->Open();

	//mapに格納する
	UIManager::GetUIManager()->addMenu("select_use_window", select_itemuse_window);
}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void BattleScene::Update(float delta_time)
{
	//共通処理
	__super::Update(delta_time);

	//バトルシーンの更新処理(切り替えなど)
	BattleSceneUpdate(delta_time);
	
	//プレイヤーのHPとMPバーの更新処理
	UIManager::GetUIManager()->PlayerStatusBarUpdate(delta_time);

	//敵のエフェクトの更新処理
	if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//ロックしてポインタを使えるようにする
		if (auto boss_monster = boss_monster_.lock()) {

			//エフェクトの描画
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationUpdate(delta_time);
		}
	}

	//エフェクトの更新処理
	nomal_attack->SkillAnimationUpdate(delta_time);

	//エフェクトの配列が無い時は処理を飛ばす
	if (!GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {

		//エフェクトの更新処理
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillAnimationUpdate(delta_time);

	}
}

//バトルシーンの切り替え
void BattleScene::BattleSceneUpdate(const float delta_time)
{
	//戦闘を行っている間はシーケンスを動かす
	if (battle_state == BattleState::BATTLE) {

		//シーケンスの更新処理
		tnl_sequence_.update(delta_time);

		//プレイヤーのターンのみ
		if (PlayerTurn) {
			//プレイヤーの更新処理
			GameManager::GetGameManager()->GetPlayer()->PlayerMoveProcess(delta_time, battle_log, enemy, nomal_attack);
		}

	}
	//待機状態かつプレイヤーの状態がDEADだった場合、お金を半分にしてマップシーンに戻す
	else if (battle_state == BattleState::BATTLE_END && GameManager::GetGameManager()->GetPlayer()->GetPlayerState() == Player::PlayerState::DEAD
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//プレイヤーのポインタを取得する
		auto& player = GameManager::GetGameManager()->GetPlayer();

		//プレイヤーのお金を半分する
		player->ReducePlayerMoney(player->GetPlayerMoney() / koni::Numeric::DIVIDER_TWO);

		//プレイヤーのHpをMAXにする
		player->GetPlayerStatusSave().SetPlayerCurentHp(player->GetPlayerStatusSave().GetMaxHp());

		//プレイヤーのバフ関連をリセットする
		BattleBuffResetProcess();

		//BattleStateを待機状態にする
		battle_state = BattleState::IDLE;

		//シーンマネージャーのインスタンスを取得する
		auto mgr = SceneManager::GetSceneManager();

		//敵が雑魚敵の場合
		if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {
			mgr->changeScene(new MapScene());
		}
		//敵がボスの場合
		else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
			mgr->changeScene(new InMapScene(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), static_cast<InMapScene::InMapState>(inmap_state_save)));
		}

	}
	//ボスの場合、マップ内で戦闘が始まる為、戦闘終了したらInMapSceneに戻る
	else if (battle_state == BattleState::BATTLE_END && enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId() != static_cast<int>(BossMonster::BossType::ZERAHKIEL)
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		auto mgr = SceneManager::GetSceneManager();

		//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), static_cast<InMapScene::InMapState>(inmap_state_save)));

	}
	//最後のボスの場合、セーブを行いエピローグを流す
	else if (battle_state == BattleState::BATTLE_END && enemy->GetEnemyType() == Enemy::Enemytype::BOSS && enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId() == static_cast<int>(BossMonster::BossType::ZERAHKIEL)
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		Weak save_event = std::make_shared <Event>();

		//セーブを行う
		save_event.lock()->SaveProcces(static_cast<int>(InMapScene::InMapState::VILLAGE));

		//セーブのテキストを呼ばないようにする
		UIManager::GetUIManager()->SaveTextFlagChange();

		//BattleStateを待機状態にする
		battle_state = BattleState::IDLE;

		//エピローグを流す
		auto mgr = SceneManager::GetSceneManager();

		//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new ResultScene());

	}
	//待機状態の場合はEnterキーのみ受付し、押されたらシーンを遷移させる
	else if (battle_state == BattleState::BATTLE_END && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//BattleStateを待機状態にする
		battle_state = BattleState::IDLE;

		//シーンを遷移させる
		auto mgr = SceneManager::GetSceneManager();
		mgr->changeScene(new MapScene());

	}
}

//------------------------------------------------------------------------------------------------------------------------
//描画
void BattleScene::Draw()
{
	//背景画像
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, back_ground_, TRUE);

	//プレイヤーのステータスの描画
	PlayerStatusDraw();

	//敵の画像の描画
	enemy->Draw();

	//バトルウィンドウの描画
	BattleDraw();

	//バトルログウィンドウの描画
	UIManager::GetUIManager()->Menu_Draw("menu_window" , BATTLE_LOG_WINDOW_POS.x, BATTLE_LOG_WINDOW_POS.y, BATTLE_LOG_WINDOW_WIDTH, BATTLE_LOG_WINDOW_HEIGHT);

	//バトルログの文字の描画
	battle_log->DrawLogs();

	//共通描画
	__super::Draw();

	//敵のエフェクトの更新処理
	if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//ロックしてポインタを使えるようにする
		if (auto boss_monster = boss_monster_.lock()) {
			//エフェクトの更新処理
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationDraw();
		}
	}

	//プレイヤーのエフェクトの描画
	nomal_attack->SkillAnimationDraw();

	//エフェクトの配列が無い時は処理を飛ばす
	if (GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {
		return;
	}
	else {
		//特技のエフェクトの描画
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillAnimationDraw();
	}
}

//バトルウィンドウの描画
void BattleScene::BattleDraw()
{
	//最初の行動の時のウィンドウ
	if (select_action_menu == MenuAction::FIRST_ACTION) {
		//コマンドウィンドウの表示
		select_comand_menu->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
	}
	//攻撃ウィンドウ
	else if (select_action_menu == MenuAction::FIGHT_ACTION) {
		//攻撃コマンドウィンドウの表示
		attack_window->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
	}
	//アイテム表示のウィンドウ
	else if (select_action_menu == MenuAction::ITEM_ACTION) {
		//コマンドウィンドウの表示
		select_comand_menu->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		//アイテムウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_WINDOW_POS.x, ITEM_WINDOW_POS.y, ITEM_WINDOW_WIDTH, ITEM_WINDOW_HEIGHT);
		//アイテムの名前の表示
		GameManager::GetGameManager()->GetInventory()->ItemMenu(ITEM_DRAW_POS, ITEM_CURENT_PAGE, CUROURY, ITEMPERPAGE_);
	}
	//アイテムを使う時のウィンドウ
	else if (select_action_menu == MenuAction::ITEM_USE_ACTION) {
		//コマンドウィンドウの表示
		select_comand_menu->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		//アイテムウィンドウの表示
		UIManager::GetUIManager()->Menu_Draw("menu_window", ITEM_WINDOW_POS.x, ITEM_WINDOW_POS.y, ITEM_WINDOW_WIDTH, ITEM_WINDOW_HEIGHT);
		//アイテムの名前の表示
		GameManager::GetGameManager()->GetInventory()->ItemMenu(ITEM_DRAW_POS, ITEM_CURENT_PAGE, CUROURY, ITEMPERPAGE_);
		//使うかやめるかの選択ウィンドウ
		select_itemuse_window->All(SELECT_ITEM_USE_WINDOW_POS.x, SELECT_ITEM_USE_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
	}
	//スキルウィンドウ
	else if (select_action_menu == MenuAction::SKILL_ACTION) {
		//攻撃の選択ウィンドウ
		attack_window->All(MENU_WINDOW_POS.x, MENU_WINDOW_POS.y, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
		//スキルの表示
		InventorySkillDraw();
	}
}


//------------------------------------------------------------------------------------------------------------------------
//アイテム関連


//アイテムの選択処理
void BattleScene::ItemSelectProcess()
{
	//インデックスの操作と取得
	GameManager::GetGameManager()->GetInventory()->ItemCurourIndex(ITEMPERPAGE_);

	//アイテムのカーソル処理
	GameManager::GetGameManager()->GetInventory()->CusorMove();

	//バックスペースを押したらメニューを戻す
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		//効果音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//行動を移す
		select_action_menu = MenuAction::FIRST_ACTION;
		//メニューを切り替える
		select_comand_menu->SelectCousourMoveFlagChange();
	}

}

//アイテムを使った際の処理
void BattleScene::ItemUseProcess()
{
	if (select_itemuse_window->GetSelectNum() == ITEMUSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//効果音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

		//アイテムを使った時の処理
		//アイテムの効果が適用できたら
		if (item_->ItemUse(GameManager::GetGameManager()->GetInventory()->GetSelectedItemId())) {
			//行動を移す
			select_action_menu = MenuAction::FIRST_ACTION;
			//カーソルを動けるようにする
			select_comand_menu->SelectCousourMoveFlagChange();
			//プレイヤーのターンを終了させる
			PlayerTurn = false;
			//シーケンスを切り替える
			tnl_sequence_.immediatelyChange(&BattleScene::seqChangeTurn);
		}
		//アイテムがの効果が適用出来なかったら
		else {
			//行動をアイテム選択メニューに戻す
			select_action_menu = MenuAction::ITEM_ACTION;
			//カーソルを動けるようにする
			//select_comand_menu->SelectCousourMoveFlagChange();
		}
	}
	//閉じるが押されたら
	else if (select_itemuse_window->GetSelectNum() == ITEMUSEMENUCLOSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		select_action_menu = MenuAction::ITEM_ACTION;
	}
	//バックスペースを押すと最初の選択メニューに戻す
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		select_action_menu = MenuAction::ITEM_ACTION;
	}
}

//敵を倒した時のアイテムドロップ処理
void BattleScene::ItemDropProcess(Enemy::EnemyConnection& enemy_)
{
	//ランダムなシード値を生成する為の変数
	std::random_device rd;

	//乱数値の最低値
	const int RAND_PROBABILITY_MIN = 0;
	//乱数値の最大値
	const int RAND_PROBABILITY_MAX = 100;

	//疑似乱数を生成する(メルセンヌツイスター)
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(RAND_PROBABILITY_MIN, RAND_PROBABILITY_MAX);

	//0から100の一様乱数を生成する
	int rand_val = static_cast<int>(dis(gen));

	//敵のアイテム配列を取得する
	auto& EnemyItemDrop = enemy->getItemDropArray();

	//ノーマルドロップの確率
	int nomal_drop = enemy_.GetNomalProbability();

	//レアドロップの確率
	int rare_drop = enemy_.GetRareProbability();

	//プレイヤーのポインタを取得する
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//インベントリのポインタを取得する
	auto& inventory_ = GameManager::GetGameManager()->GetInventory();

	//ドロップアイテム
	enum DropItemes {
		NOMAL,
		RARE,
	};

	//レアドロップ
	if (rand_val < rare_drop) {

		//ログをながす
		//持ち物がいっぱいじゃなかったら
		if (inventory_->GetInventoryList().size() != inventory_->GetInventoryMaxSize()) {
			battle_log->AddItemDrop(player->GetPlayerName(), EnemyItemDrop[RARE].GetItemName());
		}
		//持ち物がいっぱいだったら
		else {
			battle_log->AddLog("持ち物がいっぱいの為アイテムを入手する事ができませんでした");
		}

		//敵のアイテムをインベントリに格納する
		inventory_->AddInventory(EnemyItemDrop[1].GetItemId());
	}
	//通常ドロップ
	else if (rand_val > rare_drop && rand_val < (nomal_drop + rare_drop)) {

		//ログをながす
		//持ち物がいっぱいじゃなかったら
		if (inventory_->GetInventoryList().size() != inventory_->GetInventoryMaxSize()) {
			battle_log->AddItemDrop(player->GetPlayerName(), EnemyItemDrop[NOMAL].GetItemName());
		}
		//持ち物がいっぱいだったら
		else {
			battle_log->AddLog("持ち物がいっぱいの為アイテムを入手する事ができませんでした");
		}
		
		//敵のアイテムをインベントリに格納する
		inventory_->AddInventory(EnemyItemDrop[NOMAL].GetItemId());
	}
}

//変動した値をリセットする
void BattleScene::BattleBuffResetProcess()
{
	//ベースの攻撃力
	auto player_base_attck = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetBaseAttack();

	//攻撃力
	auto curent_player_attack = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetAttack();

	//インベントリ
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//武器を装備している場合
	if (!inventory->GetEquipWeaponArray().empty()) {

		for (auto& equip_weqpon : inventory->GetEquipWeaponArray()) {

			//プレイヤーの攻撃力が変化していたら
			if ((player_base_attck + equip_weqpon.GetItemDamage()) != curent_player_attack) {

			//装備時のステータスに戻す
			GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerAttack(player_base_attck + equip_weqpon.GetItemDamage());

			}
		}
	}
	//装備していなければ比較して変動していれば値を戻す
	else if(player_base_attck != curent_player_attack){

		//装備時のステータスに戻す
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerAttack(player_base_attck);

	}
	//変動していなければ処理を飛ばす
	else{
		return;
	}
	
}

//武器をセットしてエフェクトを適用させる
void BattleScene::SetWeaponType()
{
	//武器が装備されていたら
	if (!GameManager::GetGameManager()->GetInventory()->GetEquipWeaponArray().empty()) {

		auto it = GameManager::GetGameManager()->GetInventory()->GetEquipWeaponArray().begin();

		//武器のタイプを取得する
		weapon_type = (*it).GetItemWeapontype();

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

	//敵がモブの場合素早さによって逃走率を調整する
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {
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
	}
	//敵がボスの場合逃げれなくする
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		// 逃走成功率を低下させる処理
		probability = 0; 
	}

	// 逃走の成功確率を判定する
	if (rand() % 100 < probability) {
		// 逃走に成功した場合の処理
		battle_log->AddLog("うまく逃げれた");

		//戦闘BGMを止める
		SoundManager::GetSoundManager()->StopSound("sound/BGM/sentou.mp3");
		//逃げるの効果音を流す
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/nigeru.mp3", DX_PLAYTYPE_BACK);
		//音量を変える
		SoundManager::GetSoundManager()->ChangeSoundVolume(50, "sound/SoundEffect/nigeru.mp3");
		//バトルを終了させる
		battle_state = BattleState::BATTLE_END;
	}
	else {
		// 逃走に失敗した場合の処理
		battle_log->AddLog("逃げられなかった");
		tnl_sequence_.change(&BattleScene::seqChangeTurn);
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);
	}
}

//限定のボス戦での処理
bool BattleScene::CheckRequiredItemEnemyProcess()
{
	//必須アイテムのID(光の玉)
	const int REQUIRED_ITEM_ID = 39;

	//必須アイテムを持っている場合そのまま戦闘を行う
	if (EventManager::GetEventManager()->CheckEventItem(REQUIRED_ITEM_ID , FALSE)) {
		return true;
	}
	//もしもっていなければ
	else {

		//ログを流す
		battle_log->AddLog("攻撃がとおらない!?");

		//ターンを終わらせる
		PlayerTurn = false;

		//プレイヤーを待機状態にさせる
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::IDLE);

		//シーケンスを切り替える
		tnl_sequence_.immediatelyChange(&BattleScene::seqChangeTurn);

		return false;
	}
}

//------------------------------------------------------------------------------------------------------------------------
//スキル関連

//スキルが選択された時の処理
void BattleScene::SkillSelectProcess()
{
	//スキルのカーソル移動処理時のインデックス操作
	GameManager::GetGameManager()->GetInventory()->SkillCurourIndex(ITEMPERPAGE_);

	//特技のカーソル移動
	GameManager::GetGameManager()->GetInventory()->SkillCousorMove();

	//Enterキーを押された時に
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//スキルが何もない場合には処理を行わない
		if (GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {
			return;
		}

		//プレイヤーの状態を攻撃に変えて攻撃
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::SKILL);
	}
	//バックスペースを押したらメニューを戻す
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		//効果音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//行動を移す
		select_action_menu = MenuAction::FIGHT_ACTION;
		//カーソルを動けるようにする
		attack_window->SelectCousourMoveFlagChange();
	}
}

//スキルの描画
void BattleScene::InventorySkillDraw() {

	//UIマネージャーを取得する
	auto ui_manager = UIManager::GetUIManager();

	//ウィンドウの表示
	ui_manager->Menu_Draw("menu_window", SKILL_WINDOW_POS.x, SKILL_WINDOW_POS.y, SKILL_WINDOW_WIDTH, SKILL_WINDOW_HEIGHT);
	
	//インベントリを取得する
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//インベントリ内のスキルを描画する
	inventory->InventorySkill(Skill_DRAW_POS, ITEM_CURENT_PAGE , CUROURY, ITEMPERPAGE_);
	
	//スキルが空じゃない場合、スキル説明も描画する
	if (!GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {

		//スキルウィンドウの座標
		const tnl::Vector2i SKILL_WINDOW_POS = { 570 , 500 };

		//スキル説明を描画する
		inventory->SkillDetailDraw(inventory->GetSkillSelectedIndex(), SKILL_WINDOW_POS , curent_scene , enemy->GetEnemyArray()[enemy->GetEnemy_Index()].GetEnemyId());
	}

}

//スキルが使えるかどうかのチェック
bool BattleScene::SkillUseMpChack(Player::PlayerStatus& playerStatus)
{
	//スキルが無ければ処理を飛ばす
	if (GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {
		return false;
	}

	//MPが足りなかったら
	if (playerStatus.GetCurentMp() < GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->GetSkillConsumeMp()) {
		battle_log->AddLog("Mpが足りません");
		return false;
	}
	else {
		return true;
	}
}

//スキルを使用した際の処理
void BattleScene::SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_)
{
	auto& SkillList = GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()];

	//スキルが使えなったら処理を飛ばす
	if (!SkillUseMpChack(playerStatus)) {
		//もう一度選択させる
		select_action_menu = MenuAction::SKILL_ACTION;
		//プレイヤーの状態を待機状態にさせる
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

		return;
	}
	//攻撃系のスキルの場合
	if (SkillList->GetSkillType() == AttackType && !GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {

		//ボスが影の番人の場合のみ必須アイテムが必要な為、確認をはさむ
		//falseがかえってきた場合、早期リターンさせる
		if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::SHADOWENEMY) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS
			&& !CheckRequiredItemEnemyProcess()) {

			//状態を戻す
			select_action_menu = MenuAction::FIGHT_ACTION;

			//カーソルの状態を変更する
			attack_window->SelectCousourMoveFlagChange();

			return;
		}

		//ダメージを与える
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus, enemy_, battle_log);

		//MPを減らす
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillMpConsume(playerStatus);

		//エフェクトを流す
		tnl_sequence_.immediatelyChange(&BattleScene::seqAnimation);
	}
	//バフ系のスキルの場合
	else if (SkillList->GetSkillType() == BuffType && !GameManager::GetGameManager()->GetPlayer()->getSkillList().empty()) {

		//バフ効果をプレイヤーに与える
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillUse(playerStatus , battle_log);

		//MPを減らす
		GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillMpConsume(playerStatus);

		//エフェクトを流す
		tnl_sequence_.immediatelyChange(&BattleScene::seqAnimation);
	}
	

	//プレイヤーからの入力を受け付けないようにする
	PlayerTurn = false;
	
	//スキル選択ウィンドウに戻す
	select_action_menu = MenuAction::FIGHT_ACTION;
	//カーソルを動けるようにする
	attack_window->SelectCousourMoveFlagChange();

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
		if (select_comand_menu->GetSelectNum() == ATTACK_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::FIGHT_ACTION;
			//カーソルを動けないようにする
			select_comand_menu->SelectCousourMoveFlagChange();

		}
		//道具
		else if (select_comand_menu->GetSelectNum() == ITEM_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::ITEM_ACTION;
			//最初の画面のカーソルを動けなくする
			select_comand_menu->SelectCousourMoveFlagChange();
		}
		//逃げる
		else if (select_comand_menu->GetSelectNum() == FLEE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::FLEE);
		}



		break;

		//攻撃選択の時に
	case BattleScene::MenuAction::FIGHT_ACTION:

		//攻撃が押された時に
		if (attack_window->GetSelectNum() == ATTACK_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//ボスが影の番人の場合のみ必須アイテムが必要な為、確認をはさむ
			//falseがかえってきた場合、早期リターンさせる
			if(enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::SHADOWENEMY) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS
				&& !CheckRequiredItemEnemyProcess()){
				return;
			}

			//プレイヤーの状態を攻撃に変える
			GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::NOMALATTACK);

			//シーケンスを変える
			tnl_sequence_.change(&BattleScene::seqAnimation);
		}
		//特技が選択されたら
		else if (attack_window->GetSelectNum() == SPECIAL_SKILL_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//持っているスキルの選択をさせる
			select_action_menu = MenuAction::SKILL_ACTION;

			//行動を決める為の選択画面のカーソルを動けないようにする
			attack_window->SelectCousourMoveFlagChange();
		}
		//閉じるが選択されたら
		else if (attack_window->GetSelectNum() == MENU_CLOSE_ && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			select_action_menu = MenuAction::FIRST_ACTION;

			//カーソルを動けるようにする
			select_comand_menu->SelectCousourMoveFlagChange();

		}
		
		//バックスペースを押すと最初の選択メニューに戻す
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
			select_action_menu = MenuAction::FIRST_ACTION;
			
			//カーソルを動けるようにする
			select_comand_menu->SelectCousourMoveFlagChange();
		}

		break;

	case BattleScene::MenuAction::ITEM_ACTION:

		//アイテム選択処理
		ItemSelectProcess();

		//Enterキーを押したらItemUseMenuに変更する
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//決定音を鳴らす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
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
	auto player_level = player_status.GetLevel();

	//最大レベルを取得する
	auto player_max_level = GameManager::GetGameManager()->GetPlayer()->GetMaxLevel();

	//プレイヤーの必要な経験値が0以下になった時にレベルをあげる
	//レベル20以上であればレベルアップしない(今後増やして行く予定)
	
	//レベルがまだ20じゃ無ければ経験値を付与する
	if (player_status.GetExpoint() <= 0 && player_level < player_max_level)
	{
		//レベルをあげる
		//効果音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/levelup.mp3", DX_PLAYTYPE_BACK);

		//ログを出す
		battle_log->AddLog("レベルアップした！");

		//新しいレベルを取得する
		auto Playerlevel = player_status.GetLevel() + 1;

		//プレイヤーのステータスをセットする
		GameManager::GetGameManager()->GetPlayer()->SetPlayerStatus(Playerlevel);

		//プレイヤーが武器を装備していたら
		if (GameManager::GetGameManager()->GetInventory()->GetEquipAttack() != 0) {
			auto playerAttack = player_status.GetAttack();
			player_status.SetPlayerAttack(playerAttack + GameManager::GetGameManager()->GetInventory()->GetEquipAttack());
		}

		//プレイヤーが防具を装備していたら
		if (GameManager::GetGameManager()->GetInventory()->GetEquipDefance() != 0) {
			auto playerDefance = player_status.GetDefance();
			player_status.SetPlayerDefance(playerDefance + GameManager::GetGameManager()->GetInventory()->GetEquipDefance());
		}

		//スキルをセットする
		GameManager::GetGameManager()->GetPlayer()->SkillSet(battle_log);
	}
	//必要な経験値が0にならなかったら
	//もしくはレベルが最大レベルだったら
	else {
		//勝利音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/syouri.mp3", DX_PLAYTYPE_BACK);
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_40_PERCENT, "sound/SoundEffect/syouri.mp3");
	}

}


//プレイヤーのHPとMPバーの処理
void BattleScene::PlayerStatusDraw()
{
	//ウィンドウを描画する
	UIManager::GetUIManager()->Menu_Draw("menu_window", PLAYER_STATUS_WINDOW_POS.x, PLAYER_STATUS_WINDOW_POS.y, PLAYER_STATUS_WINDOW_WIDTH, PLAYER_STATUS_WINDOW_HEIGHT);
	

	//ステータスを表示する
	UIManager::GetUIManager()->PlayerStatusDrawWindow();

}

//------------------------------------------------------------------------------------------------------------------------
//敵関連

//敵の攻撃
void BattleScene::EnemyTurnProcess()
{
	//敵の行動を行う
	enemy->EnemyAction(battle_log);

	//敵が雑魚的だったらそのままプレイヤーが死んでいるかのチェックを行う
	if (enemy->GetEnemyType() == Enemy::Enemytype::MOB) {

		//プレイヤーのHpが0になったら全滅のシーンに切り替える
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::DEAD);

		//状態確認の為フラグを切り替える
		PlayerTurn = true;

		//ターンを切り替える
		tnl_sequence_.immediatelyChange(&BattleScene::seqChangeTurn);

	}
	//敵がボスだったらボスの攻撃エフェクトを流す
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//プレイヤーのHpが0になったら全滅のシーンに切り替える
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::DEAD);

		//状態確認の為フラグを切り替える
		PlayerTurn = true;

		//アニメーションを流す
		tnl_sequence_.change(&BattleScene::seqAnimation);
	}
}

//敵の死亡処理、演出
void BattleScene::DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyConnection& enemy_)
{
	
	//ログを流す
	battle_log->AddLog("敵を倒した！");

	//アイテムのドロップ処理
	ItemDropProcess(enemy_);

	//もし攻撃力が変動していたらもとに戻す
	BattleBuffResetProcess();

	//バトルフラグを切り替える
	item_->BattleFlagSwitch();

	//必要な経験値を取得する
	auto R_point = playerStatus.GetExpoint();

	//敵の経験値を取得する
	auto Enemy_expoint = enemy_.GetEnemyExpoint();

	//プレイヤーのポインタを取得する
	auto& player = GameManager::GetGameManager()->GetPlayer();

	// データ処理
	playerStatus.SetPlayerExpoint(R_point - Enemy_expoint);

	//ゴールドを手に入れた時のログを流す
	std::string log = player->GetPlayerName() + "は" + std::to_string(enemy_.GetEnemyMoney()) + "ゴールドを手に入れた！";
	battle_log->AddLog(log);
	
	//所持金を追加する
	GameManager::GetGameManager()->GetPlayer()->AddPlayerMoney(enemy_.GetEnemyMoney());

	//---敵がボスだった場合イベントを終了させる---//

	//中ボス1体目
	if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::GROVEGUARDIAN) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		EventManager::GetEventManager()->EnemyEventFlag_1Change();
	}
	//中ボス2体目
	else if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::SOULWARRIOR) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		EventManager::GetEventManager()->EnemyEventFlag_2Change();

		//闇の玉をインベントリから消去して光の玉をインベントリに加える(重要アイテム)
		if (item_) {

			//闇の玉のID
			const int BALL_OF_DARKNESS_ID = 31;
			//光の玉のID
			const int BALL_OF_LIGHT_ID = 39;

			//闇の玉をインベントリからから消去する
			GameManager::GetGameManager()->GetInventory()->InventoryItemRemove(BALL_OF_DARKNESS_ID);
			//光の玉をインベントリに追加する
			GameManager::GetGameManager()->GetInventory()->AddInventory(item_->GetItemById(BALL_OF_LIGHT_ID).GetItemId());
			//ログを流す
			battle_log->AddLog(player->GetPlayerName() + "は光の玉を手に入れた！");

		}
	}
	//中ボス3体目
	else if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::SHADOWENEMY) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {
		EventManager::GetEventManager()->EnemyEventFlag_3Change();
	}
	//ラストボス
	else if (enemy_.GetEnemyId() == static_cast<int>(BossMonster::BossType::ZERAHKIEL) && enemy->GetEnemyType() == Enemy::Enemytype::BOSS
			&& EventManager::GetEventManager()->GetLastBossFlag()) {
		//フラグを切り替える
		//裏ボス出現の為
		EventManager::GetEventManager()->LastBossFlagChange();
	}
}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連(主にターン制御)


//最初の素早さを比較する
bool BattleScene::seqIdle(float delta_time)
{
	//ステータスを取得する
	auto& PlayerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];

	//シーケンスの初期化
	if (tnl_sequence_.isStart()) {
		//素早さを比較する
		//プレイヤーが早い場合
		if (PlayerStatus.GetSpeed() >= enemyStatus.GetEnemySpeed()) {
			//プレイヤーのシーケンスへ
			tnl_sequence_.change(&BattleScene::seqPlayerAction);
		}
		//エネミーが早い場合
		else {

			//エネミーのシーケンスへ
			tnl_sequence_.change(&BattleScene::seqEnemyAction);

		}
	}

	return true;
}

//プレイヤーのターン
bool BattleScene::seqPlayerAction(float delta_time)
{
	auto& playerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()];
	auto enemyHp = enemyStatus.GetEnemyHp();

	if (tnl_sequence_.isStart()) {

		select_sequence = Sequence::PLAYERACTION;

		//ログをリセットする
		battle_log->LogReset();

		//ログを流す
		battle_log->AddLog("[ プレイヤーのターン ]");
	
	}

	//ログを流してから行動できるようにする
	if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_0_2_F) {
		PlayerTurn = true;
	}


	return false;

}

//敵のターン
bool BattleScene::seqEnemyAction(float delta_time)
{
	auto& PlayerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();
	auto& enemyStatus = enemy->GetEnemyArray()[enemy->GetEnemy_Index()]; 

	//シーケンスが始まった最初の一回のみ処理を行う
	if (tnl_sequence_.isStart()) {

		PlayerTurn = false;

		select_sequence = Sequence::ENEMYACTION;

		//ログをリセットする
		battle_log->LogReset();

		//ログを流す
		battle_log->AddLog("[ 敵のターン ]");

		//警告音をならす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/warningsound.mp3", DX_PLAYTYPE_BACK);

		//ボリュームを変える
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/warningsound.mp3");
		
	}

	//0.5秒経ってから敵の行動を行う
	if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_0_5_F) {
		//敵の行動を行う
		EnemyTurnProcess();
	}
	
	return false;
}

//ターンを切り替えるシーケンス
bool BattleScene::seqChangeTurn(float delta_time)
{

	//Enterキーを押したらシーケンスを遷移させる
	//プレイヤーのターンの場合
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		&& select_sequence == Sequence::PLAYERACTION) {
		tnl_sequence_.change(&BattleScene::seqEnemyAction);
	}
	//エネミーターンの場合
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		&& select_sequence == Sequence::ENEMYACTION) {

		//状態を通常に戻してコマンドバトルを再度行う
		GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

		//選択制御の為一時的にフラグを切り替える
		PlayerTurn = false;

		tnl_sequence_.change(&BattleScene::seqPlayerAction);
	}

	return false;
}

//エフェクトを流すシーケンス
bool BattleScene::seqAnimation(float delta_time)
{
	//プレイヤーが攻撃を選択していたら
	if (GameManager::GetGameManager()->GetPlayer()->GetPlayerState() == Player::PlayerState::NOMALATTACK) {

		if (tnl_sequence_.isStart()) {
			//スキルのエフェクト流す
			nomal_attack->SkillUseAnimation();
		}

		//プレイヤーからの入力を受け付けないようにする
		PlayerTurn = false;

		if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_1_F) {

			//のエフェクト止める
			nomal_attack->SkillAnimationStop();

			//敵が死んでいるか確認してもし死んでいたらバトルを終了させる
			if (enemy->ChackDeadEnemy()) {

				//敵の画像を消す
				enemy->DeadEnemyFlag();

				//バトルを終了させる
				battle_state = BattleState::BATTLE_END;

				//プレイヤーの状態を待機状態にさせる
				GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

				return false;

			}

			//敵が死んでいなかったらシーケンスを切り替える
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}
	}
	else if (GameManager::GetGameManager()->GetPlayer()->GetPlayerState() == Player::PlayerState::SKILL) {

		if (tnl_sequence_.isStart()) {
			//スキルのエフェクト流す
			GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillUseAnimation();

		}

		if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_1_F) {

			//スキルのエフェクト止める
			GameManager::GetGameManager()->GetPlayer()->getSkillList()[GameManager::GetGameManager()->GetInventory()->GetSkillSelectedIndex()]->SkillAnimationStop();

			//敵が死んでいるか確認してもし死んでいたらバトルを終了させる
			if (enemy->ChackDeadEnemy()) {

				//敵の画像を消す
				enemy->DeadEnemyFlag();

				//バトルを終了させる
				battle_state = BattleState::BATTLE_END;

				//プレイヤーの状態を待機状態にさせる
				GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::CHOICE);

				return false;

			}

			//プレイヤーを待機状態にさせる
			GameManager::GetGameManager()->GetPlayer()->SetPlayerState(Player::PlayerState::IDLE);

			//シーケンスを切り替える
			tnl_sequence_.change(&BattleScene::seqChangeTurn);
		}


	}
	else if (enemy->GetEnemyType() == Enemy::Enemytype::BOSS) {

		//ボスモンスターをロックして呼び出す
		auto boss_monster = boss_monster_.lock();

		if (tnl_sequence_.isStart()) {

			//スキルのエフェクト流す
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillUseAnimation();

		}

		if (tnl_sequence_.getProgressTime() > koni::Numeric::SECONDS_1_F) {

			//スキルのエフェクト止める
			boss_monster->GetEnemySkillList()[boss_monster->GetEnemySkillIndex()]->SkillAnimationStop();

			//プレイヤーのステータスを取得する
			auto& PlayerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();

			//プレイヤーのHPが0になって居なかったら
			if (PlayerStatus.GetcurentHp() > 0) {

				tnl_sequence_.change(&BattleScene::seqChangeTurn);

				return false;
			}

		}

	}

	return false;

}

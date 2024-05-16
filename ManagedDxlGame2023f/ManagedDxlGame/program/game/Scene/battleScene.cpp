#include "battleScene.h"
#include"../Manager/SceneManager.h"
#include"mapScene.h"
#include"resultScene.h"
#include "../Object/Player.h"
#include"../Object/Enemy.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

BattleScene::BattleScene(tnl::Vector3 pos,std::vector<Enemy::EnemyStatus>enemy_array, int background ) : enemy_Array(enemy_array), background_(background), map_pos(pos)
{
	enemy = std::make_shared<Enemy>();
	battle_log = std::make_shared<BattleLog>();

	//インデックスを1～4のランダムな数字に設定する
	enemy_index = rand() % 5;

	//新しい配列を生成する
	auto& enemy_ = enemy_Array[enemy_index];

	//敵のグラフィックハンドルを読み込む
	Enemy_ghdl = ResourceManager::getResourceManager()->LoadGraphEX(enemy_.getEnemyGhdl().c_str());

	enemy->InitEnemyItemDrop(enemy_index);

	//メニューの初期化
	InitMenuWindow();

	//武器のタイプを取得する
	SetWeaponType();

	//バトルフラグを切り替える(バトルのみでつかるアイテムの為)
	GameManager::getGameManager()->getInventory()->BattleFlagSwitch(0);

	//バトルログをセットする
	GameManager::getGameManager()->getInventory()->SetBattleLog(battle_log);

	//通常攻撃
	nomal_attack = std::make_shared<Nomal_Attack>(WeaponType);
}

//デストラクタ
BattleScene::~BattleScene()
{
	auto& enemy = enemy_Array[enemy_index];

	//サウンドを消去する
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/levelup.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/BGM/sentou.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/dageki_2.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/nigeru.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/syouri.mp3");

	//画像を消去する
	ResourceManager::getResourceManager()->deleteGraphEx(enemy.getEnemyGhdl().c_str());
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

	Select_Action_Menu = std::make_shared<MenuWindow>(50, 510, 250, 200, "graphics/WindowBase_01.png", select_action_coment, 3, 1);
	Select_Action_Menu->Open();

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

	select_use_window = std::make_shared<MenuWindow>(550, 510, 250, 200, "graphics/WindowBase_01.png", select_detail, 2, 1);
	select_use_window->Open();

	//バトルログのウィンドウ
	BattleLogMenu = std::make_shared<Menu>(670, 30, 600, 180, "graphics/WindowBase_01.png");
	//プレイヤーのウィンドウ
	playerStatus_window = std::make_shared<Menu>(50, 50, 250, 240, "graphics/WindowBase_01.png");
	//アイテムのウィンドウ
	item_window = std::make_shared<Menu>(300, 510, 250, 200, "graphics/WindowBase_01.png");
	//スキルウィンドウ
	skill_menu_window = std::make_shared<Menu>(300, 510, 250, 200, "graphics/WindowBase_01.png");

}

//武器のタイプをセットする
void BattleScene::SetWeaponType()
{
	//武器が装備されていたら
	if (!GameManager::getGameManager()->getInventory()->getEquipArray().empty()) {

		auto it = GameManager::getGameManager()->getInventory()->getEquipArray().begin();

		//武器のタイプを取得する
		WeaponType = (*it).getItemWeapontype();

	}
	//装備されていなかったら0にする
	else {
		WeaponType = 0;
	}

}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void BattleScene::Update(float delta_time)
{
	//シーケンスの更新処理
	sequence_.update(delta_time);

	//アニメーションの更新処理
	nomal_attack->SkillAnimationUpdate(delta_time);
	//アニメーション配列が無い時は処理を飛ばす
	if (GameManager::getGameManager()->getInventory()->getSkillNum() == 0) {
		return;
	}
	//アニメーションの更新処理
	GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillAnimationUpdate(delta_time);
	
}

//プレイヤーのターン時の更新処理
void BattleScene::PlayerUpdate()
{
	//プレイヤーのステータスを取得する
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	//敵のステータスを取得する
	auto& enemyStatus = enemy_Array[enemy_index];

	//プレイヤーのアクションの時
	if (select_sequence == Sequence::PlayerAction) {
		//メニューの更新処理
		MenuUpdate(PlayerStatus, enemyStatus);
	}
}

//------------------------------------------------------------------------------------------------------------------------
//描画
void BattleScene::Draw()
{
	//背景画像
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, background_, TRUE);

	//バトルログのウィンドウの描画
	BattleLogMenu->Menu_draw();


	//バトルウィンドウの描画
	BattleDraw();
	//バトルログの文字の描画
	battle_log->drawLogs();

	//プレイヤーのステータスの描画
	PlayerStatusDraw();

	/*DrawStringEx(10, 10, -1, "バトル画面");*/

	//敵の画像の描画
	if (!enemy_annihilation_flag) {
		DrawRotaGraph(600, 320, 0.8f, 0, Enemy_ghdl, true);
	}
	
	//アニメーションの描画
	nomal_attack->SkillAnimationDraw();

	DrawStringEx(360, 200, -1, "攻撃力\n%d", GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().getAttack());

	//アニメーション配列が無い時は処理を飛ばす
	if (GameManager::getGameManager()->getInventory()->getSkillNum() == 0) {
		return;
	}

	//特技のアニメーションの描画
	GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillAnimationDraw();

}

//バトルウィンドウの描画
void BattleScene::BattleDraw()
{
	//最初の行動の時のウィンドウ
	if (select_action_menu == MenuAction::first_action) {
		Select_Action_Menu->All();
	}
	//攻撃ウィンドウ
	else if (select_action_menu == MenuAction::fight_action) {
		attack_window->All();
	}
	//アイテム表示のウィンドウ
	else if (select_action_menu == MenuAction::item_action) {
		Select_Action_Menu->All();
		Select_Action_Menu->SetSelectCousourMove(False);
		item_window->Menu_draw();
		InventoryItemDraw();
	}
	//アイテムを使う時のウィンドウ
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
//アイテム関連


//アイテムの描画
void BattleScene::InventoryItemDraw()
{
	//インベントリのアイテムを2行ずつ表示する
	for (int i = 0; i < itemPerPage; ++i) {
		int itemIndex = currentPage * itemPerPage + i;
		if (itemIndex >= GameManager::getGameManager()->getInventory()->GetInventoryList().size()) {

			break;//インベントリの範囲外だったら終了する

		}

		auto& InventoryData = GameManager::getGameManager()->getInventory()->GetInventoryList()[itemIndex];
		std::string itemName = InventoryData.getItemName();

		//アイテムの描画
		DrawStringEx(350, StartY + i * lineHeight, Color_White, "%s", itemName.c_str());

		// 選択されたアイテムのインデックスに基づいてカーソルを描画
		if (itemIndex == selectedIndex) {
			// カーソルを描画する位置を計算
			int cursorY = StartY + i * lineHeight;
			ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", 330, cursorY, cursourSize, Color_Black, true);
		}
	}

}

//アイテムの選択処理
void BattleScene::ItemSelectProcess()
{
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		select_action_menu = MenuAction::item_use_action;
	}
	// 上キーが押されたときの処理
	//一番上にカーソルがいた場合それ以上にいかないようにする
	if (currentPage == 0 && selectedIndex == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// 何もしない
		return;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selectedIndex % itemPerPage == 0) {
			// カーソルがページ内の最上部にいる場合は、一つ上のページの最後の要素を選択
			if (currentPage > 0) {
				--currentPage;
				// インデックスを更新してページの最後の要素を選択する
				selectedIndex = (currentPage + 1) * itemPerPage - 1;
			}
		}
		else {
			// それ以外の場合は、一つ上の要素を選択
			--selectedIndex;
		}
	}

	// 下キーが押されたときの処理
	if (selectedIndex == GameManager::getGameManager()->getInventory()->GetInventoryList().size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN))return;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selectedIndex + 1) % itemPerPage == 0) {
			// カーソルがページ内の最下部にいる場合は、一つ下のページの最初の要素を選択
			//今の所プレイヤーが持てるインベントリは20で考えている為4ページ分まで開けるようにする
			if (currentPage < 4) {
				++currentPage;
				// インデックスを更新してページの最初の要素を選択する
				selectedIndex = currentPage * itemPerPage;
			}
		}
		else {
			// それ以外の場合は、一つ下の要素を選択
			++selectedIndex;
		}
	}

	// selectedIndexが有効な範囲内にあるか確認して、選択されたアイテムのIDを取得する
	if (selectedIndex >= 0 && selectedIndex < GameManager::getGameManager()->getInventory()->InventoryList.size()) {
		ItemBase selectedItem = GameManager::getGameManager()->getInventory()->GetInventoryList()[selectedIndex];
		selectedItemId = selectedItem.getItemId();
	}

	//アイテムのカーソル処理
	GameManager::getGameManager()->getInventory()->CusorMove();

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//行動を移す
		select_action_menu = MenuAction::first_action;
		//メニューを切り替える
		Select_Action_Menu->SetSelectCousourMove(True);
	}

}

//アイテムを使った際の処理
void BattleScene::ItemUseProcess()
{
	if (select_use_window->getSelectNum() == ItemUse && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//アイテムを使った時の処理
		GameManager::getGameManager()->getInventory()->InventoryItemUse(selectedItemId);
		//行動を移す
		select_action_menu = MenuAction::first_action;
		//カーソルを動けるようにする
		Select_Action_Menu->SetSelectCousourMove(True);
		//シーケンスを切り替える
		sequence_.change(&BattleScene::seqChangeScene);
		PlayerTurn = false;
	}
	//閉じるが押されたら
	else if (select_use_window->getSelectNum() == ItemUseMenuClose && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
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

	//0から1の一様乱数を生成する
	int rand_val = dis(gen);

	//敵のアイテム配列を取得する
	auto& EnemyItemDrop = enemy->getItemDropArray();

	//通常ドロップ
	if (rand_val < NomalDrop) {
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

	//レアドロップ
	else if (rand_val < RareDrop) {
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

//------------------------------------------------------------------------------------------------------------------------
//スキル関連

//スキルが選択された時の処理
void BattleScene::SkillSelectProcess()
{
	//Enterキーを押された時に
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		//スキルが何もない場合には処理を行わない
		if (GameManager::getGameManager()->getInventory()->getSkillNum() <= 0) {
			return;
		}
		//攻撃する
		SkillUseFlag = true;
	}

	// 上キーが押されたときの処理
	if (SkillCurentPage == 0 && selectedSkillIndex == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		// 何もしない
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		if (selectedSkillIndex % itemPerPage == 0) {
			// カーソルがページ内の最上部にいる場合は、一つ上のページの最後の要素を選択
			if (SkillCurentPage > 0) {
				--SkillCurentPage;
				// インデックスを更新してページの最後の要素を選択する
				selectedSkillIndex = (SkillCurentPage + 1) * itemPerPage - 1;
			}
		}
		else {
			// それ以外の場合は、一つ上の要素を選択
			--selectedSkillIndex;
		}
	}

	// 下キーが押されたときの処理
	if (selectedSkillIndex == GameManager::getGameManager()->getInventory()->getSkillList().size() - 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		// 何もしない
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		if ((selectedSkillIndex + 1) % itemPerPage == 0) {
			// カーソルがページ内の最下部にいる場合は、一つ下のページの最初の要素を選択
			// スキルが持てるインベントリは 20 で考えている為 4 ページ分まで開けるようにする
			if (SkillCurentPage < 4) {
				++SkillCurentPage;
				// インデックスを更新してページの最初の要素を選択する
				selectedSkillIndex = currentPage * itemPerPage;
			}
		}
		else {
			// それ以外の場合は、一つ下の要素を選択
			++selectedSkillIndex;
		}
	}

	// selectedSkillIndex が有効な範囲内にあるか確認して、選択されたスキルの ID を取得する
	if (selectedSkillIndex >= 0 && selectedSkillIndex < GameManager::getGameManager()->getInventory()->getSkillList().size()) {
		Shared<Skill> selectedSkill = GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex];
		selectedSkillId = selectedSkill->getId();
	}

	//特技のカーソル移動
	GameManager::getGameManager()->getInventory()->SkillCousorMove();

	//左を押したらメニューを戻す
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		//効果音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//行動を移す
		select_action_menu = MenuAction::fight_action;
		//メニューを切り替える
		attack_window->SetSelectCousourMove(True);
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
			if (skillIndex == selectedSkillIndex) {
				// カーソルを描画する位置を計算
				int cursorY = StartY + i * lineHeight;
				ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/cur_sannkaku2.png", 330, cursorY, cursourSize, 0, true);
			}
		}
	}
}

//スキルが使えるかどうかのチェック
void BattleScene::SkillUseMpChack(Player::PlayerStatus& playerStatus)
{
	if (playerStatus.getCurentMp() < GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getSkillConsumeMp()) {
		battle_log->addLog("Mpが足りません");
		Skill_Chaeck_Flag = false;
		SkillUseFlag = false;
		return;
	}
	GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillMpConsume(playerStatus);
	Skill_Chaeck_Flag = true;
}

//スキルを使用した際の処理
void BattleScene::SkillUseProcess(Player::PlayerStatus& playerStatus,Enemy::EnemyStatus& enemyStatus_)
{
	auto enemyHp = enemyStatus_.getEnemyHp();

	if (SkillUseFlag) {
		//スキルが使えるかどうかのチェックを行う
		SkillUseMpChack(playerStatus);
		//スキルが使えなったら処理を飛ばす
		if (!Skill_Chaeck_Flag) {
			select_action_menu = MenuAction::fight_action;
			return;
		}
		//攻撃系のスキルの場合
		if (GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getSkillType() == 0) {
			if (GameManager::getGameManager()->getInventory()->getSkillNum() > 0) {
				//スキルのアニメーションを流す
				GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUseAnimation();
				//特技のアニメーションを再生する
				Skill_Attack_Flag = true;
			}
			//ダメージを計算する
			SkillDamage = GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUse(playerStatus, enemyStatus_);
			enemyStatus_.SetEnemyHp(enemyHp - SkillDamage);
			battle_log->addSkillUseLog("Player", GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getName().c_str(), "Enemy", SkillDamage);
		}
		//バフ系のスキルの場合
		else if(GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getSkillType() == 1)  {
			Skill_Buff_Flag = true;
			//バフ効果を格納する
			int buff = GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUse(playerStatus);
			//音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);
			//ログを流す
			battle_log->addRecoveryLog("Player", GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getName(), buff);
		}
		//アイテム系の場合
		else {
			if (GameManager::getGameManager()->getInventory()->getSkillNum() > 0) {
				//スキルのアニメーションを流す
				GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUseAnimation();
				//特技のアニメーションを再生する
				Skill_Attack_Flag = true;
			}
			//ダメージを計算する
			SkillDamage = GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillUse(enemyStatus_);
			enemyStatus_.SetEnemyHp(enemyHp - SkillDamage);
			battle_log->addSkillUseLog("Player", GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->getName().c_str(), "Enemy", SkillDamage);
		}

		// エネミーの体力が0以下になったら、死亡演出 + レベルアップ判定
		if (enemyStatus_.getEnemyHp() <= 0) {
			deadEnemyProcess(playerStatus, enemyStatus_);
			levelUpProcess(playerStatus);
		}

		//フラグを切り替える
		SkillUseFlag = false;
		PlayerTurn = false;

		//スキル選択ウィンドウに戻す
		select_action_menu = MenuAction::fight_action;
		//カーソルを動けるようにする
		attack_window->SetSelectCousourMove(True);

	}
}

//------------------------------------------------------------------------------------------------------------------------
//メニュー関連 

//逃げるを選択した際の処理
void BattleScene::FleeProcess(float delta_time)
{
	if (flee_flag) {
		//逃げる時のSEを流す
		if (GameManager::getGameManager()->TimeCount(delta_time, flee_Time)) {
			auto mgr = SceneManager::GetInstance();
			//シーンを切り替える
			mgr->changeScene(new MapScene(map_pos));
			//サウンドを止める
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		}
	}
}

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

		//攻撃
		if (Select_Action_Menu->getSelectNum() == Attack && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::fight_action;

		}
		//道具
		else if (Select_Action_Menu->getSelectNum() == Item && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			select_action_menu = MenuAction::item_action;
		}
		//逃げる
		else if (Select_Action_Menu->getSelectNum() == Flee && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//プレイヤーと敵の素早さを取得する
			auto playerSpeed = playerStatus.getSpeed();
			auto enemySpeed = enemyStatus_.getEnemySpeed();

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
			}
			else {
				// 逃走に失敗した場合の処理
				battle_log->addLog("逃げられなかった");
				flee_flag = false;
				sequence_.change(&BattleScene::seqChangeScene);
				PlayerTurn = false;
			}
		}
		break;

		//攻撃選択の時に
	case BattleScene::MenuAction::fight_action:

		//攻撃が押された時に
		if (attack_window->getSelectNum() == Attack && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//攻撃した時のアニメーション
			Nomal_Attack_Flag = true;

			//決定音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

			// ダメージを計算し、ダメージが 0 以下ならば、1にする ( 最低でも1ダメージは与える
			int damage = nomal_attack->SkillUse(playerStatus, enemyStatus_);

			// 敵のHPを減らす
			enemyStatus_.SetEnemyHp(EnemyHp - damage);

			// 戦闘ログにダメージ結果を出力
			battle_log->addDamageLog("Player", "Enemy", damage);

			// エネミーの体力が0以下になったら、死亡演出 + レベルアップ判定
			if (enemyStatus_.getEnemyHp() <= 0) {
				deadEnemyProcess(playerStatus, enemyStatus_);
				levelUpProcess(playerStatus);
			}
			// そうでないなら、普通にアニメーション続行
			nomal_attack->SkillUseAnimation();
			PlayerTurn = false;
		}
		//特技が選択されたら
		else if (attack_window->getSelectNum() == SpecialSkill && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			select_action_menu = MenuAction::skill_action;
		}
		//閉じるが選択されたら
		else if (attack_window->getSelectNum() == MenuClose && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			select_action_menu = MenuAction::first_action;

		}

		break;

	case BattleScene::MenuAction::item_action:

		//アイテム選択処理
		ItemSelectProcess();

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
	//スキルが使えなったら処理を飛ばす
	if (!Skill_Chaeck_Flag) {
		Skill_Attack_Flag = false;
		return;
	}
	//アニメーションを流しきってからターンを入れ替える
	if (Nomal_Attack_Flag && enemyhp > 0 || Skill_Attack_Flag && enemyhp > 0) {
		//アニメーションを1秒間を流す
		if (GameManager::getGameManager()->TimeCount(delta_time, Animation_Time)) {
			//ターンを切り替える
			sequence_.change(&BattleScene::seqChangeScene);
		}
	}
	//バフ系のスキルを使った場合はターンを入れ替える
	else if (Skill_Buff_Flag) {
		//ターンを切り替える
		sequence_.change(&BattleScene::seqChangeScene);
	}
}

//レベルアップ
void BattleScene::levelUpProcess(Player::PlayerStatus& playerStatus) {

	//レベルを取得する
	auto Playerlevel = playerStatus.getLevel();

	//レベル10以上であればレベルアップしない(今後増やして行く予定)
	if (Playerlevel >= 10) {
		return;
	}

	//プレイヤーの必要な経験値が0以下になった時にレベルをあげる
	if (playerStatus.getExpoint() <= 0)
	{
		//レベルをあげる
		levelUp_flag = true;
		DeadEnemy_flag = true;
	}
	//必要な経験値が0にならなかったら
	else {
		DeadEnemy_flag = true;
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

//敵のHPが0になった時の処理
void BattleScene::deadEnemyProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_) {

	//必要な経験値を取得する
	auto R_point = playerStatus.getExpoint();

	//敵の経験値を取得する
	auto Enemy_expoint = enemyStatus_.getEnemyExpoint();

	// データ処理
	playerStatus.SetPlayerExpoint(R_point - Enemy_expoint);

	//所持金を追加する
	GameManager::getGameManager()->getPlayer()->AddPlayerMoney(enemyStatus_.getEnemyMoney());
}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連(主にターン制御)

//最初の素早さを比較する
bool BattleScene::seqIdle(float delta_time)
{
	//ステータスを取得する
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy_Array[enemy_index];

	//シーケンスの初期化
	if (sequence_.isStart()) {
		//素早さを比較する
		//プレイヤーが早い場合
		if (PlayerStatus.getSpeed() >= enemyStatus.getEnemySpeed()) {
			//プレイヤーのシーケンスへ
			sequence_.change(&BattleScene::seqPlayerAction);
			PlayerTurn = true;
		}
		//エネミーが早い場合
		else {

			//エネミーのシーケンスへ
			sequence_.change(&BattleScene::seqEnemyAction);

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
	auto& enemyStatus = enemy_Array[enemy_index];
	auto enemyHp = enemyStatus.getEnemyHp();

	if (sequence_.isStart()) {
		select_sequence = Sequence::PlayerAction;

		//ログをリセットする
		battle_log->LogReset();

		//ログを流す
		battle_log->addLog("[ プレイヤーのターン ]");

	}

	if (PlayerTurn) {
		//プレイヤーの更新処理
		PlayerUpdate();
	}

	//敵を倒したら1秒間待ってからアニメーションを止める為のフラグ
	if (DeadEnemy_flag) {

		if (GameManager::getGameManager()->TimeCount(delta_time, 1)) {
			//シーンを遷移させる為のシーケンス
			sequence_.change(&BattleScene::seqChangeScene);
		}
	}

	//スキルを使用した際の処理
	SkillUseProcess(playerStatus, enemyStatus);

	//アニメーションを使用時にアニメーションを流してからターンをチェンジさせる
	AnimationTurnChange(enemyHp, delta_time);

	//逃げる
	FleeProcess(delta_time);

	return true;

}

//敵のターン
bool BattleScene::seqEnemyAction(float delta_time)
{
	auto& PlayerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();
	auto& enemyStatus = enemy_Array[enemy_index];
	auto enemyAttack = enemyStatus.getEnemyAttack();
	auto PlayerDefance = PlayerStatus.getDefance();
	auto PlayerHp = PlayerStatus.getcurentHp();

	//シーケンスが始まった最初の一回のみ処理を行う
	if (sequence_.isStart()) {

		//アニメーションを止める
		if (Nomal_Attack_Flag) {
			nomal_attack->SkillAnimationStop();
			Nomal_Attack_Flag = false;
		}
		else if (Skill_Attack_Flag) {
			GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillAnimationStop();
			Skill_Attack_Flag = false;
		}
		//バフスキルのフラグを切り替える
		else if (Skill_Buff_Flag) {
			Skill_Buff_Flag = false;
		}

		select_sequence = Sequence::EnemyAction;

		//ログをリセットする
		battle_log->LogReset();

		//ログを流す
		battle_log->addLog("[ 敵のターン ]");

		//ダメージを計算する「
		int damage = enemyAttack - PlayerDefance;

		//ダメージが0を下回るときに0に設定する
		if (damage <= 0) { damage = 0; }

		//プレイヤーのHpを減らす
		PlayerStatus.SetPlayerCurentHp(PlayerHp - damage);

		//バトルログを流す
		battle_log->addDamagePlayerLog("Enemy", "Player", damage);

		//SEを流す
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/dageki_2.mp3", DX_PLAYTYPE_BACK);

		//ボリュームを変える
		SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/dageki_2.mp3");

		//プレイヤーのHpが0になったら全滅のシーンに切り替える
		if (PlayerStatus.getcurentHp() <= 0) {

			//ログを流す
			battle_log->addLog("全滅した");

			//プレイヤーのHpを0にする
			PlayerStatus.SetPlayerCurentHp(0);

			//BGMを止める
			SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/zenmetu.mp3", DX_PLAYTYPE_BACK);

			//シーンを遷移させる
			annihilation_flag = true;
		}
	}
	//全滅した時
	if (annihilation_flag) {
		if (GameManager::getGameManager()->TimeCount(delta_time, annihilation_Time)) {
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new ResultScene());
			annihilation_flag = false;
		}
	}
	//Hpが0にならなかったらターンを切り替える
	else if(/*SystemManager::getSystemManager()->TimeCount(delta_time, Change_Turn_Time)*/tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		sequence_.change(&BattleScene::seqPlayerAction);
		select_sequence = Sequence::PlayerAction;
		PlayerTurn = true;
	}


	return true;
}

//シーンを遷移させる為のシーケンス
bool BattleScene::seqChangeScene(float delta_time)
{
	//敵のステータス
	auto& enemyStatus = enemy_Array[enemy_index];
	//プレイヤーのステータス
	auto& playerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

	if (sequence_.isStart()) {
		// 演出
		//アニメーションをとめる
		if (Nomal_Attack_Flag) {
			nomal_attack->SkillAnimationStop();
		}
		else if (Skill_Attack_Flag) {
			GameManager::getGameManager()->getInventory()->getSkillList()[selectedSkillIndex]->SkillAnimationStop();
		}

		//敵を倒していた場合
		if (DeadEnemy_flag) {

			//ログを流す
			battle_log->addLog("敵を倒した！");

			//敵の画像を消す
			enemy_annihilation_flag = true;

			//アイテムのドロップ処理
			ItemDropProcess();

			//レベルアップしていた場合
			if (levelUp_flag) {

			//効果音を鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/levelup.mp3", DX_PLAYTYPE_BACK);

			//ログを出す
			battle_log->addLog("レベルアップした！");

			//新しいレベルを取得する
			auto Playerlevel = playerStatus.getLevel() + 1;

			//プレイヤーのステータスをセットする
			GameManager::getGameManager()->getPlayer()->SetPlayerStatus(Playerlevel);

			//プレイヤーが武器を装備していたら
			if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack) != 0) {
				auto playerAttack = playerStatus.getAttack();
				playerStatus.SetPlayerAttack(playerAttack + GameManager::getGameManager()->getInventory()->getEquipStatus(equipAttack));
			}

			//プレイヤーが防具を装備していたら
			if (GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance) != 0) {
				auto playerDefance = playerStatus.getDefance();
				playerStatus.SetPlayerDefance(playerDefance + GameManager::getGameManager()->getInventory()->getEquipStatus(equipDefance));
			}

			//スキルをセットする
			GameManager::getGameManager()->getInventory()->SkillSet();
				
		    }
			else {
				//勝利音を鳴らす
				SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/syouri.mp3", DX_PLAYTYPE_BACK);
				SoundManager::getSoundManager()->ChangeSoundVolume(60, "sound/SoundEffect/syouri.mp3");
			}

			//もし一時的に攻撃力が上がっていたらそれをリセットする
			BattleBuffResetProcess();
			
			//バトルフラグを切り替える
			GameManager::getGameManager()->getInventory()->BattleFlagSwitch(1);

			//ゴールドを手に入れた時のログを流す
			battle_log->addMoney("プレイヤー", enemyStatus.getEnemyMoney());

		}
	}
	//敵がやられていたらEnterキーを押して遷移させる
	if (DeadEnemy_flag) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new MapScene(map_pos));
		}
	}
	//敵がやられてない場合敵にターンを変更する
	else {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			sequence_.change(&BattleScene::seqEnemyAction);
			select_sequence = Sequence::EnemyAction;
		}
	}

	return false;
}
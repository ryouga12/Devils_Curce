#include "InMapScene.h"
#include"../Manager/SceneManager.h"
#include"battleScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"mapScene.h"
#include<iostream>
#include"../Manager/UiManager.h"
#include"../Manager/CsvManager.h"
#include"../Item/WeaponShop.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

//コンストラクタ
//引数 : 中に入った時の最初の座標 , 現在のシーン
InMapScene::InMapScene(tnl::Vector3 firstPos, InMapState inmap_state): first_pos(firstPos), curent_inmap_scene(inmap_state)
{
	//現在のシーンをInMapに設定
	curent_scene = SceneState::INMAP;
}

InMapScene::~InMapScene()
{

	//マップチップの解放(2096 = マップチップの総数)
	for (int i = 0; i < 2096; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}
}

//初期化
void InMapScene::InitVillageScene()
{
	//武器関連の初期化
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);

	//村長
	std::vector<std::string> herdman_npc_ghdl = { "graphics/Npc/herdman.png"  ,"graphics/Npc/herdman_right.png" ,"graphics/Npc/herdman_up.png" , "graphics/Npc/herdman_idle.png"};

	for (int i = 0; i < herdman_npc_ghdl.size(); i++) {

		herdman_hdl_pass[i] = herdman_npc_ghdl[i];
	}

	//武器商人
	std::vector<std::string> armsdealer_npc_ghdl = {"graphics/Npc/Armsdealer_left.png"  ,"graphics/Npc/Armsdealer_right.png" ,"graphics/Npc/Armsdealer_up.png"  ,"graphics/Npc/Armsdealer.png"};

	for (int i = 0; i < armsdealer_npc_ghdl.size(); i++) {

		Armsdealer_hdl_pass[i] = armsdealer_npc_ghdl[i];
	}

	//女性村人
	std::vector<std::string> female_villagers_npc_ghdl = { "graphics/Npc/female_villagers_left.png"  ,"graphics/Npc/female_villagers_right.png" ,"graphics/Npc/female_villagers_up.png"  ,"graphics/Npc/female_villagers_idle.png" };

	for (int i = 0; i < armsdealer_npc_ghdl.size(); i++) {

		female_resident_hdl_pass[i] = female_villagers_npc_ghdl[i];
	}

	//男性村人
	std::vector<std::string> male_villagers_npc_ghdl = { "graphics/Npc/male_villagers_left.png"  ,"graphics/Npc/male_villagers_right.png" ,"graphics/Npc/male_villagers_up.png"  ,"graphics/Npc/male_villagers_idle.png" };

	for (int i = 0; i < armsdealer_npc_ghdl.size(); i++) {

		resident_male_hdl_pass[i] = male_villagers_npc_ghdl[i];
	}


	//NPCの初期化
	herdman_npc = std::make_shared<Npc>("村長", tnl::Vector3(1375, 760, 0),48 , herdman_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_LEFT, 4 ,"graphics/Npc/herdman_icon.png" );
	armsdealer_npc = std::make_shared<Npc>("武器商人", tnl::Vector3(1150, 140, 0), 48 , Armsdealer_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4 ,"graphics/Npc/Armsdealer_icon.png" );
	priest_npc = std::make_shared<Npc>("神官", tnl::Vector3(239, 740, 0),48 , Priest_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4 , "graphics/Npc/Priest_icon.png");
	female_resident_npc = std::make_shared<Npc>("女性村人", tnl::Vector3(585, 667, 0), 48, female_resident_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_RIGHT, 4, "graphics/Npc/female_villagers_icon.png");
	resident_male_npc = std::make_shared<Npc>("男性村人", tnl::Vector3(1475, 360, 0), 48, resident_male_hdl_pass, NPC_SCALE, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4, "graphics/Npc/male_villagers_icon.png");


	//アクターリストにNPCを追加する
	actor_list.emplace_back(armsdealer_npc);
	actor_list.emplace_back(herdman_npc);
	actor_list.emplace_back(priest_npc);
	actor_list.emplace_back(female_resident_npc);
	actor_list.emplace_back(resident_male_npc);

	//soundManagerの初期化
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tasogare.mp3", DX_PLAYTYPE_LOOP);

	//マップチップのロード
	//村用のマップチップをロードする
	mapChip_Loding(CsvManager::getCsvManager()->GetVillagMapCsv(), CsvManager::getCsvManager()->GetVillagMapObjectCsv());

}

//ダンジョンシーン(魔王の城)の初期化
void InMapScene::InitDungeonScene()
{
	//ボス
	boss_npc = std::make_shared<Npc>("魔王", tnl::Vector3(801, 153, 0), 128 , boss_hdl_pass, 1, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4, "graphics/Npc/herdman_icon.png");
	//アクターリストにNPCを追加する
	actor_list.emplace_back(boss_npc);

	//マップチップのロード
	//ダンジョン用のマップチップをロードする
	mapChip_Loding(CsvManager::getCsvManager()->GetBossCastleFloorCsv() , CsvManager::getCsvManager()->GetBossCastleObjectCsv());

	//魔王の城内での音楽
	SoundManager::getSoundManager()->sound_Play("sound/BGM/maou_bgm_castle.mp3",DX_PLAYTYPE_LOOP);
}

//町シーンの初期化
void InMapScene::InitTownScene()
{
	//武器関連の初期化
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);

	//武器商人の画像を初期化する
	std::vector<std::string> armsdealer_npc_ghdl = { "graphics/Npc/Armsdealer_man_town_left.png"  ,"graphics/Npc/Armsdealer_man_town_right.png","graphics/Npc/Armsdealer_man_town_up.png", "graphics/Npc/Armsdealer_man_town.png" };

	for (int i = 0; i < armsdealer_npc_ghdl.size(); i++) {

		Armsdealer_hdl_pass[i] = armsdealer_npc_ghdl[i];
	}

	//町長の画像を初期化する
	std::vector<std::string> npc_ghdl = {"graphics/Npc/town_herdman_left.png"  ,"graphics/Npc/town_herdman_right.png","graphics/Npc/town_herdman_up.png","graphics/Npc/town_herdman_idle.png" };
	
	for (int i = 0; i < npc_ghdl.size(); i++) {

		herdman_hdl_pass[i] = npc_ghdl[i];
	}

	//宿屋の画像を初期化する
	std::vector<std::string> inn_npc_ghdl = {"graphics/Npc/inn_left.png" ,"graphics/Npc/inn_right.png","graphics/Npc/inn_up.png","graphics/Npc/town_inn_.png" };

	for (int i = 0; i < inn_npc_ghdl.size(); i++) {

		inn_hdl_pass[i] = inn_npc_ghdl[i];
	}

	//魔物の画像を初期化する
	std::vector<std::string> enemy_npc_ghdl = { "graphics/Npc/enemy_1_left.png" ,"graphics/Npc/enemy_1_right.png","graphics/Npc/enemy_1_up.png","graphics/Npc/enemy_1_idle.png" };

	for (int i = 0; i < enemy_npc_ghdl.size(); i++) {

		enemy_npc_pass[i] = enemy_npc_ghdl[i];
	}

	//---npcの初期化---//

	//武器商人
	armsdealer_npc = std::make_shared<Npc>("武器商人", tnl::Vector3(1730, 580, 0), 48, Armsdealer_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4 ,"graphics/Npc/Armsdealer_man_icon_.png" );

	//町長
	herdman_npc = std::make_shared<Npc>("町長", tnl::Vector3(220, 266, 0), 48, herdman_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_UP, 4 ,"graphics/Npc/town_herdman_icon.png");

	//宿屋
	inn_npc = std::make_shared<Npc>("宿屋", tnl::Vector3(780, 1062, 0), 48, inn_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4 ,"graphics/Npc/inn_icon.png");

	//敵
	//イベントフラグがtrueだったら生成させる
	if (EventManager::getEventManager()->GetEnemyEventFlag()) {
		enemy_npc = std::make_shared<Npc>("魔物", tnl::Vector3(2032, 205, 0), 48, enemy_npc_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_UP, 4, "graphics/Npc/enemy_1_icon.png");

		actor_list.emplace_back(enemy_npc);
	}

	//アクターリストにNPCを追加する
	actor_list.emplace_back(armsdealer_npc);
	actor_list.emplace_back(herdman_npc);
	actor_list.emplace_back(inn_npc);


	//soundManagerの初期化
	//町のBGMを流す
	SoundManager::getSoundManager()->sound_Play("sound/BGM/town_bgm.mp3", DX_PLAYTYPE_LOOP);

	//マップチップのロード
	mapChip_Loding(CsvManager::getCsvManager()->GetTownMapCsv(), CsvManager::getCsvManager()->GetTownObjectMapCsv());
}

//城下町シーンの初期化
void InMapScene::InitCastleTownScene()
{
	auto mgr = SceneManager::GetInstance();

	//シーンマネージャーのシーンフラグがfalseの場合のみ
	if(!mgr->GetSceneFlag()){

		//城シーンから以降した時に一度マップチップを空にする
		MapChips.clear();
		MapChip_object.clear();

		//プレイヤーの位置を変える
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(CASTLE_EXIT_POS);

		//カメラ位置も変更する
		GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());
		GameManager::getGameManager()->getCamera()->SetTargetPos();
	}

	//soundManagerの初期化
	//城下町のBGMを流す
	SoundManager::getSoundManager()->sound_Play("sound/BGM/castle_town_bgm.mp3", DX_PLAYTYPE_LOOP);

	//マップチップのロード
	mapChip_Loding(CsvManager::getCsvManager()->GetCastleTownMapCsv(), CsvManager::getCsvManager()->GetCastleTownObjectMapCsv());
}

//城シーンの初期化
void InMapScene::InitCastleScene()
{
	MapChips.clear();
	MapChip_object.clear();
	actor_list.clear();

	//プレイヤーの位置を変える
	GameManager::getGameManager()->getPlayer()->SetPlayerPosition(CASTLE_FIRST_POS);

	//カメラ位置も変更する
	GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());
	GameManager::getGameManager()->getCamera()->SetTargetPos();

	//王
	if (!king_npc) {
		king_npc = std::make_shared<Npc>("王", tnl::Vector3(944, 315, 0), 48, king_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4, "graphics/Npc/king_icon.png");
	}
	//王妃
	if (!queen_npc) {
		queen_npc = std::make_shared<Npc>("王妃", tnl::Vector3(1261, 315, 0), 48, queen_hdl_pass, 1.5f, Npc::NPC_ANIMATION::NPC_DIR_DOWN, 4, "graphics/Npc/queen_icon.png");
	}

	//アクターリストにNPCを追加する
	actor_list.emplace_back(king_npc);
	actor_list.emplace_back(queen_npc);

	//マップチップのロード
	mapChip_Loding(CsvManager::getCsvManager()->GetCastleMapCsv(), CsvManager::getCsvManager()->GetCastleObjectMapCsv());
}


void InMapScene::InitInMapScene()
{
	//Playerの初期位置を決める
	GameManager::getGameManager()->getPlayer()->SetPlayerPosition(first_pos);

	//メニューの初期化
	InitMenuWindow();

	//マップチップの初期化
	if (!mapchip) {
		mapchip = std::make_shared<MapChip>();
	}

	switch (curent_inmap_scene)
	{
	//村
	case InMapScene::InMapState::VILLAGE:


		//初期化処理
		InitVillageScene();

		//マップの高さとマップの幅を決める
		map_height = 945;																					
		map_width = 1600;																					

		break;

	//町
	case InMapScene::InMapState::TOWN:

		//マップの高さとマップの幅を決める
		map_height = 1400;
		map_width = 2240;

		//町シーンの初期化
		InitTownScene();

		break;

	//城下町
	case InMapScene::InMapState::CASTLETOWN:

		//マップの高さとマップの幅を決める
		map_height = 1008;
		map_width = 2240;

		//城下町シーンの初期化
		InitCastleTownScene();

		break;

	//城
	case InMapScene::InMapState::CASTLE:

		//マップの高さとマップの幅を決める
		map_height = 1740;
		map_width = 2240;

		//城シーンの初期化
		InitCastleScene();

		break;

	//魔王の城
	case InMapScene::InMapState::BOSSCASTLE:

		//マップの高さとマップの幅を決める
		map_height = 940;
		map_width = 1600;

		//ダンジョンシーンの初期化
		InitDungeonScene();

		break;


	default:
		break;
	}

	//イベント関連の初期化
	EventManager::getEventManager()->InitEventRelated(actor_list);

}

//---メニューの初期化---//
void InMapScene::InitMenuWindow()
{

	//通常のメニューウィンドウ
	menuWindow = UIManager::getUIManager()->getMenu("menu_window");

	//話すコマンド
	if (!dia_log_coment) {

		dia_log_coment = new MenuWindow::MenuElement_t[]{
			{100 , 100 , "話す" , 0},
			{100 , 150 , "閉じる" , 1}
		};
	}

	if (!dia_log_coment_select) {
		dia_log_coment_select = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", dia_log_coment, 2, 1);
		dia_log_coment_select->Open();
	}

}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void InMapScene::Update(float delta_time)
{
	InMapSceneUpdate(delta_time);

	__super::Update(delta_time);
}

//それぞれの更新処理
void InMapScene::InMapSceneUpdate(float delta_time)
{
	//シーケンス
	sequence_.update(delta_time);

	//Player
	GameManager::getGameManager()->getPlayer()->player_Move(delta_time, PlayerVelocity, map_height);

	//Inventory
	GameManager::getGameManager()->getInventory()->Update(delta_time);

	//UIManagerの更新処理
	//UIManager::getUIManager()->Update(delta_time);

	switch (curent_inmap_scene)
	{
	//村シーン
	case InMapScene::InMapState::VILLAGE:

		//npc
		for (auto& actor : actor_list) {
			actor->Update(delta_time);
		}


		break;

	//町シーン
	case InMapScene::InMapState::TOWN:

		//npc
		for (auto& actor : actor_list) {
			actor->Update(delta_time);
		}

		break;

	//城下町
	case InMapScene::InMapState::CASTLETOWN:


		break;

	//城シーン
	case InMapScene::InMapState::CASTLE:

		//npc
		for (auto& actor : actor_list) {
			actor->Update(delta_time);
		}

		break;

	//ボスの城シーン
	case InMapScene::InMapState::BOSSCASTLE:

		//npc
		for (auto& actor : actor_list) {
			actor->Update(delta_time);
		}

		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------------------------------------------------
//描画

void InMapScene::Draw()
{
	InMapSceneDraw();

	__super::Draw();
}

//それぞれの描画処理
void InMapScene::InMapSceneDraw()
{

	//---mapChipの描画---//
	for (auto& mapchip : MapChips) {
		mapchip->Draw(*GameManager::getGameManager()->getCamera());
	}

	//---mapChipの描画---//
	for (auto& mapchip : MapChip_object) {
		mapchip->Draw(*GameManager::getGameManager()->getCamera());
	}

	switch (curent_inmap_scene)
	{
	case InMapScene::InMapState::VILLAGE:

		//---Npcの描画---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		break;

	//町シーン
	case InMapScene::InMapState::TOWN:

		//---Npcの描画---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		break;

	//城下町
	case InMapScene::InMapState::CASTLETOWN:


		break;

	//城シーン
	case InMapScene::InMapState::CASTLE:

		//---Npcの描画---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		break;

	//ボスの城
	case InMapScene::InMapState::BOSSCASTLE:


		//---Npcの描画---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		/*DrawStringEx(100, 100, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().x);
		DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().y);*/


		break;


	default:


		break;
	}

	//---plyerの描画---//
	GameManager::getGameManager()->getPlayer()->player_draw(*GameManager::getGameManager()->getCamera(), PlyerScale);

	//インベントリの描画
	GameManager::getGameManager()->getInventory()->draw();

	//それぞれのシーケンスでの描画
	switchSequenceDraw();

	//UIManagerの描画(主にアイコンなど)
	//UIManager::getUIManager()->Draw();

	/*DrawStringEx(100, 100, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().x);
	DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().y);*/

}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

//シーケンスを切り替える
void InMapScene::SequenceChange(Sequence_num secect_sequence)
{
	//選択させるシーケンスによって他の場所でシーケンスを切り替える
	//主にイベントマネージャーなどでイベントを切り替える時に使用
	switch (secect_sequence)
	{

	//シーケンスを切り替える
	//プレイヤーが行動時
	case InMapScene::Sequence_num::MOVE:

		//通常に戻す
		sequence_.change(&InMapScene::seqMove);

		break;

	//武器屋
	case InMapScene::Sequence_num::WEAPONSHOP:

		sequence_.change(&InMapScene::seqWeaponShop);

		break;

	//メニューを開く
	case InMapScene::Sequence_num::MENUOPEN:

		sequence_.change(&InMapScene::seqMenuOpen);

		break;


	//npcと話す
	case InMapScene::Sequence_num::MENUNPC:

		//通常に戻す
		sequence_.change(&InMapScene::seqNpc);

		break;

	default:

		break;
	}
}

//初期化
bool InMapScene::seqInit(float delta_time)
{
	if (sequence_.isStart()) {

		select_num = Sequence_num::INIT;

		//初期化
		InitInMapScene();

		//シーケンスを切り替える
		sequence_.change(&InMapScene::seqMove);
	}

	return false;
}

bool InMapScene::seqMove(float delta_time)
{
	if (sequence_.isStart()) {

		if (!GameManager::getGameManager()->getPlayer()->getPlayerControl()) {
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
		}
		select_num = Sequence_num::MOVE;
	}

	//カメラの更新処理
	GameManager::getGameManager()->getCamera()->update(GameManager::getGameManager()->getPlayer()->getPlayerPos(), map_width, map_height);

	if (GameManager::getGameManager()->getInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {

		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//インベントリを表示する
		GameManager::getGameManager()->getInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
		//plyerを動けないようにする
		GameManager::getGameManager()->getPlayer()->setPlayerControl();
	}

	//当たり判定
	//インベントリを開いている時は当たり判定を消す
	if (GameManager::getGameManager()->getInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY) {
		mapChip_collision();
	}

	//プレイヤーの座標がマップ外に出たらシーンを切り替える
	//村シーンのみ塀がない為、上にいけないようにする
	if (curent_inmap_scene == InMapState::VILLAGE) {

		//プレイヤーのサイズ
		auto player_size = GameManager::getGameManager()->getPlayer()->GetPlayerSize();

		//プレイヤーの移動制限
		if (GameManager::getGameManager()->getPlayer()->getPlayerPos().y < static_cast<float>(player_size.y / 2)) {

			tnl::Vector3 player_pos = { GameManager::getGameManager()->getPlayer()->getPlayerPos().x ,static_cast<float>(player_size.y / 2) , 0 };

			GameManager::getGameManager()->getPlayer()->SetPlayerPosition(player_pos);
		}
	}
	//城下町シーンの場合上に行ったら城に移動させる
	else if (curent_inmap_scene == InMapState::CASTLETOWN && GameManager::getGameManager()->getPlayer()->getPlayerPos().y < 0) {

		curent_inmap_scene = InMapState::CASTLE;

		//初期化させて城シーンに移行させる
		sequence_.change(&InMapScene::seqInit);

		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

		auto mgr = SceneManager::GetInstance();

		if (mgr->GetSceneFlag()) {
			//フラグを切り替えてシーンを遷移させないようにする
			mgr->SceneFlagChange();
		}

		//フェードを入れる
		mgr->FadeScene(0.1f, 1.0f);

		//サウンドを止める
		SoundManager::getSoundManager()->StopSound("sound/BGM/castle_town_bgm.mp3");

		return false;

	}
	//城シーンの場合、マップ外に出たら城下町に遷移させる
	else if (GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= map_height && curent_inmap_scene == InMapState::CASTLE)
	{
		curent_inmap_scene = InMapState::CASTLETOWN;

		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

		auto mgr = SceneManager::GetInstance();

		if (mgr->GetSceneFlag()) {
			//フラグを切り替えてシーンを遷移させないようにする
			mgr->SceneFlagChange();
		}

		//フェードを入れる
		mgr->FadeScene(0.1f, 1.0f);

		//初期化させて城シーンに移行させる
		sequence_.change(&InMapScene::seqInit);

		return false;
	}
	if (GameManager::getGameManager()->getPlayer()->getPlayerPos().x >= map_width || GameManager::getGameManager()->getPlayer()->getPlayerPos().x < 0 || GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= map_height)
	{
		TimeCount = true;

		//村の場合
		if (curent_inmap_scene == InMapState::VILLAGE) {
			//音楽をStopする
			SoundManager::getSoundManager()->StopSound("sound/BGM/tasogare.mp3");
		}
		//町の場合
		else if (curent_inmap_scene == InMapState::TOWN) {
			//音楽をStopする
			SoundManager::getSoundManager()->StopSound("sound/BGM/town_bgm.mp3");
		}
		//魔王の城の場合
		else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
			//音楽をStopする
			SoundManager::getSoundManager()->StopSound("sound/BGM/maou_bgm_castle.mp3");
		}
		//城下町の場合
		else if (curent_inmap_scene == InMapState::CASTLETOWN) {
			//音楽をStopする
			SoundManager::getSoundManager()->StopSound("sound/BGM/castle_town_bgm.mp3");
		}

		//移動のSEを流す
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
	}
	//切り替える際にSEを流し切ってから遷移する
	if (TimeCount) {

		//0.2秒SEを流してから遷移させる
		if (GameManager::getGameManager()->TimeCount(delta_time, TimeSE)) {

			sequence_.change(&InMapScene::seqChangeScene);

			//シーンマネージャーのポインタを取得する
			auto mgr = SceneManager::GetInstance();

			//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
			if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

			//シーンを遷移させる
			mgr->changeScene(new MapScene(),0.3);
		}
	}
	
	
	return false;
}

//武器屋
bool InMapScene::seqWeaponShop(float delta_time)
{
	if (sequence_.isStart()) {

		select_num = Sequence_num::WEAPONSHOP;
		//毎回コメントを初期化する
		//コメントがかぶらないようにする為
		weapon_shop->buyComentClear();

		//Npcのコメントを切り替える
		EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
	}

	//武器屋の処理
	weapon_shop->BuyWeapon(GameManager::getGameManager()->getPlayer());
	weapon_shop->WeaponShopCursorMove();
	
	//0を押したらシーケンスを変える
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		//シーンを切り替える
		sequence_.change(&InMapScene::seqMove);
	}

	return false;
}

//シーンを変える時のフェードアウトを入れる為シーケンス
bool InMapScene::seqChangeScene(float delta_time)
{
	if (curent_inmap_scene == InMapState::VILLAGE) {
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(FIRST_WORLD_POS);
	}
	else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(BOSS_EXIT_POS);
	}
	else if (curent_inmap_scene == InMapState::TOWN) {
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(TOWN_EXIT_POS);
	}
	else if (curent_inmap_scene == InMapState::CASTLETOWN) {
		GameManager::getGameManager()->getPlayer()->SetPlayerPosition(CASTLE_TOWN_EXIT_POS);
	}

	return true;
}

//メニュー
bool InMapScene::seqMenuOpen(float delta_time)
{
	if (sequence_.isStart())
	{
		//プレイヤーを動けなくする
		GameManager::getGameManager()->getPlayer()->setPlayerControl();
		select_num = Sequence_num::MENUOPEN;
	}

	if (EventManager::getEventManager()->GetSelectMenu() != EventManager::MenuOpen::EMPTY && EventManager::getEventManager()->GetNpcMove() == EventManager::NpcMove::WEAPONSHOP
		&& dia_log_coment_select->getSelectNum() == 0) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			sequence_.change(&InMapScene::seqWeaponShop);
		}
	}
	else if (EventManager::getEventManager()->GetSelectMenu() == EventManager::MenuOpen::EMPTY && dia_log_coment_select->getSelectNum() == 0 || EventManager::getEventManager()->GetSelectMenu() == EventManager::MenuOpen::BOSS
		&& dia_log_coment_select->getSelectNum() == 0) {
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)){
			sequence_.change(&InMapScene::seqNpc);
		}
	}
	

	if (dia_log_coment_select->getSelectNum() == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//シーケンスを切り替える
		sequence_.change(&InMapScene::seqMove);
	}

	return false;
}

//Npcの処理
bool InMapScene::seqNpc(float delta_time)
{
	if (sequence_.isStart()) {
		select_num = Sequence_num::MENUNPC;

		//フレームをリセットしてセーブコメントをリセットする
		UIManager::getUIManager()->FrameReset();
	}

	//途中で会話を中断する為のキー
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		sequence_.change(&InMapScene::seqMove);
	}

	//イベント発生時のNpcとの更新処理
	EventManager::getEventManager()->NpcEventUpdate(delta_time, this, static_cast<int>(curent_inmap_scene) , actor_list);

	return false;
}

//シーケンス毎の描画
void InMapScene::switchSequenceDraw()
{
	switch (select_num)
	{
		//通常シーケンス
	case InMapScene::Sequence_num::MOVE:
		
		//DrawStringEx(menuWindow->menu_x + 50, menuWindow->menu_y + 350, -1, "seqIdle");

		break;

		//メニューシーケンスの時
	case InMapScene::Sequence_num::MENUOPEN:

		//Npcのコメント表示
		EventManager::getEventManager()->EventMenuWindow();

		//話すコメント
		dia_log_coment_select->All(50, 50, 250, 200);

		break;

		//Npcシーケンスの時(Npcによって動作を変える)
	case InMapScene::Sequence_num::MENUNPC:

		EventManager::getEventManager()->NpcSwitchComent(weapon_shop);

		break;

	case InMapScene::Sequence_num::WEAPONSHOP:

		EventManager::getEventManager()->NpcSwitchComent(weapon_shop);

		break;
	}
}


//------------------------------------------------------------------------------------------------------------------------
//マップチップ関連

//マップチップのロード
void InMapScene::mapChip_Loding(std::vector<std::vector<int>> map_csv ,std::vector<std::vector<int>> map_object_csv)
{
	//csvを初期化する
	map_csv_ = map_csv;

	map_csv_objct_ = map_object_csv;

	map_chip_all_size_ = map_chip_x_size_ * map_chip_y_size_;

	LoadDivGraph(gpc_map_chip_hdls_pass_.c_str(),
		map_chip_all_size_,
		map_chip_x_size_,
		map_chip_y_size_,
		map_chip_width_,
		map_chip_height_,
		gpc_map_chip_hdls_);

	//マップチップ読み込み
	for (int i = 0; i < map_csv_.size(); ++i) {
		for (int k = 0; k < map_csv_[i].size(); ++k) {
			if (-1 == map_csv_[i][k]) continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / 2 + k * mapchip->GetChipSize().x)};
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x)};
			gpc_hdl = gpc_map_chip_hdls_[map_csv_[i][k]];

			MapChips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}

	//オブジェクトのマップチップの読み込み
	for (int i = 0; i < map_csv_objct_.size(); ++i) {
		for (int k = 0; k < map_csv_objct_[i].size(); ++k) {
			if (-1 == map_csv_objct_[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / 2 + k * mapchip->GetChipSize().x) };
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x) };
			gpc_hdl = gpc_map_chip_hdls_[map_csv_objct_[i][k]];

			MapChip_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
		}
	}
	
}

//あたり判定の処理
void InMapScene::mapChip_collision()
{
	//現在のシーンが村だったら
	if (curent_inmap_scene == InMapState::VILLAGE) {
		//村の当たり判定
		VillageCollision();
	}
	//現在のシーンがボスの城だったら
	else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
		//ボスの城シーンの当たり判定
		DungeonCollison();
	}
	//現在のシーンが町だったら
	else if (curent_inmap_scene == InMapState::TOWN) {
		TownCollision();
	}
	//現在のシーンが城下町だったら
	else if (curent_inmap_scene == InMapState::CASTLETOWN) {
		CastleTownCollision();
	}
	//現在のシーンが城シーンだったら
	else if (curent_inmap_scene == InMapState::CASTLE) {
		CastleCollision();
	}
}

//村シーンでの当たり判定
void InMapScene::VillageCollision()
{
	//プレイヤーを取得する
	auto& player = GameManager::getGameManager()->getPlayer();

	//ブロックと衝突判定 & 補正
	//ゲームループ内での当たり判定のチェック]
	for (auto& mapChip : MapChip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(mapChip->MapChipPos().y / mapChip->GetChipSize().y)][static_cast<unsigned>(mapChip->MapChipPos().x / mapChip->GetChipSize().x)];


		//当たり判定
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), mapChip->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				mapChip->MapChipPos(),
				player->GetPlayerSize(),
				mapChip->GetChipSize(),
				player->getPlayerPos(),
				mapChip->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 1);


			//武器屋と話した場合
			if (mapChipValue == map_Chip_WeaponShop) {

				//アイコンのアニメーションを流す
				UIManager::getUIManager()->IconAnimation();

				if (select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//アイコンのアニメーションを止める
					UIManager::getUIManager()->IconAnimationStop();

					//シーケンスを切り替える
					EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
					EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
					sequence_.change(&InMapScene::seqMenuOpen);

				}
			}

			//村長の前の机から話した時
			else if (mapChipValue == map_Chip_Herdman) {

				//アイコンのアニメーションを流す
				UIManager::getUIManager()->IconAnimation();

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//アイコンのアニメーションを止める
					UIManager::getUIManager()->IconAnimationStop();

					//状態を切り替える
					EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

					//1からコメントを流す
					UIManager::getUIManager()->ComentClear();

					//カウントをリセットする
					UIManager::getUIManager()->CountReset();

					//コメント用の配列を初期化する
					UIManager::getUIManager()->ComentLoad(9, herdman_npc->GetNpcName());

					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}
		}



		//---Npcの当たり判定---//

		//村長
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), herdman_npc->GetNpcPos(), herdman_npc->GetNpcSizeWidth(), herdman_npc->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect herdman = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				herdman_npc->GetNpcPos(),
				player->GetPlayerSize(),
				herdman_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				herdman_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//下から当たった時
			//bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= herdman_npc->GetNpcPos().y + herdman_npc->GetNpcSizeHegiht();

			if (static_cast<int>(herdman) != 0) {

				//アイコンのアニメーションを流す
				UIManager::getUIManager()->IconAnimation();

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//アイコンのアニメーションを止める
					UIManager::getUIManager()->IconAnimationStop();

					//現在のアニメーションを保管する
					herdman_npc->SetNpcAnimationSave(herdman_npc->GetNpcAnimation());

					//アニメーションを下にする
					herdman_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(herdman));

					//1からコメントを流す
					UIManager::getUIManager()->ComentClear();

					//カウントをリセットする
					UIManager::getUIManager()->CountReset();

					//ロードするコメントの数
					const int HERDMAN_COMENT_MAX = 9;

					//コメント用の配列を初期化する
					UIManager::getUIManager()->ComentLoad(HERDMAN_COMENT_MAX, herdman_npc->GetNpcName());

					//状態を切り替える
					EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

					//シーケンスを切り替える
					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}
		}

		//神官
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), priest_npc->GetNpcPos(), priest_npc->GetNpcSizeWidth(), priest_npc->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect priest_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				priest_npc->GetNpcPos(),
				player->GetPlayerSize(),
				priest_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				priest_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//下から当たった時
			//bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= priest_npc->GetNpcPos().y + priest_npc->GetNpcSizeHegiht();

			if (priest_hit != tnl::eCorrResRect::UP) {

				//アイコンのアニメーションを流す
				UIManager::getUIManager()->IconAnimation();

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//アイコンのアニメーションを止める
					UIManager::getUIManager()->IconAnimationStop();

					//アニメーションを切り替える
					priest_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(priest_hit));

					//1からコメントを流す
					UIManager::getUIManager()->ComentClear();

					//カウントをリセットする
					UIManager::getUIManager()->CountReset();

					//イベントフラグがfalseの場合フラグを切り替える
					if (!EventManager::getEventManager()->GetEventFlag()) {
						EventManager::getEventManager()->EventFlagChange();
					}

					//ロードするコメントの数
					const int PRIEST_COMENT_MAX = 3;

					//コメント用の配列を初期化する
					UIManager::getUIManager()->ComentLoad(PRIEST_COMENT_MAX, priest_npc->GetNpcName());

					//状態を切り替える
					EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::PRIEST);
					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}
		}
		//女性村人
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), female_resident_npc->GetNpcPos(), female_resident_npc->GetNpcSizeWidth(), female_resident_npc->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect female_resident_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				female_resident_npc->GetNpcPos(),
				player->GetPlayerSize(),
				female_resident_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				female_resident_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {


				//アニメーションを切り替える
				female_resident_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(female_resident_hit));

				//1からコメントを流す
				UIManager::getUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::getUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int FEMALE_RESIDENT_COMENT_MAX = 3;

				//コメント用の配列を初期化する
				UIManager::getUIManager()->ComentLoad(FEMALE_RESIDENT_COMENT_MAX, female_resident_npc->GetNpcName());

				//状態を切り替える
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::FEMALE_RESIDENT);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
		//男性村人
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), resident_male_npc->GetNpcPos(), resident_male_npc->GetNpcSizeWidth(), resident_male_npc->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect resident_male_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				resident_male_npc->GetNpcPos(),
				player->GetPlayerSize(),
				resident_male_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				resident_male_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {


				//アニメーションを切り替える
				resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(resident_male_hit));

				//1からコメントを流す
				UIManager::getUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::getUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int RESIDENT_MALE_COMENT_MAX = 3;

				//コメント用の配列を初期化する
				UIManager::getUIManager()->ComentLoad(RESIDENT_MALE_COMENT_MAX, resident_male_npc->GetNpcName());

				//状態を切り替える
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}

	}

}
//町シーンでの当たり判定
void InMapScene::TownCollision()
{

	auto& player = GameManager::getGameManager()->getPlayer();

	for (auto& map_chip_collision : MapChip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//あたり判定
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect n = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->getPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);


			//武器屋と話した場合
			if (mapChipValue == 708 && select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				////アイコンのアニメーションを流す
				//UIManager::getUIManager()->IconAnimation();
				//シーケンスを切り替える
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
				sequence_.change(&InMapScene::seqMenuOpen);
			}

			//宿屋に話かけた時
			else if (mapChipValue == 673 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//状態を切り替える
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::INN);

				//1からコメントを流す
				UIManager::getUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::getUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//コメント用の配列を初期化する
				UIManager::getUIManager()->ComentLoad(3, inn_npc->GetNpcName());

				sequence_.change(&InMapScene::seqMenuOpen);

			}

		}
		//町長
		else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), herdman_npc->GetNpcPos(), herdman_npc->GetNpcSizeWidth(), herdman_npc->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect herdman_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				herdman_npc->GetNpcPos(),
				player->GetPlayerSize(),
				herdman_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				herdman_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//下から当たった時
			//bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= priest_npc->GetNpcPos().y + priest_npc->GetNpcSizeHegiht();

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//アニメーションを下にする
				herdman_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(herdman_hit));

				//1からコメントを流す
				UIManager::getUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::getUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int HEDMAN_COMENT_MAX = 6;

				//コメント用の配列を初期化する
				UIManager::getUIManager()->ComentLoad(HEDMAN_COMENT_MAX, herdman_npc->GetNpcName());

				//状態を切り替える
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::TOWNHERDMAN);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
		//敵のポインタが存在しない場合当たり判定を消す
		if (!enemy_npc) { return; }
		//魔物
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), enemy_npc->GetNpcPos(), enemy_npc->GetNpcSizeWidth(), enemy_npc->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect enemy_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				enemy_npc->GetNpcPos(),
				player->GetPlayerSize(),
				enemy_npc->GetNpcCharaChip(),
				player->getPlayerPos(),
				enemy_npc->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//アニメーションを変更する
				enemy_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(enemy_hit));

				//1からコメントを流す
				UIManager::getUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::getUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::getEventManager()->GetEventFlag()) {
					EventManager::getEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int ENEMY_COMENT_MAX = 5;

				//コメント用の配列を初期化する
				UIManager::getUIManager()->ComentLoad(ENEMY_COMENT_MAX, enemy_npc->GetNpcName());

				//状態を切り替える
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::ENEMY);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
		
}

//ボスの城シーンでの当たり判定
void InMapScene::DungeonCollison()
{
	auto& player = GameManager::getGameManager()->getPlayer();

	//---Npcの当たり判定---//

	
	if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), boss_npc->GetNpcPos(), boss_npc->GetNpcSizeWidth(), boss_npc->GetNpcSizeHegiht())) {

		//座標補正
		tnl::eCorrResRect boss = tnl::CorrectPositionRect(
			player->getPlayerPrevPos(),
			boss_npc->GetNpcPos(),
			player->GetPlayerSize(),
			boss_npc->GetNpcCharaChip(),
			player->getPlayerPos(),
			boss_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);

		//当たっているときにEnterキーを押したらバトルシーンに遷移させる
		//今後セリフも追加予定
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			const int BOSS_COMENT_MAX = 7;

			//イベントマネージャーの状態を更新する
			EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::BOSS);
			EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::BOSS);

			//カウントをリセットする
			UIManager::getUIManager()->CountReset();

			//1からコメントを流す
			UIManager::getUIManager()->ComentClear();

			//イベントフラグがfalseの場合フラグを切り替える
			if (!EventManager::getEventManager()->GetEventFlag()) {
				EventManager::getEventManager()->EventFlagChange();
			}

			//コメント用の配列を初期化する
			UIManager::getUIManager()->ComentLoad(BOSS_COMENT_MAX, boss_npc->GetNpcName());

			//シーケンスを切り替える
			sequence_.change(&InMapScene::seqMenuOpen);
		}
	}
}

//城下町シーンでの当たり判定
void InMapScene::CastleTownCollision()
{
	auto& player = GameManager::getGameManager()->getPlayer();

	for (auto& map_chip_collision : MapChip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//マップチップのあたり判定
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect map_chip_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->getPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);
		}
	}
}

//城シーンの当たり判定
void InMapScene::CastleCollision()
{
	auto& player = GameManager::getGameManager()->getPlayer();

	for (auto& map_chip_collision : MapChip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//マップチップのあたり判定
		if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect map_chip_hit = tnl::CorrectPositionRect(
				player->getPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->getPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);
		}
	}

	//---Npcの当たり判定---//

	//王
	if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), king_npc->GetNpcPos(), king_npc->GetNpcSizeWidth(), king_npc->GetNpcSizeHegiht())) {

		//座標補正
		tnl::eCorrResRect king_hit = tnl::CorrectPositionRect(
			player->getPlayerPrevPos(),
			king_npc->GetNpcPos(),
			player->GetPlayerSize(),
			king_npc->GetNpcCharaChip(),
			player->getPlayerPos(),
			king_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//アニメーションを変更する
				king_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(king_hit));

				//1からコメントを流す
				UIManager::getUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::getUIManager()->CountReset();

				//ロードするコメントの数
				const int KING_COMENT_MAX = 6;

				//コメント用の配列を初期化する
				UIManager::getUIManager()->ComentLoad(KING_COMENT_MAX, king_npc->GetNpcName());

				//状態を切り替える
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::KING);

				//シーケンスを切り替える
				sequence_.change(&InMapScene::seqMenuOpen);
			}
	}
	//王妃
	else if (tnl::IsIntersectRect(player->getPlayerPos(), player->GetPlayerWidth(), player->GetPlayerHight(), queen_npc->GetNpcPos(), queen_npc->GetNpcSizeWidth(), queen_npc->GetNpcSizeHegiht())) {

		//座標補正
		tnl::eCorrResRect queen_hit = tnl::CorrectPositionRect(
			player->getPlayerPrevPos(),
			queen_npc->GetNpcPos(),
			player->GetPlayerSize(),
			queen_npc->GetNpcCharaChip(),
			player->getPlayerPos(),
			queen_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);


		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//アニメーションを変更する
			queen_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(queen_hit));

			//1からコメントを流す
			UIManager::getUIManager()->ComentClear();

			//カウントをリセットする
			UIManager::getUIManager()->CountReset();

			//ロードするコメントの数
			const int QUEEN_COMENT_MAX = 3;

			//コメント用の配列を初期化する
			UIManager::getUIManager()->ComentLoad(QUEEN_COMENT_MAX, queen_npc->GetNpcName());

			//状態を切り替える
			EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::QUEEN);

			//シーケンスを切り替える
			sequence_.change(&InMapScene::seqMenuOpen);
		}
	}
}

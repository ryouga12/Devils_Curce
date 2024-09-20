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
InMapScene::InMapScene(const tnl::Vector3& firstPos, const InMapState& inmap_state): first_pos(firstPos), curent_inmap_scene(inmap_state)
{
	//現在のシーンをInMapに設定
	curent_scene = SceneState::INMAP;

	//csvの初期化
	CsvManager::GetCsvManager()->LoadCsv(curent_scene);
}

InMapScene::~InMapScene()
{
	//マップチップの解放(2096 = マップチップの総数)
	for (int i = 0; i < map_chip_all_size_; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("\n Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}
}

//初期化
void InMapScene::InitVillageScene()
{
	//武器関連の初期化
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);


	//NPCの初期化
	if (!herdman_npc) {
		herdman_npc = std::make_shared<Npc>("村長", HERDMAN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_LEFT, "graphics/Npc/herdman_icon.png");
	}
	if (!armsdealer_npc) {
		armsdealer_npc = std::make_shared<Npc>("武器商人", ARMSDEALER_VILLAGE_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/Armsdealer_icon.png");
	}
	if (!priest_npc) {
		priest_npc = std::make_shared<Npc>("神官", PRIEST_VILLAGE_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/Priest_icon.png");
	}
	if (!female_resident_npc) {
		female_resident_npc = std::make_shared<Npc>("女性村人", FEMALE_VILLAGERS_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_RIGHT, "graphics/Npc/female_villagers_icon.png");
	}
	if (!resident_male_npc) {
		resident_male_npc = std::make_shared<Npc>("男性村人", MALE_VILLAGERS_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/male_villagers_icon.png");
	}


	//アクターリストにNPCを追加する
	npc_list.emplace_back(armsdealer_npc);
	npc_list.emplace_back(herdman_npc);
	npc_list.emplace_back(priest_npc);
	npc_list.emplace_back(female_resident_npc);
	npc_list.emplace_back(resident_male_npc);

	//soundManagerの初期化
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/tasogare.mp3", DX_PLAYTYPE_LOOP);

	//マップチップのロード
	//村用のマップチップをロードする
	mapChip_Loding(CsvManager::GetCsvManager()->GetVillagMapCsv(), CsvManager::GetCsvManager()->GetVillagMapObjectCsv());

}

//ボスの城シーンの初期化
void InMapScene::InitBossCastleScene()
{
	auto mgr = SceneManager::GetSceneManager();

	//シーンマネージャーのシーンフラグがfalseの場合のみ
	if (!mgr->GetSceneFlag()) {

		//ボスの間シーンから以降した時に一度マップチップを空にする
		map_chips.clear();
		map_chip_object.clear();

		//プレイヤーの位置を変える
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_ROOM_EXIT_POS);

		//カメラ位置も変更する
		GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
		GameManager::GetGameManager()->GetCamera()->SetTargetPos();
	}

	//影の番人
	if (!shadow_npc && EventManager::GetEventManager()->GetEnemyEventFlag_3()) {

		shadow_npc = std::make_shared<Npc>("影の番人", SHADOW_ENEMY_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/shadow_monster_icon.png");

		//アクターリストにNPCを追加する
		npc_list.emplace_back(shadow_npc);
	}

	//マップチップのロード
	//ダンジョン用のマップチップをロードする
	mapChip_Loding(CsvManager::GetCsvManager()->GetBossCastleFloorCsv(), CsvManager::GetCsvManager()->GetBossCastleObjectCsv());

	//魔王の城内での音楽
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/maou_bgm_castle.mp3", DX_PLAYTYPE_LOOP);
}

//ボスの間シーンの初期化
void InMapScene::InitBossRoomScene()
{
	map_chips.clear();
	map_chip_object.clear();

	//プレイヤーの位置を変える
	GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_ROOM_FIRS_POS);

	//カメラ位置も変更する
	GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
	GameManager::GetGameManager()->GetCamera()->SetTargetPos();

	//ボス
	if (!boss_npc) {

		boss_npc = std::make_shared<Npc>("魔王", LAST_BOS_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/maou_icon.png");
	}

	//アクターリストにNPCを追加する
	npc_list.emplace_back(boss_npc);

	//マップチップのロード
	//ダンジョン用のマップチップをロードする
	mapChip_Loding(CsvManager::GetCsvManager()->GetBossRoomFloorCsv() , CsvManager::GetCsvManager()->GetBossRoomObjectCsv());

}

//町シーンの初期化
void InMapScene::InitTownScene()
{
	//武器関連の初期化
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);

	//---npcの初期化---//

	//武器商人
	if (!armsdealer_npc) {

		armsdealer_npc = std::make_shared<Npc>("武器商人", ARMSDEALER_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/Armsdealer_man_icon_.png");

	}
	//町長
	if (!herdman_npc) {

		herdman_npc = std::make_shared<Npc>("町長", TOWN_HERDMAN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_UP, "graphics/Npc/town_herdman_icon.png");
	}

	//宿屋
	if (!inn_npc) {

		//宿屋
		inn_npc = std::make_shared<Npc>("宿屋", INN_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/inn_icon.png");
	}

	//敵
	//イベントフラグがtrueだったら生成させる
	if (EventManager::GetEventManager()->GetEnemyEventFlag_1()&& !enemy_npc) {

		enemy_npc = std::make_shared<Npc>("魔物", ENEMY_POS , NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_UP, "graphics/Npc/enemy_1_icon.png");

		npc_list.emplace_back(enemy_npc);
	}

	//男性町民
	if (!resident_male_npc) {

		//男性町民
		resident_male_npc = std::make_shared<Npc>("男性町民", TOWNS_PEOPLE_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/male_villagers_icon.png");
	}


	//アクターリストにNPCを追加する
	npc_list.emplace_back(armsdealer_npc);
	npc_list.emplace_back(herdman_npc);
	npc_list.emplace_back(inn_npc);
	npc_list.emplace_back(resident_male_npc);


	//soundManagerの初期化
	//町のBGMを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/town_bgm.mp3", DX_PLAYTYPE_LOOP);

	//マップチップのロード
	mapChip_Loding(CsvManager::GetCsvManager()->GetTownMapCsv(), CsvManager::GetCsvManager()->GetTownObjectMapCsv());
}

//城下町シーンの初期化
void InMapScene::InitCastleTownScene()
{
	auto mgr = SceneManager::GetSceneManager();

	//シーンマネージャーのシーンフラグがfalseの場合のみ
	if(!mgr->GetSceneFlag()){

		//城シーンから以降した時に一度マップチップを空にする
		map_chips.clear();
		map_chip_object.clear();

		//男性住人のポインタを解放する
		//同じポインタで違う住人を城と城下町で出現させる為一度解放する
		if (resident_male_npc) { resident_male_npc.reset(); }

		//プレイヤーの位置を変える
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_EXIT_POS);

		//カメラ位置も変更する
		GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
		GameManager::GetGameManager()->GetCamera()->SetTargetPos();
	}

	//武器関連の初期化
	weapon_shop = std::make_shared<WeaponShop>(curent_inmap_scene);

	//武器商人
	if (!armsdealer_npc) {

		//ポインタの生成
		armsdealer_npc = std::make_shared<Npc>("武器商人", ARMSDEALER_CASTLE_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN,  "graphics/Npc/Armsdealer_icon.png");

	}
	//宿屋
	if (!inn_npc) {

		//宿屋
		inn_npc = std::make_shared<Npc>("宿屋", INN_CASTLE_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/inn_icon.png");
	}
	//神官
	if (!priest_npc) {

		priest_npc = std::make_shared<Npc>("神官", PRIEST_CASTLE_TOWN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/Priest_icon.png");

	}
	//男性住民
	if (!resident_male_npc) {

		//男性住民
		resident_male_npc = std::make_shared<Npc>("男性住民", MALE_RESIDENT_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_RIGHT,"graphics/Npc/male_resident_icon.png");
	}

	//アクターリストにNPCを追加する
	npc_list.emplace_back(armsdealer_npc);
	npc_list.emplace_back(inn_npc);
	npc_list.emplace_back(priest_npc);
	npc_list.emplace_back(resident_male_npc);

	//soundManagerの初期化
	//城下町のBGMを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/castle_town_bgm.mp3", DX_PLAYTYPE_LOOP);

	//マップチップのロード
	mapChip_Loding(CsvManager::GetCsvManager()->GetCastleTownMapCsv(), CsvManager::GetCsvManager()->GetCastleTownObjectMapCsv());
}

//城シーンの初期化
void InMapScene::InitCastleScene()
{
	map_chips.clear();
	map_chip_object.clear();

	//男性住人のポインタを解放する
	//同じポインタで違う住人を城と城下町で出現させる為一度解放する
	if (resident_male_npc) { resident_male_npc.reset(); }

	//プレイヤーの位置を変える
	GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_FIRST_POS);

	//カメラ位置も変更する
	GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
	GameManager::GetGameManager()->GetCamera()->SetTargetPos();

	//王
	if (!king_npc) {
		king_npc = std::make_shared<Npc>("王", KING_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/king_icon.png");
	}
	//王妃
	if (!queen_npc) {
		queen_npc = std::make_shared<Npc>("王妃", QUEEN_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/queen_icon.png");
	}
	//学者
	if (!resident_male_npc) {
		resident_male_npc = std::make_shared<Npc>("学者", SCHOLAR_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_UP, "graphics/Npc/scholar_icon.png");
	}
	//兵士
	if (!soldier_npc) {
		soldier_npc = std::make_shared<Npc>("兵士", SOLDIER_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/soldier_icon.png");
	}
	if (!second_soldier_npc) {
		second_soldier_npc = std::make_shared<Npc>("兵士", SECOND_SOLDIER_POS, NPC_CHIP_SIZE, koni::Numeric::SCALE_ONE_AND_HALF, Npc::NPC_ANIMATION::NPC_DIR_DOWN, "graphics/Npc/soldier_icon.png");
	}

	//アクターリストにNPCを追加する
	npc_list.emplace_back(king_npc);
	npc_list.emplace_back(queen_npc);
	npc_list.emplace_back(resident_male_npc);
	npc_list.emplace_back(soldier_npc);
	npc_list.emplace_back(second_soldier_npc);


	//soundManagerの初期化
	//城のBGMを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/castle.mp3", DX_PLAYTYPE_LOOP);

	//マップチップのロード
	mapChip_Loding(CsvManager::GetCsvManager()->GetCastleMapCsv(), CsvManager::GetCsvManager()->GetCastleObjectMapCsv());
}

//墓シーンの初期化
void InMapScene::InitGraveScene()
{
	//イベントフラグが経っていればポインタを生成させる
	if (EventManager::GetEventManager()->GetEnemyEventFlag_2()) {

		if (!ordeal_npc) {
			//試練の番人ポインタを生成する
			ordeal_npc = std::make_shared<Npc>("試練の番人", "graphics/Npc/ordeal_icon.png");
		}

		npc_list.emplace_back(ordeal_npc);
	}

	//soundManagerの初期化
	//勇者の墓のBGMを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/grave.mp3", DX_PLAYTYPE_LOOP);

	//マップチップのロード
	mapChip_Loding(CsvManager::GetCsvManager()->GetGraveFloorCsv(), CsvManager::GetCsvManager()->GetGraveObjectCsv());

}


void InMapScene::InitInMapScene()
{
	//Playerの初期位置を決める
	GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(first_pos);

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
	case InMapState::BOSSCASTLE:

		//マップの高さとマップの幅を決める
		map_height = 945;
		map_width = 1600;

		InitBossCastleScene();

		break;


	//魔王の間
	case InMapScene::InMapState::BOSSROOM:

		//マップの高さとマップの幅を決める
		map_height = 940;
		map_width = 1600;

		//ダンジョンシーンの初期化
		InitBossRoomScene();

		break;

	//墓
	case InMapScene::InMapState::GRAVE:

		//マップの高さとマップの幅を決める
		map_height = 945;
		map_width = 1600;

		//勇者の墓シーンの初期化
		InitGraveScene();

		break;

	default:
		break;
	}

	//Npcの画像の初期化
	//墓シーンでは画像のあるNPCを使わない為、それ以外のシーンでのみ
	if (!npc_list.empty() && curent_inmap_scene != InMapState::GRAVE) {

		for (auto& npc_list : npc_list) {

			//画像をセットする
			npc_list->SetNpcImage(npc_list->GetNpcName(),static_cast<int>(curent_inmap_scene));
		}
	}

	//イベント関連の初期化
	EventManager::GetEventManager()->InitEventRelated(npc_list);

}

//---メニューの初期化---//
void InMapScene::InitMenuWindow()
{

	//話すコマンド
	if (!dia_log_coment) {

		//---座標---//

		//話す
		const tnl::Vector2i TALK_STRING_POS = {100 , 100};

		//閉じる
		const tnl::Vector2i CLOASE_STRING_POS = { 100 , 150 };

		dia_log_coment = new MenuWindow::MenuElement_t[]{
			{TALK_STRING_POS.x ,   TALK_STRING_POS.y ,   "話す" ,	MenuWindow::Elements::FIRST_ELEMENT},
			{CLOASE_STRING_POS.x , CLOASE_STRING_POS.y , "閉じる" , MenuWindow::Elements::SECOND_ELEMENT}
		};
	}

	if (!dia_log_coment_select) {

		//話すコマンドの要素数
		const int DIA_LOG_COMENT_NUM = 2;

		dia_log_coment_select = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", dia_log_coment, DIA_LOG_COMENT_NUM);
		dia_log_coment_select->Open();
	}

	//武器屋の購入コマンド
	if (!weapon_shop_coment) {

		//---座標---//

		//購入
		const tnl::Vector2i BUY_STRING_POS = { 100 , 100 };

		//売る
		const tnl::Vector2i SELL_STRING_POS = { 100 , 150 };

		//閉じる
		const tnl::Vector2i CLOASE_STRING_POS = { 100 , 200 };

		weapon_shop_coment = new MenuWindow::MenuElement_t[]{
			{BUY_STRING_POS.x ,    BUY_STRING_POS.y ,     "購入する" , MenuWindow::Elements::FIRST_ELEMENT},
			{SELL_STRING_POS.x ,   SELL_STRING_POS.y ,    "売る"     , MenuWindow::Elements::SECOND_ELEMENT},
			{CLOASE_STRING_POS.x , CLOASE_STRING_POS.y,   "閉じる"   , MenuWindow::Elements::THERD_ELEMENT}
		};
	}

	if (!weapon_shop_coment_select) {

		//話すコマンドの要素数
		const int WEAPON_SHOP_COMENT_NUM = 3;

		weapon_shop_coment_select = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", weapon_shop_coment, WEAPON_SHOP_COMENT_NUM);
		weapon_shop_coment_select->Open();
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
	GameManager::GetGameManager()->GetPlayer()->PlayerAnimation(delta_time);;

	//Inventory
	GameManager::GetGameManager()->GetInventory()->Update(delta_time);

	//npcの更新処理
	for (auto& actor : npc_list) {
		actor->Update(delta_time);
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
	for (auto& mapchip : map_chips) {
		mapchip->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	//---mapChipの描画---//
	for (auto& mapchip : map_chip_object) {
		mapchip->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	//---Npcの描画---//
	for (auto& actor : npc_list) {
		actor->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	//---plyerの描画---//
	GameManager::GetGameManager()->GetPlayer()->Player_draw(*GameManager::GetGameManager()->GetCamera(), koni::Numeric::SCALE_ONE_AND_HALF);

	//インベントリの描画
	GameManager::GetGameManager()->GetInventory()->draw();

	//それぞれのシーケンスでの描画
	switchSequenceDraw();

	/*DrawStringEx(100, 100, koni::Color::WHITE, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().x);
	DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().y);*/

}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

//シーケンスを切り替える
void InMapScene::SequenceChangeMove()
{
	//主にイベントマネージャーなどでイベントを切り替える時に使用
	//通常に戻す
	sequence_.change(&InMapScene::seqMove);
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

//マップ外に出た際の処理
void InMapScene::InMapSceneExitProcess(const float delta_time)
{
	//プレイヤーの座標がマップ外に出たらシーンを切り替える
	//村シーンのみ塀がない為、上にいけないようにする
	if (curent_inmap_scene == InMapState::VILLAGE) {

		//プレイヤーのサイズ
		auto player_size = GameManager::GetGameManager()->GetPlayer()->GetPlayerSize();

		//プレイヤーの移動制限
		if (GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y < static_cast<float>(player_size.y / 2)) {

			tnl::Vector3 player_pos = { GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().x ,static_cast<float>(player_size.y / koni::Numeric::DIVIDER_TWO) , 0 };

			GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(player_pos);
		}
	}
	//城下町シーンの場合上に行ったら城に移動させる
	//またはボスの城の道シーンの場合上に行ったら城に遷移させる
	else if (curent_inmap_scene == InMapState::CASTLETOWN && GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y < 0
			|| curent_inmap_scene == InMapState::BOSSCASTLE && GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y < 0) {

		//城下町の場合城に移動させる
		if (curent_inmap_scene == InMapState::CASTLETOWN)
		{
			curent_inmap_scene = InMapState::CASTLE;

			//サウンドを止める
			SoundManager::GetSoundManager()->StopSound("sound/BGM/castle_town_bgm.mp3");
		}
		//ボスの城の場合、ボスの間に移動させる
		else {
			curent_inmap_scene = InMapState::BOSSROOM;
		}

		//初期化させて城シーンに移行させる
		sequence_.change(&InMapScene::seqInit);

		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

		auto mgr = SceneManager::GetSceneManager();

		//フラグを切り替えてシーンを遷移させないようにする
		if (mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		//フェードを入れる
		mgr->FadeScene(FADE_OUT_TIME, FADE_IN_TIME);

		//NPCを一度初期化する
		npc_list.clear();

		return ;

	}
	//城シーンの場合、マップ外に出たら城下町に遷移させる
	//またはボスの城のボスの間だったら階段下に移動させる
	else if (GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y >= map_height && curent_inmap_scene == InMapState::CASTLE
			|| GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y >= map_height && curent_inmap_scene == InMapState::BOSSROOM)
	{
		//城の場合、城下町に遷移させる
		if (curent_inmap_scene == InMapState::CASTLE) {
			curent_inmap_scene = InMapState::CASTLETOWN;
		}
		//ボスの間の場合、ボスの城の道に移動させる(階段下)
		else {
			curent_inmap_scene = InMapState::BOSSCASTLE;
		}
			
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

		auto mgr = SceneManager::GetSceneManager();

		//フラグを切り替えてシーンを遷移させないようにする
		if (mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		//フェードを入れる
		mgr->FadeScene(FADE_OUT_TIME, FADE_IN_TIME);

		//サウンドを止める
		SoundManager::GetSoundManager()->StopSound("sound/BGM/castle.mp3");

		//NPCを一度初期化する
		npc_list.clear();

		//初期化させて城シーンに移行させる
		sequence_.change(&InMapScene::seqInit);

		return ;
	}
	if (GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().x >= map_width || GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().x < 0 || GameManager::GetGameManager()->GetPlayer()->GetPlayerPos().y >= map_height)
	{
		TimeCount = true;

		//村の場合
		if (curent_inmap_scene == InMapState::VILLAGE) {
			//音楽をStopする
			SoundManager::GetSoundManager()->StopSound("sound/BGM/tasogare.mp3");
		}
		//町の場合
		else if (curent_inmap_scene == InMapState::TOWN) {
			//音楽をStopする
			SoundManager::GetSoundManager()->StopSound("sound/BGM/town_bgm.mp3");
		}
		//魔王の城の場合
		else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
			//音楽をStopする
			SoundManager::GetSoundManager()->StopSound("sound/BGM/maou_bgm_castle.mp3");
		}
		//城下町の場合
		else if (curent_inmap_scene == InMapState::CASTLETOWN) {
			//音楽をStopする
			SoundManager::GetSoundManager()->StopSound("sound/BGM/castle_town_bgm.mp3");
		}
		//勇者の墓の場合
		else if (curent_inmap_scene == InMapState::GRAVE) {
			SoundManager::GetSoundManager()->StopSound("sound/BGM/grave.mp3");
		}

		//移動のSEを流す
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
	}

	//切り替える際にSEを流し切ってから遷移する
	if (TimeCount) {

		//0.2秒SEを流してから遷移させる
		if (GameManager::GetGameManager()->TimeCount(delta_time, TimeSE)) {

			//プレイヤーを一時的に見えなくする
			GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

			sequence_.change(&InMapScene::seqChangeScene);

			//シーンマネージャーのポインタを取得する
			auto mgr = SceneManager::GetSceneManager();

			//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
			if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

			//シーンを遷移させる
			mgr->changeScene(new MapScene());
		}
	}
}

bool InMapScene::seqMove(float delta_time)
{
	if (sequence_.isStart()) {

		if (!GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
			GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
		}

		select_num = Sequence_num::MOVE;
	}

	//プレイヤーの動き
	GameManager::GetGameManager()->GetPlayer()->Player_Move(delta_time, PLAYER_VELOCITY, map_height);

	//カメラの更新処理
	GameManager::GetGameManager()->GetCamera()->update(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), map_width, map_height);

	if (GameManager::GetGameManager()->GetInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {

		//決定音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//インベントリを表示する
		GameManager::GetGameManager()->GetInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
		//playerを動けないようにする
		GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
	}

	//当たり判定
	//インベントリを開いている時は当たり判定を消す
	if (GameManager::GetGameManager()->GetInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY) {
		mapChip_collision();
	}

	//マップ外に出たときの処理
	InMapSceneExitProcess(delta_time);
	
	
	return false;
}

//武器屋
bool InMapScene::seqWeaponShop(float delta_time)
{
	if (sequence_.isStart()) {

		select_num = Sequence_num::WEAPONSHOP;
		
		//毎回コメントを初期化する
		//コメントがかぶらないようにする為
		weapon_shop->BuyComentClear();

		//Npcのコメントを切り替える
		EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
	}

	//武器屋の処理
	weapon_shop->Update(delta_time);
	
	//0を押したらシーケンスを変える
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		//シーンを切り替える
		sequence_.change(&InMapScene::seqMove);
		//武器屋シーケンスを待機状態に戻す
		weapon_shop->WeaponShopChangeIdle();
	}

	return false;
}

//シーンを変える時のフェードアウトを入れる為シーケンス
//シーンによって座標も切り替える
bool InMapScene::seqChangeScene(float delta_time)
{
	//村
	if (curent_inmap_scene == InMapState::VILLAGE) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(FIRST_WORLD_POS);
	}
	//ボスの城
	else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_EXIT_POS);
	}
	//町
	else if (curent_inmap_scene == InMapState::TOWN) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(TOWN_EXIT_POS);
	}
	//城下町
	else if (curent_inmap_scene == InMapState::CASTLETOWN) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_TOWN_EXIT_POS);
	}
	//勇者の墓
	else if (curent_inmap_scene == InMapState::GRAVE) {
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(GRAVE_EXIT_POS);
	}

	return false;
}

//メニュー
bool InMapScene::seqMenuOpen(float delta_time)
{
	if (sequence_.isStart())
	{
		//プレイヤーを動けなくする
		GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
		select_num = Sequence_num::MENUOPEN;
	}

	//武器の購入コマンドが買うか売るを選択した場合、武器屋シーケンスに移行する
	//買うを選択した場合
	if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::WEAPONSHOP && weapon_shop_coment_select->getSelectNum() == MenuWindow::Elements::FIRST_ELEMENT
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//シーケンスを武器屋シーケンスに切り替える
		sequence_.change(&InMapScene::seqWeaponShop);

		//武器屋の状態を切り替える
		weapon_shop->SetWeaponShopTransaction(WeaponShop::TransactionType::BUY);

		return false;
	}
	//売るを選択した場合
	else if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::WEAPONSHOP && weapon_shop_coment_select->getSelectNum() == MenuWindow::Elements::SECOND_ELEMENT
		&& tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//シーケンスを武器屋シーケンスに切り替える
		sequence_.change(&InMapScene::seqWeaponShop);

		//武器屋の状態を切り替える
		weapon_shop->SetWeaponShopTransaction(WeaponShop::TransactionType::SELL);

		return false;
	}
	//閉じるを押したら動けるようにする
	else if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::WEAPONSHOP && weapon_shop_coment_select->getSelectNum() == MenuWindow::Elements::THERD_ELEMENT) {

		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			sequence_.change(&InMapScene::seqMove);

			return false;
		}
	}

	//話すコマンド
	if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::EMPTY && dia_log_coment_select->getSelectNum() == 0 || EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::BOSS
		&& dia_log_coment_select->getSelectNum() == MenuWindow::Elements::FIRST_ELEMENT) {
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)){
			sequence_.change(&InMapScene::seqNpc);
		}
	}
	else if (dia_log_coment_select->getSelectNum() == MenuWindow::Elements::SECOND_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

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
		UIManager::GetUIManager()->FrameReset();
	}

	//途中で会話を中断する為のキー
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		sequence_.change(&InMapScene::seqMove);
	}

	//イベント発生時のNpcとの更新処理
	EventManager::GetEventManager()->NpcEventUpdate(delta_time, npc_list , this , static_cast<int>(curent_inmap_scene));

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
		
		if (EventManager::GetEventManager()->GetSelectMenu() == EventManager::MenuOpen::WEAPONSHOP) {

			weapon_shop_coment_select->All(DIALOG_POS.x, DIALOG_POS.y, WEAPON_SHOP_WINDOW_SIZE.x, WEAPON_SHOP_WINDOW_SIZE.y);
		}
		else {
			//話すコメント
			dia_log_coment_select->All(DIALOG_POS.x, DIALOG_POS.y, DIALOG_SIZE.x, DIALOG_SIZE.y);

		}
	
		break;

		//Npcシーケンスの時(Npcによって動作を変える)
	case InMapScene::Sequence_num::MENUNPC:

		EventManager::GetEventManager()->NpcSwitchComent(weapon_shop);

		break;

	case InMapScene::Sequence_num::WEAPONSHOP:

		EventManager::GetEventManager()->NpcSwitchComent(weapon_shop);

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
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / koni::Numeric::DIVIDER_TWO + k * mapchip->GetChipSize().x)};
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x)};
			gpc_hdl = gpc_map_chip_hdls_[map_csv_[i][k]];

			map_chips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}

	//オブジェクトのマップチップの読み込み
	for (int i = 0; i < map_csv_objct_.size(); ++i) {
		for (int k = 0; k < map_csv_objct_[i].size(); ++k) {
			if (-1 == map_csv_objct_[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / koni::Numeric::DIVIDER_TWO + k * mapchip->GetChipSize().x) };
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x) };
			gpc_hdl = gpc_map_chip_hdls_[map_csv_objct_[i][k]];

			map_chip_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
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
		BossCastleCollision();
	}
	//現在のシーンがボスの間だったら
	else if (curent_inmap_scene == InMapState::BOSSROOM) {
		//ボスの城シーンの当たり判定
		BossRoomCollision();
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
	//現在のシーンが勇者の墓シーンだったら
	else if (curent_inmap_scene == InMapState::GRAVE) {
		GraveSceneCollision();
	}
}

//村シーンでの当たり判定
void InMapScene::VillageCollision()
{
	//プレイヤーを取得する
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//ブロックと衝突判定 & 補正
	//ゲームループ内での当たり判定のチェック]
	for (auto& mapChip : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(mapChip->MapChipPos().y / mapChip->GetChipSize().y)][static_cast<unsigned>(mapChip->MapChipPos().x / mapChip->GetChipSize().x)];


		//当たり判定
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, mapChip->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				mapChip->MapChipPos(),
				player->GetPlayerSize(),
				mapChip->GetChipSize(),
				player->GetPlayerPos(),
				mapChip->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B , 0);

			//武器屋
			const int MAP_CHIP_WEAPONSHOP = 510;

			//武器屋と話した場合
			if (mapChipValue == MAP_CHIP_WEAPONSHOP) {

				if (select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//シーケンスを切り替える
					EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
					EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
					sequence_.change(&InMapScene::seqMenuOpen);

				}
			}
		}
	}


	//---Npcの当たり判定---//

	for (auto& npc_ : npc_list) {

		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, npc_->GetNpcPos(), npc_->GetNpcSizeWidth(), npc_->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect collison = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				npc_->GetNpcPos(),
				player->GetPlayerSize(),
				npc_->GetNpcCharaChip(),
				player->GetPlayerPos(),
				npc_->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//村長
			if (collison != tnl::eCorrResRect::LEFT && npc_->GetNpcName() == "村長") {

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//アニメーションを下にする
					herdman_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

					//1からコメントを流す
					UIManager::GetUIManager()->ComentClear();

					//カウントをリセットする
					UIManager::GetUIManager()->CountReset();

					//ロードするコメントの数
					const int HERDMAN_COMENT_MAX = 9;

					//コメント用の配列を初期化する
					UIManager::GetUIManager()->ComentLoad(HERDMAN_COMENT_MAX, herdman_npc->GetNpcName());

					//状態を切り替える
					EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

					//シーケンスを切り替える
					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}

			//神官
			else if (collison != tnl::eCorrResRect::UP && npc_->GetNpcName() == "神官") {

				if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

					//アニメーションを切り替える
					priest_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

					//1からコメントを流す
					UIManager::GetUIManager()->ComentClear();

					//カウントをリセットする
					UIManager::GetUIManager()->CountReset();

					//イベントフラグがfalseの場合フラグを切り替える
					if (!EventManager::GetEventManager()->GetEventFlag()) {
						EventManager::GetEventManager()->EventFlagChange();
					}

					//ロードするコメントの数
					const int PRIEST_COMENT_MAX = 3;

					//コメント用の配列を初期化する
					UIManager::GetUIManager()->ComentLoad(PRIEST_COMENT_MAX, priest_npc->GetNpcName());

					//状態を切り替える
					EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
					EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::PRIEST);
					sequence_.change(&InMapScene::seqMenuOpen);
				}
			}
			//女性村人
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "女性村人") {

				//アニメーションを切り替える
				female_resident_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int FEMALE_RESIDENT_COMENT_MAX = 3;

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(FEMALE_RESIDENT_COMENT_MAX, female_resident_npc->GetNpcName());

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::FEMALE_RESIDENT);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
			//男性村人
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "男性村人") {


				//アニメーションを切り替える
				resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int RESIDENT_MALE_COMENT_MAX = 3;

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(RESIDENT_MALE_COMENT_MAX, resident_male_npc->GetNpcName());

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
}
//町シーンでの当たり判定
void InMapScene::TownCollision()
{

	auto& player = GameManager::GetGameManager()->GetPlayer();

	//マップチップの当たり判定
	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//あたり判定
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect n = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);


			//武器屋のマップチップ
			const int TOWN_MAPCHIP_WEAPON_SHOP = 708;

			//宿屋のマップチップ
			const int TOWN_MAPCHIP_INN = 673;

			//武器屋と話した場合
			if (mapChipValue == TOWN_MAPCHIP_WEAPON_SHOP && select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//シーケンスを切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
				sequence_.change(&InMapScene::seqMenuOpen);
			}

			//宿屋に話かけた時
			else if (mapChipValue == TOWN_MAPCHIP_INN && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::INN);

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(3, inn_npc->GetNpcName());

				sequence_.change(&InMapScene::seqMenuOpen);

			}

		}
	}

	//npcの当たり判定
	for (auto& npc_ : npc_list) {

		//当たり判定
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, npc_->GetNpcPos(), npc_->GetNpcSizeWidth(), npc_->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect collison = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				npc_->GetNpcPos(),
				player->GetPlayerSize(),
				npc_->GetNpcCharaChip(),
				player->GetPlayerPos(),
				npc_->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//町長
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "町長") {

				//アニメーションを下にする
				herdman_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int HEDMAN_COMENT_MAX = 6;

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(HEDMAN_COMENT_MAX, herdman_npc->GetNpcName());

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::TOWNHERDMAN);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
			//男性町人
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "男性町民") {


				//アニメーションを切り替える
				resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int RESIDENT_MALE_COMENT_MAX = 3;

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(RESIDENT_MALE_COMENT_MAX, resident_male_npc->GetNpcName());

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
			//魔物
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "魔物"
				&& enemy_npc) {

				//アニメーションを変更する
				enemy_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collison));

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int ENEMY_COMENT_MAX = 5;

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(ENEMY_COMENT_MAX, enemy_npc->GetNpcName());

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::ENEMY);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}	
}

//ボスの城シーンの当たり判定
void InMapScene::BossCastleCollision()
{
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//マップチップの当たり判定
	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//あたり判定
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect n = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);


		}
	}

	if (!shadow_npc) { return; }
	//npcの当たり判定
	//あたり判定
	if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, shadow_npc->GetNpcPos(), shadow_npc->GetNpcSizeWidth(), shadow_npc->GetNpcSizeHegiht())) {

		//座標補正
		tnl::eCorrResRect shadow_collison = tnl::CorrectPositionRect(
			player->GetPlayerPrevPos(),
			shadow_npc->GetNpcPos(),
			player->GetPlayerSize(),
			shadow_npc->GetNpcCharaChip(),
			player->GetPlayerPos(),
			shadow_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);

		//影の番人
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && shadow_npc) {

			//アニメーションを変更する
			shadow_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(shadow_collison));

			//1からコメントを流す
			UIManager::GetUIManager()->ComentClear();

			//カウントをリセットする
			UIManager::GetUIManager()->CountReset();

			//イベントフラグがfalseの場合フラグを切り替える
			if (!EventManager::GetEventManager()->GetEventFlag()) {
				EventManager::GetEventManager()->EventFlagChange();
			}

			//ロードするコメントの数
			const int SHADOW_ENEMY_COMENT_MAX = 4;

			//コメント用の配列を初期化する
			UIManager::GetUIManager()->ComentLoad(SHADOW_ENEMY_COMENT_MAX, shadow_npc->GetNpcName());

			//状態を切り替える
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::SHADOWENEMY);
			sequence_.change(&InMapScene::seqMenuOpen);
		}
	}
}

//ボスの間シーンでの当たり判定
void InMapScene::BossRoomCollision()
{
	auto& player = GameManager::GetGameManager()->GetPlayer();

	//マップチップの当たり判定
	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//あたり判定
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect n = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);
		}
	}

	//---Npcの当たり判定---//

	if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, boss_npc->GetNpcPos(), boss_npc->GetNpcSizeWidth(), boss_npc->GetNpcSizeHegiht())) {

		//座標補正
		tnl::eCorrResRect boss = tnl::CorrectPositionRect(
			player->GetPlayerPrevPos(),
			boss_npc->GetNpcPos(),
			player->GetPlayerSize(),
			boss_npc->GetNpcCharaChip(),
			player->GetPlayerPos(),
			boss_npc->GetNpcPos(),
			tnl::eCorrTypeRect::PWRFL_B,
			tnl::eCorrTypeRect::PWRFL_B, 0);

		//当たっているときにEnterキーを押したらバトルシーンに遷移させる
		//今後セリフも追加予定
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			const int BOSS_COMENT_MAX = 7;

			//イベントマネージャーの状態を更新する
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::BOSS);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::BOSS);

			//カウントをリセットする
			UIManager::GetUIManager()->CountReset();

			//1からコメントを流す
			UIManager::GetUIManager()->ComentClear();

			//イベントフラグがfalseの場合フラグを切り替える
			if (!EventManager::GetEventManager()->GetEventFlag()) {
				EventManager::GetEventManager()->EventFlagChange();
			}

			//コメント用の配列を初期化する
			UIManager::GetUIManager()->ComentLoad(BOSS_COMENT_MAX, boss_npc->GetNpcName());

			//シーケンスを切り替える
			sequence_.change(&InMapScene::seqMenuOpen);
		}
	}
}

//城下町シーンでの当たり判定
void InMapScene::CastleTownCollision()
{
	auto& player = GameManager::GetGameManager()->GetPlayer();

	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//マップチップのあたり判定
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect map_chip_hit = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//武器屋のマップチップ
			const int CASTLE_TOWN_MAPCHIP_WEAPON_SHOP = 674;

			//宿屋のマップチップ
			const int CASTLE_TOWN_MAPCHIP_INN = 699;

			//武器屋と話した場合
			if (mapChipValue == CASTLE_TOWN_MAPCHIP_WEAPON_SHOP && select_num == Sequence_num::MOVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//シーケンスを切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
				sequence_.change(&InMapScene::seqMenuOpen);
			}

			//宿屋に話かけた時
			else if (mapChipValue == CASTLE_TOWN_MAPCHIP_INN && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::INN);

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(3, inn_npc->GetNpcName());

				sequence_.change(&InMapScene::seqMenuOpen);

			}
		}
	}

	for (auto& npc_ : npc_list) {

		//神官
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, npc_->GetNpcPos(), npc_->GetNpcSizeWidth(), npc_->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				npc_->GetNpcPos(),
				player->GetPlayerSize(),
				npc_->GetNpcCharaChip(),
				player->GetPlayerPos(),
				npc_->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			if (collision != tnl::eCorrResRect::UP && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
				&&npc_->GetNpcName() == "神官") {

				//アニメーションを切り替える
				priest_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int PRIEST_COMENT_MAX = 3;

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(PRIEST_COMENT_MAX, priest_npc->GetNpcName());

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::PRIEST);
				sequence_.change(&InMapScene::seqMenuOpen);
				
			}
			//男性住民
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)&& npc_->GetNpcName() == "男性住民") {


				//アニメーションを切り替える
				resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int RESIDENT_MALE_COMENT_MAX = 3;

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(RESIDENT_MALE_COMENT_MAX, resident_male_npc->GetNpcName());

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
}

//城シーンの当たり判定
void InMapScene::CastleCollision()
{
	auto& player = GameManager::GetGameManager()->GetPlayer();

	for (auto& map_chip_collision : map_chip_object) {

		int mapChipValue = map_csv_objct_[static_cast<unsigned>(map_chip_collision->MapChipPos().y / map_chip_collision->GetChipSize().x)][static_cast<unsigned>(map_chip_collision->MapChipPos().x / map_chip_collision->GetChipSize().x)];

		//マップチップのあたり判定
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, map_chip_collision->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect map_chip_hit = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				map_chip_collision->MapChipPos(),
				player->GetPlayerSize(),
				map_chip_collision->GetChipSize(),
				player->GetPlayerPos(),
				map_chip_collision->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);
		}
	}

	//---Npcの当たり判定---//

	for (auto& npc_ : npc_list) {

		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, npc_->GetNpcPos(), npc_->GetNpcSizeWidth(), npc_->GetNpcSizeHegiht())) {

			//座標補正
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				npc_->GetNpcPos(),
				player->GetPlayerSize(),
				npc_->GetNpcCharaChip(),
				player->GetPlayerPos(),
				npc_->GetNpcPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B, 0);

			//王
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "王") {

			//アニメーションを変更する
			king_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

			//1からコメントを流す
			UIManager::GetUIManager()->ComentClear();

			//カウントをリセットする
			UIManager::GetUIManager()->CountReset();

			//ロードするコメントの数
			const int KING_COMENT_MAX = 6;

			//コメント用の配列を初期化する
			UIManager::GetUIManager()->ComentLoad(KING_COMENT_MAX, king_npc->GetNpcName());

			//状態を切り替える
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::KING);

			//シーケンスを切り替える
			sequence_.change(&InMapScene::seqMenuOpen);
			}
			//王妃
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "王妃") {

			//アニメーションを変更する
			queen_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

			//1からコメントを流す
			UIManager::GetUIManager()->ComentClear();

			//カウントをリセットする
			UIManager::GetUIManager()->CountReset();

			//ロードするコメントの数
			const int QUEEN_COMENT_MAX = 3;

			//コメント用の配列を初期化する
			UIManager::GetUIManager()->ComentLoad(QUEEN_COMENT_MAX, queen_npc->GetNpcName());

			//状態を切り替える
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::QUEEN);

			//シーケンスを切り替える
			sequence_.change(&InMapScene::seqMenuOpen);
			}
			//学者
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "学者") {

			//アニメーションを変更する
			resident_male_npc->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

			//1からコメントを流す
			UIManager::GetUIManager()->ComentClear();

			//カウントをリセットする
			UIManager::GetUIManager()->CountReset();

			//ロードするコメントの数
			const int SCHOLAR_COMENT_MAX = 7;

			//コメント用の配列を初期化する
			UIManager::GetUIManager()->ComentLoad(SCHOLAR_COMENT_MAX, resident_male_npc->GetNpcName());

			//状態を切り替える
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::SCHOLAR);

			//シーケンスを切り替える
			sequence_.change(&InMapScene::seqMenuOpen);
			}
			//兵士
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) && npc_->GetNpcName() == "兵士") {

			//アニメーションを変更する
			npc_->SetNpcAnimation(static_cast<Npc::NPC_ANIMATION>(collision));

			//1からコメントを流す
			UIManager::GetUIManager()->ComentClear();

			//カウントをリセットする
			UIManager::GetUIManager()->CountReset();

			//ロードするコメントの数
			const int SOLDIER_COMENT_MAX = 3;

			//コメント用の配列を初期化する
			UIManager::GetUIManager()->ComentLoad(SOLDIER_COMENT_MAX, soldier_npc->GetNpcName());

			//状態を切り替える
			EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
			EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::RESIDENT_MALE);

			//シーケンスを切り替える
			sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
}

//勇者の墓シーンの当たり判定
void InMapScene::GraveSceneCollision()
{

	auto& player = GameManager::GetGameManager()->GetPlayer();

	//ブロックと衝突判定 & 補正
	//ゲームループ内での当たり判定のチェック]
	for (auto& mapChip : map_chip_object) {


		int mapChipValue = map_csv_objct_[static_cast<unsigned>(mapChip->MapChipPos().y / mapChip->GetChipSize().y)][static_cast<unsigned>(mapChip->MapChipPos().x / mapChip->GetChipSize().x)];

		//当たり判定
		if (tnl::IsIntersectRect(player->GetPlayerPos(), player->GetPlayerSize().x, player->GetPlayerSize().y, mapChip->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//座標補正
			tnl::eCorrResRect collision = tnl::CorrectPositionRect(
				player->GetPlayerPrevPos(),
				mapChip->MapChipPos(),
				player->GetPlayerSize(),
				mapChip->GetChipSize(),
				player->GetPlayerPos(),
				mapChip->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B);

			//墓のマップチップ
			const int MAP_CHIP_GRAVE = 1694;

			//勇者の墓に触れた時
			//イベントが終了してなければ
			if (mapChipValue == MAP_CHIP_GRAVE && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
				&& ordeal_npc) {

				//試練の番人のコメントフラグをリセットする
				EventManager::GetEventManager()->OrdealComentFlagReset();

				//状態を切り替える
				EventManager::GetEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::GetEventManager()->SetNpcComent(EventManager::NpcMove::ORDEAL);

				//1からコメントを流す
				UIManager::GetUIManager()->ComentClear();

				//カウントをリセットする
				UIManager::GetUIManager()->CountReset();

				//イベントフラグがfalseの場合フラグを切り替える
				if (!EventManager::GetEventManager()->GetEventFlag()) {
					EventManager::GetEventManager()->EventFlagChange();
				}

				//ロードするコメントの数
				const int ORDEAL_COMENT_MAX = 5;

				//コメント用の配列を初期化する
				UIManager::GetUIManager()->ComentLoad(ORDEAL_COMENT_MAX, ordeal_npc->GetNpcName());

				sequence_.change(&InMapScene::seqMenuOpen);

			}
		}
	}
}

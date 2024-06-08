#include "InMapScene.h"
#include"../Manager/SceneManager.h"
#include"battleScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"mapScene.h"
#include<iostream>
#include"../Manager/UiManager.h"
#include"../Manager/CsvManager.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

//コンストラクタ
//引数 : 中に入った時の最初の座標 , 現在のシーン
InMapScene::InMapScene(tnl::Vector3 firstPos, InMapState inmap_state): first_pos(firstPos)
{
	//初期化
	InitInMapScene(inmap_state);
}

InMapScene::~InMapScene()
{
	//サウンドを止める
	SoundManager::getSoundManager()->StopSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->StopSound("sound/SoundEffect/idou.mp3");
	SoundManager::getSoundManager()->StopSound("sound/BGM/tasogare.mp3");

	//マップチップの解放(1992 = マップチップの総数)
	for (int i = 0; i < 1992; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}
}

//初期化
void InMapScene::InitVillageScene()
{
	
	//武器関連の初期化
	weapon_shop = std::make_shared<WeaponShop>();

	//NPCの初期化
	herdman_npc = std::make_shared<Npc>("村長", tnl::Vector3(1375, 760, 0),48 , herdman_hdl_pass, 1.5f, 0, 4);
	armsdealer_npc = std::make_shared<Npc>("武器商人", tnl::Vector3(1150, 140, 0), 48 , Armsdealer_hdl_pass, 1.5f, 0, 1);
	priest_npc = std::make_shared<Npc>("神官", tnl::Vector3(239, 740, 0),48 , Priest_hdl_pass, 1.5f, 0, 3);

	//アクターリストにNPCを追加する
	actor_list.emplace_back(herdman_npc);
	actor_list.emplace_back(armsdealer_npc);
	actor_list.emplace_back(priest_npc);

	//soundManagerの初期化
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tasogare.mp3", DX_PLAYTYPE_LOOP);

	//マップチップのロード
	//村用のマップチップをロードする
	mapChip_Loding(CsvManager::getCsvManager()->GetVillagMapCsv());

}

//ダンジョンシーン(魔王の城)の初期化
void InMapScene::InitDungeonScene()
{
	//ボス
	boss_npc = std::make_shared<Npc>("Zerahkiel", tnl::Vector3(801, 153, 0), 128 , boss_hdl_pass, 1, 0, 1);
	//アクターリストにNPCを追加する
	actor_list.emplace_back(boss_npc);

	//マップチップのロード
	//ダンジョン用のマップチップをロードする
	mapChip_Loding(CsvManager::getCsvManager()->GetBossCastleFloorCsv());

	//魔王の城内での音楽
	SoundManager::getSoundManager()->sound_Play("sound/BGM/maou_bgm_castle.mp3",DX_PLAYTYPE_LOOP);
}


void InMapScene::InitInMapScene(InMapState inmap_state)
{
	//Playerの初期位置を決める
	GameManager::getGameManager()->getPlayer()->SetPlayerPosition(first_pos);

	//メニューの初期化
	InitMenuWindow();

	//マップチップの初期化
	mapchip = std::make_shared<MapChip>();

	switch (inmap_state)
	{
	//村
	case InMapScene::InMapState::VILLAGE:

		//現在のシーンを村に設定
		curent_inmap_scene = inmap_state;

		//初期化処理
		InitVillageScene();

		//マップの高さとマップの幅を決める
		map_height = 945;																					
		map_width = 1600;																					

		break;

	//町
	case InMapScene::InMapState::TOWN:

		//現在のシーンを町に設定
		curent_inmap_scene = inmap_state;

		break;

	//城
	case InMapScene::InMapState::CASTLE:

		//現在のシーンを城に設定
		curent_inmap_scene = inmap_state;

		break;

	//ダンジョン
	case InMapScene::InMapState::BOSSCASTLE:

		//現在のシーンをダンジョンに設定
		curent_inmap_scene = inmap_state;

		//マップの高さとマップの幅を決める
		map_height = 945;
		map_width = 1600;

		//ダンジョンシーンの初期化
		InitDungeonScene();

		break;


	default:
		break;
	}
}

//---メニューの初期化---//
void InMapScene::InitMenuWindow()
{

	//通常のメニューウィンドウ
	menuWindow = UIManager::getUIManager()->getMenu("menu_window");

}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void InMapScene::Update(float delta_time)
{
	InMapSceneUpdate(delta_time);
}

//それぞれの更新処理
void InMapScene::InMapSceneUpdate(float delta_time)
{
	//Player
	GameManager::getGameManager()->getPlayer()->player_Move(delta_time, PlayerVelocity);

	//シーケンス
	sequence_.update(delta_time);

	//camera
	GameManager::getGameManager()->getCamera()->update(GameManager::getGameManager()->getPlayer()->getPlayerPos(), map_width, map_height);

	//Inventory
	GameManager::getGameManager()->getInventory()->Update(delta_time);


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


		break;

	//城シーン
	case InMapScene::InMapState::CASTLE:


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
}

//それぞれの描画処理
void InMapScene::InMapSceneDraw()
{
	switch (curent_inmap_scene)
	{
	case InMapScene::InMapState::VILLAGE:

		//---mapChipの描画---//
		for (auto& mapchip : MapChips) {
			mapchip->Draw(*GameManager::getGameManager()->getCamera());
		}

		//---Npcの描画---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		break;

	//町シーン
	case InMapScene::InMapState::TOWN:


		break;

	//城シーン
	case InMapScene::InMapState::CASTLE:


		break;

	//ボスの城
	case InMapScene::InMapState::BOSSCASTLE:

		//---mapChipの描画---//
		for (auto& mapchip : MapChips) {
			mapchip->Draw(*GameManager::getGameManager()->getCamera());
		}

		for (auto& mapchip : MapChip_object) {
			mapchip->Draw(*GameManager::getGameManager()->getCamera());
		}

		//---Npcの描画---//
		for (auto& actor : actor_list) {
			actor->Draw(*GameManager::getGameManager()->getCamera());
		}

		DrawStringEx(100, 100, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().x);
		DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getPlayer()->getPlayerPos().y);


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
}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

bool InMapScene::seqIdle(float delta_time)
{
	if (sequence_.isStart()) {

		if (!GameManager::getGameManager()->getPlayer()->getPlayerControl()) {
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
		}
		select_num = Sequence_num::Idle;
	}

	if (GameManager::getGameManager()->getInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {

		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//インベントリを表示する
		GameManager::getGameManager()->getInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
		//plyerを動けないようにする
		GameManager::getGameManager()->getPlayer()->setPlayerControl();
	}

	//プレイヤーの座標がマップ外に出たらシーンを切り替える
	if (GameManager::getGameManager()->getPlayer()->getPlayerPos().x >= map_width || GameManager::getGameManager()->getPlayer()->getPlayerPos().x < 0 || GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= map_height /*tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)*/)
	{
		TimeCount = true;
		//音楽をStopする
		SoundManager::getSoundManager()->StopSound("sound/BGM/tasogare.mp3");
		//移動のSEを流す
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
	}
	//切り替える際にSEを流し切ってから遷移する
	if (TimeCount) {
		//0.2秒SEを流してから遷移させる
		if (GameManager::getGameManager()->TimeCount(delta_time, TimeSE)) {
			sequence_.change(&InMapScene::seqChangeScene);
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new MapScene(),0.3);
		}
	}

	//当たり判定
	mapChip_collision(CsvManager::getCsvManager()->GetVillagMapCsv());

	return true;
}

//武器屋
bool InMapScene::seqWeaponShop(float delta_time)
{
	if (sequence_.isStart()) {
		select_num = Sequence_num::WeaponShop;
		weapon_shop->buyComentClear();
	}

	//武器屋の処理
	weapon_shop->BuyWeapon(GameManager::getGameManager()->getPlayer());
	weapon_shop->WeaponShopCursorMove();
	
	//0を押したらシーケンスを変える
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		//シーンを切り替える
		sequence_.change(&InMapScene::seqIdle);
	}
	return true;
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
	GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());

	return true;
}

//メニュー
bool InMapScene::seqMenuOpen(float delta_time)
{
	if (sequence_.isStart())
	{
		//プレイヤーを動けなくする
		GameManager::getGameManager()->getPlayer()->setPlayerControl();
		select_num = Sequence_num::MenuOpen;
	}

	if (EventManager::getEventManager()->GetSelectMenu() != EventManager::MenuOpen::EMPTY && EventManager::getEventManager()->GetNpcMove() == EventManager::NpcMove::WEAPONSHOP) {
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
			sequence_.change(&InMapScene::seqWeaponShop);
		}
	}
	else if (EventManager::getEventManager()->GetSelectMenu() == EventManager::MenuOpen::EMPTY || EventManager::getEventManager()->GetSelectMenu() == EventManager::MenuOpen::BOSS) {
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_1)){
			sequence_.change(&InMapScene::seqNpc);
		}
	}
	

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {

		//シーケンスを切り替える
		sequence_.change(&InMapScene::seqIdle);

		//村シーンのみ
		if (curent_inmap_scene == InMapState::VILLAGE) {
			//最初の態勢と違っていたら戻す
			if (herdman_npc->GetAnimationSelect() != 0) {
				herdman_npc->NpcAnimationReset();
				herdman_npc->ChengeAnimation();
			}
		}
	}

	return false;
}

//Npcの処理
bool InMapScene::seqNpc(float delta_time)
{
	if (sequence_.isStart()) {
		select_num = Sequence_num::MenuNpc;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		sequence_.change(&InMapScene::seqIdle);
		////コメントを表示できるようにする
		//NpcComntTrue = true;
	}

	return false;
}

//シーケンス毎の描画
void InMapScene::switchSequenceDraw()
{
	switch (select_num)
	{
		//通常シーケンス
	case InMapScene::Sequence_num::Idle:
		
		//DrawStringEx(menuWindow->menu_x + 50, menuWindow->menu_y + 350, -1, "seqIdle");

		break;

		//武器屋のシーケンスの時
	case InMapScene::Sequence_num::WeaponShop:

		if (auto menu_coment_window = menuWindow.lock()) {
			menu_coment_window->Menu_draw(50, 500, 700, 200);
		}

		//コメントとアイコン
		ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/Npc/Armsdealer_icon.png", 90, 600, 1.5f, 0, true);

		//武器屋のコメントを表示する
	    UIManager::getUIManager()->armsdealerComentDraw(armsdealer_comet);

		//武器屋の描画
		weapon_shop->Draw();

		//プレイヤーの所持金を描画
		DrawStringEx(135, 100, -1, "所持金: %d", GameManager::getGameManager()->getPlayer()->getPlayerMoney());

		/*DrawStringEx(menuWindow->menu_x + 350, menuWindow->menu_y + 350, -1, "seqWeaponShop");*/

		break;

		//メニューシーケンスの時
	case InMapScene::Sequence_num::MenuOpen:

		//Npcのコメント表示
		EventManager::getEventManager()->EventMenuWindow();

		break;

		//Npcシーケンスの時(Npcによって動作を変える)
	case InMapScene::Sequence_num::MenuNpc:

		EventManager::getEventManager()->NpcSwitchComent();

		/*if (EventManager::getEventManager()->GetNpcMove() == EventManager::NpcMove::BOSS) {
			sequence_.change(&InMapScene::seqChangeScene);
		}*/

		break;


	default:
		break;
	}
}

//------------------------------------------------------------------------------------------------------------------------
//マップチップ関連

//マップチップのロード
void InMapScene::mapChip_Loding(std::vector<std::vector<int>>map_csv)
{
	// マップチップの画像のロード
	gpc_map_chip_hdls_pass_ = "graphics/image_(2024_0115_0231).png";

	map_chip_width_ = 32;
	map_chip_height_ = 32;
	map_chip_x_size_ = 8;
	map_chip_y_size_ = 249;
	map_chip_all_size_ = map_chip_x_size_ * map_chip_y_size_;

	LoadDivGraph(gpc_map_chip_hdls_pass_.c_str(),
		map_chip_all_size_,
		map_chip_x_size_,
		map_chip_y_size_,
		map_chip_width_,
		map_chip_height_,
		gpc_map_chip_hdls_);

	//マップチップ読み込み
	for (int i = 0; i < map_csv.size(); ++i) {
		for (int k = 0; k < map_csv[i].size(); ++k) {
			if (-1 == map_csv[i][k]) continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { mapchip->GetChipSize() / 2 + k * mapchip->GetChipSize()};
			pos.y = { i * mapchip->GetChipSize()};
			gpc_hdl = gpc_map_chip_hdls_[map_csv[i][k]];
			/*DrawGraph(pos.x, pos.y, gpc_hdl, true);*/
			MapChips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}

	//シーンがダンジョンだったら
	if (curent_inmap_scene == InMapState::BOSSCASTLE) {

		//オブジェクト
		auto world_object_csv = CsvManager::getCsvManager()->GetBossCastleObjectCsv();

		//オブジェクトのマップチップの読み込み
		for (int i = 0; i < world_object_csv.size(); ++i) {
			for (int k = 0; k < world_object_csv[i].size(); ++k) {
				if (-1 == world_object_csv[i][k])continue;
				tnl::Vector3 pos;
				int gpc_hdl;
				pos.x = { mapchip->GetChipSize() / 2 + k * mapchip->GetChipSize() };
				pos.y = { i * mapchip->GetChipSize() };
				gpc_hdl = gpc_map_chip_hdls_[world_object_csv[i][k]];
				MapChip_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
			}
		}
	}
}

//あたり判定の処理
void InMapScene::mapChip_collision(std::vector<std::vector<int>>map_csv)
{
	//現在のシーンが村だったら
	if (curent_inmap_scene == InMapState::VILLAGE) {
		//村の当たり判定
		VillageCollision(map_csv);
	}
	else if (curent_inmap_scene == InMapState::BOSSCASTLE) {
		//ダンジョンシーンの当たり判定
		DungeonCollison();
	}
}

//村シーンでの当たり判定
void InMapScene::VillageCollision(std::vector<std::vector<int>>map_csv)
{
	//ブロックと衝突判定 & 補正
	//ゲームループ内での当たり判定のチェック]
	for (auto& mapChip : MapChips) {

		int mapChipValue = map_csv[static_cast<unsigned>(mapChip->MapChipPos().y / mapChip->GetChipSize())][static_cast<unsigned>(mapChip->MapChipPos().x / mapChip->GetChipSize())];
		if (village_colision_values.count(mapChipValue) > 0)
		{
			int collision = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), mapChip->MapChipPos(), map_chip_width_, map_chip_height_);
			//あたり判定が作用した際にその番号が510番だった場合、windowを表示する
			if (collision) {
				//武器屋と話した場合
				if (mapChipValue == map_Chip_WeaponShop) {
					if (select_num == Sequence_num::Idle && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
						//シーケンスを切り替える
						EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::WEAPONSHOP);
						EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::WEAPONSHOP);
						sequence_.change(&InMapScene::seqMenuOpen);

					}
				}

				//村長と話した時
				else if (mapChipValue == map_Chip_Herdman) {
					if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

						//状態を切り替える
						EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
						EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

						//1からコメントを流す
						UIManager::getUIManager()->ComentClear();

						//コメント用の配列を初期化する
						UIManager::getUIManager()->ComentLoad(9, herdman_npc->GetNpcName());

						sequence_.change(&InMapScene::seqMenuOpen);
					}
				}
			}
		}
	}

	//---Npcの当たり判定---//

	//村長
	int h_colition = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(
		GameManager::getGameManager()->getPlayer()->getPlayerPos(),
		GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(),
		GameManager::getGameManager()->getPlayer()->GetPlayerWidth(),
		GameManager::getGameManager()->getPlayer()->GetPlayerHight(),
		herdman_npc->GetNpcPos(),
		herdman_npc->GetNpcSizeWidth(),
		herdman_npc->GetNpcSizeHegiht()
	);


	if (h_colition) {

		//下から当たった時
		bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= herdman_npc->GetNpcPos().y + herdman_npc->GetNpcSizeHegiht();

		if (playerOnBottom) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//アニメーションを下にする
				herdman_npc->SetNpcAnimation(1);
				herdman_npc->ChengeAnimation();

				//1からコメントを流す
				UIManager::getUIManager()->ComentClear();

				//コメント用の配列を初期化する
				UIManager::getUIManager()->ComentLoad(9, herdman_npc->GetNpcName());

				//状態を切り替える
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::HERDMAN);

				//シーケンスを切り替える
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}

	//神官
	int P_collision = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition
	(
		GameManager::getGameManager()->getPlayer()->getPlayerPos(),
		GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(),
		GameManager::getGameManager()->getPlayer()->GetPlayerWidth(),
		GameManager::getGameManager()->getPlayer()->GetPlayerHight(),
		priest_npc->GetNpcPos(),
		priest_npc->GetNpcSizeWidth(),
		priest_npc->GetNpcSizeHegiht()
	);


	if (P_collision) {
		//下から当たった時
		bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= priest_npc->GetNpcPos().y + priest_npc->GetNpcSizeHegiht();

		if (playerOnBottom) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				priest_npc->NpcAnimationReset();
				priest_npc->ChengeAnimation();
				select_num = Sequence_num::MenuOpen;
				EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::EMPTY);
				EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::PRIEST);
				sequence_.change(&InMapScene::seqMenuOpen);
			}
		}
	}
}

//ダンジョンのシーンでの当たり判定
void InMapScene::DungeonCollison()
{
	//---Npcの当たり判定---//

	//ボス
	int boss_colition = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(
		GameManager::getGameManager()->getPlayer()->getPlayerPos(),
		GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(),
		GameManager::getGameManager()->getPlayer()->GetPlayerWidth(),
		GameManager::getGameManager()->getPlayer()->GetPlayerHight(),
		boss_npc->GetNpcPos(),
		boss_npc->GetNpcSizeWidth(),
		boss_npc->GetNpcSizeHegiht()
	);

	//当たっているときにEnterキーを押したらバトルシーンに遷移させる
	//今後セリフも追加予定
	if (boss_colition && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		const int BOSS_COMENT_MAX = 6;

		//イベントマネージャーの状態を更新する
		EventManager::getEventManager()->SetMenuNpc(EventManager::MenuOpen::BOSS);
		EventManager::getEventManager()->SetNpcComent(EventManager::NpcMove::BOSS);

		//1からコメントを流す
		UIManager::getUIManager()->ComentClear();

		//コメント用の配列を初期化する
		UIManager::getUIManager()->ComentLoad(BOSS_COMENT_MAX, boss_npc->GetNpcName());

		//シーケンスを切り替える
		sequence_.change(&InMapScene::seqMenuOpen);
	}
}

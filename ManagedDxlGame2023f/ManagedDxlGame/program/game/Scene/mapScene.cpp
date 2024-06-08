#include "mapScene.h"
#include"../Manager/SceneManager.h"
#include"resultScene.h"
#include"../Object/Player.h"
#include"../Manager/CsvManager.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

MapScene::MapScene(){

	mapchip = std::make_shared<MapChip>();

	//MapChipの読み込み
	worldMapLoad();

	//歩数カウントを150から300の値のランダムの数値を入れて確率を変える
	encounterThreshold = rand() % 201 + 150;

	//BGMを流す
	SoundManager::getSoundManager()->sound_Play("sound/BGM/sfc-harukanaru-daichi.mp3", DX_PLAYTYPE_LOOP);
}

MapScene::~MapScene()
{
	//マップチップの解放(2096 = マップチップの総数)
	for (int i = 0; i < 2096; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}

	//BGMを止める
	SoundManager::getSoundManager()->StopSound("sound/BGM/sfc-harukanaru-daichi.mp3");

}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void MapScene::Update(float delta_time)
{
	//シーケンスの更新処理
	sequence_.update(delta_time);

	//プレイヤーの更新処理
	GameManager::getGameManager()->getPlayer()->player_Move(delta_time, player_velocity);

	//カメラの更新処理
	GameManager::getGameManager()->getCamera()->update(GameManager::getGameManager()->getPlayer()->getPlayerPos(), MAP_WIDTH, MAP_HEIGHT);

	//歩数を数える
	//エンカウント処理
	if (ChackEncount(GameManager::getGameManager()->getPlayer()->GetStep())) {
		//BGMを止める
		SoundManager::getSoundManager()->StopSound("sound/BGM/sfc-harukanaru-daichi.mp3");
		//バトル用のBGMを流す
		SoundManager::getSoundManager()->sound_Play("sound/BGM/sentou.mp3", DX_PLAYTYPE_LOOP);

		//0.5秒経過したらシーンを遷移させる
		if (GameManager::getGameManager()->TimeCount(delta_time, time)) {

			auto mgr = SceneManager::GetInstance();
			//シーンを遷移させる(プレイヤーの座標を渡す,敵のID,敵のポインタを渡す)
			mgr->changeScene(new BattleScene(GameManager::getGameManager()->getPlayer()->getPlayerPos(), background_hdl, std::make_shared<MobMonster>(enemy_id)));
			//歩数を0にする
			GameManager::getGameManager()->getPlayer()->StepReset();
		}
	}

	//Inventory
	GameManager::getGameManager()->getInventory()->Update(delta_time);
	
	//当たり判定
	WorldMapCollision();

}

//------------------------------------------------------------------------------------------------------------------------
//描画

void MapScene::Draw()
{
	//MapChipの描画
	for (auto& world_map : MapChip_continent) {
		world_map->Draw(*GameManager::getGameManager()->getCamera());
	}

	for (auto& world_object : MapChips_object) {
		world_object->Draw(*GameManager::getGameManager()->getCamera());
	}

	//プレイヤーの描画
	GameManager::getGameManager()->getPlayer()->player_draw(*GameManager::getGameManager()->getCamera(), player_size);

	//インベントリの描画
	GameManager::getGameManager()->getInventory()->draw();

}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

bool MapScene::seqIdle(float delta_time)
{
	//アニメーションと歩数を0にする
	if (sequence_.isStart()) {
		GameManager::getGameManager()->getPlayer()->AnimationChange(Animation_default);
		GameManager::getGameManager()->getPlayer()->StepReset();
		
	}
	//デバック処理
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new ResultScene());
	}

	//ESCを押したら、インベントリを表示する
	if (GameManager::getGameManager()->getInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
			
			//SEを流す
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			//インベントリを表示する
			GameManager::getGameManager()->getInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
			//plyerを動けないようにする
			GameManager::getGameManager()->getPlayer()->setPlayerControl();
	}

	return false;
}

bool MapScene::seqChangeScene(float delta_time)
{
	//カメラを保管座標に入れる
	GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());

	return true;
}

//------------------------------------------------------------------------------------------------------------------------
//マップチップ関連

void MapScene::WorldMapCollision()
{
	//ワールドマップのcsv
	auto world_map_csv = CsvManager::getCsvManager()->GetWorldMapCsv();

	//ワールドマップにあるオブジェクトのcsv
	auto world_object_csv = CsvManager::getCsvManager()->GetWorldMapObjectCsv();

	//オブジェクトの当たり判定
	for (auto& MapChip : MapChips_object) {
		int mapChipValue = static_cast<int>(world_object_csv[static_cast<unsigned>(MapChip->MapChipPos().y / MapChip->GetChipSize())][static_cast<unsigned>(MapChip->MapChipPos().x / MapChip->GetChipSize())]);
		if (colisionObjectValues.count(mapChipValue) > 0)
		{
			ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), MapChip->MapChipPos(), map_chip_width_, map_chip_height_);
		}
		if (villageValues.count(mapChipValue) > 0) {
			int Collision = tnl::IsIntersectRect(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), MapChip->MapChipPos(), map_chip_width_, map_chip_height_);
			if (Collision) {

				//オブジェクトに当たった状態でEnterキーを押したら遷移させる
				//主に村や町、城、ボスの城などに遷移させる
				if (mapChipValue == map_chip_village) {
					if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
						SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
						//村シーンに切り替える
						auto mgr = SceneManager::GetInstance();
						mgr->changeScene(new InMapScene(village_pos, InMapScene::InMapState::VILLAGE), 0.1);
						GameManager::getGameManager()->getPlayer()->SetPlayerPosition(village_pos);
						sequence_.change(&MapScene::seqChangeScene);
					}
				}
				//ボスの城だったら
				else if (mapChipValue == map_chip_boss_castle) {
					if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
						SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
						//ダンジョンシーンに切り替える
						auto mgr = SceneManager::GetInstance();
						mgr->changeScene(new InMapScene(boss_castle_pos, InMapScene::InMapState::BOSSCASTLE), 0.1 ,0.8);
						//座標をボスの場所にセットする
						GameManager::getGameManager()->getPlayer()->SetPlayerPosition(boss_castle_pos);
						sequence_.change(&MapScene::seqChangeScene);
					}
				}

			}
		}

	}
	//地形の当たり判定
	for (auto& worldMap_C : MapChip_continent) {
		int mapChipValue = world_map_csv[static_cast<unsigned>(worldMap_C->MapChipPos().y / worldMap_C->GetChipSize())][static_cast<unsigned>(worldMap_C->MapChipPos().x / worldMap_C->GetChipSize())];
		if (worldCollisionValues.count(mapChipValue)) {
			int collison = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), worldMap_C->MapChipPos(), map_chip_width_, map_chip_height_);
		}
		//エンカウント用の当たり判定
		if (encount_kind.count(mapChipValue)) {
			int encount = tnl::IsIntersectRect(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->GetPlayerWidth(), GameManager::getGameManager()->getPlayer()->GetPlayerHight(), worldMap_C->MapChipPos(), map_chip_width_, map_chip_height_);

				//map上のどこに居るかによって敵を変える
			if (encount) {
				//平原にいる場合
				if (mapChipValue == plain) {

					//最初の敵のIDを渡す
					enemy_id = 1;

					//mapChipに応じて背景を変える
					background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/battle_field_01.jpg");
				}
				else if (mapChipValue == plain_second) {

					//平原の敵のIDを渡す
					enemy_id = 2;

					//mapChipに応じて背景を変える
					background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/battle_field_02.jpg");;
				}
				//荒原にいる場合
				else if (mapChipValue == wilderness[0] || mapChipValue == wilderness[1]) {

					//荒原にいる敵のIDを渡す
					enemy_id = 3;

					//mapChipに応じて背景を変える
					background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/battle_field_03.jpg");;
				}
				//ボスエリアの場合
				else if (mapChipValue == boss_area) {

					//ボスエリアにいる敵のIDを渡す
					enemy_id = 4;

					//mapChipに応じて背景を変える
					background_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg014b.jpg");
				}
			}
		}
	}

}

//エンカウント
bool MapScene::ChackEncount(int step)
{
	if (step >= encounterThreshold) {
		return true;
	}

	return false;
}

//マップチップのロード
void MapScene::worldMapLoad()
{
	//ワールドマップのcsv
	auto world_map_csv = CsvManager::getCsvManager()->GetWorldMapCsv();

	//ワールドマップにあるオブジェクトのcsv
	auto world_object_csv = CsvManager::getCsvManager()->GetWorldMapObjectCsv();

	// マップチップの画像のロード
	map_chip_ghdl_pass = "graphics/image_(2024_0115_0231).png";

	map_chip_width_ = 32;
	map_chip_height_ = 32;
	map_chip_x_size_ = 8;
	map_chip_y_size_ = 262;
	map_chip_all_size_ = map_chip_x_size_ * map_chip_y_size_;

	LoadDivGraph(map_chip_ghdl_pass.c_str(),
		map_chip_all_size_,
		map_chip_x_size_,
		map_chip_y_size_,
		map_chip_width_,
		map_chip_height_,
		gpc_map_chip_hdls_);

	//平原のマップチップの読み込み
	for (int i = 0; i < world_map_csv.size(); ++i) {
		for (int k = 0; k < world_map_csv[i].size(); ++k) {
			if (-1 == world_map_csv[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { mapchip->GetChipSize() / 2 + k * mapchip->GetChipSize()};
			pos.y = { i * mapchip->GetChipSize() };
			gpc_hdl = gpc_map_chip_hdls_[world_map_csv[i][k]];
			MapChip_continent.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
		}
	}

	//オブジェクトのマップチップの読み込み
	for (int i = 0; i < world_object_csv.size(); ++i) {
		for (int k = 0; k <  world_object_csv[i].size(); ++k) {
			if (-1 == world_object_csv[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { mapchip->GetChipSize() / 2 + k * mapchip->GetChipSize()};
			pos.y = { i * mapchip->GetChipSize()};
			gpc_hdl = gpc_map_chip_hdls_[world_object_csv[i][k]];
			MapChips_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
		}
	}

}
#include "mapScene.h"
#include"../Manager/SceneManager.h"
#include"resultScene.h"
#include"../Object/Player.h"
#include"../Manager/CsvManager.h"
#include"battleScene.h"
#include"../Manager/UiManager.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

MapScene::MapScene(){

	//現在のシーンをMapSceneに設定
	curent_scene = SceneState::WORLDMAP;

	//csvの初期化
	CsvManager::GetCsvManager()->LoadCsv(curent_scene);

	//マップチップの初期化
	mapchip = std::make_shared<MapChip>();

	//MapChipの読み込み
	worldMapLoad();

	//歩数カウントを150から300の値のランダムの数値を入れて確率を変える
	encounterThreshold = (rand() % 150) + 150;

	//---マップの高さと幅を決める---//
	
	//マップの高さ
	map_height = 1550;		
	//マップの幅
	map_width = 3200;					
	

	//イベント関連の初期化
	EventManager::GetEventManager()->InitEventRelated(npc_list);

	//BGMを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/BGM/sfc-harukanaru-daichi.mp3", DX_PLAYTYPE_LOOP);
}

MapScene::~MapScene()
{
	//マップチップの解放(2096 = マップチップの総数)
	for (int i = 0; i < 2096; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}

	//BGMを止める
	SoundManager::GetSoundManager()->StopSound("sound/BGM/sfc-harukanaru-daichi.mp3");

}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

//デバック用
void MapScene::DebugMapScene()
{
	//デバック処理
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {

		auto mgr = SceneManager::GetSceneManager();

		//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new ResultScene());
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_3)) {

		//町シーンに切り替える
		auto mgr = SceneManager::GetSceneManager();

		//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(TOWN_POS, InMapScene::InMapState::TOWN), 0.1f);
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(VILLAGE_POS);
		sequence_.change(&MapScene::seqChangeScene);

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_4)) {
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
		//ダンジョンシーンに切り替える
		auto mgr = SceneManager::GetSceneManager();

		//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(BOSS_CASTLE_POS, InMapScene::InMapState::BOSSCASTLE), 0.1f, 0.8f);

		//座標をボスの場所にセットする
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_CASTLE_POS);
		sequence_.change(&MapScene::seqChangeScene);
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_5)) {

		//城下町シーンに切り替える
		auto mgr = SceneManager::GetSceneManager();

		//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(CASTLE_TOWN_POS, InMapScene::InMapState::CASTLETOWN), 0.1f);
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_TOWN_POS);
		sequence_.change(&MapScene::seqChangeScene);

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_6)) {

		//勇者の墓シーンに切り替える
		auto mgr = SceneManager::GetSceneManager();

		//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(GRAVE_POS, InMapScene::InMapState::GRAVE), 0.1f);
		GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(GRAVE_POS);
		sequence_.change(&MapScene::seqChangeScene);

	}
}


void MapScene::Update(float delta_time)
{
	//シーケンスの更新処理
	sequence_.update(delta_time);

	//プレイヤーのアニメーションの処理
	GameManager::GetGameManager()->GetPlayer()->PlayerAnimation(delta_time);

	//歩数を数える
	//エンカウント処理
	if (ChackEncount(GameManager::GetGameManager()->GetPlayer()->GetStep())) {

		//BGMを止める
		SoundManager::GetSoundManager()->StopSound("sound/BGM/sfc-harukanaru-daichi.mp3");
		//バトル用のBGMを流す
		SoundManager::GetSoundManager()->Sound_Play("sound/BGM/sentou.mp3", DX_PLAYTYPE_LOOP);

		//0.5秒経過したらシーンを遷移させる
		if (GameManager::GetGameManager()->TimeCount(delta_time, koni::Numeric::SECONDS_0_5_F)) {

			auto mgr = SceneManager::GetSceneManager();

			//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
			if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

			//シーンを遷移させる(プレイヤーの座標を渡す,敵のID,敵のポインタを渡す)
			mgr->changeScene(new BattleScene(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), background_hdl, std::make_shared<MobMonster>(enemy_id)));
			//歩数を0にする
			GameManager::GetGameManager()->GetPlayer()->StepReset();
		}
	}

	//デバック用の更新処理(主に町などに遷移させる)
	DebugMapScene();

	//Inventoryの更新処理
	GameManager::GetGameManager()->GetInventory()->Update(delta_time);

	//共通処理
	__super::Update(delta_time);

}

//------------------------------------------------------------------------------------------------------------------------
//描画

void MapScene::Draw()
{
	//MapChipの描画
	for (auto& world_map : map_chips) {
		world_map->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	for (auto& world_object : map_chip_object) {
		world_object->Draw(*GameManager::GetGameManager()->GetCamera());
	}

	//プレイヤーの描画
	GameManager::GetGameManager()->GetPlayer()->Player_draw(*GameManager::GetGameManager()->GetCamera(), koni::Numeric::SCALE_ONE_F);

	//インベントリの描画
	GameManager::GetGameManager()->GetInventory()->draw();

	//共通描画
	__super::Draw();
}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

bool MapScene::seqMove(float delta_time)
{
	//アニメーションと歩数を0にする
	if (sequence_.isStart()) {
		GameManager::GetGameManager()->GetPlayer()->AnimationChange(Animation_default);
		GameManager::GetGameManager()->GetPlayer()->StepReset();
	}

	//プレイヤーの更新処理
	GameManager::GetGameManager()->GetPlayer()->Player_Move(delta_time, player_velocity, map_height);

	//カメラの更新処理
	GameManager::GetGameManager()->GetCamera()->update(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), map_width, map_height);


	//ESCを押したら、インベントリを表示する
	if (GameManager::GetGameManager()->GetInventory()->GetSelectMenuNum() == Inventory::MenuWindow_I::EMPTY && tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)
		&& GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
			
			//SEを流す
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			//インベントリを表示する
			GameManager::GetGameManager()->GetInventory()->InventoryMenuChange(Inventory::MenuWindow_I::FIRSTMENU);
			//plyerを動けないようにする
			GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
	}

	//当たり判定
	WorldMapCollision();

	return false;
}

//イベントを起こすシーケンス
bool MapScene::seqEvent(float delta_time)
{
	if (sequence_.isStart()) {

		//プレイヤーの動きを制限する
		GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();

		//イベントフラグがfalseの場合フラグを切り替える
		if (!EventManager::GetEventManager()->GetEventFlag()) {
			EventManager::GetEventManager()->EventFlagChange();
		}

	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {

		if (!GameManager::GetGameManager()->GetPlayer()->GetPlayerControl()) {
			//プレイヤーを動けるようにする
			GameManager::GetGameManager()->GetPlayer()->PlayerControlChangeFlag();
		}

		sequence_.change(&MapScene::seqMove);
	}

	//イベント発生時のNpcとの更新処理
	EventManager::GetEventManager()->NpcEventUpdate(delta_time, npc_list,this);

	return false;
}

bool MapScene::seqChangeScene(float delta_time)
{
	//カメラを保管座標に入れる
	GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());

	return true;
}

//シーケンスを外部で切り替える
void MapScene::SequenceChangeMove()
{
	//主にイベントマネージャーなどでイベントを切り替える時に使用
	//通常に戻す
	sequence_.change(&MapScene::seqMove);
}

//------------------------------------------------------------------------------------------------------------------------
//マップチップ関連

void MapScene::WorldMapCollision()
{
	//ワールドマップのcsv
	auto world_map_csv = CsvManager::GetCsvManager()->GetWorldMapCsv();

	//ワールドマップにあるオブジェクトのcsv
	auto world_object_csv = CsvManager::GetCsvManager()->GetWorldMapObjectCsv();

	//オブジェクトの当たり判定
	for (auto& MapChip : map_chip_object) {

		int mapChipValue = static_cast<int>(world_object_csv[static_cast<unsigned>(MapChip->MapChipPos().y / MapChip->GetChipSize().x)][static_cast<unsigned>(MapChip->MapChipPos().x / MapChip->GetChipSize().x)]);
		if (colisionObjectValues.count(mapChipValue) > 0 && tnl::IsIntersectRect(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().x, GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().y, MapChip->MapChipPos(), map_chip_width_, map_chip_height_))
		{
			//座標補正
			tnl::eCorrResRect Object_hit = tnl::CorrectPositionRect(
				GameManager::GetGameManager()->GetPlayer()->GetPlayerPrevPos(),
				MapChip->MapChipPos(),
				GameManager::GetGameManager()->GetPlayer()->GetPlayerSize(),
				MapChip->GetChipSize(),
				GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(),
				MapChip->MapChipPos(),
				tnl::eCorrTypeRect::PWRFL_B,
				tnl::eCorrTypeRect::PWRFL_B , 0);

		}

		//村のマップチップ
		const int MAP_CHIP_VILLAGE = 2035;

		//ボスの城のマップチップ
		const int MAP_CHIP_BOSS_CASTLE = 2090;

		//移動階段のマップチップ
		const int MAP_CHIP_STAIRS = 334;
		//二個目の移動階段のマップチップ
		const int MAP_CHIP_SECOND_STAIRS = 335;

		//町のマップチップ
		const int MAP_CHIP_TOWN = 2032;

		//城下町のマップチップ
		const int MAP_CHIP_CASTLE_TOWN = 2069;

		//勇者の墓の入口
		const int MAP_CHIP_GRAVE_ENTRANCE = 1247;

		if (ObjectValues.count(mapChipValue) > 0) {
			int Collision = tnl::IsIntersectRect(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().x, GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().y, MapChip->MapChipPos(), map_chip_width_, map_chip_height_);
			if (Collision) {

				//オブジェクトに当たった状態でEnterキーを押したら遷移させる
				//主に村や町、城、ボスの城などに遷移させる
				if (mapChipValue == MAP_CHIP_VILLAGE) {

						SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);

						//村シーンに切り替える
						auto mgr = SceneManager::GetSceneManager();

						//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
						if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

						//プレイヤーを一時的に見えなくする
						GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

						mgr->changeScene(new InMapScene(VILLAGE_POS, InMapScene::InMapState::VILLAGE), 0.1f);
						GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(VILLAGE_POS);
						sequence_.change(&MapScene::seqChangeScene);

						break;
					
				}
				//ボスの城だったら
				else if (mapChipValue == MAP_CHIP_BOSS_CASTLE) {
						SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/idou.mp3", DX_PLAYTYPE_BACK);
						//ダンジョンシーンに切り替える
						auto mgr = SceneManager::GetSceneManager();

						//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
						if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

						//プレイヤーを一時的に見えなくする
						GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

						mgr->changeScene(new InMapScene(BOSS_CASTLE_POS, InMapScene::InMapState::BOSSCASTLE), 0.1f ,0.8f);

						//座標をボスの場所にセットする
						GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(BOSS_CASTLE_POS);
						sequence_.change(&MapScene::seqChangeScene);

						break;
				}
				//移動階段だったら
				else if (mapChipValue == MAP_CHIP_STAIRS || mapChipValue == MAP_CHIP_SECOND_STAIRS) {

						auto mgr = SceneManager::GetSceneManager();

						if (mgr->GetSceneFlag()) {
							//フラグを切り替えてシーンを遷移させないようにする
							mgr->SceneFlagChange();
						}

						//フェードを入れる
						mgr->FadeScene(FADE_OUT_TIME, FADE_IN_TIME);

						//階段の座標
						tnl::Vector3 STAIRS_POS;

						//一つ目の階段だったら
						if (mapChipValue == 334) {

							STAIRS_POS = { 1476, 172, 0 };
						}
						//二つ目の階段だったら
						else if (mapChipValue == 335) {

							STAIRS_POS = { 909, 202, 0 };
						}

						//座標を階段の場所にセットする
						GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(STAIRS_POS);

						//座標をカメラの保管座標にセットする
						GameManager::GetGameManager()->GetCamera()->SavePosition(STAIRS_POS);

						break;
					
				}
				//町シーンへの遷移
				else if (mapChipValue == MAP_CHIP_TOWN) {

					//町シーンに切り替える
					auto mgr = SceneManager::GetSceneManager();

					//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
					if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

					//プレイヤーを一時的に見えなくする
					GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

					mgr->changeScene(new InMapScene(TOWN_POS, InMapScene::InMapState::TOWN), 0.1f);
					GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(TOWN_POS);
					sequence_.change(&MapScene::seqChangeScene);

					break;
				}
				//城下町シーンへの遷移
				else if (mapChipValue == MAP_CHIP_CASTLE_TOWN) {

					//城下町シーンに切り替える
					auto mgr = SceneManager::GetSceneManager();

					//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
					if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

					//プレイヤーを一時的に見えなくする
					GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

					mgr->changeScene(new InMapScene(CASTLE_TOWN_POS, InMapScene::InMapState::CASTLETOWN), 0.1f);
					GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(CASTLE_TOWN_POS);
					sequence_.change(&MapScene::seqChangeScene);

					break;
				}
				//勇者の墓シーンへ移行する
				else if (mapChipValue == MAP_CHIP_GRAVE_ENTRANCE) {

					//勇者の墓シーンに切り替える
					auto mgr = SceneManager::GetSceneManager();

					//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
					if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

					//プレイヤーを一時的に見えなくする
					GameManager::GetGameManager()->GetPlayer()->PlayerDisplayChange();

					mgr->changeScene(new InMapScene(GRAVE_POS, InMapScene::InMapState::GRAVE), 0.1f);
					GameManager::GetGameManager()->GetPlayer()->SetPlayerPosition(GRAVE_POS);
					sequence_.change(&MapScene::seqChangeScene);

					break;

				}

			}
		}

	}
	//地形の当たり判定
	for (auto& worldMap_C : map_chips) {

		int mapChipValue = world_map_csv[static_cast<unsigned>(worldMap_C->MapChipPos().y / worldMap_C->GetChipSize().x)][static_cast<unsigned>(worldMap_C->MapChipPos().x / worldMap_C->GetChipSize().x)];
	
		//エンカウント用の当たり判定
		if (encount_kind.count(mapChipValue)&& tnl::IsIntersectRect(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().x, GameManager::GetGameManager()->GetPlayer()->GetPlayerSize().y, worldMap_C->MapChipPos(), map_chip_width_, map_chip_height_)) {

			//明るい平原
			const int LUMINOUS_GRASSLAND = 2;

			//少し暗い平原
			const int TWILIGHT_GRASSLAND = 0;

			//暗い平原
			const int SHADOW_GRASSLAND = 3;

			//雪原
			const int SNOW_FIELD = 1320;

			//荒原
			const int WILDERNESS[2] = { 1 , 6 };

			//ボスエリア
			const int BOSS_AREA = 5;

			//map上のどこに居るかによって敵を変える
			//平原にいる場合
			if (mapChipValue == LUMINOUS_GRASSLAND) {

				//最初の敵のIDを渡す
				enemy_id = 1;

				//mapChipに応じて背景を変える
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/battle_field_01.jpg");
			}
			else if (mapChipValue == TWILIGHT_GRASSLAND || mapChipValue == SHADOW_GRASSLAND) {

				//平原の敵のIDを渡す
				enemy_id = 2;

				//mapChipに応じて背景を変える
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/battle_field_02.jpg");
			}
			//荒原にいる場合
			else if (mapChipValue == WILDERNESS[0] || mapChipValue == WILDERNESS[1]) {

				//荒原にいる敵のIDを渡す
				enemy_id = 3;
 
				//mapChipに応じて背景を変える
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/battle_field_03.jpg");
			}
			//雪原にいる場合
			else if (mapChipValue == SNOW_FIELD) {

				//荒原にいる敵のIDを渡す
				enemy_id = 4;

				//mapChipに応じて背景を変える
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg005b.jpg");

			}
			//ボスエリアの場合
			else if (mapChipValue == BOSS_AREA) {

				//ボスエリアにいる敵のIDを渡す
				enemy_id = 5;

				//mapChipに応じて背景を変える
				background_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/pipo-battlebg014b.png");
			}
		}
	}

}

//エンカウント
bool MapScene::ChackEncount(const int& step)
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
	auto world_map_csv = CsvManager::GetCsvManager()->GetWorldMapCsv();

	//ワールドマップにあるオブジェクトのcsv
	auto world_object_csv = CsvManager::GetCsvManager()->GetWorldMapObjectCsv();

	map_chip_all_size_ = map_chip_x_size_ * map_chip_y_size_;

	LoadDivGraph(gpc_map_chip_hdls_pass_.c_str(),
		map_chip_all_size_,
		map_chip_x_size_,
		map_chip_y_size_,
		map_chip_width_,
		map_chip_height_,
		gpc_map_chip_hdls_);

	//平原のマップチップの読み込み
	for (int i = 0; i < world_map_csv.size(); ++i) {
		for (int k = 0; k < world_map_csv[i].size(); ++k) {
			if (-1 == world_map_csv[i][k]) continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / koni::Numeric::DIVIDER_TWO + k * mapchip->GetChipSize().x) };
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x) };
			gpc_hdl = gpc_map_chip_hdls_[world_map_csv[i][k]];

			map_chips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}

	//オブジェクトのマップチップの読み込み
	for (int i = 0; i < world_object_csv.size(); ++i) {
		for (int k = 0; k < world_object_csv[i].size(); ++k) {
			if (-1 == world_object_csv[i][k])continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { static_cast<float>(mapchip->GetChipSize().x / koni::Numeric::DIVIDER_TWO + k * mapchip->GetChipSize().x) };
			pos.y = { static_cast<float>(i * mapchip->GetChipSize().x) };
			gpc_hdl = gpc_map_chip_hdls_[world_object_csv[i][k]];

			map_chip_object.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));
		}
	}

}
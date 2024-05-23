#include "village.h"
#include"../Manager/SceneManager.h"
#include"battleScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"mapScene.h"
#include<iostream>

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

//コンストラクタ
VillageScene::VillageScene(tnl::Vector3 firstPos ): village_Pos(firstPos)
{

	//初期化処理
	InitVillageScene();

	//マップチップのロード
	mapChip_Loding();

	//デバック用
	/*player->getPlayerStatusSave().SetPlayerCurentHp(player->getPlayerStatusSave().getcurentHp() - 3);*/

}

VillageScene::~VillageScene()
{
	//サウンドを消去する
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/decision.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/idou.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/BGM/tasogare.mp3");

	//マップチップの解放(1992 = マップチップの総数)
	for (int i = 0; i < 1992; i++) {
		DeleteGraph(gpc_map_chip_hdls_[i]);
		tnl::DebugTrace("Deleted  MapChipimage at [%d]. Pointer value: %p\n", i, gpc_map_chip_hdls_[i]);
	}
}

//初期化
void VillageScene::InitVillageScene()
{
	
	//Playerの初期位置を決める
	GameManager::getGameManager()->getPlayer()->SetPlayerPosition(village_Pos);

	//武器関連の初期化
	weapon_shop = std::make_shared<WeaponShop>();
	GameManager::getGameManager()->getInventory()->SetPlayer(GameManager::getGameManager()->getPlayer());

	//インベントリの初期化
	weapon_shop->setInventory(GameManager::getGameManager()->getInventory());


	//NPCの初期化
	herdman_npc = std::make_shared<Npc>("村長", tnl::Vector3(1375, 760, 0), herdman_hdl_pass, 1.5f, 0, 4);
	Armsdealer_npc = std::make_shared<Npc>("武器商人", tnl::Vector3(1150, 140, 0), Armsdealer_hdl_pass, 1.5f, 0, 1);
	Priest_npc = std::make_shared<Npc>("神官", tnl::Vector3(239, 740, 0), Priest_hdl_pass, 1.5f, 0, 3);

	//メニューの初期化
	InitMenuWindow();

	//soundManagerの初期化
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tasogare.mp3", DX_PLAYTYPE_LOOP);

	//マップチップの初期化
	mapchip = std::make_shared<MapChip>();

}


//---メニューの初期化---//
void VillageScene::InitMenuWindow()
{

	//通常のメニューウィンドウ
	menuWindow = std::make_shared<Menu>(50, 50, 300, 270, "graphics/WindowBase_01.png");

	//メニューの選択ウィンドウ
	menuSelectWindow = std::make_shared<Menu>(50, 50, 250, 200, "graphics/WindowBase_01.png");

	//コメント表示の際のメニューウィンドウ
	menuComentWindiow = std::make_shared<Menu>(50, 500, 700, 200, "graphics/WindowBase_01.png");

}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void VillageScene::Update(float delta_time)
{
	//Player
	GameManager::getGameManager()->getPlayer()->player_Move(delta_time, PlayerVelocity);
	 
	//シーケンス
	sequence_.update(delta_time);

	//camera
	GameManager::getGameManager()->getCamera()->update(GameManager::getGameManager()->getPlayer()->getPlayerPos(), MAP_WIDTH, MAP_HEIGHT);

	//npc
	herdman_npc->Update(delta_time);
	Armsdealer_npc->Update(delta_time);
	Priest_npc->Update(delta_time);


	//Inventory
	/*GameManager::getGameManager()->UpdateCommon(delta_time);*/
	GameManager::getGameManager()->getInventory()->Update(delta_time);
		
	//当たり判定
	mapChip_collision();

}

//------------------------------------------------------------------------------------------------------------------------
//描画

void VillageScene::Draw()
{

	//---mapChipの描画---//
	for (auto mapchip : MapChips) {
		mapchip->Draw(*GameManager::getGameManager()->getCamera());
	}

	//---plyerの描画---//
	GameManager::getGameManager()->getPlayer()->player_draw(*GameManager::getGameManager()->getCamera(), PlyerScale);

	//---Npcの描画---//
	herdman_npc->Draw(*GameManager::getGameManager()->getCamera());
	Armsdealer_npc->Draw(*GameManager::getGameManager()->getCamera());
	Priest_npc->Draw(*GameManager::getGameManager()->getCamera());

	//それぞれのシーケンスでの描画
	switchSequenceDraw();

	//インベントリの描画
	GameManager::getGameManager()->getInventory()->draw();

	////カメラターゲットの座標
	//DrawStringEx(100, 100, -1, "%f", GameManager::getGameManager()->getCamera()->getTargetPos().x);
	//DrawStringEx(100, 150, -1, "%f", GameManager::getGameManager()->getCamera()->getTargetPos().y);
} 

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

bool VillageScene::seqIdle(float delta_time)
{
	if (sequence_.isStart()) {

		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlTrue);
		select_num = Sequence_num::Idle;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
		//決定音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
		//インベントリを表示する
		GameManager::getGameManager()->getInventory()->SetSelect_num(first_menu);
	}

	//プレイヤーの座標がマップ外に出たらシーンを切り替える
	if (GameManager::getGameManager()->getPlayer()->getPlayerPos().x >= MAP_WIDTH || GameManager::getGameManager()->getPlayer()->getPlayerPos().x < 0 || GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= MAP_HEIGHT /*tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)*/)
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
			GameManager::getGameManager()->getPlayer()->SetPlayerPosition(first_world_pos);
			sequence_.change(&VillageScene::seqChangeScene);
			auto mgr = SceneManager::GetInstance();
			mgr->changeScene(new MapScene(), 1.0f, 3.0f);
		}
	}

	return true;
}

//武器屋
bool VillageScene::seqWeaponShop(float delta_time)
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
		//プレイヤーを動けるようにする
		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlTrue);
		//シーンを切り替える
		sequence_.change(&VillageScene::seqIdle);
	}
	return true;
}

//シーンを変える時のフェードアウトを入れる為
bool VillageScene::seqChangeScene(float delta_time)
{
	return true;
}

//メニュー
bool VillageScene::seqMenuOpen(float delta_time)
{
	if (sequence_.isStart())
	{
		//プレイヤーを動けなくする
		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlFalse);
	}

	if (EventManager::getEventManager()->isNpcMenuOpen()) {
		if(tnl::Input::IsKeyDownTrigger(eKeys::KB_1)){
			sequence_.change(&VillageScene::seqNpc);
		}
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		sequence_.change(&VillageScene::seqIdle);
		//最初の態勢と違っていたら戻す
		if (herdman_npc->getAnimationSelect() != 0) {
			herdman_npc->NpcAnimationReset();
			herdman_npc->ChengeAnimation();
		}
		//プレイヤーを動けるようにする
		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlTrue);
	}

	return false;
}

//Npcの処理
bool VillageScene::seqNpc(float delta_time)
{
	if (sequence_.isStart()) {
		select_num = Sequence_num::MenuNpc;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_0)) {
		sequence_.change(&VillageScene::seqIdle);
		//プレイヤーを動けるようにする
		GameManager::getGameManager()->getPlayer()->setPlayerControl(PlyControlTrue);
		//コメントを表示できるようにする
		NpcComntTrue = true;
	}

	return false;
}

//シーケンス毎の描画
void VillageScene::switchSequenceDraw()
{
	switch (select_num)
	{
		//通常シーケンス
	case VillageScene::Sequence_num::Idle:
		
		//DrawStringEx(menuWindow->menu_x + 50, menuWindow->menu_y + 350, -1, "seqIdle");

		break;

		//武器屋のシーケンスの時
	case VillageScene::Sequence_num::WeaponShop:

		menuComentWindiow->Menu_draw();

		//コメントとアイコン
		ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/Npc/Armsdealer_icon.png", 90, 600, 1.5f, 0, true);

		//武器屋のコメントを表示する
	    EventManager::getEventManager()->armsdealerComentDraw(armsdealer_comet);

		//武器屋の描画
		weapon_shop->Draw();

		//プレイヤーの所持金を描画
		DrawStringEx(135, 100, -1, "所持金: %d", GameManager::getGameManager()->getPlayer()->getPlayerMoney());

		/*DrawStringEx(menuWindow->menu_x + 350, menuWindow->menu_y + 350, -1, "seqWeaponShop");*/

		break;

		//メニューシーケンスの時
	case VillageScene::Sequence_num::MenuOpen:

		//Npcのコメント表示
		EventManager::getEventManager()->NpcComentDraw();

		break;

		//武器屋シーケンスの時
	case VillageScene::Sequence_num::MenuShopOpen:

		//武器屋のコメント表示
		EventManager::getEventManager()->NpcComentDraw();

		break;

		//Npcシーケンスの時(Npcによって動作を変える)
	case VillageScene::Sequence_num::MenuNpc:

		EventManager::getEventManager()->NpcSwitchComent();

		break;


	default:
		break;
	}
}

//------------------------------------------------------------------------------------------------------------------------
//マップチップ関連

//マップチップのロード
void VillageScene::mapChip_Loding()
{
	// マップチップの画像のロード
	gpc_map_chip_hdls_pass_ = "graphics/BaseChip_pipo.png";

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

	// マップデータのロード
	map_data_csv_pass_ = "csv/map_chip_first_village.csv";
	map_csv = tnl::LoadCsv<int>(map_data_csv_pass_);

	//マップチップ読み込み
	for (int i = 0; i < map_csv.size(); ++i) {
		for (int k = 0; k < map_csv[i].size(); ++k) {
			if (-1 == map_csv[i][k]) continue;
			tnl::Vector3 pos;
			int gpc_hdl;
			pos.x = { mapchip->getChipSize() / 2 + k * mapchip->getChipSize()};
			pos.y = { i * mapchip->getChipSize()};
			gpc_hdl = gpc_map_chip_hdls_[map_csv[i][k]];
			/*DrawGraph(pos.x, pos.y, gpc_hdl, true);*/
			MapChips.emplace_back(std::make_shared<MapChip>(pos, gpc_hdl));

		}
	}
}

//あたり判定の処理
void VillageScene::mapChip_collision()
{
	 //ブロックと衝突判定 & 補正
	 //ゲームループ内での当たり判定のチェック]
	for (auto mapChip : MapChips) {
		int mapChipValue = map_csv[mapChip->MapChipPos().y / mapChip->getChipSize()][mapChip->MapChipPos().x / mapChip->getChipSize()];
		if(colisionValues.count(mapChipValue) > 0)
		{
			int collision = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition(GameManager::getGameManager()->getPlayer()->getPlayerPos(), GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth), GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight), mapChip->MapChipPos(), map_chip_width_, map_chip_height_);
			//あたり判定が作用した際にその番号が510番だった場合、windowを表示する
			if (collision) {
					//武器屋と話した場合
					if (mapChipValue == map_Chip_WeaponShop) {
						if (select_num == Sequence_num::Idle && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
							//シーケンスを切り替える
							EventManager::getEventManager()->SetMenuNpc(2);
							EventManager::getEventManager()->SetNpcComent(0);
							select_num = Sequence_num::MenuShopOpen;
							sequence_.change(&VillageScene::seqMenuOpen);
							
						}
						if (select_num == Sequence_num::MenuShopOpen && tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) {
							//シーケンスを切り替える
							sequence_.change(&VillageScene::seqWeaponShop);
						}
					}

					//村長と話した時
					if (mapChipValue == map_Chip_Herdman) {
						if (NpcComntTrue) {
							if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
								select_num = Sequence_num::MenuOpen;
								EventManager::getEventManager()->SetMenuNpc(0);
								EventManager::getEventManager()->SetNpcComent(1);
								sequence_.change(&VillageScene::seqMenuOpen);
								NpcComntTrue = false;
							}
						}
					}
					//オブジェクトに触れたとき
					if (mapChipValue == map_Chip_Object) {
						if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
							select_num = Sequence_num::MenuOpen;
							EventManager::getEventManager()->SetMenuNpc(3);
							sequence_.change(&VillageScene::seqMenuOpen);
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
					GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth),
					GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight),
					herdman_npc->getNpcPos(),
					herdman_npc->getNpcSize(npcWidth),
					herdman_npc->getNpcSize(npcHeight)
				);


	if (h_colition) {

		//下から当たった時
		bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= herdman_npc->getNpcPos().y + herdman_npc->getNpcSize(npcHeight);

		if (playerOnBottom) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				//アニメーションを下にする
				herdman_npc->SetNpcAnimation(1);
				herdman_npc->ChengeAnimation();
				select_num = Sequence_num::MenuOpen;
				EventManager::getEventManager()->SetMenuNpc(0);
				EventManager::getEventManager()->SetNpcComent(1);
				sequence_.change(&VillageScene::seqMenuOpen);
			}
		}
	}

	//神官
	int P_collision = ResourceManager::getResourceManager()->IsIntersectRectToCorrectPosition
	(
		GameManager::getGameManager()->getPlayer()->getPlayerPos(),
		GameManager::getGameManager()->getPlayer()->getPlayerPrevPos(),
		GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerWidth),
		GameManager::getGameManager()->getPlayer()->getPlayerSize(PlyerHeight),
		Priest_npc->getNpcPos(),
		Priest_npc->getNpcSize(npcWidth),
		Priest_npc->getNpcSize(npcHeight)
	);


	if (P_collision) {
		//下から当たった時
		bool playerOnBottom = GameManager::getGameManager()->getPlayer()->getPlayerPos().y >= Priest_npc->getNpcPos().y + Priest_npc->getNpcSize(npcHeight);
		
		if (playerOnBottom) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
				Priest_npc->NpcAnimationReset();
				Priest_npc->ChengeAnimation();
				select_num = Sequence_num::MenuOpen;
				EventManager::getEventManager()->SetMenuNpc(1);
				EventManager::getEventManager()->SetNpcComent(2);
				sequence_.change(&VillageScene::seqMenuOpen);
			}
		}
	}

}

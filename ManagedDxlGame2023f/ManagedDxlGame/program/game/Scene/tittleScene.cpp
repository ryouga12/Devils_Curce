#include "tittleScene.h"
#include"../Manager/UiManager.h"
#include"InMapScene.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

TittleScene::TittleScene()
{
	//現在のシーンをTittleSceneに設定
	curent_scene = SceneState::TITTLE ;

	menu_window = std::make_shared<Menu>("graphics/WindowBase_01.png");

	menu_window_white = std::make_shared<Menu>("graphics/WindowBase_02.png");
	
	UIManager::getUIManager()->addMenu("menu_window", menu_window);
	UIManager::getUIManager()->addMenu("menu_window_white", menu_window_white);

	//選択ウィンドウの初期化
	tittle_selection_coment = new MenuWindow::MenuElement_t[]{
		{SELECTION_START_X , SELECTION_START_Y , "最初からはじめる" , 0},
		{SELECTION_START_X , SELECTION_START_Y + SELECTION_OFFSET_Y , "続きからはじめる" , 1},
	};

	title_selection = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", tittle_selection_coment, SELECTION_NUM, 1);
	title_selection->Open();

	//ポインタをリセットする
	GameManager::getGameManager()->Reset();
	
	//ゲームマネージャーのポインタを生成する
	GameManager::getGameManager()->PoiterCreate();

}

TittleScene::~TittleScene()
{
	//サウンドを止める
	SoundManager::getSoundManager()->StopSound("sound/BGM/tittle_bgm.mp3");
}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void TittleScene::Update(float delta_time)
{
	//シーケンスの更新処理
	sequence_.update(delta_time);

}

//------------------------------------------------------------------------------------------------------------------------
//描画

void TittleScene::Draw()
{

	switch (curent_tittle)
	{
		//タイトルシーン
	case TittleScene::TittleState::TITTLE:

		//背景描画
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tittle_ghdl, TRUE);

		//タイトルメニューの要素
		title_selection->All(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);

		SetFontSize(60);

		DrawStringEx(450, 200, -1, "Devils curse");

		SetFontSize(16);

		break;

		//キャラメイクシーン
		//後ほど追加予定
	case TittleScene::TittleState::CHARAMIKE:



		break;

	default:

		break;
	}

}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

//初期化シーケンス
bool TittleScene::seqInit(float delta_time)
{
	switch (curent_tittle)
	{
	case TittleScene::TittleState::TITTLE:

		tittle_ghdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/tittle_background.png");

		enter_key_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/button_Enter.png");

		SoundManager::getSoundManager()->sound_Play("sound/BGM/tittle_bgm.mp3", DX_PLAYTYPE_LOOP);

		sequence_.change(&TittleScene::seqTittle);

		break;

	case TittleScene::TittleState::CHARAMIKE:

		break;

	default:
		break;
	}


	return false;
}

//タイトル内の更新処理
bool TittleScene::seqTittle(float delta_time)
{
	//はじめから始める
	if (title_selection->getSelectNum() == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SceneChange = true;
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);
		//シーケンスを切り替える
		sequence_.change(&TittleScene::seqCharaMike);
	}
	//続きから始める
	else if(title_selection->getSelectNum() == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)){
		GameManager::getGameManager()->getPlayer()->PlyerLoad();
		SceneChange = true;
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);
	}

	//1秒間SEを流してから次のシーンへ遷移する
	if (SceneChange && title_selection->getSelectNum() == 1)
	{
		GameManager::getGameManager()->TimeCount(delta_time , Time);
		sequence_.change(&TittleScene::seqChangeScene);
		auto mgr = SceneManager::GetInstance();

		auto player_pos = GameManager::getGameManager()->getPlayer()->getPlayerPos();

		mgr->changeScene(new InMapScene(player_pos, static_cast<InMapScene::InMapState>(GameManager::getGameManager()->getPlayer()->GetPlayerCurentMapState())));

		//プレイヤーの座標をカメラに保管する
		GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());
	}
	else if (SceneChange) {
		GameManager::getGameManager()->TimeCount(delta_time, Time);
		sequence_.change(&TittleScene::seqChangeScene);
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new InMapScene(first_pos_, InMapScene::InMapState::VILLAGE));

	}
	return false;
	
}

//キャラメイク内の更新処理
//後ほど追加予定
bool TittleScene::seqCharaMike(float delta_time)
{
	return false;
}



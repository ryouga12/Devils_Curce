#include "tittleScene.h"
#include"../Manager/UiManager.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

TittleScene::TittleScene()
{

	menu_window = std::make_shared<Menu>("graphics/WindowBase_01.png");

	menu_window_white = std::make_shared<Menu>("graphics/WindowBase_02.png");
	
	UIManager::getUIManager()->addMenu("menu_window", menu_window);
	UIManager::getUIManager()->addMenu("menu_window_white", menu_window_white);


	tittle_ghdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/tittle_background.png");

	enter_key_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/button_Enter.png");
	
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tittle_bgm.mp3", DX_PLAYTYPE_LOOP);

	//ポインタをリセットする
	GameManager::getGameManager()->Reset();
	
	//ゲームマネージャーのポインタを生成する
	GameManager::getGameManager()->PoiterCreate();

}

TittleScene::~TittleScene()
{
	//サウンドを止める
	SoundManager::getSoundManager()->StopSound("sound/BGM/tittle_bgm.mp3");
	SoundManager::getSoundManager()->StopSound("sound/SoundEffect/tittle_enter.mp3");

}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void TittleScene::Update(float delta_time)
{

	sequence_.update(delta_time);

}

//------------------------------------------------------------------------------------------------------------------------
//描画

void TittleScene::Draw()
{
	//背景描画
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tittle_ghdl, TRUE);

	//タイトルメニューの描画
	menu_window_white->Menu_draw(525, 500, 250, 80);

	//タイトルメニューの要素
	DrawRotaGraph(650, 525, 1, 0, enter_key_hdl ,true);
	DrawStringEx(535, 550, String_Color_Black, "Enterを押してゲームスタート");
}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

bool TittleScene::seqTittle(float delta_time)
{

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SceneChange = true;
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);
	}

	//1秒間SEを流してから次のシーンへ遷移する
	if (SceneChange)
	{
		GameManager::getGameManager()->TimeCount(delta_time , Time);
		sequence_.change(&TittleScene::seqChangeScene);
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new InMapScene(first_pos_ , InMapScene::InMapState::VILLAGE));
	}

	return true;
	
}



#include "tittleScene.h"


//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

TittleScene::TittleScene()
{

	tittle_Menu = std::make_shared<Menu>(525, 500, 250, 80, "graphics/WindowBase_02.png");

	tittle_ghdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/tittle_background.png");
	
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tittle_bgm.mp3", DX_PLAYTYPE_LOOP);


	/*enemy = std::make_shared<Enemy>();*/

}

TittleScene::~TittleScene()
{
	//サウンドを消去する
	SoundManager::getSoundManager()->daleteSound("sound/BGM/tittle_bgm.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/tittle_enter.mp3");

	//画像を消去する
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/tittle_background.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/button_Enter.png");

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
	tittle_Menu->Menu_draw();


	/*DrawStringEx(10, 10, -1, "タイトル画面");*/

	//タイトルメニューの要素
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/button_Enter.png", tittle_Menu->menu_x + 125, tittle_Menu->menu_y + 25, 1, 0, true);
	DrawStringEx(tittle_Menu->menu_x + 10, tittle_Menu->menu_y + 50, String_Color_Black, "Enterを押してゲームスタート");
}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

bool TittleScene::seqIdle(float delta_time)
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
		mgr->changeScene(new VillageScene(first_pos_));
	}

	return true;
	
}



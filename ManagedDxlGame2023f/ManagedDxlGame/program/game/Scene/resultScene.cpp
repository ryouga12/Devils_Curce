#include "resultScene.h"
#include"../Manager/UiManager.h"
#include"tittleScene.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

ResultScene::ResultScene()
{
	//現在のシーンをResultSceneに設定
	curent_scene = SceneState::RESULT;

	risult_menu = std::make_shared<Menu>("graphics/WindowBase_02.png");

	//mapを追加する
	UIManager::getUIManager()->addMenu("risult_window", risult_menu);

	//enterkeyボタン
	enter_key = ResourceManager::getResourceManager()->LoadGraphEX("graphics/button_Enter.png");


	SoundManager::getSoundManager()->sound_Play("sound/BGM/requiem.mp3", DX_PLAYTYPE_LOOP);
}

ResultScene::~ResultScene()
{
	//サウンドを止める
	SoundManager::getSoundManager()->StopSound("sound/BGM/requiem.mp3");
;
}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void ResultScene::Update(float delta_time)
{
	sequence_.update(delta_time);
}

//------------------------------------------------------------------------------------------------------------------------
//描画

void ResultScene::Draw()
{
	/*DrawStringEx(10, 10, -1, "リザルト画面");*/

	SetFontSize(100);
	DrawStringEx(450 , 200, -1, "全滅した");
	SetFontSize(16);

	risult_menu->Menu_draw(525, 500, 250, 80);

	//リザルトメニューの要素
	DrawRotaGraph(650, 525, 1, 0, enter_key, true);
	DrawStringEx(555, 550, 0, "Enterを押してください");
}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

bool ResultScene::seqIdle(float delta_time)
{
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new (TittleScene));
	}
	return true;
}

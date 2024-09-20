#include "resultScene.h"
#include"../Manager/UiManager.h"
#include"tittleScene.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

ResultScene::ResultScene()
{
	//現在のシーンをResultSceneに設定
	curent_scene = SceneState::RESULT;
}

ResultScene::~ResultScene()
{
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
	
	//半透明にする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_50_PERCENT);
	//背景描画
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, result_background, TRUE);
	//アルファ値を戻す
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_OPAQUE);

	//エピローグを表示する
	UIManager::GetUIManager()->StoryDisplay(koni::Color::WHITE);

}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

bool ResultScene::seqIdle(float delta_time)
{
	//上から少しずつ表示する
	UIManager::GetUIManager()->StoryDisplayUpdate(delta_time);

	//エピローグをを流しきるか、タブキーを押してスキップしてタイトルシーンに遷移させる
	if (UIManager::GetUIManager()->StoryDisplayUpdate(delta_time) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		|| tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) {

		auto mgr = SceneManager::GetSceneManager();
		mgr->changeScene(new (TittleScene));
	}
	
	return true;
}

bool ResultScene::seqInit(float delta_time)
{	
	//背景をロードする
	result_background = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/kaidou0331_800b.jpg");

	//ストーリーをロードする
	UIManager::GetUIManager()->StoryLoad(Story::EPILOGUE);

	//シーケンスを切り替える
	sequence_.change(&ResultScene::seqIdle);

	return false;
}

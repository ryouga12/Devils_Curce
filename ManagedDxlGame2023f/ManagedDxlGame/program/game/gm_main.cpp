#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"
#include"Scene/tittleScene.h"
#include"../game/Manager/EventManager.h"
#include"../game/Manager/GameManager.h"
#include"../game/Manager/ResourceManager.h"
#include"../game/Manager/SceneManager.h"
#include"../game/Manager/SoundManager.h"
#include"../game/Manager/UiManager.h"



//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(static_cast<unsigned>(time(0)));
	
	//最初にタイトルシーンに設定
	SceneManager::GetSceneManager(new TittleScene());
}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	//シーンの更新処理と描画処理とシーケンス処理を呼び出す
	SceneManager::GetSceneManager()->update(delta_time);
}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
	
	//シングルトンの解放
	EventManager::DelateEventManager();
	GameManager::DelateGameManager();
	ResourceManager::DelategetResourceManager();
	SceneManager::DeleteSceneManager();
	SoundManager::DelateSoundManager();
	UIManager::DeleteGetUIManager();
}

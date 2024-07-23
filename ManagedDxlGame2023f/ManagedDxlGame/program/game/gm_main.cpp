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
	SetBackgroundColor(32, 32, 32);

	SceneManager::GetInstance(new TittleScene());
}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {


	SceneManager::GetInstance()->update(delta_time);

	//DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
	
	//シングルトンの解放
	EventManager::delateEventManager();
	GameManager::delateGameManager();
	ResourceManager::delategetResourceManager();
	SceneManager::Destroy();
	SoundManager::delateSoundManager();
	UIManager::delategetUIManager();
}

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
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(static_cast<unsigned>(time(0)));
	SetBackgroundColor(32, 32, 32);

	SceneManager::GetInstance(new TittleScene());
}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {


	SceneManager::GetInstance()->update(delta_time);

	//DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
	
	//�V���O���g���̉��
	EventManager::delateEventManager();
	GameManager::delateGameManager();
	ResourceManager::delategetResourceManager();
	SceneManager::Destroy();
	SoundManager::delateSoundManager();
	UIManager::delategetUIManager();
}

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
	
	//�ŏ��Ƀ^�C�g���V�[���ɐݒ�
	SceneManager::GetSceneManager(new TittleScene());
}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	//�V�[���̍X�V�����ƕ`�揈���ƃV�[�P���X�������Ăяo��
	SceneManager::GetSceneManager()->update(delta_time);
}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
	
	//�V���O���g���̉��
	EventManager::DelateEventManager();
	GameManager::DelateGameManager();
	ResourceManager::DelategetResourceManager();
	SceneManager::DeleteSceneManager();
	SoundManager::DelateSoundManager();
	UIManager::DeleteGetUIManager();
}

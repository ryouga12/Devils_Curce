#include "tittleScene.h"


//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

TittleScene::TittleScene()
{

	tittle_Menu = std::make_shared<Menu>(525, 500, 250, 80, "graphics/WindowBase_02.png");

	tittle_ghdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/tittle_background.png");
	
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tittle_bgm.mp3", DX_PLAYTYPE_LOOP);


	/*enemy = std::make_shared<Enemy>();*/

}

TittleScene::~TittleScene()
{
	//�T�E���h����������
	SoundManager::getSoundManager()->daleteSound("sound/BGM/tittle_bgm.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/tittle_enter.mp3");

	//�摜����������
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/tittle_background.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/button_Enter.png");

}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void TittleScene::Update(float delta_time)
{
	sequence_.update(delta_time);
	
	
}

//------------------------------------------------------------------------------------------------------------------------
//�`��

void TittleScene::Draw()
{
	//�w�i�`��
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tittle_ghdl, TRUE);

	//�^�C�g�����j���[�̕`��
	tittle_Menu->Menu_draw();


	/*DrawStringEx(10, 10, -1, "�^�C�g�����");*/

	//�^�C�g�����j���[�̗v�f
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/button_Enter.png", tittle_Menu->menu_x + 125, tittle_Menu->menu_y + 25, 1, 0, true);
	DrawStringEx(tittle_Menu->menu_x + 10, tittle_Menu->menu_y + 50, String_Color_Black, "Enter�������ăQ�[���X�^�[�g");
}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

bool TittleScene::seqIdle(float delta_time)
{

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SceneChange = true;
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);
	}

	//1�b��SE�𗬂��Ă��玟�̃V�[���֑J�ڂ���
	if (SceneChange)
	{
		GameManager::getGameManager()->TimeCount(delta_time , Time);
		sequence_.change(&TittleScene::seqChangeScene);
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new VillageScene(first_pos_));
	}

	return true;
	
}



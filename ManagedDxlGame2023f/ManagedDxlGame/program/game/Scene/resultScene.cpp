#include "resultScene.h"


//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

ResultScene::ResultScene()
{
	risult_menu = std::make_shared<Menu>(525, 500, 250, 80, "graphics/WindowBase_02.png");

	SoundManager::getSoundManager()->sound_Play("sound/BGM/requiem.mp3", DX_PLAYTYPE_LOOP);
}

ResultScene::~ResultScene()
{
	//�T�E���h����������
	SoundManager::getSoundManager()->daleteSound("sound/BGM/requiem.mp3");
	//�摜����������
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/button_Enter.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/cur_sannkaku2.png");
	ResourceManager::getResourceManager()->deleteGraphEx("graphics/WindowBase_02.png");

}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void ResultScene::Update(float delta_time)
{
	sequence_.update(delta_time);
}

//------------------------------------------------------------------------------------------------------------------------
//�`��

void ResultScene::Draw()
{
	/*DrawStringEx(10, 10, -1, "���U���g���");*/

	SetFontSize(100);
	DrawStringEx(450 , 200, -1, "�S�ł���");
	SetFontSize(16);

	risult_menu->Menu_draw();

	//���U���g���j���[�̗v�f
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/button_Enter.png", risult_menu->menu_x + 125, risult_menu->menu_y + 25, 1, 0, true);
	DrawStringEx(risult_menu->menu_x + 30, risult_menu->menu_y + 50, 0, "Enter�������Ă�������");
}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

bool ResultScene::seqIdle(float delta_time)
{
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new (TittleScene));
	}
	return true;
}

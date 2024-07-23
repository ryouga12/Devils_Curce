#include "resultScene.h"
#include"../Manager/UiManager.h"
#include"tittleScene.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

ResultScene::ResultScene()
{
	//���݂̃V�[����ResultScene�ɐݒ�
	curent_scene = SceneState::RESULT;

	risult_menu = std::make_shared<Menu>("graphics/WindowBase_02.png");

	//map��ǉ�����
	UIManager::getUIManager()->addMenu("risult_window", risult_menu);

	//enterkey�{�^��
	enter_key = ResourceManager::getResourceManager()->LoadGraphEX("graphics/button_Enter.png");


	SoundManager::getSoundManager()->sound_Play("sound/BGM/requiem.mp3", DX_PLAYTYPE_LOOP);
}

ResultScene::~ResultScene()
{
	//�T�E���h���~�߂�
	SoundManager::getSoundManager()->StopSound("sound/BGM/requiem.mp3");
;
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

	risult_menu->Menu_draw(525, 500, 250, 80);

	//���U���g���j���[�̗v�f
	DrawRotaGraph(650, 525, 1, 0, enter_key, true);
	DrawStringEx(555, 550, 0, "Enter�������Ă�������");
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

#include "resultScene.h"
#include"../Manager/UiManager.h"
#include"tittleScene.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

ResultScene::ResultScene()
{
	//���݂̃V�[����ResultScene�ɐݒ�
	curent_scene = SceneState::RESULT;
}

ResultScene::~ResultScene()
{
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
	
	//�������ɂ���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_50_PERCENT);
	//�w�i�`��
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, result_background, TRUE);
	//�A���t�@�l��߂�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_OPAQUE);

	//�G�s���[�O��\������
	UIManager::GetUIManager()->StoryDisplay(koni::Color::WHITE);

}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

bool ResultScene::seqIdle(float delta_time)
{
	//�ォ�班�����\������
	UIManager::GetUIManager()->StoryDisplayUpdate(delta_time);

	//�G�s���[�O���𗬂����邩�A�^�u�L�[�������ăX�L�b�v���ă^�C�g���V�[���ɑJ�ڂ�����
	if (UIManager::GetUIManager()->StoryDisplayUpdate(delta_time) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		|| tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) {

		auto mgr = SceneManager::GetSceneManager();
		mgr->changeScene(new (TittleScene));
	}
	
	return true;
}

bool ResultScene::seqInit(float delta_time)
{	
	//�w�i�����[�h����
	result_background = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/kaidou0331_800b.jpg");

	//�X�g�[���[�����[�h����
	UIManager::GetUIManager()->StoryLoad(Story::EPILOGUE);

	//�V�[�P���X��؂�ւ���
	sequence_.change(&ResultScene::seqIdle);

	return false;
}

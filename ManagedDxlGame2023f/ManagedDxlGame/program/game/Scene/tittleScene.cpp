#include "tittleScene.h"
#include"../Manager/UiManager.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

TittleScene::TittleScene()
{

	menu_window = std::make_shared<Menu>("graphics/WindowBase_01.png");

	menu_window_white = std::make_shared<Menu>("graphics/WindowBase_02.png");
	
	UIManager::getUIManager()->addMenu("menu_window", menu_window);
	UIManager::getUIManager()->addMenu("menu_window_white", menu_window_white);


	tittle_ghdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/tittle_background.png");

	enter_key_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/button_Enter.png");
	
	SoundManager::getSoundManager()->sound_Play("sound/BGM/tittle_bgm.mp3", DX_PLAYTYPE_LOOP);

	//�|�C���^�����Z�b�g����
	GameManager::getGameManager()->Reset();
	
	//�Q�[���}�l�[�W���[�̃|�C���^�𐶐�����
	GameManager::getGameManager()->PoiterCreate();

}

TittleScene::~TittleScene()
{
	//�T�E���h���~�߂�
	SoundManager::getSoundManager()->StopSound("sound/BGM/tittle_bgm.mp3");
	SoundManager::getSoundManager()->StopSound("sound/SoundEffect/tittle_enter.mp3");

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
	menu_window_white->Menu_draw(525, 500, 250, 80);

	//�^�C�g�����j���[�̗v�f
	DrawRotaGraph(650, 525, 1, 0, enter_key_hdl ,true);
	DrawStringEx(535, 550, String_Color_Black, "Enter�������ăQ�[���X�^�[�g");
}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

bool TittleScene::seqTittle(float delta_time)
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
		mgr->changeScene(new InMapScene(first_pos_ , InMapScene::InMapState::VILLAGE));
	}

	return true;
	
}



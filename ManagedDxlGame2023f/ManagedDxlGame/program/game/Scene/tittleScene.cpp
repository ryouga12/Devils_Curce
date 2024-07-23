#include "tittleScene.h"
#include"../Manager/UiManager.h"
#include"InMapScene.h"

//------------------------------------------------------------------------------------------------------------------------
//�������@&  ���

TittleScene::TittleScene()
{
	//���݂̃V�[����TittleScene�ɐݒ�
	curent_scene = SceneState::TITTLE ;

	menu_window = std::make_shared<Menu>("graphics/WindowBase_01.png");

	menu_window_white = std::make_shared<Menu>("graphics/WindowBase_02.png");
	
	UIManager::getUIManager()->addMenu("menu_window", menu_window);
	UIManager::getUIManager()->addMenu("menu_window_white", menu_window_white);

	//�I���E�B���h�E�̏�����
	tittle_selection_coment = new MenuWindow::MenuElement_t[]{
		{SELECTION_START_X , SELECTION_START_Y , "�ŏ�����͂��߂�" , 0},
		{SELECTION_START_X , SELECTION_START_Y + SELECTION_OFFSET_Y , "��������͂��߂�" , 1},
	};

	title_selection = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", tittle_selection_coment, SELECTION_NUM, 1);
	title_selection->Open();

	//�|�C���^�����Z�b�g����
	GameManager::getGameManager()->Reset();
	
	//�Q�[���}�l�[�W���[�̃|�C���^�𐶐�����
	GameManager::getGameManager()->PoiterCreate();

}

TittleScene::~TittleScene()
{
	//�T�E���h���~�߂�
	SoundManager::getSoundManager()->StopSound("sound/BGM/tittle_bgm.mp3");
}

//------------------------------------------------------------------------------------------------------------------------
//�X�V����

void TittleScene::Update(float delta_time)
{
	//�V�[�P���X�̍X�V����
	sequence_.update(delta_time);

}

//------------------------------------------------------------------------------------------------------------------------
//�`��

void TittleScene::Draw()
{

	switch (curent_tittle)
	{
		//�^�C�g���V�[��
	case TittleScene::TittleState::TITTLE:

		//�w�i�`��
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tittle_ghdl, TRUE);

		//�^�C�g�����j���[�̗v�f
		title_selection->All(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);

		SetFontSize(60);

		DrawStringEx(450, 200, -1, "Devils curse");

		SetFontSize(16);

		break;

		//�L�������C�N�V�[��
		//��قǒǉ��\��
	case TittleScene::TittleState::CHARAMIKE:



		break;

	default:

		break;
	}

}

//------------------------------------------------------------------------------------------------------------------------
//�V�[�P���X�֘A

//�������V�[�P���X
bool TittleScene::seqInit(float delta_time)
{
	switch (curent_tittle)
	{
	case TittleScene::TittleState::TITTLE:

		tittle_ghdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/tittle_background.png");

		enter_key_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/button_Enter.png");

		SoundManager::getSoundManager()->sound_Play("sound/BGM/tittle_bgm.mp3", DX_PLAYTYPE_LOOP);

		sequence_.change(&TittleScene::seqTittle);

		break;

	case TittleScene::TittleState::CHARAMIKE:

		break;

	default:
		break;
	}


	return false;
}

//�^�C�g�����̍X�V����
bool TittleScene::seqTittle(float delta_time)
{
	//�͂��߂���n�߂�
	if (title_selection->getSelectNum() == 0 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		SceneChange = true;
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);
		//�V�[�P���X��؂�ւ���
		sequence_.change(&TittleScene::seqCharaMike);
	}
	//��������n�߂�
	else if(title_selection->getSelectNum() == 1 && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)){
		GameManager::getGameManager()->getPlayer()->PlyerLoad();
		SceneChange = true;
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);
	}

	//1�b��SE�𗬂��Ă��玟�̃V�[���֑J�ڂ���
	if (SceneChange && title_selection->getSelectNum() == 1)
	{
		GameManager::getGameManager()->TimeCount(delta_time , Time);
		sequence_.change(&TittleScene::seqChangeScene);
		auto mgr = SceneManager::GetInstance();

		auto player_pos = GameManager::getGameManager()->getPlayer()->getPlayerPos();

		mgr->changeScene(new InMapScene(player_pos, static_cast<InMapScene::InMapState>(GameManager::getGameManager()->getPlayer()->GetPlayerCurentMapState())));

		//�v���C���[�̍��W���J�����ɕۊǂ���
		GameManager::getGameManager()->getCamera()->SavePosition(GameManager::getGameManager()->getPlayer()->getPlayerPos());
	}
	else if (SceneChange) {
		GameManager::getGameManager()->TimeCount(delta_time, Time);
		sequence_.change(&TittleScene::seqChangeScene);
		auto mgr = SceneManager::GetInstance();
		mgr->changeScene(new InMapScene(first_pos_, InMapScene::InMapState::VILLAGE));

	}
	return false;
	
}

//�L�������C�N���̍X�V����
//��قǒǉ��\��
bool TittleScene::seqCharaMike(float delta_time)
{
	return false;
}



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
	
	UIManager::GetUIManager()->addMenu("menu_window", menu_window);
	UIManager::GetUIManager()->addMenu("menu_window_white", menu_window_white);

	//�I���E�B���h�E�̏�����
	tittle_selection_coment = new MenuWindow::MenuElement_t[]{
		{SELECTION_START_X , SELECTION_START_Y ,					  "�ŏ�����͂��߂�" , MenuWindow::Elements::FIRST_ELEMENT},
		{SELECTION_START_X , SELECTION_START_Y + SELECTION_OFFSET_Y , "��������͂��߂�" , MenuWindow::Elements::SECOND_ELEMENT},
	};

	title_selection = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", tittle_selection_coment, SELECTION_NUM);
	title_selection->Open();

	//�|�C���^�����Z�b�g����
	GameManager::GetGameManager()->Reset();
	
	//�Q�[���}�l�[�W���[�̃|�C���^�𐶐�����
	GameManager::GetGameManager()->PoiterCreate();

}

TittleScene::~TittleScene()
{
	//�T�E���h���~�߂�
	SoundManager::GetSoundManager()->StopSound("sound/BGM/tittle_.mp3");
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
	//��{�^���̍��W
	const tnl::Vector2i TOP_BUTTON_POS = { 450, 500 };

	//���{�^���̍��W
	const tnl::Vector2i UNDER_BUTTON_POS = { 450 , 580 };

	//���{�^���̍��W
	const tnl::Vector2i LEFT_BUTTON_POS = { 350, 650 };

	//�E�{�^���̍��W
	const tnl::Vector2i RIGHT_BUTTON_POS = { 450 , 650 };

	//�㉺���̍��W(�{�^��������̍�)
	const tnl::Vector2i BUTTON_ARROW_OFFSET = { 15 , 60 };

	//���E�{�^���̃I�t�Z�b�g���W
	const tnl::Vector2i ARROW_LEFT_AND_RIGHT_OFFSET = { 30 , 7 };

	//����{�^���̕`����W
	const tnl::Vector2i ENTER_DECISION_DRAW_POS = { 200 , 650 };

	//Enter�L�[�̌��蕶���̍��W
	const tnl::Vector2i ENTER_DECISION_STRING_POS = { 250 , 643 };

	// �v���C���[�̉摜��I�����Ă��������Ƃ������b�Z�[�W�̍��W
	const tnl::Vector2i PLAYER_SELECT_PROMPT_POS = {450 , 50};

	switch (curent_tittle)
	{
		//�^�C�g���V�[��
	case TittleScene::TittleState::TITTLE:

		//�w�i�`��
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tittle_ghdl, TRUE);

		//�^�C�g�����j���[�̗v�f
		title_selection->All(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
		
		//�^�C�g��������\������
		//DrawStringToHandle(TITTLE_STRING_POS.x, static_cast<int>(title_y), "Devils Curse", koni::Color::WHITE, UIManager::GetUIManager()->GetFontString_80());
		DrawRotaGraph(TITTLE_STRING_POS.x, static_cast<int>(title_y), koni::Numeric::SCALE_ONE_F, 0, font_hdl, true);

		//��{�^������A�C�R��
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_w.png", TOP_BUTTON_POS.x, TOP_BUTTON_POS.y, koni::Numeric::SCALE_ONE_THIRTY_F, 0, true);
		//����̕`��
		DrawStringToHandle(TOP_BUTTON_POS.x - BUTTON_ARROW_OFFSET.x, TOP_BUTTON_POS.y - BUTTON_ARROW_OFFSET.y, "��", koni::Color::WHITE , UIManager::GetUIManager()->GetFontString_30());
		//���{�^������A�C�R��
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_s.png", UNDER_BUTTON_POS.x, UNDER_BUTTON_POS.y, koni::Numeric::SCALE_ONE_THIRTY_F, 0, true);
		//�����̕`��
		DrawStringToHandle(UNDER_BUTTON_POS.x - BUTTON_ARROW_OFFSET.x, UNDER_BUTTON_POS.y + BUTTON_ARROW_OFFSET.y, "��", koni::Color::WHITE, UIManager::GetUIManager()->GetFontString_30());


		break;

		//�L�������C�N�V�[��
	case TittleScene::TittleState::CHARAIMAGE:

		//�J�[�\����\������ 
		DrawRotaGraph(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].x, PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].y - 150, koni::Numeric::SCALE_ONE_F, tnl::ToRadian(90), cousour, true);

		//�摜��\������
		for (int i = 0; i < PLAYER_IMAGE_MAX; i++) {
			DrawRotaGraph(PlAYER_IMAGE_SELECT_POS[i].x, PlAYER_IMAGE_SELECT_POS[i].y, koni::Numeric::SCALE_TRIPLE_F, 0, player_image_hdl[i], true);
		}

		//���{�^������A�C�R��
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_a.png", LEFT_BUTTON_POS.x, LEFT_BUTTON_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//�������̕`��
		DrawString(LEFT_BUTTON_POS.x + ARROW_LEFT_AND_RIGHT_OFFSET.x, LEFT_BUTTON_POS.y - ARROW_LEFT_AND_RIGHT_OFFSET.y, "��", koni::Color::WHITE);

		//�E�{�^������A�C�R��
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_d.png", RIGHT_BUTTON_POS.x, RIGHT_BUTTON_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//�E�����̕`��
		DrawString(RIGHT_BUTTON_POS.x + ARROW_LEFT_AND_RIGHT_OFFSET.x, RIGHT_BUTTON_POS.y - ARROW_LEFT_AND_RIGHT_OFFSET.y, "�E", koni::Color::WHITE);

		//Enter�L�[�̌��蕶���̕`��
		DrawString(PLAYER_SELECT_PROMPT_POS.x, PLAYER_SELECT_PROMPT_POS.y, "�v���C���[�̉摜��I�����Ă�������", koni::Color::WHITE);

		//����{�^���̕`��
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/button_Enter.png", ENTER_DECISION_DRAW_POS.x, ENTER_DECISION_DRAW_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//Enter�L�[�̌��蕶���̕`��
		DrawString(ENTER_DECISION_STRING_POS.x, ENTER_DECISION_STRING_POS.y, "����", koni::Color::WHITE);

		break;

	case TittleScene::TittleState::CHARANAME:

		//�摜��\������
		for (int i = 0; i < PLAYER_IMAGE_MAX; i++) {
			DrawRotaGraph(PlAYER_IMAGE_SELECT_POS[i].x, PlAYER_IMAGE_SELECT_POS[i].y, koni::Numeric::SCALE_TRIPLE_F, 0, player_image_hdl[i], true);
		}

		//���{�^������A�C�R��
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_a.png", LEFT_BUTTON_POS.x, LEFT_BUTTON_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//�������̕`��
		DrawString(LEFT_BUTTON_POS.x + ARROW_LEFT_AND_RIGHT_OFFSET.x, LEFT_BUTTON_POS.y - ARROW_LEFT_AND_RIGHT_OFFSET.y, "��", koni::Color::WHITE);

		//�E�{�^������A�C�R��
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_d.png", RIGHT_BUTTON_POS.x, RIGHT_BUTTON_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//�E�����̕`��
		DrawString(RIGHT_BUTTON_POS.x + ARROW_LEFT_AND_RIGHT_OFFSET.x, RIGHT_BUTTON_POS.y - ARROW_LEFT_AND_RIGHT_OFFSET.y, "�E", koni::Color::WHITE);

		//����{�^���̕`��
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/button_Enter.png", ENTER_DECISION_DRAW_POS.x, ENTER_DECISION_DRAW_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//Enter�L�[�̌��蕶���̕`��
		DrawString(ENTER_DECISION_STRING_POS.x, ENTER_DECISION_STRING_POS.y, "����", koni::Color::WHITE);

		if (!player_input_name_flag) {

			//���O���͂̍��W
			const tnl::Vector2i NAME_INPUT_STRING_POS = { 500 , 500 };

			//�E�B���h�E�̂̍��W
			const tnl::Vector2i NAME_INPUT_WINDOW_POS = { 490 , 480 };

			//�E�B���h�E�̃T�C�Y
			const tnl::Vector2i NAME_INPUT_WINDOW_SIZE = { 200 , 50 };

			//���O����͂��Ă��������Ƃ��������̍��W
			const tnl::Vector2i NAME_STRING_POS = {450 , 450};

			// �v���C���[�摜���͂ރ{�b�N�X�̃I�t�Z�b�g�i���ƍ����j
			const int PLAYER_SELECT_BOX_OFFSET_ = 80;

			// �I�������v���C���[�̉摜���͂ވׂ�BOX�̍��W�ƃT�C�Y
			const tnl::Vector3 PLAYER_SELECT_BOX_POS_TOP_LEFT = {
				static_cast<float>(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].x - PLAYER_SELECT_BOX_OFFSET_),
				static_cast<float>(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].y - PLAYER_SELECT_BOX_OFFSET_),
				0
			};
			const tnl::Vector3 PLAYER_SELECT_BOX_POS_BOTTOM_RIGHT = {
				static_cast<float>(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].x + PLAYER_SELECT_BOX_OFFSET_),
				static_cast<float>(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].y + PLAYER_SELECT_BOX_OFFSET_),
				0
			};

			// ���O����͂��Ă��������Ƃ������O�̕`��
			DrawString(NAME_STRING_POS.x, NAME_STRING_POS.y, "�v���C���[�̖��O����͂��Ă�������", koni::Color::WHITE);

			//�g�̑���
			const float THICKNESS = 3.0f;

			// �I�������v���C���[��DrawBox�ň͂�ł킩��₷������
			DrawBoxAA(
				PLAYER_SELECT_BOX_POS_TOP_LEFT.x,
				PLAYER_SELECT_BOX_POS_TOP_LEFT.y,
				PLAYER_SELECT_BOX_POS_BOTTOM_RIGHT.x,
				PLAYER_SELECT_BOX_POS_BOTTOM_RIGHT.y,
				koni::Color::WHITE, 
				// �h��Ԃ��Ȃ�
				FALSE,				
				THICKNESS
			);

			//�E�B���h�E��\������
			UIManager::GetUIManager()->Menu_Draw("menu_window", NAME_INPUT_WINDOW_POS.x, NAME_INPUT_WINDOW_POS.y, NAME_INPUT_WINDOW_SIZE.x, NAME_INPUT_WINDOW_SIZE.y);

			//�v���C���[�����͂ł��镶���̒���
			const int PLAYER_NAME_LENGTH = 8;

			//�v���C���[�̖��O���󂯕t����
			KeyInputString(NAME_INPUT_STRING_POS.x, NAME_INPUT_STRING_POS.y, PLAYER_NAME_LENGTH, temp, FALSE);

			//���O��ۑ�����
			player_input_name_flag = true;
		}

		break;

		//�v�����[�O
	case TittleScene::TittleState::PROLOGUE:

		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_50_PERCENT);
		//�w�i�`��
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tittle_ghdl, TRUE);
		//�A���t�@�l��߂�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_OPAQUE);

		//�X�g�[���[��`�悷��
		UIManager::GetUIManager()->StoryDisplay(koni::Color::WHITE);

		break;

	default:

		break;
	}

	if (warnig_message) {

		//�E�B���h�E��\������
		UIManager::GetUIManager()->GetMenu("menu_window")->Menu_draw(50, 100, 500, 200);
		//������\������
		UIManager::GetUIManager()->WarningWindow("�t�@�C�������݂��܂���B");
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

		//�^�C�g���̔w�i�̉摜���i�[����
		tittle_ghdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/tittle_background.png");
		font_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/DevilsCurse.png");
		//handle = ResourceManager::getResourceManager()->LoadGraphEX("graphics/tittle_string.png");
		//�G���^�[�L�[�̉摜���i�[����
		enter_key_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/button_Enter.png");
		
		SoundManager::GetSoundManager()->Sound_Play("sound/BGM/tittle_.mp3", DX_PLAYTYPE_LOOP);

		sequence_.change(&TittleScene::seqTittle);

		break;

	case TittleScene::TittleState::CHARAIMAGE:

		//�J�[�\���̉摜���i�[����
		cousour = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/cur_sannkaku2.png");

		//�v���C���[�̉摜���i�[����
		for (int i = 0; i < PLAYER_IMAGE_MAX; i++) {
			player_image_hdl[i] = ResourceManager::GetResourceManager()->LoadGraphEX(PLAYER_HDL[i]);
		}

		sequence_.change(&TittleScene::seqCharaMike);

		break;

	case TittleScene::TittleState::PROLOGUE:

		tittle_ghdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/prologue_.jpg");

		sequence_.change(&TittleScene::seqPrologue);

		break;

	default:
		break;
	}


	return false;
}

//�^�C�g�����̍X�V����
bool TittleScene::seqTittle(float delta_time)
{
	//---�^�C�g�������̃A�j���[�V��������---//
	elapsed_time += delta_time;

	if (elapsed_time < DURATION) {

		//�������ڕW�Ɍ������ē�����
		title_y = tnl::SmoothLerp(0.0f, static_cast<float>(TITTLE_STRING_POS.y), DURATION, elapsed_time);
	}

	//---�I������---//

	//�͂��߂���n�߂�
	if (title_selection->GetSelectNum() == MenuWindow::Elements::FIRST_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);

		//�x���������\������Ă���ꍇ�A�x������������
		if (warnig_message) { warnig_message = false; }

		auto mgr = SceneManager::GetSceneManager();

		if (mgr->GetSceneFlag()) {
			//�t���O��؂�ւ��ăV�[����J�ڂ����Ȃ��悤�ɂ���
			mgr->SceneFlagChange();
		}

		//�t�F�[�h������
		mgr->FadeScene(FADE_OUT_TIME);

		//�C�x���g�֘A�̃t���O�����Z�b�g����
		EventManager::GetEventManager()->EventFlagReset();

		//�L�������C�N�Ɉړ�������
		//�܂��摜�̑I�����s���Ă��炤
		curent_tittle = TittleState::CHARAIMAGE;
		sequence_.change(&TittleScene::seqInit);

	}
	//��������n�߂�
	else if(title_selection->GetSelectNum() == MenuWindow::Elements::SECOND_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)){

		//Enter�L�[��SE��炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);

		if (GameManager::GetGameManager()->GetPlayer()->PlayerLoad()) {

			sequence_.change(&TittleScene::seqChangeScene);
			auto mgr = SceneManager::GetSceneManager();

			auto& player = GameManager::GetGameManager()->GetPlayer();

			mgr->changeScene(new InMapScene(player->GetPlayerPos(), static_cast<InMapScene::InMapState>(GameManager::GetGameManager()->GetPlayer()->GetPlayerCurentMapState())));

			//�v���C���[�̉摜���m�肷��
			player->SetPlayerAnimationHdl(Player::PlayerEquipState::BARE_HANDS, player->GetPlayerID());

			//�v���C���[�̍��W���J�����ɕۊǂ���
			GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
		}
		//�t�@�C�����Ȃ������ꍇ�A�x����\������
		else {

			//�x��������\������
			warnig_message = true;
		}
	}

	return false;
	
}

//�L�������C�N
//��قǒǉ��\��
bool TittleScene::seqCharaMike(float delta_time)
{
	if (curent_tittle == TittleState::CHARAIMAGE) {

		//�J�[�\���̓���
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_A)) {
			curent_player_image_hdl--;
			//se���Ȃ炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
			//��ԍ���荶�ɍs���ƈ�ԉE�Ɉړ�����
			if (curent_player_image_hdl < 0) curent_player_image_hdl = PLAYER_IMAGE_WOMAN_1;
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_D)) {
			curent_player_image_hdl++;
			//se���Ȃ炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
			//��ԍ���荶�ɍs���ƈ�ԉE�Ɉړ�����
			if (curent_player_image_hdl >= PLAYER_IMAGE_MAX) curent_player_image_hdl = PLAYER_IMAGE_MAN_0;
		}
		//�G���^�[�L�[�������ꂽ��v���C���[��ID�����肷��
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//�v���C���[���擾����
			auto& player = GameManager::GetGameManager()->GetPlayer();

			//�v���C���[��ID���Z�b�g����(�v���C���[�̉摜���w�肷���)
			player->SetPlayerID(curent_player_image_hdl);

			//��Ԃ��v���C���[�̖��O�ݒ�Ɉړ�����
			curent_tittle = TittleState::CHARANAME;
		}
	}
	else if (curent_tittle == TittleState::CHARANAME) {

		//�v���C���[�̖��O�����͂��I�������
		if (player_input_name_flag) {

			//�v���C���[���擾����
			auto& player = GameManager::GetGameManager()->GetPlayer();

			//�v���C���[�̖��O��ݒ肷��
			player->SetPlayerName(temp);

			//�v���C���[�̉摜���m�肷��
			player->SetPlayerAnimationHdl(Player::PlayerEquipState::BARE_HANDS, player->GetPlayerID());

			//�V�[�P���X���v�����[�O�Ɉړ�����
			sequence_.change(&TittleScene::seqInit);

			curent_tittle = TittleState::PROLOGUE;

			player_input_name_flag = false;
		}
		
	}

	return false;
}

//�v�����[�O
bool TittleScene::seqPrologue(float delta_time)
{
	if (sequence_.isStart()) {
		//�v�����[�O�̃��[�h
		UIManager::GetUIManager()->StoryLoad(Story::PROLOGUE);
		
	}

	//�v�����[�O�𗬂����邩�A�^�u�L�[�������ăX�L�b�v���đ��V�[���ɑJ�ڂ�����
	if (UIManager::GetUIManager()->StoryDisplayUpdate(delta_time) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		|| tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) {
		sequence_.change(&TittleScene::seqChangeScene);
		auto mgr = SceneManager::GetSceneManager();

		//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(FIRST_POS_, InMapScene::InMapState::VILLAGE));
		return false;
	}

	return false;
}



#include "UiManager.h"
#include"CsvManager.h"
#include"GameManager.h"
#include"ResourceManager.h"

UIManager* UIManager::GetUIManager()
{
	static UIManager* p_instance = nullptr;

	if (!p_instance)p_instance = new UIManager();
	return p_instance;
}

void UIManager::DeleteGetUIManager()
{
	delete GetUIManager();
}

void UIManager::addMenu(const std::string& menuName, const std::shared_ptr<Menu>& menu)
{
	//���j���[�}�b�v�ɂ��邩���m�F����
	auto it = menu_map.find(menuName);

	//���łɂ���Ώ������΂�
	if (it != menu_map.end()) {
		return;
	}
	//����������Ηv�f��ǉ�����
	else {
		menu_map[menuName] = menu;
	}

}

void UIManager::Menu_Draw(const std::string& menuName, const int&  menu_x, const int& menu_y, const int&  menu_width, const int& menu_height)
{
	//���O����������
	auto it = menu_map.find(menuName);

	//����������`�悷��
	if (it != menu_map.end()) {
		(*it).second->Menu_draw(menu_x, menu_y, menu_width, menu_height);
	}
	//������Ȃ������珈�����΂�
	else {
		return;
	}

}

//�R�����g�����[�h����
void UIManager::ComentLoad(const int& max_num , const std::string& name)
{
	//��x�R�����g������������
	coment_.clear();

	auto coment_csv = CsvManager::GetCsvManager()->GetComentCsv();

	for (int i = 1; i < coment_csv.size(); i++) {

		//���O��csv��1�s�ڂׁ̈A0
		if (name == coment_csv[i][0].c_str()) {

			for (int k = 1; k < max_num; k++) {

				coment_.emplace_back(coment_csv[i][k]);

			}
		}

	}
}

//�R�����g��`�悷��
void UIManager::ComentDraw(const tnl::Vector2i& coment_pos)
{
	//�R�����g�`��
	DrawStringEx(coment_pos.x, coment_pos.y, koni::Color::WHITE, "%s", coment_[curent_num].c_str());

	//�����\��������W
	const int ADD_X = 380;  const int ADD_Y = 50;

	//���镶���̕`��
	DrawStringEx(coment_pos.x + ADD_X, coment_pos.y + ADD_Y, koni::Color::WHITE, "(back space : ����)");

}

//�R�����g�����Ɉړ�����
void UIManager::ComentNextByInput(const int& max_draw_num)
{
	//�`��̍ő吔�𒴂��ĂȂ����
	if (count < max_draw_num) {
		//Enter�L�[���󂯕t����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//SE��炷
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			//���𑝂₷
			curent_num++;
			//�J�E���g�𑝂₷
			count++;
		}
	}
}

// �v���C���[�̃X�e�[�^�X��HP�o�[�ŕ\������
void UIManager::PlayerStatusDrawWindow()
{
	// �v���C���[�A�C�R���̈ʒu
	const tnl::Vector2i PLAYER_ICON_POS = { 90 ,120 };

	// HP�o�[�̈ʒu�A���A����
	const tnl::Vector2i HP_BAR_POS = { 75 ,155 };
	const int HP_BAR_WIDTH = 75;
	const int HP_BAR_HEIGHT = 30;

	// MP�o�[�̈ʒu�A���A����
	const tnl::Vector2i MP_BAR_POS = { 75 ,215 };
	const int MP_BAR_WIDTH = 75;
	const int MP_BAR_HEIGHT = 30;

	// �v���C���[���\���̈ʒu
	const tnl::Vector2i PLAYER_INFO_POS = { 75 ,70 };
	const tnl::Vector2i LEVEL_INFO_POS = { 120 ,110 };
	const int HP_INFO_Y = 190;
	const int MP_INFO_Y = 250;

	// �v���C���[�̃X�e�[�^�X���擾����
	auto& player = GameManager::GetGameManager()->GetPlayer();

	// �v���C���[�̉摜��`��
	DrawRotaGraph(PLAYER_ICON_POS.x, PLAYER_ICON_POS.y, koni::Numeric::SCALE_ONE_THIRTY_F, 0, player->GetPlayerIcon(), true);

	// HP�o�[�̉摜�����[�h
	int hpber_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/hpbar.png");
	// MP�o�[�̉摜�����[�h
	int mpber_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/mpbar.png");

	// �v���C���[��HP��3����؂����ꍇ�AHP�o�[��ԐF�ɕύX
	if (player->GetPlayerStatusSave().GetcurentHp() < player->GetPlayerStatusSave().GetMaxHp() * koni::Numeric::PERCENT_30) {
		hpber_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/red1_.png");
	}

	// HP�o�[��`��
	DrawExtendGraph(HP_BAR_POS.x, HP_BAR_POS.y, (HP_BAR_POS.x + static_cast<int>(current_hp_bar)), HP_BAR_POS.y + HP_BAR_HEIGHT, hpber_hdl, true);
	// MP�o�[��`��
	DrawExtendGraph(MP_BAR_POS.x, MP_BAR_POS.y, (MP_BAR_POS.x + static_cast<int>(current_mp_bar)), MP_BAR_POS.y + MP_BAR_HEIGHT, mpber_hdl, true);

	// �v���C���[��HP����\��
	DrawStringEx(PLAYER_INFO_POS.x, PLAYER_INFO_POS.y, koni::Color::WHITE, "%s" , GameManager::GetGameManager()->GetPlayer()->GetPlayerName().c_str());
	DrawStringEx(LEVEL_INFO_POS.x, LEVEL_INFO_POS.y, koni::Color::WHITE, " Lv %d", player->GetPlayerStatusSave().GetLevel());
	DrawStringEx(PLAYER_INFO_POS.x, HP_INFO_Y, koni::Color::WHITE, " Hp : %d / %d", player->GetPlayerStatusSave().GetcurentHp(), player->GetPlayerStatusSave().GetMaxHp());
	DrawStringEx(PLAYER_INFO_POS.x, MP_INFO_Y, koni::Color::WHITE, " Mp : %d / %d", player->GetPlayerStatusSave().GetCurentMp(), player->GetPlayerStatusSave().GetMaxMp());

	//�X�e�[�^�X�A�C�R��(�U��)
	const int STATUS_ATTACK_ICON = GetStatusAttackIconHdl();
	//�X�e�[�^�X�A�C�R��(�h��)
	const int STATUS_DEFANCE_ICON = GetStatusDefanceIconHdl();
	//�X�e�[�^�X�̖����l(��L�̒萔�Ɣ�r���Ēl���ϓ����Ă��邩���m�F�����)
	const int STATUS_NON_CHANGE = 0;
	//�X�e�[�^�X�A�C�R��(�U��)��x���W�I�t�Z�b�g
	const int STATUS_ATTACK_X_OFFSET = 110;
	//�X�e�[�^�X�A�C�R��(�h��)��x���W�I�t�Z�b�g
	const int STATUS_DEFANCE_X_OFFSET = 160;

	//�X�e�[�^�X���ϓ����Ă����
	if (STATUS_ATTACK_ICON > STATUS_NON_CHANGE) {

		// �X�e�[�^�X�̃A�C�R����`��
		DrawRotaGraph(PLAYER_ICON_POS.x + STATUS_ATTACK_X_OFFSET, PLAYER_ICON_POS.y, koni::Numeric::SCALE_ONE_F, 0, STATUS_ATTACK_ICON, true);

	}
	//�X�e�[�^�X���ϓ����Ă����
	if (STATUS_DEFANCE_ICON > STATUS_NON_CHANGE) {

		// �X�e�[�^�X�̃A�C�R����`��
		DrawRotaGraph(PLAYER_ICON_POS.x + STATUS_DEFANCE_X_OFFSET, PLAYER_ICON_POS.y, koni::Numeric::SCALE_ONE_F, 0, STATUS_DEFANCE_ICON, true);

	}

}


//�Z�[�u�������̕�����\��������
void UIManager::SaveText(const tnl::Vector2i& text_pos)
{
	//�t���[���̍ő�l
	const int MAX_FLAME = 60;

	//�Z�[�u���Ă��鎞��
	if (save_flag) {

		//�t���[�����C���N�������g
		frame++;

		DrawStringEx(text_pos.x, text_pos.y, koni::Color::WHITE, "�Z�[�u���ł��B");

		//�t���[������60�𒴂�����
		if (frame >= MAX_FLAME) {

			//�t���O��؂�ւ���
			save_flag = false;

		}

	}
	//�t���[������60�𒴂��Ă��t���O�������Ă��Ȃ�������
	else if (frame >= MAX_FLAME && !save_flag) {

		//�Z�[�u�������������̃e�L�X�g���o��
		DrawStringEx(text_pos.x, text_pos.y, koni::Color::WHITE , "�Z�[�u���܂����B");
	}

}

//�v���C���[�̑������
void UIManager::PlayerMoveDetail(const std::vector<std::string>& detail_text)
{
	if (player_detail_window_flag) {

		//���j���[�E�B���h�E�̍��W
		const tnl::Vector2i DETAIL_WINDOW_POS = { 850, 300 };

		//���j���[�E�B���h�E�̃T�C�Y
		const int DETAIL_WINDOW_WIDTH = 400;
		const int DETAIL_WINDOW_HEIGHT = 400;

		//������������̍��W
		const tnl::Vector2i MOVE_DETAIL_STRING_POS = { 900, 350 };

		Menu_Draw("menu_window",DETAIL_WINDOW_POS.x, DETAIL_WINDOW_POS.y, DETAIL_WINDOW_WIDTH, DETAIL_WINDOW_HEIGHT);

		for (int i = 0; i < detail_text.size(); i++)
		{
			DrawStringEx(MOVE_DETAIL_STRING_POS.x, MOVE_DETAIL_STRING_POS.y + (i * ADD_OFSET), koni::Color::WHITE, "%s", detail_text[i].c_str());
		}
	}
}

//HP�o�[��MP�o�[�̌v�Z
void UIManager::PlayerStatusBarUpdate(const float delta_time)
{
	// �v���C���[�̃X�e�[�^�X���擾����
	auto& playerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();

	// HP�o�[�ő�̒���
	int maxHpBar = 200;
	// MP�o�[�̍ő�̒���
	int maxMpBer = 200;

	// HP�̊������v�Z����
	float hp_ratio = static_cast<float>(playerStatus.GetcurentHp()) / playerStatus.GetMaxHp();
	float newHpBar = maxHpBar * hp_ratio;

	// HP�o�[�̕�Ԃ��X�V
	if (std::fabs(newHpBar - hp_end) > FLT_EPSILON) {
		hp_start = current_hp_bar;
		hp_end = newHpBar;
		hp_lerp_time = 0.0f;
	}

	hp_lerp_time += delta_time;

	hp_lerp_time = (hp_lerp_time > LERPDURATION) ? LERPDURATION : hp_lerp_time;

	current_hp_bar = tnl::SmoothLerp(hp_start, hp_end, LERPDURATION, hp_lerp_time, 0);

	// MP�̊������v�Z����
	float mp_ratio = static_cast<float>(playerStatus.GetCurentMp()) / playerStatus.GetMaxMp();
	float newMpBar = maxMpBer * mp_ratio;

	// MP�o�[�̕�Ԃ��X�V
	if (std::fabs(newMpBar - mp_end) > FLT_EPSILON) {
		mp_start = current_mp_bar;
		mp_end = newMpBar;
		mp_lerp_time = 0.0f;
	}
	mp_lerp_time += delta_time; 

	mp_lerp_time = (mp_lerp_time > LERPDURATION) ? LERPDURATION : mp_lerp_time;

	current_mp_bar = tnl::SmoothLerp(mp_start, mp_end, LERPDURATION, mp_lerp_time, 0);
}

//�X�g�[���[�𗬂�
bool UIManager::StoryDisplayUpdate(const float delta_time)
{
	story_display_timer += delta_time;

	// �^�C�}�[���Ԋu�𒴂����玟�̃R�����g�ɒǉ�
	if (story_display_timer > koni::Numeric::SECONDS_1_AND_HALF) {
		story_display_timer = 0.0f; // �^�C�}�[�����Z�b�g

		// �X�g�[���[����łȂ��ꍇ�A���̃R�����g��ǉ�
		if (current_story_comment_index < story_.size()) {
			displayed_story_coments.push_back(story_[current_story_comment_index]);
			current_story_comment_index++;
		}

	}
	else {

		// �R�����g���S�ĕ\�����ꂽ��true��Ԃ�
		if (current_story_comment_index >= story_.size()) {
			
			story_end_flag = true;

			return true;
		}
	}

	return false;
}

//�X�g�[���[�̕`��
void UIManager::StoryDisplay(const int& font_color)
{
	// ����Y���W
	int y_offset = 80;
	//X���W
	const int STORY_COMENT_X = 100;

    for (const auto& comment : displayed_story_coments) {

        DrawStringEx(STORY_COMENT_X, y_offset, font_color, "%s", comment.c_str());
        y_offset += ADD_OFSET; // �e�R�����g�̕\���ʒu�����炷

    }

	if (story_end_flag) {

		//�摜��\������
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/button_Enter.png", ENTER_KEY_POS.x, ENTER_KEY_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		DrawStringEx(ENTER_KEY_STRING_POS.x, ENTER_KEY_STRING_POS.y, font_color, "���ɐi��");
	}

	//�摜��\������
	//�^�u�L�[�ŃX�L�b�v�ł���悤�ɂ���ׁA�\������
	ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon146.png", TAB_KEY_POS.x, TAB_KEY_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

	DrawStringEx(TAB_KEY_STRING_POS.x, TAB_KEY_STRING_POS.y, font_color, "�X�g�[���[���X�L�b�v����");
}

//�X�g�[���[��csv���烍�[�h����
void UIManager::StoryLoad(const int& section_type)
{
	//��x�R�����g������������
	displayed_story_coments.clear();
	current_story_comment_index = 0;
	story_end_flag = false;
	story_.clear();

	auto story_csv = CsvManager::GetCsvManager()->GetStoryCsv();

	for (int i = 1; i < story_csv.size(); i++) {

		//�ǂ̃^�C�v�Ȃ̂����m�F���ăX�g�[���[���i�[����
		if (section_type == std::stoi(story_csv[i][0].c_str())) {

			story_.emplace_back(story_csv[i][1]);
		}
	}
}

//�x���p��UI��\������
void UIManager::WarningWindow(const std::string& warnig_message)
{
	const tnl::Vector2i WARNING_MESSAGE_POS = { 80 , 130 };

	//�x��������\������
	DrawStringEx(WARNING_MESSAGE_POS.x, WARNING_MESSAGE_POS.y, koni::Color::WHITE, "%s" , warnig_message.c_str());

}

//�C�x���g�p�̕�����\������(�A�C�e�����g�p�����ۂȂ�)
void UIManager::DisplayEventMessage()
{
	//�E�B���h�E�̒ʒm�t���O��true��������
	if (is_notification_displayed) {

		//�E�B���h�E�̍��W
		const tnl::Vector2i DISPLAY_WINDOW_POS = { 50, 500 };

		//�E�B���h�E�̕��ƍ���
		const int DISPLAY_WINDOW_HEIGHT = 200;
		const int DISPLAY_WINDOW_WIDTH = 700;

		//���镶���̍��W
		const tnl::Vector2i DISPLAY_CLOSE_STRING_POS = { 550 , 650 };

		//�E�B���h�E�̕\�����s��
		GetMenu("menu_window")->Menu_draw(DISPLAY_WINDOW_POS.x, DISPLAY_WINDOW_POS.y, DISPLAY_WINDOW_WIDTH, DISPLAY_WINDOW_HEIGHT);

		//�����̕\��
		DrawStringEx(DISPLAY_WINDOW_POS.x + ADD_OFSET, DISPLAY_WINDOW_POS.y + ADD_OFSET, koni::Color::WHITE, "%s", display_event_message.c_str());

		//���镶��
		DrawStringEx(DISPLAY_CLOSE_STRING_POS.x, DISPLAY_CLOSE_STRING_POS.y, koni::Color::WHITE, "back space : ����");
	}
}

//�f�o�b�N�p�̑������
void UIManager::DebugDetailWindow(const BaseScene::SceneState& curent_scene_state)
{
	//���[���h�}�b�v�ɂ��鎞�̂�
	if (curent_scene_state == BaseScene::SceneState::WORLDMAP && player_detail_window_flag) {

		//���j���[�E�B���h�E�̍��W
		const tnl::Vector2i DETAIL_WINDOW_POS = { 50, 305 };

		//���j���[�E�B���h�E�̃T�C�Y
		const int DETAIL_WINDOW_WIDTH = 400;
		const int DETAIL_WINDOW_HEIGHT = 400;

		//������������̍��W
		const tnl::Vector2i MOVE_DETAIL_STRING_POS = { 100, 335 };

		Menu_Draw("menu_window", DETAIL_WINDOW_POS.x, DETAIL_WINDOW_POS.y, DETAIL_WINDOW_WIDTH, DETAIL_WINDOW_HEIGHT);

		for (int i = 0; i < debug_detail_texts.size(); i++)
		{
			DrawStringEx(MOVE_DETAIL_STRING_POS.x, MOVE_DETAIL_STRING_POS.y + (i * ADD_OFSET), koni::Color::WHITE, "%s", debug_detail_texts[i].c_str());
		}
	}
}

UIManager::UIManager()
{
	//�t�H���g���쐬����
	string_handle_80 = CreateFontToHandle(NULL, koni::Numeric::FONT_SIZE_80, koni::Numeric::FONT_SIZE_10, DX_FONTTYPE_EDGE);

	//�t�H���g���쐬����
	string_handle_100 = CreateFontToHandle(NULL, koni::Numeric::FONT_SIZE_100, koni::Numeric::FONT_SIZE_10, DX_FONTTYPE_EDGE);

	//�t�H���g���쐬����
	string_handle_30 = CreateFontToHandle(NULL, koni::Numeric::FONT_SIZE_30, koni::Numeric::FONT_SIZE_10, DX_FONTTYPE_EDGE);
}

UIManager::~UIManager()
{
	//���̃��j���[���������
	menu_map.clear();

	// �쐬�����t�H���g�f�[�^���폜����
	DeleteFontToHandle(string_handle_80);
	DeleteFontToHandle(string_handle_100);
	DeleteFontToHandle(string_handle_30);

}

//�󋵂ɂ���Ėh��n�̃A�C�R����Ԃ�(��ɃX�e�[�^�X�Ȃǂ̃A�C�R��)
int UIManager::GetStatusDefanceIconHdl()
{
	//�x�[�X�̖h���
	int player_base_defance = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetBaseDefance();

	//���݂̖h���
	int curent_player_defance = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetDefance();

	//�C���x���g��
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//����𑕔����Ă���ꍇ
	if (!inventory->GetEquipAromorArray().empty()) {

		for (auto& equip_weqpon : inventory->GetEquipAromorArray()) {

			//�v���C���[�̖h��͂��㏸���Ă�����
			if (player_base_defance + equip_weqpon.GetItemDefance() < curent_player_defance) {

				int status_defance_up_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_defance_up.png");

				//�X�e�[�^�X�A�b�v�̃A�C�R����Ԃ�
				return status_defance_up_icon;
			}
			//�v���C���[�̖h��͂��������Ă�����
			else if ((player_base_defance + equip_weqpon.GetItemDamage()) > curent_player_defance) {

				int status_defance_down_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_defance_down.png");

				//�X�e�[�^�X�_�E���̃A�C�R����Ԃ�
				return status_defance_down_icon;
			}
		}
	}
	//�������Ă��Ȃ��čU���͂��㏸���Ă��
	else if (player_base_defance < curent_player_defance) {

		int status_defance_up_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_defance_up.png");

		//�X�e�[�^�X�A�b�v�̃A�C�R����Ԃ�
		return status_defance_up_icon;
	}
	//�������Ă��Ȃ��čU���͂��������Ă����
	else if (player_base_defance > curent_player_defance) {

		int status_defance_down_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_defance_down.png");

		//�X�e�[�^�X�_�E���̃A�C�R����Ԃ�
		return status_defance_down_icon;

	}

	//���������܂������Ȃ������ꍇ�}�C�i�X�l��n��
	return ERROR_VALUE;
}

//�󋵂ɂ���čU���n�̃A�C�R����Ԃ�(��ɃX�e�[�^�X�Ȃǂ̃A�C�R��)
int UIManager::GetStatusAttackIconHdl()
{

	//�x�[�X�̍U����
	int player_base_attck = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetBaseAttack();

	//�U����
	int curent_player_attack = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetAttack();

	//�C���x���g��
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//����𑕔����Ă���ꍇ
	if (!inventory->GetEquipWeaponArray().empty()) {

		for (auto& equip_weqpon : inventory->GetEquipWeaponArray()) {

			//�v���C���[�̍U���͂��㏸���Ă�����
			if ((player_base_attck + equip_weqpon.GetItemDamage()) < curent_player_attack) {

				int status_attack_up_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_attack_up.png");

				//�X�e�[�^�X�A�b�v�̃A�C�R����Ԃ�
				return status_attack_up_icon;
			}
			//�v���C���[�̍U���͂��������Ă�����
			else if ((player_base_attck + equip_weqpon.GetItemDamage()) > curent_player_attack) {

				int status_attack_down_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_attack_down.png");

				//�X�e�[�^�X�_�E���̃A�C�R����Ԃ�
				return status_attack_down_icon;
			}
		}
	}
	//�������Ă��Ȃ��čU���͂��㏸���Ă��
	else if (player_base_attck < curent_player_attack) {

		int status_attack_up_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_attack_up.png");

		//�X�e�[�^�X�A�b�v�̃A�C�R����Ԃ�
		return status_attack_up_icon;
	}
	//�������Ă��Ȃ��čU���͂��������Ă����
	else if (player_base_attck > curent_player_attack) {

		int status_attack_down_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_attack_down.png");

		//�X�e�[�^�X�_�E���̃A�C�R����Ԃ�
		return status_attack_down_icon;

	}


	//���������܂������Ȃ������ꍇ�}�C�i�X�l��n��
	return ERROR_VALUE;
	
}

//void UIManager::IconAnimation()
//{
//	//�|�C���^�𐶐�����
//	if (icon_animation == nullptr) {
//
//		//�v���C���[�̍��W
//		auto palyer_pos = GameManager::getGameManager()->getPlayer()->getPlayerPos();
//
//		//�J�����̍��W
//		auto camera_pos = GameManager::getGameManager()->getCamera()->getTargetPos();
//
//		// �v���C���[�̕`��ʒu���v�Z
//		tnl::Vector3 draw_pos = palyer_pos - camera_pos + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
//
//		//�A�C�R���p�̃A�j���[�V�����p�̃|�C���^�𐶐�����
//		icon_animation = std::make_shared<Animation>("graphics/talk_icon.png", draw_pos.x, draw_pos.y - 80, 3, 1, 32, 32, 3, 8);
//
//		//�`����s��
//		icon_animation->play_animation();
//	}
//	//�|�C���^�������ꍇ
//	else {
//
//		//�v���C���[�̍��W
//		auto palyer_pos = GameManager::getGameManager()->getPlayer()->getPlayerPos();
//
//		//�J�����̍��W
//		auto camera_pos = GameManager::getGameManager()->getCamera()->getTargetPos();
//
//		// �v���C���[�̕`��ʒu���v�Z
//		tnl::Vector3 draw_pos = palyer_pos - camera_pos + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
//
//		//�A�C�R���̍��W��؂�ւ���
//		icon_animation->SetAnimationPos(draw_pos.x , draw_pos.y - 50);
//
//		//�`����s��
//		icon_animation->play_animation();
//	}
//
//}

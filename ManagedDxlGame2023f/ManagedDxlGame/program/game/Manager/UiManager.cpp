#include "UiManager.h"
#include"CsvManager.h"
#include"GameManager.h"

UIManager* UIManager::getUIManager()
{
	static UIManager* p_instance = nullptr;

	if (!p_instance)p_instance = new UIManager();
	return p_instance;
}

void UIManager::Update(float delta_time)
{
	//�A�C�R���p�̃A�j���[�V����
	if (icon_animation) {
		icon_animation->update(delta_time);
	}
}

void UIManager::Draw()
{
	//�A�C�R���p�̃A�j���[�V�����̕`��
	if (icon_animation) {
		icon_animation->draw();
	}
}

void UIManager::addMenu(const std::string& menuName, std::shared_ptr<Menu> menu)
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

//���퉮�̃R�����g�\��
void UIManager::armsdealerComentDraw(int type)
{
	//���퉮�ɍŏ��ɘb�������Ƃ��̃R�����g
	if (type == ARMSDEALER_FIRST_COMET) {
		DrawString(WEAPONSHOP.x, WEAPONSHOP.y, "���퉮", COLOR_WHITE);
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, COLOR_WHITE, "�u ��������Ⴂ�I�v");
		DrawStringEx(WEAPONSHOP_BUY.x, WEAPONSHOP_BUY.y, -1, "(1 : ������w������)");
		DrawStringEx(MENU_CLOSE.x, MENU_CLOSE.y, COLOR_WHITE, "(0 : ���j���[�����)");
	}
	//�Ȃɂ������Ƃ��̃R�����g
	else {
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, COLOR_WHITE, "�u ���𔃂��񂾂� �v");
	}
}

//�R�����g�����[�h����
void UIManager::ComentLoad(const int max_num , const std::string& name)
{
	//��x�R�����g������������
	coment_.clear();

	auto coment_csv = CsvManager::getCsvManager()->GetComentCsv();

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

	DrawStringEx(coment_pos.x, coment_pos.y, COLOR_WHITE, "%s", coment_[curent_num].c_str());

	//�����\��������W
	const int ADD_X = 450;  const int ADD_Y = 50;

	DrawStringEx(coment_pos.x + ADD_X, coment_pos.y + ADD_Y, -1, "(0 : ����)");

}

//�����l��\���ł���p�̃R�����g
//void UIManager::ComentDraw(const tnl::Vector2i& coment_pos, int value)
//{
//	DrawStringEx(coment_pos.x, coment_pos.y, COLOR_WHITE, "%s", coment_[curent_num].c_str(), value);
//
//	//�����\��������W
//	const int ADD_X = 450;  const int ADD_Y = 50;
//
//	DrawStringEx(coment_pos.x + ADD_X, coment_pos.y + ADD_Y, -1, "(0 : ����)");
//}

//�R�����g�����Ɉړ�����
void UIManager::ComentNextByInput(const int max_draw_num)
{
	//�`��̍ő吔�𒴂��ĂȂ����
	if (count < max_draw_num) {
		//Enter�L�[���󂯕t����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���𑝂₷
			curent_num++;
			//�J�E���g�𑝂₷
			count++;
		}
	}
}

//�v���C���[�̃X�e�[�^�X��HP�o�[�ŕ\������
void UIManager::PlayerStatusDrawWindow()
{
	//�v���C���[�̃X�e�[�^�X���擾����
	auto& plyerstatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

	//�v���C���[�̉摜
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/Player/Player_Icon_sentou.png", 90, 120, 1.3f, 0, true);

	//hp�o�[�̉摜
	int hpber_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/hpbar.png");
	//mp�o�[�̉摜
	int mpber_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/mpbar.png");
	//hp�o�[�ő�̒���
	int maxHpBar = 200;
	//mp�o�[�̍ő�̒���
	int maxMpBer = 200;

	// HP�̊������v�Z����
	float hp_ratio = plyerstatus.GetcurentHp() / plyerstatus.GetMaxHp();
	// HP�o�[�̒������v�Z����
	int hpbar = static_cast<int>(maxHpBar * hp_ratio);

	//int hpvar = 0;

	//if (hpbar <= hpbaa) {
	//	 
	//	hpbaa -= 2;/*hp�o�[�̌��鑬�x*/
	//}


	// MP�̊������v�Z����
	float mp_ratio = plyerstatus.GetCurentMp() / plyerstatus.GetMaxMp();

	// MP�o�[�̒������v�Z����
	int mpbar = static_cast<int>(maxMpBer * mp_ratio);

	DrawExtendGraph(75, 155, (75 + hpbar), 185, hpber_hdl, true);
	DrawExtendGraph(75, 215, (75 + mpbar), 245, mpber_hdl, true);

	//�v���C���[��HP��\������
	DrawStringEx(75, 70, -1, "Player");
	DrawStringEx(120, 110, -1, " Lv %d", plyerstatus.GetLevel());
	DrawStringEx(75, 190, -1, " Hp : %d / %d", static_cast<int>(plyerstatus.GetcurentHp()), static_cast<int>(plyerstatus.GetMaxHp()));
	DrawStringEx(75, 250, -1, " Mp : %d / %d", static_cast<int>(plyerstatus.GetCurentMp()), static_cast<int>(plyerstatus.GetMaxMp()));

}

//�Z�[�u�������̕�����\��������
void UIManager::SaveText(const tnl::Vector3& text_pos)
{
	//�Z�[�u���Ă��鎞��
	if (save_flag) {

		//�t���[�����C���N�������g
		frame++;

		DrawStringEx(text_pos.x, text_pos.y, -1, "�Z�[�u���ł��B");

		//�t���[������60�𒴂�����
		if (frame >= 60) {

			//�t���O��؂�ւ���
			save_flag = false;

		}

	}
	//�t���[������60�𒴂��Ă��t���O�������Ă��Ȃ�������
	else if (frame >= 60 && !save_flag) {

		//�Z�[�u�������������̃e�L�X�g���o��
		DrawStringEx(text_pos.x, text_pos.y, -1, "�Z�[�u���܂����B");
	}

}

//�v���C���[�̑������
void UIManager::PlayerMoveDetail(const std::vector<std::string>& detail_text)
{
	if (player_detail_window_flag) {

		auto menu_window = getMenu("menu_window");

		menu_window->Menu_draw(850, 300, 400, 400);

		for (int i = 0; i < detail_text.size(); i++)
		{
			DrawStringEx(900, 350 + (i * 50), -1, "%s", detail_text[i].c_str());
		}
	}
}

void UIManager::IconAnimation()
{
	//�|�C���^�𐶐�����
	if (icon_animation == nullptr) {

		//�v���C���[�̍��W
		auto palyer_pos = GameManager::getGameManager()->getPlayer()->getPlayerPos();

		//�J�����̍��W
		auto camera_pos = GameManager::getGameManager()->getCamera()->getTargetPos();

		// �v���C���[�̕`��ʒu���v�Z
		tnl::Vector3 draw_pos = palyer_pos - camera_pos + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

		//�A�C�R���p�̃A�j���[�V�����p�̃|�C���^�𐶐�����
		icon_animation = std::make_shared<Animation>("graphics/talk_icon.png", draw_pos.x, draw_pos.y - 80, 3, 1, 32, 32, 3, 8);

		//�`����s��
		icon_animation->play_animation();
	}
	//�|�C���^�������ꍇ
	else {

		//�v���C���[�̍��W
		auto palyer_pos = GameManager::getGameManager()->getPlayer()->getPlayerPos();

		//�J�����̍��W
		auto camera_pos = GameManager::getGameManager()->getCamera()->getTargetPos();

		// �v���C���[�̕`��ʒu���v�Z
		tnl::Vector3 draw_pos = palyer_pos - camera_pos + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

		//�A�C�R���̍��W��؂�ւ���
		icon_animation->SetAnimationPos(draw_pos.x , draw_pos.y - 50);

		//�`����s��
		icon_animation->play_animation();
	}

}

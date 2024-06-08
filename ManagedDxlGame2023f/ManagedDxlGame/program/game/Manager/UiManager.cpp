#include "UiManager.h"
#include"CsvManager.h"
#include"GameManager.h"

UIManager* UIManager::getUIManager()
{
	static UIManager* p_instance = nullptr;

	if (!p_instance)p_instance = new UIManager();
	return p_instance;
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
void UIManager::ComentDraw(const tnl::Vector2i& coment_pos,const int max_draw_num)
{
	//�`��̍ő吔�𒴂��ĂȂ����
	if (curent_num < max_draw_num) {
		//Enter�L�[���󂯕t����
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//���𑝂₷
			curent_num++;
			//�J�E���g�𑝂₷
			count++;
		}
	}

	DrawStringEx(coment_pos.x, coment_pos.y, COLOR_WHITE, "%s", coment_[curent_num].c_str());


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

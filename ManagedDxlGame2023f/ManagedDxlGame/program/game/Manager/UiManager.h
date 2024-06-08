#pragma once
//------------------------------------------------------------------------------------------------------------
//
//UI���܂Ƃ߂�}�l�[�W���[
//
//-------------------------------------------------------------------------------------------------------------

#include "../../dxlib_ext/dxlib_ext.h"
#include"../Menu/MenuWindow.h"
#include <unordered_map>

class CsvManager;
class GameManager;

class UIManager final {

public:

	static UIManager* getUIManager();
	static void delategetUIManager() { delete getUIManager(); }

	//���j���[��ǉ�����
	void addMenu(const std::string& menuName, std::shared_ptr<Menu> menu);

	// ���j���[���擾����
	std::shared_ptr<Menu> getMenu(const std::string& menuName) {

		auto it = menu_map.find(menuName);

		if (it != menu_map.end()) {
			return it->second;
		}

		return nullptr;
	}

	//���j���[��`�悷��
	void Menu_Draw(const std::string& menuName, const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height);

	//���퉮�̃R�����g
	void armsdealerComentDraw(int type);

	//�R�����g�����[�h����
	void ComentLoad(const int max_num ,const std::string& name);

	//�R�����g��\������
	void ComentDraw(const tnl::Vector2i& coment_pos, const int max_draw_num);

	//�R�����g�����������ĂP����\��������
	void ComentClear() { curent_num = 0; }

	//�J�E���^�[���擾����
	int GetCount()const { return count; }

	//�J�E���^�[�����Z�b�g����
	void CountReset() { count = 0; }

	//�v���C���[�̃X�e�[�^�X��HP�o�[�ŕ\������
	void PlayerStatusDrawWindow();
	
private:

	UIManager(){}
	~UIManager() {};

	//���j���[���Ǘ�����}�b�v
	std::unordered_map<std::string, Shared<Menu>>menu_map;

	//�R�����g���i�[����z��
	std::vector<std::string>coment_;

	//---���ꂼ��̃R�����g---//

	//���폤�l�̃R�����g�̍��W
	const tnl::Vector2i ARMSDEALERCOMENT = { 150, 550 };

	//���퉮�̘b���������Ƃ��̃R�����g
	const int ARMSDEALER_FIRST_COMET = 0;

	//���퉮�̕����̍��W
	const tnl::Vector2i WEAPONSHOP = { 60 , 520 };

	//���퉮�̍w���R�����g�̍��W
	const tnl::Vector2i WEAPONSHOP_BUY = { 70, 100 };

	//���퉮�̃��j���[�����R�����g�̍��W
	const tnl::Vector2i MENU_CLOSE = { 70, 150 };

	//���F
	const int COLOR_WHITE = -1;

	//���݂̃R�����g
	int curent_num = 0;

	//�J�E���^�[(��ɃJ�E���g���g���ăR�����g��̃V�[����J�ڂ������肷��)
	int count = 0;
};
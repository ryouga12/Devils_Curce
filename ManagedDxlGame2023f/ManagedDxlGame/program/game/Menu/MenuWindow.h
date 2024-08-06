
///UI�Ɋւ���N���X
///�I�����������Ȃ�Menu��
///�I����������MenuWindow������



#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include<string>
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"

class Menu {
public:

	Menu() = default;

	//new����Ƃ���gh��path�������œn��
	Menu(const std::string& gh_path);

	virtual~Menu();

	//���j���[�̕`����s��
	//arg_1 : X���W
	//arg_2 : Y���W
	//arg_3 : ���̃T�C�Y
	//arg_4 : �c�̃T�C�Y
	void Menu_draw(const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height);

private:

	int menu_x, menu_y, menu_width, menu_height = 0;
	int menu_gh[9] = { 0 };
	
protected:

	bool menu_live;

};

class MenuWindow final: public Menu {
public:

	//���j���[�̗v�f�̍\����
	typedef struct {
		int x;
		int y;
		std::string name;
		int menu_num;
	}MenuElement_t;
	
	//�I�����̂��郁�j���[�̃E�B���h�E
	//arg_1 : �摜��path
	//arg_2 : ���j���[�̍\���̂̃|�C���^
	//arg_3 : �v�f�̐�
	MenuWindow(const std::string& gh_path, MenuElement_t* elements, const int& elements_num);
	~MenuWindow()override;

	//���j���[���J��
	void Open();

	//�J�[�\������
	void Read();

	//���j���[�̕`��ƃJ�[�\��������s����悤�ɂ���
	void All(const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height);

	int getSelectNum() { return Select_Num;}

	//�J�[�\���𐧌䂷��
	void SetSelectCousourMove();

	//�J�[�\���̐���t���O���擾����
	bool getSelectCursour(){
		return manage_select_flag;
	}

	bool getAcrionFlag() { return is_active; }

	//�������󂯕t����悤�ɂ���
	void IsActiveChangeFlag() { is_active = !is_active; }

	//���j���[���g���ۂ̒萔�֘A

	//�v�f
	enum Elements {

	//��v�f��
	FIRST_ELEMENT,
	//��v�f��
	SECOND_ELEMENT,
	//�O�v�f��
	THERD_ELEMENT,
	//�l�v�f��
	FOUR_ELEMENT,
	//�ܗv�f��
	FIVE_ELEMENT,
	//�ő�l
	ELEMENTS_MAX
	};


private:

	//���݂̑I��ԍ�
	int Select_Num;

	bool menuInit = false;

	int read_menu_element_num = 0;

	int string_color_black = 0;
	int elements_num_;
	bool m_IsStartOpend;									//ESC�L�[�ŊJ����1�t���[����

	//Menu�̍\���̂̃|�C���^
	MenuElement_t*  MenuElement = nullptr;

	int cursor_gh = 0;
	int coursorX = 0;
	int coursorY = 0;

	bool second_menu_flag = false;
	bool manage_select_flag = true;//Read�֐���SelectNum�𓮂������ǂ��� true�Ȃ瓮����
	bool is_active = false;		   //�v���C���[����Enter�L�[�Ȃǂ��󂯕t���邩�ǂ����̃t���O
};
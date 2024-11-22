
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

	//---�萔�n---//

	//���j���[�z��̃T�C�Y
	static const int MENU_GHDL_ARRAY_MAX_SIZE = 9;

	//�����̐�
	const int GHDL_WIDTH_NUM = 3;
	//�c�̐�
	const int GHDL_HEIGHT_NUM = 3;
	//���̃T�C�Y
	const int GHDL_WIDTH_SIZE = 16;
	//�c�̃T�C�Y
	const int GHDL_HEIGHT_SIZE = 16;

	//���j���[�̃O���t�B�b�N�̃n���h��
	int menu_gh[MENU_GHDL_ARRAY_MAX_SIZE] = { 0 };

	//�ǉ����W
	const int ADD_OFSET_POS = 16;
	
protected:

	//���j���[�֘A�̌Ăяo���t���O
	bool menu_live = false;

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

	//�I��ԍ����擾����
	int GetSelectNum() { return Select_Num;}

	//�J�[�\���𐧌䂷��
	void SelectCousourMoveFlagChange();

	//�J�[�\���̐���t���O���擾����
	bool GetSelectCursour(){
		return manage_select_flag;
	}

	bool GetAcrionFlag() { return is_active; }

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

	//�������t���O
	bool menuInit = false;

	//�v�f�̐�
	int elements_num_;

	//ESC�L�[�ŊJ����1�t���[����
	bool m_IsStartOpend;									

	//Menu�̍\���̂̃|�C���^
	MenuElement_t*  MenuElement = nullptr;

	//���j���[�E�B���h�E��X���W
	int coursorX = 0;
	//���j���[�E�B���h�E��Y���W
	int coursorY = 0;

	//Read�֐���SelectNum�𓮂������ǂ��� true�Ȃ瓮����
	bool manage_select_flag = true;

	//�v���C���[����Enter�L�[�Ȃǂ��󂯕t���邩�ǂ����̃t���O
	bool is_active = false;		  

	//---�萔�n---//

	//�J�[�\����X���W�̈ʒu��ݒ肷��ׂ̈����l
	const int MINUS_OFFSET_COURSOR_POS = 20;

	//�J�[�\���̒������W
	const int ADJUSTMENT_COURSOR_POS = 8;

};
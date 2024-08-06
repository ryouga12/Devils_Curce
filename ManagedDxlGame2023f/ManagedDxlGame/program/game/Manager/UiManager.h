#pragma once
//------------------------------------------------------------------------------------------------------------
//
//UI���܂Ƃ߂�}�l�[�W���[
//
//-------------------------------------------------------------------------------------------------------------

#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"
#include"../Menu/MenuWindow.h"
#include <unordered_map>

class CsvManager;
class GameManager;

class UIManager final {

public:

	static UIManager* getUIManager();
	static void delategetUIManager() { delete getUIManager(); }

	//���j���[��ǉ�����(��ɑ��̏��ŃE�B���h���g���񂵂����ꍇ�Ɏg�p)
	//arg_ : ���j���[�̖��O
	//arg_ : �ǉ�������Menu�N���X��Shared�|�C���^
	void addMenu(const std::string& menuName, const std::shared_ptr<Menu>& menu);

	// ���j���[���擾����
	std::shared_ptr<Menu> getMenu(const std::string& menuName) {

		auto it = menu_map.find(menuName);

		if (it != menu_map.end()) {
			return it->second;
		}

		return nullptr;
	}

	//���j���[��`�悷��
	//arg_1 : �L�[�̖��O
	//arg_2 : X���W
	//arg_3 : Y���W
	//arg_4 : Menu�̕��̃T�C�Y
	//arg_5 : Menu�̏c�̃T�C�Y
	void Menu_Draw(const std::string& menuName, const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height);

	//���퉮�̃R�����g
	void armsdealerComentDraw(const int& type);

	//�R�����g�����[�h����
	//arg_1 : �R�����g�̐�
	//arg_2 : ���O(NPC�Ȃǂ̖��O���Ƃ炵���킹�Ă���ɊY�������R�����g�����[�h����)
	void ComentLoad(const int& max_num ,const std::string& name);

	//�R�����g��\������
	void ComentDraw(const tnl::Vector2i& coment_pos);

	//�R�����g�����Ɉړ�����
	//arg_1 : �R�����g�̕`�搔
	void ComentNextByInput(const int& max_draw_num);

	//�R�����g�����������ĂP����\��������
	void ComentClear() { curent_num = 0; }

	//�J�E���^�[���擾����
	int GetCount()const { return count; }

	//�J�E���^�[�����Z�b�g����
	void CountReset() { count = 0; }

	//�t���[�������Z�b�g����
	void FrameReset() { frame = 0; }

	//�v���C���[�̃X�e�[�^�X��HP�o�[�ŕ\������
	void PlayerStatusDrawWindow();

	//�Z�[�u�������̕�����\��������
	void SaveText(const tnl::Vector2i& text_pos );

	//�Z�[�u�e�L�X�g�̕\����؂�ւ���i�Z�[�u�e�L�X�g��\������j
	void SaveTextFlagChange() { save_flag = !save_flag; }

	//�v���C���[�̑������
	void PlayerMoveDetail(const std::vector<std::string>& detail_text);

	//�����̕\����؂�ւ���
	void PlayerDetailSwitchDisplay() {
		player_detail_window_flag = !player_detail_window_flag;
	}

	//��������̕\�����Ǘ�����t���O���擾����
	const bool GetPlayerDetailFlage()const {
		return player_detail_window_flag;
	}

	//��������̃e�L�X�g���擾����
	const std::vector<std::string>& GetPlayerMoveDetailText() const {
		return operation_instructions;
	}

	//�o�g���p�̐����̃e�L�X�g���擾����
	const std::vector<std::string>&GetBattlePlayerMoveDetailText() const {
		return battle_operation_instructions;
	}

	//HP�o�[��MP�o�[�̌v�Z
	void PlayerStatusBarUpdate(const float delta_time);

	//�X�g�[���[�̕������ӂ₵�Ă���
	//������������true��Ԃ�
	bool StoryDisplayUpdate(const float delta_time);

	//�X�g�[���[�̕�����\������
	void StoryDisplay(const int& font_color);

	//�X�g�[���[�̕����̃��[�h
	void StoryLoad(const int& section_type);

private:


	UIManager() = default;
	~UIManager();

	//���j���[���Ǘ�����}�b�v
	std::unordered_map<std::string, Shared<Menu>>menu_map;

	//�R�����g���i�[����z��
	std::vector<std::string>coment_;

	//�X�g���[�p�̔z��
	std::vector<std::string>story_;

	//�X�g�[���[�p�̃J�E���g
	int curent_story = 0;

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

	//���݂̃R�����g
	int curent_num = 0;

	//�J�E���^�[(��ɃJ�E���g���g���ăR�����g��̃V�[����J�ڂ������肷��)
	int count = 0;
	
	//�Z�[�u�������̃R�����g�p�̃t���O
	bool save_flag = false;
	
	//�t���[��(��Ƀt���[���𐔂��ĂȂɂ��C�x���g���N�������߂Ɏg��)
	int frame = 0;

	//�����\����؂�ւ���ׂ̃t���O
	bool player_detail_window_flag = true;

	// HP�o�[�̌��݂̒�����ێ�����ϐ�
	float current_hp_bar = 0;

	// MP�o�[�̌��݂̒�����ێ�����ϐ�
	float current_mp_bar = 0;

	// HP�o�[�̕�ԂɎg�p����o�ߎ��Ԃ�ێ�����ϐ�
	float hp_lerp_time = 0;

	// MP�o�[�̕�ԂɎg�p����o�ߎ��Ԃ�ێ�����ϐ�
	float mp_lerp_time = 0;

	// ��Ԃ̑����ԁi�b�j��ێ�����萔
	const float LERPDURATION = 1.0f;

	// ��Ԃ̊J�n���_�ł�HP�o�[�̒�����ێ�����ϐ�
	float hp_start = 0;

	// ��Ԃ̏I�����_�ł�HP�o�[�̒�����ێ�����ϐ�
	float hp_end = 0;

	// ��Ԃ̊J�n���_�ł�MP�o�[�̒�����ێ�����ϐ�
	float mp_start = 0;

	// ��Ԃ̏I�����_�ł�MP�o�[�̒�����ێ�����ϐ�
	float mp_end = 0;

	// ���ݕ\�����Ă���R�����g�̃C���f�b�N�X
	int current_story_comment_index = 0; 
	
	// ���ݕ\�����̃R�����g�̃��X�g
	std::vector<std::string> displayed_story_coments;
	
	// �R�����g�\���̃^�C�}�[
	float story_display_timer = 0.0f; 

	//�ǉ����W
	const int ADD_OFSET = 50;

//--------------------------------------------------------------------------------------------------------------
//UI�̐������p�̕�����

	//�������
	std::vector<std::string> operation_instructions
		= {"�������", "W : ��" ,"S : ��", "A : ��" , "D : �E","ESC : ������J��","TAB : �E�B���h�E�̕\����؂�ւ���"};

	//�o�g���V�[���p�̑������
	std::vector<std::string> battle_operation_instructions
		= { "�������" , "�\���L�[ : �J�[�\���̈ړ�" , "�� : ��O�ɖ߂�","Enter : ���� , ���ɐi��" ,"TAB : �E�B���h�E�̕\����؂�ւ���" };
};
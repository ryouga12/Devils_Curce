#pragma once
#include<string>
#include<vector>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"

//---------------------------------------------------------------------------------------------------------
//�A�j���[�V�����N���X(�A�j���[�V������ݒ肵�čĐ����~�Ȃǂ��s����)


class Animation final {
public:

	Animation() = default;

	//�A�j���[�V����
	//arg_1 :  �n���h�� 
	//arg_2 :  X���W  
	//arg_3 :  Y���W 
	//arg_4 : �A�j���[�V�����̕��̐� 
	//arg_5 : �A�j���[�V�����̏c�̐� 
	//arg_6 :  ���̃T�C�Y 
	//arg_7 :  �c�̃T�C�Y 
	//arg_8 :  �t���[���̑����@
	//arg_9 :  �x������b�� 
	//arg_10 :  �G�t�F�N�g�̃T�C�Y(�f�t�H���g��2�Őݒ��ς������ꍇ�͈������g�p����)) 
	Animation(const std::string& gh_pass, const int& x, const int& y, const int& width_num, const int& height_num, const int& width_size, const int& height_size, const int& frame_count, const int& delay , const float& size = 2.0f);
	~Animation();

	//�A�j���[�V�������Đ�����֐�
	void play_animation();

	//�A�j���[�V�������~����֐�
	void stop_animation();

	//�A�j���[�V�������ꎞ��~����֐�
	void pause_animation();

	//�A�j���[�V�����̍X�V������֐�
	void update(const float delta_time);

	//�A�j���[�V�����̕`�悷��֐�
	void draw();

	//�A�j���[�V�����̏I�����������֐�
	bool is_finished() const;

	enum class AnimationState {
		//�Đ�
		PLAYING,
		//�ꎞ��~
		PAUSE,
		//��~
		STOP,
	};

private:

	//�O���t�B�b�N�n���h��
	std::string m_graph_handle;
	//�t���[���̕���
	int m_width_num = 0;
	//�t���[���̍�����
	int m_height_num = 0;
	//���̃T�C�Y
	int m_width_size = 0;
	//�����̃T�C�Y
	int m_height_size = 0;
	//�t���[���̑���
	int m_frame_num = 0;
	//���݂̃t���[��
	int m_current_frame = 0;
	//�t���[���Ԃ̃C���^�[�o��
	int m_frame_interval = 0;
	//�^�C�}�[
	int m_timer = 0;
	//�O���t�B�b�N�n���h���̔z��
	int* m_gh_ar = 0;
	//�`���x���W
	int m_draw_pos_x = 0;
	//�`���y���W
	int m_draw_pos_y = 0;
	//�A�j���[�V�����̏��
	AnimationState m_state;
	//�G�t�F�N�g�̃T�C�Y
	float m_size = 0;	
	//�A�j���[�V�������~�߂�t���O
	bool m_stop_flag = false;
};


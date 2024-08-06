#pragma once
#include<string>
#include<vector>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"

enum class AnimationState {
	PLAYING,//�Đ�
	PAUSE,//�ꎞ��~
	STOP,//��~
};

class Animation final {
public:

	Animation() = default;

	//�A�j���[�V����
	//arg_1 :  �n���h�� 
	//arg_2 :  X���W  
	//arg_3 : Y���W 
	//arg_4 : �A�j���[�V�����̕��̐� 
	//arg_5 : �A�j���[�V�����̏c�̐� 
	//arg_6 :  ���̃T�C�Y 
	//arg_7 : �c�̃T�C�Y 
	//arg_8 :  �t���[���̑����@
	//arg_ 9 : �x������b�� 
	//arg_10 :  �G�t�F�N�g�̃T�C�Y(�f�t�H���g��2�Őݒ��ς������ꍇ�͈������g�p����)) 
	Animation(const std::string& gh_pass, const int& x, const int& y, const int& width_num, const int& height_num, const int& width_size, const int& height_size, const int& frame_count, const int& delay , const float& size = 2.0f);
	~Animation();

	void play_animation();//�A�j���[�V�������Đ�����֐�
	void stop_animation();//�A�j���[�V�������~����֐�
	void pause_animation();//�A�j���[�V�������ꎞ��~����֐�
	void update(const float delta_time);//�A�j���[�V�����̍X�V������֐�
	void draw();//�A�j���[�V�����̕`�悷��֐�
	bool is_finished() const;//�A�j���[�V�����̏I�����������֐�
	void SetAnimationPos(const int new_draw_pos_x , const int new_draw_pos_y) { m_draw_pos_x = new_draw_pos_x, m_draw_pos_y = new_draw_pos_y; }

private:
	std::string m_graph_handle;//�O���t�B�b�N�n���h��
	int m_width_num;//�t���[���̕���
	int m_height_num;//�t���[���̍�����
	int m_width_size;//���̃T�C�Y
	int m_height_size;//�����̃T�C�Y
	int m_frame_num;//�t���[���̑���
	int m_current_frame;//���݂̃t���[��
	int m_frame_interval;//�t���[���Ԃ̃C���^�[�o��
	int m_timer;//�^�C�}�[
	int* m_gh_ar;//�O���t�B�b�N�n���h���̔z��
	int m_draw_pos_x;//�`���x���W					 
	int m_draw_pos_y;//�`���y���W
	AnimationState m_state;//�A�j���[�V�����̏��
	float m_size;	//�G�t�F�N�g�̃T�C�Y
	bool stop_flag = false;
};


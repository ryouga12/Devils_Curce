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

class Animation {
public:

	//�A�j���[�V����
	//���� : (�n���h�� , X���W , Y���W , �A�j���[�V�����̕��̐� ,�A�j���[�V�����̏c�̐� , ���̃T�C�Y, �c�̃T�C�Y , �t���[���̑����@,�x������b�� )
	Animation(const std::string gh_pass, int x, int y, int width_num, int height_num, int width_size, int height_size, int frame_count, int delay);
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
	bool stop_flag = false;
};


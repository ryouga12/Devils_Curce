#pragma once
///
///�^�C�g����ʂ̏����Ȃ�
///

#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/SceneManager.h"
#include"InMapScene.h"
#include"../Manager/GameManager.h"
#include"../Manager/SoundManager.h"

class UIManager;;


class TittleScene : public BaseScene  {
public:

	TittleScene();
	~TittleScene()override;

	void Update(float delta_time) override;
	void Draw()override;

	//�^�C�g�����Ŏg�������S�ʂ𕪂���
	enum class TittleState {
		//�ŏ��̑I�����
		TITTLE,
		//�`�����n�߂����̃L�������C�N
		CHARAMIKE,
	};

	


private:
	tnl::Sequence<TittleScene> sequence_ = tnl::Sequence<TittleScene>(this, &TittleScene::seqTittle);
	bool seqTittle(float delta_time);


	//�w�i�摜
	int tittle_ghdl = 0;

	//�G���^�[�L�[�̉摜
	int enter_key_hdl = 0;

	//�J���[�i���j
	int String_Color_Black = 0;


	//�ŏ��ɑJ�ڂ������̍��W
	tnl::Vector3 first_pos_ = { 195, 312 , 0 };						

	//�x��������b��
	float Time = 1.0f;
	


//------------------------------------------------------------------------------------------------------------------------	
//�|�C���^

	Shared<Menu> menu_window = nullptr;
	Shared<Menu>menu_window_white = nullptr;

	//�V�[����؂�ւ���
	bool SceneChange = false;
	bool seqChangeScene(float delta_time) { return true; }

};
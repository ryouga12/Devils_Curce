#pragma once
///
///�^�C�g����ʂ̏����Ȃ�
///


#include"../Scene/BaseScene.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/SceneManager.h"
#include"village.h"
#include"../Manager/GameManager.h"
#include"../Menu/MenuWindow.h"
#include"../Item/Item.h"
#include"../Object/Enemy.h"
#include"../Manager/SoundManager.h"
#include"../../game/Skill/Skill.h"


class TittleScene : public BaseScene  {
public:

	TittleScene();
	~TittleScene()override;

	void Update(float delta_time) override;
	void Draw()override;

	


private:
	tnl::Sequence<TittleScene> sequence_ = tnl::Sequence<TittleScene>(this, &TittleScene::seqIdle);
	bool seqIdle(float delta_time);


	//�w�i�摜
	int tittle_ghdl = 0;

	//�J���[�i���j
	int String_Color_Black = 0;


	//�ŏ��ɑJ�ڂ������̍��W
	tnl::Vector3 first_pos_ = { 195, 312 , 0 };						

	//�x��������b��
	float Time = 1.0f;
	


//------------------------------------------------------------------------------------------------------------------------	
//�|�C���^

	Shared<Menu> tittle_Menu = nullptr;
	Shared<Inventory>inventory_ = nullptr;
	Shared<Player>myplayer = nullptr;
	Shared<Skill>skill = nullptr;
	/*Shared<Enemy>enemy = nullptr;*/
	

	//�V�[����؂�ւ���
	bool SceneChange = false;
	bool seqChangeScene(float delta_time) { return true; }

};
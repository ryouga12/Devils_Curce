//------------------------------------------------------------------------------------------------------------
//
//�Q�[�����̃I�u�W�F�N�g�̊��N���X(Player��Npc�Ȃ�)
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../System/Camera.h"
#include<string>
#include"../../koni_name_space/common/common_value.h"

class BattleScene;
class EventManager;

class Actor {
public:

	Actor() = default;
	virtual~Actor()= default;

	virtual void Update(float delta_time) {};
	virtual void Draw(const KonCamera& camera) {};


	//��̃|�C���^���Z�b�g����
	void SetBattleScene(BattleScene* battle_scene) {
		battle_scene_ = battle_scene;
	}


protected:
	
	//��̃|�C���^��p�ӂ��Ă��ꂼ��Őݒ肵�ď�����ς���
	//Shead�|�C���^�[�ɂ���ꍇ�Aweak�|�C���^�[�ɂ���K�v������
	BattleScene* battle_scene_ = nullptr;


	//---�A�j���[�V�����Ŏg���t���[���֘A---//

	//���t���[��(3)
	const int TOTAL_FRAME = 3;							
	//���t���[��(3)
	const int HORIZONTAL_FRAME = 3;						
	//�c�t���[��(1)
	const int VERTICAL_FRAME = 1;						
	//�������� 1�t���[��������̕�(48)
	const int HORIZONTAL_ONEFRAME = 48;					
	//�c������ 1�t���[��������̕�(48)
	const int VERTICAL_ONEFRAME = 48;					

	//�A�j���[�V������؂�ւ���b��
	static constexpr float ChangeAnimationTime = 0.3f;
	//�t���[��
	static constexpr int animationFlame = 3;


};
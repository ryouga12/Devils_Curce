#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/Camera.h"
#include<string>

class BattleScene;
class EventManager;

class Actor {
public:

	Actor() {};
	virtual~Actor() {};

	virtual void Update(float delta_time) {};
	virtual void Draw(const KonCamera& camera) {};

	enum class ActorType {
		NONE,
		ENEMY,

	};

	//��̃|�C���^���Z�b�g����
	void SetBattleScene(BattleScene* battle_scene) {
		battle_scene_ = battle_scene;
	}

	//�o�g���V�[���̃|�C���^���������
	/*void DeleteBattleScene() {
		delete battle_scene;
	}*/

protected:
	
	//��̃|�C���^��p�ӂ��Ă��ꂼ��Őݒ肵�ď�����ς���
	//Shead�|�C���^�[�ɂ���ꍇ�Aweak�|�C���^�[�ɂ���K�v������
	BattleScene* battle_scene_ = nullptr;

	//��
	const int COLOR_BRACK = 0;

	//���F
	const int COLOR_WHITE = -1;
};
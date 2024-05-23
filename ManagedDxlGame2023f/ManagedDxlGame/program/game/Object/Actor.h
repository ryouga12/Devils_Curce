#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include<string>

class BattleScene;

class Actor {
public:

	Actor() {};
	virtual~Actor() {};

	enum class ActorType {
		NONE,
		ENEMY,

	};

	ActorType actor_type = ActorType::NONE;

	//��̃|�C���^���Z�b�g����
	void SetBattleScene(BattleScene* battle_scene) {
		battleScene = battle_scene;
	}

protected:
	
	//��̃|�C���^��p�ӂ��Ă��ꂼ��Őݒ肵�ď�����ς���
	//Shead�|�C���^�[�ɂ���ꍇ�Aweak�|�C���^�[�ɂ���K�v������
	BattleScene* battleScene = nullptr;

};
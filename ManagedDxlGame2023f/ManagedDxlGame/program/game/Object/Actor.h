#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include<string>

class BattleScene;

class Actor {
protected:
	
	//��̃|�C���^��p�ӂ��Ă��ꂼ��Őݒ肵�ď�����ς���
	Shared<BattleScene>battleScene = nullptr;

public:

	//��̃|�C���^���Z�b�g����
	void SetBattleScene(Shared<BattleScene>battle_scene) {
		battleScene = battle_scene;
	}
};
#pragma once
#include"../Scene/InMapScene.h"
#include"../Scene/battleScene.h"
#include"../Manager/EventManager.h"
#include"../Object/NPC.h"
#include"../Manager/UiManager.h"

//---------------------------------------------------------------------------------------------------------
//�C�x���g���̎��s�������Ǘ�����N���X

class EventNpc final {
public:

	//���s�����̏�����
	void InitEventFunction(const std::string& npc_name);

	//�Z�[�u����
	//������l�����Ȃ������ꍇ�A���ɂȂ�
	void SaveProcces(const int curent_map_state = 0);

	//�R�����g����
	//�R�����g�𗬂���������s���ɖ߂�
	void ComentProcess(InMapScene* inmap_scene, const int MAX_COMENT);

	//�h���̉񕜏���
	void InnProcess(const int money);

	//�R�����g�p�̊֐����Ăяo��
	std::function<void(InMapScene*, int)>GetComentEvent(const std::string& npc_name) {

		//���������݂��Ă�����
		if (auto itr = coment_function_map.find(npc_name); itr != coment_function_map.end()) {
			if ((*itr).first == npc_name) {
				return coment_function_map[npc_name];
			}
			itr++;
		}
		//������������Ȃ�������
		else {
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("�֐���������܂���ł���");
			tnl::DebugTrace("\n------------------------------------------------------------");
		}
	}

	//Npc�̏������Ăяo��
	//����������͔C�ӂŎw��\
	void EventSelectProcces(const std::string& npc_name, const int value , const int value_2 = 0 , const std::string& bgm_1 = "" , const std::string& bgm_2 = "");

	//�G�ɘb���������̃R�����g����
	void EventEnemyProcces(const int back_ground, const int& enemy_id,const std::string& bgm_1, const std::string& bgm_2);

private:

	//�R�����g�𗬂����邾���̏���
	std::map <std::string ,std::function<void(InMapScene*, int) >> coment_function_map;

	//������int�^��������map(��ɑI���E�B���h�E�Ȃǂ̏����Ŏg�p����)
	std::map<std::string, std::function<void(int , int , const std::string& , const std::string&)>> function_map;

	//������A�j���[�V�������s�������̃t���O
	bool coment_animation_flag = false;
};
#pragma once
#include<unordered_map>
#include"../Scene/BaseScene.h"
#include"../Scene/battleScene.h"
#include"../Manager/EventManager.h"
#include"../Object/NPC.h"
#include"../Manager/UiManager.h"

//---------------------------------------------------------------------------------------------------------
//�C�x���g���̎��s�������Ǘ�����N���X

class Event final {
public:

	Event() = default;
	~Event();

	//���s�����̏�����
	//arg_1 : NPC�N���X�̃��X�g
	//NPC���ɏ����̏��������s��
	void InitEventFunction(const std::list<Shared<Npc>>& npc_list);

	//�Z�[�u����
	//arg_1 : �ǂ̃}�b�v�ɂ��邩(InMapScene��enum class��InMapState������)
	void SaveProcces(const int& curent_map_state);

	//�R�����g����
	//�R�����g�𗬂���������s���ɖ߂�
	//arg_1 : �V�[���̃|�C���^(�V�[���̃V�[�P���X�ɐG����)
	//arg_2 : �R�����g�̐�
	void ComentProcess(BaseScene* curent_map_scene, const int& MAX_COMENT);

	//�h���̉񕜏���
	//arg_1 : ���邨���̗�
	void InnProcess(const int& money);

	//�R�����g�p�̊֐����Ăяo��
	//arg_1 : NPC�̖��O
	std::function<void(BaseScene*, int)>GetComentEvent(const std::string& npc_name) {

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

		return 0;
	}

	//NPC�̏������Ăяo��
	//��O��������͓���̏ꍇ�Ŏg�p
	//arg_1 : NPC�̖��O
	//arg_2 : �������
	//arg_3 : �������(�o�g���V�[���ɑJ�ڂ��鏈���̂ݎg�p)
	//arg_4 : �����O��(�o�g���V�[���ɑJ�ڂ��鏈���̂ݎg�p)
	//arg_5 : �����l��(�o�g���V�[���ɑJ�ڂ��鏈���̂ݎg�p)
	//arg_6 : �����܂�(�o�g���V�[���ɑJ�ڂ��鏈���̂ݎg�p)
	void EventSelectProcces(const std::string& npc_name, const int& value , const int& value_2 = 0 , const std::string& bgm_1 = "" , const std::string& bgm_2 = "", const int& value_3 = 0);

	//�G�ɘb���������̃R�����g����
	//��Ƀo�g���Ɉڍs������
	//arg_1 : �w�i
	//arg_2 : �G��ID
	//arg_3 : �~�߂�BGM
	//arg_4 : �V��������BGM
	//arg_5 : �V�[���̂ǂ��ɂ���̂�(InMapScene��InMapState�̗񋓑̂���w�肷��)
	void EventEnemyProcces(const int& back_ground, const int& enemy_id,const std::string& bgm_1, const std::string& bgm_2, const int& in_map_state);

	//�R�����g�𗬂��؂��Ă���Enter�L�[�ŃV�[����J�ڂ�����
	bool ComentEventChange(const int& count_max);

private:

	//�R�����g�𗬂����邾���̏���
	std::unordered_map<std::string ,std::function<void(BaseScene*, int) >> coment_function_map;

	//������int�^��������map(��ɑI���E�B���h�E�Ȃǂ̏����Ŏg�p����)
	std::unordered_map<std::string, std::function<void(int , int , const std::string& , const std::string& , int)>> function_map;

	//������A�j���[�V�������s�������̃t���O
	bool coment_animation_flag = false;
};
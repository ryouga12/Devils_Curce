#include"EventNpc.h"
#include"../Manager/SceneManager.h"
#include"../Scene/InMapScene.h"


void EventNpc::InitEventFunction(const std::string& npc_name)
{
	//���O���擾���Ă��ꂼ����s����������������
	if (npc_name == "����") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}
	else if (npc_name == "����") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}
	else if (npc_name == "�_��") {
		function_map[npc_name] = std::bind(&EventNpc::SaveProcces, this, std::placeholders::_1);
	}
	else if (npc_name == "�h��") {
		function_map[npc_name] = std::bind(&EventNpc::InnProcess, this, std::placeholders::_1);
	}
	else if (npc_name == "����") {
		function_map[npc_name] = std::bind(&EventNpc::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	}
	else if (npc_name == "����") {
		function_map[npc_name] = std::bind(&EventNpc::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	}
	else if (npc_name == "��") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}
	else if (npc_name == "����") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}
	else if (npc_name == "�������l") {
		coment_function_map[npc_name] = std::bind(&EventNpc::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
	}

}

//�Z�[�u����
void EventNpc::SaveProcces(int curent_map_state)
{
	//�v���C���[�Ƀ}�b�v�̏�Ԃ�`����(���[�h�����ۂɂǂ̃}�b�v�ɋ��邩�c�������)
	GameManager::getGameManager()->getPlayer()->SetPlayerCurentMapMemory(curent_map_state);
	//�Z�[�u���s��
	GameManager::getGameManager()->getPlayer()->PlyerSave();
	//�Z�[�u�̃e�L�X�g���Ăяo��
	UIManager::getUIManager()->SaveTextFlagChange();
}

void EventNpc::ComentProcess(InMapScene* inmap_scene, const int MAX_COMENT)
{
	//�R�����g��\������
	if (EventManager::getEventManager()->ComentEventChange(MAX_COMENT) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//�R�����g�t���O��false��������
		//�����t���[���𑖂点�Ȃ���
		if (!coment_animation_flag) {

			coment_animation_flag = true;

		}
		else {
			inmap_scene->SequenceChange(InMapScene::Sequence_num::MOVE);
			coment_animation_flag = false;
		}
	}
}

//�h������
void EventNpc::InnProcess(const int money)
{
	//---�h�������p---//

	//���݂�HP���擾����
	auto player_curent_hp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetcurentHp();

	//���݂�MP���擾����
	auto player_curent_mp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetCurentMp();

	//�ő�HP���擾����
	auto player_max_hp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetMaxHp();

	//�ő�MP���擾����
	auto player_max_mp = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetMaxMp();

	//�V�[���}�l�[�W���[�̃C���X�^���X���擾����
	auto mgr = SceneManager::GetInstance();

	if (mgr->GetSceneFlag()) {
		//�t���O��؂�ւ��ăV�[����J�ڂ����Ȃ��悤�ɂ���
		mgr->SceneFlagChange();
	}

	//�t�F�[�h������
	SceneManager::GetInstance()->FadeScene();

	//�v���C���[�̂���������������
	GameManager::getGameManager()->getPlayer()->ReducePlayerMoney(money);

	//�񕜉���炷
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/heal_inn.mp3", DX_PLAYTYPE_BACK);

	//HP��MP���񕜂�����
	if (player_curent_hp < player_max_hp) {
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentHp(player_max_hp);
	}
	if (player_curent_mp < player_max_mp) {
		GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentMp(player_max_mp);
	}


}

void EventNpc::EventSelectProcces(const std::string& npc_name, const int value, const int value_2, const std::string& bgm_1, const std::string& bgm_2)
{
	//���������݂��Ă�����
	if (auto itr = function_map.find(npc_name); itr != function_map.end()) {
		if ((*itr).first == npc_name) {
			(*itr).second(value , value_2 , bgm_1, bgm_2);
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

//�G�ɘb���������̃R�����g����
void EventNpc::EventEnemyProcces(const int back_ground , const int& enemy_id, const std::string& bgm_1, const std::string& bgm_2)
{
	//BGM���~�߂�
	SoundManager::getSoundManager()->StopSound(bgm_1);

	//�o�g���p��BGM�𗬂�
	SoundManager::getSoundManager()->sound_Play(bgm_2, DX_PLAYTYPE_LOOP);

	auto mgr = SceneManager::GetInstance();

	//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
	if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

	//�V�[����J�ڂ�����(�v���C���[�̍��W��n��,�G��ID,�G�̃|�C���^��n��,�ǂ��̃}�b�v�ɂ��邩�̏�Ԃ�n��)
	mgr->changeScene(new BattleScene(GameManager::getGameManager()->getPlayer()->getPlayerPos(), back_ground, std::make_shared<BossMonster>(enemy_id) , static_cast<int>(InMapScene::InMapState::TOWN)));

}

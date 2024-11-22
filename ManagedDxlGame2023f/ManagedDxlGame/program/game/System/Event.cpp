#include"Event.h"
#include"../Manager/SceneManager.h"
#include"../Scene/InMapScene.h"
#include"../Manager/GameManager.h"


Event::~Event()
{
	//�����i�[�p�̔z����N���A����
	coment_function_map.clear();
	function_map.clear();
}

void Event::InitEventFunction(const std::list<Shared<Npc>>& npc_list)
{
	//npc�ɂ���ď���������������
	for (auto& npc_list_ : npc_list) {

		//�|�C���^�����݂��Ȃ��ꍇ�z��𔲂���
		if (!npc_list_) { break; }

		//���O���擾���Ă��ꂼ����s����������������
		if (npc_list_->GetNpcName() == "����") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "����") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "�_��") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::SaveProcces, this, std::placeholders::_1);
		}
		else if (npc_list_->GetNpcName() == "�h��") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::InnProcess, this, std::placeholders::_1);
		}
		else if (npc_list_->GetNpcName() == "����") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 , std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "����") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "��") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "����") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "�������l") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "�j�����l") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "�j������") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "�j���Z��") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "�w��") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "���m") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
		else if (npc_list_->GetNpcName() == "�����̔Ԑl") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 , std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "�e�̔Ԑl") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "����") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "�D��") {
			function_map[npc_list_->GetNpcName()] = std::bind(&Event::EventEnemyProcces, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}
		else if (npc_list_->GetNpcName() == "�m��") {
			coment_function_map[npc_list_->GetNpcName()] = std::bind(&Event::ComentProcess, this, std::placeholders::_1, std::placeholders::_2);
		}
	}
}

//�Z�[�u����
void Event::SaveProcces(const int& curent_map_state)
{
	//�v���C���[�Ƀ}�b�v�̏�Ԃ�`����(���[�h�����ۂɂǂ̃}�b�v�ɋ��邩�c�������)
	GameManager::GetGameManager()->GetPlayer()->SetPlayerCurentMapMemory(curent_map_state);
	//�Z�[�u���s��
	GameManager::GetGameManager()->GetPlayer()->PlayerSave();
	//�Z�[�u�̃e�L�X�g���Ăяo��
	UIManager::GetUIManager()->SaveTextFlagChange();
}

//�R�����g�𗬂�
void Event::ComentProcess(BaseScene* curent_map_scene, const int& MAX_COMENT)
{
	//�R�����g��\������
	if (ComentEventChange(MAX_COMENT) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		//�R�����g�t���O��false��������
		//�����t���[���𑖂点�Ȃ���
		if (!coment_animation_flag) {

			coment_animation_flag = true;

		}
		else {
			curent_map_scene->SequenceChangeMove();
			coment_animation_flag = false;
		}
	}
}

//�h������
void Event::InnProcess(const int& money)
{
	//����������Ȃ���΂������^�[������
	if (auto player_curenent_money = GameManager::GetGameManager()->GetPlayer()->GetPlayerMoney() < money) { return; }

	//---�h�������p---//

	//���݂�HP���擾����
	auto player_curent_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp();

	//���݂�MP���擾����
	auto player_curent_mp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetCurentMp();

	//�ő�HP���擾����
	auto player_max_hp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxHp();

	//�ő�MP���擾����
	auto player_max_mp = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetMaxMp();

	//�V�[���}�l�[�W���[�̃C���X�^���X���擾����
	auto mgr = SceneManager::GetSceneManager();

	if (mgr->GetSceneFlag()) {
		//�t���O��؂�ւ��ăV�[����J�ڂ����Ȃ��悤�ɂ���
		mgr->SceneFlagChange();
	}

	//�t�F�[�h������
	SceneManager::GetSceneManager()->FadeScene();

	//�v���C���[�̂���������������
	GameManager::GetGameManager()->GetPlayer()->ReducePlayerMoney(money);

	//�񕜉���炷
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/heal_inn.mp3", DX_PLAYTYPE_BACK);

	//HP��MP���񕜂�����
	if (player_curent_hp < player_max_hp) {
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentHp(player_max_hp);
	}
	if (player_curent_mp < player_max_mp) {
		GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentMp(player_max_mp);
	}


}

//�������Ăяo��
void Event::EventSelectProcces(const std::string& npc_name, const int& value, const int& value_2, const std::string& bgm_1, const std::string& bgm_2 , const int& value_3)
{
	//���������݂��Ă�����
	if (auto itr = function_map.find(npc_name); itr != function_map.end()) {
		if ((*itr).first == npc_name) {
			(*itr).second(value , value_2 , bgm_1, bgm_2 , value_3);
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
void Event::EventEnemyProcces(const int& back_ground , const int& enemy_id, const std::string& bgm_1, const std::string& bgm_2 , const int& in_map_state)
{
	//BGM���~�߂�
	SoundManager::GetSoundManager()->StopSound(bgm_1);

	//�o�g���p��BGM�𗬂�
	SoundManager::GetSoundManager()->Sound_Play(bgm_2, DX_PLAYTYPE_LOOP);

	auto mgr = SceneManager::GetSceneManager();

	//�t���O��false�̏ꍇtrue�ɕύX����(�V�[����J�ڂ������)
	if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

	//�V�[����J�ڂ�����(�v���C���[�̍��W��n��,�G��ID,�G�̃|�C���^��n��,�ǂ��̃}�b�v�ɂ��邩�̏�Ԃ�n��)
	mgr->changeScene(new BattleScene(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos(), back_ground, std::make_shared<BossMonster>(enemy_id), in_map_state));

}

//�R�����g�𗬂��؂��Ă���Enter�L�[�ŃV�[����J�ڂ�����
bool Event::ComentEventChange(const int& count_max)
{
	//�R�����g���R�}����ŕ\������
	UIManager::GetUIManager()->ComentNextByInput(count_max);

	//�w��̃R�����g�ȏ�ɂȂ�����true��Ԃ�
	if (UIManager::GetUIManager()->GetCount() >= count_max) {
		return true;
	}

	return false;
}
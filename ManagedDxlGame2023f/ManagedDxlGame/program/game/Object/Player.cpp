#include "Player.h"
#include"../System/Camera.h"
#include"../Menu/BattleLog.h"
#include"Enemy.h"
#include"../Skill/Skill.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/GameManager.h"
#include"../Manager/CsvManager.h"
#include"../Scene/battleScene.h"
#include"../Scene/InMapScene.h"


Player::Player() : money(50)
{
	//csv�̓ǂݎ��
	plyer_status_csv_info
		= CsvManager::GetCsvManager()->GetPlayerStatusCsv();

	//plyer�̃X�e�[�^�X���擾����
	PlyStatusLoad();

	//�X�e�[�^�X��ݒ肷��(Lv1����X�^�[�g)
	SetPlayerStatus(1);

	//�X�L���̃|�C���^�𐶐�����
	skill = std::make_shared<Skill>();
	skill->AddSkillList();

}

Player::~Player()
{

	//�v���C���[�̉摜���
	for (int i = 0; i < DIR_MAX; i++) {
		for (int k = 0; k < 5; k++) {
			DeleteGraph(anim_hdls[i][k]);
			// �����̃|�C���^�̒l���f�o�b�O�g���[�X�Ŋm�F
			tnl::DebugTrace("\n-----------------------------------------------------------------------------\n");
			tnl::DebugTrace("\nDeleted Playerimage at [%d][%d]. Pointer value: %p\n", i, k, anim_hdls[i][k]);
		}
	}
}

//�v���C���[�̓���
//���� : Delta_time , �������x�@,�@�}�b�v�̍���(�ړ������̈�)
void Player::Player_Move(const float& delta_time , const float& velocity , const int& MAPHEIGHT)
{
	//prev_pos = plyer_pos;

	if (play_control) {

		//�v���C���[�̓���

		//����΂�
		if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) {

			plyer_pos.x += velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y += velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;
			prev_pos = plyer_pos;

		}
		//�E��΂�
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) {

			plyer_pos.x -= velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y += velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;
			prev_pos = plyer_pos;

		}
		//�����΂�
		else if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) {	

			plyer_pos.x += velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y -= velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;
			prev_pos = plyer_pos;
		}
		//�E���΂�
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) {

			plyer_pos.x -= velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y -= velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;
			prev_pos = plyer_pos;
		}
		//��
		else if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			plyer_pos.x -= velocity;
			numberStep++;
			prev_pos = plyer_pos;
			
		}
		//�E
		else if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			plyer_pos.x += velocity;
			numberStep++;
			prev_pos = plyer_pos;
			
		}
		//��
		else if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			plyer_pos.y -= velocity;
			numberStep++;
			prev_pos = plyer_pos;
			
		}
		//��
		else if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			plyer_pos.y += velocity;
			numberStep++;
			prev_pos = plyer_pos;
		}

		//�A�j���[�V�����̓���
		
		//����΂�
		if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_W)))	   { anim_ctrl_dir = DIR_DIAGONAL_UPPER_LEFT; }
		//�E��΂�
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) { anim_ctrl_dir = DIR_DIAGONAL_UPPER_RIGTH; }
		//�����΂�
		else if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) { anim_ctrl_dir = DIR_DIAGONAL_DOWN_LEFT; }
		//�E���΂�
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) { anim_ctrl_dir = DIR_DIAGONA_DOWN_RIGTH; }
		//��
		else if (tnl::Input::IsKeyDown(eKeys::KB_W))										   { anim_ctrl_dir = DIR_UP; }
		//��
		else if (tnl::Input::IsKeyDown(eKeys::KB_S))										   { anim_ctrl_dir = DIR_DOWN; }
		//��
		else if (tnl::Input::IsKeyDown(eKeys::KB_A))										   { anim_ctrl_dir = DIR_LEFT; }
		//�E
		else if (tnl::Input::IsKeyDown(eKeys::KB_D))										   { anim_ctrl_dir = DIR_RIGHT; }
	}
}

//�`��
void Player::Player_draw(const KonCamera& camera ,const float& scale)
{
	if (player_display_flag) {
		tnl::Vector3 draw_pos = plyer_pos - camera.GetTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
		DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), scale, 0, anim_hdls[anim_ctrl_dir][anim_ctrl_frame], true);
	}
}

//�v���C���[�̃A�j���[�V��������
void Player::PlayerAnimation(const float& delta_time)
{
	//�A�j���[�V����
	anim_time_count += delta_time;
	if (anim_time_count > ChangeAnimationTime) {
		anim_ctrl_frame++;
		anim_ctrl_frame %= animationFlame;

		anim_time_count = 0;
	}
}

//�v���C���[�̍s������
void Player::PlayerMoveProcess(float delta_time , Shared<BattleLog>& basttle_log , Shared<Enemy>& enemy , Shared<Nomal_Attack>& nomal_attack)
{
	switch (player_state)
	{
	
	//�ʏ펞
	case Player::PlayerState::CHOICE:

		//�v���C���[�̃R�}���h����
		battle_scene_->MenuUpdate(player_status_save, enemy->GetEnemyArray()[enemy->GetEnemy_Index()]);

		break;

	//�ʏ�U����
	case Player::PlayerState::NOMALATTACK:

		//�ʏ�U�����̏���
		PlayerAttackProcess(enemy->GetEnemyArray()[enemy->GetEnemy_Index()], basttle_log , nomal_attack);

		break;


	//���Z�̍U����
	case Player::PlayerState::SKILL:

		//���Z�g�p���̏���
		battle_scene_->SkillUseProcess(player_status_save , enemy->GetEnemyArray()[enemy->GetEnemy_Index()]);

		break;

	//������
	case Player::PlayerState::FLEE:

		//������ۂ̏���
		battle_scene_->FleeProcess(player_status_save, enemy->GetEnemyArray()[enemy->GetEnemy_Index()], delta_time);

		break;

	//���S��
	case Player::PlayerState::DEAD:

		//�v���C���[�̎��S����
		DeadPlayerProcess(basttle_log);

		break;

	//�ҋ@���
	case Player::PlayerState::IDLE:


		break;

	default:

		tnl::DebugTrace("\n------------------------------------------------------------");
		tnl::DebugTrace("\n ���܂��I���ł��܂���ł���");
		tnl::DebugTrace("\n------------------------------------------------------------");

		break;
	}
}

//csv�ŃX�e�[�^�X��ǂݍ���
void Player::PlyStatusLoad()
{
	int max_num = static_cast<int>(plyer_status_csv_info.size());

	player_status_total_num = max_num;

	for (int y = 1; y < max_num; y++) {

		//csv����level�̎擾
		player_status_save.SetPlayerLevel(plyer_status_csv_info[y][0].getInt());
		//csv����maxHp�̎擾
		player_status_save.SetPlayerMaxHp(plyer_status_csv_info[y][1].getInt());
		//csv����hp�̎擾
		player_status_save.SetPlayerCurentHp(plyer_status_csv_info[y][1].getInt());
		//csv����Attack�̎擾
		player_status_save.SetPlayerAttack(plyer_status_csv_info[y][2].getInt());
		//csv����base_attack�̎擾
		player_status_save.SetPlayerBaseAttack(plyer_status_csv_info[y][2].getInt());
		//csv����base_defance�̎擾
		player_status_save.SetBaseDefance(plyer_status_csv_info[y][3].getInt());
		//csv����Defance�̎擾
		player_status_save.SetPlayerDefance(plyer_status_csv_info[y][3].getInt());
		//csv����Speed�̎擾
		player_status_save.SetPlayerSpeed(plyer_status_csv_info[y][4].getInt());
		//csv����K�v�Ȍo���l�̎擾
		player_status_save.SetPlayerExpoint(plyer_status_csv_info[y][5].getInt());
		//csv����ő�Mp���擾
		player_status_save.SetPlayerCurentMp(plyer_status_csv_info[y][6].getInt());
		//csv����Mp���擾
		player_status_save.SetPlayerMaxMp(plyer_status_csv_info[y][6].getInt());
		//csv����magicPower�̎擾
		player_status_save.SetMagicPower(plyer_status_csv_info[y][7].getInt());

		player_status_type.emplace_back(player_status_save);

	}	

}

//�v���C���[�̉摜��ǂݍ���
//ID�ɂ���ĉ摜��؂�ւ���
//ID : 2�����z��̉��s�ڂ������Ă��邩
void Player::SetPlayerAnimationHdl(const int& ghdl_id , const int& player_id)
{
	//�A�j���[�V�����摜���i�[�z��
	std::vector<std::string> player_image;

	auto playerAnimationSize = CsvManager::GetCsvManager()->GetPlayerAnimationImage().size();

	for (int i = 1; i < playerAnimationSize; i++) {
		if (ghdl_id == std::stoi(CsvManager::GetCsvManager()->GetPlayerAnimationImage()[i][0].c_str()) && player_id == std::stoi(CsvManager::GetCsvManager()->GetPlayerAnimationImage()[i][9].c_str())) {
			for (int k = 1; k < DIR_MAX + 1 ; k++) {
				player_image.emplace_back(CsvManager::GetCsvManager()->GetPlayerAnimationImage()[i][k]);
			}
		}
	}

	for (int i = 0; i < DIR_MAX; i++) {

		LoadDivGraph(
			player_image[i].c_str(),						// �摜�̃p�X
			TOTAL_FRAME,									// ���t���[����
			HORIZONTAL_FRAME,								// ���t���[����
			VERTICAL_FRAME,									// �c�t���[����
			HORIZONTAL_ONEFRAME,							// �������� 1�t���[��������̕�
			VERTICAL_ONEFRAME,								// �c������ 1�t���[��������̕�
			anim_hdls[i]);									// �摜�n���h�����i�[����z��);
	}

	//�v���C���[�̃A�C�R�������[�h����
	player_icon = ResourceManager::GetResourceManager()->LoadGraphEX(PLAYER_BATTLE_ICON[player_id]);
}

//�Z�[�u����ۂ̃X�L���̒ǉ�
void Player::SaveReadSkill(const int& skill_id )
{
	// �X�L�����X�g���擾
	auto& skill_list = skill->GetOverallSkills();

	// skill_id �Ɉ�v����X�L��������
	auto it = std::find_if(skill_list.begin(), skill_list.end(),
		[skill_id](const Shared<Skill>& skill) { return skill->getId() == skill_id; });

	// �X�L�������������ꍇ
	if (it != skill_list.end()) {
		player_skill_list.emplace_back(*it);
	}
	// �X�L����������Ȃ������ꍇ
	else {
		tnl::DebugTrace("----------------------------------------------------------------\n");
		tnl::DebugTrace("�X�L����ǉ��ł��܂���ł���\n");
	}
}

Player::PlayerStatus Player::GetPlyerStatus(const int& level) const
{
	auto it = std::find_if(player_status_type.begin(), player_status_type.end(), [level]
	(const PlayerStatus& status) {return status.GetLevel() == level; });

	if (it != player_status_type.end()) {
		return *it;
	}
	else {
		// ID�ɑΉ�����A�C�e����������Ȃ��ꍇ�́A�K���ȃf�t�H���g�l��Ԃ����G���[�������s���Ȃ�
		return PlayerStatus{};
	}
}

//�v���C���[�̎��S����
void Player::DeadPlayerProcess(Shared<BattleLog>& battle_log)
{
	//�v���C���[��HP��0�����������
	if (player_status_save.GetcurentHp() <= 0) {

		//���O�𗬂�
		battle_log->AddLog("�S�ł���");

		//���O�𗬂�
		battle_log->AddLog("�v���C���[�͂����𔼕�������");

		//�v���C���[��Hp��0�ɂ���
		player_status_save.SetPlayerCurentHp(0);

		//BGM���~�߂�
		SoundManager::GetSoundManager()->StopSound("sound/BGM/sentou.mp3");
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/zenmetu.mp3", DX_PLAYTYPE_BACK);

		//�o�g�����I��������
		battle_scene_->SetBattleState(BattleScene::BattleState::IDLE);
	}
	else {
		return;
	}
}

//�v���C���[�̒ʏ�U������
void Player::PlayerAttackProcess(Enemy::EnemyConnection& enemy_status, Shared<BattleLog>& battle_log , Shared<Nomal_Attack>& nomal_attack)
{
	//�G������ŋ����珈�����Ƃ΂�
	if (enemy_status.GetEnemyHp() <= 0)return;

	//���艹��炷
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

	//�G�Ƀ_���[�W��^����
	nomal_attack->SkillUse(player_status_save, enemy_status ,battle_log);

}

//�X�L�����Z�b�g����
void Player::SkillSet(const Shared<BattleLog>& battle_log)
{
	switch (player_status_save.GetLevel())
	{
		//���x��2�̎��ɉΉ��a����o����
	case 2:
		AddSkill(std::make_shared<FlameSlash>());

		if (battle_log) {
			//���O�𗬂�
			battle_log->AddLog("�Ή��a����o����!");
		}

		break;
		//���x��3�̎��Ƀq�[�����o����
	case 3:
		
		AddSkill(std::make_shared<Heal>());

		if (battle_log) {
			battle_log->AddLog("�q�[�����o����!");
		}

		break;
		//���x��4�̎��ɃA�C�X�u���X�g���o����
	case 4:

		AddSkill(std::make_shared<IceBlast>());

		if (battle_log) {
			battle_log->AddLog("�A�C�X�u���X�g���o����!");
		}

		break;
		//���x��5�̎��ɃT���_�[�{���g���o����
	case 5:
		AddSkill(std::make_shared<ThunderBolt>());

		if (battle_log) {
			battle_log->AddLog("�T���_�[�{���g���o����!");
		}

		break;

	//���x��8�̎��Ƀt�@�C�A�[�X�g�[�����o����
	case 8:

		AddSkill(std::make_shared<FireStorm>());

		if (battle_log) {
			battle_log->AddLog("�t�@�C�A�[�X�g�[�����o�����I");
		}

		break;

	//���x��10�̎��ɃE�H�[�^�[�u���[�h���o����
	case 10:

		AddSkill(std::make_shared<WaterBlade>());

		if (battle_log) {
			battle_log->AddLog("�E�H�[�^�[�u���[�h���o�����I");
		}

		break;

	//���x��14�̎��ɃC���t�F���m���o����
	case 14:

		AddSkill(std::make_shared<Inferno>());

		if (battle_log) {
			battle_log->AddLog("�C���t�F���m���o�����I");
		}

		break;

	//���x��16�̎��Ƀu���U�[�h���o����
	case 16:

		AddSkill(std::make_shared<Blizzard>());

		if (battle_log) {
			battle_log->AddLog("�u���U�[�h���o�����I");
		}

	//���x��18�̎��Ƀ��C�g�j���O�X���b�V�����o����
	case 18:

		AddSkill(std::make_shared<LightningSlash>());

		if (battle_log) {
			battle_log->AddLog("���C�g�j���O�X���b�V�����o�����I");
		}

		break;
	}
}

//�X�L����ǉ�����
void Player::AddSkill(const Shared<Skill>& skill)
{
	player_skill_list.emplace_back(skill);
	skill_num++;
}

//�Z�[�u
void Player::PlayerSave()
{
	FILE* fp = nullptr;
	fopen_s(&fp, "player.bin", "wb");

	if (fp) {

		// Player�X�e�[�^�X���擾����

		//hp
		int player_hp = player_status_save.GetcurentHp();

		//max_hp
		int player_max_hp = player_status_save.GetMaxHp();

		//mp
		int player_mp = player_status_save.GetCurentMp();

		//max_mp
		int player_max_mp = player_status_save.GetMaxMp();

		//���@��
		int player_magic_power = player_status_save.GetMagicPower();

		//�U����
		int player_attack = player_status_save.GetBaseAttack();

		//�h���
		int player_defense = player_status_save.GetBaseDefance();

		//�f����
		int player_speed = player_status_save.GetSpeed();

		//�v���C���[�̉摜ID
		int player_graphics_id = player_id;

		// �v���C���[�̖��O��ۑ�����
		const std::string& player_name = name;

		//���O�̒���
		size_t name_length = player_name.size();

		//���x��
		int player_level = player_status_save.GetLevel();

		//�K�v�Ȍo���l
		int player_expoint = player_status_save.GetExpoint();

		//�����蔻��̊֌W�ō��W����������
		tnl::Vector3 add_pos = { 0, 20 , 0 };

		//���݈ʒu
		tnl::Vector3 player_pos = plyer_pos + add_pos;

		//�C���x���g���̃T�C�Y���񂵂Ĕz��Ɋi�[����
		const auto& player_list = GameManager::GetGameManager()->GetInventory()->GetInventoryList();
		std::vector<int> player_item_list;

		//�C���x���g���ɃA�C�e���������
		if (!player_list.empty()) {
			for (int i = 0; i < player_list.size(); i++) {
				player_item_list.emplace_back(GameManager::GetGameManager()->GetInventory()->GetInventoryList()[i].GetItemId());
			}
		}

		//�X�L�����X�g�̃T�C�Y���񂵂Ĕz��Ɋi�[����
		std::vector<int> player_skill_list_array;

		//�X�L�����X�g�ɃX�L���������
		if (!player_skill_list.empty()) {

			for (int i = 0; i < player_skill_list.size(); i++) {
				player_skill_list_array.emplace_back(player_skill_list[i]->getId());
			}
		}

		//�C�x���g�֌W�̃t���O
		bool event_flag_1 = EventManager::GetEventManager()->GetEnemyEventFlag_1();
		bool event_flag_2 = EventManager::GetEventManager()->GetEnemyEventFlag_2();
		bool event_flag_3 = EventManager::GetEventManager()->GetEnemyEventFlag_3();

		//�v���C���[�̃X�e�[�^�X��ۑ�����

		//hp
		fwrite(&player_hp, sizeof(int), 1, fp);
		//max_hp
		fwrite(&player_max_hp, sizeof(int), 1, fp);
		//mp
		fwrite(&player_mp, sizeof(int), 1, fp);
		//max_mp 
		fwrite(&player_max_mp, sizeof(int), 1, fp);
		//���@��
		fwrite(&player_magic_power, sizeof(int), 1, fp);
		//�U����
		fwrite(&player_attack, sizeof(int), 1, fp);
		//�h���
		fwrite(&player_defense, sizeof(int), 1, fp);
		//�f����
		fwrite(&player_speed, sizeof(int), 1, fp);
		//�v���C���[��ID
		fwrite(&player_graphics_id, sizeof(int), 1, fp);
		//���x��
		fwrite(&player_level, sizeof(int), 1, fp);
		//�K�v�Ȍo���l
		fwrite(&player_expoint, sizeof(int), 1, fp);
		//���ݍ��W
		fwrite(&player_pos, sizeof(tnl::Vector3), 1, fp);
		//�v���C���[���L�����Ă����}�b�v���
		fwrite(&player_curent_map_memory, sizeof(int), 1, fp);
		//������
		fwrite(&money, sizeof(int), 1, fp);
		//�C�x���g�̃t���O�֌W�̕ۑ�
		fwrite(&event_flag_1, sizeof(bool), 1, fp);
		fwrite(&event_flag_2, sizeof(bool), 1, fp);
		fwrite(&event_flag_3, sizeof(bool), 1, fp);

		// ������̒�����ۑ�
		fwrite(&name_length, sizeof(size_t), 1, fp); 
		// �����񂻂̂��̂�ۑ�
		fwrite(player_name.c_str(), sizeof(char), name_length, fp); 

		//�C���x���g���̕ۑ�
		// �T�C�Y���i�[����
		size_t vector_size = player_item_list.size();
		fwrite(&vector_size, sizeof(size_t), 1, fp);

		// �v�f��ۑ�����
		fwrite(player_item_list.data(), sizeof(int), vector_size, fp);

		//�X�L�����X�g�̕ۑ�
		//�T�C�Y���i�[����
		size_t skill_size = player_skill_list_array.size();
		fwrite(&skill_size, sizeof(size_t), 1, fp);

		//�X�L����ۑ�����
		fwrite(player_skill_list_array.data(), sizeof(int), skill_size, fp);

		fclose(fp);
	}

}

//���[�h
void Player::PlayerLoad()
{
	FILE* fp = nullptr;
	fopen_s(&fp, "player.bin", "rb");

	if (fp) {

		// �ꎞ�I�ɕۑ����Ă����X�e�[�^�X
		int player_hp;
		int player_max_hp;
		int player_mp;
		int player_max_mp;
		int player_magic_power;
		int player_attack;
		int player_defense;
		int player_speed;
		int player_graphics_id;
		int player_level;
		int player_expoint;
		tnl::Vector3 player_pos;
		std::vector<int> player_inventory_list;
		std::vector<int> playe_skill_list;
		std::vector<int> playe_weapon_list;
		bool event_flag_1;
		bool event_flag_2;
		bool event_flag_3;

		// �X�e�[�^�X�����[�h����

		//hp
		fread(&player_hp, sizeof(int), 1, fp);
		//max_hp
		fread(&player_max_hp, sizeof(int), 1, fp);
		//mp
		fread(&player_mp, sizeof(int), 1, fp);
		//max_mp
		fread(&player_max_mp, sizeof(int), 1, fp);
		//���@��
		fread(&player_magic_power, sizeof(int), 1, fp);
		//�U����
		fread(&player_attack, sizeof(int), 1, fp);
		//�h���
		fread(&player_defense, sizeof(int), 1, fp);
		//�f����
		fread(&player_speed, sizeof(int), 1, fp);
		//�v���C���[��ID
		fread(&player_graphics_id, sizeof(int), 1, fp);
		//���x��
		fread(&player_level, sizeof(int), 1, fp);
		//�K�v�Ȍo���l
		fread(&player_expoint, sizeof(int), 1, fp);
		//���ݍ��W
		fread(&player_pos, sizeof(tnl::Vector3), 1, fp);
		//�v���C���[���L�����Ă���}�b�v���
		fread(&player_curent_map_memory, sizeof(int), 1, fp);
		//������
		fread(&money, sizeof(int), 1, fp);
		//�C�x���g�̃t���O�֌W
		fread(&event_flag_1, sizeof(bool), 1, fp);
		fread(&event_flag_2, sizeof(bool), 1, fp);
		fread(&event_flag_3, sizeof(bool), 1, fp);

		// �v���C���[�̖��O�����[�h����
		size_t name_length;

		// ������̒��������[�h
		fread(&name_length, sizeof(size_t), 1, fp);
		//�T�C�Y�����肷��
		std::string player_name;
		player_name.resize(name_length);

		// �����񂻂̂��̂����[�h
		fread(&player_name[0], sizeof(char), name_length, fp);

		//�C���x���g���̃��[�h
		// �z��̃T�C�Y�����[�h����
		size_t vector_size;
		fread(&vector_size, sizeof(size_t), 1, fp);

		// �v�f��ێ����邽�߂ɔz��̃T�C�Y��ύX
		player_inventory_list.resize(vector_size);

		//�v�f��z��ɕۑ�����
		fread(player_inventory_list.data(), sizeof(int), vector_size, fp);

		//�X�L�����X�g�̃��[�h
		// �z��̃T�C�Y�����[�h����
		size_t skill_size;
		fread(&skill_size, sizeof(size_t), 1, fp);

		// �v�f��ێ����邽�߂ɔz��̃T�C�Y��ύX
		playe_skill_list.resize(skill_size);

		//�v�f��z��ɕۑ�����
		fread(playe_skill_list.data(), sizeof(int), skill_size, fp);

		// �v���C���[�̃X�e�[�^�X���Z�b�g����
		player_status_save.SetPlayerCurentHp(player_hp);
		player_status_save.SetPlayerMaxHp(player_max_hp);
		player_status_save.SetPlayerCurentMp(player_mp);
		player_status_save.SetPlayerMaxMp(player_max_mp);
		player_status_save.SetMagicPower(player_magic_power);
		player_status_save.SetPlayerAttack(player_attack);
		player_status_save.SetPlayerBaseAttack(player_attack);
		player_status_save.SetPlayerDefance(player_defense);
		player_status_save.SetBaseDefance(player_defense);
		player_status_save.SetPlayerSpeed(player_speed);
		player_status_save.SetPlayerLevel(player_level);
		player_status_save.SetPlayerExpoint(player_expoint);
		plyer_pos = player_pos;
		player_id = player_graphics_id;
		name = player_name;

		//�C�x���g�֌W�̃t���O���Z�b�g����
		EventManager::GetEventManager()->SetEnemyEventFlag_1(event_flag_1);
		EventManager::GetEventManager()->SetEnemyEventFlag_2(event_flag_2);
		EventManager::GetEventManager()->SetEnemyEventFlag_3(event_flag_3);


		// �C���x���g���̗v�f���i�[����
		for (int item : player_inventory_list) {
			GameManager::GetGameManager()->GetInventory()->AddInventory(item);
		}

		//�X�L�����X�g�̗v�f���i�[����
		for (int skill : playe_skill_list) {
			SaveReadSkill(skill);
		}

		fclose(fp);
	}
}

//�X�L����S�Ēǉ�����(�f�o�b�N�p)
void Player::DebugAddSkill()
{
	//�X�L���̃f�o�b�N�J�E���g
	static int debug_skill_count = 0;

	//���̂݃X�L����ǉ��ł���
	if (debug_skill_count < 1) {
		for (auto& skill_lists : skill->GetOverallSkills()) {
			SaveReadSkill(skill_lists->getId());
		}
		debug_skill_count++;
	}
}

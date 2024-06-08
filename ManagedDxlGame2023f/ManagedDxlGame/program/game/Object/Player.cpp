#include "Player.h"
#include"../Manager/Camera.h"
#include"../Menu/BattleLog.h"
#include"Enemy.h"
#include"../Skill/Skill.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/GameManager.h"
#include"../Manager/CsvManager.h"
#include"../Scene/battleScene.h"


Player::Player() : money(1000)
{
	name = "Player";
	
	//csv�̓ǂݎ��
	PlyerStatus_Csv_Info
		= CsvManager::getCsvManager()->GetPlayerStatusCsv();

	//plyer�̃X�e�[�^�X���擾����
	PlyStatusLoad();

	//�X�e�[�^�X��ݒ肷��(Lv1����X�^�[�g)
	SetPlayerStatus(1);

	//�n�܂�͉����������Ă��Ȃ���0����X�^�[�g
	SetPlayerAnimationHdl(0);
}

Player::~Player()
{

	//�v���C���[�̉摜���
	for (int i = 0; i < static_cast<int>(DIR_MAX); i++) {
		for (int k = 0; k < 5; k++) {
			DeleteGraph(anim_hdls[i][k]);
			// �����̃|�C���^�̒l���f�o�b�O�g���[�X�Ŋm�F
			tnl::DebugTrace("-----------------------------------------------------------------------------\n");
			tnl::DebugTrace("Deleted Playerimage at [%d][%d]. Pointer value: %p\n", i, k, anim_hdls[i][k]);
		}
	}
}

void Player::Update(float delta_time)
{

}


void Player::player_Move(float delta_time , const float& velocity)
{
	prev_pos = plyer_pos;

	//�A�j���[�V����
	anim_time_count += delta_time;
	if (anim_time_count > ChangeAnimationTime) {
		anim_ctrl_frame++;
		anim_ctrl_frame %= animationFlame;

		anim_time_count = 0;
	}

	if (plyControl) {

		//�v���C���[�̓���

		//����΂�
		if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) {

			plyer_pos.x += velocity * static_cast<float>(std::sin(CHARA_WIDTH / 2));
			plyer_pos.y += velocity * static_cast<float>(std::sin(CHARA_HEIGHT / 2));
			numberStep++;

		}
		//�E��΂�
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) {

			plyer_pos.x -= velocity * static_cast<float>(std::sin(CHARA_WIDTH / 2));
			plyer_pos.y += velocity * static_cast<float>(std::sin(CHARA_HEIGHT / 2));
			numberStep++;

		}
		//�����΂�
		else if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) {	

			plyer_pos.x += velocity * static_cast<float>(std::sin(CHARA_WIDTH / 2));
			plyer_pos.y -= velocity * static_cast<float>(std::sin(CHARA_HEIGHT / 2));
			numberStep++;
		}
		//�E���΂�
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) {

			plyer_pos.x -= velocity * static_cast<float>(std::sin(CHARA_WIDTH / 2));
			plyer_pos.y -= velocity * static_cast<float>(std::sin(CHARA_HEIGHT / 2));
			numberStep++;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			plyer_pos.x -= velocity;
			numberStep++;
			
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			plyer_pos.x += velocity;
			numberStep++;
			
		}

		else if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			plyer_pos.y -= velocity;
			numberStep++;
			
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			plyer_pos.y += velocity;
			numberStep++;
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
void Player::player_draw(const KonCamera& camera ,float scale)
{
	tnl::Vector3 draw_pos = plyer_pos - camera.getTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), scale, 0, anim_hdls[anim_ctrl_dir][anim_ctrl_frame], true);

}

//�v���C���[�̍s������
void Player::PlayerMoveProcess(float delta_time , Shared<BattleLog>& basttle_log , Shared<Enemy>& enemy , Shared<Nomal_Attack>& nomal_attack)
{
	switch (player_state)
	{
	
	//�ʏ펞
	case Player::PlayerState::IDLE:

		//�v���C���[�̃R�}���h����
		battle_scene_->MenuUpdate(plyerstatusSave, enemy->GetEnemyArray()[enemy->GetEnemy_Index()]);

		break;

	//�ʏ�U����
	case Player::PlayerState::NOMALATTACK:

		//�ʏ�U�����̏���
		PlayerAttackProcess(enemy->GetEnemyArray()[enemy->GetEnemy_Index()], basttle_log , nomal_attack);

		break;


	//���Z�̍U����
	case Player::PlayerState::SKILL:

		//���Z�g�p���̏���
		battle_scene_->SkillUseProcess(plyerstatusSave , enemy->GetEnemyArray()[enemy->GetEnemy_Index()]);

		break;

	//������
	case Player::PlayerState::FLEE:

		//������ۂ̏���
		battle_scene_->FleeProcess(plyerstatusSave, enemy->GetEnemyArray()[enemy->GetEnemy_Index()], delta_time);

		break;

	//���S��
	case Player::PlayerState::DEAD:

		//�v���C���[�̎��S����
		DeadPlayerProcess(basttle_log);

		break;


	default:
		break;
	}
}

//csv�ŃX�e�[�^�X��ǂݍ���
void Player::PlyStatusLoad()
{
	int max_num = static_cast<int>(PlyerStatus_Csv_Info.size());

	Ply_Status_Total_num = max_num;

	for (int y = 1; y < max_num; y++) {

		//csv����level�̎擾
		plyerstatusSave.SetPlayerLevel(PlyerStatus_Csv_Info[y][0].getInt());
		//csv����maxHp�̎擾
		plyerstatusSave.SetPlayerMaxHp(PlyerStatus_Csv_Info[y][1].getFloat());
		//csv����hp�̎擾
		plyerstatusSave.SetPlayerCurentHp(PlyerStatus_Csv_Info[y][1].getFloat());
		//csv����Attack�̎擾
		plyerstatusSave.SetPlayerAttack(PlyerStatus_Csv_Info[y][2].getInt());
		//csv����Defance�̎擾
		plyerstatusSave.SetPlayerDefance(PlyerStatus_Csv_Info[y][3].getInt());
		//csv����Speed�̎擾
		plyerstatusSave.SetPlayerSpeed(PlyerStatus_Csv_Info[y][4].getInt());
		//csv����K�v�Ȍo���l���������̎擾
		plyerstatusSave.SetPlayerExpoint(PlyerStatus_Csv_Info[y][5].getInt());
		//csv����ő�Mp���擾
		plyerstatusSave.SetPlayerCurentMp(PlyerStatus_Csv_Info[y][6].getFloat());
		//csv����Mp���擾
		plyerstatusSave.SetPlayerMaxMp(PlyerStatus_Csv_Info[y][6].getFloat());
		//csv����magicPower�̎擾
		plyerstatusSave.SetMagicPower(PlyerStatus_Csv_Info[y][7].getInt());

		Ply_Status_Type.emplace_back(plyerstatusSave);

	}	

}

//�v���C���[�̉摜��ǂݍ���
//ID�ɂ���ĉ摜��؂�ւ���
//ID : 2�����z��̉��s�ڂ������Ă��邩
void Player::SetPlayerAnimationHdl(int ghdl_id)
{
	//�A�j���[�V�����摜���i�[�z��
	std::vector<std::string> player_image;

	auto playerAnimationSize = CsvManager::getCsvManager()->GetPlayerAnimationImage().size();

	for (int i = 1; i < playerAnimationSize; i++) {
		if (ghdl_id == std::stoi(CsvManager::getCsvManager()->GetPlayerAnimationImage()[i][0].c_str())) {
			for (int k = 1; k < DIR_MAX + 1 ; k++) {
				player_image.emplace_back(CsvManager::getCsvManager()->GetPlayerAnimationImage()[i][k]);
			}
		}
	}

	for (int i = 0; i < DIR_MAX; i++) {

		LoadDivGraph(
			player_image[i].c_str(),						// �摜�̃p�X
			Total_Frame,									// ���t���[����
			Horizontal_frame,								// ���t���[����
			Vertical_frame,									// �c�t���[����
			Horizontal_oneframe,							// �������� 1�t���[��������̕�
			Vertical_oneframe,								// �c������ 1�t���[��������̕�
			anim_hdls[i]);									// �摜�n���h�����i�[����z��);
	}
}

Player::PlayerStatus Player::GetPlyerStatus(int level) const
{
	auto it = std::find_if(Ply_Status_Type.begin(), Ply_Status_Type.end(), [level]
	(const PlayerStatus& status) {return status.GetLevel() == level; });

	if (it != Ply_Status_Type.end()) {
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
	if (plyerstatusSave.GetcurentHp() <= 0) {

		//���O�𗬂�
		battle_log->addLog("�S�ł���");

		//�v���C���[��Hp��0�ɂ���
		plyerstatusSave.SetPlayerCurentHp(0);

		//BGM���~�߂�
		SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/zenmetu.mp3", DX_PLAYTYPE_BACK);

		//�o�g�����I��������
		battle_scene_->SetBattleState(BattleScene::BattleState::IDLE);
	}
	else {
		return;
	}
}

//�v���C���[�̒ʏ�U������
void Player::PlayerAttackProcess(Enemy::EnemyStatus& enemy_status, Shared<BattleLog>& battle_log , Shared<Nomal_Attack>& nomal_attack)
{
	//�G������ŋ����珈�����Ƃ΂�
	if (enemy_status.GetEnemyHp() <= 0)return;

	//���艹��炷
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

	//�G�Ƀ_���[�W��^����
	nomal_attack->SkillUse(plyerstatusSave, enemy_status ,battle_log);

}

//�X�L�����Z�b�g����
void Player::SkillSet(Shared<BattleLog>& battle_log)
{
	switch (plyerstatusSave.GetLevel())
	{
		//���x��1�̎��͉����o���Ȃ�
	case 1:
		break;
		//���x��2�̎��ɉΉ��a����o����
	case 2:
		AddSkill(std::make_shared<FlameSlash>());

		//���O�𗬂�
		battle_log->addLog("�Ή��a����o����!");

		break;
		//���x��3�̎��Ƀq�[�����o����
	case 3:
		
		AddSkill(std::make_shared<Heal>());

		battle_log->addLog("�q�[�����o����!");

		break;
		//���x��4�̎��ɃA�C�X�u���X�g���o����
	case 4:
		AddSkill(std::make_shared<IceBlast>());

		battle_log->addLog("�A�C�X�u���X�g���o����!");

		break;
		//���x��5�̎��ɃT���_�[�{���g���o����
	case 5:
		AddSkill(std::make_shared<ThunderBolt>());

		battle_log->addLog("�T���_�[�{���g���o����!");

	default:
		break;
	}
}

//�X�L����ǉ�����
void Player::AddSkill(Shared<Skill> skill)
{
	SkillList.emplace_back(skill);
	SkillNum++;
}

//void Player::PlyerSave()
//{
//	FILE* fp = nullptr;
//	fopen_s(&fp, "player.bin", "wb");
//
//		
//	if (fp) {
//		fwrite(&plyerstatusSave.hp, sizeof(int), 1, fp);
//		fwrite(&plyerstatusSave.Attack, sizeof(int), 1, fp);
//		fwrite(&plyerstatusSave.Defance, sizeof(int), 1, fp);
//		fwrite(&plyerstatusSave.Speed, sizeof(int), 1, fp);
//		fwrite(&plyerstatusSave.level, sizeof(int), 1, fp);
//		fwrite(&plyerstatusSave.R_expoint, sizeof(int), 1, fp);
//		fwrite(&plyer_pos, sizeof(tnl::Vector3), 1, fp);
//
//		fclose(fp);
//	}
//
//	
//}
//
//void Player::PlyerLoad()
//{
//	FILE* fp = nullptr;
//	fopen_s(&fp, "player.bin", "rb");
//
//
//		if (fp) {
//			fread(&plyerstatusSave.hp, sizeof(int), 1, fp);
//			fread(&plyerstatusSave.Attack, sizeof(int), 1, fp);
//			fread(&plyerstatusSave.Defance, sizeof(int), 1, fp);
//			fread(&plyerstatusSave.Speed, sizeof(int), 1, fp);
//			fread(&plyerstatusSave.level, sizeof(int), 1, fp);
//			fread(&plyerstatusSave.R_expoint, sizeof(int), 1, fp);
//			fread(&plyer_pos, sizeof(tnl::Vector3), 1, fp);
//
//			fclose(fp);
//		}
//
//}



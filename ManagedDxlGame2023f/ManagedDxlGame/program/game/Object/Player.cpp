#include "Player.h"
#include"../Manager/Camera.h"


Player::Player() : money(1000)
{
	name = "Player";
	
	//csv�̓ǂݎ��
	PlyerStatus_Csv_Info
		= tnl::LoadCsv<tnl::CsvCell>("csv/PlyerStatus.csv");


	//�A�j���[�V�����̃��[�h����
	std::string files[DIR_MAX] = {
		"graphics/Player/pipo-charachip_Idle.png",
		"graphics/Player/pipo-charachip_UP.png",
		"graphics/Player/pipo-charachip_Left.png",
		"graphics/Player/pipo-charachip_Right.png"
		
	};

	for (int i = 0; i < 4; i++) {
		LoadDivGraph(
			files[i].c_str(),				// �摜�̃p�X
			Total_Frame,					// ���t���[����
			Horizontal_frame,				// ���t���[����
			Vertical_frame,					// �c�t���[����
			Horizontal_oneframe,			// �������� 1�t���[��������̕�
			Vertical_oneframe,				// �c������ 1�t���[��������̕�
			anim_hdls[i]);					// �摜�n���h�����i�[����z��);
	}

	//plyer�̃X�e�[�^�X���擾����
	PlyStatusLoad();

	//�X�e�[�^�X��ݒ肷��(Lv1����X�^�[�g)
	SetPlayerStatus(1);

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
		if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			plyer_pos.x -= velocity;
			numberStep++;
			
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			plyer_pos.x += velocity;
			numberStep++;
			
		}

		if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			plyer_pos.y -= velocity;
			numberStep++;
			
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			plyer_pos.y += velocity;
			numberStep++;
		}


		//�A�j���[�V�����̓���
		if (tnl::Input::IsKeyDown(eKeys::KB_W))    anim_ctrl_dir = DIR_UP;
		if (tnl::Input::IsKeyDown(eKeys::KB_S))    anim_ctrl_dir = DIR_DOWN;
		if (tnl::Input::IsKeyDown(eKeys::KB_A))	   anim_ctrl_dir = DIR_LEFT;
		if (tnl::Input::IsKeyDown(eKeys::KB_D))    anim_ctrl_dir = DIR_RIGHT;

	}
}

//�`��
void Player::player_draw(const KonCamera& camera ,float scale)
{
	tnl::Vector3 draw_pos = plyer_pos - camera.getTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(draw_pos.x, draw_pos.y, scale, 0, anim_hdls[anim_ctrl_dir][anim_ctrl_frame], true);

}

//csv�ŃX�e�[�^�X��ǂݍ���
void Player::PlyStatusLoad()
{
	int max_num = PlyerStatus_Csv_Info.size();

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

//�v���C���[�̃R���g���[�����Ǘ�����
void Player::setPlayerControl(int controlFlag)
{
	//0�̏ꍇtrue�ɂ���
	if (controlFlag == 0) {
		plyControl = true;
	}
	//1�̏ꍇfalse�ɂ���
	else if (controlFlag == 1) {
		plyControl = false;
	}
}

//�v���C���[�̃T�C�Y���擾����
int Player::getPlayerSize(int type)
{
	if (type == Width) {
		return CHARA_WIDTH;
	}
	if (type == Height) {
		return CHARA_HEIGHT;
	}

}

Player::PlayerStatus Player::GetPlyerStatus(int level) const
{
	auto it = std::find_if(Ply_Status_Type.begin(), Ply_Status_Type.end(), [level]
	(const PlayerStatus& status) {return status.getLevel() == level; });

	if (it != Ply_Status_Type.end()) {
		return *it;
	}
	else {
		// ID�ɑΉ�����A�C�e����������Ȃ��ꍇ�́A�K���ȃf�t�H���g�l��Ԃ����G���[�������s���Ȃ�
		return PlayerStatus{};
	}
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



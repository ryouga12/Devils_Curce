#include "Player.h"
#include"../Manager/Camera.h"


Player::Player() : money(1000)
{
	name = "Player";
	
	//csvの読み取り
	PlyerStatus_Csv_Info
		= tnl::LoadCsv<tnl::CsvCell>("csv/PlyerStatus.csv");


	//アニメーションのロード処理
	std::string files[DIR_MAX] = {
		"graphics/Player/pipo-charachip_Idle.png",
		"graphics/Player/pipo-charachip_UP.png",
		"graphics/Player/pipo-charachip_Left.png",
		"graphics/Player/pipo-charachip_Right.png"
		
	};

	for (int i = 0; i < 4; i++) {
		LoadDivGraph(
			files[i].c_str(),				// 画像のパス
			Total_Frame,					// 総フレーム数
			Horizontal_frame,				// 横フレーム数
			Vertical_frame,					// 縦フレーム数
			Horizontal_oneframe,			// 横方向へ 1フレームあたりの幅
			Vertical_oneframe,				// 縦方向へ 1フレームあたりの幅
			anim_hdls[i]);					// 画像ハンドルを格納する配列名);
	}

	//plyerのステータスを取得する
	PlyStatusLoad();

	//ステータスを設定する(Lv1からスタート)
	SetPlayerStatus(1);

}

Player::~Player()
{

	//プレイヤーの画像解放
	for (int i = 0; i < static_cast<int>(DIR_MAX); i++) {
		for (int k = 0; k < 5; k++) {
			DeleteGraph(anim_hdls[i][k]);
			// 解放後のポインタの値をデバッグトレースで確認
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

	//アニメーション
	anim_time_count += delta_time;
	if (anim_time_count > ChangeAnimationTime) {
		anim_ctrl_frame++;
		anim_ctrl_frame %= animationFlame;

		anim_time_count = 0;
	}

	if (plyControl) {

		//プレイヤーの動き
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


		//アニメーションの動き
		if (tnl::Input::IsKeyDown(eKeys::KB_W))    anim_ctrl_dir = DIR_UP;
		if (tnl::Input::IsKeyDown(eKeys::KB_S))    anim_ctrl_dir = DIR_DOWN;
		if (tnl::Input::IsKeyDown(eKeys::KB_A))	   anim_ctrl_dir = DIR_LEFT;
		if (tnl::Input::IsKeyDown(eKeys::KB_D))    anim_ctrl_dir = DIR_RIGHT;

	}
}

//描画
void Player::player_draw(const KonCamera& camera ,float scale)
{
	tnl::Vector3 draw_pos = plyer_pos - camera.getTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(draw_pos.x, draw_pos.y, scale, 0, anim_hdls[anim_ctrl_dir][anim_ctrl_frame], true);

}

//csvでステータスを読み込む
void Player::PlyStatusLoad()
{
	int max_num = PlyerStatus_Csv_Info.size();

	Ply_Status_Total_num = max_num;

	for (int y = 1; y < max_num; y++) {

		//csvからlevelの取得
		plyerstatusSave.SetPlayerLevel(PlyerStatus_Csv_Info[y][0].getInt());
		//csvからmaxHpの取得
		plyerstatusSave.SetPlayerMaxHp(PlyerStatus_Csv_Info[y][1].getFloat());
		//csvからhpの取得
		plyerstatusSave.SetPlayerCurentHp(PlyerStatus_Csv_Info[y][1].getFloat());
		//csvからAttackの取得
		plyerstatusSave.SetPlayerAttack(PlyerStatus_Csv_Info[y][2].getInt());
		//csvからDefanceの取得
		plyerstatusSave.SetPlayerDefance(PlyerStatus_Csv_Info[y][3].getInt());
		//csvからSpeedの取得
		plyerstatusSave.SetPlayerSpeed(PlyerStatus_Csv_Info[y][4].getInt());
		//csvから必要な経験値がいくつかの取得
		plyerstatusSave.SetPlayerExpoint(PlyerStatus_Csv_Info[y][5].getInt());
		//csvから最大Mpを取得
		plyerstatusSave.SetPlayerCurentMp(PlyerStatus_Csv_Info[y][6].getFloat());
		//csvからMpを取得
		plyerstatusSave.SetPlayerMaxMp(PlyerStatus_Csv_Info[y][6].getFloat());
		//csvからmagicPowerの取得
		plyerstatusSave.SetMagicPower(PlyerStatus_Csv_Info[y][7].getInt());

		Ply_Status_Type.emplace_back(plyerstatusSave);

	}	

}

//プレイヤーのコントロールを管理する
void Player::setPlayerControl(int controlFlag)
{
	//0の場合trueにする
	if (controlFlag == 0) {
		plyControl = true;
	}
	//1の場合falseにする
	else if (controlFlag == 1) {
		plyControl = false;
	}
}

//プレイヤーのサイズを取得する
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
		// IDに対応するアイテムが見つからない場合は、適当なデフォルト値を返すかエラー処理を行うなど
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



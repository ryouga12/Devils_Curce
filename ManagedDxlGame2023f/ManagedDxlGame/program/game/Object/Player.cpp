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


Player::Player() : money(1000)
{
	name = "Player";
	
	//csvの読み取り
	PlyerStatus_Csv_Info
		= CsvManager::getCsvManager()->GetPlayerStatusCsv();

	//plyerのステータスを取得する
	PlyStatusLoad();

	//ステータスを設定する(Lv1からスタート)
	SetPlayerStatus(7);

	//始まりは何も装備していない為0からスタート
	SetPlayerAnimationHdl(0);

	//デバック
	SaveReadSkill(1);
	SaveReadSkill(2);
	SaveReadSkill(3);

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

//プレイヤーの動き
//引数 : Delta_time , 動く速度　,　マップの高さ(移動制限の為)
void Player::player_Move(float delta_time , const float& velocity , const int MAPHEIGHT)
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

		//左上斜め
		if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) {

			plyer_pos.x += velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y += velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;

		}
		//右上斜め
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) {

			plyer_pos.x -= velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y += velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;

		}
		//左下斜め
		else if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) {	

			plyer_pos.x += velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y -= velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;
		}
		//右下斜め
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) {

			plyer_pos.x -= velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y -= velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
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

		//アニメーションの動き
		
		//左上斜め
		if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_W)))	   { anim_ctrl_dir = DIR_DIAGONAL_UPPER_LEFT; }
		//右上斜め
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) { anim_ctrl_dir = DIR_DIAGONAL_UPPER_RIGTH; }
		//左下斜め
		else if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) { anim_ctrl_dir = DIR_DIAGONAL_DOWN_LEFT; }
		//右下斜め
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) { anim_ctrl_dir = DIR_DIAGONA_DOWN_RIGTH; }
		//上
		else if (tnl::Input::IsKeyDown(eKeys::KB_W))										   { anim_ctrl_dir = DIR_UP; }
		//下
		else if (tnl::Input::IsKeyDown(eKeys::KB_S))										   { anim_ctrl_dir = DIR_DOWN; }
		//左
		else if (tnl::Input::IsKeyDown(eKeys::KB_A))										   { anim_ctrl_dir = DIR_LEFT; }
		//右
		else if (tnl::Input::IsKeyDown(eKeys::KB_D))										   { anim_ctrl_dir = DIR_RIGHT; }

	}
}

//描画
void Player::player_draw(const KonCamera& camera ,float scale)
{
	tnl::Vector3 draw_pos = plyer_pos - camera.getTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
	DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), scale, 0, anim_hdls[anim_ctrl_dir][anim_ctrl_frame], true);

}

//プレイヤーの行動処理
void Player::PlayerMoveProcess(float delta_time , Shared<BattleLog>& basttle_log , Shared<Enemy>& enemy , Shared<Nomal_Attack>& nomal_attack)
{
	switch (player_state)
	{
	
	//通常時
	case Player::PlayerState::IDLE:

		//プレイヤーのコマンド処理
		battle_scene_->MenuUpdate(plyerstatusSave, enemy->GetEnemyArray()[enemy->GetEnemy_Index()]);

		break;

	//通常攻撃時
	case Player::PlayerState::NOMALATTACK:

		//通常攻撃時の処理
		PlayerAttackProcess(enemy->GetEnemyArray()[enemy->GetEnemy_Index()], basttle_log , nomal_attack);

		break;


	//特技の攻撃時
	case Player::PlayerState::SKILL:

		//特技使用時の処理
		battle_scene_->SkillUseProcess(plyerstatusSave , enemy->GetEnemyArray()[enemy->GetEnemy_Index()]);

		break;

	//逃走時
	case Player::PlayerState::FLEE:

		//逃げる際の処理
		battle_scene_->FleeProcess(plyerstatusSave, enemy->GetEnemyArray()[enemy->GetEnemy_Index()], delta_time);

		break;

	//死亡時
	case Player::PlayerState::DEAD:

		//プレイヤーの死亡処理
		DeadPlayerProcess(basttle_log);

		break;


	default:
		break;
	}
}

//csvでステータスを読み込む
void Player::PlyStatusLoad()
{
	int max_num = static_cast<int>(PlyerStatus_Csv_Info.size());

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
		//csvから必要な経験値の取得
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

//プレイヤーの画像を読み込む
//IDによって画像を切り替える
//ID : 2次元配列の何行目をさしているか
void Player::SetPlayerAnimationHdl(int ghdl_id)
{
	//アニメーション画像を格納配列
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
			player_image[i].c_str(),						// 画像のパス
			Total_Frame,									// 総フレーム数
			Horizontal_frame,								// 横フレーム数
			Vertical_frame,									// 縦フレーム数
			Horizontal_oneframe,							// 横方向へ 1フレームあたりの幅
			Vertical_oneframe,								// 縦方向へ 1フレームあたりの幅
			anim_hdls[i]);									// 画像ハンドルを格納する配列名);
	}
}

//セーブする際のスキルの追加
void Player::SaveReadSkill(int skill_id )
{
	// スキルリストを取得
	auto& skill_list = GameManager::getGameManager()->getSkill()->GetOverallSkills();

	// skill_id に一致するスキルを検索
	auto it = std::find_if(skill_list.begin(), skill_list.end(),
		[skill_id](const Shared<Skill>& skill) { return skill->getId() == skill_id; });

	// スキルが見つかった場合
	if (it != skill_list.end()) {
		player_skill_list.emplace_back(*it);
	}
	// スキルが見つからなかった場合
	else {
		tnl::DebugTrace("----------------------------------------------------------------\n");
		tnl::DebugTrace("スキルを追加できませんでした\n");
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
		// IDに対応するアイテムが見つからない場合は、適当なデフォルト値を返すかエラー処理を行うなど
		return PlayerStatus{};
	}
}

//プレイヤーの死亡処理
void Player::DeadPlayerProcess(Shared<BattleLog>& battle_log)
{
	//プレイヤーのHPが0を下回ったら
	if (plyerstatusSave.GetcurentHp() <= 0) {

		//ログを流す
		battle_log->addLog("全滅した");

		//プレイヤーのHpを0にする
		plyerstatusSave.SetPlayerCurentHp(0);

		//BGMを止める
		SoundManager::getSoundManager()->StopSound("sound/BGM/sentou.mp3");
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/zenmetu.mp3", DX_PLAYTYPE_BACK);

		//バトルを終了させる
		battle_scene_->SetBattleState(BattleScene::BattleState::IDLE);
	}
	else {
		return;
	}
}

//プレイヤーの通常攻撃処理
void Player::PlayerAttackProcess(Enemy::EnemyConnection& enemy_status, Shared<BattleLog>& battle_log , Shared<Nomal_Attack>& nomal_attack)
{
	//敵が死んで居たら処理をとばす
	if (enemy_status.GetEnemyHp() <= 0)return;

	//決定音を鳴らす
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

	//敵にダメージを与える
	nomal_attack->SkillUse(plyerstatusSave, enemy_status ,battle_log);

}

//スキルをセットする
void Player::SkillSet(Shared<BattleLog> battle_log)
{
	switch (plyerstatusSave.GetLevel())
	{
		//レベル1の時は何も覚えない
	case 1:
		break;

		//レベル2の時に火炎斬りを覚える
	case 2:
		AddSkill(std::make_shared<FlameSlash>());

		if (battle_log) {
			//ログを流す
			battle_log->addLog("火炎斬りを覚えた!");
		}

		break;
		//レベル3の時にヒールを覚える
	case 3:
		
		AddSkill(std::make_shared<Heal>());

		if (battle_log) {
			battle_log->addLog("ヒールを覚えた!");
		}

		break;
		//レベル4の時にアイスブラストを覚える
	case 4:

		AddSkill(std::make_shared<IceBlast>());

		if (battle_log) {
			battle_log->addLog("アイスブラストを覚えた!");
		}

		break;
		//レベル5の時にサンダーボルトを覚える
	case 5:
		AddSkill(std::make_shared<ThunderBolt>());

		if (battle_log) {
			battle_log->addLog("サンダーボルトを覚えた!");
		}

		break;
	}
}

//スキルを追加する
void Player::AddSkill(const Shared<Skill>& skill)
{
	player_skill_list.emplace_back(skill);
	SkillNum++;
}

//セーブ
void Player::PlyerSave()
{
	FILE* fp = nullptr;
	fopen_s(&fp, "player.bin", "wb");

	if (fp) {

		// Playerステータスを取得する
		
		//hp
		float player_hp = plyerstatusSave.GetcurentHp();

		//max_hp
		float player_max_hp = plyerstatusSave.GetMaxHp();

		//mp
		float player_mp = plyerstatusSave.GetCurentMp();

		//max_mp
		float player_max_mp = plyerstatusSave.GetMaxMp();

		//魔法力
		int player_magic_power = plyerstatusSave.GetMagicPower();

		//攻撃力
		int player_attack;

		//もし武器が装備されていたら
		if (GameManager::getGameManager()->getInventory()->GetEquipAttack() != 0) {
			//現在の攻撃力から装備武器の攻撃力を引いてから
			//攻撃力を代入する
			player_attack = plyerstatusSave.GetAttack() - GameManager::getGameManager()->getInventory()->GetEquipAttack();
		}
		//装備されていなかったら
		else {
			player_attack = plyerstatusSave.GetAttack();
		}

		//防御力
		int player_defense;

		//もし防具が装備されていたら
		if (GameManager::getGameManager()->getInventory()->GetEquipDefance() != 0) {
			//現在の防御力から装備防具の防御力を引いてから
			plyerstatusSave.SetPlayerDefance(plyerstatusSave.GetDefance() - GameManager::getGameManager()->getInventory()->GetEquipDefance());
			//防御力を代入する
			player_defense = plyerstatusSave.GetDefance();
		}
		//装備されていなかったら
		else {
			//プレイヤーの防御力を取得する
			player_defense = plyerstatusSave.GetDefance();
		}

		//素早さ
		int player_speed = plyerstatusSave.GetSpeed();

		//レベル
		int player_level = plyerstatusSave.GetLevel();

		//必要な経験値
		int player_expoint = plyerstatusSave.GetExpoint();

		//当たり判定の関係で座標を少し調整
		tnl::Vector3 add_pos = { 0, 20 , 0 };

		//現在位置
		tnl::Vector3 player_pos = plyer_pos + add_pos;
		
		//インベントリのサイズ分回して配列に格納する
		const auto& player_list = GameManager::getGameManager()->getInventory()->GetInventoryList();
		std::vector<int> player_item_list;

		//インベントリにアイテムがあれば
		if (!player_list.empty()) {
			for (int i = 0; i < player_list.size(); i++) {
				player_item_list.emplace_back(GameManager::getGameManager()->getInventory()->GetInventoryList()[i].getItemId());
			}
		}

		//スキルリストのサイズ分回して配列に格納する
		std::vector<int> player_skill_list_array;

		//スキルリストにスキルがあれば
		if (!player_skill_list.empty()) {

			for (int i = 0; i < player_skill_list.size(); i++) {
				player_skill_list_array.emplace_back(player_skill_list[i]->getId());
			}
		}
	

		//プレイヤーのステータスを保存する

		//hp
		fwrite(&player_hp, sizeof(float), 1, fp);
		//max_hp
		fwrite(&player_max_hp, sizeof(float), 1, fp);
		//mp
		fwrite(&player_mp, sizeof(float), 1, fp);
		//max_mp 
		fwrite(&player_max_mp, sizeof(float), 1, fp);
		//魔法力
		fwrite(&player_magic_power, sizeof(float), 1, fp);
		//攻撃力
		fwrite(&player_attack, sizeof(int), 1, fp);
		//防御力
		fwrite(&player_defense, sizeof(int), 1, fp);
		//素早さ
		fwrite(&player_speed, sizeof(int), 1, fp);
		//レベル
		fwrite(&player_level, sizeof(int), 1, fp);
		//必要な経験値
		fwrite(&player_expoint, sizeof(int), 1, fp);
		//現在座標
		fwrite(&player_pos, sizeof(tnl::Vector3), 1, fp);
		//プレイヤーが記憶しておくマップ状態
		fwrite(&player_curent_map_memory, sizeof(int), 1, fp);
		//所持金
		fwrite(&money, sizeof(int), 1, fp);


		//インベントリの保存
		// サイズを格納する
		size_t vector_size = player_item_list.size();
		fwrite(&vector_size, sizeof(size_t), 1, fp);

		// 要素を保存する
		fwrite(player_item_list.data(), sizeof(int), vector_size, fp);

		//スキルリストの保存
		//サイズを格納する
		size_t skill_size = player_skill_list_array.size();
		fwrite(&skill_size, sizeof(size_t), 1, fp);

		//スキルを保存する
		fwrite(player_skill_list_array.data(), sizeof(int), skill_size, fp);

		fclose(fp);
	}
	
}

//ロード
void Player::PlyerLoad()
{
	FILE* fp = nullptr;
	fopen_s(&fp, "player.bin", "rb");

	if (fp) {

		// 一時的に保存しておくステータス
		float player_hp;
		float player_max_hp;
		float player_mp;
		float player_max_mp;
		int player_magic_power;
		int player_attack;
		int player_defense;
		int player_speed;
		int player_level;
		int player_expoint;
		tnl::Vector3 player_pos;
		std::vector<int> player_inventory_list;
		std::vector<int> playe_skill_list;
		std::vector<int> playe_weapon_list;

		// ステータスをロードする

		//hp
		fread(&player_hp, sizeof(float), 1, fp);
		//max_hp
		fread(&player_max_hp, sizeof(float), 1, fp);
		//mp
		fread(&player_mp, sizeof(float), 1, fp);
		//max_mp
		fread(&player_max_mp, sizeof(float), 1, fp);
		//魔法力
		fread(&player_magic_power, sizeof(float), 1, fp);
		//攻撃力
		fread(&player_attack, sizeof(int), 1, fp);
		//防御力
		fread(&player_defense, sizeof(int), 1, fp);
		//素早さ
		fread(&player_speed, sizeof(int), 1, fp);
		//レベル
		fread(&player_level, sizeof(int), 1, fp);
		//必要な経験値
		fread(&player_expoint, sizeof(int), 1, fp);
		//現在座標
		fread(&player_pos, sizeof(tnl::Vector3), 1, fp);
		//プレイヤーが記憶しているマップ状態
		fread(&player_curent_map_memory, sizeof(int), 1, fp);
		//所持金
		fread(&money, sizeof(int), 1, fp);

		//インベントリのロード
		// 配列のサイズをロードする
		size_t vector_size;
		fread(&vector_size, sizeof(size_t), 1, fp);

		// 要素を保持するために配列のサイズを変更
		player_inventory_list.resize(vector_size);

		//要素を配列に保存する
		fread(player_inventory_list.data(), sizeof(int), vector_size, fp);

		//スキルリストのロード
		// 配列のサイズをロードする
		size_t skill_size;
		fread(&skill_size, sizeof(size_t), 1, fp);

		// 要素を保持するために配列のサイズを変更
		playe_skill_list.resize(skill_size);

		//要素を配列に保存する
		fread(playe_skill_list.data(), sizeof(int), skill_size, fp);


		// プレイヤーのステータスをセットする
		plyerstatusSave.SetPlayerCurentHp(player_hp);
		plyerstatusSave.SetPlayerMaxHp(player_max_hp);
		plyerstatusSave.SetPlayerCurentMp(player_mp);
		plyerstatusSave.SetPlayerMaxMp(player_max_mp);
		plyerstatusSave.SetMagicPower(player_magic_power);
		plyerstatusSave.SetPlayerAttack(player_attack);
		plyerstatusSave.SetPlayerDefance(player_defense);
		plyerstatusSave.SetPlayerSpeed(player_speed);
		plyerstatusSave.SetPlayerLevel(player_level);
		plyerstatusSave.SetPlayerExpoint(player_expoint);
		plyer_pos = player_pos;

		
		// インベントリの要素を格納する
		for (int  item : player_inventory_list) {
			GameManager::getGameManager()->getInventory()->AddInventory(item);
		}

		//スキルリストの要素を格納する
		for (int skill : playe_skill_list) {
			SaveReadSkill(skill);
		}

		fclose(fp);
	}
}



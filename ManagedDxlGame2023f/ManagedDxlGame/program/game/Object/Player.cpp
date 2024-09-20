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
	//csvの読み取り
	plyer_status_csv_info
		= CsvManager::GetCsvManager()->GetPlayerStatusCsv();

	//plyerのステータスを取得する
	PlyStatusLoad();

	//ステータスを設定する(Lv1からスタート)
	SetPlayerStatus(1);

	//スキルのポインタを生成する
	skill = std::make_shared<Skill>();
	skill->AddSkillList();

}

Player::~Player()
{

	//プレイヤーの画像解放
	for (int i = 0; i < DIR_MAX; i++) {
		for (int k = 0; k < 5; k++) {
			DeleteGraph(anim_hdls[i][k]);
			// 解放後のポインタの値をデバッグトレースで確認
			tnl::DebugTrace("\n-----------------------------------------------------------------------------\n");
			tnl::DebugTrace("\nDeleted Playerimage at [%d][%d]. Pointer value: %p\n", i, k, anim_hdls[i][k]);
		}
	}
}

//プレイヤーの動き
//引数 : Delta_time , 動く速度　,　マップの高さ(移動制限の為)
void Player::Player_Move(const float& delta_time , const float& velocity , const int& MAPHEIGHT)
{
	//prev_pos = plyer_pos;

	if (play_control) {

		//プレイヤーの動き

		//左上斜め
		if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) {

			plyer_pos.x += velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y += velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;
			prev_pos = plyer_pos;

		}
		//右上斜め
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_W))) {

			plyer_pos.x -= velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y += velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;
			prev_pos = plyer_pos;

		}
		//左下斜め
		else if ((tnl::Input::IsKeyDown(eKeys::KB_A)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) {	

			plyer_pos.x += velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y -= velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;
			prev_pos = plyer_pos;
		}
		//右下斜め
		else if ((tnl::Input::IsKeyDown(eKeys::KB_D)) && (tnl::Input::IsKeyDown(eKeys::KB_S))) {

			plyer_pos.x -= velocity * static_cast<float>(std::sin(CHARASIZE.x / 2));
			plyer_pos.y -= velocity * static_cast<float>(std::sin(CHARASIZE.y / 2));
			numberStep++;
			prev_pos = plyer_pos;
		}
		//左
		else if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			plyer_pos.x -= velocity;
			numberStep++;
			prev_pos = plyer_pos;
			
		}
		//右
		else if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			plyer_pos.x += velocity;
			numberStep++;
			prev_pos = plyer_pos;
			
		}
		//上
		else if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			plyer_pos.y -= velocity;
			numberStep++;
			prev_pos = plyer_pos;
			
		}
		//下
		else if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			plyer_pos.y += velocity;
			numberStep++;
			prev_pos = plyer_pos;
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
void Player::Player_draw(const KonCamera& camera ,const float& scale)
{
	if (player_display_flag) {
		tnl::Vector3 draw_pos = plyer_pos - camera.GetTargetPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
		DrawRotaGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), scale, 0, anim_hdls[anim_ctrl_dir][anim_ctrl_frame], true);
	}
}

//プレイヤーのアニメーション処理
void Player::PlayerAnimation(const float& delta_time)
{
	//アニメーション
	anim_time_count += delta_time;
	if (anim_time_count > ChangeAnimationTime) {
		anim_ctrl_frame++;
		anim_ctrl_frame %= animationFlame;

		anim_time_count = 0;
	}
}

//プレイヤーの行動処理
void Player::PlayerMoveProcess(float delta_time , Shared<BattleLog>& basttle_log , Shared<Enemy>& enemy , Shared<Nomal_Attack>& nomal_attack)
{
	switch (player_state)
	{
	
	//通常時
	case Player::PlayerState::CHOICE:

		//プレイヤーのコマンド処理
		battle_scene_->MenuUpdate(player_status_save, enemy->GetEnemyArray()[enemy->GetEnemy_Index()]);

		break;

	//通常攻撃時
	case Player::PlayerState::NOMALATTACK:

		//通常攻撃時の処理
		PlayerAttackProcess(enemy->GetEnemyArray()[enemy->GetEnemy_Index()], basttle_log , nomal_attack);

		break;


	//特技の攻撃時
	case Player::PlayerState::SKILL:

		//特技使用時の処理
		battle_scene_->SkillUseProcess(player_status_save , enemy->GetEnemyArray()[enemy->GetEnemy_Index()]);

		break;

	//逃走時
	case Player::PlayerState::FLEE:

		//逃げる際の処理
		battle_scene_->FleeProcess(player_status_save, enemy->GetEnemyArray()[enemy->GetEnemy_Index()], delta_time);

		break;

	//死亡時
	case Player::PlayerState::DEAD:

		//プレイヤーの死亡処理
		DeadPlayerProcess(basttle_log);

		break;

	//待機状態
	case Player::PlayerState::IDLE:


		break;

	default:

		tnl::DebugTrace("\n------------------------------------------------------------");
		tnl::DebugTrace("\n うまく選択できませんでした");
		tnl::DebugTrace("\n------------------------------------------------------------");

		break;
	}
}

//csvでステータスを読み込む
void Player::PlyStatusLoad()
{
	int max_num = static_cast<int>(plyer_status_csv_info.size());

	player_status_total_num = max_num;

	for (int y = 1; y < max_num; y++) {

		//csvからlevelの取得
		player_status_save.SetPlayerLevel(plyer_status_csv_info[y][0].getInt());
		//csvからmaxHpの取得
		player_status_save.SetPlayerMaxHp(plyer_status_csv_info[y][1].getInt());
		//csvからhpの取得
		player_status_save.SetPlayerCurentHp(plyer_status_csv_info[y][1].getInt());
		//csvからAttackの取得
		player_status_save.SetPlayerAttack(plyer_status_csv_info[y][2].getInt());
		//csvからbase_attackの取得
		player_status_save.SetPlayerBaseAttack(plyer_status_csv_info[y][2].getInt());
		//csvからbase_defanceの取得
		player_status_save.SetBaseDefance(plyer_status_csv_info[y][3].getInt());
		//csvからDefanceの取得
		player_status_save.SetPlayerDefance(plyer_status_csv_info[y][3].getInt());
		//csvからSpeedの取得
		player_status_save.SetPlayerSpeed(plyer_status_csv_info[y][4].getInt());
		//csvから必要な経験値の取得
		player_status_save.SetPlayerExpoint(plyer_status_csv_info[y][5].getInt());
		//csvから最大Mpを取得
		player_status_save.SetPlayerCurentMp(plyer_status_csv_info[y][6].getInt());
		//csvからMpを取得
		player_status_save.SetPlayerMaxMp(plyer_status_csv_info[y][6].getInt());
		//csvからmagicPowerの取得
		player_status_save.SetMagicPower(plyer_status_csv_info[y][7].getInt());

		player_status_type.emplace_back(player_status_save);

	}	

}

//プレイヤーの画像を読み込む
//IDによって画像を切り替える
//ID : 2次元配列の何行目をさしているか
void Player::SetPlayerAnimationHdl(const int& ghdl_id , const int& player_id)
{
	//アニメーション画像を格納配列
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
			player_image[i].c_str(),						// 画像のパス
			TOTAL_FRAME,									// 総フレーム数
			HORIZONTAL_FRAME,								// 横フレーム数
			VERTICAL_FRAME,									// 縦フレーム数
			HORIZONTAL_ONEFRAME,							// 横方向へ 1フレームあたりの幅
			VERTICAL_ONEFRAME,								// 縦方向へ 1フレームあたりの幅
			anim_hdls[i]);									// 画像ハンドルを格納する配列名);
	}

	//プレイヤーのアイコンをロードする
	player_icon = ResourceManager::GetResourceManager()->LoadGraphEX(PLAYER_BATTLE_ICON[player_id]);
}

//セーブする際のスキルの追加
void Player::SaveReadSkill(const int& skill_id )
{
	// スキルリストを取得
	auto& skill_list = skill->GetOverallSkills();

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

Player::PlayerStatus Player::GetPlyerStatus(const int& level) const
{
	auto it = std::find_if(player_status_type.begin(), player_status_type.end(), [level]
	(const PlayerStatus& status) {return status.GetLevel() == level; });

	if (it != player_status_type.end()) {
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
	if (player_status_save.GetcurentHp() <= 0) {

		//ログを流す
		battle_log->AddLog("全滅した");

		//ログを流す
		battle_log->AddLog("プレイヤーはお金を半分失った");

		//プレイヤーのHpを0にする
		player_status_save.SetPlayerCurentHp(0);

		//BGMを止める
		SoundManager::GetSoundManager()->StopSound("sound/BGM/sentou.mp3");
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/zenmetu.mp3", DX_PLAYTYPE_BACK);

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
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);

	//敵にダメージを与える
	nomal_attack->SkillUse(player_status_save, enemy_status ,battle_log);

}

//スキルをセットする
void Player::SkillSet(const Shared<BattleLog>& battle_log)
{
	switch (player_status_save.GetLevel())
	{
		//レベル2の時に火炎斬りを覚える
	case 2:
		AddSkill(std::make_shared<FlameSlash>());

		if (battle_log) {
			//ログを流す
			battle_log->AddLog("火炎斬りを覚えた!");
		}

		break;
		//レベル3の時にヒールを覚える
	case 3:
		
		AddSkill(std::make_shared<Heal>());

		if (battle_log) {
			battle_log->AddLog("ヒールを覚えた!");
		}

		break;
		//レベル4の時にアイスブラストを覚える
	case 4:

		AddSkill(std::make_shared<IceBlast>());

		if (battle_log) {
			battle_log->AddLog("アイスブラストを覚えた!");
		}

		break;
		//レベル5の時にサンダーボルトを覚える
	case 5:
		AddSkill(std::make_shared<ThunderBolt>());

		if (battle_log) {
			battle_log->AddLog("サンダーボルトを覚えた!");
		}

		break;

	//レベル8の時にファイアーストームを覚える
	case 8:

		AddSkill(std::make_shared<FireStorm>());

		if (battle_log) {
			battle_log->AddLog("ファイアーストームを覚えた！");
		}

		break;

	//レベル10の時にウォーターブレードを覚える
	case 10:

		AddSkill(std::make_shared<WaterBlade>());

		if (battle_log) {
			battle_log->AddLog("ウォーターブレードを覚えた！");
		}

		break;

	//レベル14の時にインフェルノを覚える
	case 14:

		AddSkill(std::make_shared<Inferno>());

		if (battle_log) {
			battle_log->AddLog("インフェルノを覚えた！");
		}

		break;

	//レベル16の時にブリザードを覚える
	case 16:

		AddSkill(std::make_shared<Blizzard>());

		if (battle_log) {
			battle_log->AddLog("ブリザードを覚えた！");
		}

	//レベル18の時にライトニングスラッシュを覚える
	case 18:

		AddSkill(std::make_shared<LightningSlash>());

		if (battle_log) {
			battle_log->AddLog("ライトニングスラッシュを覚えた！");
		}

		break;
	}
}

//スキルを追加する
void Player::AddSkill(const Shared<Skill>& skill)
{
	player_skill_list.emplace_back(skill);
	skill_num++;
}

//セーブ
void Player::PlayerSave()
{
	FILE* fp = nullptr;
	fopen_s(&fp, "player.bin", "wb");

	if (fp) {

		// Playerステータスを取得する

		//hp
		int player_hp = player_status_save.GetcurentHp();

		//max_hp
		int player_max_hp = player_status_save.GetMaxHp();

		//mp
		int player_mp = player_status_save.GetCurentMp();

		//max_mp
		int player_max_mp = player_status_save.GetMaxMp();

		//魔法力
		int player_magic_power = player_status_save.GetMagicPower();

		//攻撃力
		int player_attack = player_status_save.GetBaseAttack();

		//防御力
		int player_defense = player_status_save.GetBaseDefance();

		//素早さ
		int player_speed = player_status_save.GetSpeed();

		//プレイヤーの画像ID
		int player_graphics_id = player_id;

		// プレイヤーの名前を保存する
		const std::string& player_name = name;

		//名前の長さ
		size_t name_length = player_name.size();

		//レベル
		int player_level = player_status_save.GetLevel();

		//必要な経験値
		int player_expoint = player_status_save.GetExpoint();

		//当たり判定の関係で座標を少し調整
		tnl::Vector3 add_pos = { 0, 20 , 0 };

		//現在位置
		tnl::Vector3 player_pos = plyer_pos + add_pos;

		//インベントリのサイズ分回して配列に格納する
		const auto& player_list = GameManager::GetGameManager()->GetInventory()->GetInventoryList();
		std::vector<int> player_item_list;

		//インベントリにアイテムがあれば
		if (!player_list.empty()) {
			for (int i = 0; i < player_list.size(); i++) {
				player_item_list.emplace_back(GameManager::GetGameManager()->GetInventory()->GetInventoryList()[i].GetItemId());
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

		//イベント関係のフラグ
		bool event_flag_1 = EventManager::GetEventManager()->GetEnemyEventFlag_1();
		bool event_flag_2 = EventManager::GetEventManager()->GetEnemyEventFlag_2();
		bool event_flag_3 = EventManager::GetEventManager()->GetEnemyEventFlag_3();

		//プレイヤーのステータスを保存する

		//hp
		fwrite(&player_hp, sizeof(int), 1, fp);
		//max_hp
		fwrite(&player_max_hp, sizeof(int), 1, fp);
		//mp
		fwrite(&player_mp, sizeof(int), 1, fp);
		//max_mp 
		fwrite(&player_max_mp, sizeof(int), 1, fp);
		//魔法力
		fwrite(&player_magic_power, sizeof(int), 1, fp);
		//攻撃力
		fwrite(&player_attack, sizeof(int), 1, fp);
		//防御力
		fwrite(&player_defense, sizeof(int), 1, fp);
		//素早さ
		fwrite(&player_speed, sizeof(int), 1, fp);
		//プレイヤーのID
		fwrite(&player_graphics_id, sizeof(int), 1, fp);
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
		//イベントのフラグ関係の保存
		fwrite(&event_flag_1, sizeof(bool), 1, fp);
		fwrite(&event_flag_2, sizeof(bool), 1, fp);
		fwrite(&event_flag_3, sizeof(bool), 1, fp);

		// 文字列の長さを保存
		fwrite(&name_length, sizeof(size_t), 1, fp); 
		// 文字列そのものを保存
		fwrite(player_name.c_str(), sizeof(char), name_length, fp); 

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
void Player::PlayerLoad()
{
	FILE* fp = nullptr;
	fopen_s(&fp, "player.bin", "rb");

	if (fp) {

		// 一時的に保存しておくステータス
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

		// ステータスをロードする

		//hp
		fread(&player_hp, sizeof(int), 1, fp);
		//max_hp
		fread(&player_max_hp, sizeof(int), 1, fp);
		//mp
		fread(&player_mp, sizeof(int), 1, fp);
		//max_mp
		fread(&player_max_mp, sizeof(int), 1, fp);
		//魔法力
		fread(&player_magic_power, sizeof(int), 1, fp);
		//攻撃力
		fread(&player_attack, sizeof(int), 1, fp);
		//防御力
		fread(&player_defense, sizeof(int), 1, fp);
		//素早さ
		fread(&player_speed, sizeof(int), 1, fp);
		//プレイヤーのID
		fread(&player_graphics_id, sizeof(int), 1, fp);
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
		//イベントのフラグ関係
		fread(&event_flag_1, sizeof(bool), 1, fp);
		fread(&event_flag_2, sizeof(bool), 1, fp);
		fread(&event_flag_3, sizeof(bool), 1, fp);

		// プレイヤーの名前をロードする
		size_t name_length;

		// 文字列の長さをロード
		fread(&name_length, sizeof(size_t), 1, fp);
		//サイズを決定する
		std::string player_name;
		player_name.resize(name_length);

		// 文字列そのものをロード
		fread(&player_name[0], sizeof(char), name_length, fp);

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

		//イベント関係のフラグをセットする
		EventManager::GetEventManager()->SetEnemyEventFlag_1(event_flag_1);
		EventManager::GetEventManager()->SetEnemyEventFlag_2(event_flag_2);
		EventManager::GetEventManager()->SetEnemyEventFlag_3(event_flag_3);


		// インベントリの要素を格納する
		for (int item : player_inventory_list) {
			GameManager::GetGameManager()->GetInventory()->AddInventory(item);
		}

		//スキルリストの要素を格納する
		for (int skill : playe_skill_list) {
			SaveReadSkill(skill);
		}

		fclose(fp);
	}
}

//スキルを全て追加する(デバック用)
void Player::DebugAddSkill()
{
	//スキルのデバックカウント
	static int debug_skill_count = 0;

	//一回のみスキルを追加できる
	if (debug_skill_count < 1) {
		for (auto& skill_lists : skill->GetOverallSkills()) {
			SaveReadSkill(skill_lists->getId());
		}
		debug_skill_count++;
	}
}

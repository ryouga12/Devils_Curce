#include "Enemy.h"
#include"../Scene/battleScene.h"
#include"../Menu/BattleLog.h"
#include"../Manager/GameManager.h"
#include"../Object/Player.h"
#include"../Manager/CsvManager.h"
#include"../Manager/EventManager.h"

Enemy::Enemy()
{
	//csvの読み込み	
	enemy_csv_array = CsvManager::GetCsvManager()->GetEnemyStatusCsv();

	//Enemyのステータスをロードする関数
	EnemyLoadTypeInfo();
	
	//Enemyの初期化
	EnemyInit();

	//アイテム
	item = std::make_shared<Item>();

}

//描画処理
void Enemy::Draw()
{
	if (!dead_enemy_flag) {
		DrawRotaGraphF(ENEMY_POS.x, ENEMY_POS.y, koni::Numeric::SCALE_TRIPLE_AND_HALF, koni::Color::BLACK, enemy_hdl, true);
	}
}

void Enemy::EnemyLoadTypeInfo()
{

	int max_num = static_cast<int>(enemy_csv_array.size());

	enemy_status_total_num = max_num;

	for (int y = 1; y < max_num; y++) {
		
		//csvから名前の取得
		enemy_status_type.SetEnemyName(enemy_csv_array[y][0].c_str());
		//csvからidの取得
		enemy_status_type.SetEnemyId(std::stoi(enemy_csv_array[y][1].c_str()));
		//csvからhpの取得
		enemy_status_type.SetEnemyHp(std::stoi(enemy_csv_array[y][2].c_str()));
		//csvから攻撃力の取得
		enemy_status_type.SetEnemyAttack(std::stoi(enemy_csv_array[y][3].c_str()));
		//csvから防御力の取得
		enemy_status_type.SetEnemyDefance(std::stoi(enemy_csv_array[y][4].c_str()));
		//csvからすばやさの取得
		enemy_status_type.SetEnemySpeed(std::stoi(enemy_csv_array[y][5].c_str()));
		//csvから経験値の取得
		enemy_status_type.SetEnemyExpoint(std::stoi(enemy_csv_array[y][6].c_str()));
		//csvからお金の取得
		enemy_status_type.SetEnemyMoney(std::stoi(enemy_csv_array[y][7].c_str()));
		//csvからghpathの取得
		enemy_status_type.SetGhdl(enemy_csv_array[y][8].c_str());
		//csvから炎耐性値を取得
		enemy_status_type.SetFireResist((float)std::atof(enemy_csv_array[y][9].c_str()));
		//csvから氷耐性値の取得
		enemy_status_type.SetIceResist((float)std::atof(enemy_csv_array[y][10].c_str()));
		//csvから雷耐性値の取得
		enemy_status_type.SetThunderResist((float)std::atof(enemy_csv_array[y][11].c_str()));
		//csvから通常ドロップのIDを取得
		enemy_status_type.SetNomalDropID(std::stoi(enemy_csv_array[y][12].c_str()));
		//csvからレアドロップのIDを取得
		enemy_status_type.SetRareDoropID(std::stoi(enemy_csv_array[y][13].c_str()));
		//csvから攻撃時のSEのpathを取得
		enemy_status_type.SetAttackSe(enemy_csv_array[y][14].c_str());
		//csvから魔法力を取得
		enemy_status_type.SetMagicPower(std::stoi(enemy_csv_array[y][15].c_str()));
		//csvからノーマルドロップの確率を取得する
		enemy_status_type.SetNomalDropProbability(std::stoi(enemy_csv_array[y][16].c_str()));
		//csvからレアドロップの確率を取得する
		enemy_status_type.SetRareDropProbability(std::stoi(enemy_csv_array[y][17].c_str()));
		//csvから敵の種族を取得する
		enemy_status_type.SetEnemyRace(enemy_csv_array[y][18].c_str());

		enemy_type_array.emplace_back(enemy_status_type);
	}


}

void Enemy::EnemyInit()
{
	//エラー用の配列をクリアして初期化する
	null_array.clear();
}

Enemy::EnemyConnection Enemy::GetEnemyStatus(int id) const
{
	auto it = std::find_if(enemy_type_array.begin(), enemy_type_array.end(), [id]
	(const EnemyConnection& status) {return status.GetEnemyId() == id; });

	if (it != enemy_type_array.end()) {
		return *it;
	}
	else {
		// IDに対応するアイテムが見つからない場合は、適当なデフォルト値を返すかエラー処理を行うなど
		return EnemyConnection{};
	}
}

//アイテムのドロップを決める
void Enemy::InitEnemyItemDrop(const int& EnemyID)
{
	enemy_drop_item.clear();

	//配列のIDを比較して該当したIDをインベントリに格納する
	for (int i = 0; i < enemy_csv_array.size(); i++) {

		//配列の1行目は説明の為+1して確認を行う
		if (EnemyID == std::stoi(enemy_csv_array[i + 1][1].c_str())) {

			//インベントリにそれぞれに対応したアイテムを格納
			enemy_drop_item.emplace_back(item->GetItemById(std::stoi(enemy_csv_array[i + 1][12].c_str())));
 			enemy_drop_item.emplace_back(item->GetItemById(std::stoi(enemy_csv_array[i + 1][13].c_str())));

			break;
		}
	}

}

//敵の配列を初期化する
//引数のIDはどのマップチップに居るかで配列に入れる敵の情報を決める
void Enemy::InitEnemyArray(const int& id)
{
	//敵の配列をクリアする
	//敵の配列を一度クリアして同じ情報を取得できないようにする
	enemy_array.clear();

	//初期の敵(弱い)
	if (id == 1) {
		for (int i = 0; i < ENEMY_NUM; i++) {
			enemy_array.emplace_back(GetEnemyStatus(i));
		}
	}
	//平原の敵(普通)
	else if (id == 2) {

		//csvの6行目から5体選出する
		for (int i = 6; i < enemy_csv_array.size(); i++) {

			//敵によってIDを取得する
			//サイズが5個になるまで回す
			if (ENEMY_NUM > enemy_array.size()) {

				auto enmeyid = GetEnemyStatus(std::stoi(enemy_csv_array[i][1].c_str()));

				//敵を配列に格納する
				enemy_array.emplace_back(GetEnemyStatus(enmeyid.GetEnemyId()));
			}
			//敵の最大の数を上回ったら処理を抜ける
			else {
				break;
			}
		}
	}
	//荒原の敵(少し強い)
	else if (id == 3) {

		//csvの11番目から5体選出する
		for (int i = 11; i < enemy_csv_array.size(); i++) {

			//敵によってIDを取得する
			//サイズが5個になるまで回す
			if (ENEMY_NUM > enemy_array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(enemy_csv_array[i][1].c_str()));

				//敵を配列に格納する
				enemy_array.emplace_back(GetEnemyStatus(enmeyid.GetEnemyId()));
			}
			//敵の最大の数を上回ったら処理を抜ける
			else {
				break;
			}
		}
	}
	//雪原(強い)
	else if (id == 4) {

		//csvの16番目から5体選出する
		for (int i = 23; i < enemy_csv_array.size(); i++) {

			//敵によってIDを取得する
			//サイズが5個になるまで回す
			if (ENEMY_NUM > enemy_array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(enemy_csv_array[i][1].c_str()));

				//敵を配列に格納する
				enemy_array.emplace_back(GetEnemyStatus(enmeyid.GetEnemyId()));
			}
			//敵の最大の数を上回ったら処理を抜ける
			else {
				break;
			}
		}
	}
	//ボスエリアにいる敵(強い)
	else if (id == 5) {

		//csvの16番目から5体選出する
		for (int i = 16; i < enemy_csv_array.size(); i++) {

			//敵によってIDを取得する
			//サイズが5個になるまで回す
			if (ENEMY_NUM > enemy_array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(enemy_csv_array[i][1].c_str()));

				//敵を配列に格納する
				enemy_array.emplace_back(GetEnemyStatus(enmeyid.GetEnemyId()));
			}
			//敵の最大の数を上回ったら処理を抜ける
			else {
				break;
			}
		}
	}
}

//敵の死亡処理
bool Enemy::ChackDeadEnemy()
{
	//敵のHpが0になったかチェックを行う
	if (enemy_array[enemy_index].GetEnemyHp() <= 0) {

		//敵が死んでたら死亡処理を呼び出す
		battle_scene_->DeadEnemyProcces(GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave(), enemy_array[enemy_index]);

		//レベルアップ処理
		battle_scene_->ChackPlayerLevelUp(GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave());

		return true;
	}
	else {
		return false;
	}
	
}

//------------------------------------------------------------------------------------------------------------
//
//雑魚モンスター
//
//-------------------------------------------------------------------------------------------------------------


MobMonster::MobMonster(const int& enemy_id)
{
	//タイプをモブにする
	enemy_type = Enemytype::MOB;

	//敵を配列に格納する
	InitEnemyArray(enemy_id);

	//インデックスを1～4のランダムな数字に設定する
	enemy_index = rand() % ENEMY_NUM;

	//新しい配列を生成する
	auto& enemy_ = enemy_array[enemy_index];

	//敵のグラフィックハンドルを読み込む
	enemy_hdl = ResourceManager::GetResourceManager()->LoadGraphEX(enemy_.GetEnemyGhdl().c_str());

	//敵のアイテムドロップを格納する
	InitEnemyItemDrop(enemy_.GetEnemyId());
}

//雑魚モンスターの攻撃処理
void MobMonster::EnemyAction(const Shared<BattleLog>& battle_log)
{
	//プレイヤーの防御力を取得する
	auto player_defance = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetDefance();

	//プレイヤーの名前を取得する
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//ダメージを計算する「
	int damage = enemy_array[enemy_index].GetEnemyAttack() - player_defance;

	//ダメージが0を下回るときに1に設定する
	if (damage <= 0) { damage = 1; }

	//プレイヤーのHpを減らす
	GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentHp(GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp() - damage);

	//バトルログを流す
	const std::string  log = player_name + "は" + enemy_array[enemy_index].GetEnemyName() + "から" + std::to_string(damage) + "ダメージをくらった";
	battle_log->AddLog(log);

	// SEを流す
	SoundManager::GetSoundManager()->Sound_Play(enemy_array[enemy_index].GetSeAttack().c_str(), DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(80, enemy_array[enemy_index].GetSeAttack().c_str());


}

//------------------------------------------------------------------------------------------------------------
//
//ボスモンスター
//
//-------------------------------------------------------------------------------------------------------------


BossMonster::BossMonster(const int& enemy_id) {

	//敵の種類をボスに変更する
	enemy_type = Enemytype::BOSS;

	//スキルを格納する
	InitEnemySkill(enemy_id);

	//敵の死亡処理など配列に入れた値を参照している為配列に格納する
	enemy_array.emplace_back(GetEnemyStatus(enemy_id));

	//配列は一つの為0を入れる
	enemy_index = 0;

	//敵の配列を格納する
	auto& enemy = enemy_array[enemy_index];

	//敵のグラフィックハンドルを読み込む
	enemy_hdl = ResourceManager::GetResourceManager()->LoadGraphEX(enemy.GetEnemyGhdl().c_str());

	//敵のアイテムドロップを格納する
	InitEnemyItemDrop(enemy.GetEnemyId());
}

//敵のスキル配列の初期化
void BossMonster::InitEnemySkill(const int& enemy_id)
{
	//敵のスキル配列を初期化する
	enemy_skill_.clear();

	switch (enemy_id)
	{
	
	//最後のボス
	case static_cast<int>(BossType::ZERAHKIEL):

		//それぞれのスキルを格納する
		enemy_skill_.emplace_back(std::make_shared<ChaosFlare>());
		enemy_skill_.emplace_back(std::make_shared<DeathScytheWind>());
		enemy_skill_.emplace_back(std::make_shared<DrakClaw>());
		enemy_skill_.emplace_back(std::make_shared<WaveDarkness>());
		enemy_skill_.emplace_back(std::make_shared<HellFlameBlade>());

		break;

	//中ボス1体目
	case static_cast<int>(BossType::GROVEGUARDIAN):

		//それぞれのスキルを格納する
		enemy_skill_.emplace_back(std::make_shared<LifeWind>());
		enemy_skill_.emplace_back(std::make_shared<EnergyBlast>());
		enemy_skill_.emplace_back(std::make_shared<DarkCharge>());

		break;

	//中ボス2体目
	case static_cast<int>(BossType::SOULWARRIOR):

		//それぞれのスキルを格納する
		enemy_skill_.emplace_back(std::make_shared<SlashLight>());
		enemy_skill_.emplace_back(std::make_shared<PowerHeroes>());
		enemy_skill_.emplace_back(std::make_shared<LightSpear>());

		break;

	//中ボス3体目
	case static_cast<int>(BossType::SHADOWENEMY):

		//それぞれのスキルを格納する
		enemy_skill_.emplace_back(std::make_shared<GrimReaperSickle>());
		enemy_skill_.emplace_back(std::make_shared<SoulReper>());
		enemy_skill_.emplace_back(std::make_shared<LightUnderworld>());

		break;

	//サブボス
	case static_cast<int>(BossType::CORPORAL):

		//それぞれのスキルを格納する
		enemy_skill_.emplace_back(std::make_shared<FlameSlash>(Skill::SkillUserType::ENEMY));
		enemy_skill_.emplace_back(std::make_shared<WaterBlade>(Skill::SkillUserType::ENEMY));
		enemy_skill_.emplace_back(std::make_shared<ThunderBolt>(Skill::SkillUserType::ENEMY));

		break;


	//裏ボス
	case static_cast<int>(BossType::PIRATE):

		//それぞれのスキルを格納する
		enemy_skill_.emplace_back(std::make_shared<WaterForge>(Skill::SkillUserType::ENEMY));
		enemy_skill_.emplace_back(std::make_shared<WaterBlade>(Skill::SkillUserType::ENEMY));
		enemy_skill_.emplace_back(std::make_shared<RainOfHell>());
		enemy_skill_.emplace_back(std::make_shared<ThunderBolt>(Skill::SkillUserType::ENEMY));

		break;

	default:

		tnl::DebugTrace("\n-------------------------------------------------------------");
		tnl::DebugTrace("\nスキルを初期化できませんでした");
		tnl::DebugTrace("\n-------------------------------------------------------------");

		break;
	}
}

//ボスモンスターの攻撃処理
void BossMonster::EnemyAction(const Shared<BattleLog>& battle_log)
{
	//敵のスキル配列が空じゃなかったら
	if (!enemy_skill_.empty()) {
		
		auto enemy_skill_size = enemy_skill_.size();

		//ランダムでインデックスを取得する
		enmey_skill_index = rand() % enemy_skill_size;

		//敵のスキルが攻撃系だった場合ダメージ計算を行う
		if (enemy_skill_[enmey_skill_index]->GetSkillType() == AttackType) {

			//プレイヤーのステータス
			auto& player_status = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();

			//プレイヤーに攻撃する
			enemy_skill_[enmey_skill_index]->SkillUse(player_status, enemy_array[enemy_index], battle_log);

			//プレイヤーのhpが0を下回ったら0に設定する
			if (player_status.GetcurentHp() <= 0) {
				player_status.SetPlayerCurentHp(0);
			}
		}
		//バフスキルだった場合、バフ効果を適用させる
		else if (enemy_skill_[enmey_skill_index]->GetSkillType() == BuffType) {

			//エネミーにバフ効果を適用させる
			enemy_skill_[enmey_skill_index]->SkillUse(enemy_array[enemy_index], battle_log);

		}
		
	}
	else {
		tnl::DebugTrace("\n-------------------------------------------------------------");
		tnl::DebugTrace("\n攻撃できませんでした");
		tnl::DebugTrace("\n-------------------------------------------------------------");
		return;

	}
}

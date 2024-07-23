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
	enemy_csv_array = CsvManager::getCsvManager()->GetEnemyStatusCsv();

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
		DrawRotaGraphF(ENEMY_POS.x, ENEMY_POS.y, ENEMY_SIZE, COLOR_BRACK, enemy_hdl, true);
	}
}

void Enemy::EnemyLoadTypeInfo()
{

	int max_num = static_cast<int>(enemy_csv_array.size());

	EnemyStatus_Total_Num = max_num;

	for (int y = 1; y < max_num; y++) {
		
		//csvから名前の取得
		Enemy_Status_Type.SetEnemyName(enemy_csv_array[y][0].c_str());
		//csvからidの取得
		Enemy_Status_Type.SetEnemyId(std::stoi(enemy_csv_array[y][1].c_str()));
		//csvからhpの取得
		Enemy_Status_Type.SetEnemyHp(std::stoi(enemy_csv_array[y][2].c_str()));
		//csvから攻撃力の取得
		Enemy_Status_Type.SetEnemyAttack(std::stoi(enemy_csv_array[y][3].c_str()));
		//csvから防御力の取得
		Enemy_Status_Type.SetEnemyDefance(std::stoi(enemy_csv_array[y][4].c_str()));
		//csvからすばやさの取得
		Enemy_Status_Type.SetEnemySpeed(std::stoi(enemy_csv_array[y][5].c_str()));
		//csvから経験値の取得
		Enemy_Status_Type.SetEnemyExpoint(std::stoi(enemy_csv_array[y][6].c_str()));
		//csvからお金の取得
		Enemy_Status_Type.SetEnemyMoney(std::stoi(enemy_csv_array[y][7].c_str()));
		//csvからghpathの取得
		Enemy_Status_Type.SetGhdl(enemy_csv_array[y][8].c_str());
		//csvから炎耐性値を取得
		Enemy_Status_Type.SetFireResist(static_cast<float>(std::stoi(enemy_csv_array[y][9].c_str())));
		//csvから氷耐性値の取得
		Enemy_Status_Type.SetIceResist(static_cast<float>(std::stoi(enemy_csv_array[y][10].c_str())));
		//csvから雷耐性値の取得
		Enemy_Status_Type.SetThunderResist(static_cast<float>(std::stoi(enemy_csv_array[y][11].c_str())));
		//csvから通常ドロップのIDを取得
		Enemy_Status_Type.SetNomalDropID(std::stoi(enemy_csv_array[y][12].c_str()));
		//csvからレアドロップのIDを取得
		Enemy_Status_Type.SetRareDoropID(std::stoi(enemy_csv_array[y][13].c_str()));
		//csvから攻撃時のSEのpathを取得
		Enemy_Status_Type.SetAttackSe(enemy_csv_array[y][14].c_str());
		//csvから魔法力を取得
		Enemy_Status_Type.SetMagicPower(std::stoi(enemy_csv_array[y][15].c_str()));
		//csvからノーマルドロップの確率を取得する
		Enemy_Status_Type.SetNomalDropProbability(std::stoi(enemy_csv_array[y][16].c_str()));
		//csvからレアドロップの確率を取得する
		Enemy_Status_Type.SetRareDropProbability(std::stoi(enemy_csv_array[y][17].c_str()));

		Enemy_Type_Array.emplace_back(Enemy_Status_Type);
	}


}

void Enemy::EnemyInit()
{
	//エラー用の配列をクリアして初期化する
	null_array.clear();
}

Enemy::EnemyConnection Enemy::GetEnemyStatus(int id) const
{
	auto it = std::find_if(Enemy_Type_Array.begin(), Enemy_Type_Array.end(), [id]
	(const EnemyConnection& status) {return status.GetEnemyId() == id; });

	if (it != Enemy_Type_Array.end()) {
		return *it;
	}
	else {
		// IDに対応するアイテムが見つからない場合は、適当なデフォルト値を返すかエラー処理を行うなど
		return EnemyConnection{};
	}
}

//アイテムのドロップを決める
void Enemy::InitEnemyItemDrop(int EnemyID)
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
void Enemy::InitEnemyArray(int id)
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
	//ボスエリアにいる敵(強い)
	else if (id == 4) {

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
		battle_scene_->DeadEnemyProcces(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave(), enemy_array[enemy_index]);

		//レベルアップ処理
		battle_scene_->ChackPlayerLevelUp(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave());

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


MobMonster::MobMonster(const int enemy_id)
{
	//タイプをモブにする
	enemy_type = Enemytype::MOB;

	//敵を配列に格納する
	InitEnemyArray(enemy_id);

	//インデックスを1～4のランダムな数字に設定する
	enemy_index = rand() % 5;

	//新しい配列を生成する
	auto& enemy_ = enemy_array[enemy_index];

	//敵のグラフィックハンドルを読み込む
	enemy_hdl = ResourceManager::getResourceManager()->LoadGraphEX(enemy_.GetEnemyGhdl().c_str());

	//敵のアイテムドロップを格納する
	InitEnemyItemDrop(enemy_.GetEnemyId());
}

//雑魚モンスターの攻撃処理
void MobMonster::EnemyAction(Shared<BattleLog>battle_log)
{
	//プレイヤーの防御力を取得する
	auto PlayerDefance = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetDefance();

	//ダメージを計算する「
	int damage = enemy_array[enemy_index].GetEnemyAttack() - PlayerDefance;

	//ダメージが0を下回るときに0に設定する
	if (damage <= 0) { damage = 0; }

	//プレイヤーのHpを減らす
	GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentHp(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetcurentHp() - damage);

	//カメラシェイクを起こす(ダメージを受けている感じを出す)
	// 後程実装予定
	//GameManager::getGameManager()->getCamera()->StartShake(1, 1.5f);

	//バトルログを流す
	battle_log->addDamagePlayerLog("Enemy", "Player", damage);

	// SEを流す
	SoundManager::getSoundManager()->sound_Play(enemy_array[enemy_index].GetSeAttack().c_str(), DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(80, enemy_array[enemy_index].GetSeAttack().c_str());


}

//------------------------------------------------------------------------------------------------------------
//
//ボスモンスター
//
//-------------------------------------------------------------------------------------------------------------


BossMonster::BossMonster(int enemy_id) {

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
	enemy_hdl = ResourceManager::getResourceManager()->LoadGraphEX(enemy.GetEnemyGhdl().c_str());

	//敵のアイテムドロップを格納する
	InitEnemyItemDrop(enemy.GetEnemyId());
}

//敵のスキル配列の初期化
void BossMonster::InitEnemySkill(int enemy_id)
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

		break;

	case static_cast<int>(BossType::GROVEGUARDIAN):

		//それぞれのスキルを格納する
		enemy_skill_.emplace_back(std::make_shared<ChaosFlare>());
		enemy_skill_.emplace_back(std::make_shared<DeathScytheWind>());
		enemy_skill_.emplace_back(std::make_shared<DrakClaw>());

		break;

	default:

		tnl::DebugTrace("-------------------------------------------------------------");
		tnl::DebugTrace("スキルを初期化できませんでした");
		tnl::DebugTrace("-------------------------------------------------------------");

		break;
	}
}

//ボスモンスターの攻撃処理
void BossMonster::EnemyAction(Shared<BattleLog> battle_log)
{
	//敵のスキル配列が空じゃなかったら
	if (!enemy_skill_.empty()) {
		
		//ランダムでインデックスを取得する(スキルの総数は3の為 0 から 2 のインデックスを取得する)
		enmey_skill_index = rand() % 3;

		//プレイヤーのステータス
		auto& player_status = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

		//プレイヤーに攻撃する
		enemy_skill_[enmey_skill_index]->SkillUse(player_status, enemy_array[enemy_index], battle_log);

		//プレイヤーのhpが0を下回ったら0に設定する
		if (player_status.GetcurentHp() <= 0) {
			player_status.SetPlayerCurentHp(0);
		}
	}
	else {
		tnl::DebugTrace("-------------------------------------------------------------");
		tnl::DebugTrace("攻撃できませんでした");
		tnl::DebugTrace("-------------------------------------------------------------");
		return;

	}
}

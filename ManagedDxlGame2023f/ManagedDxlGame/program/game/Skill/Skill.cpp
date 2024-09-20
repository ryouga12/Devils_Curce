#include "Skill.h"
#include"../Menu/BattleLog.h"
#include <ctime>
#include <random>
#include"../Manager/GameManager.h"

Skill::Skill(const int _id, const std::string& _name, const float _power, const std::string& _description, const int consumeMp, const int skilltype) : BaseSkill(_id, _name, _power, _description, consumeMp, skilltype)
{

}

Skill::~Skill()
{
	tnl::DebugTrace("\n------------------------------------------------------------");
	tnl::DebugTrace("\n %sが解放されました", skill_name.c_str());
	tnl::DebugTrace("\n------------------------------------------------------------");

}

void Skill::SkillUseAnimation()
{
	Effect_Animation->play_animation();
}

void Skill::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

void Skill::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

void Skill::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//mp処理
void Skill::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mpを取得する
	auto CurentMp = playerStatus.GetCurentMp();

	//現在のMpから消費Mpを減らす
	auto consumeMp = CurentMp - consume_mp;

	//Mpを消費させる
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//全体のスキルを格納する
void Skill::AddSkillList()
{
	//空の場合のみ
	if (skill_list.empty()) {
		skill_list.emplace_back(std::make_shared<FlameSlash>());
		skill_list.emplace_back(std::make_shared<FireStorm>());
		skill_list.emplace_back(std::make_shared<Inferno>());
		skill_list.emplace_back(std::make_shared<IceBlast>());
		skill_list.emplace_back(std::make_shared<WaterBlade>());
		skill_list.emplace_back(std::make_shared<Blizzard>());
		skill_list.emplace_back(std::make_shared<ThunderBolt>());
		skill_list.emplace_back(std::make_shared<LightningSlash>());
		skill_list.emplace_back(std::make_shared<Heal>());
		skill_list.emplace_back(std::make_shared<HighHeels>());
		skill_list.emplace_back(std::make_shared<SlimBell>());
		skill_list.emplace_back(std::make_shared<SnakeBell>());
	}
}

//ランダムファクターの値を設定する
void Skill::initializeRandomFactor()
{
	// 現在の時刻をシードとしてランダムエンジンを初期化
	std::default_random_engine generator(static_cast<unsigned int>(std::time(nullptr)));

	// 0.95 から 1.05 の範囲で一様に分布するランダム値を生成
	std::uniform_real_distribution<float> distribution(0.95f, 1.05f); // ±5%のランダム変動

	// ランダムファクターを生成し、メンバ変数 random_factor に格納
	random_factor = distribution(generator);
}

void Skill::PlayerReductionRate(float& damage)
{
	// プレイヤーのレベルを取得
	int playerLevel = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetLevel();

	// レベルによる軽減率を計算
	float reductionFactor = playerLevel * BASE_REDUCTION_RATE; // レベルごとに3.5%軽減

	// 最大軽減率を設定（70%軽減）
	reductionFactor = (reductionFactor > koni::Numeric::PERCENT_70) ? koni::Numeric::PERCENT_70 : reductionFactor;

	// 軽減率を適用
	damage *= (koni::Numeric::FLOAT_VALUE_10 - reductionFactor);
}

void Skill::CheckEpicWeapon(float& damage , Enemy::EnemyConnection& enemyStatus_)
{
	auto weapon_array = GameManager::GetGameManager()->GetInventory()->getEquipArray();

	// Epic属性のチェック
	bool isEpic = false;

	//武器が特殊武器か確認する
	for (auto& weapon_ : weapon_array) {
		isEpic = weapon_.GetEpic();

		//武器が特殊武器でかつ、ダメージを多く与えられる敵だった場合1.3倍の補正をかける
		if (isEpic && weapon_.GetEpicCorrection() == enemyStatus_.GetEnemyRace()) {
			damage *= koni::Numeric::SCALE_ONE_THIRTY_F;
		}
	}
}

//---------------------------------------------------------------------------------------------------------

//通常攻撃
Nomal_Attack::Nomal_Attack(int weapon_type): Skill(0, "攻撃", 1, "通常攻撃",0 , AttackType)
{
	//エフェクトの幅のサイズ
	effect_width_size = 120;

	//エフェクトの縦のサイズ
	effect_height_size = 120;



	//武器の種類によってアニメーションを変える
	switch (weapon_type)
	{
	case EMPTY:

		//エフェクトの幅の数
		effect_width_num = 5;

		//エフェクトの高さの数 
		effect_height_num = 2;

		//エフェクトのフレームの合計
		effect_frame_all_num = effect_width_num * effect_height_num;

		//何も装備していない時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack＿Empty.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Seサウンド
		sound_text = "sound/SoundEffect/dageki_3.mp3";

		break;

	case SWOED:

		//座標を変える
		effect_pos.x = 630;

		//エフェクトの幅の数
		effect_width_num = 5;

		//エフェクトの高さの数 
		effect_height_num = 1;

		//エフェクトのフレームの合計
		effect_frame_all_num = effect_width_num * effect_height_num;

		//剣を装備している時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Sword.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Seサウンド
		sound_text = "sound/SoundEffect/zanngeki_2.mp3";

		break;

	case HAMMER:

		//エフェクトの幅の数
		effect_width_num = 7;

		//エフェクトの高さの数 
		effect_height_num = 1;

		//エフェクトのフレームの合計
		effect_frame_all_num = effect_width_num * effect_height_num;

		//ハンマーを装備している時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Hammer.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Seサウンド
		sound_text = "sound/SoundEffect/dageki.mp3";

		break;

	case SPEAR:

		//エフェクトの幅の数
		effect_width_num = 7;

		//エフェクトの高さの数 
		effect_height_num = 1;

		//エフェクトのフレームの合計
		effect_frame_all_num = effect_width_num * effect_height_num;

		//槍を装備している時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Spear.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Seサウンド
		sound_text = "sound/SoundEffect/spear.mp3";

		break;

	case LONGSWORD:

		//エフェクトの幅の数
		effect_width_num = 5;

		//エフェクトの高さの数 
		effect_height_num = 2;

		//エフェクトの幅のサイズ
		effect_width_size = 192;

		//エフェクトの縦のサイズ
		effect_height_size = 192;

		//エフェクトのフレームの合計
		effect_frame_all_num = effect_width_num * effect_height_num;

		//ロングソードを装備している時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Long_Sword.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Seサウンド
		sound_text = "sound/SoundEffect/long_sword.mp3";

		break;


	default:

		tnl::DebugTrace("---アニメーションを生成できませんでした。---");
		tnl::DebugTrace("---Seを生成できませんでした---");

		break;
	}

}

Nomal_Attack::~Nomal_Attack()
{
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/dageki_3.mp3");
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/zanngeki_2.mp3");
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/dageki.mp3");
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/spear.mp3");
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/long_sword.mp3");
}

//通常攻撃を行った時の処理(ドラクエ式計算式(インフレにも対応できる))
void Nomal_Attack::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto player_level = playerStatus.GetLevel();
	auto weapon_array = GameManager::GetGameManager()->GetInventory()->getEquipArray();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//スケーリングファクターの値を設定する
	//プレイヤーの現在レベルに応じて威力を設定する
	//プレイヤーのレベルが上がった際にそれを実感してもらう為
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//ランダムファクターの値を初期化する
	//ランダム要素を加える事で戦闘にハラハラ感を出す
	initializeRandomFactor();

	// ダメージを計算する（スケーリングファクターとランダムファクターを適用）
	float damage = static_cast<float>(((PlayerAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) - EnemyDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR)) * power) * scaling_factor * random_factor);

	// ダメージが0を下回ったら1にする
	if (damage <= 0) {
		damage = 1;
	}

	//特殊武器かを確認してもし特殊武器だった場合補正をかける
	CheckEpicWeapon(damage, enemyStatus_);

	// 敵のHPを減らす
	enemyStatus_.SetEnemyHp(enemyStatus_.GetEnemyHp() - static_cast<int>(damage));

	// 戦闘ログにダメージ結果を出力
	const std::string log = player_name + "は" + enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	// SEを流す
	SoundManager::GetSoundManager()->Sound_Play(sound_text.c_str(), DX_PLAYTYPE_BACK);

	// ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, sound_text.c_str());

}

//------------------------------------------------------------------------------------------------------------------------------

//火炎斬り
FlameSlash::FlameSlash() : Skill(1, "火炎斬り", 1.5f, "炎の力をまとった攻撃",2 , AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 3;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/FlameSlash.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//火炎斬りを使った時の処理
void FlameSlash::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyFireResist = enemyStatus_.GetFireResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んでいたら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//スケーリングファクターの値を設定する
	//プレイヤーの現在レベルに応じて威力を設定する
	//プレイヤーのレベルが上がった際にそれを実感してもらう為
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//ランダム値を決める
	initializeRandomFactor();

	// 攻撃力から防御力を引いて基本ダメージを計算
	float baseDamage = static_cast<float>(((PlayerAttack - EnemyDefance)* power) * scaling_factor * random_factor);

	// ダメージが負の値にならないようにする
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// 属性攻撃力による補正を加える
	float damage = baseDamage * EnemyFireResist;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//特殊武器かを確認してもし特殊武器だった場合補正をかける
	CheckEpicWeapon(damage, enemyStatus_);

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "は火の力を纏い攻撃した!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/FlameSlash.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/FlameSlash.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//ファイアーストーム

FireStorm::FireStorm():Skill(19, "ファイアーストーム", 1.8f, "炎の渦で攻撃する", 3, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 6;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 3;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/fire_storm.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay,koni::Numeric::SCALE_TRIPLE_F);

}

void FireStorm::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyFireResist = enemyStatus_.GetFireResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//スケーリングファクターの値を設定する
	//プレイヤーの現在レベルに応じて威力を設定する
	//プレイヤーのレベルが上がった際にそれを実感してもらう為
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//ランダム値を決める
	initializeRandomFactor();

	// 基本ダメージの計算
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

	// ダメージが負の値にならないようにする
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// 属性攻撃力による補正を加える
	float damage = baseDamage * EnemyFireResist;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "は炎の渦を作り出した!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/vortex.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/vortex.mp3");
}

//------------------------------------------------------------------------------------------------------------------------------

//インフェルノ

Inferno::Inferno() : Skill(22, "インフェルノ", 2.5f, "炎の業火で攻撃する", 8, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 4;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 5;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/inferno.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_TRIPLE_F);
}

void Inferno::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyFireResist = enemyStatus_.GetFireResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//スケーリングファクターの値を設定する
	//プレイヤーの現在レベルに応じて威力を設定する
	//プレイヤーのレベルが上がった際にそれを実感してもらう為
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//ランダム値を決める
	initializeRandomFactor();

	// 基本ダメージの計算
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

	// ダメージが負の値にならないようにする
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// 属性攻撃力による補正を加える
	float damage = baseDamage * EnemyFireResist;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "が燃え盛る炎を解き放ち、全てを焼き尽くした！";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/Hellfire.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/Hellfire.mp3");
}

//------------------------------------------------------------------------------------------------------------------------------

//アイスブラスト
IceBlast::IceBlast():Skill(2, "アイスブラスト", 1.8f, "氷属性の爆発魔法", 3 , AttackType)
{
	//座標を変える
	effect_pos.y = 360;

	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 2;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	Effect_Animation = std::make_shared<Animation>("graphics/Effect/IceBlast.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_width_size, effect_frame_all_num, effect_delay);
}

//アイスブラストを使った時の計算処理
void IceBlast::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const  Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyIceResist = enemyStatus_.GetIceResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//スケーリングファクターの値を設定する
	//プレイヤーの現在レベルに応じて威力を設定する
	//プレイヤーのレベルが上がった際にそれを実感してもらう為
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//ランダム値を決める
	initializeRandomFactor();

	// 攻撃力から防御力を引いて基本ダメージを計算
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

	// ダメージが負の値にならないようにする
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// 属性攻撃力による補正を加える
	float damage = baseDamage * EnemyIceResist;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "は小さな冷気の塊を作り出し爆発させた!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/ice_blast.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/ice_blast.mp3");
	
}

//------------------------------------------------------------------------------------------------------------------------------

//ウォーターブレード

WaterBlade::WaterBlade() : Skill(23, "ウォーターブレード", 1.5f, "水を纏い攻撃する", 3, AttackType)
{
	//座標を変える
	effect_pos.y = 380;

	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 6;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延秒数を変更する
	effect_delay = 4;

	Effect_Animation = std::make_shared<Animation>("graphics/Effect/weter_blade.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_width_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_QUINTUPLE_F);
}

void WaterBlade::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyIceResist = enemyStatus_.GetIceResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んでいたら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//スケーリングファクターの値を設定する
	//プレイヤーの現在レベルに応じて威力を設定する
	//プレイヤーのレベルが上がった際にそれを実感してもらう為
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//ランダム値を決める
	initializeRandomFactor();

	// 攻撃力から防御力を引いて基本ダメージを計算
	float baseDamage = static_cast<float>(((PlayerAttack - EnemyDefance) * power) * scaling_factor * random_factor);

	// ダメージが負の値にならないようにする
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// 属性攻撃力による補正を加える
	float damage = baseDamage * EnemyIceResist;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//特殊武器かを確認してもし特殊武器だった場合補正をかける
	CheckEpicWeapon(damage, enemyStatus_);

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "は水の力を纏い攻撃した!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/swingdown.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/swingdown.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//ブリザード

Blizzard::Blizzard() : Skill(24, "ブリザード", 2.5f, "とてつもない冷気を作り出し爆発させる", 8, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 2;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延秒数を変更する
	effect_delay = 7;

	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Blizzard.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_width_size, effect_frame_all_num, effect_delay , koni::Numeric::SCALE_DOUBLE_AND_HALF);

}

void Blizzard::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyIceResist = enemyStatus_.GetIceResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//スケーリングファクターの値を設定する
	//プレイヤーの現在レベルに応じて威力を設定する
	//プレイヤーのレベルが上がった際にそれを実感してもらう為
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//ランダム値を決める
	initializeRandomFactor();

	// 基本ダメージの計算
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

	// ダメージが負の値にならないようにする
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// 属性攻撃力による補正を加える
	float damage = baseDamage * EnemyIceResist;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "はとてつもない冷気を作り出し爆発させた！";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/Blizzard.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/Blizzard.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//サンダーボルト
ThunderBolt::ThunderBolt():Skill(3, "サンダーボルト", 1.5f, "雷属性の爆発魔法", 5 , AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 3;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/ThunderBolt.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//サンダーボルト使った時の処理
void ThunderBolt::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyThunderResist = enemyStatus_.GetThunderResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();


	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	//スケーリングファクターの値を設定する
	//プレイヤーの現在レベルに応じて威力を設定する
	//プレイヤーのレベルが上がった際にそれを実感してもらう為
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//ランダム値を決める
	initializeRandomFactor();

	// 攻撃力から防御力を引いて基本ダメージを計算
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

	// ダメージが負の値にならないようにする
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// 属性攻撃力による補正を加える
	float damage = baseDamage * EnemyThunderResist;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "は小さな雷を発生させ、解き放った!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/ThunderBolt.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/ThunderBolt.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//ライトニングスラッシュ

LightningSlash::LightningSlash() : Skill(25, "ライトニングスラッシュ", 2.3f, "雷の力を帯びた斬撃でダメージを与える", 8, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 3;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 640;

	//エフェクトの縦のサイズ
	effect_height_size = 480;

	//遅延秒数を変更する
	effect_delay = 3;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/LightningSlash.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

}

void LightningSlash::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyThunderResist = enemyStatus_.GetThunderResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んでいたら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//スケーリングファクターの値を設定する
	//プレイヤーの現在レベルに応じて威力を設定する
	//プレイヤーのレベルが上がった際にそれを実感してもらう為
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//ランダム値を決める
	initializeRandomFactor();

	// 攻撃力から防御力を引いて基本ダメージを計算
	float baseDamage = static_cast<float>(((PlayerAttack - EnemyDefance) * power) * scaling_factor * random_factor);

	// ダメージが負の値にならないようにする
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// 属性攻撃力による補正を加える
	float damage = baseDamage * EnemyThunderResist;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//特殊武器かを確認してもし特殊武器だった場合補正をかける
	CheckEpicWeapon(damage, enemyStatus_);

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "は雷の力を帯びた斬撃を発生させた!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/sword_2.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/sword_2.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//回復呪文

Heal::Heal(): Skill(4, "ヒール", 0, "Hpを回復する", 2, BuffType)
{
	//座標を決める
	effect_pos = { 160 , 160 };

	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 2;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//エフェクトを生成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Heal.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//ヒールを使った際の処理
void Heal::SkillUse(Player::PlayerStatus& playerStatus, const Shared<BattleLog>& battle_log)
{
	auto max_hp =  playerStatus.GetMaxHp();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//Hpがmaxhpよりも下回っていた場合使用できる
	if (playerStatus.GetcurentHp()< max_hp) {
		// HPを最大Hpの30％回復させる
		float percentage = 0.3f;
		int healAmount = static_cast<int>(max_hp * percentage);

		//Hpを回復する
		playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() + healAmount);

		// HPがMAX_HPを超える場合はMAX_HPにクリップする
		if (playerStatus.GetcurentHp() > max_hp) {
			playerStatus.SetPlayerCurentHp(max_hp);
		}

		//回復音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		//ボリュームを変える
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/ThunderBolt.mp3");


		//バトルログを流す
		battle_log->AddRecoveryLog(player_name, skill_name, healAmount);
	}

}

//------------------------------------------------------------------------------------------------------------------------------

//ハイヒール

HighHeels::HighHeels() :Skill(26, "ハイヒール", 0, "Hpを大回復する", 4, BuffType)
{
	//座標を決める
	effect_pos = { 160 , 160 };

	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 2;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 5;

	//エフェクトを生成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/HighHeels.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

}

void HighHeels::SkillUse(Player::PlayerStatus& playerStatus, const Shared<BattleLog>& battle_log)
{
	auto max_hp = playerStatus.GetMaxHp();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//Hpがmaxhpよりも下回っていた場合使用できる
	if (playerStatus.GetcurentHp() < max_hp) {
		// HPを最大Hpの30％回復させる
		float percentage = 0.5f;
		int healAmount = static_cast<int>(max_hp * percentage);

		//Hpを回復する
		playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() + healAmount);

		// HPがMAX_HPを超える場合はMAX_HPにクリップする
		if (playerStatus.GetcurentHp() > max_hp) {
			playerStatus.SetPlayerCurentHp(max_hp);
		}

		//回復音を鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		//ボリュームを変える
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/ThunderBolt.mp3");


		//バトルログを流す
		battle_log->AddRecoveryLog(player_name, skill_name, healAmount);
	}
}


//---------------------------------------------------------------------------------------------------------
//アイテム系のスキル

//スライムの攻撃
SlimBell::SlimBell() : Skill(20 , "スライムを呼ぶ",5, "スライムの力を借りて攻撃する", 0 , AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 2;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 240;

	//エフェクトの縦のサイズ
	effect_height_size = 240;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/slim.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//スライムの攻撃計算
void SlimBell::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0)return;
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//ダメージを計算する
	auto damage = (power  - enemyStatus_.GetEnemyDefance() / static_cast<float>(koni::Numeric::DIVIDER_TWO));

	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "が鈴をならした。遠くからスライムがやってきた!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);


	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/dageki_2.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/dageki_2.mp3");

}

//アースワーの鈴を使った時の処理
SnakeBell::SnakeBell(): Skill(21 , "アースワームを呼ぶ" , 10 , "アースワームの力を借りて攻撃する", 0 , AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 2;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 240;

	//エフェクトの縦のサイズ
	effect_height_size = 240;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Snake.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num , effect_delay);
}

//スネークの鈴を使った時の処理
void SnakeBell::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0)return;
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//ダメージを計算する
	auto damage = (power  - enemyStatus_.GetEnemyDefance() / static_cast<float>(koni::Numeric::DIVIDER_TWO));

	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	std::string log = player_name + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = player_name + "が鈴をならした。アースワームがやってきて敵に目掛けて飛びついた";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);


}

//---------------------------------------------------------------------------------------------------------
//敵のスキル

//カオスフレア
ChaosFlare::ChaosFlare():Skill(5 , "カオスフレア", 2.3f , "一点集中の火の玉を放ち、命中した敵に大ダメージを与える",0 , AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 4;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 3;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/chaos_flare.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//カオスフレア使った時の処理
void ChaosFlare::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const  Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//プレイヤーが死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	//ランダム値を設定させる
	initializeRandomFactor();

	// 基本ダメージの計算
	float damage =(EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * random_factor;

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString()+ "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は魔力を極限まで高め、放った";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/enemy_chaos_flare.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/enemy_chaos_flare.mp3");
}

//デスウィンド
DeathScytheWind::DeathScytheWind() :Skill(6, "デスウィンド", 2.3f, "強力な風属性の魔法を放つ", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 4;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DeathScytheWind.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay , koni::Numeric::SCALE_TRIPLE_F);
}

//デスサイズウィンド使った時の処理
void DeathScytheWind::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//プレイヤーが死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 基本ダメージの計算
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は暗黒の風を作り出した";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/enemy_death_scythe_wind.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/enemy_death_scythe_wind.mp3");
}

//ダーククロウ
DrakClaw::DrakClaw() : Skill(7 , "ダーククロウ" , 3 , "闇の爪の攻撃",0 ,AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 5;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DrakClaw.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//ダーククロウ使った時の処理
void DrakClaw::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 攻撃力から防御力を引いてダメージを計算
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//プレイヤーのHpを減らす
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は自分の魔力で爪を作り出し、攻撃する";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/enemy_drak_claw.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/enemy_drak_claw.mp3");
}

//生命の風
LifeWind::LifeWind() : Skill(8, "生命の風", 1.2f, "相手を攻撃しHpを少回復する", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 4;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Life_Wind.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//スキル使用時の処理
void LifeWind::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//プレイヤーが死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 基本ダメージの計算
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//与えたダメージの3％のHpを回復させる
	int heal_amoument = static_cast<int>(damage * koni::Numeric::PERCENT_30);

	//敵のHpを取得する
	auto enemy_hp = enemyStatus_.GetEnemyHp();

	//敵のHpを少し回復する
	enemyStatus_.SetEnemyHp(enemy_hp + heal_amoument);

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は突風を作りだし攻撃した";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/life_wind.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/life_wind.mp3");
}

//エナジーブラスト
EnergyBlast::EnergyBlast() : Skill(9, "エナジーブラスト", 1.5f, "魔力を高めてビームを放つ", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 4;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 5;

	//エフェクトのサイズ
	const float EFFECT_SIZE = 3.5f;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/energy_blast.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, EFFECT_SIZE);
}

//エナジーブラストを使った時の処理
void EnergyBlast::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	// プレイヤーが死んでいたら処理をとばす
	if (playerStatus.GetcurentHp() <= 0) return;

	// 基本ダメージ計算にランダム要素を追加
	initializeRandomFactor();

	// 敵の魔力を少しだけダメージに加味し、魔法力の10％を上乗せする
	float magic_boost = EnemyMagicPower * koni::Numeric::PERCENT_10; 

	// 基本ダメージ計算
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power + magic_boost) * BASE_MULTIPLIER * random_factor;

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	// プレイヤーのHPを減らす
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	// バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は魔力を集中させ解き放った";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	// SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/energy_blast.mp3", DX_PLAYTYPE_BACK);

	// ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_50_PERCENT, "sound/SoundEffect/energy_blast.mp3");
}


//ダークチャージ
DarkCharge::DarkCharge() : Skill(10, "ダークチャージ", 2.0f, "闇の玉からエネルギーを抽出し放つ", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 3;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 7;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DarkCharge.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

void DarkCharge::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//プレイヤーが死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 基本ダメージの計算
	float damage =(EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は玉から闇の力を抽出し、取り込んで放出した";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/effect_se_enemy.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/effect_se_enemy.mp3");
}

//闇の波動
WaveDarkness::WaveDarkness(): Skill(11, "闇の波動", 2.5f, "闇の力を増幅させ攻撃する", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 4;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 4;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/wave_drak.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay,koni::Numeric::SCALE_DOUBLE_AND_HALF);
}

void WaveDarkness::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//プレイヤーが死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 攻撃力から防御力を引いて基本ダメージを計算
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は闇の力を増幅させ解き放った";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/effect_se_enemy.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/effect_se_enemy.mp3");
}

//地獄の炎刃
HellFlameBlade::HellFlameBlade():Skill(12, "地獄の炎刃", 2.6f, "地獄の炎を纏い攻撃する", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 4;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 4;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/hell_fire_slach.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_DOUBLE_AND_HALF);

}

void HellFlameBlade::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 攻撃力から防御力を引いてダメージを計算
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//プレイヤーのHpを減らす
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は地獄の炎を纏い攻撃した";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/sword_2.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/sword_2.mp3");

}

//光の斬撃
SlashLight::SlashLight(): Skill(13, "光の斬撃", 1.3f, "光を纏い攻撃する", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 2;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/slash_light.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_TRIPLE_AND_HALF);

}

void SlashLight::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 敵の攻撃力をを少しだけダメージに加味し、攻撃力の10％を上乗せする
	float attack_boost = EnemyAttack * koni::Numeric::PERCENT_10;

	// 攻撃力から防御力を引いてダメージを計算
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power * attack_boost) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//プレイヤーのHpを減らす
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は光を纏い斬撃を発生させた";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/slash_flash.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/slash_flash.mp3");

}

//英雄の力
PowerHeroes::PowerHeroes(): Skill(14, "英雄の力", 1.2f, "力と防御力を増幅させる", 0, BuffType)
{
	//エフェクトの座標を変更する
	effect_pos.y = 500;

	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 4;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 4;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/PowerHeroes.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay , koni::Numeric::SCALE_TRIPLE_AND_HALF);

}

void PowerHeroes::SkillUse(Enemy::EnemyConnection& enemyStatus, const Shared<BattleLog>& battle_log)
{
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵の攻撃力を1.2倍上昇させる
	enemyStatus.SetEnemyAttack(static_cast<int>(enemyStatus.GetEnemyAttack() * power));

	//敵の防御力を1.2倍上昇させる
	enemyStatus.SetEnemyDefance(static_cast<int>(enemyStatus.GetEnemyDefance() * power));

	//バトルログを流す
	std::string log = enemyStatus.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus.GetEnemyString() + "は精神統一して自身の攻撃力と防御力を上昇させた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/status_up.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/status_up.mp3");

}

//光の槍
LightSpear::LightSpear() : Skill(15, "光の槍", 2.0f, "光を槍に変化させ攻撃する", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 3;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 5;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/LightSpear.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_DOUBLE_F);

}


void LightSpear::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 攻撃力から防御力を引いてダメージを計算
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//プレイヤーのHpを減らす
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は光の槍を作り出し攻撃してきた";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/light_spear.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/light_spear.mp3");
}

//死神の鎌
GrimReaperSickle::GrimReaperSickle() : Skill(16, "死神の鎌", 2.0f, "死神の鎌でこうげきする", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 4;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 5;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/GrimReaperSickle.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_TRIPLE_AND_HALF);

}


void GrimReaperSickle::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//敵が死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 攻撃力から防御力を引いてダメージを計算
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//プレイヤーのHpを減らす
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	// 敵の攻撃力を10％上昇させる
	int attack_boost = static_cast<int>(EnemyAttack * koni::Numeric::PERCENT_10);

	//攻撃力を上昇させる
	enemyStatus_.SetEnemyAttack(EnemyAttack + attack_boost);

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は死神の鎌で攻撃してきた！";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/enemy_death_scythe_wind.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/enemy_death_scythe_wind.mp3");

}

//魂の刈り取り
SoulReper::SoulReper(): Skill(17, "魂の刈り取り", 1.8f, "魂を刈り取り少しHPを回復する", 0, AttackType)
{
	//エフェクトの座標を変更する
	effect_pos.y = 500;

	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 3;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 5;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/SoulReper.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_SIXFOLD_F);

}

void SoulReper::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//プレイヤーが死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 攻撃力から防御力を引いて基本ダメージを計算
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//与えたダメージの5％のHpを回復させる
	int heal_amoument = static_cast<int>(damage * koni::Numeric::PERCENT_50);

	//敵のHpを取得する
	auto enemy_hp = enemyStatus_.GetEnemyHp();

	//敵のHpを少し回復する
	enemyStatus_.SetEnemyHp(enemy_hp + heal_amoument);

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は無数の魂をを作り出しぶつけてきた";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/effect_se_enemy.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/effect_se_enemy.mp3");
}

//冥府の光
LightUnderworld::LightUnderworld() : Skill(18, "冥府の光", 1.6f, "冥府の光を浴びせて攻撃する、相手の攻撃力を減少させる", 0, AttackType)
{
	//エフェクトの幅の数
	effect_width_num = 5;

	//エフェクトの高さの数 
	effect_height_num = 3;

	//エフェクトのフレームの合計
	effect_frame_all_num = effect_width_num * effect_height_num;

	//エフェクトの幅のサイズ
	effect_width_size = 192;

	//エフェクトの縦のサイズ
	effect_height_size = 192;

	//遅延時間を変更する
	effect_delay = 5;

	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/LightUnderworld.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_TRIPLE_AND_HALF);

}

void LightUnderworld::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//プレイヤーが死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 基本ダメージの計算
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//プレイヤーのレベルによる軽減率の計算
	PlayerReductionRate(damage);

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	//プレイヤーの攻撃力を10％減少させる
	int player_attack_down_parcentage = static_cast<int>(playerStatus.GetAttack() * koni::Numeric::PERCENT_10);

	//プレイヤーの攻撃力を少し減少させる
	playerStatus.SetPlayerAttack(playerStatus.GetAttack() - player_attack_down_parcentage);

	//バトルログを流す
	std::string log = enemyStatus_.GetEnemyString() + "が" + skill_name + "を使用しました。";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "は冥府の光を呼び出し攻撃してきた";
	battle_log->AddLog(log);
	log = player_name + "の攻撃力が少し減少した";
	battle_log->AddLog(log);
	log = player_name + "に" + std::to_string(static_cast<int>(damage)) + "のダメージを与えた";
	battle_log->AddLog(log);

	//SEを流す
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/effect_se_enemy.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/effect_se_enemy.mp3");
}

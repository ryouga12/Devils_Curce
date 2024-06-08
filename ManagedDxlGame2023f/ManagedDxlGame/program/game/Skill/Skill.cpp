#include "Skill.h"
#include"../Menu/BattleLog.h"

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

//---------------------------------------------------------------------------------------------------------

//通常攻撃
Nomal_Attack::Nomal_Attack(int weapon_type): Skill(0, "攻撃", 1, "通常攻撃",0 , AttackType)
{

	//武器の種類によってアニメーションを変える
	switch (weapon_type)
	{
	case EMPTY:

		//何も装備していない時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack＿Empty.png", 600, 350, 5, 2, 120, 120, 6, 8);

		//Seサウンド
		SoundText = "sound/SoundEffect/dageki_3.mp3";

		break;

	case SWOED:

		//剣を装備している時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Sword.png", 630, 350, 5, 1, 120, 120, 5, 8);

		//Seサウンド
		SoundText = "sound/SoundEffect/zanngeki_2.mp3";

		break;

	case HAMMER:

		//ハンマーを装備している時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Hammer.png", 600, 350, 7, 1, 120, 120, 7, 8);

		//Seサウンド
		SoundText = "sound/SoundEffect/dageki.mp3";

		break;

	case SPEAR:

		//槍を装備している時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Spear.png", 600, 350, 7, 1, 120, 120, 7, 8);

		break;

	default:

		tnl::DebugTrace("---アニメーションを生成できませんでした。---");
		tnl::DebugTrace("---Seを生成できませんでした---");

		break;
	}

}

Nomal_Attack::~Nomal_Attack()
{
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/dageki_3.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/zanngeki_2.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/dageki.mp3");
}

//通常攻撃を行った時の処理(ドラクエ式計算式(インフレにも対応できる))
void Nomal_Attack::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_ ,Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();

	//ダメージを計算する
	int damage = static_cast<int>((PlayerAttack / 2 - EnemyDefance / 4) * Power);

	//ダメージが0を下回ったら1にする
	if (damage <= 0) {
		damage = 1;
	}

	// 敵のHPを減らす
	enemyStatus_.SetEnemyHp(enemyStatus_.GetEnemyHp() - damage);

	// 戦闘ログにダメージ結果を出力
	battle_log->addDamageLog("Player", "Enemy", damage);

	//SEを流す
	SoundManager::getSoundManager()->sound_Play(SoundText.c_str(), DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, SoundText.c_str());

}

//------------------------------------------------------------------------------------------------------------------------------

//火炎斬り
FlameSlash::FlameSlash() : Skill(1, "火炎斬り", 1.5f, "炎の力をまとった攻撃",2 , AttackType)
{
	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/FlameSlash.png", 600, 350, 5, 3, 192, 192, 15, 8);
}

//火炎斬りを使った時の処理
void FlameSlash::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_ , Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyFireResist = enemyStatus_.GetFireResist();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	// 攻撃力から防御力を引いて基本ダメージを計算
	float baseDamage = (PlayerAttack * Power) - EnemyDefance;

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
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/FlameSlash.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/FlameSlash.mp3");

}

//火炎斬りを使った時のMP処理
void FlameSlash::SkillMpConsume(Player::PlayerStatus& playerStatus) {

	//Mpを取得する
	auto CurentMp = playerStatus.GetCurentMp();

	//現在のMpから消費Mpを減らす
	auto consumeMp = CurentMp - ConsumeMp;

	//Mpを消費させる
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//------------------------------------------------------------------------------------------------------------------------------

//アイスブラスト
IceBlast::IceBlast():Skill(2, "アイスブラスト", 2.0f, "氷属性の爆発魔法", 3 , AttackType)
{
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/IceBlast.png", 600, 320, 5, 2, 192, 192, 10, 8);
}

//アイスブラストを使った時の計算処理
void IceBlast::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyIceResist = enemyStatus_.GetIceResist();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	// 攻撃力から防御力を引いて基本ダメージを計算
	float baseDamage = (PlayerMagicPower * Power) - EnemyDefance;

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
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/ice_blast.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/ice_blast.mp3");
	
}

//アイスブラスト使った時のMP処理
void IceBlast::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mpを取得する
	auto CurentMp = playerStatus.GetCurentMp();

	//現在のMpから消費Mpを減らす
	auto consumeMp = CurentMp - ConsumeMp;

	//Mpを消費させる
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//------------------------------------------------------------------------------------------------------------------------------

//サンダーボルト
ThunderBolt::ThunderBolt():Skill(3, "サンダーボルト", 2.5f, "雷属性の爆発魔法", 5 , AttackType)
{
	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/ThunderBolt.png", 600, 350, 5, 3, 192, 192, 15, 8);
}

//サンダーボルト使った時の処理
void ThunderBolt::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyThunderResist = enemyStatus_.GetThunderResist();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	// 攻撃力から防御力を引いて基本ダメージを計算
	float baseDamage = (PlayerMagicPower * Power) - EnemyDefance;

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
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/ThunderBolt.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/ThunderBolt.mp3");

}

//サンダーボルト使った時のMP処理
void ThunderBolt::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mpを取得する
	auto CurentMp = playerStatus.GetCurentMp();

	//現在のMpから消費Mpを減らす
	auto consumeMp = CurentMp - ConsumeMp;

	//Mpを消費させる
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//回復呪文
Heal::Heal(): Skill(4, "ヒール", 0, "Hpを回復する", 2, BuffType)
{
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Heal.png", 160, 160, 5, 2, 192, 192, 10, 8);
}

//ヒールを使った際の処理
void Heal::SkillUse(Player::PlayerStatus& playerStatus, Shared<BattleLog>& battle_log)
{
	auto max_hp =  playerStatus.GetMaxHp();

	//Hpがmaxhpよりも下回っていた場合使用できる
	if (playerStatus.GetcurentHp()< max_hp) {
		// HPを最大Hpの30％回復させる
		float percentage = 0.3;
		int healAmount = static_cast<int>(max_hp * percentage);

		//Hpを回復する
		playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() + healAmount);

		// HPがMAX_HPを超える場合はMAX_HPにクリップする
		if (playerStatus.GetcurentHp() > max_hp) {
			playerStatus.SetPlayerCurentHp(max_hp);
		}

		//回復音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		//ボリュームを変える
		SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/ThunderBolt.mp3");


		//バトルログを流す
		battle_log->addRecoveryLog("Player", SkillName, healAmount);
	}

}

//ヒールを使った時のMP処理
void Heal::SkillMpConsume(Player::PlayerStatus& playerStatus) {

	//Mpを取得する
	auto CurentMp = playerStatus.GetCurentMp();

	//現在のMpから消費Mpを減らす
	auto consumeMp = CurentMp - ConsumeMp;

	//Mpを消費させる
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//---------------------------------------------------------------------------------------------------------
//アイテム系のスキル

//スライムの攻撃
SlimBell::SlimBell() : Skill(20 , "スライムを呼ぶ",5, "スライムの力を借りて攻撃する", 0 , ItemType)
{
	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/slim.png", 600, 350, 5, 2, 240, 240, 6, 8);
}

//スライムの攻撃計算
void SlimBell::SkillUse(Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	//ダメージを計算する
	auto damage = (Power  - enemyStatus_.GetEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/dageki_2.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/dageki_2.mp3");

}

//スネークの鈴を使った時の処理
SnakeBell::SnakeBell(): Skill(21 , "スネークを呼ぶ" , 10 , "スネークの力を借りて攻撃する", 0 , ItemType)
{
	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Snake.png", 600, 350, 5, 2, 240, 240, 8, 8);
}

//スネークの鈴を使った時の処理
void SnakeBell::SkillUse(Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	//敵が死んで居たら処理をとばす
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	//ダメージを計算する
	auto damage = (Power  - enemyStatus_.GetEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//バトルログを流す
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

}

//---------------------------------------------------------------------------------------------------------
//敵のスキル

//カオスフレア
ChaosFlare::ChaosFlare():Skill(5 , "カオスフレア", 3.5f , "一点集中の火の玉を放ち、命中した敵に大ダメージを与える",0 , AttackType)
{
	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/chaos_flare.png", 600, 350, 5, 4, 192, 192, 20, 3);
}

//カオスフレア使った時の処理
void ChaosFlare::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto PlayerDefance = playerStatus.GetDefance();

	//プレイヤーが死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 攻撃力から防御力を引いて基本ダメージを計算
	float damage = (EnemyMagicPower * Power) - PlayerDefance;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - damage);

	//バトルログを流す
	battle_log->addSkillUseLog("Enemy", SkillName, "Player", static_cast<int>(damage));

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/enemy_chaos_flare.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/enemy_chaos_flare.mp3");
}

//デスウィンド
DeathScytheWind::DeathScytheWind() :Skill(6, "デスウィンド", 3.5f, "強力な風属性の魔法を放つ", 0, AttackType)
{
	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DeathScytheWind.png", 600, 350, 5, 4, 192, 192, 20, 8);
}

//デスサイズウィンド使った時の処理
void DeathScytheWind::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto PlayerDefance = playerStatus.GetDefance();

	//プレイヤーが死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 攻撃力から防御力を引いて基本ダメージを計算
	float damage = (EnemyMagicPower * Power) - PlayerDefance;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - damage);

	//バトルログを流す
	battle_log->addSkillUseLog("Enemy", SkillName, "Player", static_cast<int>(damage));

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/enemy_death_scythe_wind.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/enemy_death_scythe_wind.mp3");
}

//ダーククロウ
DrakClaw::DrakClaw() : Skill(7 , "ダーククロウ" , 3 , "闇の爪の攻撃",0 ,AttackType)
{
	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DrakClaw.png", 600, 350, 5, 5, 192, 192, 25, 8);
}

//ダーククロウ使った時の処理
void DrakClaw::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();

	//敵が死んで居たら処理をとばす
	if (playerStatus.GetcurentHp() <= 0)return;

	// 攻撃力から防御力を引いてダメージを計算
	float damage = (EnemyAttack / 2 * Power) - PlayerDefance / 4;

	// ダメージが負の値にならないようにする
	if (damage <= 0) {
		damage = 1;
	}

	//プレイヤーのHpを減らす
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - damage);

	//バトルログを流す
	battle_log->addSkillUseLog("Enemy", SkillName, "Player", static_cast<int>(damage));

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/enemy_drak_claw.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/enemy_drak_claw.mp3");
}

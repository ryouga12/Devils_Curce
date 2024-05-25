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

void Skill::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mpを取得する
	auto CurentMp = playerStatus.getCurentMp();

	//現在のMpから消費Mpを減らす
	auto consumeMp = CurentMp - ConsumeMp;

	//Mpを消費させる
	playerStatus.SetPlayerCurentMp(consumeMp);
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
		Sound = SoundManager::getSoundManager()->LoadSoundBGM("sound/SoundEffect/dageki_3.mp3");

		break;

	case SWOED:

		//剣を装備している時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Sword.png", 630, 350, 5, 1, 120, 120, 5, 8);

		//Seサウンド
		Sound = SoundManager::getSoundManager()->LoadSoundBGM("sound/SoundEffect/zanngeki_2.mp3");

		break;

	case HAMMER:

		//ハンマーを装備している時のアニメーション
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Hammer.png", 600, 350, 7, 1, 120, 120, 7, 8);

		//Seサウンド
		Sound = SoundManager::getSoundManager()->LoadSoundBGM("sound/SoundEffect/dageki.mp3");

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
	auto PlayerAttack = playerStatus.getAttack();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();

	//ダメージを計算する
	int damage = (PlayerAttack / 2 - EnemyDefance / 4) * Power;

	//ダメージが0を下回ったら1にする
	if (damage <= 0) {
		damage = 1;
	}

	// 敵のHPを減らす
	enemyStatus_.SetEnemyHp(enemyStatus_.getEnemyHp() - damage);

	// 戦闘ログにダメージ結果を出力
	battle_log->addDamageLog("Player", "Enemy", damage);
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
	auto PlayerAttack = playerStatus.getAttack();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyFireResist = enemyStatus_.getFireResist();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.getEnemyHp() <= 0)return;

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
	enemyStatus_.SetEnemyHp(enemyStatus_.getEnemyHp() - damage);

	//バトルログを流す
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", damage);

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/FlameSlash.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/FlameSlash.mp3");

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
	auto PlayerMagicPower = playerStatus.getMagicPower();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyIceResist = enemyStatus_.getIceResist();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.getEnemyHp() <= 0)return;

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
	enemyStatus_.SetEnemyHp(enemyStatus_.getEnemyHp() - damage);

	//バトルログを流す
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", damage);

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/ice_blast.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/ice_blast.mp3");
	
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
	auto PlayerMagicPower = playerStatus.getMagicPower();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyThunderResist = enemyStatus_.getThunderResist();

	//敵が死んで居たら処理をとばす
	if (enemyStatus_.getEnemyHp() <= 0)return;

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
	enemyStatus_.SetEnemyHp(enemyStatus_.getEnemyHp() - damage);

	//バトルログを流す
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", damage);

	//SEを流す
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/ThunderBolt.mp3", DX_PLAYTYPE_BACK);

	//ボリュームを変える
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/ThunderBolt.mp3");

}

//回復呪文
Heal::Heal(): Skill(4, "ヒール", 0, "Hpを回復する", 2, BuffType)
{
}

//ヒールを使った際の処理
void Heal::SkillUse(Player::PlayerStatus& playerStatus, Shared<BattleLog>& battle_log)
{
	auto max_hp =  playerStatus.getMaxHp();

	//Hpがmaxhpよりも下回っていた場合使用できる
	if (playerStatus.getcurentHp()< max_hp) {
		// HPを最大Hpの30％回復させる
		float percentage = 0.3;
		int healAmount = static_cast<int>(max_hp * percentage);

		//Hpを回復する
		playerStatus.SetPlayerCurentHp(playerStatus.getcurentHp() + healAmount);

		// HPがMAX_HPを超える場合はMAX_HPにクリップする
		if (playerStatus.getcurentHp() > max_hp) {
			playerStatus.SetPlayerCurentHp(max_hp);
		}

		//回復音を鳴らす
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		//バトルログを流す
		battle_log->addRecoveryLog("Player", SkillName, healAmount);
	}
	
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
	if (enemyStatus_.getEnemyHp() <= 0)return;

	//ダメージを計算する
	auto damage = (Power  - enemyStatus_.getEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(enemyStatus_.getEnemyHp() - damage);

	//バトルログを流す
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", damage);

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
	if (enemyStatus_.getEnemyHp() <= 0)return;

	//ダメージを計算する
	auto damage = (Power  - enemyStatus_.getEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	//敵のHpを減らす
	enemyStatus_.SetEnemyHp(enemyStatus_.getEnemyHp() - damage);

	//バトルログを流す
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", damage);

}
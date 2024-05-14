#include "Skill.h"


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
int Nomal_Attack::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerAttack = playerStatus.getAttack();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();

	//ダメージを計算する
	int damage = (PlayerAttack / 2 - EnemyDefance / 4) * Power;

	//ダメージが0を下回ったら1にする
	if (damage <= 0) {
		damage = 1;
	}

	return damage;
}

//通常攻撃を行った時のアニメーション
void Nomal_Attack::SkillUseAnimation()
{
	//アニメーションの流す
	Effect_Animation->play_animation();

	//音を鳴らす
	PlaySoundMem(Sound, DX_PLAYTYPE_BACK);

}

//通常攻撃のアニメーションを止める
void Nomal_Attack::SkillAnimationStop()
{
	//通常攻撃のアニメーションを止める
	Effect_Animation->stop_animation();
}

//通常攻撃のアニメーションの描画
void Nomal_Attack::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//通常攻撃をした際のアニメーションの更新処理
void Nomal_Attack::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//------------------------------------------------------------------------------------------------------------------------------

//火炎斬り
FlameSlash::FlameSlash() : Skill(1, "火炎斬り", 1.5f, "炎の力をまとった攻撃",2 , AttackType)
{
	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/FlameSlash.png", 600, 350, 5, 3, 192, 192, 15, 8);
}

//火炎斬りを使った時の処理
int FlameSlash::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerAttack = playerStatus.getAttack();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyFireResist = enemyStatus_.getFireResist();

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

	return static_cast<int>(damage);
}

//火炎斬りを使った際のアニメーション処理
void FlameSlash::SkillUseAnimation()
{
	Effect_Animation->play_animation();

	//サウンド
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/FlameSlash.mp3", DX_PLAYTYPE_BACK);
	//音量を変える(音量 , ghpath)
	SoundManager::getSoundManager()->ChangeSoundVolume(100, "sound/SoundEffect/FlameSlash.mp3");
}

//火炎斬りのアニメーションを止める
void FlameSlash::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//火炎斬り使った際のアニメーションの描画
void FlameSlash::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//火炎斬りを使った際のアニメーションの更新処理
void FlameSlash::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//火炎斬りを使った時のMp処理
void FlameSlash::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mpを取得する
	auto CurentMp = playerStatus.getCurentMp();

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
int IceBlast::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerMagicPower = playerStatus.getMagicPower();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyIceResist = enemyStatus_.getIceResist();

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

	return static_cast<int>(damage);
	
}

//アイスブラスト使った際のアニメーション処理
void IceBlast::SkillUseAnimation()
{
	Effect_Animation->play_animation();
}

void IceBlast::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//アイスブラストアニメーションを止める
void IceBlast::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//アイスブラスト使った際のアニメーションの更新処理
void IceBlast::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//アイスブラスト使った時のMp処理
void IceBlast::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mpを取得する
	auto CurentMp = playerStatus.getCurentMp();

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
int ThunderBolt::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerMagicPower = playerStatus.getMagicPower();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyThunderResist = enemyStatus_.getThunderResist();

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

	return static_cast<int>(damage);
}

//サンダーボルト行った時のアニメーション
void ThunderBolt::SkillUseAnimation()
{
	Effect_Animation->play_animation();

	//サウンド
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/dengeki.mp3", DX_PLAYTYPE_BACK);
	//音量を変える(音量 , ghpath)
	SoundManager::getSoundManager()->ChangeSoundVolume(50, "sound/SoundEffect/dengeki.mp3");
}

//サンダーボルト使った際のアニメーションを止める
void ThunderBolt::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//サンダーボルト使った際のアニメーションの描画
void ThunderBolt::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//サンダーボルト使った際のアニメーションの更新処理
void ThunderBolt::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//サンダーボルト使った時のMp処理
void ThunderBolt::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mpを取得する
	auto CurentMp = playerStatus.getCurentMp();

	//現在のMpから消費Mpを減らす
	auto consumeMp = CurentMp - ConsumeMp;

	//Mpを消費させる
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//回復呪文
Heal::Heal(): Skill(4, "ヒール", 0, "Hpを回復する", 2, BuffType)
{
}

//ヒールを使った際の処理
int Heal::SkillUse(Player::PlayerStatus& playerStatus)
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

		return healAmount;
	}
	
	//下回ってなかったら使用できない
	return 0;
}

//ヒールを使った時のMp処理
void Heal::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mpを取得する
	auto CurentMp = playerStatus.getCurentMp();

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
int SlimBell::SkillUse(Enemy::EnemyStatus& enemyStatus_)
{
	//ダメージを計算する
	auto damage = (Power  - enemyStatus_.getEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	return static_cast<int>(damage);
}

//スライムの鈴を使った時のアニメーション
void SlimBell::SkillUseAnimation()
{
	Effect_Animation->play_animation();
}

//スライムの鈴使った際のアニメーションを止める
void SlimBell::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//スライムの使った際のアニメーションの描画
void SlimBell::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//スライムの使った際のアニメーションの更新処理
void SlimBell::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

SnakeBell::SnakeBell(): Skill(21 , "スネークを呼ぶ" , 10 , "スネークの力を借りて攻撃する", 0 , ItemType)
{
	//アニメーションを作成する
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Snake.png", 600, 350, 5, 2, 240, 240, 8, 8);
}

//スネークの鈴を使った時の処理
int SnakeBell::SkillUse(Enemy::EnemyStatus& enemyStatus_)
{
	//ダメージを計算する
	auto damage = (Power  - enemyStatus_.getEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	return static_cast<int>(damage);
}

//スネークの鈴を使った時のアニメーション
void SnakeBell::SkillUseAnimation()
{
	Effect_Animation->play_animation();
}

//スネークの使った際のアニメーションを止める
void SnakeBell::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//スネークの使った際のアニメーションの描画
void SnakeBell::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//スネークの使った際のアニメーションの更新処理
void SnakeBell::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

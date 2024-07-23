#pragma once
#include"BaseSkill.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include<unordered_map>
#include"../../game/Object/Player.h"
#include"../../game/Object/Enemy.h"
#include"../../game/Effect/animation.h"
#include"../Manager/SoundManager.h"

class BattleLog;

//攻撃系
static const int AttackType = 0;
//バフ系
static const int BuffType = 1;
//アイテム系
static const int ItemType = 2;


class Skill : public BaseSkill {
public:

	Skill() {}
	//(ID , 名前 , 攻撃力 , 特技の説明、特技の消費Mp、スキルのタイプ) を初期化する
	Skill(int _id, const std::string _name, float _power, const std::string& _description, int consumeMp, int skilltype); 
	virtual~Skill(){}

	//攻撃スキルを使った時のダメージ計算
	virtual void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log) {}

	//バフスキルを使った時の処理
	virtual void SkillUse(Player::PlayerStatus& playerStatus, Shared<BattleLog>& battle_log) {};

	//アイテム系を使った時の処理
	virtual void SkillUse(Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log) {};


	//スキルを使った際のアニメーションの再生
	virtual void SkillUseAnimation();

	//スキルを使った際のアニメーションを止める
	virtual void SkillAnimationStop();

	//スキルを使った際のアニメーションの描画
	virtual void SkillAnimationDraw();

	//スキルを使った際のアニメーションの更新処理
	virtual void SkillAnimationUpdate(float delta_time);

	//スキルを使った時のMp処理
	virtual void SkillMpConsume(Player::PlayerStatus& playerStatus) {};
	
	//Idを取得する
	int getId() const { return Id; }
	//スキルの名前を取得する
	std::string getName() const { return SkillName; }
	//スキルの詳細を取得する
	std::string getDescription() const { return description; }
	//スキルのダメージを取得する
	float getSkillPower() { return Power; }
	//スキルの必要Mpを取得する
	int getSkillConsumeMp()const { return ConsumeMp; }
	//スキルのタイプを取得する
	int getSkillType()const{return SkillType;}

	//全体のスキルを格納する
	void AddSkillList();

	//全体のスキルリストを取得する
	std::vector<Shared<Skill>>& GetOverallSkills() {
		return skill_list;
	}

private:

	//全体のスキルリスト
	//主にスキルのポインタのセーブロードに使用
	std::vector<Shared<Skill>>skill_list;

protected:

	//エフェクト用のアニメーション
	Shared<Animation>Effect_Animation = nullptr;
	
};

//通常攻撃
class Nomal_Attack final  : public Skill {
public:

	//通常攻撃
	Nomal_Attack(int weapon_type );
	~Nomal_Attack() override;

	//通常攻撃を行った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , Shared<BattleLog>& battle_log)override;

private:

	//武器によって通常攻撃のエフェクトを変える
	enum {
		EMPTY,
		SWOED,
		HAMMER,
		SPEAR
	};

	std::string SoundText;

};

//火炎斬り
class FlameSlash final  : public Skill {
public:

	FlameSlash();
	~FlameSlash()override{}

	//火炎斬りを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , Shared<BattleLog>& battle_log)override;

	//火炎斬りを使った時のMP処理
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;

};

//アイスブラスト
class IceBlast final : public Skill {
public:

	IceBlast();
	~IceBlast()override {}

	//アイスブラスト使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log)override;

	//アイスブラスト使った時のMP処理
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;
};

//サンダーボルト
class ThunderBolt final : public Skill {
public:

	ThunderBolt();
	~ThunderBolt()override {}

	//サンダーボルト使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , Shared<BattleLog>& battle_log)override;

	//サンダーボルト使った時のMP処理
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;
};

//回復呪文
class Heal final : public Skill {

public:

	Heal();
	~Heal()override {}

	//ヒールを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus , Shared<BattleLog>& battle_log)override;

	//ヒールを使った時のMP処理
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;
};


//---------------------------------------------------------------------------------------------------------
//アイテム系のスキル

//スライムの鈴
class SlimBell final : public Skill {
	
public:

	SlimBell();
	~SlimBell()override {};

	//スライムの鈴を使った時の処理
	void SkillUse(Enemy::EnemyConnection& enemyStatus_ , Shared<BattleLog>& battle_log)override;

};

//スネークの鈴[
class SnakeBell final : public Skill {
public:

	SnakeBell();
	~SnakeBell()override {}

	//スネークの鈴を使った時の処理
	void SkillUse(Enemy::EnemyConnection& enemyStatus_ ,Shared<BattleLog>& battle_log)override;

};

//---------------------------------------------------------------------------------------------------------
//敵のスキル

//カオスフレア
//強力な火属性魔法
class ChaosFlare : public Skill {
public:

	ChaosFlare();
	~ChaosFlare()override {};

	//カオスフレアを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log)override;

};

//デスサイズウィンド
//強力な風
class DeathScytheWind : public Skill {
public:

	DeathScytheWind();
	~DeathScytheWind()override {};

	//デスサイズウィンドを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log)override;

};

//ダーククロウ
class DrakClaw : public Skill {
public:

	DrakClaw();
	~DrakClaw()override {};

	//ダーククロウ使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log)override;

};

//
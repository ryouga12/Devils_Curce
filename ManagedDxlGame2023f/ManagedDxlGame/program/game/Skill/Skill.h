#pragma once
#include"BaseSkill.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include<unordered_map>
#include"../../game/Object/Player.h"
#include"../../game/Object/Enemy.h"
#include"../../game/Manager/animation.h"
#include"../Manager/SoundManager.h"

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
	Skill(int _id , const std::string _name ,float _power , const std::string& _description, int consumeMp , int skilltype) :BaseSkill(_id ,_name, _power,_description ,consumeMp, skilltype) {}
	virtual~Skill(){}

	//攻撃スキルを使った時のダメージ計算
	virtual int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_) { return 0; }

	//バフスキルを使った時の処理
	virtual int SkillUse(Player::PlayerStatus& playerStatus) { return 0; };

	//アイテム系を使った時の処理
	virtual int SkillUse(Enemy::EnemyStatus& enemyStatus_) { return 0; };


	//スキルを使った際のアニメーションの再生
	virtual void SkillUseAnimation() {}

	//スキルを使った際のアニメーションを止める
	virtual void SkillAnimationStop() {};

	//スキルを使った際のアニメーションの描画
	virtual void SkillAnimationDraw() {};

	//スキルを使った際のアニメーションの更新処理
	virtual void SkillAnimationUpdate(float delta_time) {}

	//スキルを使った時のMp処理
	virtual void SkillMpConsume(Player::PlayerStatus& playerStatus) {}
	
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
	int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//通常攻撃を行った時のアニメーション
	void SkillUseAnimation()override;

	//スキルを使った際のアニメーションを止める
	void SkillAnimationStop()override;

	//スキルを使った際のアニメーションの描画
	void SkillAnimationDraw()override;

	//スキルを使った際のアニメーションの更新処理
	void SkillAnimationUpdate(float delta_time)override;

private:

	//武器によって通常攻撃のエフェクトを変える
	enum {
		EMPTY,
		SWOED,
		HAMMER,
		SPEAR
	};

	int Sound = 0;

};

//火炎斬り
class FlameSlash final  : public Skill {
public:

	FlameSlash();
	~FlameSlash()override{}

	//火炎斬りを使った時の処理
	int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//火炎斬りを行った時のアニメーション
	void SkillUseAnimation()override;

	//火炎斬りを使った際のアニメーションを止める
	void SkillAnimationStop()override;

	//火炎斬りを使った際のアニメーションの描画
	void SkillAnimationDraw()override;

	//火炎斬りを使った際のアニメーションの更新処理
	void SkillAnimationUpdate(float delta_time)override;

	//火炎斬りを使った時のMp処理
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;

};

//アイスブラスト
class IceBlast final : public Skill {
public:

	IceBlast();
	~IceBlast()override {}

	//アイスブラスト使った時の処理
	int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//アイスブラスト行った時のアニメーション
	void SkillUseAnimation()override;

	//アイスブラスト使った際のアニメーションを止める
	void SkillAnimationStop()override;

	//アイスブラスト使った際のアニメーションの描画
	void SkillAnimationDraw()override;

	//アイスブラスト使った際のアニメーションの更新処理
	void SkillAnimationUpdate(float delta_time)override;

	//アイスブラスト使った時のMp処理
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;

};

//サンダーボルト
class ThunderBolt final : public Skill {
public:

	ThunderBolt();
	~ThunderBolt()override {}

	//サンダーボルト使った時の処理
	int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//サンダーボルト行った時のアニメーション
	void SkillUseAnimation()override;

	//サンダーボルト使った際のアニメーションを止める
	void SkillAnimationStop()override;

	//サンダーボルト使った際のアニメーションの描画
	void SkillAnimationDraw()override;

	//サンダーボルト使った際のアニメーションの更新処理
	void SkillAnimationUpdate(float delta_time)override;

	//サンダーボルト使った時のMp処理
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;

};

//回復呪文
class Heal final : public Skill {

public:

	Heal();
	~Heal()override {}

	//ヒールを使った時の処理
	int SkillUse(Player::PlayerStatus& playerStatus)override;

	//ヒール使った時のMp処理
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
	int SkillUse(Enemy::EnemyStatus& enemyStatus_)override;
	
	//スライムの鈴を使った時のアニメーション
	void SkillUseAnimation()override;

	//スライムの使った際のアニメーションを止める
	void SkillAnimationStop()override;

	//スライムの使った際のアニメーションの描画
	void SkillAnimationDraw()override;

	//スライムの使った際のアニメーションの更新処理
	void SkillAnimationUpdate(float delta_time)override;
};

//スネークの鈴[
class SnakeBell final : public Skill {
public:

	SnakeBell();
	~SnakeBell()override {}

	//スネークの鈴を使った時の処理
	int SkillUse(Enemy::EnemyStatus& enemyStatus_)override;

	//スネークの鈴を使った時のアニメーション
	void SkillUseAnimation()override;

	//スネークの使った際のアニメーションを止める
	void SkillAnimationStop()override;

	//スネークの使った際のアニメーションの描画
	void SkillAnimationDraw()override;

	//スネークの使った際のアニメーションの更新処理
	void SkillAnimationUpdate(float delta_time)override;

};

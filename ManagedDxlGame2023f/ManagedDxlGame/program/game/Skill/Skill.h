//------------------------------------------------------------------------------------------------------------
//
//スキルクラス
//
//-------------------------------------------------------------------------------------------------------------


#pragma once
#include"BaseSkill.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include<unordered_map>
#include"../../game/Object/Player.h"
#include"../../game/Object/Enemy.h"
#include"../../game/Effect/animation.h"
#include"../Manager/SoundManager.h"
#include"../../koni_name_space/common/common_value.h"

class BattleLog;

//攻撃系
static const int AttackType = 0;
//バフ系
static const int BuffType = 1;

class Skill : public BaseSkill {
public:

	Skill() = default;

	//スキルクラスのコンストラクタ
	//arg_1 : ID 
	//arg_2 : 名前 
	//arg_3 : 攻撃力 
	//arg_4 : 特技の説明
	//arg_5 : 特技の消費Mp
	//arg_6 : スキルのタイプ
	Skill(const int _id, const std::string& _name, const float _power, const std::string& _description, const int consumeMp, const int skilltype); 
	virtual~Skill();

	//攻撃スキルを使った時のダメージ計算
	//arg_1 : プレイヤーのステータス
	//arg_2 : エネミーのステータス
	//arg_3  : バトルログのスマートポインタ
	virtual void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log) {}

	//バフスキルを使った時の処理(プレイヤー)
	//arg_1  : プレイヤーのステータス
	//arg_2  : バトルログのスマートポインタ
	virtual void SkillUse(Player::PlayerStatus& playerStatus, const Shared<BattleLog>& battle_log) {};

	//敵のバフ系のスキル
	//arg_1  : エネミーのステータス
	//arg_2  : バトルログのスマートポインタ
	virtual void SkillUse(Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log) {};


	//スキルを使った際のアニメーションの再生
	virtual void SkillUseAnimation();

	//スキルを使った際のアニメーションを止める
	virtual void SkillAnimationStop();

	//スキルを使った際のアニメーションの描画
	virtual void SkillAnimationDraw();

	//スキルを使った際のアニメーションの更新処理
	virtual void SkillAnimationUpdate(float delta_time);

	//スキルを使った時のMp処理
	virtual void SkillMpConsume(Player::PlayerStatus& playerStatus);
	
	//Idを取得する
	int getId() const { return skill_id; }
	//スキルの名前を取得する
	std::string getName() const { return skill_name; }
	//スキルの詳細を取得する
	std::string getDescription() const { return description; }
	//スキルのダメージを取得する
	float getSkillPower() { return power; }
	//スキルの必要Mpを取得する
	int getSkillConsumeMp()const { return consume_mp; }
	//スキルのタイプを取得する
	int getSkillType()const{return skill_type;}

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

	// スケーリングファクターを定義
	float scaling_factor = 0;

	// 基本ダメージの倍率
	const float BASE_MULTIPLIER = 0.8f;

	// ランダムファクター
	float random_factor = 0;

	//ランダムファクターの値を決める
	void initializeRandomFactor();

	//プレイヤーのレベルによる軽減率の計算
	void PlayerReductionRate(float& damage);

	//武器が特殊武器かを確認する
	void CheckEpicWeapon(float& damage , Enemy::EnemyConnection& enemyStatus_);

	//マックスレベル
	const int MAX_LEVEL = 20;

	//ベースの軽減率
	const float BASE_REDUCTION_RATE = 0.035f;

	//---アニメーションを初期化する際の変数---//
	//今回はわかりやすいようにコンストラクタ内で初期化する
	
	//エフェクトの座標
	//デフォルトは x_600 , y_350
	//変えたい場合コンストラクタで切り替える
	tnl::Vector2i effect_pos = { 600 , 350 };
	
	//エフェクトの幅の数
	int effect_width_num = 0;
	//エフェクトの高さの数 
	int effect_height_num = 0;
	//エフェクトの幅のサイズ
	int effect_width_size = 0;
	//エフェクトの縦のサイズ
	int effect_height_size = 0;
	//エフェクトのフレームの総数
	int effect_frame_all_num = 0;
	//エフェクトの遅延秒数
	//通常は8秒に設定(技によって切り替える)
	int effect_delay = 8;

};

//通常攻撃
class Nomal_Attack final  : public Skill {
public:

	//通常攻撃
	Nomal_Attack(int weapon_type );
	~Nomal_Attack() override;

	//通常攻撃を行った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)override;

private:

	//武器によって通常攻撃のエフェクトを変える
	enum {
		EMPTY,
		SWOED,
		HAMMER,
		SPEAR,
		LONGSWORD
	};

	//SE
	std::string sound_text;

};

//火炎斬り
class FlameSlash final  : public Skill {
public:

	FlameSlash();
	~FlameSlash()override{}

	//火炎斬りを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)override;

	
};

//ファイアストーム
class FireStorm : public Skill {
public:

	FireStorm();
	~FireStorm()override {};

	//ファイアストームを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//インフェルノ
class Inferno : public Skill {
public:

	Inferno();
	~Inferno()override {};

	//インフェルノを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//アイスブラスト
class IceBlast final : public Skill {
public:

	IceBlast();
	~IceBlast()override {}

	//アイスブラスト使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//ウォーターブレード
class WaterBlade : public Skill {
public:

	WaterBlade();
	~WaterBlade()override {};

	//ウォーターブレードを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;
};

//プリザード
class Blizzard : public Skill {
public:

	Blizzard();
	~Blizzard()override {};

	//プリザードを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};


//サンダーボルト
class ThunderBolt final : public Skill {
public:

	ThunderBolt();
	~ThunderBolt()override {}

	//サンダーボルト使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)override;

};

//ライトニングスラッシュ
class LightningSlash : public Skill {
public:

	LightningSlash();
	~LightningSlash()override {};

	//ライトニングスラッシュを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//回復呪文

//ヒール
class Heal final : public Skill {

public:

	Heal();
	~Heal()override {}

	//ヒールを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus , const Shared<BattleLog>& battle_log)override;
};

//ハイヒール
class HighHeels : public Skill {
public:

	HighHeels();
	~HighHeels()override {};

	//ハイヒール使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, const Shared<BattleLog>& battle_log)override;

};


//---------------------------------------------------------------------------------------------------------
//アイテム系のスキル

//スライムの鈴
class SlimBell final : public Skill {
	
public:

	SlimBell();
	~SlimBell()override {};

	//スライムの鈴を使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//スネークの鈴[
class SnakeBell final : public Skill {
public:

	SnakeBell();
	~SnakeBell()override {}

	//スネークの鈴を使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//---------------------------------------------------------------------------------------------------------
//敵のスキル

//カオスフレア
//強力な火属性魔法
class ChaosFlare final : public Skill {
public:

	ChaosFlare();
	~ChaosFlare()override {};

	//カオスフレアを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//デスサイズウィンド
//強力な風
class DeathScytheWind final : public Skill {
public:

	DeathScytheWind();
	~DeathScytheWind()override {};

	//デスサイズウィンドを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//ダーククロウ
class DrakClaw final : public Skill {
public:

	DrakClaw();
	~DrakClaw()override {};

	//ダーククロウ使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//闇の波動
class WaveDarkness : public Skill {
public:

	WaveDarkness();
	~WaveDarkness()override {};
		
	//闇の波動を使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//地獄の炎刃
class HellFlameBlade : public Skill {
public:
	
	HellFlameBlade();
	~HellFlameBlade() override {};

	//地獄の炎刃を使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};


//生命の風
class LifeWind final : public Skill {
public:

	LifeWind();
	~LifeWind()override {};

	//生命の風を使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//エナジーブラスト
class EnergyBlast final : public Skill {
public:

	EnergyBlast();
	~EnergyBlast()override {};

	//エナジーブラストを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//ダークチャージ
class DarkCharge final : public Skill {
public:

	DarkCharge();
	~DarkCharge()override {};

	//ダークチャージ使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//光の斬撃
class SlashLight : public Skill {
public:

	SlashLight();
	~SlashLight()override {};

	//光の斬撃を使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//英雄の力
class PowerHeroes : public Skill {
public:

	PowerHeroes();
	~PowerHeroes()override {};

	//英雄の力を使った時の処理
	void SkillUse(Enemy::EnemyConnection& enemyStatus, const Shared<BattleLog>& battle_log)override;

};

//光の槍
class LightSpear : public Skill {
public:

	LightSpear();
	~LightSpear()override {};

	//光の槍を使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//死神の鎌
class GrimReaperSickle : public Skill {
public:

	GrimReaperSickle();
	~GrimReaperSickle()override {};

	//死神の鎌を使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//魂の刈り取り
class SoulReper : public Skill {
public:

	SoulReper();
	~SoulReper()override {};

	//魂の刈り取りを使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//冥府の光
class LightUnderworld : public Skill {
public:

	LightUnderworld();
	~LightUnderworld()override {};


	//冥府の光を使った時の処理
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};
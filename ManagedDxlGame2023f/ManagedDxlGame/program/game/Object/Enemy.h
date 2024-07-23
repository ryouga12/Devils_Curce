#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Item/Item.h"
#include"Actor.h"

class ItemBase;
class BattleLog;
class GameManager;
class SoundManager;
class SceneManager;
class CsvManager;
class Player;

class Enemy : public Actor{
public:

	Enemy();

	//描画処理
	void Draw();

	//Enemyのステータス
	struct EnemyConnection
	{
	public:

		//nameを取得する
		std::string GetEnemyString() const {
			return name;
		}
		//IDを取得する
		int GetEnemyId() const {
			return id;
		}
		//Hpを取得する
		int GetEnemyHp() const{
			return hp;
		}
		//Attackを取得する
		int GetEnemyAttack() const {
			return Attack;
		}
		//Defanceを取得する
		int GetEnemyDefance() const {
			return Defance;
		}
		//Speedを取得する
		int GetEnemySpeed() const {
			return Speed;
		}
		//経験値を取得する
		int GetEnemyExpoint() const {
			return ex_point;
		}
		//moneyを取得する
		int GetEnemyMoney() const {
			return money;
		}
		//ghdlを取得する
		const std::string& GetEnemyGhdl()const {
			return gh;
		}
		//それぞれの耐性を取得する
		float GetFireResist() const {
			return FireResist;
		}
		float GetIceResist() const {
			return IceResist;
		}
		float GetThunderResist() const {
			return ThunderResist;
		}
		//通常ドロップのアイテムのIDを取得する
		int GetNomalDrop()const {
			return Nomal_Drop;
		}
		//レアドロップのアイテムIDを取得する
		int GetRareDrop()const {
			return RareDrop;
		}
		//seのサウンドを取得する
		std::string GetSeAttack()const {
			return attack_se;
		}
		//魔法力を取得する
		int GetMagicPower()const {
			return magic_power;
		}
		//ノーマルドロップの確率を取得する
		int GetNomalProbability()const {
			return nomal_drop_probability;
		}
		//レアドロップの確率を取得する
		int GetRareProbability()const {
			return rare_drop_probability;
		}

		//---セッター---//

		//敵の名前をセットする
		void SetEnemyName(std::string newname) {
			name = newname;
		}
		//敵のIDをセットする
		void SetEnemyId(int newid){
			id = newid;
		}
		//敵のHpをセットする
		void SetEnemyHp(int newhp) {
			hp = newhp;
		}
		//敵のAttackをセットする
		void SetEnemyAttack(int newattack) {
			Attack = newattack;
		}
		//敵のDefanceをセットする
		void SetEnemyDefance(int newdefance) {
			Defance = newdefance;
		}
		//敵のSpeedをセットする
		void SetEnemySpeed(int newspeed) {
			Speed = newspeed;
		}
		//敵の経験値をセットする
		void SetEnemyExpoint(int newexpoint) {
			ex_point = newexpoint;
		}
		//敵のmoneyをセットする
		void SetEnemyMoney(int newmoney) {
			money = newmoney;
		}
		//敵のハンドルをセットする
		void SetGhdl(std::string newgh) {
			gh = newgh;
		}
		//敵の耐性をセットする
		void SetFireResist(float newresist) {
			FireResist = newresist;
		}
		void SetIceResist(float newresist) {
			IceResist = newresist;
		}
		void SetThunderResist(float newresist) {
			ThunderResist = newresist;
		}
		//敵の通常ドロップのIDをセットする
		void SetNomalDropID(int newID) {
			Nomal_Drop = newID;
		}
		//敵のレアドロップのIDをセットする
		void SetRareDoropID(int newID) {
			RareDrop = newID;
		}
		//攻撃時のSEをセットする
		void SetAttackSe(const std::string& new_se) {
			attack_se = new_se;
		}
		//魔法力をセットする
		void SetMagicPower(int new_magic_power) {
			magic_power = new_magic_power;
		}
		//ノーマルドロップアイテムの確率をセットする
		void SetNomalDropProbability(int new_nomal_probability) {
			nomal_drop_probability = new_nomal_probability;
		}
		//レアドロップアイテムの確率をセットする
		void SetRareDropProbability(int new_rare_probability) {
			rare_drop_probability = new_rare_probability;
		}

	private:

		//名前
		std::string name;
		//ID
		int id = 0;
		//体力
		int hp = 0;
		//攻撃力
		int Attack = 0;
		//防御力
		int Defance = 0;
		//素早さ
		int Speed = 0;
		//得られる経験値
		int ex_point = 0;
		//得られるお金
		int money = 0;
		//画像
		std::string gh;
		//炎耐性
		float FireResist = 0;
		//氷耐性
		float IceResist = 0;
		//雷耐性
		float ThunderResist = 0;
		//ノーマルドロップアイテム
		int Nomal_Drop = 0;
		//レアドロップアイテム
		int RareDrop = 0;
		//攻撃時のSE
		std::string attack_se;
		//魔法力
		int magic_power = 0;
		//ノーマルドロップアイテムのドロップ率
		int nomal_drop_probability = 0;
		//レアドロップアイテムのドロップ率
		int rare_drop_probability = 0;
	};

	//Enemyのステータスを読み込む為の関数
	void EnemyLoadTypeInfo();

	//敵の初期化
	void EnemyInit();

	//敵ののステータスのidを取得する
	EnemyConnection GetEnemyStatus(int id) const;
	
	//敵の配列を受け取る為のゲッター
	std::vector<EnemyConnection>& GetEnemyArray(){
		return enemy_array;

		//敵の配列を取得できなかったらエラー用の配列を返す
		if (enemy_array.size() == 0) {
			return null_array;
		}
	}

	//敵のIDによってドロップのアイテムを切り替える
	void InitEnemyItemDrop(int EnemyID);

	//敵が所持しているアイテムの配列を取得する
	std::vector<ItemBase>& getItemDropArray() {
		return enemy_drop_item;
	}

	//敵の情報を配列に格納する
	void InitEnemyArray(int id);

	//敵の攻撃処理(主に子クラスで定義する)
	virtual void EnemyAction(Shared<BattleLog>battle_log) {};

	//敵の死亡処理
	virtual bool ChackDeadEnemy();

	//敵のインデックスを取得する
	int GetEnemy_Index()const {
		return enemy_index;
	}

	//フラグ立てる(画像を消す)
	void DeadEnemyFlag() {
		dead_enemy_flag = true;
	}

	//敵の種類
	enum class Enemytype {
		NONE,
		//雑魚敵
		MOB,
		//ボス敵
		BOSS
	};

	//敵のタイプをセットする
	void SetEnemyType(Enemytype enemyType) {
		enemy_type = enemyType;
	}

	//敵のタイプを取得する
	Enemytype GetEnemyType()const {
		return enemy_type;
	}

private:

	//Enemyのステータスを入れておく変数
	EnemyConnection Enemy_Status_Type;

	//構造体の総数
	int EnemyStatus_Total_Num;

	//構造体を格納する配列
	std::vector<EnemyConnection>Enemy_Type_Array;

	//csvを入れておく配列
	std::vector<std::vector<std::string>>enemy_csv_array;

protected:

	//エラー時に返す用の配列
	std::vector<EnemyConnection>null_array;
	//敵を入れておく配列
	std::vector<EnemyConnection>enemy_array;

private:

	//敵のサイズ
	const float ENEMY_SIZE = 0.8f;

	//---座標系---//

	//敵の表示座標
	const tnl::Vector3 ENEMY_POS = { 600 , 320 , 0 };



	//---敵のドロップアイテムを格納しおく配列---//

	std::vector<ItemBase>enemy_drop_item;

	//アイテムのポインタ
	Shared<Item>item = nullptr;

	//敵の数
	const int ENEMY_NUM = 5;

	//死んだ判定のフラグ(主にfalseの場合敵の画像を表示する)
	bool dead_enemy_flag = false;

protected:
	
	//敵の画像ハンドル
	int enemy_hdl = 0;

	//敵のインデックス
	int enemy_index = 0;

	Enemytype enemy_type = Enemytype::NONE;

};

//------------------------------------------------------------------------------------------------------------
//
//雑魚モンスター
//
//-------------------------------------------------------------------------------------------------------------

class MobMonster : public Enemy {
public:

	MobMonster() {};
	MobMonster(const int enemy_id);
	~MobMonster()override {};

	//敵の攻撃処理
	void EnemyAction(Shared<BattleLog>battle_log)override;

private:

};

//------------------------------------------------------------------------------------------------------------
//
//ボスモンスター
//
//-------------------------------------------------------------------------------------------------------------

class BossMonster : public Enemy {

public:

	BossMonster(int enemy_id);
	~BossMonster()override {};

	//敵の攻撃処理
	void EnemyAction(Shared<BattleLog>battle_log)override;

	//敵のスキル配列の初期化
	void InitEnemySkill(int enemy_id);

	//敵のスキル配列を取得する
	std::vector <Shared<Skill>>& GetEnemySkillList() {
		return enemy_skill_;
	}

	//敵のスキル用のインデックスを取得する
	int GetEnemySkillIndex()const {
		return enmey_skill_index;
	}

private:

	//敵のボスの種類
	//どのボスを指定してるか
	//定数名はそれぞれモンスターの名前
	enum class BossType {
		NONE,
		//最後のボス
		ZERAHKIEL = 20,
		//中ボス1体目
		GROVEGUARDIAN = 21
	};

	//敵のスキル
	std::vector<Shared<Skill>>enemy_skill_;

	//敵のスキルの数
	const int ENEMY_SKILL_MAX_NUM = 3;

	//敵のスキル用のインデックス
	int enmey_skill_index = 0;

};
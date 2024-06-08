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
	struct EnemyStatus
	{
	public:

		//nameを返すゲッター
		std::string GetEnemyString() const {
			return name;
		}
		//IDを返すゲッター
		int GetEnemyId() const {
			return id;
		}
		//Hpを返すゲッター
		int GetEnemyHp() const{
			return hp;
		}
		//Attackを返すゲッター
		int GetEnemyAttack() const {
			return Attack;
		}
		//Defanceを返すゲッター
		int GetEnemyDefance() const {
			return Defance;
		}
		//Speedを返すゲッター
		int GetEnemySpeed() const {
			return Speed;
		}
		//経験値を返すゲッター
		int GetEnemyExpoint() const {
			return ex_point;
		}
		//moneyを返すゲッター
		int GetEnemyMoney() const {
			return money;
		}
		//ghdlを返すゲッター
		const std::string& GetEnemyGhdl()const {
			return gh;
		}
		//それぞれの耐性を返すゲッター
		float GetFireResist() const {
			return FireResist;
		}
		float GetIceResist() const {
			return IceResist;
		}
		float GetThunderResist() const {
			return ThunderResist;
		}
		//通常ドロップのアイテムのIDを返す
		int GetNomalDrop()const {
			return Nomal_Drop;
		}
		//レアドロップのアイテムIDを返す
		int GetRareDrop()const {
			return RareDrop;
		}
		//seのサウンドを返す
		std::string GetSeAttack()const {
			return attack_se;
		}
		//魔法力を返す
		int GetMagicPower()const {
			return magic_power;
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

	private:

		std::string name;
		int id = 0;
		int hp = 0;
		int Attack = 0;
		int Defance = 0;
		int Speed = 0;
		int ex_point = 0;
		int money = 0;
		std::string gh;
		float FireResist = 0;
		float IceResist = 0;
		float ThunderResist = 0;
		int Nomal_Drop = 0;
		int RareDrop = 0;
		std::string attack_se;
		int magic_power = 0;
	};

	//Enemyのステータスを読み込む為の関数
	void EnemyLoadTypeInfo();

	//敵の初期化
	void EnemyInit();

	//敵ののステータスのidを取得する
	EnemyStatus GetEnemyStatus(int id) const;
	
	//敵の配列を受け取る為のゲッター
	std::vector<EnemyStatus>& GetEnemyArray(){
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
	EnemyStatus Enemy_Status_Type;

	//構造体の総数
	int EnemyStatus_Total_Num;

	//構造体を格納する配列
	std::vector<EnemyStatus>Enemy_Type_Array;

	//csvを入れておく配列
	std::vector<std::vector<std::string>>enemy_csv_array;

	//---敵を入れておく配列---//
protected:

	//エラー時に返す用の配列
	std::vector<EnemyStatus>null_array;
	//敵を入れておく配列
	std::vector<EnemyStatus>enemy_array;

private:

	//敵のサイズ
	const float ENEMY_SIZE = 0.8f;

	//---座標系---//
	const tnl::Vector2i ENEMY_POS = { 600 , 320 };



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
	MobMonster(int enemy_id);
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

	BossMonster();
	~BossMonster()override {};

	//敵の攻撃処理
	void EnemyAction(Shared<BattleLog>battle_log)override;

	//敵のスキル配列を取得する
	std::vector <Shared<Skill>>& GetEnemySkillList() {
		return enemy_skill_;
	}

	//敵のスキル用のインデックスを取得する
	int GetEnemySkillIndex()const {
		return enmey_skill_index;
	}

private:

	//敵のスキル
	std::vector<Shared<Skill>>enemy_skill_;

	//敵のスキルの数
	const int ENEMY_SKILL_MAX_NUM = 3;

	//敵のスキル用のインデックス
	int enmey_skill_index = 0;

};
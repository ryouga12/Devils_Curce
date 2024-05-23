#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Item/Item.h"
#include"Actor.h"

class ItemBase;
class BattleLog;
class GameManager;
class SoundManager;
class SceneManager;
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
		std::string getEnemyString() const {
			return name;
		}
		//IDを返すゲッター
		int getEnemyId() const {
			return id;
		}
		//Hpを返すゲッター
		int getEnemyHp() const{
			return hp;
		}
		//Attackを返すゲッター
		int getEnemyAttack() const {
			return Attack;
		}
		//Defanceを返すゲッター
		int getEnemyDefance() const {
			return Defance;
		}
		//Speedを返すゲッター
		int getEnemySpeed() const {
			return Speed;
		}
		//経験値を返すゲッター
		int getEnemyExpoint() const {
			return ex_point;
		}
		//moneyを返すゲッター
		int getEnemyMoney() const {
			return money;
		}
		//ghdlを返すゲッター
		const std::string& getEnemyGhdl()const {
			return gh;
		}
		//それぞれの耐性を返すゲッター
		float getFireResist() const {
			return FireResist;
		}
		float getIceResist() const {
			return IceResist;
		}
		float getThunderResist() const {
			return ThunderResist;
		}
		//通常ドロップのアイテムのIDを返す
		int getNomalDrop()const {
			return Nomal_Drop;
		}
		//レアドロップのアイテムIDを返す
		int getRareDrop()const {
			return RareDrop;
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

	private:

		std::string name;
		int id;
		int hp;
		int Attack;
		int Defance;
		int Speed;
		int ex_point;
		int money;
		std::string gh;
		float FireResist;
		float IceResist;
		float ThunderResist;
		int Nomal_Drop;
		int RareDrop;

	};

	//Enemyのステータスを読み込む為の関数
	void EnemyLoadTypeInfo();

	//敵の初期化
	void EnemyInit();

	//敵ののステータスのidを取得する
	EnemyStatus GetEnemyStatus(int id) const;
	
	//敵の配列を受け取る為のゲッター
	std::vector<EnemyStatus>& GetEnemyArray(){
		return Enemy_Array;

		//敵の配列を取得できなかったらエラー用の配列を返す
		if (Enemy_Array.size() == 0) {
			return Null_Array;
		}
	}

	//敵のIDによってドロップのアイテムを切り替える
	void InitEnemyItemDrop(int EnemyID);

	//敵が所持しているアイテムの配列を取得する
	std::vector<ItemBase>& getItemDropArray() {
		return Enemy_Drop_Item;
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


	//敵のポインタを初期化する
	void InitEnemyPointer(Shared<Enemy>&enemy_pointer , int enemy_id);

	//敵の種類
	enum class Enemytype {
		NONE,
		MOB,
		BOSS
	};

	//敵のタイプをセットする
	void SetEnemyType(Enemytype enemyType) {
		enemy_type = enemyType;
	}

private:

	Enemytype enemy_type = Enemytype::NONE;

	//Enemyのステータスを入れておく変数
	EnemyStatus Enemy_Status_Type;

	//構造体の総数
	int EnemyStatus_Total_Num;

	//構造体を格納する配列
	std::vector<EnemyStatus>Enemy_Type_Array;

	//csvを入れておく配列
	std::vector<std::vector<std::string>>Enemy_Csv_Array;

	//---敵を入れておく配列---//
protected:
	//エラー時に返す用の配列
	std::vector<EnemyStatus>Null_Array;
	//敵を入れておく配列
	std::vector<EnemyStatus>Enemy_Array;

private:
	//---敵のドロップアイテムを格納しおく配列---//

	std::vector<ItemBase>Enemy_Drop_Item;

	//アイテムのポインタ
	Shared<Item>item = nullptr;

	//敵の数
	int Enemy_num = 5;

	//死んだ判定のフラグ(主にfalseの場合敵の画像を表示する)
	bool dead_enemy_flag = false;

protected:
	
	//敵の画像ハンドル
	int enemy_hdl = 0;

	//敵のインデックス
	int enemy_index = 0;

};

//------------------------------------------------------------------------------------------------------------
//
//雑魚モンスター
//
//-------------------------------------------------------------------------------------------------------------

class MobMonster : public Enemy {
public:

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

	/*BossMonster() {};
	~BossMonster()override {};*/

	//敵の攻撃処理
	/*void EnemyAction(Player::PlayerStatus& player, Shared<BattleLog>battle_log)override;*/

private:


};
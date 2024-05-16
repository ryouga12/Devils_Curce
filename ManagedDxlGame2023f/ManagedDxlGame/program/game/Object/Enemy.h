#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Item/ItemBase.h"
#include"../Item/Item.h"

class ItemBase;

class Enemy {
public:

	Enemy();

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
	std::vector<EnemyStatus> GetEnemyArray(int id); 

	//敵のIDによってドロップのアイテムを切り替える
	void InitEnemyItemDrop(int EnemyID);

	//敵が所持しているアイテムの配列を取得する
	std::vector<ItemBase>& getItemDropArray() {
		return Enemy_Drop_Item;
	}

private:

	//Enemyのステータスを入れておく変数
	EnemyStatus Enemy_Status_Type;

	//構造体の総数
	int EnemyStatus_Total_Num;

	//構造体を格納する配列
	std::vector<EnemyStatus>Enemy_Type_Array;

	//csvを入れておく配列
	std::vector<std::vector<std::string>>Enemy_Csv_Array;

	//---敵を入れておく配列---//

	//最初のエリアで出てくる敵
	std::vector<EnemyStatus>First_Area_Enemy;
	//次のエリアで出てくる敵
	std::vector<EnemyStatus>Second_Area_Enemy;
	//さらに次のエリアで出てくる敵
	std::vector<EnemyStatus>Third_Area_Enemy;
	//ボスのエリアで出て来る敵
	std::vector<EnemyStatus>Boss_Area_Enemy;
	//エラー時に返す用の配列
	std::vector<EnemyStatus>Null_Array;

	//---敵のドロップアイテムを格納しおく配列---//

	std::vector<ItemBase>Enemy_Drop_Item;

	//敵のIDの列挙体
	enum {
		SLIM,
		BAT,
		SNAKE,
		ARMORBEETLE,
		MONSTERDOG,
		BLOODBAT,
		TYRANTWORM,
		MONSTERPLANT,
		GOBLIN,
		GHOSTMUSH,
		SALAMANDER,
		MOKUJIN,
		CATKNIGHT,
		PUMOKINGHOST,
		NEOSLIM,
		WOODWALKER,
		BIGGOBLIN,
		GOLDREAPER,
		BLOODHOUND,
		LITTLEDEMON
	};

	//アイテムのポインタ
	Shared<Item>item = nullptr;

};
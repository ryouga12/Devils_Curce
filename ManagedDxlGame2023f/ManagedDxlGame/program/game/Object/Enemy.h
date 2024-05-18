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

	//エラー時に返す用の配列
	std::vector<EnemyStatus>Null_Array;
	//敵を入れておく配列
	std::vector<EnemyStatus>Enemy_Array;


	//---敵のドロップアイテムを格納しおく配列---//

	std::vector<ItemBase>Enemy_Drop_Item;

	//アイテムのポインタ
	Shared<Item>item = nullptr;

	//敵の数
	int Enemy_num = 5;
};

//------------------------------------------------------------------------------------------------------------
//
//雑魚モンスター
//
//-------------------------------------------------------------------------------------------------------------

class MobMonster : public Enemy {
public:

private:

};

//------------------------------------------------------------------------------------------------------------
//
//ボスモンスター
//
//-------------------------------------------------------------------------------------------------------------

class BossMonster : public Enemy {

public:

private:


};
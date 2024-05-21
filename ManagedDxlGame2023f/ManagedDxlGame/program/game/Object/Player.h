#pragma once
///
///プレイヤーに関するクラス
/// 


#include "../../dxlib_ext/dxlib_ext.h"
#include"Actor.h"
#include"../Manager/Camera.h"

//アニメーションを切り替える秒数
static const float ChangeAnimationTime = 0.1f;
//フレーム
static const int animationFlame = 3;

//アニメーション
enum {
	//下
	DIR_DOWN,
	//上
	DIR_UP,
	//左
	DIR_LEFT,
	//右
	DIR_RIGHT,
	//最大値
	DIR_MAX
};

class Player : public Actor {
public:

	
	Player();
	~Player();

	void Update(float delta_time);

	//playerのステータス
	struct PlayerStatus {
	public:

		//levelを返すゲッター
		int getLevel() const {
			return level;
		}
		//maxHpを返すゲッター
		float getMaxHp()const {
			return maxHp;
		}
		//hpを返すゲッター
		float getcurentHp() const{
			return curenthp;
		}
		//Attackを返すゲッター
		int getAttack() const{
			return Attack;
		}
		//Defanceを返すゲッター
		int getDefance() const {
			return Defance;
		}
		//Speedを返すゲッター
		int getSpeed() const {
			return Speed;
		}
		//必要な経験値を返すゲッター
		int getExpoint() const {
			return R_expoint;
		}
		//最大MPを返すゲッター
		float getMaxMp() const {
			return maxMp;
		}
		//現在Mpを返すゲッター
		float getCurentMp()const {
			return curentMp;
		}
		//魔法力を返すゲッター
		int getMagicPower()const {
			return magicPower;
		}

		//levelをセットする
		void SetPlayerLevel(int newlevel) {
			level = newlevel;
		}
		//maxHpをセットする
		void SetPlayerMaxHp(float newmaxhp) {
			maxHp = newmaxhp;
		}
		//hpをセットする
		void SetPlayerCurentHp(float newhp) {
			curenthp = newhp;
		}
		//Attackをセットする
		void SetPlayerAttack(int newattack) {
			Attack = newattack;
		}
		//Defanceをセットする
		void SetPlayerDefance(int newdefance) {
			Defance = newdefance;
		}
		//Speedをセットする
		void SetPlayerSpeed(int newspeed) {
			Speed = newspeed;
		}
		//必要な経験値をセットする
		void SetPlayerExpoint(int newexpoint) {
			R_expoint = newexpoint;
		}
		//最大Mpをセットする
		void SetPlayerMaxMp(float newmaxmp) {
			maxMp = newmaxmp;
		}
		//現在のMpをセットする
		void SetPlayerCurentMp(float newmp) {
			curentMp = newmp;
		}
		//魔法力をセットする
		void SetMagicPower(int newmagicpower) {
			magicPower = newmagicpower;
		}
			
	private:

		int level;
		float maxHp;
		float curenthp;
		int Attack;
		int Defance;
		int Speed;
		int R_expoint;
		float maxMp;
		float curentMp;
		int magicPower;

	};

private:

	//---プレイヤー関係---//
	float first_pos_x = 195;						//ゲームが始まった時のｘ座標
	float first_pos_y = 312;						//ゲームが始まった時のy座標
	int Total_Frame = 3;							//プレイヤーの総フレーム(3)
	int Horizontal_frame = 3;						//プレイヤーの横フレーム(3)
	int Vertical_frame = 1;							//プレイヤーの縦フレーム(1)
	int Horizontal_oneframe = 32;					//横方向へ 1フレームあたりの幅(32)
	int Vertical_oneframe = 32;						//縦方向へ 1フレームあたりの幅(32)

	//plyerの歩数
	int numberStep = 0;

	//plyerの動きの制御
	bool plyControl = false;

	//playerの移動前座標
	tnl::Vector3 prev_pos = { 0 ,0,0 };

	//プレイヤーの高さ
	const int CHARA_HEIGHT = 48;	

	//プレイヤーの幅
	const int CHARA_WIDTH = 48;						

	//プレイヤーのサイズを取得する為のタイプ
	int Width = 1; int Height = 2;

	//プレイヤーの座標
	tnl::Vector3 plyer_pos = {};

	//プレイヤーのお金
	int money = 0;

	//ステータスのインデックス
	int PlyerIndex = 0;

	//ステータス情報
	PlayerStatus plyerstatusSave;

	//名前
	std::string name = "";

public:
	
	 void SetPlayerPosition(const tnl::Vector3& new_pos) {
		plyer_pos = new_pos;
	 }

	//プレイヤーのステータスをロードする
	void PlyStatusLoad();


	//プレイヤーの動き
	void player_Move(float delta_time, const float& velocity);

	//プレイヤーの描画
	void player_draw(const KonCamera& camera, float scale);

	//プレイヤーの座標を取得する
	tnl::Vector3&  getPlayerPos() { return plyer_pos; }

	//プレイヤーの移動前座標を取得する
	tnl::Vector3& getPlayerPrevPos(){ return prev_pos; }
	
	//プレイヤーの動きを制御する
	void setPlayerControl(int controlFlag);


	//所持金を取得する
	inline int getPlayerMoney(){ return money; }

	//所持金を追加する
	inline void AddPlayerMoney(int add_money) {
		money += add_money;
	}

	//所持金を減らす
	inline void ReducePlayerMoney(int reduce_money) {
		money -= reduce_money;
	}

	//プレイヤーのサイズを取得する
	int getPlayerSize(int type);

	//プレイヤーのステータス配列を取得する
	PlayerStatus& getPlayerStatusSave(){
		return plyerstatusSave;
	}

private:

	//アニメーションに使う変数
	float anim_time_count = 0;
	int anim_ctrl_dir = DIR_DOWN;
	int anim_ctrl_frame = 0;
	int anim_hdls[DIR_MAX][5];

	//Statusを入れる変数
	/*PlyerStatus plyer_Status;*/

	//構造体の総数
	int Ply_Status_Total_num;

	//構造体を格納する配列
	std::list<PlayerStatus>Ply_Status_Type;

	//csv読み取り用の配列
	std::vector<std::vector<tnl::CsvCell>>PlyerStatus_Csv_Info;


public:

	//プレイヤーのステータスを他のクラスなどで取得する
	PlayerStatus GetPlyerStatus(int level) const;

	//ステータスをセットする
	void SetPlayerStatus(int level) {
		plyerstatusSave = GetPlyerStatus(level);
	}

	//Playerのアニメーションを変化させる
	void AnimationChange(int number) {
		anim_ctrl_dir = number;
	}

	//歩数を0にする
	void StepReset() {
		numberStep = 0;
	}
	//歩数を取得する
	int GetStep() {
		return numberStep;
	}

	bool getPlayerControl() {
		return plyControl;
	}

	//セーブロード機能を実験中（のちに追加予定）
	/*void PlyerSave();
	void PlyerLoad();*/



};


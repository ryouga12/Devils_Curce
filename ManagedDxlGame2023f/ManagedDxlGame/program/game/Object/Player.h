#pragma once
///
///プレイヤーに関するクラス
/// 


#include "../../dxlib_ext/dxlib_ext.h"
#include"Actor.h"
#include"../System/Camera.h"
#include"Enemy.h"

class BattleLog;
class SoundManager;
class GameManager;
class SceneManager;
class Skill;
class Nomal_Attack;
class CsvManager;
class InMapScene;
class CsvManager;

class Player : public Actor {
public:

	
	Player();
	~Player();

	void Update(float delta_time);

	//playerのステータス
	struct PlayerStatus {
	public:

		//levelを取得する
		int GetLevel() const {
			return level;
		}
		//maxHpを取得する
		float GetMaxHp()const {
			return maxHp;
		}
		//hpを取得する
		float GetcurentHp() const{
			return curenthp;
		}
		//Attackを取得する
		int GetAttack() const{
			return Attack;
		}
		//Defanceを取得する
		int GetDefance() const {
			return Defance;
		}
		//Speedを取得する
		int GetSpeed() const {
			return Speed;
		}
		//必要な経験値を取得する
		int GetExpoint() const {
			return R_expoint;
		}
		//最大MPを取得する
		float GetMaxMp() const {
			return maxMp;
		}
		//現在Mpを取得する
		float GetCurentMp()const {
			return curentMp;
		}
		//魔法力を取得する
		int GetMagicPower()const {
			return magicPower;
		}
		//プレイヤーのIDを取得する
		int GetPlayerID()const {
			return player_id;
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
		//プレイヤーのIDをセットする
		void SetPlayerID(int new_player_id) {
			player_id = new_player_id;
		}

	private:

		//プレイヤーのレベル
		int level;
		//プレイヤーの最大HP
		float maxHp;
		//プレイヤーの現在のHP
		float curenthp;
		//プレイヤーの攻撃力
		int Attack;
		//プレイヤーの防御力
		int Defance;
		//プレイヤーの素早さ
		int Speed;
		//プレイヤーの必要な経験値
		int R_expoint;
		//プレイヤーの最大MP
		float maxMp;
		//プレイヤーの現在のMP
		float curentMp;
		//プレイヤーの魔法力
		int magicPower;
		//プレイヤーのID
		int player_id;

	};

	//アニメーション
	enum {
		//下
		DIR_DOWN,
		//左
		DIR_LEFT,
		//右
		DIR_RIGHT,
		//上
		DIR_UP,
		//左下
		DIR_DIAGONAL_DOWN_LEFT,
		//右下
		DIR_DIAGONA_DOWN_RIGTH,
		//左上
		DIR_DIAGONAL_UPPER_LEFT,
		//右上
		DIR_DIAGONAL_UPPER_RIGTH,
		//最大値
		DIR_MAX
	};

	//プレイヤーの状態
	enum class PlayerState {
		IDLE,
		NOMALATTACK,
		SKILL,
		FLEE,
		DEAD,
	};

	//プレイヤーの状態を切り替える
	void SetPlayerState(PlayerState new_player_state) {
		player_state = new_player_state;
	}

	//プレイヤーの状態を取得する
	PlayerState GetPlayerState(){
		return player_state;
	}

	//プレイヤーの行動
	void PlayerMoveProcess(float delta_time, Shared<BattleLog>& basttle_log , Shared<Enemy>& enemy, Shared<Nomal_Attack>& nomal_attack);

	bool getPlayerControl()const {
		return plyControl;
	}

private:

	//---プレイヤー関係---//

	//現在のプレイヤーの状態
	PlayerState player_state = PlayerState::IDLE;

	float first_pos_x = 195;						//ゲームが始まった時のｘ座標
	float first_pos_y = 312;						//ゲームが始まった時のy座標
	int Total_Frame = 3;							//プレイヤーの総フレーム(3)
	int Horizontal_frame = 3;						//プレイヤーの横フレーム(3)
	int Vertical_frame = 1;							//プレイヤーの縦フレーム(1)
	int Horizontal_oneframe = 48;					//横方向へ 1フレームあたりの幅(48)
	int Vertical_oneframe = 48;						//縦方向へ 1フレームあたりの幅(48)

	//plyerの歩数
	int numberStep = 0;

	//plyerの動きの制御
	bool plyControl = false;

	//playerの移動前座標
	tnl::Vector3 prev_pos = { 0 ,0,0 };

	//プレイヤーの高さ
	//プレイヤーの幅
	const tnl::Vector2i CHARASIZE = { 36 ,72 };

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

	//武器のタイプ(通常攻撃のエフェクトの切り替えの為)
	int weapon_type = 0;

public:
	
	 void SetPlayerPosition(const tnl::Vector3& new_pos) {
		plyer_pos = new_pos;
	 }

	//プレイヤーのステータスをロードする
	void PlyStatusLoad();

	//プレイヤーの画像をセットする
	void SetPlayerAnimationHdl(int ghdl_id);

	//プレイヤーの動き
	//引数 : Delta_time , 動く速度　, マップの高さ(移動制限の為)
	void player_Move(float delta_time, const float& velocity ,const int MAPHEIGHT);

	//プレイヤーの描画
	//引数 : カメラ , プレイヤーのサイズ
	void player_draw(const KonCamera& camera, float scale);

	//プレイヤーの座標を取得する
	tnl::Vector3&  getPlayerPos() { return plyer_pos; }

	//プレイヤーの移動前座標を取得する
	tnl::Vector3& getPlayerPrevPos(){ return prev_pos; }
	
	//プレイヤーの動きを制御する
	void setPlayerControl() {
		plyControl = !plyControl;
	}


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
	int GetPlayerWidth()const {
		return CHARASIZE.x;
	}
	int GetPlayerHight()const {
		return CHARASIZE.y;
	}
	tnl::Vector2i GetPlayerSize()const {
		return CHARASIZE;
	}

	//プレイヤーのステータス配列を取得する
	PlayerStatus& getPlayerStatusSave(){
		return plyerstatusSave;
	}

	//プレイヤーにマップの状態をセットする(主にプレイヤーがセーブの際どこに居るか認識する為)
	void SetPlayerCurentMapMemory(int curent_inmap) {
		player_curent_map_memory = curent_inmap;
	}

	//プレイヤーがどのマップの状態にいるか取得する
	int GetPlayerCurentMapState()const {
		return player_curent_map_memory;
	}

	//スキルを追加する
	void SaveReadSkill(int skill_id);

private:

	//アニメーションに使う変数
	float anim_time_count = 0;
	int anim_ctrl_dir = DIR_DOWN;
	int anim_ctrl_frame = 0;
	int anim_hdls[DIR_MAX][9];

	//構造体の総数
	int Ply_Status_Total_num;

	//構造体を格納する配列
	std::list<PlayerStatus>Ply_Status_Type;

	//csv読み取り用の配列
	std::vector<std::vector<tnl::CsvCell>>PlyerStatus_Csv_Info;

	//全滅した時に流すSEの音
	const float annihilation_Time = 3.5f;

	//特技用のインベントリ
	std::vector<Shared<Skill>> player_skill_list;

	//特技の数
	int SkillNum = 0;

	//プレイヤーに保存しておくマップの状態の情報
	int player_curent_map_memory = 0;

	//---フラグ関係---//


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

	//スキルの配列を取得する
	std::vector<Shared<Skill>>& getSkillList() {
		return player_skill_list;
	}

	//スキルの個数を取得する
	int getSkillNum() {
		return SkillNum;
	}

	//プレイヤーの死亡処理
	void DeadPlayerProcess(Shared<BattleLog>& battle_log);

	//プレイヤーの攻撃処理
	void PlayerAttackProcess(Enemy::EnemyConnection& enemy_status,Shared<BattleLog>& battle_log , Shared<Nomal_Attack>& nomal_attack);

	//スキルをセットする
	void SkillSet(Shared<BattleLog> battle_log = nullptr);

	//特技を追加する
	void AddSkill(const Shared<Skill>& skill);

	//セーブロード機能を実験中（のちに追加予定）
	void PlyerSave();
	void PlyerLoad();

};


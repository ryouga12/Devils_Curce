#pragma once
//------------------------------------------------------------------------------------------------------------
//
//プレイヤーに関するクラス
//
//-------------------------------------------------------------------------------------------------------------


#include "../../dxlib_ext/dxlib_ext.h"
#include"Actor.h"
#include"../System/Camera.h"
#include"Enemy.h"

class BattleLog;
class Skill;
class Nomal_Attack;
class InMapScene;

class Player final: public Actor {
public:

	Player();
	~Player();

	//playerのステータス
	struct PlayerStatus {
	public:

		//levelを取得する
		int GetLevel() const {
			return level;
		}
		//maxHpを取得する
		int GetMaxHp()const {
			return maxHp;
		}
		//hpを取得する
		int GetcurentHp() const{
			return curenthp;
		}
		//Attackを取得する
		int GetAttack() const{
			return Attack;
		}
		//基礎攻撃力を取得する
		int GetBaseAttack()const {
			return base_attack;
		}
		//基礎防御力を取得する
		int GetBaseDefance()const {
			return base_defance;
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
		int GetMaxMp() const {
			return maxMp;
		}
		//現在Mpを取得する
		int GetCurentMp()const {
			return curentMp;
		}
		//魔法力を取得する
		int GetMagicPower()const {
			return magicPower;
		}

		//levelをセットする
		void SetPlayerLevel(const int newlevel) {
			level = newlevel;
		}
		//maxHpをセットする
		void SetPlayerMaxHp(const int newmaxhp) {
			maxHp = newmaxhp;
		}
		//hpをセットする
		void SetPlayerCurentHp(const int newhp) {
			curenthp = newhp;
		}
		//Attackをセットする
		void SetPlayerAttack(const int newattack) {
			Attack = newattack;
		}
		//Defanceをセットする
		void SetPlayerDefance(const int newdefance) {
			Defance = newdefance;
		}
		//基礎攻撃力をセットする
		void SetPlayerBaseAttack(const int new_attack) {
			base_attack = new_attack;
		}
		//基礎防御力をセットする
		void SetBaseDefance(const int new_defance) {
			base_defance = new_defance;
		}
		//Speedをセットする
		void SetPlayerSpeed(const int newspeed) {
			Speed = newspeed;
		}
		//必要な経験値をセットする
		void SetPlayerExpoint(const int newexpoint) {
			R_expoint = newexpoint;
		}
		//最大Mpをセットする
		void SetPlayerMaxMp(const int newmaxmp) {
			maxMp = newmaxmp;
		}
		//現在のMpをセットする
		void SetPlayerCurentMp(const int newmp) {
			curentMp = newmp;
		}
		//魔法力をセットする
		void SetMagicPower(const int newmagicpower) {
			magicPower = newmagicpower;
		}

	private:

		//プレイヤーのレベル
		int level;
		//プレイヤーの最大HP
		int maxHp;
		//プレイヤーの現在のHP
		int curenthp;
		//プレイヤーの基礎攻撃力
		int base_attack;
		//プレイヤーの攻撃力
		int Attack;
		//プレイヤーの基礎防御力
		int base_defance;
		//プレイヤーの防御力
		int Defance;
		//プレイヤーの素早さ
		int Speed;
		//プレイヤーの必要な経験値
		int R_expoint;
		//プレイヤーの最大MP
		int maxMp;
		//プレイヤーの現在のMP
		int curentMp;
		//プレイヤーの魔法力
		int magicPower;
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
		//選択状態
		CHOICE,
		//通常攻撃
		NOMALATTACK,
		//スキル使用時
		SKILL,
		//逃げる
		FLEE,
		//死亡
		DEAD,
		//待機状態
		IDLE
	};

	//プレイヤーの見た目の状態
	enum PlayerEquipState {
		//素手
		BARE_HANDS,
		//剣
		SWORD,
		//ハンマー
		HAMMER,
		//槍
		SPEAR,
		//ロングソード
		LONGSWORD
	};

	//プレイヤーの状態を切り替える
	void SetPlayerState(PlayerState new_player_state) {
		player_state = new_player_state;
	}

	//プレイヤーの状態を取得する
	PlayerState GetPlayerState(){
		return player_state;
	}

	//プレイヤーのバトル時の行動
	//arg_1 : Delta_time
	//arg_2 : バトルログのスマートポインタ
	//arg_3 : 敵のスマートポインタ
	//arg_4 : 通常攻撃のスマートポインタ
	void PlayerMoveProcess(float delta_time, Shared<BattleLog>& basttle_log , Shared<Enemy>& enemy, Shared<Nomal_Attack>& nomal_attack);

	//プレイヤーの行動制限のフラグを取得する
	bool GetPlayerControl()const {
		return play_control;
	}

	//プレイヤーの名前をセットする
	void SetPlayerName(const std::string& new_player_name) {
		name = new_player_name;
	}

	//プレイヤーの名前を取得する
	const std::string& GetPlayerName()const {
		return name;
	}

	//プレイヤーのIDを取得する
	int GetPlayerID()const {
		return player_id;
	}

	//プレイヤーのIDをセットする
	void SetPlayerID(const int new_player_id) {
		player_id = new_player_id;
	}

	//プレイヤーのアイコンを取得する
	int GetPlayerIcon()const {
		return player_icon;
	}
	//プレイヤーの座標をセットする
	void SetPlayerPosition(const tnl::Vector3& new_pos) {
		plyer_pos = new_pos;
		prev_pos = new_pos;
	}

	//プレイヤーのステータスをロードする
	void PlyStatusLoad();

	//プレイヤーの画像をセットする
	//主にcsvなどに記述してまとめて設定する
	//arg_1 : 画像のID(装備などで分ける際のID)
	//arg_2 : プレイヤーのID
	void SetPlayerAnimationHdl(const int& ghdl_id, const int& player_id);

	//プレイヤーの動き
	//arg_1 : delta_time 
	//arg_2 : 動く速度　
	//arg_3 : マップの高さ(移動制限の為)
	void Player_Move(const float& delta_time, const float& velocity, const int& MAPHEIGHT);

	//プレイヤーの描画
	//arg_1 : カメラ 
	//arg_2 : プレイヤーのサイズ
	void Player_draw(const KonCamera& camera, const float& scale);

	//プレイヤーの座標を取得する
	tnl::Vector3& GetPlayerPos() { return plyer_pos; }

	//プレイヤーの移動前座標を取得する
	tnl::Vector3& GetPlayerPrevPos() { return prev_pos; }

	//プレイヤーの動きを制御する
	void PlayerControlChangeFlag() {
		play_control = !play_control;
	}

	//プレイヤーのアニメーション処理
	void PlayerAnimation(const float& delta_time);

	//所持金を取得する
	int GetPlayerMoney()const { return money; }

	//所持金を追加する
	void AddPlayerMoney(const int& add_money) {
		money += add_money;
	}

	//所持金を減らす
	void ReducePlayerMoney(const int& reduce_money) {
		money -= reduce_money;
	}

	//プレイヤーのサイズを取得する
	const tnl::Vector2i& GetPlayerSize()const {
		return CHARASIZE;
	}

	//プレイヤーのステータス配列を取得する
	PlayerStatus& GetPlayerStatusSave() {
		return player_status_save;
	}

	//プレイヤーにマップの状態をセットする(主にプレイヤーがセーブの際どこに居るか認識する為)
	void SetPlayerCurentMapMemory(int curent_inmap) {
		player_curent_map_memory = curent_inmap;
	}

	//プレイヤーがどのマップの状態にいるか取得する
	int GetPlayerCurentMapState()const {
		return player_curent_map_memory;
	}

	//取得したIDのスキルを追加する
	//主にセーブやデバックに使用する
	//arg_1 : スキルID
	void SaveReadSkill(const int& skill_id);

	//最大レベルを取得する
	int GetMaxLevel() const {
		return MAX_LEVEL;
	}

	//プレイヤーのステータスを他のクラスなどで取得する
	PlayerStatus GetPlyerStatus(const int& level) const;

	//ステータスをセットする
	void SetPlayerStatus(const int& level) {
		player_status_save = GetPlyerStatus(level);
	}

	//Playerのアニメーションを変化させる
	void AnimationChange(const int& number) {
		anim_ctrl_dir = number;
	}

	//歩数を0にする
	void StepReset() {
		numberStep = 0;
	}
	//歩数を取得する
	int GetStep()const {
		return numberStep;
	}

	//スキルの配列を取得する
	const std::vector<Shared<Skill>>& getSkillList() {
		return player_skill_list;
	}

	//プレイヤーの死亡処理
	void DeadPlayerProcess(Shared<BattleLog>& battle_log);

	//プレイヤーの攻撃処理
	void PlayerAttackProcess(Enemy::EnemyConnection& enemy_status, Shared<BattleLog>& battle_log, Shared<Nomal_Attack>& nomal_attack);

	//スキルをセットする
	//レベルに応じてスキルを追加する
	//arg_1 : バトルログのポインタ(主にバトルシーンで取得したというログを流す為)
	void SkillSet(const Shared<BattleLog>& battle_log);

	//特技を追加する
	void AddSkill(const Shared<Skill>& skill);

	//セーブロード
	void PlayerSave();
	bool PlayerLoad();


	//プレイヤーの表示フラグを切り替える
	//プレイヤーを一時的に見えなくする為
	void PlayerDisplayChange() {
		player_display_flag = !player_display_flag;
	}

	//プレイヤーの表示フラグを取得する
	bool GetPlayerDisplayFlag()const {
		return player_display_flag;
	}

	//スキルを全て追加する(デバック用)
	void DebugAddSkill();

	//選択したスキルを消去する
	void SkillRemove(const int& skill_id);

private:

	//---プレイヤー関係---//

	//名前
	std::string name;

	//プレイヤーのマックスレベル
	const int MAX_LEVEL = 20;

	//プレイヤーのID
	int player_id = 0;

	//現在のプレイヤーの状態
	PlayerState player_state = PlayerState::CHOICE;

	//plyerの歩数
	int numberStep = 0;

	//plyerの動きの制御
	bool play_control = false;

	//playerの移動前座標
	tnl::Vector3 prev_pos = { 0 ,0,0 };

	//プレイヤーの高さ
	//プレイヤーの幅
	const tnl::Vector2i CHARASIZE = { 36 ,72 };

	//プレイヤーの座標
	tnl::Vector3 plyer_pos = {};

	//プレイヤーのお金
	int money = 50;

	//ステータス情報
	PlayerStatus player_status_save;

	//プレイヤーのステータス表示や戦闘時のアイコンのハンドル
	std::string PLAYER_BATTLE_ICON[4] =
	{
	 "graphics/Player/player_image_0/Player_Icon_sentou.png",
	 "graphics/Player/player_image_01/Player_Icon_sentou.png",
	 "graphics/Player/player_girl_image_02/Player_Icon_sentou.png",
	 "graphics/Player/player_girl_image_03/Player_Icon_sentou.png"
	};

	//プレイヤーのステータス表示や戦闘時のアイコン
	int player_icon = 0;

	//スキルのスマートポインタ
	Shared<Skill>skill = nullptr;

	//アニメーションに使う変数
	float anim_time_count = 0;
	int anim_ctrl_dir = DIR_DOWN;
	int anim_ctrl_frame = 0;
	int anim_hdls[DIR_MAX][9];

	//構造体を格納する配列
	std::list<PlayerStatus>player_status_type;

	//csv読み取り用の配列
	std::vector<std::vector<tnl::CsvCell>>plyer_status_csv_info;

	//特技用のインベントリ
	std::vector<Shared<Skill>> player_skill_list;

	//特技の数
	int skill_num = 0;

	//プレイヤーに保存しておくマップの状態の情報
	int player_curent_map_memory = 0;

	//---フラグ関係---//

	//プレイヤーの表示を管理するフラグ
	bool player_display_flag = true;
};


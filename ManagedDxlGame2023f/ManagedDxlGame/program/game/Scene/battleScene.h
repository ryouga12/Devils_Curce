#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/GameManager.h"
#include"../Manager/SceneManager.h"
#include<random>

//-----------------------------------------------------------------------------------------------------
//
//フィールドの戦闘シーン
//
//-----------------------------------------------------------------------------------------------------

class Player;
class Enemy;
class Menu;
class MenuWindow;
class MapScene;
class RusultScene;
class Battle_Log;
class BaseScene;
class Actor;

class BattleScene : public  BaseScene{

public:

	BattleScene() {};
	BattleScene(tnl::Vector3 pos ,int background, int EnemyID , Shared<Enemy>enemy);
	virtual~BattleScene()override;

	//更新処理
	virtual void Update(float delta_time)override;

	//描画
	virtual void Draw()override;

	//バトルウィンドウの描画
	void BattleDraw();

	//バトルシーン内で切り替える関数
	void MenuUpdate(Player::PlayerStatus& playerStatus,Enemy::EnemyStatus& enemyStatus_);

	//アイテム選択時の処理
	void ItemSelectProcess();

	//アイテムのを使った際の処理
	void ItemUseProcess();

	//スキル選択時の処理
	void SkillSelectProcess();

	//メニューの初期化
	void InitMenuWindow();

	//アニメーション時のターンチェンジ
	void AnimationTurnChange(int enemyhp, float delta_time);

	//敵の死亡処理、演出
	void DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyStatus& enemy_status);

	// レベルアップ判定 & 処理
	void ChackPlayerLevelUp(Player::PlayerStatus& player_status);

	//HPやMPバーの処理
	void PlayerStatusDraw();

	//アイテムのドロップ処理
	void ItemDropProcess();

	//一時的に上がった攻撃力などをリセットする
	void BattleBuffResetProcess();

	//武器のタイプをSetする
	void SetWeaponType();

	//逃げるを選択した際の処理
	void FleeProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus , float delta_time);


	//バトルシーンの状態を管理する
	enum class BattleState {
		IDLE,	//バトルが終了した待機状態
		BATTLE, //戦闘
	};

	//バトルの状態をセットする
	void SetBattleState(BattleState newState) {
		battle_state = newState;
	}

private:

	//背景の画像
	int background_ = 0;

	//座標を一時的に保存しておく変数
	tnl::Vector3 map_pos = {};

	//装備した時の攻撃力と防御力
	const int equipAttack = 0;
	const int equipDefance = 1;

	//遅延させる秒数
	const float Time = 1.0f;

	//逃げれる確率
	int probability = 50;

	//通常ドロップ
	const float NomalDrop = 30;

	//レアドロップ
	const float RareDrop = 10;

//------------------------------------------------------------------------------------------------------------------------
//それぞれのフラグ

	//レベルアップ時のフラグ
	bool levelUp_flag = false;

	//逃げる時のフラグ
	bool flee_flag = false;

	//プレイヤーのターンを示すフラグ
	bool PlayerTurn = false;

	//スキルを使用した時のフラグ(攻撃系)
	bool Skill_Attack_Flag = false;

	//スキルを使用した時のフラグ(バフ系)
	bool Skill_Buff_Flag = false;

	//特技を使えるかのフラグ(主にMpが足りたらtrue、足りなかったらfalse)
	bool Skill_Chaeck_Flag = true;


//------------------------------------------------------------------------------------------------------------------------
//シーケンス

private:

	TNL_CO_SEQUENCE(BattleScene, &BattleScene::seqIdle);

	bool seqIdle(float delta_time);
	//プレイヤーのシーケンス
	bool seqPlayerAction(float delta_time);

	//エネミーのシーケンス
	bool seqEnemyAction(float delta_time);

	//遷移させる為のシーケンス
	bool seqChangeTurn(float delta_time);

	//アニメーションを流す為のシーケンス
	bool seqAnimation(float delta_time);

	enum class Sequence {
		Idle,
		PlayerAction,
		EnemyAction
	};

	Sequence select_sequence = Sequence::Idle;

	//バトルシーンの状態
	BattleState battle_state = BattleState::BATTLE;

//--------------------------------------------------------------------------------------------------------------------------
//ポインタ変数
private:

	//---スマートポインタ---//

	//敵
	Shared<Enemy>enemy = nullptr;
	//行動を選択するウィンドウ
	Shared<MenuWindow>select_comand_menu = nullptr;
	//バトルログを流すウィンドウ
	Shared<Menu>battle_log_window = nullptr;
	//ステータスを表示するウィンドウ
	Shared<Menu>playerStatus_window = nullptr;
	//バトルログ
	Shared<BattleLog>battle_log = nullptr;
	//攻撃の際のウィンドウ
	Shared<MenuWindow>attack_window = nullptr;
	//アイテムを表示するウィンドウ
	Shared<Menu>item_window = nullptr;
	//アイテムを使うウィンドウ
	Shared<MenuWindow>select_itemuse_window = nullptr;
	//スキルを表示するウィンドウ
	Shared<Menu>skill_menu_window = nullptr;
	//通常攻撃
	Shared<Nomal_Attack>nomal_attack = nullptr;
	
	//---ポインタ---//

	//---ウィドウ---//
	
	//最初の行動の選択
	MenuWindow::MenuElement_t* select_action_coment = nullptr;
	//攻撃行動の選択
	MenuWindow::MenuElement_t* select_action_attack = nullptr;
	//アイテムの使用の選択
	MenuWindow::MenuElement_t* select_detail = nullptr;
	

//--------------------------------------------------------------------------------------------------------------------------
//メニューなどの表示関連

private:

	//どの動作を行うかでメニューを決める
	enum class MenuAction {
		first_action,
		fight_action,
		item_action,
		item_use_action,
		skill_action,
	};

	MenuAction select_action_menu = MenuAction::first_action;

	//表示させるアイテム数
	const int itemPerPage = 4;
	//アイテムを表示するy座標
	const int StartY = 550;
	//アイテムを表示する際の座標
	const tnl::Vector2i item_draw_pos = { 350 , 550 };
	//アイテムのページ数の描画座標
	tnl::Vector2i item_curent_page = { 400 , 700 };
	//各行の高さ
	const int lineHeight = 30;
	//各ページ
	int currentPage = 0;
	//カーソルのサイズ
	const float cursourSize = 0.3f;
	//カーソルのY座標
	const int curourY = 330;
	//アイテムを使う
	const int ItemUse = 0;
	//アイテムを使うウィンドウを閉じる
	const int ItemUseMenuClose = 1;
	//黒色
	const int Color_Black = 0;
	//白色
	const int Color_White = -1;
	//ターンを切り替える時に少し遅延を入れる
	const float Change_Turn_Time = 1;
	//アニメーションを流す時間
	const float Animation_Time = 1;
	//逃げる時のSEを流す時間
	const float flee_Time = 1;
	//メニュー選択で攻撃が選択された場合
	int Attack = 0;
	//メニュー選択で道具が選択された場合
	int Item = 1;
	//メニューを選択で逃げるが選択された時
	int Flee = 2;
	//攻撃の選択画面で特技が選択された時
	int SpecialSkill = 1;
	//選択画面で閉じるが押された時
	int MenuClose = 2;
	//カーソル制御するための変数
	int False = 1, True = 0;

//--------------------------------------------------------------------------------------------------------------------------
//ログ関連
private:

//--------------------------------------------------------------------------------------------------------------------------
//スキル関連
private:

	//スキルを使用した際のダメージ
	int SkillDamage = 0;

	//スキルべージ
	int SkillCurentPage = 0;

	//武器のタイプ(通常攻撃のエフェクトの切り替えの為)
	int weapon_type = 0;

public:

	//バトルシーン用のスキル表示
	void InventorySkillDraw();

	//スキルを使用した際のMp確認
	bool SkillUseMpChack(Player::PlayerStatus& playerStatus);

	//スキルを使用した際の処理
	void SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_);


//---------------------------------------------------
private:

	//全滅した時に流すSEの音
	const float annihilation_Time = 3.5f;

};
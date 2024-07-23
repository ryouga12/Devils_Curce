#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include"../Item/Item.h"
#include"../Object/Player.h"
#include"../Menu/MenuWindow.h"
#include<random>

//-----------------------------------------------------------------------------------------------------
//
//フィールドの戦闘シーン
//
//-----------------------------------------------------------------------------------------------------

//class Enemy;
class MapScene;
class RusultScene;
class BattleLog;
class BaseScene;
class GameManager;
class UIManager;

class BattleScene : public  BaseScene{

public:

	BattleScene() {};

	//バトルシーンのコンストラクタ
	//引数 : arg1 保管する座標 、atg2 バトルの背景 、arg3 エネミー 、arg4 ボスの時はマップ内の為どこのマップにいるかも格納しておく 
	BattleScene(tnl::Vector3 pos ,int background , Shared<Enemy> enemy ,int inmap_state = 0);
	virtual~BattleScene()override;

	//更新処理
	virtual void Update(float delta_time)override;

	//描画
	virtual void Draw()override;

	//バトルウィンドウの描画
	void BattleDraw();

	//バトルシーン内で切り替える関数
	void MenuUpdate(Player::PlayerStatus& playerStatus,Enemy::EnemyConnection& enemy_);

	//アイテム選択時の処理
	void ItemSelectProcess();

	//アイテムのを使った際の処理
	void ItemUseProcess();

	//スキル選択時の処理
	void SkillSelectProcess();

	//メニューの初期化
	void InitMenuWindow();

	//敵の死亡処理、演出
	void DeadEnemyProcces(Player::PlayerStatus& playerStatus,Enemy::EnemyConnection& enemy_);

	// レベルアップ判定 & 処理
	void ChackPlayerLevelUp(Player::PlayerStatus& player_status);

	//HPやMPバーの処理
	void PlayerStatusDraw();

	//アイテムのドロップ処理
	void ItemDropProcess(Enemy::EnemyConnection& enemy_);

	//一時的に上がった攻撃力などをリセットする
	void BattleBuffResetProcess();

	//武器のタイプをSetする
	void SetWeaponType();

	//逃げるを選択した際の処理
	void FleeProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_ , float delta_time);


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
	int back_ground_ = 0;

	//座標を一時的に保存しておく変数
	tnl::Vector3 map_pos = {};

	//遅延させる秒数
	const float Time = 1.0f;

	//逃げれる確率
	int probability = 50;

	//InMapSceneの状態を保管しておく変数
	//これを渡して戦闘終了時にどのマップ内に居るかを判断する
	int inmap_state_save = 0;

	//最後のボスのID
	const int LAST_BOSS_ID = 20;

//------------------------------------------------------------------------------------------------------------------------
//それぞれのフラグ

	//プレイヤーのターンを示すフラグ
	bool PlayerTurn = false;

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
		IDLE,
		PLAYERACTION,
		ENEMYACTION
	};

	Sequence select_sequence = Sequence::IDLE;

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
	//ウィンドウ
	Weak<Menu>window_log;
	//バトルログ
	Shared<BattleLog>battle_log = nullptr;
	//攻撃の際のウィンドウ
	Shared<MenuWindow>attack_window = nullptr;
	//アイテムを使うウィンドウ
	Shared<MenuWindow>select_itemuse_window = nullptr;
	//通常攻撃
	Shared<Nomal_Attack>nomal_attack = nullptr;
	//アイテム
	Shared<Item>item_ = nullptr;
	
	//---ポインタ---//

	//---ウィンドウ---//
	
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
		FIRST_ACTION,
		FIGHT_ACTION,
		ITEM_ACTION,
		ITEM_USE_ACTION,
		SKILL_ACTION,
	};

	MenuAction select_action_menu = MenuAction::FIRST_ACTION;

	//表示させるアイテム数
	const int ITEMPERPAGE_ = 4;
	//アイテムを表示するy座標
	const int STARTY = 550;
	//アイテムを表示する際の座標
	const tnl::Vector2i ITEM_DRAW_POS = { 350 , 540};
	//アイテムのページ数の描画座標
	const tnl::Vector2i ITEM_CURENT_PAGE = { 400 , 690};
	//各行の高さ
	const int LINEHEIGHT = 30;
	//各ページ
	int currentPage = 0;
	//カーソルのサイズ
	const float CURSOURSIZE = 0.3f;
	//カーソルのY座標
	const int CUROURY = 330;
	//アイテムを使う
	const int ITEMUSE_ = 0;
	//アイテムを使うウィンドウを閉じる
	const int ITEMUSEMENUCLOSE_ = 1;
	//メニュー選択で攻撃が選択された場合
	const int ATTACK_ = 0;
	//メニュー選択で道具が選択された場合
	const int ITEM_ = 1;
	//メニューを選択で逃げるが選択された時
	const int FLEE_ = 2;
	//攻撃の選択画面で特技が選択された時
	const int SPECIAL_SKILL_ = 1;
	//選択画面で閉じるが押された時
	const int MENU_CLOSE_ = 2;

	//---メニュー系の座標---//

	//メニューウィンドウの共通座標
	
	//要素一つ目
	const tnl::Vector2i MENU_COMENT_POS_FIRST = { 100 , 540};
	//要素二つ目
	const tnl::Vector2i MENU_COMENT_POS_SECOND = { 100 , 590 };
	//要素三つ目
	const tnl::Vector2i MENU_COMENT_POS_THIRD = { 100 , 640};

	//アイテム使用時のメニューウィンドウ座標
	
	//使う
	const tnl::Vector2i MENU_ITEM_USE_POS = { 600 , 550 };
	//閉じる
	const tnl::Vector2i MENU_ITEM_CLOSE = { 600 , 600 };
//--------------------------------------------------------------------------------------------------------------------------
//ログ関連
private:

//--------------------------------------------------------------------------------------------------------------------------
//スキル関連
private:

	//武器のタイプ(通常攻撃のエフェクトの切り替えの為)
	int weapon_type = 0;

public:

	//バトルシーン用のスキル表示
	void InventorySkillDraw();

	//スキルを使用した際のMp確認
	bool SkillUseMpChack(Player::PlayerStatus& playerStatus);

	//スキルを使用した際の処理
	void SkillUseProcess(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemy_);


//---------------------------------------------------
private:

	//全滅した時に流すSEの音
	const float ANNIHILATION_TIME = 3.5f;

};
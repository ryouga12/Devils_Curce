#pragma once
//------------------------------------------------------------------------------------------------------------
//
//UIをまとめるマネージャー
//
//-------------------------------------------------------------------------------------------------------------

#include "../../dxlib_ext/dxlib_ext.h"
#include"../Menu/MenuWindow.h"
#include <unordered_map>

class CsvManager;
class GameManager;

class UIManager final {

public:

	static UIManager* getUIManager();
	static void delategetUIManager() { delete getUIManager(); }

	//更新処理
	void Update(float delta_time);

	//描画
	void Draw();

	//メニューを追加する
	void addMenu(const std::string& menuName, std::shared_ptr<Menu> menu);

	// メニューを取得する
	std::shared_ptr<Menu> getMenu(const std::string& menuName) {

		auto it = menu_map.find(menuName);

		if (it != menu_map.end()) {
			return it->second;
		}

		return nullptr;
	}

	//メニューを描画する
	void Menu_Draw(const std::string& menuName, const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height);

	//武器屋のコメント
	void armsdealerComentDraw(int type);

	//コメントをロードする
	void ComentLoad(const int max_num ,const std::string& name);

	//コメントを表示する
	void ComentDraw(const tnl::Vector2i& coment_pos);

	//void ComentDraw(const tnl::Vector2i& coment_pos , int value);

	//コメントを次に移動する
	void ComentNextByInput(const int max_draw_num);

	//コメントを初期化して１から表示させる
	void ComentClear() { curent_num = 0; }

	//カウンターを取得する
	int GetCount()const { return count; }

	//カウンターをリセットする
	void CountReset() { count = 0; }

	//フレームをリセットする
	void FrameReset() { frame = 0; }

	//プレイヤーのステータスをHPバーで表示する
	void PlayerStatusDrawWindow();

	//セーブした時の文字を表示させる
	void SaveText(const tnl::Vector3& text_pos );

	//セーブテキストの表示を切り替える（セーブテキストを表示する）
	void SaveTextFlagChange() { save_flag = !save_flag; }

	//プレイヤーの操作説明
	void PlayerMoveDetail(const std::vector<std::string>& detail_text);

	//説明の表示を切り替える
	void PlayerDetailSwitchDisplay() {
		player_detail_window_flag = !player_detail_window_flag;
	}

	//操作説明のテキストを取得する
	std::vector<std::string>&GetPlayerMoveDetailText() {
		return operation_instructions;
	}

	//バトル用の説明のテキストを取得する
	std::vector<std::string>&GetBattlePlayerMoveDetailText() {
		return battle_operation_instructions;
	}

	//話かける際のアイコンの表示
	void IconAnimation();

	//アイコン用のアニメーションを止める
	void IconAnimationStop() { if (icon_animation) { icon_animation->stop_animation();  } }

private:

	UIManager(){}
	~UIManager() {};

	//メニューを管理するマップ
	std::unordered_map<std::string, Shared<Menu>>menu_map;

	//コメントを格納する配列
	std::vector<std::string>coment_;

	//---それぞれのコメント---//

	//武器商人のコメントの座標
	const tnl::Vector2i ARMSDEALERCOMENT = { 150, 550 };

	//武器屋の話しかけたときのコメント
	const int ARMSDEALER_FIRST_COMET = 0;

	//武器屋の文字の座標
	const tnl::Vector2i WEAPONSHOP = { 60 , 520 };

	//武器屋の購入コメントの座標
	const tnl::Vector2i WEAPONSHOP_BUY = { 70, 100 };

	//武器屋のメニューを閉じるコメントの座標
	const tnl::Vector2i MENU_CLOSE = { 70, 150 };

	//白色
	const int COLOR_WHITE = -1;

	//現在のコメント
	int curent_num = 0;

	//カウンター(主にカウントを使ってコメント後のシーンを遷移させたりする)
	int count = 0;
	
	//セーブした時のコメント用のフラグ
	bool save_flag = false;
	
	//フレーム(主にフレームを数えてなにかイベントを起こすために使う)
	int frame = 0;

	//説明表示を切り替える為のフラグ
	bool player_detail_window_flag = true;

	//アイコンのアニメーション用のポインタ
	Shared<Animation>icon_animation = nullptr;

//--------------------------------------------------------------------------------------------------------------
//UIの説明文用の文字列

	//操作説明
	std::vector<std::string> operation_instructions
		= {"操作説明", "W : 上" ,"S : 下", "A : 左" , "D : 右","ESC : 道具を開く","TAB : ウィンドウの表示を切り替える"};

	//バトルシーン用の操作説明
	std::vector<std::string> battle_operation_instructions
		= { "操作説明" , "十字キー : カーソルの移動" , "← : 一つ前に戻る","Enter : 決定 , 次に進む" ,"TAB : ウィンドウの表示を切り替える" };
};
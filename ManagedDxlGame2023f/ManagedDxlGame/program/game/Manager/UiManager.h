#pragma once
//------------------------------------------------------------------------------------------------------------
//
//UIをまとめるマネージャー
//
//-------------------------------------------------------------------------------------------------------------

#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"
#include"../Menu/MenuWindow.h"
#include <unordered_map>


class UIManager final {

public:

	static UIManager* GetUIManager();
	static void DeleteGetUIManager();

	//メニューを追加する(主に他の所でウィンドを使い回したい場合に使用)
	//arg_ : メニューの名前
	//arg_ : 追加したいMenuクラスのSharedポインタ
	void addMenu(const std::string& menuName, const std::shared_ptr<Menu>& menu);

	// メニューを取得する
	//arg_1 : 取得したいメニューの名前
	std::shared_ptr<Menu> getMenu(const std::string& menuName) {

		auto it = menu_map.find(menuName);

		if (it != menu_map.end()) {
			return it->second;
		}

		return nullptr;
	}

	//メニューを描画する
	//arg_1 : キーの名前
	//arg_2 : X座標
	//arg_3 : Y座標
	//arg_4 : Menuの幅のサイズ
	//arg_5 : Menuの縦のサイズ
	void Menu_Draw(const std::string& menuName, const int& menu_x, const int& menu_y, const int& menu_width, const int& menu_height);

	//コメントをロードする
	//arg_1 : コメントの数
	//arg_2 : 名前(NPCなどの名前を照らし合わせてそれに該当したコメントをロードする)
	void ComentLoad(const int& max_num ,const std::string& name);

	//コメントを表示する
	void ComentDraw(const tnl::Vector2i& coment_pos);

	//コメントを次に移動する
	//arg_1 : コメントの描画数
	void ComentNextByInput(const int& max_draw_num);

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
	//arg_1 : 表示させたい座標
	void SaveText(const tnl::Vector2i& text_pos );

	//セーブテキストの表示を切り替える（セーブテキストを表示する）
	void SaveTextFlagChange() { save_flag = !save_flag; }

	//プレイヤーの操作説明
	void PlayerMoveDetail(const std::vector<std::string>& detail_text);

	//説明の表示を切り替える
	void PlayerDetailSwitchDisplay() {
		player_detail_window_flag = !player_detail_window_flag;
	}

	//操作説明の表示を管理するフラグを取得する
	const bool GetPlayerDetailFlage()const {
		return player_detail_window_flag;
	}

	//操作説明のテキストを取得する
	const std::vector<std::string>& GetPlayerMoveDetailText() const {
		return operation_instructions;
	}

	//バトル用の説明のテキストを取得する
	const std::vector<std::string>&GetBattlePlayerMoveDetailText() const {
		return battle_operation_instructions;
	}

	//HPバーやMPバーの計算
	void PlayerStatusBarUpdate(const float delta_time);

	//ストーリーの文字をふやしていく
	//流しきったらtrueを返す
	bool StoryDisplayUpdate(const float delta_time);

	//ストーリーの文字を表示する
	void StoryDisplay(const int& font_color);

	//ストーリーの文字のロード
	void StoryLoad(const int& section_type);

	//作成したフォントを取得する
	//(サイズ80, フォントタイプ : DX_FONTTYPE_EDGE)
	int GetFontString_80()const { return string_handle_80; }

	//作成したフォントを取得する
	//(サイズ100, フォントタイプ : DX_FONTTYPE_EDGE)
	int GetFontString_100()const { return string_handle_100; }

	//作成したフォントを取得する
	//(サイズ30, フォントタイプ : DX_FONTTYPE_EDGE)
	int GetFontString_30()const { return string_handle_30; }


private:


	UIManager();
	~UIManager();

	//メニューを管理するマップ
	std::unordered_map<std::string, Shared<Menu>>menu_map;

	//コメントを格納する配列
	std::vector<std::string>coment_;

	//ストリー用の配列
	std::vector<std::string>story_;

	//ストーリー用のカウント
	int curent_story = 0;

	//フォント(サイズ80 , フォントタイプ : DX_FONTTYPE_EDGE)
	int string_handle_80 = 0;

	//フォント(サイズ100 , フォントタイプ : DX_FONTTYPE_EDGE)
	int string_handle_100 = 0;

	//フォント(サイズ30 , フォントタイプ: DX_FONTTYPE_EDGE)
	int string_handle_30 = 0;


	//---それぞれのコメント---//

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

	//---HP・MPバー関連---//

	// HPバーの現在の長さを保持する変数
	float current_hp_bar = 0;

	// MPバーの現在の長さを保持する変数
	float current_mp_bar = 0;

	// HPバーの補間に使用する経過時間を保持する変数
	float hp_lerp_time = 0;

	// MPバーの補間に使用する経過時間を保持する変数
	float mp_lerp_time = 0;

	// 補間の総時間（秒）を保持する定数
	const float LERPDURATION = 1.0f;

	// 補間の開始時点でのHPバーの長さを保持する変数
	float hp_start = 0;

	// 補間の終了時点でのHPバーの長さを保持する変数
	float hp_end = 0;

	// 補間の開始時点でのMPバーの長さを保持する変数
	float mp_start = 0;

	// 補間の終了時点でのMPバーの長さを保持する変数
	float mp_end = 0;

	//---ストーリー(プロローグ・エピローグ)関連---//

	// 現在表示しているコメントのインデックス
	int current_story_comment_index = 0; 
	
	// 現在表示中のコメントのリスト
	std::vector<std::string> displayed_story_coments;
	
	// コメント表示のタイマー
	float story_display_timer = 0.0f; 

	//追加座標
	const int ADD_OFSET = 45;

	//ストーリーを全て表示したら切り替えるフラグ(主に次に進み文字を表示する為)
	bool story_end_flag = false;

	//Enterキーのボタンを表示する為の座標
	const tnl::Vector2i ENTER_KEY_POS = { 760 ,630 };

	//Enterキーの表示するための座標
	const tnl::Vector2i ENTER_KEY_STRING_POS = { 790 , 620};

	//Tabキーのボタンを表示する為の座標
	const tnl::Vector2i TAB_KEY_POS = {930 , 630 };

	//Tabキーの表示するための座標
	const tnl::Vector2i TAB_KEY_STRING_POS = { 960 , 620 };

//--------------------------------------------------------------------------------------------------------------
//UIの説明文用の文字列

	//操作説明
	std::vector<std::string> operation_instructions
		= {"操作説明", "W : 上" ,"S : 下", "A : 左" , "D : 右","ESC : 道具を開く","TAB : ウィンドウの表示を切り替える"};

	//バトルシーン用の操作説明
	std::vector<std::string> battle_operation_instructions
		= { "操作説明" ,  "W : 上" ,"S : 下", "A : 左" , "D : 右" , "back space : 一つ前に戻る","Enter : 決定 , 次に進む" ,"TAB : ウィンドウの表示を切り替える" };
};
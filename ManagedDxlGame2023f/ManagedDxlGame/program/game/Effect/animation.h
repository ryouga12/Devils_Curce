#pragma once
#include<string>
#include<vector>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"

enum class AnimationState {
	PLAYING,//再生
	PAUSE,//一時停止
	STOP,//停止
};

class Animation final {
public:

	Animation() = default;

	//アニメーション
	//arg_1 :  ハンドル 
	//arg_2 :  X座標  
	//arg_3 : Y座標 
	//arg_4 : アニメーションの幅の数 
	//arg_5 : アニメーションの縦の数 
	//arg_6 :  幅のサイズ 
	//arg_7 : 縦のサイズ 
	//arg_8 :  フレームの総数　
	//arg_ 9 : 遅延する秒数 
	//arg_10 :  エフェクトのサイズ(デフォルトは2で設定を変えたい場合は引数を使用する)) 
	Animation(const std::string& gh_pass, const int& x, const int& y, const int& width_num, const int& height_num, const int& width_size, const int& height_size, const int& frame_count, const int& delay , const float& size = 2.0f);
	~Animation();

	void play_animation();//アニメーションを再生する関数
	void stop_animation();//アニメーションを停止する関数
	void pause_animation();//アニメーションを一時停止する関数
	void update(const float delta_time);//アニメーションの更新をする関数
	void draw();//アニメーションの描画する関数
	bool is_finished() const;//アニメーションの終了判定をする関数
	void SetAnimationPos(const int new_draw_pos_x , const int new_draw_pos_y) { m_draw_pos_x = new_draw_pos_x, m_draw_pos_y = new_draw_pos_y; }

private:
	std::string m_graph_handle;//グラフィックハンドル
	int m_width_num;//フレームの幅数
	int m_height_num;//フレームの高さ数
	int m_width_size;//幅のサイズ
	int m_height_size;//高さのサイズ
	int m_frame_num;//フレームの総数
	int m_current_frame;//現在のフレーム
	int m_frame_interval;//フレーム間のインターバル
	int m_timer;//タイマー
	int* m_gh_ar;//グラフィックハンドルの配列
	int m_draw_pos_x;//描画のx座標					 
	int m_draw_pos_y;//描画のy座標
	AnimationState m_state;//アニメーションの状態
	float m_size;	//エフェクトのサイズ
	bool stop_flag = false;
};


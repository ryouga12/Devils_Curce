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

class Animation {
public:

	//アニメーション
	//引数 : (ハンドル , X座標 , Y座標 , アニメーションの幅の数 ,アニメーションの縦の数 , 幅のサイズ, 縦のサイズ , フレームの総数　,遅延する秒数 )
	Animation(const std::string gh_pass, int x, int y, int width_num, int height_num, int width_size, int height_size, int frame_count, int delay);
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
	bool stop_flag = false;
};


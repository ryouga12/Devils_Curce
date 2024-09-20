#pragma once
#include<string>
#include<vector>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"

//---------------------------------------------------------------------------------------------------------
//アニメーションクラス(アニメーションを設定して再生や停止などを行える)


class Animation final {
public:

	Animation() = default;

	//アニメーション
	//arg_1 :  ハンドル 
	//arg_2 :  X座標  
	//arg_3 :  Y座標 
	//arg_4 : アニメーションの幅の数 
	//arg_5 : アニメーションの縦の数 
	//arg_6 :  幅のサイズ 
	//arg_7 :  縦のサイズ 
	//arg_8 :  フレームの総数　
	//arg_9 :  遅延する秒数 
	//arg_10 :  エフェクトのサイズ(デフォルトは2で設定を変えたい場合は引数を使用する)) 
	Animation(const std::string& gh_pass, const int& x, const int& y, const int& width_num, const int& height_num, const int& width_size, const int& height_size, const int& frame_count, const int& delay , const float& size = 2.0f);
	~Animation();

	//アニメーションを再生する関数
	void play_animation();

	//アニメーションを停止する関数
	void stop_animation();

	//アニメーションを一時停止する関数
	void pause_animation();

	//アニメーションの更新をする関数
	void update(const float delta_time);

	//アニメーションの描画する関数
	void draw();

	//アニメーションの終了判定をする関数
	bool is_finished() const;

	enum class AnimationState {
		//再生
		PLAYING,
		//一時停止
		PAUSE,
		//停止
		STOP,
	};

private:

	//グラフィックハンドル
	std::string m_graph_handle;
	//フレームの幅数
	int m_width_num = 0;
	//フレームの高さ数
	int m_height_num = 0;
	//幅のサイズ
	int m_width_size = 0;
	//高さのサイズ
	int m_height_size = 0;
	//フレームの総数
	int m_frame_num = 0;
	//現在のフレーム
	int m_current_frame = 0;
	//フレーム間のインターバル
	int m_frame_interval = 0;
	//タイマー
	int m_timer = 0;
	//グラフィックハンドルの配列
	int* m_gh_ar = 0;
	//描画のx座標
	int m_draw_pos_x = 0;
	//描画のy座標
	int m_draw_pos_y = 0;
	//アニメーションの状態
	AnimationState m_state;
	//エフェクトのサイズ
	float m_size = 0;	
	//アニメーションを止めるフラグ
	bool m_stop_flag = false;
};


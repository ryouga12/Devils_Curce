
///-----------------------------------------------------------------------------------------
///共通で使う値などを管理する(今後色々と追加していく)
///-----------------------------------------------------------------------------------------


#pragma once
#include "DxLib.h"
#include <stdio.h>

namespace koni {

    //色
    namespace Color {

        //白
        static constexpr int32_t WHITE = -1;
        //黒
        static constexpr int32_t BLACK = 0;
        //赤
        static constexpr int32_t RED = 0xff0000;
        //黄色
        static constexpr int32_t YELLOW = 0xffff00;
        //青
        static constexpr int32_t BLUE = 0x0000ff;
        //シアン
        static constexpr int32_t CYAN = 0x00ffff;
        //緑
        static constexpr int32_t GREEN = 0x00ff00;
    }

    // 数値に関連する定数を管理する
    namespace Numeric {

        // 浮動小数点数定数
        static constexpr float FLOAT_VALUE_01 = 0.1f;           // 0.1 の浮動小数点数定数
        static constexpr float FLOAT_VALUE_02 = 0.2f;           // 0.2 の浮動小数点数定数
        static constexpr float FLOAT_VALUE_03 = 0.3f;           // 0.3 の浮動小数点数定数
        static constexpr float FLOAT_VALUE_04 = 0.4f;           // 0.4 の浮動小数点数定数
        static constexpr float FLOAT_VALUE_05 = 0.5f;           // 0.5 の浮動小数点数定数
        static constexpr float FLOAT_VALUE_06 = 0.6f;           // 0.6 の浮動小数点数定数
        static constexpr float FLOAT_VALUE_07 = 0.7f;           // 0.7 の浮動小数点数定数
        static constexpr float FLOAT_VALUE_08 = 0.8f;           // 0.8 の浮動小数点数定数
        static constexpr float FLOAT_VALUE_09 = 0.9f;           // 0.9 の浮動小数点数定数
        static constexpr float FLOAT_VALUE_10 = 1.0f;           // 1.0 の浮動小数点数定数


        // パーセンテージ定数
        static constexpr float PERCENT_5 = 0.05f;               // 5%
        static constexpr float PERCENT_10 = 0.1f;               // 10%
        static constexpr float PERCENT_20 = 0.2f;               // 20%
        static constexpr float PERCENT_30 = 0.3f;               // 30%
        static constexpr float PERCENT_40 = 0.4f;               // 40%
        static constexpr float PERCENT_50 = 0.5f;               // 50%
        static constexpr float PERCENT_60 = 0.6f;               // 60%
        static constexpr float PERCENT_70 = 0.7f;               // 70%
        static constexpr float PERCENT_80 = 0.8f;               // 80%
        static constexpr float PERCENT_90 = 0.9f;               // 90%
        static constexpr float PERCENT_100 = 1.0f;              // 100%

        // 倍率（スケール）定数
       
        //float 
        static constexpr float SCALE_HALF_F = 0.5f;             // 半分のスケール（0.5倍）
        static constexpr float SCALE_EIGHTY_PERCENT_F = 0.8f;   // 0.8倍のスケール
        static constexpr float SCALE_ONE_F = 1.0f;              // 等倍（1.0倍）
        static constexpr float SCALE_ONE_THIRTY_F = 1.3f;       // 1.3倍のスケール
        static constexpr float SCALE_ONE_AND_HALF = 1.5f;       // 1.5倍のスケール
        static constexpr float SCALE_ONE_EIGHT_F = 1.8f;        // 1.8倍のスケール
        static constexpr float SCALE_DOUBLE_F = 2.0f;           // 2倍のスケール
        static constexpr float SCALE_DOUBLE_AND_HALF = 2.5f;    // 2.5倍のスケール
        static constexpr float SCALE_TRIPLE_F = 3.0f;           // 3倍のスケール
        static constexpr float SCALE_TRIPLE_AND_HALF = 3.5f;    // 3.5倍のスケール
        static constexpr float SCALE_QUADRUPLE_F = 4.0f;        // 4倍のスケール
        static constexpr float SCALE_QUINTUPLE_F = 5.0f;        // 5倍のスケール
        static constexpr float SCALE_SIXFOLD_F = 6.0f;          // 6倍のスケール

        //int
        static constexpr int32_t SCALE_ONE_I = 1;               // 等倍（1倍）
        static constexpr int32_t SCALE_DOUBLE_I = 2;            // 2倍のスケール
        static constexpr int32_t SCALE_TRIPLE_I = 3;            // 3倍のスケール
        static constexpr int32_t SCALE_QUADRUPLE_I = 4;         // 4倍のスケール
        static constexpr int32_t SCALE_QUINTUPLE_I = 5;         // 5倍のスケール
        static constexpr int32_t SCALE_SIXFOLD_I = 6;           // 6倍のスケール

        // 割る数（ディバイダー）定数
        static constexpr int32_t DIVIDER_TWO = 2;               // 2で割る定数
        static constexpr int32_t DIVIDER_THREE = 3;             // 3で割る定数
        static constexpr int32_t DIVIDER_FOUR = 4;              // 4で割る定数

        // 角度定数
        static constexpr float ANGLE_90_DEGREES = 90.0f;        // 90度の角度定数
        static constexpr float ANGLE_180_DEGREES = 180.0f;      // 180度の角度定数
        static constexpr float ANGLE_270_DEGREES = 270.0f;      // 270度の角度定数
        static constexpr float ANGLE_360_DEGREES = 360.0f;      // 360度の角度定数

        // その他の数値定数
        static constexpr float PI = 3.14159f;                    // 円周率

        // アルファ値定数 (0-255)
        static constexpr int32_t ALPHA_TRANSPARENT = 0;          // 完全に透明
        static constexpr int32_t ALPHA_10_PERCENT = 25;          // 10%の透明度
        static constexpr int32_t ALPHA_20_PERCENT = 51;          // 20%の透明度
        static constexpr int32_t ALPHA_30_PERCENT = 76;          // 30%の透明度
        static constexpr int32_t ALPHA_40_PERCENT = 102;         // 40%の透明度
        static constexpr int32_t ALPHA_50_PERCENT = 127;         // 50%の透明度
        static constexpr int32_t ALPHA_60_PERCENT = 153;         // 60%の透明度
        static constexpr int32_t ALPHA_70_PERCENT = 178;         // 70%の透明度
        static constexpr int32_t ALPHA_80_PERCENT = 204;         // 80%の透明度
        static constexpr int32_t ALPHA_90_PERCENT = 229;         // 90%の透明度
        static constexpr int32_t ALPHA_OPAQUE = 255;             // 完全に不透明

        // 音量定数 (0-100)
        static constexpr int32_t VOLUME_MUTE = 0;                // 無音
        static constexpr int32_t VOLUME_10_PERCENT = 10;         // 10%の音量
        static constexpr int32_t VOLUME_20_PERCENT = 20;         // 20%の音量
        static constexpr int32_t VOLUME_30_PERCENT = 30;         // 30%の音量
        static constexpr int32_t VOLUME_40_PERCENT = 40;         // 40%の音量
        static constexpr int32_t VOLUME_50_PERCENT = 50;         // 50%の音量
        static constexpr int32_t VOLUME_60_PERCENT = 60;         // 60%の音量
        static constexpr int32_t VOLUME_70_PERCENT = 70;         // 70%の音量
        static constexpr int32_t VOLUME_80_PERCENT = 80;         // 80%の音量
        static constexpr int32_t VOLUME_90_PERCENT = 90;         // 90%の音量
        static constexpr int32_t VOLUME_MAX = 100;               // 最大音量

        // 音量の最大値 (0-255)
        static constexpr int32_t VOLUME_MAX_255 = 255;           // 255の最大音量

        // 時間定数 (秒)

        //int型
        static constexpr int32_t SECONDS_1_I = 1;                  // 1秒
        static constexpr int32_t SECONDS_2_I = 2;                  // 2秒
        static constexpr int32_t SECONDS_3_I = 3;                  // 3秒
        static constexpr int32_t SECONDS_4_I = 4;                  // 4秒
        static constexpr int32_t SECONDS_5_I = 5;                  // 5秒
        static constexpr int32_t SECONDS_6_I = 6;                  // 6秒
        static constexpr int32_t SECONDS_7_I = 7;                  // 7秒
        static constexpr int32_t SECONDS_8_I = 8;                  // 8秒
        static constexpr int32_t SECONDS_9_I = 9;                  // 9秒
        static constexpr int32_t SECONDS_10_I = 10;                // 10秒

        //flaot型
        static constexpr float SECONDS_0_2_F = 0.2f;                // 0.2秒(float)
        static constexpr float SECONDS_0_5_F = 0.5f;                // 0.5秒(float)
        static constexpr float SECONDS_0_8_F = 0.8f;                // 0.8秒(float)
        static constexpr float SECONDS_1_F = 1.0f;                  // 1秒(float)
        static constexpr float SECONDS_1_AND_HALF = 1.5f;           // 1.5秒(float)
        static constexpr float SECONDS_2_F = 2.0f;                  // 2秒(float)
        static constexpr float SECONDS_3_F = 3.0f;                  // 3秒(float)
        static constexpr float SECONDS_4_F = 4.0f;                  // 4秒(float)
        static constexpr float SECONDS_5_F = 5.0f;                  // 5秒(float)
        static constexpr float SECONDS_6_F = 6.0f;                  // 6秒(float)
        static constexpr float SECONDS_7_F = 7.0f;                  // 7秒(float)
        static constexpr float SECONDS_8_F = 8.0f;                  // 8秒(float)
        static constexpr float SECONDS_9_F = 9.0f;                  // 9秒(float)
        static constexpr float SECONDS_10_F = 10.0f;                // 10秒(float)


        // フォントサイズ定数
        static constexpr int32_t FONT_SIZE_10 = 10;                 //10(サイズ)
        static constexpr int32_t FONT_SIZE_20 = 20;                 //20(サイズ)
        static constexpr int32_t FONT_SIZE_30 = 30;                 //30(サイズ)
        static constexpr int32_t FONT_SIZE_40 = 40;                 //40(サイズ)
        static constexpr int32_t FONT_SIZE_50 = 50;                 //50(サイズ)
        static constexpr int32_t FONT_SIZE_60 = 60;                 //60(サイズ)
        static constexpr int32_t FONT_SIZE_70 = 70;                 //70(サイズ)
        static constexpr int32_t FONT_SIZE_80 = 80;                 //80(サイズ)
        static constexpr int32_t FONT_SIZE_90 = 90;                 //90(サイズ)
        static constexpr int32_t FONT_SIZE_100 = 100;               //100(サイズ)

        static constexpr int32_t DEFAULT_FONT_SIZE_ = 16;           //16(デフォルトサイズ)

        
    }
}

///-----------------------------------------------------------------------------------------
///���ʂŎg���l�Ȃǂ��Ǘ�����(����F�X�ƒǉ����Ă���)
///-----------------------------------------------------------------------------------------


#pragma once
#include "DxLib.h"
#include <stdio.h>

namespace koni {

    //�F
    namespace Color {

        //��
        static constexpr int32_t WHITE = -1;
        //��
        static constexpr int32_t BLACK = 0;
        //��
        static constexpr int32_t RED = 0xff0000;
        //���F
        static constexpr int32_t YELLOW = 0xffff00;
        //��
        static constexpr int32_t BLUE = 0x0000ff;
        //�V�A��
        static constexpr int32_t CYAN = 0x00ffff;
        //��
        static constexpr int32_t GREEN = 0x00ff00;
    }

    // ���l�Ɋ֘A����萔���Ǘ�����
    namespace Numeric {

        // ���������_���萔
        static constexpr float FLOAT_VALUE_01 = 0.1f;           // 0.1 �̕��������_���萔
        static constexpr float FLOAT_VALUE_02 = 0.2f;           // 0.2 �̕��������_���萔
        static constexpr float FLOAT_VALUE_03 = 0.3f;           // 0.3 �̕��������_���萔
        static constexpr float FLOAT_VALUE_04 = 0.4f;           // 0.4 �̕��������_���萔
        static constexpr float FLOAT_VALUE_05 = 0.5f;           // 0.5 �̕��������_���萔
        static constexpr float FLOAT_VALUE_06 = 0.6f;           // 0.6 �̕��������_���萔
        static constexpr float FLOAT_VALUE_07 = 0.7f;           // 0.7 �̕��������_���萔
        static constexpr float FLOAT_VALUE_08 = 0.8f;           // 0.8 �̕��������_���萔
        static constexpr float FLOAT_VALUE_09 = 0.9f;           // 0.9 �̕��������_���萔
        static constexpr float FLOAT_VALUE_10 = 1.0f;           // 1.0 �̕��������_���萔


        // �p�[�Z���e�[�W�萔
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

        // �{���i�X�P�[���j�萔
       
        //float 
        static constexpr float SCALE_HALF_F = 0.5f;             // �����̃X�P�[���i0.5�{�j
        static constexpr float SCALE_EIGHTY_PERCENT_F = 0.8f;   // 0.8�{�̃X�P�[��
        static constexpr float SCALE_ONE_F = 1.0f;              // ���{�i1.0�{�j
        static constexpr float SCALE_ONE_THIRTY_F = 1.3f;       // 1.3�{�̃X�P�[��
        static constexpr float SCALE_ONE_AND_HALF = 1.5f;       // 1.5�{�̃X�P�[��
        static constexpr float SCALE_ONE_EIGHT_F = 1.8f;        // 1.8�{�̃X�P�[��
        static constexpr float SCALE_DOUBLE_F = 2.0f;           // 2�{�̃X�P�[��
        static constexpr float SCALE_DOUBLE_AND_HALF = 2.5f;    // 2.5�{�̃X�P�[��
        static constexpr float SCALE_TRIPLE_F = 3.0f;           // 3�{�̃X�P�[��
        static constexpr float SCALE_TRIPLE_AND_HALF = 3.5f;    // 3.5�{�̃X�P�[��
        static constexpr float SCALE_QUADRUPLE_F = 4.0f;        // 4�{�̃X�P�[��
        static constexpr float SCALE_QUINTUPLE_F = 5.0f;        // 5�{�̃X�P�[��
        static constexpr float SCALE_SIXFOLD_F = 6.0f;          // 6�{�̃X�P�[��

        //int
        static constexpr int32_t SCALE_ONE_I = 1;               // ���{�i1�{�j
        static constexpr int32_t SCALE_DOUBLE_I = 2;            // 2�{�̃X�P�[��
        static constexpr int32_t SCALE_TRIPLE_I = 3;            // 3�{�̃X�P�[��
        static constexpr int32_t SCALE_QUADRUPLE_I = 4;         // 4�{�̃X�P�[��
        static constexpr int32_t SCALE_QUINTUPLE_I = 5;         // 5�{�̃X�P�[��
        static constexpr int32_t SCALE_SIXFOLD_I = 6;           // 6�{�̃X�P�[��

        // ���鐔�i�f�B�o�C�_�[�j�萔
        static constexpr int32_t DIVIDER_TWO = 2;               // 2�Ŋ���萔
        static constexpr int32_t DIVIDER_THREE = 3;             // 3�Ŋ���萔
        static constexpr int32_t DIVIDER_FOUR = 4;              // 4�Ŋ���萔

        // �p�x�萔
        static constexpr float ANGLE_90_DEGREES = 90.0f;        // 90�x�̊p�x�萔
        static constexpr float ANGLE_180_DEGREES = 180.0f;      // 180�x�̊p�x�萔
        static constexpr float ANGLE_270_DEGREES = 270.0f;      // 270�x�̊p�x�萔
        static constexpr float ANGLE_360_DEGREES = 360.0f;      // 360�x�̊p�x�萔

        // ���̑��̐��l�萔
        static constexpr float PI = 3.14159f;                    // �~����

        // �A���t�@�l�萔 (0-255)
        static constexpr int32_t ALPHA_TRANSPARENT = 0;          // ���S�ɓ���
        static constexpr int32_t ALPHA_10_PERCENT = 25;          // 10%�̓����x
        static constexpr int32_t ALPHA_20_PERCENT = 51;          // 20%�̓����x
        static constexpr int32_t ALPHA_30_PERCENT = 76;          // 30%�̓����x
        static constexpr int32_t ALPHA_40_PERCENT = 102;         // 40%�̓����x
        static constexpr int32_t ALPHA_50_PERCENT = 127;         // 50%�̓����x
        static constexpr int32_t ALPHA_60_PERCENT = 153;         // 60%�̓����x
        static constexpr int32_t ALPHA_70_PERCENT = 178;         // 70%�̓����x
        static constexpr int32_t ALPHA_80_PERCENT = 204;         // 80%�̓����x
        static constexpr int32_t ALPHA_90_PERCENT = 229;         // 90%�̓����x
        static constexpr int32_t ALPHA_OPAQUE = 255;             // ���S�ɕs����

        // ���ʒ萔 (0-100)
        static constexpr int32_t VOLUME_MUTE = 0;                // ����
        static constexpr int32_t VOLUME_10_PERCENT = 10;         // 10%�̉���
        static constexpr int32_t VOLUME_20_PERCENT = 20;         // 20%�̉���
        static constexpr int32_t VOLUME_30_PERCENT = 30;         // 30%�̉���
        static constexpr int32_t VOLUME_40_PERCENT = 40;         // 40%�̉���
        static constexpr int32_t VOLUME_50_PERCENT = 50;         // 50%�̉���
        static constexpr int32_t VOLUME_60_PERCENT = 60;         // 60%�̉���
        static constexpr int32_t VOLUME_70_PERCENT = 70;         // 70%�̉���
        static constexpr int32_t VOLUME_80_PERCENT = 80;         // 80%�̉���
        static constexpr int32_t VOLUME_90_PERCENT = 90;         // 90%�̉���
        static constexpr int32_t VOLUME_MAX = 100;               // �ő剹��

        // ���ʂ̍ő�l (0-255)
        static constexpr int32_t VOLUME_MAX_255 = 255;           // 255�̍ő剹��

        // ���Ԓ萔 (�b)

        //int�^
        static constexpr int32_t SECONDS_1_I = 1;                  // 1�b
        static constexpr int32_t SECONDS_2_I = 2;                  // 2�b
        static constexpr int32_t SECONDS_3_I = 3;                  // 3�b
        static constexpr int32_t SECONDS_4_I = 4;                  // 4�b
        static constexpr int32_t SECONDS_5_I = 5;                  // 5�b
        static constexpr int32_t SECONDS_6_I = 6;                  // 6�b
        static constexpr int32_t SECONDS_7_I = 7;                  // 7�b
        static constexpr int32_t SECONDS_8_I = 8;                  // 8�b
        static constexpr int32_t SECONDS_9_I = 9;                  // 9�b
        static constexpr int32_t SECONDS_10_I = 10;                // 10�b

        //flaot�^
        static constexpr float SECONDS_0_2_F = 0.2f;                // 0.2�b(float)
        static constexpr float SECONDS_0_5_F = 0.5f;                // 0.5�b(float)
        static constexpr float SECONDS_0_8_F = 0.8f;                // 0.8�b(float)
        static constexpr float SECONDS_1_F = 1.0f;                  // 1�b(float)
        static constexpr float SECONDS_1_AND_HALF = 1.5f;           // 1.5�b(float)
        static constexpr float SECONDS_2_F = 2.0f;                  // 2�b(float)
        static constexpr float SECONDS_3_F = 3.0f;                  // 3�b(float)
        static constexpr float SECONDS_4_F = 4.0f;                  // 4�b(float)
        static constexpr float SECONDS_5_F = 5.0f;                  // 5�b(float)
        static constexpr float SECONDS_6_F = 6.0f;                  // 6�b(float)
        static constexpr float SECONDS_7_F = 7.0f;                  // 7�b(float)
        static constexpr float SECONDS_8_F = 8.0f;                  // 8�b(float)
        static constexpr float SECONDS_9_F = 9.0f;                  // 9�b(float)
        static constexpr float SECONDS_10_F = 10.0f;                // 10�b(float)


        // �t�H���g�T�C�Y�萔
        static constexpr int32_t FONT_SIZE_10 = 10;                 //10(�T�C�Y)
        static constexpr int32_t FONT_SIZE_20 = 20;                 //20(�T�C�Y)
        static constexpr int32_t FONT_SIZE_30 = 30;                 //30(�T�C�Y)
        static constexpr int32_t FONT_SIZE_40 = 40;                 //40(�T�C�Y)
        static constexpr int32_t FONT_SIZE_50 = 50;                 //50(�T�C�Y)
        static constexpr int32_t FONT_SIZE_60 = 60;                 //60(�T�C�Y)
        static constexpr int32_t FONT_SIZE_70 = 70;                 //70(�T�C�Y)
        static constexpr int32_t FONT_SIZE_80 = 80;                 //80(�T�C�Y)
        static constexpr int32_t FONT_SIZE_90 = 90;                 //90(�T�C�Y)
        static constexpr int32_t FONT_SIZE_100 = 100;               //100(�T�C�Y)

        static constexpr int32_t DEFAULT_FONT_SIZE_ = 16;           //16(�f�t�H���g�T�C�Y)

        
    }
}
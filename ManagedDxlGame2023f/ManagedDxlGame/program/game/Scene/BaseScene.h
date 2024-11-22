//------------------------------------------------------------------------------------------------------------
//
//シーンの基底クラス
//
//-------------------------------------------------------------------------------------------------------------



#pragma once
#include"../Object/NPC.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Map/Mapchip.h"


class BaseScene {
public:

    virtual void Update(float delta_time) = 0;
    virtual void Draw() = 0;
  
    BaseScene() = default;
    virtual ~BaseScene() = default;

    //シーンの状態
    enum class SceneState {
        //タイトル
        TITTLE,
        //インマップ(村や町、城など)
        INMAP,
        //ワールドマップ
        WORLDMAP,
        //バトル
        BATTLE,
        //リザルト
        RESULT
    };

     //現在のシーンを取得する
    SceneState GetCurentScene()const {
        return curent_scene;
    }

    //他の所でシーケンスを切り替える
    virtual void SequenceChangeMove() {};

protected:

    

    //ストーリー
    enum Story {    
        //プロローグ
        PROLOGUE,
        //エピローグ
        EPILOGUE
    };

    SceneState curent_scene = SceneState::TITTLE;

    //警告用のフラグ
    bool warnig_message = false;

    //Npcリスト
    std::list<Shared<Npc>>npc_list;

    //---mapChip関連の変数---//
    std::string gpc_map_chip_hdls_pass_ = "graphics/image_(2024_0115_0231).png";							// 画像パス
    int map_chip_width_ = 32;																				// マップチップの幅
    int map_chip_height_ = 32;																				// マップチップの高さ
    int map_chip_all_size_ = 0;																				// マップチップの総フレーム数
    int map_chip_x_size_ = 8;																				// マップチップの横フレーム数
    int map_chip_y_size_ = 262;																				// マップチップの縦フレーム数
    int gpc_map_chip_hdls_[2096] = {};																		// 画像データ格納
    std::string map_data_csv_pass_;																			// マップCSVデータのパス
    int map_height = 0;																					        //マップの高さ
    int map_width = 0;																					        //マップの幅

    //マップチップを格納する場所
    std::list<Shared<MapChip>> map_chips;
    //オブジェクト用のマップチップ
    std::list<Shared<MapChip>> map_chip_object;

    //マップチップのポインタ
    Shared<MapChip>mapchip = nullptr;

    //フェードアウト時間
    const float FADE_OUT_TIME = 0.1f;
    //フェードイン時間
    const float FADE_IN_TIME = 1.0f;

    //当たり判定補正値
    const float HIT_JUDGMENT_CORRECTION_VALUE = 0;
};



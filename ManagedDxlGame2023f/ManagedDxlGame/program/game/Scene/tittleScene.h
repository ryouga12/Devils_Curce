//------------------------------------------------------------------------------------------------------------
//
//タイトルシーン(最初にプレイヤーの名前を決めるキャラメイクシーンとプロローグを流すシーンがある)
//
//-------------------------------------------------------------------------------------------------------------


#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include "../Manager/SceneManager.h"
#include "../Manager/GameManager.h"
#include "../Manager/SoundManager.h"

class UIManager;

class TittleScene final: public BaseScene {
public:

    TittleScene();
    ~TittleScene() override;

    void Update(float delta_time) override;
    void Draw() override;

    // タイトル内で使う処理全般を分ける
    enum class TittleState {

        // 最初の選択画面
        TITTLE,
        //キャラの画像の選択画面
        CHARAIMAGE,
        // 冒険を始めた時のキャラメイク
        CHARANAME,
        //プロローグ
        PROLOGUE
    };

    void SetTittleState(TittleState new_tittle) {
        curent_tittle = new_tittle;
    }

private:

    //プレイヤーのキャラメイクで選択できるイメージの列挙体
    enum PlayerImageSelect {
        //男性一つ目
        PLAYER_IMAGE_MAN_0,
        //男性二つ目
        PLAYER_IMAGE_MAN_1,
        //女性一つ目
        PLAYER_IMAGE_WOMAN_0,
        //女性二つ目
        PLAYER_IMAGE_WOMAN_1,
        //最大数
        PLAYER_IMAGE_MAX
    };

    //キャラメイクで使用する座標
    const tnl::Vector2i PlAYER_IMAGE_SELECT_POS[PLAYER_IMAGE_MAX] = {
        {300 ,300},
        {500, 300},
        {700, 300},
        {900 ,300},
    };

    //プレイヤーの画像を表示する為の格納しておく配列
    int player_image_hdl[PLAYER_IMAGE_MAX];

    //プレイヤーの画像のハンドル
    const std::string PLAYER_HDL[PLAYER_IMAGE_MAX] =
    {
     "graphics/Player/player_image_0/Player_Icon.png",
     "graphics/Player/player_image_01/Player_Icon.png",
     "graphics/Player/player_girl_image_02/Player_Icon.png",
     "graphics/Player/player_girl_image_03/Player_Icon.png"
    };

    //カーソル
    int cousour = 0;

    //現在のカーソルの位置
    int curent_player_image_hdl = PlayerImageSelect::PLAYER_IMAGE_MAN_0;

    //プレイヤーからの名前の受付が完了したかの確認フラグ
    bool player_input_name_flag = false;

    // 名前の入力 (今の所、10文字で固定)
    char temp[8] = { 0 };

    // メニュー関連の定数
    const int MENU_Y = 450;
    const int MENU_X = 500;
    const int MENU_WIDTH = 250;
    const int MENU_HEIGHT = 200;

    // タイトル関連の定数
    const int TITLE_TOP = 200;
    const int TITLE_LEFT = 450;
    const int FONT_SIZE_TITLE = 60;
    const int FONT_SIZE_DEFAULT = 16;

    // メニュー選択肢関連の定数
    const int SELECTION_NUM = 2;
    const int SELECTION_START_X = 550;
    const int SELECTION_START_Y = 500;
    const int SELECTION_OFFSET_Y = 50;

    //タイトルの文字の座標
    const tnl::Vector2i TITTLE_STRING_POS = { 380, 200 };

    // タイトル内の状態
    TittleState curent_tittle = TittleState::TITTLE;

    // シーケンス関連
    tnl::Sequence<TittleScene> sequence_ = tnl::Sequence<TittleScene>(this, &TittleScene::seqInit);

    // 背景画像
    int tittle_ghdl = 0;

    // エンターキーの画像
    int enter_key_hdl = 0;

    // 最初に遷移した時の座標
    const tnl::Vector3 FIRST_POS_ = { 195, 312, 0 };

    // ポインタ
    Shared<Menu> menu_window = nullptr;
    Shared<Menu> menu_window_white = nullptr;

    Shared<MenuWindow> title_selection = nullptr;
    MenuWindow::MenuElement_t* tittle_selection_coment = nullptr;

    // シーンを切り替える
    bool SceneChange = false;
    bool seqChangeScene(float delta_time) { return true; }

    // 初期化シーケンス
    bool seqInit(float delta_time);

    // タイトル内の更新処理
    bool seqTittle(float delta_time);

    // キャラメイク内の更新処理
    bool seqCharaMike(float delta_time);

    //プロローグの更新処理
    bool seqPrologue(float delta_time);
    

};

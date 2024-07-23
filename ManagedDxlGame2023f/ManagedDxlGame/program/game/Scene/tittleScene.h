#pragma once

#include "../../dxlib_ext/dxlib_ext.h"
#include "../Manager/SceneManager.h"
#include "../Manager/GameManager.h"
#include "../Manager/SoundManager.h"

class UIManager;

class TittleScene : public BaseScene {
public:
    TittleScene();
    ~TittleScene() override;

    void Update(float delta_time) override;
    void Draw() override;

    // タイトル内で使う処理全般を分ける
    enum class TittleState {
        // 最初の選択画面
        TITTLE,
        // 冒険を始めた時のキャラメイク
        CHARAMIKE,
    };

    void SetTittleState(TittleState new_tittle) {
        curent_tittle = new_tittle;
    }

private:

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

    // タイトル内の状態
    TittleState curent_tittle = TittleState::TITTLE;

    // シーケンス関連
    tnl::Sequence<TittleScene> sequence_ = tnl::Sequence<TittleScene>(this, &TittleScene::seqInit);

    // 背景画像
    int tittle_ghdl = 0;

    // エンターキーの画像
    int enter_key_hdl = 0;

    // 最初に遷移した時の座標
    tnl::Vector3 first_pos_ = { 195, 312, 0 };

    // 遅延させる秒数
    float Time = 1.0f;

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
};

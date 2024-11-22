#include "tittleScene.h"
#include"../Manager/UiManager.h"
#include"InMapScene.h"

//------------------------------------------------------------------------------------------------------------------------
//初期化　&  解放

TittleScene::TittleScene()
{
	//現在のシーンをTittleSceneに設定
	curent_scene = SceneState::TITTLE ;

	menu_window = std::make_shared<Menu>("graphics/WindowBase_01.png");

	menu_window_white = std::make_shared<Menu>("graphics/WindowBase_02.png");
	
	UIManager::GetUIManager()->addMenu("menu_window", menu_window);
	UIManager::GetUIManager()->addMenu("menu_window_white", menu_window_white);

	//選択ウィンドウの初期化
	tittle_selection_coment = new MenuWindow::MenuElement_t[]{
		{SELECTION_START_X , SELECTION_START_Y ,					  "最初からはじめる" , MenuWindow::Elements::FIRST_ELEMENT},
		{SELECTION_START_X , SELECTION_START_Y + SELECTION_OFFSET_Y , "続きからはじめる" , MenuWindow::Elements::SECOND_ELEMENT},
	};

	title_selection = std::make_shared<MenuWindow>("graphics/WindowBase_01.png", tittle_selection_coment, SELECTION_NUM);
	title_selection->Open();

	//ポインタをリセットする
	GameManager::GetGameManager()->Reset();
	
	//ゲームマネージャーのポインタを生成する
	GameManager::GetGameManager()->PoiterCreate();

}

TittleScene::~TittleScene()
{
	//サウンドを止める
	SoundManager::GetSoundManager()->StopSound("sound/BGM/tittle_.mp3");
}

//------------------------------------------------------------------------------------------------------------------------
//更新処理

void TittleScene::Update(float delta_time)
{
	//シーケンスの更新処理
	sequence_.update(delta_time);

}

//------------------------------------------------------------------------------------------------------------------------
//描画

void TittleScene::Draw()
{
	//上ボタンの座標
	const tnl::Vector2i TOP_BUTTON_POS = { 450, 500 };

	//下ボタンの座標
	const tnl::Vector2i UNDER_BUTTON_POS = { 450 , 580 };

	//左ボタンの座標
	const tnl::Vector2i LEFT_BUTTON_POS = { 350, 650 };

	//右ボタンの座標
	const tnl::Vector2i RIGHT_BUTTON_POS = { 450 , 650 };

	//上下矢印の座標(ボタンから矢印の差)
	const tnl::Vector2i BUTTON_ARROW_OFFSET = { 15 , 60 };

	//左右ボタンのオフセット座標
	const tnl::Vector2i ARROW_LEFT_AND_RIGHT_OFFSET = { 30 , 7 };

	//決定ボタンの描画座標
	const tnl::Vector2i ENTER_DECISION_DRAW_POS = { 200 , 650 };

	//Enterキーの決定文字の座標
	const tnl::Vector2i ENTER_DECISION_STRING_POS = { 250 , 643 };

	// プレイヤーの画像を選択してくださいというメッセージの座標
	const tnl::Vector2i PLAYER_SELECT_PROMPT_POS = {450 , 50};

	switch (curent_tittle)
	{
		//タイトルシーン
	case TittleScene::TittleState::TITTLE:

		//背景描画
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tittle_ghdl, TRUE);

		//タイトルメニューの要素
		title_selection->All(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
		
		//タイトル文字を表示する
		//DrawStringToHandle(TITTLE_STRING_POS.x, static_cast<int>(title_y), "Devils Curse", koni::Color::WHITE, UIManager::GetUIManager()->GetFontString_80());
		DrawRotaGraph(TITTLE_STRING_POS.x, static_cast<int>(title_y), koni::Numeric::SCALE_ONE_F, 0, font_hdl, true);

		//上ボタン操作アイコン
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_w.png", TOP_BUTTON_POS.x, TOP_BUTTON_POS.y, koni::Numeric::SCALE_ONE_THIRTY_F, 0, true);
		//上矢印の描画
		DrawStringToHandle(TOP_BUTTON_POS.x - BUTTON_ARROW_OFFSET.x, TOP_BUTTON_POS.y - BUTTON_ARROW_OFFSET.y, "↑", koni::Color::WHITE , UIManager::GetUIManager()->GetFontString_30());
		//下ボタン操作アイコン
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_s.png", UNDER_BUTTON_POS.x, UNDER_BUTTON_POS.y, koni::Numeric::SCALE_ONE_THIRTY_F, 0, true);
		//下矢印の描画
		DrawStringToHandle(UNDER_BUTTON_POS.x - BUTTON_ARROW_OFFSET.x, UNDER_BUTTON_POS.y + BUTTON_ARROW_OFFSET.y, "↓", koni::Color::WHITE, UIManager::GetUIManager()->GetFontString_30());


		break;

		//キャラメイクシーン
	case TittleScene::TittleState::CHARAIMAGE:

		//カーソルを表示する 
		DrawRotaGraph(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].x, PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].y - 150, koni::Numeric::SCALE_ONE_F, tnl::ToRadian(90), cousour, true);

		//画像を表示する
		for (int i = 0; i < PLAYER_IMAGE_MAX; i++) {
			DrawRotaGraph(PlAYER_IMAGE_SELECT_POS[i].x, PlAYER_IMAGE_SELECT_POS[i].y, koni::Numeric::SCALE_TRIPLE_F, 0, player_image_hdl[i], true);
		}

		//左ボタン操作アイコン
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_a.png", LEFT_BUTTON_POS.x, LEFT_BUTTON_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//左文字の描画
		DrawString(LEFT_BUTTON_POS.x + ARROW_LEFT_AND_RIGHT_OFFSET.x, LEFT_BUTTON_POS.y - ARROW_LEFT_AND_RIGHT_OFFSET.y, "左", koni::Color::WHITE);

		//右ボタン操作アイコン
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_d.png", RIGHT_BUTTON_POS.x, RIGHT_BUTTON_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//右文字の描画
		DrawString(RIGHT_BUTTON_POS.x + ARROW_LEFT_AND_RIGHT_OFFSET.x, RIGHT_BUTTON_POS.y - ARROW_LEFT_AND_RIGHT_OFFSET.y, "右", koni::Color::WHITE);

		//Enterキーの決定文字の描画
		DrawString(PLAYER_SELECT_PROMPT_POS.x, PLAYER_SELECT_PROMPT_POS.y, "プレイヤーの画像を選択してください", koni::Color::WHITE);

		//決定ボタンの描画
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/button_Enter.png", ENTER_DECISION_DRAW_POS.x, ENTER_DECISION_DRAW_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//Enterキーの決定文字の描画
		DrawString(ENTER_DECISION_STRING_POS.x, ENTER_DECISION_STRING_POS.y, "決定", koni::Color::WHITE);

		break;

	case TittleScene::TittleState::CHARANAME:

		//画像を表示する
		for (int i = 0; i < PLAYER_IMAGE_MAX; i++) {
			DrawRotaGraph(PlAYER_IMAGE_SELECT_POS[i].x, PlAYER_IMAGE_SELECT_POS[i].y, koni::Numeric::SCALE_TRIPLE_F, 0, player_image_hdl[i], true);
		}

		//左ボタン操作アイコン
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_a.png", LEFT_BUTTON_POS.x, LEFT_BUTTON_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//左文字の描画
		DrawString(LEFT_BUTTON_POS.x + ARROW_LEFT_AND_RIGHT_OFFSET.x, LEFT_BUTTON_POS.y - ARROW_LEFT_AND_RIGHT_OFFSET.y, "左", koni::Color::WHITE);

		//右ボタン操作アイコン
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon_d.png", RIGHT_BUTTON_POS.x, RIGHT_BUTTON_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//右文字の描画
		DrawString(RIGHT_BUTTON_POS.x + ARROW_LEFT_AND_RIGHT_OFFSET.x, RIGHT_BUTTON_POS.y - ARROW_LEFT_AND_RIGHT_OFFSET.y, "右", koni::Color::WHITE);

		//決定ボタンの描画
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/button_Enter.png", ENTER_DECISION_DRAW_POS.x, ENTER_DECISION_DRAW_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		//Enterキーの決定文字の描画
		DrawString(ENTER_DECISION_STRING_POS.x, ENTER_DECISION_STRING_POS.y, "決定", koni::Color::WHITE);

		if (!player_input_name_flag) {

			//名前入力の座標
			const tnl::Vector2i NAME_INPUT_STRING_POS = { 500 , 500 };

			//ウィンドウのの座標
			const tnl::Vector2i NAME_INPUT_WINDOW_POS = { 490 , 480 };

			//ウィンドウのサイズ
			const tnl::Vector2i NAME_INPUT_WINDOW_SIZE = { 200 , 50 };

			//名前を入力してくださいという文字の座標
			const tnl::Vector2i NAME_STRING_POS = {450 , 450};

			// プレイヤー画像を囲むボックスのオフセット（幅と高さ）
			const int PLAYER_SELECT_BOX_OFFSET_ = 80;

			// 選択したプレイヤーの画像を囲む為のBOXの座標とサイズ
			const tnl::Vector3 PLAYER_SELECT_BOX_POS_TOP_LEFT = {
				static_cast<float>(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].x - PLAYER_SELECT_BOX_OFFSET_),
				static_cast<float>(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].y - PLAYER_SELECT_BOX_OFFSET_),
				0
			};
			const tnl::Vector3 PLAYER_SELECT_BOX_POS_BOTTOM_RIGHT = {
				static_cast<float>(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].x + PLAYER_SELECT_BOX_OFFSET_),
				static_cast<float>(PlAYER_IMAGE_SELECT_POS[curent_player_image_hdl].y + PLAYER_SELECT_BOX_OFFSET_),
				0
			};

			// 名前を入力してくださいというログの描画
			DrawString(NAME_STRING_POS.x, NAME_STRING_POS.y, "プレイヤーの名前を入力してください", koni::Color::WHITE);

			//枠の太さ
			const float THICKNESS = 3.0f;

			// 選択したプレイヤーにDrawBoxで囲んでわかりやすくする
			DrawBoxAA(
				PLAYER_SELECT_BOX_POS_TOP_LEFT.x,
				PLAYER_SELECT_BOX_POS_TOP_LEFT.y,
				PLAYER_SELECT_BOX_POS_BOTTOM_RIGHT.x,
				PLAYER_SELECT_BOX_POS_BOTTOM_RIGHT.y,
				koni::Color::WHITE, 
				// 塗りつぶさない
				FALSE,				
				THICKNESS
			);

			//ウィンドウを表示する
			UIManager::GetUIManager()->Menu_Draw("menu_window", NAME_INPUT_WINDOW_POS.x, NAME_INPUT_WINDOW_POS.y, NAME_INPUT_WINDOW_SIZE.x, NAME_INPUT_WINDOW_SIZE.y);

			//プレイヤーが入力できる文字の長さ
			const int PLAYER_NAME_LENGTH = 8;

			//プレイヤーの名前を受け付ける
			KeyInputString(NAME_INPUT_STRING_POS.x, NAME_INPUT_STRING_POS.y, PLAYER_NAME_LENGTH, temp, FALSE);

			//名前を保存する
			player_input_name_flag = true;
		}

		break;

		//プロローグ
	case TittleScene::TittleState::PROLOGUE:

		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_50_PERCENT);
		//背景描画
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, tittle_ghdl, TRUE);
		//アルファ値を戻す
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, koni::Numeric::ALPHA_OPAQUE);

		//ストーリーを描画する
		UIManager::GetUIManager()->StoryDisplay(koni::Color::WHITE);

		break;

	default:

		break;
	}

	if (warnig_message) {

		//ウィンドウを表示する
		UIManager::GetUIManager()->GetMenu("menu_window")->Menu_draw(50, 100, 500, 200);
		//文字を表示する
		UIManager::GetUIManager()->WarningWindow("ファイルが存在しません。");
	}

}

//------------------------------------------------------------------------------------------------------------------------
//シーケンス関連

//初期化シーケンス
bool TittleScene::seqInit(float delta_time)
{
	switch (curent_tittle)
	{
	case TittleScene::TittleState::TITTLE:

		//タイトルの背景の画像を格納する
		tittle_ghdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/tittle_background.png");
		font_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/DevilsCurse.png");
		//handle = ResourceManager::getResourceManager()->LoadGraphEX("graphics/tittle_string.png");
		//エンターキーの画像を格納する
		enter_key_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/button_Enter.png");
		
		SoundManager::GetSoundManager()->Sound_Play("sound/BGM/tittle_.mp3", DX_PLAYTYPE_LOOP);

		sequence_.change(&TittleScene::seqTittle);

		break;

	case TittleScene::TittleState::CHARAIMAGE:

		//カーソルの画像を格納する
		cousour = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/cur_sannkaku2.png");

		//プレイヤーの画像を格納する
		for (int i = 0; i < PLAYER_IMAGE_MAX; i++) {
			player_image_hdl[i] = ResourceManager::GetResourceManager()->LoadGraphEX(PLAYER_HDL[i]);
		}

		sequence_.change(&TittleScene::seqCharaMike);

		break;

	case TittleScene::TittleState::PROLOGUE:

		tittle_ghdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/haikei/prologue_.jpg");

		sequence_.change(&TittleScene::seqPrologue);

		break;

	default:
		break;
	}


	return false;
}

//タイトル内の更新処理
bool TittleScene::seqTittle(float delta_time)
{
	//---タイトル文字のアニメーション処理---//
	elapsed_time += delta_time;

	if (elapsed_time < DURATION) {

		//ゆっくり目標に向かって動かす
		title_y = tnl::SmoothLerp(0.0f, static_cast<float>(TITTLE_STRING_POS.y), DURATION, elapsed_time);
	}

	//---選択処理---//

	//はじめから始める
	if (title_selection->GetSelectNum() == MenuWindow::Elements::FIRST_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);

		//警告文字が表示されている場合、警告文字を消す
		if (warnig_message) { warnig_message = false; }

		auto mgr = SceneManager::GetSceneManager();

		if (mgr->GetSceneFlag()) {
			//フラグを切り替えてシーンを遷移させないようにする
			mgr->SceneFlagChange();
		}

		//フェードを入れる
		mgr->FadeScene(FADE_OUT_TIME);

		//イベント関連のフラグをリセットする
		EventManager::GetEventManager()->EventFlagReset();

		//キャラメイクに移動させる
		//まず画像の選択を行ってもらう
		curent_tittle = TittleState::CHARAIMAGE;
		sequence_.change(&TittleScene::seqInit);

	}
	//続きから始める
	else if(title_selection->GetSelectNum() == MenuWindow::Elements::SECOND_ELEMENT && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)){

		//EnterキーのSEを鳴らす
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/tittle_enter.mp3", DX_PLAYTYPE_BACK);

		if (GameManager::GetGameManager()->GetPlayer()->PlayerLoad()) {

			sequence_.change(&TittleScene::seqChangeScene);
			auto mgr = SceneManager::GetSceneManager();

			auto& player = GameManager::GetGameManager()->GetPlayer();

			mgr->changeScene(new InMapScene(player->GetPlayerPos(), static_cast<InMapScene::InMapState>(GameManager::GetGameManager()->GetPlayer()->GetPlayerCurentMapState())));

			//プレイヤーの画像を確定する
			player->SetPlayerAnimationHdl(Player::PlayerEquipState::BARE_HANDS, player->GetPlayerID());

			//プレイヤーの座標をカメラに保管する
			GameManager::GetGameManager()->GetCamera()->SavePosition(GameManager::GetGameManager()->GetPlayer()->GetPlayerPos());
		}
		//ファイルがなかった場合、警告を表示する
		else {

			//警告文字を表示する
			warnig_message = true;
		}
	}

	return false;
	
}

//キャラメイク
//後ほど追加予定
bool TittleScene::seqCharaMike(float delta_time)
{
	if (curent_tittle == TittleState::CHARAIMAGE) {

		//カーソルの動き
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_A)) {
			curent_player_image_hdl--;
			//seをならす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
			//一番左より左に行くと一番右に移動する
			if (curent_player_image_hdl < 0) curent_player_image_hdl = PLAYER_IMAGE_WOMAN_1;
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_D)) {
			curent_player_image_hdl++;
			//seをならす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/cousour_bgm.mp3", DX_PLAYTYPE_BACK);
			//一番左より左に行くと一番右に移動する
			if (curent_player_image_hdl >= PLAYER_IMAGE_MAX) curent_player_image_hdl = PLAYER_IMAGE_MAN_0;
		}
		//エンターキーが押されたらプレイヤーのIDを決定する
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			//プレイヤーを取得する
			auto& player = GameManager::GetGameManager()->GetPlayer();

			//プレイヤーのIDをセットする(プレイヤーの画像を指定する為)
			player->SetPlayerID(curent_player_image_hdl);

			//状態をプレイヤーの名前設定に移動する
			curent_tittle = TittleState::CHARANAME;
		}
	}
	else if (curent_tittle == TittleState::CHARANAME) {

		//プレイヤーの名前が入力し終わったら
		if (player_input_name_flag) {

			//プレイヤーを取得する
			auto& player = GameManager::GetGameManager()->GetPlayer();

			//プレイヤーの名前を設定する
			player->SetPlayerName(temp);

			//プレイヤーの画像を確定する
			player->SetPlayerAnimationHdl(Player::PlayerEquipState::BARE_HANDS, player->GetPlayerID());

			//シーケンスをプロローグに移動する
			sequence_.change(&TittleScene::seqInit);

			curent_tittle = TittleState::PROLOGUE;

			player_input_name_flag = false;
		}
		
	}

	return false;
}

//プロローグ
bool TittleScene::seqPrologue(float delta_time)
{
	if (sequence_.isStart()) {
		//プロローグのロード
		UIManager::GetUIManager()->StoryLoad(Story::PROLOGUE);
		
	}

	//プロローグを流しきるか、タブキーを押してスキップして村シーンに遷移させる
	if (UIManager::GetUIManager()->StoryDisplayUpdate(delta_time) && tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)
		|| tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) {
		sequence_.change(&TittleScene::seqChangeScene);
		auto mgr = SceneManager::GetSceneManager();

		//フラグがfalseの場合trueに変更する(シーンを遷移させる為)
		if (!mgr->GetSceneFlag()) { mgr->SceneFlagChange(); }

		mgr->changeScene(new InMapScene(FIRST_POS_, InMapScene::InMapState::VILLAGE));
		return false;
	}

	return false;
}



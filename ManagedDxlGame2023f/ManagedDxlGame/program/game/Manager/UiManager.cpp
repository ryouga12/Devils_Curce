#include "UiManager.h"
#include"CsvManager.h"
#include"GameManager.h"

UIManager* UIManager::getUIManager()
{
	static UIManager* p_instance = nullptr;

	if (!p_instance)p_instance = new UIManager();
	return p_instance;
}

void UIManager::addMenu(const std::string& menuName, const std::shared_ptr<Menu>& menu)
{
	//メニューマップにあるかを確認して
	auto it = menu_map.find(menuName);

	//すでにあれば処理を飛ばす
	if (it != menu_map.end()) {
		return;
	}
	//もし無ければ要素を追加する
	else {
		menu_map[menuName] = menu;
	}

}

void UIManager::Menu_Draw(const std::string& menuName, const int&  menu_x, const int& menu_y, const int&  menu_width, const int& menu_height)
{
	//名前を検索する
	auto it = menu_map.find(menuName);

	//見つかったら描画する
	if (it != menu_map.end()) {
		(*it).second->Menu_draw(menu_x, menu_y, menu_width, menu_height);
	}
	//見つからなかったら処理を飛ばす
	else {
		return;
	}

}

//武器屋のコメント表示
void UIManager::armsdealerComentDraw(const int& type)
{
	//武器屋に最初に話かけたときのコメント
	if (type == ARMSDEALER_FIRST_COMET) {
		DrawString(WEAPONSHOP.x, WEAPONSHOP.y, "武器屋", koni::Color::WHITE);
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, koni::Color::WHITE, "「 いらっしゃい！」");
		DrawStringEx(WEAPONSHOP_BUY.x, WEAPONSHOP_BUY.y, koni::Color::WHITE, "(1 : 武器を購入する)");
		DrawStringEx(MENU_CLOSE.x, MENU_CLOSE.y, koni::Color::WHITE, "(0 : メニューを閉じる)");
	}
	//なにか買うときのコメント
	else {
		DrawStringEx(ARMSDEALERCOMENT.x, ARMSDEALERCOMENT.y, koni::Color::WHITE, "「 何を買うんだい 」");
	}
}

//コメントをロードする
void UIManager::ComentLoad(const int& max_num , const std::string& name)
{
	//一度コメントを初期化する
	coment_.clear();

	auto coment_csv = CsvManager::getCsvManager()->GetComentCsv();

	for (int i = 1; i < coment_csv.size(); i++) {

		//名前がcsvの1行目の為、0
		if (name == coment_csv[i][0].c_str()) {

			for (int k = 1; k < max_num; k++) {

				coment_.emplace_back(coment_csv[i][k]);

			}
		}

	}
}

//コメントを描画する
void UIManager::ComentDraw(const tnl::Vector2i& coment_pos)
{
	//コメント描画
	DrawStringEx(coment_pos.x, coment_pos.y, koni::Color::WHITE, "%s", coment_[curent_num].c_str());

	//閉じるを表示する座標
	const int ADD_X = 450;  const int ADD_Y = 50;

	//閉じる文字の描画
	DrawStringEx(coment_pos.x + ADD_X, coment_pos.y + ADD_Y, koni::Color::WHITE, "(0 : 閉じる)");

}

//コメントを次に移動する
void UIManager::ComentNextByInput(const int& max_draw_num)
{
	//描画の最大数を超えてなければ
	if (count < max_draw_num) {
		//Enterキーを受け付ける
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//SEを鳴らす
			SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
			//数を増やす
			curent_num++;
			//カウントを増やす
			count++;
		}
	}
}

// プレイヤーのステータスをHPバーで表示する
void UIManager::PlayerStatusDrawWindow()
{
	// プレイヤーアイコンの位置
	const tnl::Vector2i PLAYER_ICON_POS = { 90 ,120 };

	// HPバーの位置、幅、高さ
	const tnl::Vector2i HP_BAR_POS = { 75 ,155 };
	const int HP_BAR_WIDTH = 75;
	const int HP_BAR_HEIGHT = 30;

	// MPバーの位置、幅、高さ
	const tnl::Vector2i MP_BAR_POS = { 75 ,215 };
	const int MP_BAR_WIDTH = 75;
	const int MP_BAR_HEIGHT = 30;

	// プレイヤー情報表示の位置
	const tnl::Vector2i PLAYER_INFO_POS = { 75 ,70 };
	const tnl::Vector2i LEVEL_INFO_POS = { 120 ,110 };
	const int HP_INFO_Y = 190;
	const int MP_INFO_Y = 250;

	// プレイヤーのステータスを取得する
	auto& playerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

	// プレイヤーの画像を描画
	ResourceManager::getResourceManager()->DrawRotaGraphEx("graphics/Player/Player_Icon_sentou.png", PLAYER_ICON_POS.x, PLAYER_ICON_POS.y, koni::Numeric::SCALE_ONE_THIRTY_F, 0, true);

	// HPバーの画像をロード
	int hpber_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/hpbar.png");
	// MPバーの画像をロード
	int mpber_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/mpbar.png");

	// プレイヤーのHPが3割を切った場合、HPバーを赤色に変更
	if (playerStatus.GetcurentHp() < playerStatus.GetMaxHp() * koni::Numeric::PERCENT_30) {
		hpber_hdl = ResourceManager::getResourceManager()->LoadGraphEX("graphics/red1_.png");
	}

	// HPバーを描画
	DrawExtendGraph(HP_BAR_POS.x, HP_BAR_POS.y, (HP_BAR_POS.x + static_cast<int>(current_hp_bar)), HP_BAR_POS.y + HP_BAR_HEIGHT, hpber_hdl, true);
	// MPバーを描画
	DrawExtendGraph(MP_BAR_POS.x, MP_BAR_POS.y, (MP_BAR_POS.x + static_cast<int>(current_mp_bar)), MP_BAR_POS.y + MP_BAR_HEIGHT, mpber_hdl, true);

	// プレイヤーのHP情報を表示
	DrawStringEx(PLAYER_INFO_POS.x, PLAYER_INFO_POS.y, koni::Color::WHITE, "Player");
	DrawStringEx(LEVEL_INFO_POS.x, LEVEL_INFO_POS.y, koni::Color::WHITE, " Lv %d", playerStatus.GetLevel());
	DrawStringEx(PLAYER_INFO_POS.x, HP_INFO_Y, koni::Color::WHITE, " Hp : %d / %d", playerStatus.GetcurentHp(), playerStatus.GetMaxHp());
	DrawStringEx(PLAYER_INFO_POS.x, MP_INFO_Y, koni::Color::WHITE, " Mp : %d / %d", playerStatus.GetCurentMp(), playerStatus.GetMaxMp());
}


//セーブした時の文字を表示させる
void UIManager::SaveText(const tnl::Vector2i& text_pos)
{
	//セーブしている時に
	if (save_flag) {

		//フレームをインクリメント
		frame++;

		DrawStringEx(text_pos.x, text_pos.y, koni::Color::WHITE, "セーブ中です。");

		//フレーム数が60を超えたら
		if (frame >= 60) {

			//フラグを切り替える
			save_flag = false;

		}

	}
	//フレーム数が60を超えてかつフラグが立っていなかったら
	else if (frame >= 60 && !save_flag) {

		//セーブが完了した時のテキストを出す
		DrawStringEx(text_pos.x, text_pos.y, koni::Color::WHITE , "セーブしました。");
	}

}

//プレイヤーの操作説明
void UIManager::PlayerMoveDetail(const std::vector<std::string>& detail_text)
{
	if (player_detail_window_flag) {

		//メニューウィンドウの座標
		const tnl::Vector2i DETAIL_WINDOW_POS = { 850, 300 };

		//メニューウィンドウのサイズ
		const int DETAIL_WINDOW_WIDTH = 400;
		const int DETAIL_WINDOW_HEIGHT = 400;

		//操作説明文字の座標
		const tnl::Vector2i MOVE_DETAIL_STRING_POS = { 900, 350 };

		auto menu_window = getMenu("menu_window");

		menu_window->Menu_draw(DETAIL_WINDOW_POS.x, DETAIL_WINDOW_POS.y, DETAIL_WINDOW_WIDTH, DETAIL_WINDOW_HEIGHT);

		for (int i = 0; i < detail_text.size(); i++)
		{
			DrawStringEx(MOVE_DETAIL_STRING_POS.x, MOVE_DETAIL_STRING_POS.y + (i * ADD_OFSET), koni::Color::WHITE, "%s", detail_text[i].c_str());
		}
	}
}

//HPバーやMPバーの計算
void UIManager::PlayerStatusBarUpdate(const float delta_time)
{
	// プレイヤーのステータスを取得する
	auto& playerStatus = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

	// HPバー最大の長さ
	int maxHpBar = 200;
	// MPバーの最大の長さ
	int maxMpBer = 200;

	// HPの割合を計算する
	float hp_ratio = static_cast<float>(playerStatus.GetcurentHp()) / playerStatus.GetMaxHp();
	float newHpBar = maxHpBar * hp_ratio;

	// HPバーの補間を更新
	if (std::fabs(newHpBar - hp_end) > FLT_EPSILON) {
		hp_start = current_hp_bar;
		hp_end = newHpBar;
		hp_lerp_time = 0.0f;
	}

	hp_lerp_time += delta_time;

	hp_lerp_time = (hp_lerp_time > LERPDURATION) ? LERPDURATION : hp_lerp_time;

	current_hp_bar = tnl::SmoothLerp(hp_start, hp_end, LERPDURATION, hp_lerp_time, 0);

	// MPの割合を計算する
	float mp_ratio = static_cast<float>(playerStatus.GetCurentMp()) / playerStatus.GetMaxMp();
	float newMpBar = maxMpBer * mp_ratio;

	// MPバーの補間を更新
	if (std::fabs(newMpBar - mp_end) > FLT_EPSILON) {
		mp_start = current_mp_bar;
		mp_end = newMpBar;
		mp_lerp_time = 0.0f;
	}
	mp_lerp_time += delta_time; 

	mp_lerp_time = (mp_lerp_time > LERPDURATION) ? LERPDURATION : mp_lerp_time;

	current_mp_bar = tnl::SmoothLerp(mp_start, mp_end, LERPDURATION, mp_lerp_time, 0);
}


bool UIManager::StoryDisplayUpdate(const float delta_time)
{
	story_display_timer += delta_time;

	// タイマーが間隔を超えたら次のコメントに追加
	if (story_display_timer > koni::Numeric::SECONDS_1_AND_HALF) {
		story_display_timer = 0.0f; // タイマーをリセット

		// ストーリーが空でない場合、次のコメントを追加
		if (current_story_comment_index < story_.size()) {
			displayed_story_coments.push_back(story_[current_story_comment_index]);
			current_story_comment_index++;
		}

	}
	else {

		// コメントが全て表示されたらtrueを返す
		if (current_story_comment_index >= story_.size()) {
			
			return true;
		}
	}

	return false;
}

void UIManager::StoryDisplay(const int& font_color)
{
	// 初期Y座標
	int y_offset = 80;
	//X座標
	const int STORY_COMENT_X = 100;

    for (const auto& comment : displayed_story_coments) {
        DrawStringEx(STORY_COMENT_X, y_offset, font_color, "%s", comment.c_str());
        y_offset += ADD_OFSET; // 各コメントの表示位置をずらす
    }
}

void UIManager::StoryLoad(const int& section_type)
{
	//一度コメントを初期化する
	displayed_story_coments.clear();
	current_story_comment_index = 0;
	story_.clear();

	auto story_csv = CsvManager::getCsvManager()->GetStoryCsv();

	for (int i = 1; i < story_csv.size(); i++) {

		//どのタイプなのかを確認してストーリーを格納する
		if (section_type == std::stoi(story_csv[i][0].c_str())) {

			story_.emplace_back(story_csv[i][1]);
		}
	}
}

UIManager::~UIManager()
{
	//中のメニューを解放する
	menu_map.clear();
}

//void UIManager::IconAnimation()
//{
//	//ポインタを生成する
//	if (icon_animation == nullptr) {
//
//		//プレイヤーの座標
//		auto palyer_pos = GameManager::getGameManager()->getPlayer()->getPlayerPos();
//
//		//カメラの座標
//		auto camera_pos = GameManager::getGameManager()->getCamera()->getTargetPos();
//
//		// プレイヤーの描画位置を計算
//		tnl::Vector3 draw_pos = palyer_pos - camera_pos + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
//
//		//アイコン用のアニメーション用のポインタを生成する
//		icon_animation = std::make_shared<Animation>("graphics/talk_icon.png", draw_pos.x, draw_pos.y - 80, 3, 1, 32, 32, 3, 8);
//
//		//描画を行う
//		icon_animation->play_animation();
//	}
//	//ポインタが生成場合
//	else {
//
//		//プレイヤーの座標
//		auto palyer_pos = GameManager::getGameManager()->getPlayer()->getPlayerPos();
//
//		//カメラの座標
//		auto camera_pos = GameManager::getGameManager()->getCamera()->getTargetPos();
//
//		// プレイヤーの描画位置を計算
//		tnl::Vector3 draw_pos = palyer_pos - camera_pos + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
//
//		//アイコンの座標を切り替える
//		icon_animation->SetAnimationPos(draw_pos.x , draw_pos.y - 50);
//
//		//描画を行う
//		icon_animation->play_animation();
//	}
//
//}

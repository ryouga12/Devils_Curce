#include "UiManager.h"
#include"CsvManager.h"
#include"GameManager.h"
#include"ResourceManager.h"

UIManager* UIManager::GetUIManager()
{
	static UIManager* p_instance = nullptr;

	if (!p_instance)p_instance = new UIManager();
	return p_instance;
}

void UIManager::DeleteGetUIManager()
{
	delete GetUIManager();
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

//コメントをロードする
void UIManager::ComentLoad(const int& max_num , const std::string& name)
{
	//一度コメントを初期化する
	coment_.clear();

	auto coment_csv = CsvManager::GetCsvManager()->GetComentCsv();

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
	const int ADD_X = 380;  const int ADD_Y = 50;

	//閉じる文字の描画
	DrawStringEx(coment_pos.x + ADD_X, coment_pos.y + ADD_Y, koni::Color::WHITE, "(back space : 閉じる)");

}

//コメントを次に移動する
void UIManager::ComentNextByInput(const int& max_draw_num)
{
	//描画の最大数を超えてなければ
	if (count < max_draw_num) {
		//Enterキーを受け付ける
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			//SEを鳴らす
			SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/decision.mp3", DX_PLAYTYPE_BACK);
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
	auto& player = GameManager::GetGameManager()->GetPlayer();

	// プレイヤーの画像を描画
	DrawRotaGraph(PLAYER_ICON_POS.x, PLAYER_ICON_POS.y, koni::Numeric::SCALE_ONE_THIRTY_F, 0, player->GetPlayerIcon(), true);

	// HPバーの画像をロード
	int hpber_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/hpbar.png");
	// MPバーの画像をロード
	int mpber_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/mpbar.png");

	// プレイヤーのHPが3割を切った場合、HPバーを赤色に変更
	if (player->GetPlayerStatusSave().GetcurentHp() < player->GetPlayerStatusSave().GetMaxHp() * koni::Numeric::PERCENT_30) {
		hpber_hdl = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/red1_.png");
	}

	// HPバーを描画
	DrawExtendGraph(HP_BAR_POS.x, HP_BAR_POS.y, (HP_BAR_POS.x + static_cast<int>(current_hp_bar)), HP_BAR_POS.y + HP_BAR_HEIGHT, hpber_hdl, true);
	// MPバーを描画
	DrawExtendGraph(MP_BAR_POS.x, MP_BAR_POS.y, (MP_BAR_POS.x + static_cast<int>(current_mp_bar)), MP_BAR_POS.y + MP_BAR_HEIGHT, mpber_hdl, true);

	// プレイヤーのHP情報を表示
	DrawStringEx(PLAYER_INFO_POS.x, PLAYER_INFO_POS.y, koni::Color::WHITE, "%s" , GameManager::GetGameManager()->GetPlayer()->GetPlayerName().c_str());
	DrawStringEx(LEVEL_INFO_POS.x, LEVEL_INFO_POS.y, koni::Color::WHITE, " Lv %d", player->GetPlayerStatusSave().GetLevel());
	DrawStringEx(PLAYER_INFO_POS.x, HP_INFO_Y, koni::Color::WHITE, " Hp : %d / %d", player->GetPlayerStatusSave().GetcurentHp(), player->GetPlayerStatusSave().GetMaxHp());
	DrawStringEx(PLAYER_INFO_POS.x, MP_INFO_Y, koni::Color::WHITE, " Mp : %d / %d", player->GetPlayerStatusSave().GetCurentMp(), player->GetPlayerStatusSave().GetMaxMp());

	//ステータスアイコン(攻撃)
	const int STATUS_ATTACK_ICON = GetStatusAttackIconHdl();
	//ステータスアイコン(防御)
	const int STATUS_DEFANCE_ICON = GetStatusDefanceIconHdl();
	//ステータスの無効値(上記の定数と比較して値が変動しているかを確認する為)
	const int STATUS_NON_CHANGE = 0;
	//ステータスアイコン(攻撃)のx座標オフセット
	const int STATUS_ATTACK_X_OFFSET = 110;
	//ステータスアイコン(防御)のx座標オフセット
	const int STATUS_DEFANCE_X_OFFSET = 160;

	//ステータスが変動していれば
	if (STATUS_ATTACK_ICON > STATUS_NON_CHANGE) {

		// ステータスのアイコンを描画
		DrawRotaGraph(PLAYER_ICON_POS.x + STATUS_ATTACK_X_OFFSET, PLAYER_ICON_POS.y, koni::Numeric::SCALE_ONE_F, 0, STATUS_ATTACK_ICON, true);

	}
	//ステータスが変動していれば
	if (STATUS_DEFANCE_ICON > STATUS_NON_CHANGE) {

		// ステータスのアイコンを描画
		DrawRotaGraph(PLAYER_ICON_POS.x + STATUS_DEFANCE_X_OFFSET, PLAYER_ICON_POS.y, koni::Numeric::SCALE_ONE_F, 0, STATUS_DEFANCE_ICON, true);

	}

}


//セーブした時の文字を表示させる
void UIManager::SaveText(const tnl::Vector2i& text_pos)
{
	//フレームの最大値
	const int MAX_FLAME = 60;

	//セーブしている時に
	if (save_flag) {

		//フレームをインクリメント
		frame++;

		DrawStringEx(text_pos.x, text_pos.y, koni::Color::WHITE, "セーブ中です。");

		//フレーム数が60を超えたら
		if (frame >= MAX_FLAME) {

			//フラグを切り替える
			save_flag = false;

		}

	}
	//フレーム数が60を超えてかつフラグが立っていなかったら
	else if (frame >= MAX_FLAME && !save_flag) {

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

		Menu_Draw("menu_window",DETAIL_WINDOW_POS.x, DETAIL_WINDOW_POS.y, DETAIL_WINDOW_WIDTH, DETAIL_WINDOW_HEIGHT);

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
	auto& playerStatus = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();

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

//ストーリーを流す
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
			
			story_end_flag = true;

			return true;
		}
	}

	return false;
}

//ストーリーの描画
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

	if (story_end_flag) {

		//画像を表示する
		ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/button_Enter.png", ENTER_KEY_POS.x, ENTER_KEY_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

		DrawStringEx(ENTER_KEY_STRING_POS.x, ENTER_KEY_STRING_POS.y, font_color, "次に進む");
	}

	//画像を表示する
	//タブキーでスキップできるようにする為、表示する
	ResourceManager::GetResourceManager()->DrawRotaGraphEx("graphics/icon146.png", TAB_KEY_POS.x, TAB_KEY_POS.y, koni::Numeric::SCALE_ONE_F, 0, true);

	DrawStringEx(TAB_KEY_STRING_POS.x, TAB_KEY_STRING_POS.y, font_color, "ストーリーをスキップする");
}

//ストーリーをcsvからロードする
void UIManager::StoryLoad(const int& section_type)
{
	//一度コメントを初期化する
	displayed_story_coments.clear();
	current_story_comment_index = 0;
	story_end_flag = false;
	story_.clear();

	auto story_csv = CsvManager::GetCsvManager()->GetStoryCsv();

	for (int i = 1; i < story_csv.size(); i++) {

		//どのタイプなのかを確認してストーリーを格納する
		if (section_type == std::stoi(story_csv[i][0].c_str())) {

			story_.emplace_back(story_csv[i][1]);
		}
	}
}

//警告用のUIを表示する
void UIManager::WarningWindow(const std::string& warnig_message)
{
	const tnl::Vector2i WARNING_MESSAGE_POS = { 80 , 130 };

	//警告文字を表示する
	DrawStringEx(WARNING_MESSAGE_POS.x, WARNING_MESSAGE_POS.y, koni::Color::WHITE, "%s" , warnig_message.c_str());

}

//イベント用の文字を表示する(アイテムを使用した際など)
void UIManager::DisplayEventMessage()
{
	//ウィンドウの通知フラグがtrueだったら
	if (is_notification_displayed) {

		//ウィンドウの座標
		const tnl::Vector2i DISPLAY_WINDOW_POS = { 50, 500 };

		//ウィンドウの幅と高さ
		const int DISPLAY_WINDOW_HEIGHT = 200;
		const int DISPLAY_WINDOW_WIDTH = 700;

		//閉じる文字の座標
		const tnl::Vector2i DISPLAY_CLOSE_STRING_POS = { 550 , 650 };

		//ウィンドウの表示を行う
		GetMenu("menu_window")->Menu_draw(DISPLAY_WINDOW_POS.x, DISPLAY_WINDOW_POS.y, DISPLAY_WINDOW_WIDTH, DISPLAY_WINDOW_HEIGHT);

		//文字の表示
		DrawStringEx(DISPLAY_WINDOW_POS.x + ADD_OFSET, DISPLAY_WINDOW_POS.y + ADD_OFSET, koni::Color::WHITE, "%s", display_event_message.c_str());

		//閉じる文字
		DrawStringEx(DISPLAY_CLOSE_STRING_POS.x, DISPLAY_CLOSE_STRING_POS.y, koni::Color::WHITE, "back space : 閉じる");
	}
}

//デバック用の操作説明
void UIManager::DebugDetailWindow(const BaseScene::SceneState& curent_scene_state)
{
	//ワールドマップにいる時のみ
	if (curent_scene_state == BaseScene::SceneState::WORLDMAP && player_detail_window_flag) {

		//メニューウィンドウの座標
		const tnl::Vector2i DETAIL_WINDOW_POS = { 50, 305 };

		//メニューウィンドウのサイズ
		const int DETAIL_WINDOW_WIDTH = 400;
		const int DETAIL_WINDOW_HEIGHT = 400;

		//操作説明文字の座標
		const tnl::Vector2i MOVE_DETAIL_STRING_POS = { 100, 335 };

		Menu_Draw("menu_window", DETAIL_WINDOW_POS.x, DETAIL_WINDOW_POS.y, DETAIL_WINDOW_WIDTH, DETAIL_WINDOW_HEIGHT);

		for (int i = 0; i < debug_detail_texts.size(); i++)
		{
			DrawStringEx(MOVE_DETAIL_STRING_POS.x, MOVE_DETAIL_STRING_POS.y + (i * ADD_OFSET), koni::Color::WHITE, "%s", debug_detail_texts[i].c_str());
		}
	}
}

UIManager::UIManager()
{
	//フォントを作成する
	string_handle_80 = CreateFontToHandle(NULL, koni::Numeric::FONT_SIZE_80, koni::Numeric::FONT_SIZE_10, DX_FONTTYPE_EDGE);

	//フォントを作成する
	string_handle_100 = CreateFontToHandle(NULL, koni::Numeric::FONT_SIZE_100, koni::Numeric::FONT_SIZE_10, DX_FONTTYPE_EDGE);

	//フォントを作成する
	string_handle_30 = CreateFontToHandle(NULL, koni::Numeric::FONT_SIZE_30, koni::Numeric::FONT_SIZE_10, DX_FONTTYPE_EDGE);
}

UIManager::~UIManager()
{
	//中のメニューを解放する
	menu_map.clear();

	// 作成したフォントデータを削除する
	DeleteFontToHandle(string_handle_80);
	DeleteFontToHandle(string_handle_100);
	DeleteFontToHandle(string_handle_30);

}

//状況によって防御系のアイコンを返す(主にステータスなどのアイコン)
int UIManager::GetStatusDefanceIconHdl()
{
	//ベースの防御力
	int player_base_defance = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetBaseDefance();

	//現在の防御力
	int curent_player_defance = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetDefance();

	//インベントリ
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//武器を装備している場合
	if (!inventory->GetEquipAromorArray().empty()) {

		for (auto& equip_weqpon : inventory->GetEquipAromorArray()) {

			//プレイヤーの防御力が上昇していたら
			if (player_base_defance + equip_weqpon.GetItemDefance() < curent_player_defance) {

				int status_defance_up_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_defance_up.png");

				//ステータスアップのアイコンを返す
				return status_defance_up_icon;
			}
			//プレイヤーの防御力が減少していたら
			else if ((player_base_defance + equip_weqpon.GetItemDamage()) > curent_player_defance) {

				int status_defance_down_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_defance_down.png");

				//ステータスダウンのアイコンを返す
				return status_defance_down_icon;
			}
		}
	}
	//装備していなくて攻撃力が上昇してれば
	else if (player_base_defance < curent_player_defance) {

		int status_defance_up_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_defance_up.png");

		//ステータスアップのアイコンを返す
		return status_defance_up_icon;
	}
	//装備していなくて攻撃力が減少していれば
	else if (player_base_defance > curent_player_defance) {

		int status_defance_down_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_defance_down.png");

		//ステータスダウンのアイコンを返す
		return status_defance_down_icon;

	}

	//処理がうまくいかなかった場合マイナス値を渡す
	return ERROR_VALUE;
}

//状況によって攻撃系のアイコンを返す(主にステータスなどのアイコン)
int UIManager::GetStatusAttackIconHdl()
{

	//ベースの攻撃力
	int player_base_attck = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetBaseAttack();

	//攻撃力
	int curent_player_attack = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetAttack();

	//インベントリ
	auto& inventory = GameManager::GetGameManager()->GetInventory();

	//武器を装備している場合
	if (!inventory->GetEquipWeaponArray().empty()) {

		for (auto& equip_weqpon : inventory->GetEquipWeaponArray()) {

			//プレイヤーの攻撃力が上昇していたら
			if ((player_base_attck + equip_weqpon.GetItemDamage()) < curent_player_attack) {

				int status_attack_up_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_attack_up.png");

				//ステータスアップのアイコンを返す
				return status_attack_up_icon;
			}
			//プレイヤーの攻撃力が減少していたら
			else if ((player_base_attck + equip_weqpon.GetItemDamage()) > curent_player_attack) {

				int status_attack_down_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_attack_down.png");

				//ステータスダウンのアイコンを返す
				return status_attack_down_icon;
			}
		}
	}
	//装備していなくて攻撃力が上昇してれば
	else if (player_base_attck < curent_player_attack) {

		int status_attack_up_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_attack_up.png");

		//ステータスアップのアイコンを返す
		return status_attack_up_icon;
	}
	//装備していなくて攻撃力が減少していれば
	else if (player_base_attck > curent_player_attack) {

		int status_attack_down_icon = ResourceManager::GetResourceManager()->LoadGraphEX("graphics/State_Icon/status_attack_down.png");

		//ステータスダウンのアイコンを返す
		return status_attack_down_icon;

	}


	//処理がうまくいかなかった場合マイナス値を渡す
	return ERROR_VALUE;
	
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

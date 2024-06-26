#include "ResourceManager.h"
#include "../../dxlib_ext/dxlib_ext.h"

ResourceManager* ResourceManager::getResourceManager()
{
	static ResourceManager* p_instance = nullptr;

	if (!p_instance)p_instance = new ResourceManager();
	return p_instance;
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	//画像を消去する
	ghmap.clear();
}

void ResourceManager::Update()
{
	
}

void ResourceManager::Draw()
{

	
}


//画像を重複して読み込まないようにこれを使う
int ResourceManager::LoadGraphEX(std::string gh)
{

	auto it = ghmap.find(gh);
	if (it != ghmap.end()) {
		return ghmap[gh];
	}

	else {
		int loadgh = LoadGraph(gh.c_str());
		ghmap.insert(std::make_pair(gh, loadgh));
	}


	return ghmap[gh];
}

//保存した画像を消去する
bool ResourceManager::deleteGraphEx(std::string ghpath)
{
	if (auto it = ghmap.find(ghpath); it != ghmap.end()) {
		if (DeleteSoundMem(it->second) != -1) {
			ghmap.erase(ghpath);
			return true;
		}
		else {
			tnl::DebugTrace("\ ResourceManagerリソース解放 => %s のメモリ解放に失敗しました \n", ghpath.c_str());
			return false;
		}
	}

	// 発見できなかった場合
	tnl::DebugTrace("\n ResourceManagerリソース解放 => %s が見つかりませんでした \n", ghpath.c_str());
	return false;

}

//ロードした画像を描画する
void ResourceManager::DrawRotaGraphEx(const std::string& ghpath, int x, int y, double Exlate, double Angle , int TransFlag)
{
	int ghdl = LoadGraphEX(ghpath);
	DrawRotaGraph(x, y, Exlate, Angle, ghdl, TransFlag);

}

//当たり判定と座標補正
//講師が用意してくれた関数です
int ResourceManager::IsIntersectRectToCorrectPosition(tnl::Vector3& a_now, const tnl::Vector3& a_prev, const int a_rect_size_w, const int a_rect_size_h, const tnl::Vector3& b, const int b_rect_size_w, const int b_rect_size_h)
{
	{

		const int NO_HIT = 0;
		const int CORRECT_LEFT = 1;
		const int CORRECT_RIGHT = 2;
		const int CORRECT_UP = 3;
		const int CORRECT_DOWN = 4;

		if (!tnl::IsIntersectRect(a_now, a_rect_size_w, a_rect_size_h, b, b_rect_size_w, b_rect_size_h)) return NO_HIT;

		tnl::Vector3 a_near = tnl::GetNearestRectPoint(a_prev, (float)a_rect_size_w, (float)a_rect_size_h, b);
		int n = GetRegionPointAndRect(a_near, b, b_rect_size_w, b_rect_size_h);

		if (0 == n) {
			float y = (a_now - a_prev).y;
			// 下向きに移動している場合は上に補正
			if (y >= 0) {
				a_now.y = b.y - (b_rect_size_h / 2) - (a_rect_size_h / 2);
				return CORRECT_UP;
			}
			// 上向きに移動している場合は左右どちらかに補正
			else {
				// 右に補正
				if (a_now.x > b.x) {
					a_now.x = b.x + (b_rect_size_w / 2) + (a_rect_size_w / 2);
					return CORRECT_RIGHT;
				}
				// 左に補正
				else {
					a_now.x = b.x - (b_rect_size_w / 2) - (a_rect_size_w / 2);
					return CORRECT_LEFT;
				}
			}
		}
		else if (2 == n) {
			float y = (a_now - a_prev).y;
			// 上向きに移動している場合は下に補正
			if (y <= 0) {
				a_now.y = b.y + (b_rect_size_h / 2) + (a_rect_size_h / 2);
				return CORRECT_DOWN;
			}
			// 上向きに移動している場合は左右どちらかに補正
			else {
				// 右に補正
				if (a_now.x > b.x) {
					a_now.x = b.x + (b_rect_size_w / 2) + (a_rect_size_w / 2);
					return CORRECT_RIGHT;
				}
				// 左に補正
				else {
					a_now.x = b.x - (b_rect_size_w / 2) - (a_rect_size_w / 2);
					return CORRECT_LEFT;
				}
			}
		}
		else if (1 == n) {
			float x = (a_now - a_prev).x;
			// 左向きに移動している場合は右に補正
			if (x <= 0) {
				a_now.x = b.x + (b_rect_size_w / 2) + (a_rect_size_w / 2);
				return CORRECT_RIGHT;
			}
			// 右向きに移動している場合は上下どちらかに補正
			else {
				// 上に補正
				if (a_now.y < b.y) {
					a_now.y = b.y - (b_rect_size_h / 2) - (a_rect_size_h / 2);
					return CORRECT_UP;
				}
				// 下に補正
				else {
					a_now.y = b.y + (b_rect_size_h / 2) + (a_rect_size_h / 2);
					return CORRECT_DOWN;
				}
			}
		}
		else if (3 == n) {
			float x = (a_now - a_prev).x;
			// 右向きに移動している場合は左に補正
			if (x >= 0) {
				a_now.x = b.x - (b_rect_size_w / 2) - (a_rect_size_w / 2);
				return CORRECT_LEFT;
			}
			// 左向きに移動している場合は上下どちらかに補正
			else {
				// 上に補正
				if (a_now.y < b.y) {
					a_now.y = b.y - (b_rect_size_h / 2) - (a_rect_size_h / 2);
					return CORRECT_UP;
				}
				// 下に補正
				else {
					a_now.y = b.y + (b_rect_size_h / 2) + (a_rect_size_h / 2);
					return CORRECT_DOWN;
				}
			}
		}
		return NO_HIT;
	}
}

int ResourceManager::GetRegionPointAndRect(const tnl::Vector3& p, const tnl::Vector3& rp, const int rect_w, const int rect_h)
{
	tnl::Vector3 v1 = tnl::Vector3::Normalize({ float(rect_h), float(rect_w), 0 });
	tnl::Vector3 v2 = tnl::Vector3::Normalize({ float(rect_h), float(-rect_w), 0 });
	//		tnl::Vector3 vc1 = v1.cross(tnl::Vector3(0, 0, 1));
	//		tnl::Vector3 vc2 = v2.cross(tnl::Vector3(0, 0, 1));
	tnl::Vector3 vc1 = v1;
	tnl::Vector3 vc2 = v2;

	int s1 = tnl::GetSidesPointAndPlane(p, vc1, rp);
	int s2 = tnl::GetSidesPointAndPlane(p, vc2, rp);
	if (s1 >= 0 && s2 >= 0) {
		return 1;
	}
	else if (s1 >= 0 && s2 <= 0) {
		return 2;
	}
	else if (s1 <= 0 && s2 >= 0) {
		return 0;
	}
	else {
		return 3;
	}
}

//std::vector<int> ResourceManager::LoadAnimationGraphs(const std::string& gh_pass, int frame_count) {
//
//	std::vector<int> graph_handles;
//	for (int i = 0; i < frame_count; ++i) {
//		std::string file_path = gh_pass; 
//		int handle = LoadGraphEX(file_path);
//		graph_handles.push_back(handle);
//	}
//	return graph_handles;
//
//}
//
//int ResourceManager::CreateAnimation(const std::string& gh_pass, int x, int y, int frame_count, int delay)
//{
//	// アニメーションの画像をロード
//	std::vector<int> graph_handles = LoadAnimationGraphs(gh_pass, frame_count);
//
//	// アニメーションオブジェクトを作成
//	Animation* animation = new Animation(x, y, frame_count, delay);
//	animation->set_graph_handles(graph_handles);
//
//	// アニメーションハンドルを生成してマップに登録
//	int handle = ++m_next_handle;
//	m_animation_map[handle] = animation;
//
//	return handle;
//}
//
//Animation* ResourceManager::GetAnimation(int handle)
//{
//	auto it = m_animation_map.find(handle);
//	if (it != m_animation_map.end()) {
//		return it->second;
//	}
//	return nullptr; // 見つからなかった場合はnullptrを返す
//}
//

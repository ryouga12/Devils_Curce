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
int ResourceManager::LoadGraphEX(const std::string& gh)
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
			tnl::DebugTrace("ResourceManagerリソース解放 = > % s のメモリ解放に失敗しました \n", ghpath.c_str());
			return false;
		}
	}

	// 発見できなかった場合
	tnl::DebugTrace("ResourceManagerリソース解放 => %s が見つかりませんでした \n", ghpath.c_str());
	return false;

}

//ロードした画像を描画する
void ResourceManager::DrawRotaGraphEx(const std::string& ghpath, const int& x, const int& y, const double& Exlate, const double& Angle, const int& TransFlag)
{
	int ghdl = LoadGraphEX(ghpath);
	DrawRotaGraph(x, y, Exlate, Angle, ghdl, TransFlag);

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

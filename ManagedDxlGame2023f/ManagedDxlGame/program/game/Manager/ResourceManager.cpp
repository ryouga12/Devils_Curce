#include "ResourceManager.h"
#include "../../dxlib_ext/dxlib_ext.h"

ResourceManager* ResourceManager::GetResourceManager()
{
	static ResourceManager* p_instance = nullptr;

	if (!p_instance)p_instance = new ResourceManager();
	return p_instance;
}

void ResourceManager::DelategetResourceManager()
{
	delete GetResourceManager();
}

ResourceManager::~ResourceManager()
{
	//�摜����������
	ghmap.clear();
}

//�摜���d�����ēǂݍ��܂Ȃ��悤�ɂ�����g��
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

//�ۑ������摜����������
bool ResourceManager::DeleteGraphEx(const std::string& ghpath)
{
	if (auto it = ghmap.find(ghpath); it != ghmap.end()) {
		if (DeleteGraph(it->second) != -1) {
			ghmap.erase(ghpath);
			tnl::DebugTrace("\n------------------------------------------------------------");
			tnl::DebugTrace("\nResourceManager���\�[�X��� = > %s �̃���������ɐ������܂���", ghpath.c_str());
			tnl::DebugTrace("\n------------------------------------------------------------");
			return true;

		}
		else {
			tnl::DebugTrace("\nResourceManager���\�[�X��� = > % s �̃���������Ɏ��s���܂���", ghpath.c_str());
			return false;
		}
	}

	// �����ł��Ȃ������ꍇ
	tnl::DebugTrace("\nResourceManager���\�[�X��� => %s ��������܂���ł���", ghpath.c_str());
	return false;

}

//�z����摜���ꊇ�ŉ������
void ResourceManager::DeleteGraphAryEx(int gh_ary[] , const int& max_num)
{
	for (int i = 0; i < max_num; i++) {
		DeleteGraph(gh_ary[i]);
		tnl::DebugTrace("\n Deleted   at [%d]. Pointer value: %p\n", i, gh_ary[i]);
	}
}

//���[�h�����摜��`�悷��
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
//	// �A�j���[�V�����̉摜�����[�h
//	std::vector<int> graph_handles = LoadAnimationGraphs(gh_pass, frame_count);
//
//	// �A�j���[�V�����I�u�W�F�N�g���쐬
//	Animation* animation = new Animation(x, y, frame_count, delay);
//	animation->set_graph_handles(graph_handles);
//
//	// �A�j���[�V�����n���h���𐶐����ă}�b�v�ɓo�^
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
//	return nullptr; // ������Ȃ������ꍇ��nullptr��Ԃ�
//}
//

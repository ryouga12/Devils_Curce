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
	
}

void ResourceManager::Update()
{
	
}

void ResourceManager::Draw()
{

	
}


//�摜���d�����ēǂݍ��܂Ȃ��悤�ɂ�����g��
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

//�ۑ������摜����������
bool ResourceManager::deleteGraphEx(std::string ghpath)
{
	if (auto it = ghmap.find(ghpath); it != ghmap.end()) {
		if (DeleteSoundMem(it->second) != -1) {
			ghmap.erase(ghpath);
			return true;
		}
		else {
			tnl::DebugTrace("\ ResourceManager���\�[�X��� => %s �̃���������Ɏ��s���܂��� \n", ghpath.c_str());
			return false;
		}
	}

	// �����ł��Ȃ������ꍇ
	tnl::DebugTrace("\n ResourceManager���\�[�X��� => %s ��������܂���ł��� \n", ghpath.c_str());
	return false;

}

//���[�h�����摜��`�悷��
void ResourceManager::DrawRotaGraphEx(const std::string& ghpath, int x, int y, double Exlate, double Angle , int TransFlag)
{
	int ghdl = LoadGraphEX(ghpath);
	DrawRotaGraph(x, y, Exlate, Angle, ghdl, TransFlag);

}

//�����蔻��ƍ��W�␳
//�u�t���p�ӂ��Ă��ꂽ�֐��ł�
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
			// �������Ɉړ����Ă���ꍇ�͏�ɕ␳
			if (y >= 0) {
				a_now.y = b.y - (b_rect_size_h / 2) - (a_rect_size_h / 2);
				return CORRECT_UP;
			}
			// ������Ɉړ����Ă���ꍇ�͍��E�ǂ��炩�ɕ␳
			else {
				// �E�ɕ␳
				if (a_now.x > b.x) {
					a_now.x = b.x + (b_rect_size_w / 2) + (a_rect_size_w / 2);
					return CORRECT_RIGHT;
				}
				// ���ɕ␳
				else {
					a_now.x = b.x - (b_rect_size_w / 2) - (a_rect_size_w / 2);
					return CORRECT_LEFT;
				}
			}
		}
		else if (2 == n) {
			float y = (a_now - a_prev).y;
			// ������Ɉړ����Ă���ꍇ�͉��ɕ␳
			if (y <= 0) {
				a_now.y = b.y + (b_rect_size_h / 2) + (a_rect_size_h / 2);
				return CORRECT_DOWN;
			}
			// ������Ɉړ����Ă���ꍇ�͍��E�ǂ��炩�ɕ␳
			else {
				// �E�ɕ␳
				if (a_now.x > b.x) {
					a_now.x = b.x + (b_rect_size_w / 2) + (a_rect_size_w / 2);
					return CORRECT_RIGHT;
				}
				// ���ɕ␳
				else {
					a_now.x = b.x - (b_rect_size_w / 2) - (a_rect_size_w / 2);
					return CORRECT_LEFT;
				}
			}
		}
		else if (1 == n) {
			float x = (a_now - a_prev).x;
			// �������Ɉړ����Ă���ꍇ�͉E�ɕ␳
			if (x <= 0) {
				a_now.x = b.x + (b_rect_size_w / 2) + (a_rect_size_w / 2);
				return CORRECT_RIGHT;
			}
			// �E�����Ɉړ����Ă���ꍇ�͏㉺�ǂ��炩�ɕ␳
			else {
				// ��ɕ␳
				if (a_now.y < b.y) {
					a_now.y = b.y - (b_rect_size_h / 2) - (a_rect_size_h / 2);
					return CORRECT_UP;
				}
				// ���ɕ␳
				else {
					a_now.y = b.y + (b_rect_size_h / 2) + (a_rect_size_h / 2);
					return CORRECT_DOWN;
				}
			}
		}
		else if (3 == n) {
			float x = (a_now - a_prev).x;
			// �E�����Ɉړ����Ă���ꍇ�͍��ɕ␳
			if (x >= 0) {
				a_now.x = b.x - (b_rect_size_w / 2) - (a_rect_size_w / 2);
				return CORRECT_LEFT;
			}
			// �������Ɉړ����Ă���ꍇ�͏㉺�ǂ��炩�ɕ␳
			else {
				// ��ɕ␳
				if (a_now.y < b.y) {
					a_now.y = b.y - (b_rect_size_h / 2) - (a_rect_size_h / 2);
					return CORRECT_UP;
				}
				// ���ɕ␳
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
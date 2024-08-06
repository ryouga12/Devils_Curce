#include "Animation.h"

Animation::Animation(const std::string& gh_pass, const int& x, const int& y, const int& width_num, const int& height_num, const int& width_size, const int& height_size, const int& frame_count, const int& delay , const float& size)
	:m_draw_pos_x(x), m_draw_pos_y(y), m_width_num(width_num), m_height_num(height_num), m_width_size(width_size), m_height_size(height_size), m_frame_num(frame_count),
	m_current_frame(0), m_frame_interval(delay), m_timer(0), m_state(AnimationState::STOP), m_gh_ar(nullptr),m_size(size)
{
	m_gh_ar = new int[m_frame_num];

	m_graph_handle = gh_pass;

	LoadDivGraph (m_graph_handle.c_str(), m_frame_num, m_width_num, m_height_num, m_width_size, m_height_size, m_gh_ar);

}

Animation::~Animation()
{
	//ƒ|ƒCƒ“ƒ^‚Ì‰ð•ú
	if (m_gh_ar != nullptr) {
		delete[] m_gh_ar;
		m_gh_ar = nullptr;
	}
}


void Animation::play_animation()
{
	if (!stop_flag) {
		m_state = AnimationState::PLAYING;
	}
}

void Animation::stop_animation()
{
	m_state = AnimationState::STOP;
	m_current_frame = 0;
	m_timer = 0;
}

void Animation::pause_animation()
{
	m_state = AnimationState::PAUSE;
}

void Animation::update(const float deltatime)
{
	if (m_state == AnimationState::PLAYING) {
		m_timer++;
		if (m_timer >= m_frame_interval) {
			m_timer = 0;
			m_current_frame++;
			if (m_current_frame >= m_frame_num) {
				m_current_frame = 0;
			}
		}
	}
	
}

void Animation::draw()
{
	if (m_gh_ar != nullptr && (m_state == AnimationState::PLAYING || m_state == AnimationState::PAUSE)) {
		DrawRotaGraph(m_draw_pos_x, m_draw_pos_y, m_size, 0, m_gh_ar[m_current_frame], true);
	}
}

bool Animation::is_finished() const
{
	return false;
}


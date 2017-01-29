#include "BasicCamera.h"

BasicCamera::BasicCamera()
	: m_pos(0, 10, 10)
	, m_target(0, 0, 0)
	, m_dist(10)
	, m_dir(0, 1, 1)
{
	auto offset = MDX::Vector3fMul(m_dir, m_dist);
	m_pos = MDX::Vector3fAdd(m_target, offset);
}

BasicCamera::~BasicCamera()
{
}

void BasicCamera::Update(MDX::View* view)
{
	if (GetKeyState(VK_UP) < 0) {
		m_dist -= 0.1f;
	}
	else if (GetKeyState(VK_DOWN) < 0) {
		m_dist += 0.1f;
	}

	auto offset = MDX::Vector3fMul(m_dir, m_dist);
	m_pos = MDX::Vector3fAdd(m_target, offset);

	view->SetView(m_pos, m_target);
}
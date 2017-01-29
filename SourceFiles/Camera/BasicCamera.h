#ifndef __BASIC_CAMERA_H__
#define __BASIC_CAMERA_H__

#include "../../MDX/MDX.h"

class BasicCamera
{
public:
	BasicCamera();
	~BasicCamera();

	void Update(MDX::View* view);

private:
	Vector3f	m_pos;
	Vector3f	m_target;
	float		m_dist;
	Vector3f	m_dir;
};


#endif // !__BASIC_CAMERA_H__

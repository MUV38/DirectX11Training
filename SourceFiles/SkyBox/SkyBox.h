#ifndef __SKY_BOX_H__
#define __SKY_BOX_H__

#include <memory>

#include "../../MDX/MDX.h"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();

public:
	void Init(const char* cubeMapFilename);

	void Release();

	void Update(const Vector3f& viewPos);

private:
	static void DrawLauncher(void* arg);
	void Draw();

private:
	std::unique_ptr<MDX::DXTKMesh>	m_mesh;
	std::shared_ptr<MDX::Shader>	m_shader;
	std::shared_ptr<MDX::Texture>	m_cubeMapTex;
};

#endif // !__SKY_BOX_H__

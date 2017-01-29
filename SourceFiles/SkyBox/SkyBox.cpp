#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
	Release();
}

void SkyBox::Init(const char* cubeMapFilename)
{
	// メッシュ読み込み
	m_mesh.reset(new MDX::DXTKMesh());
	m_mesh->Load("DATA\\SkyBox\\SkyBox.sdkmesh");
	m_mesh->SetOnBeginDraw(&DrawLauncher, this);
	MDX_GET_RENDER_MANAGER->AddActor(m_mesh.get());

	// シェーダー読み込み
	m_shader.reset(new MDX::Shader());
	m_shader->LoadVSFromCso("VS_SkyBox", MDX::Shader::VERTEX_LAYOUT_3D);
	m_shader->LoadPSFromCso("PS_SkyBox");

	// テクスチャ読み込み
	m_cubeMapTex.reset(new MDX::Texture());
	MDX::Texture::LoadInfo loadInfo;
	{
		loadInfo.filename = cubeMapFilename;
		loadInfo.forceSRGB = true;
	}
	m_cubeMapTex->Load(loadInfo);

	m_mesh->SetShader(m_shader);
}

void SkyBox::Release()
{
	m_shader.reset();
}

void SkyBox::Update(const Vector3f& viewPos)
{
	m_mesh->SetPos(viewPos);
}

void SkyBox::DrawLauncher(void * arg)
{
	reinterpret_cast<SkyBox*>(arg)->Draw();
}
void SkyBox::Draw()
{
	m_cubeMapTex->PSSetShaderResource(1);
}
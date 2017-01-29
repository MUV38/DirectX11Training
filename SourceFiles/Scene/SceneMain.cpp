#include "SceneMain.h"
#include "../../MDX/MDX.h"
#include "../SkyBox/SkyBox.h"
#include "../Camera/BasicCamera.h"

MDX::Shader g_shader;
ID3D11Buffer* g_vertexBuffer = nullptr;
MDX::Font g_font;
MDX::View g_view;
MDX::Box g_box;
MDX::Texture g_texture;
MDX::MmfMesh* g_mesh;
SkyBox*	g_skybox;
BasicCamera g_camera;
MDX::Shader g_postEffectShader;

MDX::MmfMesh* g_pbrMesh[10*10];

SceneMain::SceneMain(){

}

SceneMain::~SceneMain(){
	Release();
}

// 初期化
bool SceneMain::Init(){
	g_shader.LoadVSFromCso("VertexShader", MDX::Shader::VERTEX_LAYOUT_3D);
	g_shader.LoadPSFromCso("PixelShader");

	g_postEffectShader.LoadVSFromCso("VS_PostEffect", MDX::Shader::VERTEX_LAYOUT_2D);
	g_postEffectShader.LoadPSFromCso("PS_PostEffect");

	DirectX::XMFLOAT3 vertices[] = {
		DirectX::XMFLOAT3(0.0f, 0.5f, 0.5f),
		DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),
		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f)
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DirectX::XMFLOAT3) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = vertices;

	MDX_GET_DEVICE->CreateBuffer(&bd, &initData, &g_vertexBuffer);

	g_font.Init();

	g_box.Create();

	MDX::Texture::LoadInfo info;
	info.filename = "DATA\\Texture\\tile.tif";
	info.forceSRGB = true;
	g_texture.Load(info);

	g_mesh = new MDX::MmfMesh();
	g_mesh->Load("DATA\\Sphere\\Sphere.mmf");
	MDX_GET_RENDER_MANAGER->AddActor(g_mesh);

	g_skybox = new SkyBox();
	g_skybox->Init("DATA\\SkyBox\\SkyBox1_radiance.dds");

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			g_pbrMesh[j+i*10] = new MDX::MmfMesh();
			g_pbrMesh[j+i*10]->Load("DATA\\Sphere\\Sphere.mmf");
			MDX_GET_RENDER_MANAGER->AddActor(g_pbrMesh[j+i*10]);
		}
	}

	return true;
}

// 解放
bool SceneMain::Release(){
	g_shader.Release();
	g_postEffectShader.Release();
	g_vertexBuffer->Release();
	g_box.Release();
	g_texture.Release();
	g_mesh->Release();
	g_skybox->Release();

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			g_pbrMesh[j + i * 10]->Release();
		}
	}

	return true;
}

// 更新
bool SceneMain::Update(){
	//g_view.SetView(DirectX::XMFLOAT3(0,0, 3), DirectX::XMFLOAT3(0, 0, 0));

	g_camera.Update(&g_view);

	DirectX::XMUINT2 ws = MDX_GET_DX_SYSTEM->GetWindowSize();
	float aspect = ws.x / static_cast<float>(ws.y);
	g_view.SetProjection(DirectX::XM_PI*0.25f, aspect, 1.0f, 1000.0f);
	
	g_view.Apply();

	static float rot = 0.0f;
	rot += 0.001f;
	g_box.SetAngle(DirectX::XMFLOAT3(0, rot, 0));

	g_mesh->SetScale(Vector3f(0.01f, 0.01f, 0.01f));
	//g_mesh->SetAngle(DirectX::XMFLOAT3(0, rot, 0));

	// ライト
	static float lightAngleX = 0;
	static float lightAngleY = 0;
	if (GetKeyState('W') < 0) {
		lightAngleX -= 0.001f;
	}
	else if (GetKeyState('S') < 0) {
		lightAngleX += 0.001f;
	}
	if( GetKeyState('A') < 0 ){
		lightAngleY += 0.001f;
	}else if( GetKeyState('D') < 0 ){
		lightAngleY -= 0.001f;
	}
	Vector3f lightDir(0, 0, -1);
	DirectX::XMMATRIX matX = DirectX::XMMatrixRotationX(lightAngleX);
	DirectX::XMMATRIX matY = DirectX::XMMatrixRotationY(lightAngleY);
	DirectX::XMVECTOR vec;
	vec = DirectX::XMLoadFloat3(&lightDir);
	vec = DirectX::XMVector3Normalize(vec);
	vec = DirectX::XMVector3TransformCoord(vec, matX);
	vec = DirectX::XMVector3TransformCoord(vec, matY);
	DirectX::XMStoreFloat3(&lightDir, vec);

	// 環境の構築
	MDX::RenderManager::CBEnvironment env;
	env.viewPos = g_view.GetViewPos();

	env.vecLight = lightDir;
	MDX_GET_RENDER_MANAGER->UpdateCBEnvironmment(&env);
	
	// PBRパラメーター
	float baseColor[3];
	{
		MDX::RenderManager::CBPbr* pbrParam = g_mesh->GetPbrParam();
		{
			ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("PBR Parameter");
			// ベースカラー設定
			{
				baseColor[0] = pbrParam->baseColor.x; 
				baseColor[1] = pbrParam->baseColor.y; 
				baseColor[2] = pbrParam->baseColor.z;
				ImGui::ColorEdit3("BaseColor", baseColor);
				pbrParam->baseColor.x = baseColor[0];
				pbrParam->baseColor.y = baseColor[1];
				pbrParam->baseColor.z = baseColor[2];
			}

			ImGui::SliderFloat("Roughness", &pbrParam->roughness, 0.0f, 1.0f);
			ImGui::SliderFloat("Metallic", &pbrParam->metallic, 0.0f, 1.0f);
			ImGui::End();
		}
	}

	g_skybox->Update(g_view.GetViewPos());

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			MDX::RenderManager::CBPbr* pbrParam = g_pbrMesh[j + i * 10]->GetPbrParam();
			pbrParam->baseColor.x = baseColor[0];
			pbrParam->baseColor.y = baseColor[1];
			pbrParam->baseColor.z = baseColor[2];

			pbrParam->roughness = (float)i / 10.0f;
			pbrParam->metallic = (float)j / 10.0f;

			float offset = 2.0f;
			g_pbrMesh[j + i * 10]->SetPos(Vector3f(offset * i - offset * 5.0f, 0.0f, offset * j - offset * 5.0f));
			g_pbrMesh[j + i * 10]->SetScale(Vector3f(0.01f, 0.01f, 0.01f));
		}
	}

	return false;
}

// 描画
void SceneMain::Draw(){

	//MDX::Sprite::Render2D(100, 100, 512, 512, g_texture.GetShaderResourceView());
	g_font.Draw("DirectX11 Render Test", 20, 10);
}
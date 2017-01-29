#include "MDX_Mesh.h"
#include "../Shader/MDX_DefaultVertexLayout.h"
#include "../System/MDX_System.h"
#include <fstream>
#include <cassert>

namespace MDX{
	MmfMesh::MmfMesh(){
	}

	MmfMesh::~MmfMesh(){
		Release();
	}

	// ファイル読み込み
	bool MmfMesh::Load(const char* filename){
		std::string extension = GetExtension(filename);
		if(extension != "mmf"){
			return false;
		}

		// ファイル読み込み
		std::ifstream fin;
		fin.open(filename, std::ios::binary);

		if(!fin){
			return false;
		}

		// 頂点情報
		fin.read(reinterpret_cast<char*>(&m_mmfInfo.numVertices), sizeof(int));
		m_mmfInfo.vertices = new Vector3f[m_mmfInfo.numVertices];
		for(int i=0 ; i<m_mmfInfo.numVertices ; i++){
			fin.read(reinterpret_cast<char*>(&m_mmfInfo.vertices[i]), sizeof(Vector3f));
		}
		// 面情報
		fin.read(reinterpret_cast<char*>(&m_mmfInfo.numFaces), sizeof(int));
		m_mmfInfo.faces = new PolygonInfo[m_mmfInfo.numFaces];
		for(int i=0 ; i<m_mmfInfo.numFaces ; i++){
			fin.read(reinterpret_cast<char*>(&m_mmfInfo.faces[i]), sizeof(PolygonInfo));
		}
		// マテリアル情報
		fin.read(reinterpret_cast<char*>(&m_mmfInfo.numMaterials), sizeof(int));
		m_mmfInfo.numFacesEveryMaterial = new int[m_mmfInfo.numMaterials];
		for(int i=0 ; i<m_mmfInfo.numMaterials ; i++){
			fin.read(reinterpret_cast<char*>(&m_mmfInfo.texName[i]), sizeof(char)*MAX_TEXTURE_NAME);
			fin.read(reinterpret_cast<char*>(&m_mmfInfo.numFacesEveryMaterial[i]), sizeof(int));
		}

		fin.close();

		// テクスチャ読み込み
		m_texture = new Texture*[m_mmfInfo.numMaterials];
		MDX::Texture::LoadInfo loadInfo;
		loadInfo.Init();
		loadInfo.forceSRGB = true;
		// 0番インデックスはテクスチャが張られてなかった時に貼るテクスチャ
		m_texture[0] = new Texture();
		loadInfo.filename = "DATA\\MDX_Resource\\no_tex.tif";
		m_texture[0]->Load(loadInfo);
		// フォルダ名を取得
		std::string str = filename;
		UINT str_index = str.find_last_of("\\");
		std::string folder = str.substr(0, str_index+1);
		for(int i=1 ; i<m_mmfInfo.numMaterials ; i++){
			m_texture[i] = new Texture();
			bool flag = true;
			//テクスチャ名がない材質は仮のテクスチャを貼る
			if(m_mmfInfo.texName[i] != "" ){
				loadInfo.filename = folder;
				loadInfo.filename += m_mmfInfo.texName[i];
				flag = m_texture[i]->Load(loadInfo);
			}else{
				loadInfo.filename = "DATA\\MDX_Resource\\no_tex.tif";
				m_texture[i]->Load(loadInfo);
			}
			assert(flag&&"MDX::Meshテクスチャ読み込み失敗"); // テクスチャ読み込み失敗
		}

		//	情報を頂点バッファ、インデックスバッファにコピー
		DefaultVertexInfo*	work = new DefaultVertexInfo[m_mmfInfo.numFaces*3];
		WORD*	indices = new WORD[m_mmfInfo.numFaces*3];
		int index = 0;
		for( int f=0 ; f<m_mmfInfo.numFaces ; f++ ){
			for( int n=0 ; n<3 ; n++ ){
				index = m_mmfInfo.faces[f].indices[n];
				indices[f*3+n] = f*3+n;
				work[f*3+n].pos.x = m_mmfInfo.vertices[index].x;
				work[f*3+n].pos.y = m_mmfInfo.vertices[index].y;
				work[f*3+n].pos.z = m_mmfInfo.vertices[index].z;
				work[f*3+n].normal.x = m_mmfInfo.faces[f].vertexNormals[n].x;
				work[f*3+n].normal.y = m_mmfInfo.faces[f].vertexNormals[n].y;
				work[f*3+n].normal.z = m_mmfInfo.faces[f].vertexNormals[n].z;
				work[f*3+n].uv.x = m_mmfInfo.faces[f].uv[n][0];
				work[f*3+n].uv.y = m_mmfInfo.faces[f].uv[n][1];
				work[f*3+n].color = m_mmfInfo.faces[f].color;
			}
		}

		HRESULT	hr = S_OK;

		//	頂点バッファ作成
		D3D11_BUFFER_DESC	bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(DefaultVertexInfo) * m_mmfInfo.numFaces * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA	initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = work;
		hr = MDX_GET_DEVICE->CreateBuffer( &bd, &initData, &m_vertexBuffer );
		assert(SUCCEEDED(hr) && "MDX::Mesh頂点バッファ作成失敗");

		//	インデックスバッファ作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * m_mmfInfo.numFaces * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = indices;
		hr = MDX_GET_DEVICE->CreateBuffer( &bd, &initData, &m_indexBuffer );
		assert(SUCCEEDED(hr) && "MDX::Meshインデックスバッファ作成失敗");

		SAFE_DELETE_ARRAY( work );
		SAFE_DELETE_ARRAY( indices );

		return true;
	}

	// 解放
	void MmfMesh::Release(){
		SAFE_RELEASE(m_vertexBuffer);
		SAFE_RELEASE(m_indexBuffer);
		m_mmfInfo.Release();
		if( m_texture ){
			for(int i=0 ; i<m_mmfInfo.numMaterials ; i++){
				SAFE_DELETE(m_texture[i]);
			}
		}
		SAFE_DELETE_ARRAY(m_texture);
		BaseActor::Release();
	}

	// 更新
	void MmfMesh::Update(){
		BaseActor::Update();
	}

	// 描画
	void MmfMesh::Draw(){
		BaseActor::Draw();

		//	頂点バッファ設定
		UINT	stride = sizeof(DefaultVertexInfo);
		UINT	offset = 0;
		MDX_GET_IMMEDIATE_CONTEXT->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	
		//	インデックスバッファ設定
		MDX_GET_IMMEDIATE_CONTEXT->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		//	ポリゴンの形成方法設定
		MDX_GET_IMMEDIATE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
		//	材質ごとに描画
		int	renderPolygon = 0;	//	レンダリングされたポリゴン数
		for( int m=0 ; m<m_mmfInfo.numMaterials ; m++ ){
			//	テクスチャ、サンプラ設定
			if(m_texture[m] ){
				m_texture[m]->PSSetShaderResource( 0 );
				MDX_GET_IMMEDIATE_CONTEXT->PSSetSamplers(0, 1, &m_sampler);
				//mdxRenderState::SetPSSamplerState( mdxSamplerState::FILTER_ANISOTROPIC_ADDRESS_WRAP, 0 );
			}
			MDX_GET_IMMEDIATE_CONTEXT->DrawIndexed(m_mmfInfo.numFacesEveryMaterial[m]*3, renderPolygon*3, 0);
			renderPolygon += m_mmfInfo.numFacesEveryMaterial[m];
		}
	}
}
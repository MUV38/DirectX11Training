#include "MDX_Sprite.h"
#include "../System/MDX_System.h"
#include "../Utillity/MDX_Utillity.h"
#include "../RenderState/MDX_RenderState.h"

namespace MDX{

	ID3D11Buffer*	Sprite::m_pVB2D = NULL;
	ID3D11Buffer*	Sprite::m_pVB2DRewrite = NULL;
	ID3D11Buffer*	Sprite::m_pIB2D = NULL;
	ID3D11Buffer*	Sprite::m_pCB2DTransform = NULL;
	Shader			Sprite::m_shader;

	Sprite::Sprite(){
		//m_pVB2D = NULL;
		//m_pVB2DRewrite = NULL;
		//m_pIB2D = NULL;
		//m_pCB2DTransform = NULL;
	}

	Sprite::~Sprite(){
		Release();
	}



	// ������
	bool Sprite::Initialize(){
		HRESULT	hr;

		//	2D�p���_���
		mdx2DVertex	vertices[] = {
			{ -0.5f, -0.5f, 0.0f },
			{ -0.5f, 0.5f, 0.0f },
			{ 0.5f, -0.5f, 0.0f },
			{  0.5f, 0.5f, 0.0f }
		};

		//	2D�p���_�o�b�t�@�쐬
		D3D11_BUFFER_DESC	bd;
		ZeroMemory( &bd, sizeof( bd ) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( mdx2DVertex ) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA	InitData;
		ZeroMemory( &InitData, sizeof( InitData ) );
		InitData.pSysMem = vertices;
		hr = MDX_GET_DEVICE->CreateBuffer( &bd, &InitData, &m_pVB2D );
		assert(SUCCEEDED(hr) && "�|���S���N���X���������s�I�I");

		//	�`�掞�ɏ���������2D�p���_�o�b�t�@
		bd.ByteWidth = sizeof( mdx2DVertexRewrite ) * 4;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = MDX_GET_DEVICE->CreateBuffer( &bd, NULL, &m_pVB2DRewrite );
		assert(SUCCEEDED(hr) && "�|���S���N���X���������s�I�I" );

		//	2D�p�C���f�b�N�X���
		WORD	indices[] = {
			0, 1, 2, 3
		};

		//	2D�p�C���f�b�N�X�o�b�t�@�쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) *4;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = indices;
		hr = MDX_GET_DEVICE->CreateBuffer( &bd, &InitData, &m_pIB2D );
		assert(SUCCEEDED(hr) && "�|���S���N���X���������s�I�I" );

		//	2D���_�ϊ��s��p�R���X�^���g�o�b�t�@�쐬
		ZeroMemory( &bd, sizeof( bd ) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( mdxCB2DTransformMatrix );
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = MDX_GET_DEVICE->CreateBuffer( &bd, NULL, &m_pCB2DTransform );
		assert(SUCCEEDED(hr) && "�|���S���N���X���������s�I�I" );

		// �V�F�[�_�[
		m_shader.LoadVSFromCso("VS_2D", Shader::VERTEX_LAYOUT_2D);
		m_shader.LoadPSFromCso("PS_2D");

		return true;
	}

	// ���
	void Sprite::Release(){
		SAFE_RELEASE( m_pVB2D );
		SAFE_RELEASE( m_pVB2DRewrite );
		SAFE_RELEASE( m_pIB2D );
		SAFE_RELEASE( m_pCB2DTransform );
		m_shader.Release();
	}

	// 2�c�|���S���`��
	void Sprite::Render2D( int x, int y, int width, int height, int srcX, int srcY, int srcWidth, int srcHeight, Texture* texture, float angle, float scale, DWORD color ){
		m_shader.SetShader();
		
		DirectX::XMUINT2 windowSize = MDX_GET_DX_SYSTEM->GetWindowSize();

		//	�ϊ��s��Z�o
		mdxCB2DTransformMatrix	trans;

		//	���[���h�ϊ��s��
		DirectX::XMMATRIX W = DirectX::XMMatrixIdentity();
		DirectX::XMStoreFloat4x4(&trans.matWorld, W);
		trans.matWorld._11 = (float)width;
		trans.matWorld._22 = (float)height;
		trans.matWorld._41 = (float)x - (float)windowSize.x*0.5f;
		trans.matWorld._42 = (float)windowSize.y*0.5f - y;
		trans.matWorld._41 += (float)width*0.5f;
		trans.matWorld._42 += -(float)height*0.5f;
		W = DirectX::XMLoadFloat4x4(&trans.matWorld);

		//	�X�P�[�����O�s��
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale, scale, scale);
	
		//	��]�s��
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationZ(angle);

		DirectX::XMStoreFloat4x4(&trans.matWorld, S * R * W);

		//	�ˉe�s��
		DirectX::XMMATRIX P = DirectX::XMMatrixIdentity();
		DirectX::XMStoreFloat4x4(&trans.matProjection, P);
		trans.matProjection._11 = 2.0f / (float)windowSize.x;
		trans.matProjection._22 = 2.0f / (float)windowSize.y;

		DirectX::XMStoreFloat4x4(&trans.matWorld, DirectX::XMMatrixTranspose((DirectX::XMLoadFloat4x4(&trans.matWorld))));
		DirectX::XMStoreFloat4x4(&trans.matProjection, DirectX::XMMatrixTranspose((DirectX::XMLoadFloat4x4(&trans.matProjection))));
	
		//	�ϊ��s��ݒ�
		MDX_GET_IMMEDIATE_CONTEXT->UpdateSubresource( m_pCB2DTransform, 0, NULL, &trans, 0, 0 );
		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers( 0, 1, &m_pCB2DTransform );

		mdx2DVertexRewrite	data[4];
		//	�t�u�ݒ�
		if( texture ){
			float	offsetX, offsetY, sx, sy, sw, sh;
			offsetX = 1.0f / (float)texture->GetWidth();
			offsetY = 1.0f / (float)texture->GetHeight();
			sx = srcX * offsetX;
			sy = srcY * offsetY;
			sw = (srcX + srcWidth) * offsetX;
			sh = (srcY + srcHeight) * offsetY;
			data[0].tu = sx;	data[0].tv = sh;
			data[1].tu = sx;	data[1].tv = sy;
			data[2].tu = sw;	data[2].tv = sh;
			data[3].tu = sw;	data[3].tv = sy;
		}else{
			data[0].tu = 1;	data[0].tv = 0;
			data[1].tu = 0;	data[1].tv = 0;
			data[2].tu = 1;	data[2].tv = 1;
			data[3].tu = 0;	data[3].tv = 1;
		}
		//	�J���[�ݒ�
		data[0].color = data[1].color = data[2].color = data[3].color = color; 
	
		//	���_�o�b�t�@��������
		D3D11_MAPPED_SUBRESOURCE	msr;
		MDX_GET_IMMEDIATE_CONTEXT->Map( m_pVB2DRewrite, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr );
		memcpy( msr.pData, data, sizeof( mdx2DVertexRewrite )*4 );
		MDX_GET_IMMEDIATE_CONTEXT->Unmap( m_pVB2DRewrite, 0 );

		//	�e�N�X�`��&�T���v���ݒ�
		if( texture ){
			texture->PSSetShaderResource( 0 );
			RenderState::SetPSSamplerState( SamplerStateType::FILTER_POINT_ADDRESS_WRAP, 0 );
		}

		//	���_�o�b�t�@�ݒ�
		UINT	stride = sizeof( mdx2DVertex );
		UINT	offset = 0;
		MDX_GET_IMMEDIATE_CONTEXT->IASetVertexBuffers( 0, 1, &m_pVB2D, &stride, &offset );
		//	�������������_�o�b�t�@�Z�b�g
		stride = sizeof( mdx2DVertexRewrite );
		offset = 0;
		MDX_GET_IMMEDIATE_CONTEXT->IASetVertexBuffers( 1, 1, &m_pVB2DRewrite, &stride, &offset );

		//	�C���f�b�N�X�o�b�t�@�ݒ�
		MDX_GET_IMMEDIATE_CONTEXT->IASetIndexBuffer( m_pIB2D, DXGI_FORMAT_R16_UINT, 0 );

		//	�|���S���̌`�����@�ݒ�
		MDX_GET_IMMEDIATE_CONTEXT->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

		//	�|���S���`��
		MDX_GET_IMMEDIATE_CONTEXT->DrawIndexed( 4, 0, 0 );
	}

	// 2�c�|���S���`��
	void Sprite::Render2D( int x, int y, int width, int height, 
						   MDX::Shader* shader,
						   ID3D11ShaderResourceView* shaderResource,  UINT texWidth, UINT texHeight,
						   float angle, float scale, DWORD color ){

		if(shader){
			shader->SetShader();
		}else{
			m_shader.SetShader();
		}
		
		DirectX::XMUINT2 windowSize = MDX_GET_DX_SYSTEM->GetWindowSize();

		//	�ϊ��s��Z�o
		mdxCB2DTransformMatrix	trans;

		//	���[���h�ϊ��s��
		DirectX::XMMATRIX W = DirectX::XMMatrixIdentity();
		DirectX::XMStoreFloat4x4(&trans.matWorld, W);
		trans.matWorld._11 = (float)width;
		trans.matWorld._22 = (float)height;
		trans.matWorld._41 = (float)x - (float)windowSize.x*0.5f;
		trans.matWorld._42 = (float)windowSize.y*0.5f - y;
		trans.matWorld._41 += (float)width*0.5f;
		trans.matWorld._42 += -(float)height*0.5f;
		W = DirectX::XMLoadFloat4x4(&trans.matWorld);

		//	�X�P�[�����O�s��
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale, scale, scale);
	
		//	��]�s��
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationZ(angle);

		DirectX::XMStoreFloat4x4(&trans.matWorld, S * R * W);

		//	�ˉe�s��
		DirectX::XMMATRIX P = DirectX::XMMatrixIdentity();
		DirectX::XMStoreFloat4x4(&trans.matProjection, P);
		trans.matProjection._11 = 2.0f / (float)windowSize.x;
		trans.matProjection._22 = 2.0f / (float)windowSize.y;

		DirectX::XMStoreFloat4x4(&trans.matWorld, DirectX::XMMatrixTranspose((DirectX::XMLoadFloat4x4(&trans.matWorld))));
		DirectX::XMStoreFloat4x4(&trans.matProjection, DirectX::XMMatrixTranspose((DirectX::XMLoadFloat4x4(&trans.matProjection))));
	
		//	�ϊ��s��ݒ�
		MDX_GET_IMMEDIATE_CONTEXT->UpdateSubresource( m_pCB2DTransform, 0, NULL, &trans, 0, 0 );
		MDX_GET_IMMEDIATE_CONTEXT->VSSetConstantBuffers( 0, 1, &m_pCB2DTransform );

		mdx2DVertexRewrite	data[4];
		//	�t�u�ݒ�
		data[0].tu = 1;	data[0].tv = 0;
		data[1].tu = 0;	data[1].tv = 0;
		data[2].tu = 1;	data[2].tv = 1;
		data[3].tu = 0;	data[3].tv = 1;

		//	�J���[�ݒ�
		data[0].color = data[1].color = data[2].color = data[3].color = color; 
	
		//	���_�o�b�t�@��������
		D3D11_MAPPED_SUBRESOURCE	msr;
		MDX_GET_IMMEDIATE_CONTEXT->Map( m_pVB2DRewrite, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr );
		memcpy( msr.pData, data, sizeof( mdx2DVertexRewrite )*4 );
		MDX_GET_IMMEDIATE_CONTEXT->Unmap( m_pVB2DRewrite, 0 );

		//	�e�N�X�`��&�T���v���ݒ�
		if( shaderResource ){
			MDX_GET_IMMEDIATE_CONTEXT->PSSetShaderResources(0, 1, &shaderResource);
			RenderState::SetPSSamplerState( SamplerStateType::FILTER_POINT_ADDRESS_WRAP, 0 );
		}

		//	���_�o�b�t�@�ݒ�
		UINT	stride = sizeof( mdx2DVertex );
		UINT	offset = 0;
		MDX_GET_IMMEDIATE_CONTEXT->IASetVertexBuffers( 0, 1, &m_pVB2D, &stride, &offset );
		//	�������������_�o�b�t�@�Z�b�g
		stride = sizeof( mdx2DVertexRewrite );
		offset = 0;
		MDX_GET_IMMEDIATE_CONTEXT->IASetVertexBuffers( 1, 1, &m_pVB2DRewrite, &stride, &offset );

		//	�C���f�b�N�X�o�b�t�@�ݒ�
		MDX_GET_IMMEDIATE_CONTEXT->IASetIndexBuffer( m_pIB2D, DXGI_FORMAT_R16_UINT, 0 );

		//	�|���S���̌`�����@�ݒ�
		MDX_GET_IMMEDIATE_CONTEXT->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

		//	�|���S���`��
		MDX_GET_IMMEDIATE_CONTEXT->DrawIndexed( 4, 0, 0 );
	}

}

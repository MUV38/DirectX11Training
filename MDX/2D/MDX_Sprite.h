#ifndef	MDX_SPRITE_H
#define	MDX_SPRITE_H

#include <d3d11.h>
#include "../Math/MDX_Math.h"
#include "../Texture/MDX_Texture.h"
#include "../Shader/MDX_Shader.h"

namespace MDX
{
	/**
	* @brief 2D���_���\����
	*/
	struct mdx2DVertex
	{
		float	x, y, z;
	};
	/**
	* @brief �`�掞�ɏ���������2D���_���\����
	*/
	struct mdx2DVertexRewrite
	{
		float tu, tv;
		DWORD color;
	};
	/**
	* @brief 2D�`��̓��̓��C�A�E�g
	*/
	//static D3D11_INPUT_ELEMENT_DESC	g_inputLayout2D[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_B8G8R8A8_UNORM, 1, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};
	/**
	* @brief 2D���_�ϊ��s��p�R���X�^���g�o�b�t�@
	*/
	struct mdxCB2DTransformMatrix
	{
		Matrix4X4	matWorld;
		Matrix4X4	matProjection;
	};

	//*********************************************************************************************
	//		@name �|���S���`��N���X
	//*********************************************************************************************
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		/**
		* @brief ������
		* @return ������������?
		*/
		static bool Initialize();

		/**
		* @brief ���
		*/
		static void Release();

		/**
		* @brief 2�c�|���S���`��
		* @param[in] x X���W
		* @param[in] y Y���W
		* @param[in] width ����
		* @param[in] height �c��
		* @param[in] srcX �؂���J�n�ʒu�̂w���W
		* @param[in] srcY �؂���J�n�ʒu�̂x���W
		* @param[in] srcWidth �����̐؂���T�C�Y
		* @param[in] srcHeight �c���̐؂���T�C�Y
		* @param[in] texture �e�N�X�`��(���W�X�^��t0�Œ�)
		* @param[in] angle ��]��(���W�A��)
		* @param[in] scale �X�P�[��
		* @param[in] color �F
		*/
		static void Render2D( int x, int y, int width, int height, int srcX, int srcY, int srcWidth, int srcHeight, Texture* texture=nullptr, float angle = 0, float scale = 1, DWORD color = 0xFFFFFFFF );

		/**
		* @brief 2�c�|���S���`��
		* @param[in] x X���W
		* @param[in] y Y���W
		* @param[in] width ����
		* @param[in] height �c��
		* @param[in] shaderResource �e�N�X�`��(���W�X�^��t0�Œ�)
		* @param[in] texWidth �e�N�X�`������
		* @param[in] texHeight �e�N�X�`���c��
		* @param[in] angle ��]��(���W�A��)
		* @param[in] scale �X�P�[��
		* @param[in] color �F
		*/
		static void Render2D( int x, int y, int width, int height,
							  MDX::Shader* shader = nullptr,
							  ID3D11ShaderResourceView* shaderResource=nullptr, UINT texWidth = 1, UINT texHeight=1,
							  float angle = 0, float scale = 1, DWORD color = 0xFFFFFFFF );

	private:
		//	2D�p�o�b�t�@
		static ID3D11Buffer*	m_pVB2D;		//	�ύX����Ȃ�
		static ID3D11Buffer*	m_pVB2DRewrite;	//	�`�掞�ɏ���������
		static ID3D11Buffer*	m_pIB2D;
		static ID3D11Buffer*	m_pCB2DTransform;
		static Shader			m_shader;


	}; // class Sprite
} // namespace MDX

#endif // MDX_SPRITE_H

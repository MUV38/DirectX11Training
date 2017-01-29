/**
* @file MDX_Shader.h
* @brief �V�F�[�_�[�֘A�̒�`
* @author Toru Miyawaki
*/

#ifndef MDX_SHADER_H
#define MDX_SHADER_H

#include <d3d11.h>

/**�V�F�[�_�[��ǂݍ��ރ��[�g�p�X�̒萔*/
#define ROOT_PAHT_SHADER "DATA/Shader/";

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief �V�F�[�_�[�N���X
	*/
	class Shader{
	public:
		/**
		* @brief ���_���C�A�E�g�̎��
		*/
		enum VertexLayoutType{
			VERTEX_LAYOUT_3D,
			VERTEX_LAYOUT_2D,
			VERTEX_LAYOUT_TYPE_MAX
		};
	public:
		Shader();
		~Shader();

		/**
		* @brief VertexShader��cso�t�@�C���ǂݍ���
		* @param [in] filename �t�@�C����
		* @param [in] vertexLayoutType ���_���C�A�E�g�̎��
		* @return �t�@�C�����ǂݍ��߂���
		* "DATA/Shader"���f�t�H���g�p�X�Ȃ̂Œ���
		*/
		bool LoadVSFromCso(const char* filename, VertexLayoutType vevertexLayoutTypertex);
		
		/**
		* @brief PixelShader��cso�t�@�C���ǂݍ���
		* @param [in] filename �t�@�C����
		* @return �t�@�C�����ǂݍ��߂���
		* "DATA/Shader"���f�t�H���g�p�X�Ȃ̂Œ���
		*/
		bool LoadPSFromCso(const char* filename);

		/**
		* @brief ���
		*/
		void Release();

		/**
		* @brief �V�F�[�_�[��ݒ�
		*/
		void SetShader();
	private:

		ID3D11VertexShader* m_vertexShader = nullptr;
		ID3D11PixelShader* m_pixelShader = nullptr;
		ID3D11InputLayout* m_inputLayout = nullptr;
	};
}

#endif // !MDX_SHADER_H

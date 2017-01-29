/**
* @file MDX_ConstantBuffer.h
* @brief �R���X�^���g�o�b�t�@�N���X�̒�`
* @author Toru Miyawaki
*/

#ifndef MDX_CONSTANT_BUFFER_H
#define MDX_CONSTANT_BUFFER_H

#include <d3d11.h>

/**
* @brief namespace MDX
*/
namespace MDX {
	/**
	* @brief �R���X�^���g�o�b�t�@�N���X
	*/
	class ConstantBuffer
	{
	public:
		ConstantBuffer();
		~ConstantBuffer();

		/**
		* @brief �쐬
		* @param [in] byteWidth �o�C�g�T�C�Y
		* @return �쐬�������ǂ���
		*/
		bool Create(UINT byteWidth);

		/**
		* @brief ���
		*/
		void Release();

		/**
		* @brief �}�b�v
		* @return �}�b�v�������\�[�X
		*/
		D3D11_MAPPED_SUBRESOURCE Map();

		/**
		* @brief �A���}�b�v
		*/
		void Unmap();

	public:
		/**
		* @brief ���_�V�F�[�_�[�ɐݒ�
		*/
		void SetVS(UINT slot);

		/**
		* @brief �s�N�Z���V�F�[�_�[�ɐݒ�
		*/
		void SetPS(UINT slot);

	private:
		ID3D11Buffer* m_buffer;
	};

}

#endif // !MDX_CONSTANT_BUFFER_H

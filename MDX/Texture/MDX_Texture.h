/**
* @file MDX_Texture.h
* @brief �e�N�X�`���N���X
* @author Toru Miyawaki
*/

#ifndef MDX_TEXTURE_H
#define	MDX_TEXTURE_H

#include <d3d11.h>
#include <string>

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief �e�N�X�`���N���X
	*/
	class Texture{
	public:
		/**
		* @brief �ǂݍ��ݏ��
		*/
		class LoadInfo{
		public:
			std::string filename;
			size_t maxSize;
			D3D11_USAGE usage;
			unsigned int bindFlags;
			unsigned int cpuAccessFlags;
			unsigned int miscFlags; 
			bool forceSRGB;

			LoadInfo(){
				Init();
			}

			/**
			* @brief ������
			*/
			void Init(){
				filename = "";
				maxSize = 0;
				usage = D3D11_USAGE_DEFAULT;
				bindFlags = D3D11_BIND_SHADER_RESOURCE;
				cpuAccessFlags = 0;
				miscFlags = 0;
				forceSRGB = false;
			}
		};

	public:
		Texture();
		~Texture();

		/**
		* @brief �e�N�X�`���ǂݍ���
		* @param [in] info �ǂݍ��ݏ��
		* @return �ǂݍ��ݐ������H
		*/
		bool Load(const LoadInfo& info);

		/**
		* @brief ���
		*/
		void Release();

		/**
		* @brief �s�N�Z���V�F�[�_�[�ɃV�F�[�_�[���\�[�X�ݒ�
		* @param [in] slot �X���b�g�ԍ�
		*/
		void PSSetShaderResource(UINT slot);

		/**
		* @brief �Q�b�^�[
		*/
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_srv; }
		UINT GetWidth() const { return m_width; }
		UINT GetHeight() const { return m_height; }


	private:
		/**
		* @brief WIC�e�N�X�`���ǂݍ���
		*/
		bool LoadWICTexture(const LoadInfo& info);

		/**
		* @brief DDS�e�N�X�`���ǂݍ���
		*/
		bool LoadDDSTexture(const LoadInfo& info);

		/**
		* @brief �e�N�X�`���̃T�C�Y�擾
		*/
		void GetTextureSize(UINT* width, UINT* height);

	private:
		ID3D11Resource* m_resource;
		ID3D11ShaderResourceView* m_srv;

		UINT m_width;
		UINT m_height;
	};
}

#endif // !MDX_TEXTURE_H

/**
* @file MDX_RenderManager.h
* @brief �����_�[�}�l�[�W���[�̒�`
* @author Toru Miyawaki
*/

#ifndef MDX_RENDER_MANAGER_H
#define MDX_RENDER_MANAGER_H

#include <d3d11.h>

#include <vector>
#include <memory>

#include "../Shader/MDX_Shader.h"
#include "../Math/MDX_Math.h"
#include "../Texture/MDX_Texture.h"
#include "MDX_RenderTarget.h"

/**
* @brief namespace MDX
*/
namespace MDX{
	class IActor;

	/**
	* @brief �����_�[�}�l�[�W���[
	*/
	class RenderManager{
	public:
		/**
		* @brief �ϊ��s��R���X�^���g�o�b�t�@
		*/
		struct CBMatrix{
			Matrix4X4 matWorld;
			Matrix4X4 matView;
			Matrix4X4 matProjection;
		};
		/**
		* @brief �V�F�[�_�[���֘A�ϐ�
		*/
		struct CBEnvironment{
			Vector3f viewPos;
			float tmp_viewPos;
			Vector3f vecLight;
			float tmp_vecLight;
		};

		/**
		* @brief �����x�[�X�����_�����O�p�R���X�^���g�o�b�t�@
		*/
		struct CBPbr{
			Vector4f	baseColor; // �֋X��̃p�����[�^
			float		roughness;
			float		metallic;
			Vector2f	tmp;
		};
	public:
		RenderManager();
		~RenderManager();

		/**
		* @brief ������
		* @return �������������H
		*/
		bool Init();

		/**
		* @brief ���
		*/
		void Release();

		/**
		* @brief �X�V
		*/
		void Update();

		/**
		* @brief �`��
		*/
		void Draw();

		/**
		* @brief �A�N�^�[�̒ǉ�
		* @param [in] �A�N�^�[�N���X
		*/
		void AddActor(IActor* actor);

		/**
		* @brief �ϊ��s��R���X�^���g�o�b�t�@�X�V
		* @param [in] world ���[���h�ϊ��s��
		* IActor�N���X���p���������̂͂��ꂼ��̏ꏊ�ŌĂяo��
		*/
		void UpdateCBMatrix(const Matrix4X4& world);

		/**
		* @brief ���p�R���X�^���g�o�b�t�@�X�V
		* @param [in] environment �R���X�^���g�o�b�t�@�X�V�Ɏg�����
		* �ǂ����ŌĂяo��
		*/
		void UpdateCBEnvironmment(CBEnvironment* environment);

		/**
		* @brief PBR�p�R���X�^���g�o�b�t�@�X�V
		* @param [in] pbr PBR�Ɏg�����
		* IActor�N���X���p���������̂͂��ꂼ��̏ꏊ�ŌĂяo��
		*/
		void UpdateCBPbr(CBPbr* pbr);

		/**
		* @brief �Q�b�^�[
		*/
		Matrix4X4 GetMatWorld() const { return m_matrix.matWorld; }
		Matrix4X4 GetMatView() const { return m_matrix.matView; }
		Matrix4X4 GetMatProjection() const { return m_matrix.matProjection; }
		std::shared_ptr<Shader> GetDefaultShader() const { return m_shader; }

		/**
		* @brief �Z�b�^�[
		*/
		void SetMatWorld(const Matrix4X4& world){ m_matrix.matWorld = world; }
		void SetMatView(const Matrix4X4& view){ m_matrix.matView = view; }
		void SetMatProjection(const Matrix4X4& prj){ m_matrix.matProjection = prj; }

	private:
		/**
		* @brief �����_�[�^�[�Q�b�g�̎��
		*/
		enum RenderTargetType{
			RT_HDR,
			RT_DOWN_SCALED_HDR,
			RT_BRIGHT_PASS_FILTER,
			RT_BLOOM,
			RT_BLOOM_FINAL,
			RT_LUMINANCE,
			RT_TYPE_MAX
		};

		static const int NUM_TONEMAP_TEXTURE = 4;
		static const int NUM_BLOOM_TEXTURE = 4;


	private:
		/**
		* @brief �A�N�^�[�̕`��
		*/
		void DrawActor(IActor* actor);

	private:
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<Shader> m_shaderPostEffect;

		std::shared_ptr<Texture>	m_defaultIrradianceTex;
		std::shared_ptr<Texture>	m_defaultRadianceTex;

		ID3D11Buffer* m_cbMatrix = nullptr;
		ID3D11Buffer* m_cbEnvironment = nullptr;
		ID3D11Buffer* m_cbPbr = nullptr;

		std::vector<IActor*> m_actor;

		CBMatrix m_matrix;

		RenderTarget m_renderTarget[RT_TYPE_MAX];
		RenderTarget m_toneMapTexture[NUM_TONEMAP_TEXTURE];
		RenderTarget m_bloomTexture[NUM_BLOOM_TEXTURE];
	};


}

#endif // !MDX_RENDER_MANAGER_H

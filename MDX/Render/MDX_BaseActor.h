/**
* @file MDX_BaseActor.h
* @brief �A�N�^�[���N���X
* @author Toru Miyawaki
*/

#ifndef MDX_BASE_ACTOR_H
#define MDX_BASE_ACTOR_H

#include <d3d11.h>

#include <memory>
#include <functional>

#include "MDX_IActor.h"
#include "../Math/MDX_Math.h"
#include "../Shader/MDX_Shader.h"
#include "MDX_RenderManager.h"

typedef void(*OnBeginDrawPtr)(void*);

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief �A�N�^�[���N���X
	*/
	class BaseActor : public IActor
	{
	public:
		BaseActor();
		virtual ~BaseActor();

		/**
		* @brief �풓�f�[�^������
		* �A�v���P�[�V�������������Ɉ�񂾂��Ăяo��
		*/
		static bool InitResidentData();

		/**
		* @brief �풓�f�[�^���
		* �A�v���P�[�V����������Ɉ�񂾂��Ăяo��
		*/
		static void ReleaseResidentData();

		/**
		* @brief ���
		* �p����̓����\�b�h���ŌĂяo������
		*/
		virtual void Release();

		/**
		* @brief �X�V
		* �p����̓����\�b�h���ŌĂяo������
		*/
		virtual void Update();

		/**
		* @brief �`��
		* �p����̓����\�b�h���ŌĂяo������
		*/
		virtual void Draw();

		/**
		* @brief �ϊ��s��R���X�^���g�o�b�t�@�X�V
		*/
		void BaseActor::UpdateCBMatrix();
		
		/**
		* @brief PBR�p�R���X�^���g�o�b�t�@�X�V
		*/
		void BaseActor::UpdateCBPbr();

		/**
		* @biref �Q�b�^�[
		*/
		Vector3f GetPos() const { return m_pos; }
		Vector3f GetAngle() const { return m_angle; }
		Vector3f GetScale() const { return m_scale; }
		RenderManager::CBPbr* GetPbrParam() { return &m_pbr; }
		std::shared_ptr<Shader> GetShader() { return m_shader; }

		/**
		* @brief �Z�b�^�[
		*/
		void SetPos(const Vector3f& pos){ m_pos = pos; }
		void SetAngle(const Vector3f& angle){ m_angle = angle; }
		void SetScale(const Vector3f& scale){ m_scale = scale; }
		void SetPbrParam(const RenderManager::CBPbr& pbr) { m_pbr = pbr; }
		void SetShader(std::shared_ptr<Shader> shader){ m_shader = shader; }

		void SetOnBeginDraw(OnBeginDrawPtr callback, void* arg){ m_OnBeginDraw = callback; m_OnBeginDrawArg = arg; }

	//-------------------------------------------------------------------------
	// PBR�p�����[�^�[
	//-------------------------------------------------------------------------
	public:
		/**
		* @biref �Q�b�^�[
		*/
		float GetRughness() const { return m_pbr.roughness; }
		float GetMetallic() const { return m_pbr.metallic; }

		/**
		* @biref �Z�b�^�[
		*/
		void SetRoughness(float roughness);
		void SetMetallic(float metallic);

	//-------------------------------------------------------------------------
	// �R�[���o�b�N
	//-------------------------------------------------------------------------
	protected:
		OnBeginDrawPtr	m_OnBeginDraw;
		void*			m_OnBeginDrawArg;

	protected:
		Vector3f m_pos = Vector3f(0, 0, 0);
		Vector3f m_angle = Vector3f(0, 0, 0);
		Vector3f m_scale = Vector3f(1, 1, 1);


		// TODO: �`��n�ϐ��������ɂ���̂͂�������
		static ID3D11SamplerState* m_sampler;
		RenderManager::CBPbr m_pbr;
		std::shared_ptr<Shader>	m_shader;
	};

}

#endif // !MDX_BASE_ACTOR_H

/**
* @file MDX_IActor.h
* @brief �A�N�^�[�̒�`
* @author Toru Miyawaki
*/

#ifndef MDX_IACTOR_H
#define MDX_IACTOR_H

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief �A�N�^�[�N���X
	*/
	class IActor{
	public:
		virtual ~IActor(){}

		/**
		* @brief ���
		*/
		virtual void Release() = 0;

		/**
		* @brief �X�V
		*/
		virtual void Update() = 0;

		/**
		* @brief �`��
		*/
		virtual void Draw() = 0;

		/**
		* @brief �ϊ��s��R���X�^���g�o�b�t�@�X�V
		*/
		virtual void UpdateCBMatrix() = 0;
		
		/**
		* @brief PBR�p�R���X�^���g�o�b�t�@�X�V
		*/
		virtual void UpdateCBPbr() = 0;

	};

}

#endif // !MDX_IACTOR_H

/**
* @file MDX_Mesh.h
* @brief ���b�V���N���X�̒�`
* @author Toru Miyawaki
*/

#ifndef MDX_MESH_H
#define MDX_MESH_H

#include <d3d11.h>
#include "../Texture/MDX_Texture.h"
#include "../Math/MDX_Math.h"
#include "../Utillity/MDX_Utillity.h"
#include "MDX_BaseMesh.h"

/**
* @brief namespace MDX
*/
namespace MDX{
	/**
	* @brief mmf���b�V���N���X
	*/
	class MmfMesh : public BaseMesh
	{
	public:
		MmfMesh();
		~MmfMesh();

		/**
		* @brief �t�@�C���ǂݍ���
		* @param [in] filename �t�@�C����
		* @return �ǂݍ��ݐ������H
		*/
		bool Load(const char* filename) override;

		/**
		* @brief ���
		*/
		void Release() override;

		/**
		* @brief �X�V
		*/
		void Update() override;

		/**
		* @brief �`��
		*/
		void Draw() override;

	private:
		static const int MAX_TEXTURE = 32;
		static const int MAX_TEXTURE_NAME = 128;

		/**
		* @brief �|���S�����
		*/
		struct PolygonInfo{
			int			indices[3];
			float		uv[3][2];
			int			useMaterialIndex;
			Vector3f	vertexNormals[3];
			Vector3f	faceNormal;
			DWORD		color;
		};
		/**
		* @brief mmf���
		*/
		class MmfInfo{
		public:
			int numVertices = 0;
			int numFaces = 0;
			int numMaterials = 0;

			Vector3f* vertices = nullptr;
			PolygonInfo* faces = nullptr;
			int* numFacesEveryMaterial = nullptr; //! �ގ����Ƃ̖ʐ�

			char texName[MAX_TEXTURE][MAX_TEXTURE_NAME];

		public:
			~MmfInfo(){
				Release();
			}

			/**
			* @brief ���
			*/
			void Release(){
				SAFE_DELETE_ARRAY(vertices);
				SAFE_DELETE_ARRAY(faces);
				SAFE_DELETE_ARRAY(numFacesEveryMaterial);
			}
		};

	private:
		MmfInfo m_mmfInfo;

		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;

		Texture** m_texture;


	};
}

#endif // !MDX_MESH_H

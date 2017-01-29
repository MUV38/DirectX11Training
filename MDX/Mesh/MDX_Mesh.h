/**
* @file MDX_Mesh.h
* @brief メッシュクラスの定義
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
	* @brief mmfメッシュクラス
	*/
	class MmfMesh : public BaseMesh
	{
	public:
		MmfMesh();
		~MmfMesh();

		/**
		* @brief ファイル読み込み
		* @param [in] filename ファイル名
		* @return 読み込み成功か？
		*/
		bool Load(const char* filename) override;

		/**
		* @brief 解放
		*/
		void Release() override;

		/**
		* @brief 更新
		*/
		void Update() override;

		/**
		* @brief 描画
		*/
		void Draw() override;

	private:
		static const int MAX_TEXTURE = 32;
		static const int MAX_TEXTURE_NAME = 128;

		/**
		* @brief ポリゴン情報
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
		* @brief mmf情報
		*/
		class MmfInfo{
		public:
			int numVertices = 0;
			int numFaces = 0;
			int numMaterials = 0;

			Vector3f* vertices = nullptr;
			PolygonInfo* faces = nullptr;
			int* numFacesEveryMaterial = nullptr; //! 材質ごとの面数

			char texName[MAX_TEXTURE][MAX_TEXTURE_NAME];

		public:
			~MmfInfo(){
				Release();
			}

			/**
			* @brief 解放
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

#ifndef __MDX_VECTOR3F_H__
#define __MDX_VECTOR3F_H__

#include <DirectXMath.h>

namespace MDX
{
	// add
	DirectX::XMFLOAT3 Vector3fAdd(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
	// sub
	DirectX::XMFLOAT3 Vector3fSub(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
	// mul
	DirectX::XMFLOAT3 Vector3fMul(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
	DirectX::XMFLOAT3 Vector3fMul(const DirectX::XMFLOAT3& v1, float f);
	// div
	DirectX::XMFLOAT3 Vector3fDiv(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
	DirectX::XMFLOAT3 Vector3fDiv(const DirectX::XMFLOAT3& v1, float f);
}

#endif // !__MDX_VECTOR3F_H__

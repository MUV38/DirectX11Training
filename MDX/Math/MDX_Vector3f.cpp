#include "MDX_Vector3f.h"

namespace MDX
{
	// add
	DirectX::XMFLOAT3 Vector3fAdd(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
	{
		DirectX::XMFLOAT3 out;
		out.x = v1.x + v2.x;
		out.y = v1.y + v2.y;
		out.z = v1.z + v2.z;
		return out;
	}
	// sub
	DirectX::XMFLOAT3 Vector3fSub(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
	{
		DirectX::XMFLOAT3 out;
		out.x = v1.x - v2.x;
		out.y = v1.y - v2.y;
		out.z = v1.z - v2.z;
		return out;
	}
	// mul
	DirectX::XMFLOAT3 Vector3fMul(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
	{
		DirectX::XMFLOAT3 out;
		out.x = v1.x * v2.x;
		out.y = v1.y * v2.y;
		out.z = v1.z * v2.z;
		return out;
	}
	DirectX::XMFLOAT3 Vector3fMul(const DirectX::XMFLOAT3& v1, float f)
	{
		DirectX::XMFLOAT3 out;
		out.x = v1.x * f;
		out.y = v1.y * f;
		out.z = v1.z * f;
		return out;
	}
	// div
	DirectX::XMFLOAT3 Vector3fDiv(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
	{
		DirectX::XMFLOAT3 out;
		out.x = v1.x / v2.x;
		out.y = v1.y / v2.y;
		out.z = v1.z / v2.z;
		return out;
	}
	DirectX::XMFLOAT3 Vector3fDiv(const DirectX::XMFLOAT3& v1, float f)
	{
		DirectX::XMFLOAT3 out;
		out.x = v1.x / f;
		out.y = v1.y / f;
		out.z = v1.z / f;
		return out;
	}

}

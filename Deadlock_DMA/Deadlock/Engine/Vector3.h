#pragma once

class Vector3
{
public:
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };
public:
	inline float Distance(const Vector3& Other) const
	{
		return std::sqrt((Other.x - x) * (Other.x - x) + (Other.y - y) * (Other.y - y) + (Other.z - z) * (Other.z - z));
	}
	Vector3 operator+(const Vector3& Other) const
	{
		return Vector3{ x + Other.x, y + Other.y, z + Other.z };
	}
	Vector3 operator*(float Scalar) const
	{
		return Vector3{ x * Scalar, y * Scalar, z * Scalar };
	}
	void operator+=(const Vector3& Other)
	{
		x += Other.x;
		y += Other.y;
		z += Other.z;
	}
};
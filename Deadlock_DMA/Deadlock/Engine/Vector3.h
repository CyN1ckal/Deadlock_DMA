#pragma once

class Vector3
{
public:
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };
	Vector3 operator-(const Vector3& r) const { return { x - r.x, y - r.y, z - r.z }; }
	float Length() const { return std::sqrt(x * x + y * y + z * z); }
};
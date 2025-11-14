#pragma once

struct Vector2
{
	float x{ 0.0f };
	float y{ 0.0f };

public:
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2{ x - other.x, y - other.y };
	}
};
#pragma once
#include <cmath>
#include "includes.h"
#include "imgui/imgui.h"

struct Vec3
{
	float x{ 0.f }; float y{ 0.f }; float z{ 0.f };

	constexpr Vec3 operator+(Vec3 other) noexcept
	{
		return { this->x + other.x, this->y + other.y, this->z + other.z };
	}
	constexpr Vec3 operator-(Vec3 other) noexcept
	{
		return { this->x - other.x, this->y - other.y, this->z - other.z };
	}
	constexpr Vec3 operator*(Vec3 other) noexcept
	{
		return { this->x * other.x, this->y * other.y, this->z * other.z };
	}
	constexpr Vec3 operator/(Vec3 other) noexcept
	{
		return { this->x / other.x, this->y / other.y, this->z / other.z };
	}
	constexpr bool operator==(Vec3 other) noexcept
	{
		if (this->x == other.x && this->y == other.y && this->z == other.z) return true;
		return false;
	}
	constexpr bool operator>(Vec3 other) noexcept
	{
		if (this->x > other.x && this->y > other.y && this->z > other.z) return true;
		return false;
	}
	constexpr bool operator<(Vec3 other) noexcept
	{
		if (this->x < other.x && this->y < other.y && this->z < other.z) return true;
		return false;
	}
	constexpr Vec3 operator++() noexcept
	{
		return { this->x++, this->y++, this->z++ };
	}
	constexpr Vec3 operator--() noexcept
	{
		return { this->x--, this->y--, this->z-- };
	}

	float DistanceTo(Vec3 other)
	{
		return std::sqrt(((this->x - other.x) * (this->x - other.x)) + ((this->y - other.y) * (this->y - other.y))
			+ ((this->z - other.z) * (this->z - other.z)));
	}
};

struct Vec2
{
	float x{ 0.f }; float y{ 0.f };

	constexpr Vec2 operator+(Vec2 other) noexcept
	{
		return { this->x + other.x, this->y + other.y };
	}
	constexpr Vec2 operator-(Vec2 other) noexcept
	{
		return { this->x - other.x, this->y - other.y };
	}
	constexpr Vec2 operator*(Vec2 other) noexcept
	{
		return { this->x * other.x, this->y * other.y };
	}
	constexpr Vec2 operator/(Vec2 other) noexcept
	{
		return { this->x / other.x, this->y / other.y };
	}
	constexpr bool operator==(Vec2 other) noexcept
	{
		if (this->x == other.x && this->y == other.y) return true;
		return false;
	}
	constexpr bool operator>(Vec2 other) noexcept
	{
		if (this->x > other.x && this->y > other.y) return true;
		return false;
	}
	constexpr bool operator<(Vec2 other) noexcept
	{
		if (this->x < other.x && this->y < other.y) return true;
		return false;
	}
	constexpr Vec2 operator++() noexcept
	{
		return { this->x++, this->y++ };
	}
	constexpr Vec2 operator--() noexcept
	{
		return { this->x--, this->y-- };
	}

	float DistanceTo(Vec2 other)
	{
		return std::sqrt(((this->x - other.x) * (this->x - other.x)) + ((this->y - other.y) * (this->y - other.y)));
	}

	ImVec2 ToImVec2()
	{
		return ImVec2(this->x, this->y);
	}
};

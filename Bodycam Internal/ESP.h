#pragma once
#include "includes.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Vector.h"
#include <array>
#include "globals.h"


constexpr Vec2 ScreenCenter{ 1920 / 2, 1080 / 2 };
constexpr float PI{ 3.14f };

namespace ESP
{
	inline void DrawLineFVector(SDK::FVector2D posA, SDK::FVector2D posB, ImColor color, float t = 1.f, bool outline = false)
	{

		if (outline)
			draw_list->AddLine(ImVec2(posA.X, posA.Y), ImVec2(posB.X, posB.Y), ImColor(0.f, 0.f, 0.f), t + 1.f);
		draw_list->AddLine(ImVec2(posA.X, posA.Y), ImVec2(posB.X, posB.Y), color, t);
	}
	inline void DrawBox(Vec2 A, Vec2 B, ImColor color, bool outline = true, float rounding = 0.f, float t = 1.f)
	{
		if (!outline)
			draw_list->AddRect(A.ToImVec2(), B.ToImVec2(), color, rounding, t);
		else
		{
			draw_list->AddRect(A.ToImVec2(), B.ToImVec2(), color, rounding, t);
			draw_list->AddRect({ A.x + 1, A.y + 1 }, { B.x - 1, B.y - 1 }, Colors::Black, rounding, t);
			draw_list->AddRect({ A.x - 1, A.y - 1 }, { B.x + 1, B.y + 1 }, Colors::Black, rounding, t);
		}
	}
	inline void DrawLine(Vec2 A, Vec2 B, ImColor color, bool outline = true, float t = 1.f)
	{
		if (!outline)
			draw_list->AddLine(A.ToImVec2(), B.ToImVec2(), color, t);
		else
		{
			draw_list->AddLine(A.ToImVec2(), B.ToImVec2(), Colors::Black, t + 1);
			draw_list->AddLine(A.ToImVec2(), B.ToImVec2(), color, t);
		}
	}
	inline void DrawCircle(Vec2 pos, float r, ImColor color, int segments = 12, float t = 1.f)
	{
		draw_list->AddCircle(pos.ToImVec2(), r, color, segments, t);
	}
	inline void DrawCircleFilled(Vec2 pos, float r, ImColor color, int segments = 12)
	{
		draw_list->AddCircleFilled(pos.ToImVec2(), r, color, segments);
	}
	inline void DrawBoxFilled(Vec2 A, Vec2 B, ImColor color, float rounding = 0.f, float t = 1.f)
	{
		draw_list->AddRectFilled(A.ToImVec2(), B.ToImVec2(), color, rounding);
	}
	inline void DrawCorneredBox(Vec2 A, Vec2 B, float w, float h, ImColor color)
	{
		const float wi = w / 3;
		const float hi = h / 3;

		DrawLine({ A.x - w, A.y }, { A.x - w + wi, A.y }, color);
		DrawLine({ A.x + w, A.y }, { A.x + w - wi, A.y }, color);

		DrawLine({ A.x - w, A.y }, { A.x - w, A.y + hi }, color);
		DrawLine({ A.x + w, A.y }, { A.x + w, A.y + hi }, color);

		DrawLine({ B.x - w, B.y }, { B.x - w + wi, B.y }, color);
		DrawLine({ B.x + w, B.y }, { B.x + w - wi, B.y }, color);

		DrawLine({ B.x - w, B.y }, { B.x - w, B.y - hi }, color);
		DrawLine({ B.x + w, B.y }, { B.x + w, B.y - hi }, color);
	}
	inline void DrawCrosshair(float size, float t, ImColor color)
	{
		DrawLine({ ScreenCenter.x - size, ScreenCenter.y }, { ScreenCenter.x + size, ScreenCenter.y }, color, true, t);
		DrawLine({ ScreenCenter.x, ScreenCenter.y - size }, { ScreenCenter.x, ScreenCenter.y + size }, color, true, t);
	}
	inline void DrawText2(Vec2 pos, ImColor color, const char* text)
	{
		draw_list->AddText(ImVec2(pos.x, pos.y), color, text);
	}
	inline void Draw3DBox(SDK::FVector& center, SDK::FVector& Bounds, SDK::APlayerController* Controller, ImColor col, bool outline = true)
	{
		const float wX = (center.X + Bounds.X) - (center.X - Bounds.X);
		const float wY = (center.Y + Bounds.Y) - (center.Y - Bounds.Y);

		SDK::FVector Top{ center.X, center.Y, center.Z + Bounds.Z };
		SDK::FVector Bottom{ center.X, center.Y, center.Z - Bounds.Z };

		SDK::FVector a1 = { center.X - wX / 2, center.Y + wY / 2, Bottom.Z };
		SDK::FVector a2 = { center.X + wX / 2, center.Y + wY / 2, Bottom.Z };
		SDK::FVector a3 = { center.X - wX / 2, center.Y - wY / 2, Bottom.Z };
		SDK::FVector a4 = { center.X + wX / 2, center.Y - wY / 2, Bottom.Z };

		SDK::FVector b1 = { center.X - wX / 2, center.Y + wY / 2, Top.Z };
		SDK::FVector b2 = { center.X + wX / 2, center.Y + wY / 2, Top.Z };
		SDK::FVector b3 = { center.X - wX / 2, center.Y - wY / 2, Top.Z };
		SDK::FVector b4 = { center.X + wX / 2, center.Y - wY / 2, Top.Z };

		SDK::FVector2D a1w2s{};
		SDK::FVector2D a2w2s{};
		SDK::FVector2D a3w2s{};
		SDK::FVector2D a4w2s{};

		SDK::FVector2D b1w2s{};
		SDK::FVector2D b2w2s{};
		SDK::FVector2D b3w2s{};
		SDK::FVector2D b4w2s{};

		if (Controller->ProjectWorldLocationToScreen(a1, &a1w2s, false) && Controller->ProjectWorldLocationToScreen(a2, &a2w2s, false)
			&& Controller->ProjectWorldLocationToScreen(a3, &a3w2s, false) && Controller->ProjectWorldLocationToScreen(a4, &a4w2s, false)
			&& Controller->ProjectWorldLocationToScreen(b1, &b1w2s, false) && Controller->ProjectWorldLocationToScreen(b2, &b2w2s, false)
			&& Controller->ProjectWorldLocationToScreen(b3, &b3w2s, false) && Controller->ProjectWorldLocationToScreen(b4, &b4w2s, false))
		{
			ESP::DrawLineFVector(a1w2s, a2w2s, col, 1.f, outline);
			ESP::DrawLineFVector(a2w2s, a4w2s, col, 1.f, outline);
			ESP::DrawLineFVector(a4w2s, a3w2s, col, 1.f, outline);
			ESP::DrawLineFVector(a3w2s, a1w2s, col, 1.f, outline);

			ESP::DrawLineFVector(b1w2s, b2w2s, col, 1.f, outline);
			ESP::DrawLineFVector(b2w2s, b4w2s, col, 1.f, outline);
			ESP::DrawLineFVector(b4w2s, b3w2s, col, 1.f, outline);
			ESP::DrawLineFVector(b3w2s, b1w2s, col, 1.f, outline);

			ESP::DrawLineFVector(a1w2s, b1w2s, col, 1.f, outline);
			ESP::DrawLineFVector(a2w2s, b2w2s, col, 1.f, outline);
			ESP::DrawLineFVector(a3w2s, b3w2s, col, 1.f, outline);
			ESP::DrawLineFVector(a4w2s, b4w2s, col, 1.f, outline);
		}
	}
}
#pragma once
#include "../Global.h"
#include "..//ImGui/imgui.h"
#include "..//ImGui/imgui_internal.h"
#include "..//ImGui/imgui_impl_dx11.h"


namespace Drawing {
	enum DrawingEnum
	{
		Text,
		Line,
		Square,
		DRectangle,
		Triangle,
		Circle
	};

	struct D3DVector
	{
		float x = 0;
		float y = 0;
	};

	struct D3DColor
	{
		float r = 255;
		float g = 255;
		float b = 255;
	};

	struct D3DText
	{
		std::string* Text;
		float Size = 10.0f;
		float Transparency = 1.0f;
		bool Center = false;
		bool Outline = false;
		D3DVector Position;
		D3DColor Color;
	};

	struct D3DCircle
	{
		D3DVector Position;
		float Radius = 0.0f;
		D3DColor Color;
		float Thickness = 1.0f;
		bool Filled = false;
		float Transparency = 1.0f;
		int NumSides = 50;
	};

	struct D3DLine
	{
		D3DVector From;
		D3DVector To;
		D3DColor Color;
		float Thickness = 1.0f;
		float Transparency = 1.0f;
	};

	struct D3DSquare
	{
		D3DVector Position;
		D3DVector Size;
		D3DColor Color;
		float Thickness = 1.0f;
		bool Filled = false;
		float Transparency = 1.0f;
	};

	struct D3DDrawing
	{
		DrawingEnum type;
		bool Visible = true;
		uintptr_t* Object;
	};

	extern std::vector<D3DDrawing*> DrawingsVector;

	D3DVector getvec(lua_State* rL, int idx);

	D3DColor getcolor(lua_State* rL, int idx);

	void makevec(lua_State* rL, D3DVector sex);

	void makecolor(lua_State* rL, D3DColor sex);

	int TextRemove(lua_State* rL);

	int TextIndex(lua_State* rL);

	int TextNewIndex(lua_State* rL);

	int CircleRemove(lua_State* rL);

	int CircleIndex(lua_State* rL);

	int CircleNewIndex(lua_State* rL);

	int LineRemove(lua_State* rL);

	int LineIndex(lua_State* rL);

	int LineNewIndex(lua_State* rL);

	int SquareRemove(lua_State* rL);

	int SquareIndex(lua_State* rL);

	int SquareNewIndex(lua_State* rL);

	void lua_setmetatable(lua_State* ls, const char* tname);

	int lua_newmetatable(lua_State* rL, const char* name);

	int New(lua_State* rL);

	int Clear(lua_State* rL);

	int SetUpDrawing(lua_State* rL);
}
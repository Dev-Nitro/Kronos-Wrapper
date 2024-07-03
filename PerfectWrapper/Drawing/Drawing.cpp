#include "Drawing.h"
#include "../XHosted.h"
#include "../Environment.h"


namespace Drawing {
	std::vector<D3DDrawing*> DrawingsVector;


	D3DVector getvec(lua_State* rL, int idx)
	{
		lua_pushvalue(rL, idx);
		lua_getfield(rL, -1, "X");
		const float x = (float)lua_tonumber(rL, -1);
		lua_getfield(rL, -2, "Y");
		const float y = (float)lua_tonumber(rL, -1);
		return { x , y };
	}

	D3DColor getcolor(lua_State* rL, int idx)
	{
		lua_pushvalue(rL, idx);
		lua_getfield(rL, -1, "r");
		const float r = (float)lua_tonumber(rL, -1);
		lua_getfield(rL, -2, "g");
		const float g = (float)lua_tonumber(rL, -1);
		lua_getfield(rL, -3, "b");
		const float b = (float)lua_tonumber(rL, -1);
		return { r, g, b };
	}

	void makevec(lua_State* rL, D3DVector sex)
	{
		lua_getfield(rL, -10002, "Vector2");
		lua_getfield(rL, -1, "new");
		lua_pushnumber(rL, sex.x);
		lua_pushnumber(rL, sex.y);
		lua_pcall(rL, 2, 1, 0);
	}

	void makecolor(lua_State* rL, D3DColor sex)
	{
		lua_getfield(rL, -10002, "Color3");
		lua_getfield(rL, -1, "new");
		lua_pushnumber(rL, sex.r);
		lua_pushnumber(rL, sex.g);
		lua_pushnumber(rL, sex.b);
		lua_pcall(rL, 3, 1, 0);
	}

	int TextRemove(lua_State* rL)
	{
		auto Text = (D3DText*)lua_touserdata(rL, -1);
		for (int i = 0; i < DrawingsVector.size(); i++)
		{
			if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Text))
			{
				DrawingsVector.erase(DrawingsVector.begin() + i);
			}
		}
		auto Space = ExtraSpace::GetRoot(rL);
		r_lua_pushlightuserdata(Space->RL, Text);
		r_lua_rawget(Space->RL, LUA_REGISTRYINDEX);
		uintptr_t RText = r_lua_touserdata(Space->RL, -1);
		r_lua_pop(Space->RL, 1);

		lua_pushlightuserdata(rL, reinterpret_cast<void*>(RText));
		lua_pushnumber(rL, RText);
		lua_rawset(rL, LUA_REGISTRYINDEX);

		r_lua_pushlightuserdata(Space->RL, Text);
		r_lua_pushnumber(Space->RL, reinterpret_cast<uintptr_t>(Text));
		r_lua_rawset(Space->RL, LUA_REGISTRYINDEX, NULL);
		return 0;
	}

	int TextIndex(lua_State* rL)
	{
		const char* Key = lua_tolstring(rL, -1, 0);
		const auto TextUD = (D3DText*)lua_touserdata(rL, -2);
		if (!strcmp("Text", Key))
		{
			lua_pushstring(rL, TextUD->Text->c_str());
		}
		else if (!strcmp("Remove", Key))
		{
			lua_pushcclosure(rL, TextRemove, 0);
		}
		else if (!strcmp("Size", Key))
		{
			lua_pushnumber(rL, TextUD->Size);
		}
		else if (!strcmp("Position", Key))
		{
			makevec(rL, TextUD->Position);
		}
		else if (!strcmp("Color", Key))
		{
			makecolor(rL, TextUD->Color);
		}
		else if (!strcmp("Center", Key))
		{
			lua_pushboolean(rL, TextUD->Center);
		}
		else if (!strcmp("Outline", Key))
		{
			lua_pushboolean(rL, TextUD->Outline);
		}
		else if (!strcmp("Visible", Key))
		{
			for (int i = 0; i < DrawingsVector.size(); i++)
			{
				if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(TextUD))
				{
					lua_pushboolean(rL, DrawingsVector[i]->Visible);
				}
			}
		}
		else if (!strcmp("TextBounds", Key))
		{

			const auto TextSize = ImGui::GetCurrentContext()->Font->CalcTextSizeA(TextUD->Size, FLT_MAX, 0.0f, TextUD->Text->c_str());
			lua_getfield(rL, -10002, "Vector2");
			lua_getfield(rL, -1, "new");
			lua_pushnumber(rL, TextSize.x);
			lua_pushnumber(rL, TextSize.y);
			lua_pcall(rL, 2, 1, 0);
		}
		return 1;
	}

	int TextNewIndex(lua_State* rL)
	{
		const char* Key = lua_tolstring(rL, -2, 0);
		const auto TextUD = (D3DText*)lua_touserdata(rL, -3);
		if (!strcmp("Text", Key))
		{
			const auto text = lua_tolstring(rL, -1, 0);
			TextUD->Text = new std::string(text);
		}
		else if (!strcmp("Size", Key))
		{
			TextUD->Size = float(lua_tonumber(rL, -1));
		}
		else if (!strcmp("Position", Key))
		{
			TextUD->Position = getvec(rL, -1);
		}
		else if (!strcmp("Color", Key))
		{
			TextUD->Color = getcolor(rL, -1);
		}
		else if (!strcmp("Outline", Key))
		{
			TextUD->Outline = lua_toboolean(rL, -1);
		}
		else if (!strcmp("Center", Key))
		{
			TextUD->Center = lua_toboolean(rL, -1);
		}
		else if (!strcmp("Transparency", Key))
		{
			TextUD->Transparency = (float)lua_tonumber(rL, -1);
		}
		else if (!strcmp("Visible", Key))
		{
			for (int i = 0; i < DrawingsVector.size(); i++)
			{
				if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(TextUD))
				{
					DrawingsVector[i]->Visible = lua_toboolean(rL, -1);
				}
			}
		}
		return 0;
	}

	int CircleRemove(lua_State* rL)
	{
		auto Circle = (D3DCircle*)lua_touserdata(rL, -1);
		for (int i = 0; i < DrawingsVector.size(); i++)
		{
			if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Circle))
			{
				DrawingsVector.erase(DrawingsVector.begin() + i);
			}
		}
		auto Space = ExtraSpace::GetRoot(rL);
		r_lua_pushlightuserdata(Space->RL, Circle);
		r_lua_rawget(Space->RL, LUA_REGISTRYINDEX);
		uintptr_t  RCircle = r_lua_touserdata(Space->RL, -1);
		r_lua_pop(Space->RL, 1);

		lua_pushlightuserdata(rL, reinterpret_cast<void*>(RCircle));
		lua_pushnumber(rL, RCircle);
		lua_rawset(rL, LUA_REGISTRYINDEX);

		r_lua_pushlightuserdata(Space->RL, Circle);
		r_lua_pushnumber(Space->RL, reinterpret_cast<uintptr_t>(Circle));
		r_lua_rawset(Space->RL, LUA_REGISTRYINDEX, NULL);
		return 0;
	}

	int CircleIndex(lua_State* rL)
	{
		const char* Key = lua_tolstring(rL, -1, 0);
		const auto Circle = (D3DCircle*)lua_touserdata(rL, -2);
		if (!strcmp("Remove", Key))
		{
			lua_pushcclosure(rL, CircleRemove, 0);
		}
		else if (!strcmp("Position", Key))
		{
			makevec(rL, Circle->Position);
		}
		else if (!strcmp("Color", Key))
		{
			makecolor(rL, Circle->Color);
		}
		else if (!strcmp("Radius", Key))
		{
			lua_pushnumber(rL, Circle->Radius);
		}
		else if (!strcmp("Thickness", Key))
		{
			lua_pushnumber(rL, Circle->Thickness);
		}
		else if (!strcmp("Filled", Key))
		{
			lua_pushboolean(rL, Circle->Filled);
		}
		else if (!strcmp("Transparency", Key))
		{
			lua_pushnumber(rL, Circle->Transparency);
		}
		else if (!strcmp("NumSides", Key))
		{
			lua_pushnumber(rL, Circle->NumSides);
		}
		else if (!strcmp("Visible", Key))
		{
			for (int i = 0; i < DrawingsVector.size(); i++)
			{
				if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Circle))
				{
					lua_pushboolean(rL, DrawingsVector[i]->Visible);
				}
			}
		}
		return 1;
	}

	int CircleNewIndex(lua_State* rL)
	{
		const char* Key = lua_tolstring(rL, -2, 0);
		const auto Circle = (D3DCircle*)lua_touserdata(rL, -3);
		if (!strcmp("Position", Key))
		{
			Circle->Position = getvec(rL, -1);
		}
		else if (!strcmp("Radius", Key))
		{
			Circle->Radius = (float)lua_tonumber(rL, -1);
		}
		else if (!strcmp("Color", Key))
		{
			Circle->Color = getcolor(rL, -1);
		}
		else if (!strcmp("Thickness", Key))
		{
			Circle->Thickness = (float)lua_tonumber(rL, -1);
		}
		else if (!strcmp("Filled", Key))
		{
			Circle->Filled = lua_toboolean(rL, -1);
		}
		else if (!strcmp("Transparency", Key))
		{
			Circle->Transparency = (float)lua_tonumber(rL, -1);
		}
		else if (!strcmp("NumSides", Key))
		{
			Circle->NumSides = lua_tonumber(rL, -1);
		}
		else if (!strcmp("Visible", Key))
		{
			for (int i = 0; i < DrawingsVector.size(); i++)
			{
				if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Circle))
				{
					DrawingsVector[i]->Visible = lua_toboolean(rL, -1);
				}
			}
		}
		return 0;
	}

	int LineRemove(lua_State* rL)
	{
		auto Line = (D3DLine*)lua_touserdata(rL, -1);

		for (int i = 0; i < DrawingsVector.size(); i++)
		{
			if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Line))
			{
				DrawingsVector.erase(DrawingsVector.begin() + i);
			}
		}
		auto Space = ExtraSpace::GetRoot(rL);
		r_lua_pushlightuserdata(Space->RL, Line);
		r_lua_rawget(Space->RL, LUA_REGISTRYINDEX);
		uintptr_t  RLine = r_lua_touserdata(Space->RL, -1);
		r_lua_pop(Space->RL, 1);

		lua_pushlightuserdata(rL, reinterpret_cast<void*>(RLine));
		lua_pushnumber(rL, RLine);
		lua_rawset(rL, LUA_REGISTRYINDEX);

		r_lua_pushlightuserdata(Space->RL, Line);
		r_lua_pushnumber(Space->RL, reinterpret_cast<uintptr_t>(Line));
		r_lua_rawset(Space->RL, LUA_REGISTRYINDEX, NULL);
		return 0;
	}


	int LineIndex(lua_State* rL)
	{
		const char* Key = lua_tolstring(rL, -1, 0);
		const auto Line = (D3DLine*)lua_touserdata(rL, -2);
		if (!strcmp("Remove", Key))
		{
			lua_pushcclosure(rL, LineRemove, 0);
		}
		else if (!strcmp("From", Key))
		{
			makevec(rL, Line->From);
		}
		else if (!strcmp("To", Key))
		{
			makevec(rL, Line->To);
		}
		else if (!strcmp("Color", Key))
		{
			makecolor(rL, Line->Color);
		}
		else if (!strcmp("Thickness", Key))
		{
			lua_pushnumber(rL, Line->Thickness);
		}
		else if (!strcmp("Transparency", Key))
		{
			lua_pushnumber(rL, Line->Transparency);
		}
		else if (!strcmp("Visible", Key))
		{
			for (int i = 0; i < DrawingsVector.size(); i++)
			{
				if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Line))
				{
					lua_pushboolean(rL, DrawingsVector[i]->Visible);
				}
			}
		}
		return 1;
	}

	int LineNewIndex(lua_State* rL)
	{
		const char* Key = lua_tolstring(rL, -2, 0);
		const auto Line = (D3DLine*)lua_touserdata(rL, -3);
		if (!strcmp("From", Key))
		{
			Line->From = getvec(rL, -1);
		}
		else if (!strcmp("To", Key))
		{
			Line->To = getvec(rL, -1);
		}
		else if (!strcmp("Color", Key))
		{
			Line->Color = getcolor(rL, -1);
		}
		else if (!strcmp("Thickness", Key))
		{
			Line->Thickness = (float)lua_tonumber(rL, -1);
		}
		else if (!strcmp("Transparency", Key))
		{
			Line->Transparency = (float)lua_tonumber(rL, -1);
		}
		else if (!strcmp("Visible", Key))
		{
			for (int i = 0; i < DrawingsVector.size(); i++)
			{
				if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Line))
				{
					DrawingsVector[i]->Visible = lua_toboolean(rL, -1);
				}
			}
		}
		return 0;
	}

	int SquareRemove(lua_State* rL)
	{
		auto Square = (D3DSquare*)lua_touserdata(rL, -1);
		for (int i = 0; i < DrawingsVector.size(); i++)
		{
			if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Square))
			{
				DrawingsVector.erase(DrawingsVector.begin() + i);
			}
		}
		auto Space = ExtraSpace::GetRoot(rL);
		r_lua_pushlightuserdata(Space->RL, Square);
		r_lua_rawget(Space->RL, LUA_REGISTRYINDEX);
		uintptr_t  RSquare = r_lua_touserdata(Space->RL, -1);
		r_lua_pop(Space->RL, 1);

		lua_pushlightuserdata(rL, reinterpret_cast<void*>(RSquare));
		lua_pushnumber(rL, RSquare);
		lua_rawset(rL, LUA_REGISTRYINDEX);

		r_lua_pushlightuserdata(Space->RL, Square);
		r_lua_pushnumber(Space->RL, reinterpret_cast<uintptr_t>(Square));
		r_lua_rawset(Space->RL, LUA_REGISTRYINDEX, NULL);
		return 0;
	}

	int SquareIndex(lua_State* rL)
	{
		const char* Key = lua_tolstring(rL, -1, 0);
		const auto Square = (D3DSquare*)lua_touserdata(rL, -2);
		if (!strcmp("Remove", Key))
		{
			lua_pushcclosure(rL, SquareRemove, 0);
		}
		else if (!strcmp("Position", Key))
		{
			makevec(rL, Square->Position);
		}
		else if (!strcmp("Size", Key))
		{
			makevec(rL, Square->Size);
		}
		else if (!strcmp("Color", Key))
		{
			makecolor(rL, Square->Color);
		}
		else if (!strcmp("Thickness", Key))
		{
			lua_pushnumber(rL, Square->Thickness);
		}
		else if (!strcmp("Filled", Key))
		{
			lua_pushboolean(rL, Square->Filled);
		}
		else if (!strcmp("Transparency", Key))
		{
			lua_pushnumber(rL, Square->Transparency);
		}
		else if (!strcmp("Visible", Key))
		{
			for (int i = 0; i < DrawingsVector.size(); i++)
			{
				if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Square))
				{
					lua_pushboolean(rL, DrawingsVector[i]->Visible);
				}
			}
		}
		return 1;
	}

	int SquareNewIndex(lua_State* rL)
	{
		const char* Key = lua_tolstring(rL, 2, 0);
		const auto Square = (D3DSquare*)lua_touserdata(rL, 1);
		if (!strcmp("Position", Key))
		{
			Square->Position = getvec(rL, 3);
		}
		else if (!strcmp("Size", Key))
		{
			Square->Size = getvec(rL, 3);
		}
		else if (!strcmp("Color", Key))
		{
			Square->Color = getcolor(rL, -1);
		}
		else if (!strcmp("Thickness", Key))
		{
			Square->Thickness = (float)lua_tonumber(rL, 3);
		}
		else if (!strcmp("Filled", Key))
		{
			Square->Filled = lua_toboolean(rL, 3);
		}
		else if (!strcmp("Transparency", Key))
		{
			Square->Transparency = (float)lua_tonumber(rL, 3);
		}
		else if (!strcmp("Visible", Key))
		{
			for (int i = 0; i < DrawingsVector.size(); i++)
			{
				if (DrawingsVector[i]->Object == reinterpret_cast<uintptr_t*>(Square))
				{
					DrawingsVector[i]->Visible = lua_toboolean(rL, 3);
				}
			}
		}
		return 0;
	}

	void lua_setmetatable(lua_State* ls, const char* tname)
	{
		luaL_getmetatable(ls, tname);
		lua_setmetatable(ls, -2);
	}


	int lua_newmetatable(lua_State* rL, const char* name)
	{
		lua_getfield(rL, -10000, name);
		if (!lua_isnil(rL, -1))
			return 0;
		lua_pop(rL, 1);
		lua_createtable(rL, 0, 0);
		lua_pushvalue(rL, -1);
		lua_setfield(rL, -10000, name);
		return 0;
	}

	int New(lua_State* rL)
	{
		const char* Key = lua_tostring(rL, -1);
		if (!strcmp("Text", Key))
		{
			D3DDrawing* Text = new D3DDrawing;
			Text->type = DrawingEnum::Text;
			const auto TextUD = reinterpret_cast<D3DText*>(lua_newuserdata(rL, sizeof(D3DText)));
			auto Space = ExtraSpace::GetRoot(rL);
			Env::Wrapping::Wrap(rL, Space->RL, -1);
			r_lua_pop(Space->RL, 1);
			*TextUD = D3DText();
			lua_setmetatable(rL, "Text");
			Text->Object = reinterpret_cast<uintptr_t*>(TextUD);
			DrawingsVector.push_back(Text);
		}
		else if (!strcmp("Circle", Key))
		{
			D3DDrawing* Circle = new D3DDrawing;
			Circle->type = DrawingEnum::Circle;
			const auto CircleUD = reinterpret_cast<D3DCircle*>(lua_newuserdata(rL, sizeof(D3DCircle)));
			auto Space = ExtraSpace::GetRoot(rL);
			Env::Wrapping::Wrap(rL, Space->RL, -1);
			r_lua_pop(Space->RL, 1);
			lua_setmetatable(rL, "Circle");
			*CircleUD = D3DCircle();
			Circle->Object = reinterpret_cast<uintptr_t*>(CircleUD);
			DrawingsVector.push_back(Circle);
		}
		else if (!strcmp("Line", Key))
		{
			D3DDrawing* Line = new D3DDrawing;
			Line->type = DrawingEnum::Line;
			const auto LineUD = reinterpret_cast<D3DLine*>(lua_newuserdata(rL, sizeof(D3DLine)));
			auto Space = ExtraSpace::GetRoot(rL);
			Env::Wrapping::Wrap(rL, Space->RL, -1);
			r_lua_pop(Space->RL, 1);
			*LineUD = D3DLine();
			lua_setmetatable(rL, "Line");
			Line->Object = reinterpret_cast<uintptr_t*>(LineUD);
			DrawingsVector.push_back(Line);
		}
		else if (!strcmp("Square", Key))
		{
			D3DDrawing* Square = new D3DDrawing;
			Square->type = DrawingEnum::Square;
			const auto SquareUD = reinterpret_cast<D3DSquare*>(lua_newuserdata(rL, sizeof(D3DSquare)));
			auto Space = ExtraSpace::GetRoot(rL);
			Env::Wrapping::Wrap(rL, Space->RL, -1);
			r_lua_pop(Space->RL, 1);
			*SquareUD = D3DSquare();
			lua_setmetatable(rL, "Square");
			Square->Object = reinterpret_cast<uintptr_t*>(SquareUD);
			DrawingsVector.push_back(Square);
		}
		return 1;
	}

	int Clear(lua_State* rL)
	{
		DrawingsVector.clear();
		return 0;
	}

	int SetUpDrawing(lua_State* rL)
	{
		lua_newmetatable(rL, "Text");
		lua_pushcclosure(rL, TextIndex, 0);
		lua_setfield(rL, -2, "__index");
		lua_pushcclosure(rL, TextNewIndex, 0);
		lua_setfield(rL, -2, "__newindex");
		lua_pop(rL, 1);

		lua_newmetatable(rL, "Circle");
		lua_pushcclosure(rL, CircleIndex, 0);
		lua_setfield(rL, -2, "__index");
		lua_pushcclosure(rL, CircleNewIndex, 0);
		lua_setfield(rL, -2, "__newindex");
		lua_pop(rL, 1);

		lua_newmetatable(rL, "Line");
		lua_pushcclosure(rL, LineIndex, 0);
		lua_setfield(rL, -2, "__index");
		lua_pushcclosure(rL, LineNewIndex, 0);
		lua_setfield(rL, -2, "__newindex");
		lua_pop(rL, 1);

		lua_newmetatable(rL, "Square");
		lua_pushcclosure(rL, SquareIndex, 0);
		lua_setfield(rL, -2, "__index");
		lua_pushcclosure(rL, SquareNewIndex, 0);
		lua_setfield(rL, -2, "__newindex");
		lua_pop(rL, 1);

		lua_createtable(rL, 0, 0);
		{
			lua_pushcclosure(rL, &New, 0);
			lua_setfield(rL, -2, "new");

			lua_pushcclosure(rL, &Clear, 0);
			lua_setfield(rL, -2, "Clear");

			lua_pushcclosure(rL, &Clear, 0);
			lua_setfield(rL, -2, "clear");
		}
		lua_setfield(rL, LUA_GLOBALSINDEX, "Drawing");
		return 0;
	}
}
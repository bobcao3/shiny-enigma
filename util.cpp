//=============================================================================
// ■ Util
//-----------------------------------------------------------------------------
//   A module for dealing with random things in Lua.
//   As the number of the functions grows, they will eventually be moved
//   to a new file and have their own category.
//=============================================================================

namespace Util {
	//-------------------------------------------------------------------------
	// ● to_color
	//   Unspecified alpha will be set to 255.
	//-------------------------------------------------------------------------
	void to_color(lua_State* L, int index, SDL_Color* color) {
		lua_getfield(L, index, "r"); color->r = lua_tointeger(L, -1);
		lua_getfield(L, index, "g"); color->g = lua_tointeger(L, -1);
		lua_getfield(L, index, "b"); color->b = lua_tointeger(L, -1);
		lua_getfield(L, index, "a"); color->a = luaL_optinteger(L, -1, 255);
		lua_pop(L, 4);
	}
	//-------------------------------------------------------------------------
	// ● create_color
	//-------------------------------------------------------------------------
	void create_color(lua_State* L, const SDL_Color* color) {
		lua_createtable(L, 0, 4);
		lua_pushnumber(L, color->r); lua_setfield(L, -2, "r");
		lua_pushnumber(L, color->g); lua_setfield(L, -2, "g");
		lua_pushnumber(L, color->b); lua_setfield(L, -2, "b");
		lua_pushnumber(L, color->a); lua_setfield(L, -2, "a");
	}
	//-------------------------------------------------------------------------
	// ● class(parent = nil)
	//-------------------------------------------------------------------------
	int new_class(lua_State* L) {
		bool has_parent = !lua_isnoneornil(L, 1);
		lua_createtable(L, 0, 2);
		// t.__index = t
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		// if arg[1] then
		if (has_parent) {
			// setmetatable(t, arg[1])
			lua_pushvalue(L, 1);
			lua_setmetatable(L, -2);
		}
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"class", new_class},
			{NULL, NULL}
		};
		luaL_register(L, "Util", reg);
		lua_pop(L, 1);
	}
}

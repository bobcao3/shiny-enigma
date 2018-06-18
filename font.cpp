//=============================================================================
// ■ Font
//-----------------------------------------------------------------------------
//   A Lua class for handling fonts.
//=============================================================================

namespace Font {
	//-------------------------------------------------------------------------
	// ● check_font
	//-------------------------------------------------------------------------
	TTF_Font* check_font(lua_State* L, int index) {
		return *((TTF_Font**) luaL_checkudata(L, index, "Font"));
	}
	//-------------------------------------------------------------------------
	// ● ~Font()
	//-------------------------------------------------------------------------
	int lua_gc(lua_State* L) {
		TTF_Font* font = check_font(L, 1);
		TTF_CloseFont(font);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"__gc", lua_gc},
			{NULL, NULL}
		};
		luaL_newmetatable(L, "Font");
		luaL_register(L, NULL, reg);
		lua_setfield(L, -1, "__index");
	}
}

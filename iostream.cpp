//=============================================================================
// ■ IOStream
//-----------------------------------------------------------------------------
//   A Lua module that wraps SDL_RWops.
//=============================================================================

namespace IOStream {
	//-------------------------------------------------------------------------
	// ● check_iostream
	//-------------------------------------------------------------------------
	SDL_RWops* check_iostream(lua_State* L, int index) {
		return (SDL_RWops*) Util::check_usertable(L, index, "IOStream");
	}
	//-------------------------------------------------------------------------
	// ● close()
	//-------------------------------------------------------------------------
	int lua_close(lua_State* L) {
		lua_rawgeti(L, 1, 0);
		SDL_RWops* context = (SDL_RWops*) lua_touserdata(L, -1);
		SDL_RWclose(context);
		lua_rawgeti(L, 1, 1);
		void* freeme = lua_touserdata(L, -1);
		if (freeme) free(freeme);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● get_size()
	//   Returns -1 if the size can't be determined.
	//-------------------------------------------------------------------------
	int lua_get_size(lua_State* L) {
		lua_pushnumber(L, SDL_RWsize(check_iostream(L, lua_upvalueindex(1))));
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● get_pos()
	//   Returns -1 if the pos can't be determined.
	//-------------------------------------------------------------------------
	int lua_get_pos(lua_State* L) {
		lua_pushnumber(L, SDL_RWtell(check_iostream(L, lua_upvalueindex(1))));
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● set_pos(offset, whence = "set")
	//    whence: "set", "cur" or "end"
	//-------------------------------------------------------------------------
	int lua_set_pos(lua_State* L) {
		Sint64 offset = luaL_checkinteger(L, 1);
		int whence;
		const char* s = luaL_optstring(L, 2, "set");
		if (strcmp(s, "set") == 0) {
			whence = RW_SEEK_SET;
		} else if (strcmp(s, "cur") == 0) {
			whence = RW_SEEK_CUR;
		} else if (strcmp(s, "end") == 0) {
			whence = RW_SEEK_END;
		}
		Sint64 r = SDL_RWseek(
			check_iostream(L, lua_upvalueindex(1)),
			offset, whence
		);
		if (r == -1) Util::sdlerror(L, "SDL_RWseek() == -1");
		lua_pushnumber(L, r);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● create_iostream
	//-------------------------------------------------------------------------
	void create_iostream(lua_State* L, SDL_RWops* context, void* freeme = NULL) {
		// t = {}
		lua_createtable(L, 2, 4);
		// t[0] = context
		lua_pushlightuserdata(L, context);
		lua_rawseti(L, -2, 0);
		// t[1] = freeme to be freed
		lua_pushlightuserdata(L, freeme);
		lua_rawseti(L, -2, 1);
		const luaL_reg reg[] = {
			{"close", lua_close},
			{"get_size", lua_get_size},
			{"get_pos", lua_get_pos},
			{"set_pos", lua_set_pos},
			{NULL, NULL}
		};
		Util::instance_register(L, reg);
	}
	//-------------------------------------------------------------------------
	// ● new(size, mode)
	//    mode: "r" or "r+"
	//-------------------------------------------------------------------------
	int lua_new_instance(lua_State* L) {
		int size = luaL_checkinteger(L, 1);
		void* mem = malloc(size);
		if (!mem) luaL_error(L, "cannot allocate memory");

		bool readonly = true;
		const char* mode = luaL_checkstring(L, 2);
		while (mode) {
			switch (*mode) {
			case '+':
				readonly = false;
				break;
			case 'r':
				break;
			default:
				luaL_argerror(L, 2, "unexpected mode");
			}
			mode++;
		}

		SDL_RWops* context;
		if (readonly) {
			context = SDL_RWFromMem(mem, size);
		} else {
			context = SDL_RWFromConstMem(mem, size);
		}
		if (!context) {
			Util::sdlerror(L, "SDL_RWFromMem/SDL_RWFromConstMem() == NULL");
		}
		create_iostream(L, context, mem);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● open(filename, mode)
	//-------------------------------------------------------------------------
	int lua_open_instance(lua_State* L) {
		SDL_RWops* context = SDL_RWFromFile(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
		if (!context) Util::sdlerror(L, "SDL_RWFromFile() == NULL");
		create_iostream(L, context);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"new", lua_new_instance},
			{"open", lua_open_instance},
			{NULL, NULL}
		};
		luaL_register(L, "IOStream", reg);
		lua_pop(L, 1);
	}
}

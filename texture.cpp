//=============================================================================
// ■ Texture
//-----------------------------------------------------------------------------
//   A Lua class for handling textures.
//=============================================================================

namespace Texture {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	// ● check_texture
	//-------------------------------------------------------------------------
	inline SDL_Texture* check_texture(lua_State* L, int index) {
		return (SDL_Texture*) Util::check_usertable(L, index, "texture");
	}
	//-------------------------------------------------------------------------
	// ● ~Texture()
	//-------------------------------------------------------------------------
	int lua_gc(lua_State* L) {
		SDL_Texture* texture = check_texture(L, 1);
		SDL_DestroyTexture(texture);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● get_rect() → rect
	//-------------------------------------------------------------------------
	int lua_get_rect(lua_State* L) {
		SDL_Texture* texture = check_texture(L, lua_upvalueindex(1));
		Uint32 format;
		int w, h, access;
		SDL_QueryTexture(texture, &format, &access, &w, &h);
		SDL_Rect rect = {0, 0, w, h};
		Geometry::create_rect(L, &rect);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● set_color(color)
	//-------------------------------------------------------------------------
	int lua_set_color(lua_State* L) {
		SDL_Texture* texture = check_texture(L, lua_upvalueindex(1));
		SDL_Color color;
		Util::to_color(L, 1, &color);
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● set_blend(mode)
	//-------------------------------------------------------------------------
	int lua_set_blend(lua_State* L) {
		SDL_Texture* texture = check_texture(L, lua_upvalueindex(1));
		SDL_SetTextureBlendMode(texture, Graphics::check_blend(L, 1));
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● create_texture
	//-------------------------------------------------------------------------
	void create_texture(lua_State* L, SDL_Texture* texture) {
		// t = {}
		lua_createtable(L, 1, 4);
		// t[0] = texture
		lua_pushlightuserdata(L, texture);
		lua_rawseti(L, -2, 0);
		const luaL_reg reg[] = {
			{"__gc", lua_gc},
			{"get_rect", lua_get_rect},
			{"set_color", lua_set_color},
			{"set_blend", lua_set_blend},
			{NULL, NULL}
		};
		Util::instance_register(L, reg);
	}
	//-------------------------------------------------------------------------
	// ● new(filename)
	//-------------------------------------------------------------------------
	int lua_new_instance(lua_State* L) {
		const char* filename = lua_tostring(L, 1);
		create_texture(L, IMG_LoadTexture($renderer, Util::rtp(filename)));
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● render_text(font, text) → texture reference
	//   font: font ID or reference
	//-------------------------------------------------------------------------
	int lua_render_text(lua_State* L) {
		TTF_Font* font;
		if (lua_isuserdata(L, 1)) {
			font = Font::check_font(L, 1);
		} else {
			font = $font[luaL_checkinteger(L, 1)];
		}
		const char* text = luaL_checkstring(L, 2);
		if (!*text) luaL_argerror(L, 2, "no text to render");
		SDL_Color color = {255, 255, 255, 255};
		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
		if (!surface) luaL_error(L, "TTF_RenderUTF8() == NULL");
		create_texture(L, SDL_CreateTextureFromSurface($renderer, surface));
		SDL_FreeSurface(surface);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init_load_assets
	//-------------------------------------------------------------------------
	void init_load_assets() {
		for (int i = 0; i < 3; i++) {
			SDL_Surface* surface;
			SDL_Texture* texture;
			char* filename;
			{
				char name[16];
				sprintf(name, "tex%d.png", i);
				filename = Util::rtp(name);
			}
			switch (i) {
			case 1:
				surface = IMG_Load(filename);
				{
					SDL_Surface* converted;
					converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
					SDL_FreeSurface(surface);
					surface = converted;
				}
				#define pixel ((Uint32*) (((Uint8*) surface->pixels) + y * surface->pitch))[x]
				for (int y = 0; y < surface->h; y++) {
					for (int x = 0; x < surface->w; x++) {
						SDL_Color color;
						SDL_GetRGBA(pixel, surface->format, &color.r, &color.g, &color.b, &color.a);
						color.a = color.r;
						color.r = color.g = color.b = 255;
						pixel = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
					}
				}
				#undef pixel
				texture = SDL_CreateTextureFromSurface($renderer, surface);
				SDL_FreeSurface(surface);
				break;
			default:
				texture = IMG_LoadTexture($renderer, filename);
			}
			$texture[i] = texture;
		}
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		init_load_assets();
		const luaL_reg reg[] = {
			{"new", lua_new_instance},
			{"render_text", lua_render_text},
			{NULL, NULL}
		};
		luaL_register(L, "Texture", reg);
		// Texture.stock = {…}
		lua_createtable(L, 3, 0);
		for (int i = 0; i < 3; i++) {
			create_texture(L, $texture[i]);
			lua_rawseti(L, -2, i);
		}
		lua_setfield(L, -2, "stock");
		lua_pop(L, 1);
	}
}

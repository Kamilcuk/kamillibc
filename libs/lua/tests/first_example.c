// Based on https://www.lua.org/pil/24.1.html
// Based on https://www.lua.org/pil/1.html
// https://stackoverflow.com/questions/25238892/syntax-change-in-lua-5-2-c-api/25239074
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	const char buff[] = "print(\"Hello World\")";
	const int error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
	if (error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1); /* pop error message from the stack */
	}

	lua_close(L);
	return 0;
}

//// PASS_REGULAR_EXPRESSION Hello World


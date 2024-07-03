#pragma once

#include <functional>
#include <json.h>

#ifndef XHostedPremium_H

#define XHostedPremium_H

extern uintptr_t x_Top;
extern uintptr_t x_Base;
extern uintptr_t x_Identity2;
extern uintptr_t x_ScriptPtr;

extern uintptr_t x_k;
extern uintptr_t x_sizek;
extern uintptr_t x_p;
extern uintptr_t x_sizep;
extern uintptr_t x_code;
extern uintptr_t x_sizecode;
extern uintptr_t x_sizelineinfo;
extern uintptr_t x_sizeupvalues;
extern uintptr_t x_protonups;
extern uintptr_t x_upvalues;
extern uintptr_t x_numparams;
extern uintptr_t x_is_vararg;
extern uintptr_t x_maxstacksize;
extern uintptr_t x_lineinfo;
extern uintptr_t x_sizelocvars;
extern uintptr_t x_locvars;
extern uintptr_t x_linedefined;
extern uintptr_t x_lastlinedefined;
extern uintptr_t x_source;

extern uintptr_t x_IsC;
extern uintptr_t x_closurenups;

extern uintptr_t x_rootgc;
extern uintptr_t x_gchtt;
extern uintptr_t x_gchmarked;

extern uintptr_t x_IsLoaded;
extern uintptr_t x_InstanceClass;
extern uintptr_t x_Networked;
extern uintptr_t x_IsReadOnly;
extern uintptr_t x_namecall;

extern uintptr_t x_RobloxInstancePushFunction;

extern uintptr_t x_UnLockModule;

extern uintptr_t R_LUA_TNIL, R_LUA_TLIGHTUSERDATA, R_LUA_TNUMBER, R_LUA_TBOOLEAN, R_LUA_TSTRING, R_LUA_TTHREAD, R_LUA_TFUNCTION, R_LUA_TTABLE, R_LUA_TUSERDATA, R_LUA_TPROTO, R_LUA_TUPVALUE;
extern std::function<void(uintptr_t, int, const char*)> r_lua_getfield;
extern std::function<void(uintptr_t, int, const char*)> r_lua_setfield;
extern std::function<char* (uintptr_t, int, size_t*)> r_lua_tolstring;
extern std::function<void(uintptr_t, int)> r_lua_settop;
extern std::function<bool(uintptr_t, int)> r_lua_toboolean;
extern std::function<void(uintptr_t, int)> r_lua_pushvalue;
extern std::function<int(int, double)> r_lua_pushnumber;
extern std::function<void(uintptr_t, const char*)> r_lua_pushstring;
extern std::function<int(uintptr_t, int, int, int)> r_lua_pcall;
extern std::function<void(uintptr_t)> r_lua_pushnil;
extern std::function<int(uintptr_t, int)> r_lua_next;
extern std::function<void(uintptr_t, bool)> r_lua_pushboolean;
extern std::function<double(uintptr_t, int, int)> r_lua_tonumber;
extern std::function<void(uintptr_t, int, int, int, int)> r_lua_pushcclosure;
extern std::function<void(uintptr_t, int, int)> r_lua_createtable;
extern std::function<uintptr_t(uintptr_t)> r_lua_newthread;
extern std::function<uintptr_t* (int, unsigned int, char)> r_lua_newuserdata;
extern std::function<void(uintptr_t, int, int)> r_lua_rawgeti;
extern std::function<void(uintptr_t, int, int)> r_lua_rawseti;
extern std::function<int(uintptr_t, int)> r_lua_replace;
extern std::function<int(uintptr_t, int)> r_lua_getmetatable;
extern std::function<void(uintptr_t, int)> r_lua_setmetatable;
extern std::function<uintptr_t(uintptr_t, int)> r_lua_touserdata;
extern std::function<int(uintptr_t, int)> r_lua_type;
extern std::function<const char* (uintptr_t, int)> r_lua_typename;
extern std::function<int(uintptr_t, int)> r_luaL_ref;
extern std::function<void(uintptr_t, int, int)> r_luaL_unref;
extern std::function<void(uintptr_t, int)> r_lua_settable;
extern std::function<void(uintptr_t, void*)> r_lua_pushlightuserdata;
extern std::function<int(uintptr_t)> r_lua_gettop;
extern std::function<uintptr_t* (uintptr_t, int)> r_index2adr;
extern std::function<uintptr_t(uintptr_t, int)> r_lua_tothread;
extern std::function<void(uintptr_t, int, int)> r_lua_rawset;
extern std::function<void(uintptr_t, int)> r_lua_rawget;
extern std::function<void(uintptr_t, int, int)> r_lua_call;
extern std::function<void(uintptr_t, int)> r_lua_remove;
extern std::function<const uintptr_t* (uintptr_t, int)> r_lua_topointer;
extern std::function<bool(uintptr_t, int)> r_lua_iscfunction;
extern std::function<void(uintptr_t, int)> r_lua_gettable;
extern std::function<size_t(uintptr_t, int)> r_lua_objlen;
extern std::function<void(uintptr_t)> r_lua_pushthread;
extern std::function<void(uintptr_t, uintptr_t, int)> r_lua_xmove;
extern std::function<uintptr_t* (uintptr_t, int, int)> r_lua_getupvalue;
extern std::function<uintptr_t* (uintptr_t, int, int)> r_lua_setupvalue;
extern std::function<void(uintptr_t, int)> r_lua_setfenv;
extern std::function<void(uintptr_t, int)> r_lua_getfenv;
extern std::function<int(uintptr_t)> r_lua_error;
extern std::function<int(uintptr_t, int)> r_lua_yield;
extern std::function<void(uintptr_t, const char*)> r_print;
extern std::function<void(uintptr_t, const char*)> r_luaL_error;
extern std::function<void(uintptr_t, int)> r_lua_checkstack;
extern std::function<int(uintptr_t, uintptr_t, int)> r_lua_resume;
extern std::function<void(uintptr_t, int)> r_lua_insert;
extern std::function<void(uintptr_t, int, int)> r_luaL_checktype;

extern std::function<bool(uintptr_t, int)> r_lua_isnumber;
extern std::function<bool(uintptr_t, int)> r_lua_isuserdata;
extern std::function<void(uintptr_t)> r_spawn;
extern std::function<void(uintptr_t, float, uintptr_t)> r_fireclickdetector;
extern std::function<void(uintptr_t, uintptr_t, bool)> r_lua_setreadonly;
extern std::function<bool(uintptr_t, int)> r_lua_getreadonly;
extern std::function<int(uintptr_t, const char*, const char*, int)> r_deserialize;
extern std::function<uintptr_t(uintptr_t, uintptr_t)> r_getdatamodel;
extern std::function<int(uintptr_t, int, uintptr_t)> r_sandboxthread;
extern std::function<uintptr_t(uintptr_t, uintptr_t)> r_lua_atpanic;
extern std::function<void(uintptr_t, int)> r_lua_concat;
extern std::function<int(uintptr_t, int, int)> r_lua_gc;
extern std::function<void(uintptr_t, ptrdiff_t)> r_lua_pushinteger;
extern std::function<int(uintptr_t, int, int)> r_lua_rawequal;
extern std::function<ptrdiff_t(uintptr_t, int)> r_lua_tointeger;

extern std::function<const char* (uintptr_t, int, uintptr_t*)> r_aux_upvalue;
extern std::function<uintptr_t(uintptr_t)> r_luaF_newproto;
extern std::function<void(uintptr_t, uintptr_t)> r_luaF_freeproto;
extern std::function<uintptr_t(uintptr_t, int, uintptr_t)> r_luaF_newcclosure;
extern std::function<uintptr_t(uintptr_t, int, uintptr_t, uintptr_t)> r_luaF_newlclosure;

extern std::function<uintptr_t(uintptr_t, const std::string&)> r_findfirstchild;
extern std::function<void(uintptr_t, uintptr_t, const std::string&)> r_findfirstchildofclass;
extern Json::Value JSON;

bool Run(uintptr_t RetCheck(uintptr_t));
uintptr_t GetDataModel();
uintptr_t GetScriptContext();
uintptr_t GetLevel(uintptr_t RobloxState);
void SetLevel(uintptr_t RobloxState, int Level);
uintptr_t GetRState();
uintptr_t GetProto(uintptr_t Closure);
uintptr_t DeObf(uintptr_t Proto, uintptr_t Offset);
uintptr_t GetAddress(uintptr_t Closure);
void SetAddress(uintptr_t Closure, uintptr_t To);
const char* GetName(uintptr_t Closure);
uintptr_t GetGlobalState(uintptr_t RL);

#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_setglobal(rL,s)	r_lua_setfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)

#endif

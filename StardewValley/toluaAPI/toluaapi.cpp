/*
** Lua binding: toluaapi
** Generated automatically by tolua++-version on [The timestamp has been removed by xiaou].
*/

/****************************************************************************
 Copyright (c) 2011 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

/* Exported function */
TOLUA_API int  tolua_toluaapi_open (lua_State* tolua_S);

#include "tolua_fix.h"
#include "CCLuaValue.h"
#include "TLEditInterface.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: resetPropList */
#ifndef TOLUA_DISABLE_tolua_toluaapi_resetPropList00
static int tolua_toluaapi_resetPropList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   resetPropList();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resetPropList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: addPropGroup */
#ifndef TOLUA_DISABLE_tolua_toluaapi_addPropGroup00
static int tolua_toluaapi_addPropGroup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string strGroupName = ((const std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   addPropGroup(strGroupName);
   tolua_pushcppstring(tolua_S,(const char*)strGroupName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addPropGroup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: addPropItem */
#ifndef TOLUA_DISABLE_tolua_toluaapi_addPropItem00
static int tolua_toluaapi_addPropItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string strItemInfo = ((const std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   addPropItem(strItemInfo);
   tolua_pushcppstring(tolua_S,(const char*)strItemInfo);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addPropItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_toluaapi_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"resetPropList",tolua_toluaapi_resetPropList00);
  tolua_function(tolua_S,"addPropGroup",tolua_toluaapi_addPropGroup00);
  tolua_function(tolua_S,"addPropItem",tolua_toluaapi_addPropItem00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_toluaapi (lua_State* tolua_S) {
 return tolua_toluaapi_open(tolua_S);
};
#endif


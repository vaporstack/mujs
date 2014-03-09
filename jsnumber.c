#include "jsi.h"
#include "jsvalue.h"
#include "jsbuiltin.h"

static void jsB_new_Number(js_State *J)
{
	js_newnumber(J, js_isdefined(J, 1) ? js_tonumber(J, 1) : 0);
}

static void jsB_Number(js_State *J)
{
	js_pushnumber(J, js_isdefined(J, 1) ? js_tonumber(J, 1) : 0);
}

static void Np_valueOf(js_State *J)
{
	js_Object *self = js_toobject(J, 0);
	if (self->type != JS_CNUMBER) js_typeerror(J, "not a number");
	js_pushnumber(J, self->u.number);
}

static void Np_toString(js_State *J)
{
	js_Object *self = js_toobject(J, 0);
	int radix = js_isundefined(J, 1) ? 10 : js_tointeger(J, 1);
	if (self->type != JS_CNUMBER) js_typeerror(J, "not a number");
	if (radix < 2 || radix > 36)
		js_rangeerror(J, "invalid radix");
	if (radix != 10)
		js_rangeerror(J, "invalid radix");
	js_pushliteral(J, jsV_numbertostring(J, self->u.number));
}

/* Customized ToString() on a number */
void numtostr(js_State *J, const char *fmt, int w, double n)
{
	char buf[40];
	if (isnan(n)) js_pushliteral(J, "NaN");
	else if (isinf(n)) js_pushliteral(J, n < 0 ? "-Infinity" : "Infinity");
	else if (n == 0) js_pushliteral(J, "0");
	else {
		sprintf(buf, fmt, w, n);
		js_pushstring(J, buf);
	}
}

static void Np_toFixed(js_State *J)
{
	js_Object *self = js_toobject(J, 0);
	int width = js_tointeger(J, 1);
	if (self->type != JS_CNUMBER) js_typeerror(J, "not a number");
	numtostr(J, "%.*f", width, self->u.number);
}

static void Np_toExponential(js_State *J)
{
	js_Object *self = js_toobject(J, 0);
	int width = js_tointeger(J, 1);
	if (self->type != JS_CNUMBER) js_typeerror(J, "not a number");
	numtostr(J, "%.*e", width, self->u.number);
}

static void Np_toPrecision(js_State *J)
{
	js_Object *self = js_toobject(J, 0);
	int width = js_tointeger(J, 1);
	if (self->type != JS_CNUMBER) js_typeerror(J, "not a number");
	numtostr(J, "%.*g", width, self->u.number);
}

void jsB_initnumber(js_State *J)
{
	J->Number_prototype->u.number = 0;

	js_pushobject(J, J->Number_prototype);
	{
		jsB_propf(J, "valueOf", Np_valueOf, 0);
		jsB_propf(J, "toString", Np_toString, 1);
		jsB_propf(J, "toLocaleString", Np_toString, 0);
		jsB_propf(J, "toFixed", Np_toFixed, 1);
		jsB_propf(J, "toExponential", Np_toExponential, 1);
		jsB_propf(J, "toPrecision", Np_toPrecision, 1);
	}
	js_newcconstructor(J, jsB_Number, jsB_new_Number, 1);
	{
		jsB_propn(J, "MAX_VALUE", DBL_MAX);
		jsB_propn(J, "MIN_VALUE", DBL_MIN);
		jsB_propn(J, "NaN", NAN);
		jsB_propn(J, "NEGATIVE_INFINITY", -INFINITY);
		jsB_propn(J, "POSITIVE_INFINITY", INFINITY);
	}
	js_defglobal(J, "Number", JS_DONTENUM);
}

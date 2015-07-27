/*
 * The MIT License (MIT)
 *
 * Copyright (c) Microsoft Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __PYJIT_H__
#define __PYJIT_H__

#define FEATURE_NO_HOST
#define USE_STL
#include <stdint.h>
#include <windows.h>
#include <wchar.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <share.h>
#include <cstdlib>
#include <intrin.h>

#include <Python.h>

#include <vector>
#include <hash_map>

#include <Python.h>

// binary operator helpers
#define METHOD_ADD_TOKEN					0x00000000
#define METHOD_MULTIPLY_TOKEN				0x00000001
#define METHOD_SUBTRACT_TOKEN				0x00000002
#define METHOD_DIVIDE_TOKEN					0x00000003
#define METHOD_FLOORDIVIDE_TOKEN			0x00000004
#define METHOD_POWER_TOKEN					0x00000005
#define METHOD_MODULO_TOKEN					0x00000006
#define METHOD_SUBSCR_TOKEN					0x00000007
#define METHOD_STOREMAP_TOKEN				0x00000008
#define METHOD_RICHCMP_TOKEN				0x00000009
#define METHOD_CONTAINS_TOKEN				0x0000000A
#define METHOD_NOTCONTAINS_TOKEN			0x0000000B
#define METHOD_STORESUBSCR_TOKEN			0x0000000C
#define METHOD_DELETESUBSCR_TOKEN			0x0000000D
#define METHOD_NEWFUNCTION_TOKEN			0x0000000E
#define METHOD_GETITER_TOKEN				0x0000000F
#define METHOD_DECREF_TOKEN					0x00000010
#define METHOD_GETBUILDCLASS_TOKEN			0x00000011
#define METHOD_LOADNAME_TOKEN				0x00000012
#define METHOD_STORENAME_TOKEN				0x00000013
#define METHOD_UNPACK_SEQUENCE_TOKEN		0x00000014
#define METHOD_UNPACK_SEQUENCEEX_TOKEN		0x00000015
#define METHOD_DELETENAME_TOKEN				0x00000016
#define METHOD_PYCELL_SET_TOKEN				0x00000017
#define METHOD_SET_CLOSURE					0x00000018
#define METHOD_BUILD_SLICE					0x00000019
#define METHOD_UNARY_POSITIVE				0x0000001A
#define METHOD_UNARY_NEGATIVE				0x0000001B
#define METHOD_UNARY_NOT					0x0000001C
#define METHOD_UNARY_INVERT					0x0000001D
#define METHOD_MATRIX_MULTIPLY_TOKEN		0x0000001E
#define METHOD_BINARY_LSHIFT_TOKEN			0x0000001F
#define METHOD_BINARY_RSHIFT_TOKEN			0x00000020
#define METHOD_BINARY_AND_TOKEN				0x00000021
#define METHOD_BINARY_XOR_TOKEN				0x00000022
#define METHOD_BINARY_OR_TOKEN				0x00000023
#define METHOD_LIST_APPEND_TOKEN			0x00000024
#define METHOD_SET_ADD_TOKEN				0x00000025
#define METHOD_INPLACE_POWER_TOKEN			0x00000026
#define METHOD_INPLACE_MULTIPLY_TOKEN		0x00000027
#define METHOD_INPLACE_MATRIX_MULTIPLY_TOKEN 0x00000028
#define METHOD_INPLACE_TRUE_DIVIDE_TOKEN	0x00000029
#define METHOD_INPLACE_FLOOR_DIVIDE_TOKEN	0x0000002A
#define METHOD_INPLACE_MODULO_TOKEN			0x0000002B
#define METHOD_INPLACE_ADD_TOKEN			0x0000002C
#define METHOD_INPLACE_SUBTRACT_TOKEN		0x0000002D
#define METHOD_INPLACE_LSHIFT_TOKEN			0x0000002E
#define METHOD_INPLACE_RSHIFT_TOKEN			0x0000002F
#define METHOD_INPLACE_AND_TOKEN			0x00000030
#define METHOD_INPLACE_XOR_TOKEN			0x00000031
#define METHOD_INPLACE_OR_TOKEN				0x00000032
#define METHOD_MAP_ADD_TOKEN				0x00000033
#define METHOD_PRINT_EXPR_TOKEN				0x00000034
#define METHOD_LOAD_CLASSDEREF_TOKEN		0x00000035
#define METHOD_PREPARE_EXCEPTION			0x00000036
#define METHOD_DO_RAISE						0x00000037
#define METHOD_EH_TRACE						0x00000038
#define METHOD_COMPARE_EXCEPTIONS			0x00000039
#define METHOD_UNBOUND_LOCAL				0x0000003A
#define METHOD_DEBUG_TRACE					0x0000003B
#define METHOD_FUNC_SET_DEFAULTS			0x0000003C
#define	METHOD_CALLNKW_TOKEN				0x0000003D
#define	METHOD_DEBUG_DUMP_FRAME				0x0000003E
#define METHOD_UNWIND_EH					0x0000003F
#define METHOD_PY_CHECKFUNCTIONRESULT		0x00000040
#define METHOD_PY_PUSHFRAME					0x00000041
#define METHOD_PY_POPFRAME					0x00000042
#define METHOD_PY_IMPORTNAME				0x00000043
#define METHOD_PY_FANCYCALL					0x00000044
#define METHOD_PY_IMPORTFROM				0x00000045
#define METHOD_PY_IMPORTSTAR				0x00000046
#define METHOD_PY_FUNC_SET_ANNOTATIONS		0x00000047
#define METHOD_PY_FUNC_SET_KW_DEFAULTS		0x00000048
#define METHOD_IS							0x00000049
#define METHOD_ISNOT						0x0000004A
#define METHOD_GETITER_OPTIMIZED_TOKEN		0x0000004B

// call helpers
#define METHOD_CALL0_TOKEN		0x00010000
#define METHOD_CALL1_TOKEN		0x00010001
#define METHOD_CALL2_TOKEN		0x00010002
#define METHOD_CALL3_TOKEN		0x00010003
#define METHOD_CALL4_TOKEN		0x00010004
#define METHOD_CALL5_TOKEN		0x00010005
#define METHOD_CALL6_TOKEN		0x00010006
#define METHOD_CALL7_TOKEN		0x00010007
#define METHOD_CALL8_TOKEN		0x00010008
#define METHOD_CALL9_TOKEN		0x00010009
#define METHOD_CALLN_TOKEN		0x00010100

// Py* helpers
#define METHOD_PYTUPLE_NEW			0x00020000
#define METHOD_PYLIST_NEW			0x00020001
#define METHOD_PYDICT_NEWPRESIZED	0x00020002
#define METHOD_PYSET_NEW			0x00020003
#define METHOD_PYSET_ADD			0x00020004
#define METHOD_PYOBJECT_ISTRUE		0x00020005
#define METHOD_PYITER_NEXT			0x00020006
#define METHOD_PYCELL_GET			0x00020007
#define METHOD_PYERR_RESTORE		0x00020008

// Misc helpers
#define METHOD_LOADGLOBAL_TOKEN		0x00030000
#define METHOD_LOADATTR_TOKEN		0x00030001
#define METHOD_STOREATTR_TOKEN		0x00030002
#define METHOD_DELETEATTR_TOKEN		0x00030003
#define METHOD_STOREGLOBAL_TOKEN	0x00030004
#define METHOD_DELETEGLOBAL_TOKEN	0x00030005

// signatures for calli methods
#define SIG_ITERNEXT_TOKEN			0x00040000
#define SIG_ITERNEXT_OPTIMIZED_TOKEN	0x00040001

extern "C" __declspec(dllexport) PVOID JitCompile(PyCodeObject* code);
extern "C" __declspec(dllexport) void JitInit();

#endif
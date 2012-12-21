/*
    Copyright (c) 2012 Matthew Stump

    This file is part of libcql.

    libcql is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    libcql is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __CQL_H_INCLUDED__
#define __CQL_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#if !defined WINCE
#include <errno.h>
#endif
#include <stddef.h>
#include <stdio.h>
#if defined _WIN32
#include <winsock2.h>
#endif

#if defined _WIN32
#   if defined DLL_EXPORT
#       define CQL_EXPORT __declspec(dllexport)
#   else
#       define CQL_EXPORT __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define CQL_EXPORT __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define CQL_EXPORT __attribute__ ((visibility("default")))
#   else
#       define CQL_EXPORT
#   endif
#endif

#define CQL_VERSION_MAJOR 0
#define CQL_VERSION_MINOR 0
#define CQL_VERSION_PATCH 1

#ifdef __cplusplus
}
#endif

#endif

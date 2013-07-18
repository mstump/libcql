/*
  Copyright (c) 2013 Matthew Stump

  This file is part of libcql.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef __CQL_C_H_INCLUDED__
#define __CQL_C_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#if !defined _WIN32_WCE
#include <errno.h>
#endif
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/*  Handle DSO symbol visibility */
#if defined _WIN32
#   if defined CQL_STATIC
#       define CQL_EXPORT
#   elif defined DLL_EXPORT
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

/******************************************************************************/
/*                                                                            */
/* CQL Context and Pool                                                       */
/*                                                                            */
/******************************************************************************/

CQL_EXPORT void*
cql_context_new();

CQL_EXPORT void
cql_context_free(
    void* context);

CQL_EXPORT bool
cql_init(
    void*  context,
    void** status);

CQL_EXPORT bool
cql_init_ssl(
    void*  context,
    void** status);

CQL_EXPORT void
cql_close(
    void*  context);

CQL_EXPORT void*
cql_add_client(
    void*          context,
    void*          host,
    size_t         host_size,
    unsigned short port);

/******************************************************************************/
/*                                                                            */
/* Futures                                                                    */
/*                                                                            */
/******************************************************************************/

CQL_EXPORT bool
cql_future_ready(
    void* context,
    void* future);

CQL_EXPORT void
cql_future_wait(
    void* context,
    void* future);

CQL_EXPORT bool
cql_future_success(
    void* context,
    void* future);

CQL_EXPORT int
cql_future_get_error_code(
    void* context,
    void* future);

CQL_EXPORT void
cql_future_get_error_message(
    void*  context,
    void*  future,
    char** message);

/******************************************************************************/
/*                                                                            */
/* Queries and Prepared Statements                                            */
/*                                                                            */
/******************************************************************************/

CQL_EXPORT void*
cql_query(
    void*  context,
    void*  query,
    size_t query_size);

CQL_EXPORT void*
cql_prepare(
    void*  context,
    void*  query,
    size_t query_size);

CQL_EXPORT void*
cql_execute_new(
    void*  context,
    void*  query,
    size_t query_size);

CQL_EXPORT void*
cql_execute(
    void*  context,
    void*  prepared,
    int    consistency);

CQL_EXPORT void
cql_execute_push_data(
    void*  context,
    void*  prepared,
    void*  data,
    size_t data_size);

CQL_EXPORT void
cql_execute_push_bool(
    void* context,
    void* prepared,
    bool  param);

CQL_EXPORT void
cql_execute_push_short(
    void* context,
    void* prepared,
    short param);

CQL_EXPORT void
cql_execute_push_int(
    void* context,
    void* prepared,
    int   param);

CQL_EXPORT void
cql_execute_push_float(
    void* context,
    void* prepared,
    float param);

CQL_EXPORT void
cql_execute_push_double(
    void* context,
    void* prepared,
    float param);

CQL_EXPORT void
cql_execute_push_bigint(
    void*   context,
    void*   prepared,
    int64_t param);



#undef CQL_EXPORT

#ifdef __cplusplus
}
#endif

#endif // __CQL_C_H_INCLUDED__

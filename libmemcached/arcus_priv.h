/*
 * arcus-c-client : Arcus C client
 * Copyright 2010-2014 NAVER Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __LIBMEMCACHED_ARCUS_PRIV_H__
#define __LIBMEMCACHED_ARCUS_PRIV_H__

#ifdef LIBMEMCACHED_WITH_ZK_INTEGRATION

#include <zookeeper.h>
#define ZOOKEEPER_C_CLIENT 1
#include <zookeeper_log.h>

#include "libmemcached/memcached_util.h"

#define ARCUS_MAX_PROXY_FILE_LENGTH 10240

struct arcus_proc_mutex
{
  int fd;
  int locked;
  char *fname;
};

typedef struct arcus_proxy_data_st
{
  struct arcus_proc_mutex mutex;
  volatile uint32_t version;
  size_t            size;
  char serverlist[ARCUS_MAX_PROXY_FILE_LENGTH - 512];
} arcus_proxy_data_st;

typedef struct arcus_st {
  /**
   * @note zookeeper configurations
   */
  struct arcus_zk_st
  {
    zhandle_t  *handle;
    clientid_t myid;
    char       ensemble_list[1024];
    char       svc_code[256];
    char       path[256];
    uint32_t   port;
    uint32_t   session_timeout;
    size_t     maxbytes;
    int        last_rc;
    struct String_vector last_strings;
    bool       is_1_7;
  } zk;

  struct arcus_proxy_st
  {
    char                name[256];
    uint32_t            current_version;
    arcus_proxy_data_st *data;
  } proxy;

  memcached_pool_st *pool;

  bool is_initializing;
  bool is_proxy;
} arcus_st;

/* 1.7 cluster:
 * groupname is the name of the group this server belongs to.
 * hostname is the name of the master server.  If there are no masters, then
 * hostname = NULL.
 */
struct memcached_server_info
{
  const char    *groupname;
  const char    *hostname;
  unsigned short port;
  bool           exist;
};

LIBMEMCACHED_API
void arcus_server_check_for_update(memcached_st *ptr);

#else /* LIBMEMCACHED_WITH_ZK_INTEGRATION */

static inline void arcus_server_check_for_update(memcached_st *)
{
  /* Nothing */
}
#endif

#endif /* __LIBMEMCACHED_ARCUS_PRIV_H__ */

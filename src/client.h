/*
  Copyright (C) 2007 Tomash Brechko.  All rights reserved.

  When used to build Perl module:

  This library is free software; you can redistribute it and/or modify
  it under the same terms as Perl itself, either Perl version 5.8.8
  or, at your option, any later version of Perl 5 you may have
  available.

  When used as a standalone library:

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

#ifndef CLIENT_H
#define CLIENT_H 1

#include <stddef.h>


struct client;


enum server_status {
  MEMCACHED_SUCCESS,
  MEMCACHED_FAILURE,
  MEMCACHED_EAGAIN,
  MEMCACHED_ERROR,
  MEMCACHED_UNKNOWN,
  MEMCACHED_CLOSED
};

enum set_cmd_e { CMD_SET, CMD_ADD, CMD_REPLACE, CMD_APPEND, CMD_PREPEND };

enum get_cmd_e { CMD_GET, CMD_GETS };

enum arith_cmd_e { CMD_INCR, CMD_DECR };


typedef unsigned int flags_type;
#define FMT_FLAGS "%u"

typedef int exptime_type;
#define FMT_EXPTIME "%d"

typedef unsigned int delay_type;
#define FMT_DELAY "%u"

typedef size_t value_size_type;
#define FMT_VALUE_SIZE "%zu"

typedef unsigned long long cas_type;
#define FMT_CAS "%llu"

typedef unsigned long long arith_type;
#define FMT_ARITH "%llu"


typedef char *(*get_key_func)(void *arg, int key_index, size_t *key_len);

typedef void *(*alloc_value_func)(void *arg, value_size_type value_size);
typedef void (*store_value_func)(void *arg, int key_index, flags_type flags,
                                 int use_cas, cas_type cas);
typedef void (*free_value_func)(void *arg);

struct value_object
{
  alloc_value_func alloc;
  store_value_func store;
  free_value_func free;

  void *arg;
};


extern
struct client *
client_init();

extern
void
client_destroy(struct client *c);

/*
  client_set_ketama_points() should be called before adding any server.
*/
extern
int
client_set_ketama_points(struct client *c, int ketama_points);

extern
int
client_add_server(struct client *c, const char *host, size_t host_len,
                  const char *port, size_t port_len, double weight);

extern
int
client_set_prefix(struct client *c, const char *ns, size_t ns_len);

extern
void
client_set_connect_timeout(struct client *c, int to);

extern
void
client_set_io_timeout(struct client *c, int to);

extern
void
client_set_max_failures(struct client *c, int f);

extern
void
client_set_failure_timeout(struct client *c, int to);

extern
void
client_set_close_on_error(struct client *c, int enable);

extern
void
client_set_noreply(struct client *c, int enable);

extern
int
client_set(struct client *c, enum set_cmd_e cmd,
           const char *key, size_t key_len,
           flags_type flags, exptime_type exptime,
           const void *value, value_size_type value_size, int noreply);

extern
int
client_cas(struct client *c, const char *key, size_t key_len,
           cas_type cas, flags_type flags, exptime_type exptime,
           const void *value, value_size_type value_size, int noreply);

extern
int
client_get(struct client *c, enum get_cmd_e cmd,
           const char *key, size_t key_len, struct value_object *o);

extern
int
client_mget(struct client *c, enum get_cmd_e cmd,
            int key_count, get_key_func get_key, struct value_object *o);

extern
int
client_arith(struct client *c, enum arith_cmd_e cmd,
             const char *key, size_t key_len,
             arith_type arg, arith_type *result, int noreply);

extern
int
client_delete(struct client *c, const char *key, size_t key_len,
              delay_type delay, int noreply);

extern
int
client_flush_all(struct client *c, delay_type delay, int noreply);


#endif /* ! CLIENT_H */
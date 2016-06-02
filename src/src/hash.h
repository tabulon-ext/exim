/*
 *  Exim - an Internet mail transport agent
 *
 *  Copyright (C) 2016  Exim maintainers
 *
 *  Hash interface functions
 */

#include "exim.h"

#if !defined(HASH_H)	/* entire file */
#define HASH_H

#include "sha_ver.h"
#include "blob.h"

#ifdef SHA_OPENSSL
# include <openssl/sha.h>
#elif defined SHA_GNUTLS
# include <gnutls/crypto.h>
#elif defined(SHA_GCRYPT)
# include <gcrypt.h>
#elif defined(SHA_POLARSSL)
# include "pdkim/pdkim.h"		/*XXX ugly */
# include "pdkim/polarssl/sha1.h"
# include "pdkim/polarssl/sha2.h"
#endif


/* Hash context for the exim_sha_* routines */

typedef struct {
  BOOL is_sha1;
  int hashlen;

#ifdef SHA_OPENSSL
  union {
    SHA_CTX      sha1;       /* SHA1 block                                */
    SHA256_CTX   sha2;       /* SHA256 block                              */
  } u;

#elif defined(SHA_GNUTLS)
  gnutls_hash_hd_t sha;      /* Either SHA1 or SHA256 block               */

#elif defined(SHA_GCRYPT)
  gcry_md_hd_t sha;          /* Either SHA1 or SHA256 block               */

#elif defined(SHA_POLARSSL)
  union {
    sha1_context sha1;       /* SHA1 block                                */
    sha2_context sha2;       /* SHA256 block                              */
  } u;

#elif defined(SHA_NATIVE)
  sha1 sha1;
#endif

} hctx;

extern void     exim_sha_init(hctx *, BOOL);
extern void     exim_sha_update(hctx *, const uschar *a, int);
extern void     exim_sha_finish(hctx *, blob *);
extern int      exim_sha_hashlen(hctx *);

#endif
/* End of File */

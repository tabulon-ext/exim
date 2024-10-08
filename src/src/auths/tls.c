/*************************************************
*     Exim - an Internet mail transport agent    *
*************************************************/

/* Copyright (c) The Exim Maintainers 2024 */
/* Copyright (c) Jeremy Harris 1995 - 2020 */
/* See the file NOTICE for conditions of use and distribution. */
/* SPDX-License-Identifier: GPL-2.0-or-later */

/* This file provides an Exim authenticator driver for
a server to verify a client SSL certificate
*/


#include "../exim.h"

#ifdef AUTH_TLS		/* Remainder of file */
#include "tls.h"

/* Options specific to the tls authentication mechanism. */

optionlist auth_tls_options[] = {
  { "server_param",     opt_stringptr,
      OPT_OFF(auth_tls_options_block, server_param1) },
  { "server_param1",    opt_stringptr,
      OPT_OFF(auth_tls_options_block, server_param1) },
  { "server_param2",    opt_stringptr,
      OPT_OFF(auth_tls_options_block, server_param2) },
  { "server_param3",    opt_stringptr,
      OPT_OFF(auth_tls_options_block, server_param3) },
};

/* Size of the options list. An extern variable has to be used so that its
address can appear in the tables drtables.c. */

int auth_tls_options_count = nelem(auth_tls_options);

/* Default private options block for the authentication method. */

auth_tls_options_block auth_tls_option_defaults = {
    NULL,	/* server_param1 */
    NULL,	/* server_param2 */
    NULL,	/* server_param3 */
};


#ifdef MACRO_PREDEF

/* Dummy values */
void auth_tls_init(driver_instance *ablock) {}
int auth_tls_server(auth_instance *ablock, uschar *data) {return 0;}
int auth_tls_client(auth_instance *ablock, void * sx,
  int timeout, uschar *buffer, int buffsize) {return 0;}

#else   /*!MACRO_PREDEF*/




/*************************************************
*          Initialization entry point            *
*************************************************/

/* Called for each instance, after its options have been read, to
enable consistency checks to be done, or anything else that needs
to be set up. */

void
auth_tls_init(driver_instance * a)
{
auth_instance * ablock = (auth_instance *)a;
ablock->public_name = a->name;	/* needed for core code */
}



/*************************************************
*             Server entry point                 *
*************************************************/

/* For interface, see auths/README */

int
auth_tls_server(auth_instance *ablock, uschar *data)
{
auth_tls_options_block * ob = ablock->drinst.options_block;

if (ob->server_param1)
  auth_vars[expand_nmax++] = expand_string(ob->server_param1);
if (ob->server_param2)
  auth_vars[expand_nmax++] = expand_string(ob->server_param2);
if (ob->server_param3)
  auth_vars[expand_nmax++] = expand_string(ob->server_param3);
return auth_check_serv_cond(ablock);
}


# ifdef DYNLOOKUP
#  define tls_auth_info _auth_info
# endif

auth_info tls_auth_info = {
.drinfo = {
  .driver_name =	US"tls",                   /* lookup name */
  .options =		auth_tls_options,
  .options_count =	&auth_tls_options_count,
  .options_block =	&auth_tls_option_defaults,
  .options_len =	sizeof(auth_tls_options_block),
  .init =		auth_tls_init,
# ifdef DYNLOOKUP
  .dyn_magic =		AUTH_MAGIC,
# endif
  },
.servercode =		auth_tls_server,
.clientcode =		NULL,
.version_report =	NULL,
.macros_create =	NULL,
};

#endif	/*!MACRO_PREDEF*/
#endif	/*AUTH_TLS*/
/* End of tls.c */

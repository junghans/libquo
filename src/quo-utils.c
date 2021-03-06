/*
 * Copyright (c) 2013-2016 Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This software was produced under U.S. Government contract DE-AC52-06NA25396
 * for Los Alamos National Laboratory (LANL), which is operated by Los Alamos
 * National Security, LLC for the U.S. Department of Energy. The U.S. Government
 * has rights to use, reproduce, and distribute this software.  NEITHER THE
 * GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS
 * OR IMPLIED, OR ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If
 * software is modified to produce derivative works, such modified software
 * should be clearly marked, so as not to confuse it with the version available
 * from LANL.
 *
 * Additionally, redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following conditions
 * are met:
 *
 * · Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * · Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * · Neither the name of Los Alamos National Security, LLC, Los Alamos
 *   National Laboratory, LANL, the U.S. Government, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL
 * SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file quo-utils.c
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "quo-utils.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif
#include <errno.h>

#define QUO_TMPDIR_ENV_VAR_STR "QUO_TMPDIR"

/* ////////////////////////////////////////////////////////////////////////// */
int
quo_utils_whoami(char **whoami)
{
    char *me = NULL;

    if (!whoami) return QUO_ERR_INVLD_ARG;
    if (NULL == (me = getenv("USER"))) {
        me = "JuanPerez";
    }
    if (-1 == asprintf(whoami, "%s", me)) return QUO_ERR_OOR;
    return QUO_SUCCESS;
}

/* ////////////////////////////////////////////////////////////////////////// */
int
quo_utils_tmpdir(char **tmpdir)
{
    char *dir = NULL;

    if (!tmpdir) return QUO_ERR_INVLD_ARG;
    if (NULL == (dir = getenv(QUO_TMPDIR_ENV_VAR_STR))) {
        dir = "/tmp";
    }
    if (-1 == asprintf(tmpdir, "%s", dir)) return QUO_ERR_OOR;
    return QUO_SUCCESS;
}

/* ////////////////////////////////////////////////////////////////////////// */
int
quo_utils_path_usable(const char *path, bool *usable, int *errc)
{
    if (!path || !usable || !errc) return QUO_ERR_INVLD_ARG;
    *errc = 0; *usable = true;
    if (-1 == access(path, R_OK | W_OK)) {
        *errc = errno;
        *usable = false;
    }
    return QUO_SUCCESS;
}

/* ////////////////////////////////////////////////////////////////////////// */
int
quo_utils_envvar_set(const char *the_envvar,
                     bool *set)
{
    if (!the_envvar || !set) return QUO_ERR_INVLD_ARG;

    *set = (NULL != getenv(the_envvar));

    return QUO_SUCCESS;
}

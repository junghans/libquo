/*
 * Copyright (c) 2017      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the libquo project. See the LICENSE file at the
 * top-level directory of this distribution.
 */

/*
 * Parallel library that calculates z(s) for any s > 1.0, where z is the
 * Euler–Riemann zeta function.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>

#include "mpi.h"

enum {
    ZETA_SUCCESS = 0,
    ZETA_FAILURE,
    ZETA_INVALID_ARG,
    ZETA_OOR
};

static int
zetai(double s,
      int64_t low,
      int64_t high,
      double *restrict z)
{
    for (int64_t zi = 0, i = low; i < high; ++i, ++zi) {
        z[zi] = 1.0 / powl(i, s);
    }

    return ZETA_SUCCESS;
}

static double
sigma_zeta(int64_t n,
           double *restrict z)
{
    double result = 0.0;
    for (int64_t i = 0; i < n; ++i) {
        result += z[i];
    }
    return result;
}

int
zeta(double s,
     int64_t n,
     double **z,
     MPI_Comm comm,
     double *result)
{
    if (s <= 1.0) return ZETA_INVALID_ARG;

    int cid = 0, csize = 0;
    if (MPI_SUCCESS != MPI_Comm_rank(comm, &cid))   return ZETA_FAILURE;
    if (MPI_SUCCESS != MPI_Comm_size(comm, &csize)) return ZETA_FAILURE;

    /* My bounds. */
    int64_t low  = 1 + (cid * n);
    int64_t high = (cid + 1) * n;
    /* My array of zeta(i). */
    double *ztmp = calloc(n, sizeof(ztmp));
    if (!ztmp) return ZETA_OOR;
    /* Fill in local z(i)s. */
    int rc = zetai(s, low, high, ztmp);
    if (ZETA_SUCCESS != rc) return rc;
    /* Calculate local sum */
    double partial_sum = sigma_zeta(n, ztmp);
    /* Calculate global sum */
    if (MPI_SUCCESS != MPI_Reduce(&partial_sum, result,
                                  1, MPI_DOUBLE, MPI_SUM,
                                  0, comm)) {
        rc = ZETA_FAILURE;
        goto out;
    }
out:
    if (ZETA_SUCCESS != rc) free(ztmp);
    else *z = ztmp;
    return rc;
}

int
main(int argc, char **argv)
{
    if (MPI_SUCCESS != MPI_Init(&argc, &argv)) return EXIT_FAILURE;
    int cid = 0;
    if (MPI_SUCCESS != MPI_Comm_rank(MPI_COMM_WORLD, &cid)) return EXIT_FAILURE;

    const int64_t n = 1000000;
    const double s = 2.0;
    double *z = NULL;
    double res = 0.0;

    int zrc = zeta(s, n, &z, MPI_COMM_WORLD, &res);
    if (ZETA_SUCCESS != zrc) goto out;

    const bool emit = (0 == cid);
    if (emit) printf("z(%lf) = %.16lf\n", s, res);

out:
    if (MPI_SUCCESS != MPI_Finalize()) zrc = ZETA_FAILURE;
    return (ZETA_SUCCESS == zrc ? EXIT_SUCCESS : EXIT_FAILURE);
}

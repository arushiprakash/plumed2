/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
These files are semi-automatic translations by f2c from the original netlib LAPACK library.
The source has been modified to (mostly) use modern C formatting, and to get rid of
compiler warnings. Any errors in doing this should be blamed on the Gromacs developers, and
not the reference LAPACK implementation.

The reference LAPACK implementation is available from http://www.netlib.org/lapack 

LAPACK does not come with a formal named "license", but a general statement saying:

"The reference LAPACK is a freely-available software package. It is available from netlib
via anonymous ftp and the World Wide Web. Thus, it can be included in commercial software
packages (and has been). We only ask that proper credit be given to the authors."

While the rest of Gromacs is LGPL, we think it's only fair to give you the same rights to
our modified LAPACK files as the original netlib versions, so do what you want with them.

However, be warned that we have only tested that they to the right thing in the cases used
in Gromacs (primarily full & sparse matrix diagonalization), so in most cases it is a much
better idea to use the full reference implementation.

Erik Lindahl, 2008-10-07.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2012,2013, by the GROMACS development team, led by
 * David van der Spoel, Berk Hess, Erik Lindahl, and including many
 * others, as listed in the AUTHORS file in the top-level source
 * directory and at http://www.gromacs.org.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * http://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at http://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out http://www.gromacs.org.
 */
#include <math.h>

#include "simple.h"

#include "lapack.h"
#include "lapack_limits.h"

#include "blas/blas.h"
namespace PLMD{
namespace lapack{
using namespace blas;
void
PLUMED_BLAS_F77_FUNC(slarrbx,SLARRBX)(int *n, 
	 float *d__, 
	 float *l, 
	 float *ld, 
	 float *lld, 
	 int *ifirst, 
	 int *ilast, 
	 float *rtol1, 
	 float *rtol2, 
	 int *offset, 
	 float *w, 
	 float *wgap, 
	 float *werr, 
	 float *work,
	 int *iwork, 
	 int *info)
{
    int i__1, i__2, i__3;
    float d__1, d__2;

    int i__, j, k, p;
    float s;
    int i1, i2, ii, kk;
    float fac, gap, mid;
    int cnt;
    float eps, tmp, left;
    int nint, prev, next, nleft;
    float right, width, dplus, error;
    int nright, olnint;
    k = 0;
    right = 0.0;

    --iwork;
    --work;
    --werr;
    --wgap;
    --w;
    --lld;
    --ld;
    --l;
    --d__;

    *info = 0;
    eps = PLUMED_GMX_FLOAT_EPS;
    i__1 = *n << 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iwork[i__] = 0;
    }
    i1 = *ifirst;
    i2 = *ifirst;
    prev = 0;
    i__1 = *ilast;
    for (i__ = *ifirst; i__ <= i__1; ++i__) {
	ii = i__ - *offset;
	if (i__ == *ifirst) {
	    gap = wgap[ii];
	} else if (i__ == *ilast) {
	    gap = wgap[ii - 1];
	} else {
	    d__1 = wgap[ii - 1], d__2 = wgap[ii];
	    gap = (d__1<d__2) ? d__1 : d__2;
	}
	error = werr[ii];
	k = i__ << 1;
	iwork[k - 1] = 1;
	i2 = i__;
    }

    i__ = i1;
    nint = 0;
L30:
    if (i__ <= i2) {
	ii = i__ - *offset;
	if (iwork[(i__ << 1) - 1] == 1) {
	    fac = 1.;
	    left = w[ii] - werr[ii];


L40:
	    if (i__ > i1 && left <= right) {
		left = right;
		cnt = i__ - 1;
	    } else {
		s = -left;
		cnt = 0;
		i__1 = *n - 1;
		for (j = 1; j <= i__1; ++j) {
		    dplus = d__[j] + s;
		    s = s * lld[j] / dplus - left;
		    if (dplus < 0.) {
			++cnt;
		    }
		}
		dplus = d__[*n] + s;
		if (dplus < 0.) {
		    ++cnt;
		}
		if (! (s > 0. || s < 1.)) {

		    cnt = 0;
		    s = -left;
		    i__1 = *n - 1;
		    for (j = 1; j <= i__1; ++j) {
			dplus = d__[j] + s;
			if (dplus < 0.) {
			    ++cnt;
			}
			tmp = lld[j] / dplus;
			if (fabs(tmp)<PLUMED_GMX_FLOAT_MIN) {
			    s = lld[j] - left;
			} else {
			    s = s * tmp - left;
			}
		    }
		    dplus = d__[*n] + s;
		    if (dplus < 0.) {
			++cnt;
		    }
		}
		if (cnt > i__ - 1) {
		    left -= werr[ii] * fac;
		    fac *= 2.;
		    goto L40;
		}
	    }
	    nleft = cnt + 1;
	    i1 = (i1<nleft) ? i1 : nleft;
	    fac = 1.;
	    right = w[ii] + werr[ii];
L60:
	    s = -right;
	    cnt = 0;
	    i__1 = *n - 1;
	    for (j = 1; j <= i__1; ++j) {
		dplus = d__[j] + s;
		s = s * lld[j] / dplus - right;
		if (dplus < 0.) {
		    ++cnt;
		}
	    }
	    dplus = d__[*n] + s;
	    if (dplus < 0.) {
		++cnt;
	    }
	    if (! (s > 0. || s < 1.)) {

		cnt = 0;
		s = -right;
		i__1 = *n - 1;
		for (j = 1; j <= i__1; ++j) {
		    dplus = d__[j] + s;
		    if (dplus < 0.) {
			++cnt;
		    }
		    tmp = lld[j] / dplus;
		    if (fabs(tmp)<PLUMED_GMX_FLOAT_MIN) {
			s = lld[j] - right;
		    } else {
			s = s * tmp - right;
		    }
		}
		dplus = d__[*n] + s;
		if (dplus < 0.) {
		    ++cnt;
		}
	    }
	    if (cnt < i__) {
		right += werr[ii] * fac;
		fac *= 2.;
		goto L60;
	    }
	    cnt = (cnt<i2) ? cnt : i2;
	    ++nint;
	    k = nleft << 1;
	    work[k - 1] = left;
	    work[k] = right;
	    i__ = cnt + 1;
	    iwork[k - 1] = i__;
	    iwork[k] = cnt;
	    if (prev != nleft - 1) {
		work[k - 2] = left;
	    }
	    prev = nleft;
	} else {
	    right = work[i__ * 2];

	    ++iwork[k - 1];
	    prev = i__;
	    ++i__;
	}
	goto L30;
    }
    if (i__ <= *n && iwork[(i__ << 1) - 1] != -1) {
	work[(i__ << 1) - 1] = work[prev * 2];
    }

L80:
    prev = i1 - 1;
    olnint = nint;
    i__ = i1;
    i__1 = olnint;
    for (p = 1; p <= i__1; ++p) {
	k = i__ << 1;
	left = work[k - 1];
	right = work[k];
	next = iwork[k - 1];
	nright = iwork[k];
	mid = (left + right) * .5;
	width = right - mid;
	d__1 = fabs(left);
	d__2 = fabs(right);
	tmp = (d__1>d__2) ? d__1 : d__2;

	gap = 0.;
	if (i__ == nright) {
	    if (prev > 0 && next <= *n) {
		d__1 = left - work[k - 2], d__2 = work[k + 1] - right;
		gap = (d__1<d__2) ? d__1 : d__2;
	    } else if (prev > 0) {
		gap = left - work[k - 2];
	    } else if (next <= *n) {
		gap = work[k + 1] - right;
	    }
	}
	d__1 = *rtol1 * gap, d__2 = *rtol2 * tmp;
	if (width < ((d__1>d__2) ? d__1 : d__2)) {
	    --nint;
	    iwork[k - 1] = 0;
	    kk = k;
	    i__2 = nright;
	    for (j = i__ + 1; j <= i__2; ++j) {
		kk += 2;
		iwork[kk - 1] = 0;
		work[kk - 1] = left;
		work[kk] = right;
		wgap[j - 1 - *offset] = 0.;
	    }
	    if (i1 == i__) {
		i1 = next;
	    } else {
		iwork[(prev << 1) - 1] = next;
	    }
	    i__ = next;
	    continue;
	}
	prev = i__;

	s = -mid;
	cnt = 0;
	i__2 = *n - 1;
	for (j = 1; j <= i__2; ++j) {
	    dplus = d__[j] + s;
	    s = s * lld[j] / dplus - mid;
	    if (dplus < 0.) {
		++cnt;
	    }
	}
	dplus = d__[*n] + s;
	if (dplus < 0.) {
	    ++cnt;
	}
	if (! (s > 0. || s < 1.)) {
	    cnt = 0;
	    s = -mid;
	    i__2 = *n - 1;
	    for (j = 1; j <= i__2; ++j) {
		dplus = d__[j] + s;
		if (dplus < 0.) {
		    ++cnt;
		}
		tmp = lld[j] / dplus;
		if (fabs(tmp)<PLUMED_GMX_FLOAT_MIN) {
		    s = lld[j] - mid;
		} else {
		    s = s * tmp - mid;
		}
	    }
	    dplus = d__[*n] + s;
	    if (dplus < 0.) {
		++cnt;
	    }
	}
	i__2 = i__ - 1, i__3 = (nright<cnt) ? nright : cnt;
	cnt = (i__2>i__3) ? i__2 : i__3;
	if (cnt == i__ - 1) {
	    work[k - 1] = mid;
	} else if (cnt == nright) {
	    work[k] = mid;
	} else {
	    iwork[k] = cnt;
	    ++cnt;
	    iwork[k - 1] = cnt;
	    kk = cnt << 1;
	    iwork[kk - 1] = next;
	    iwork[kk] = nright;
	    work[k] = mid;
	    work[kk - 1] = mid;
	    work[kk] = right;
	    prev = cnt;
	    if (cnt - 1 > i__) {
		work[kk - 2] = mid;
	    }
	    if (cnt > *ifirst && cnt <= *ilast) {
		++nint;
	    } else if (cnt <= *ifirst) {
		i1 = cnt;
	    }
	}
	i__ = next;
    }
    if (nint > 0) {
	goto L80;
    }
    i__1 = *ilast;
    for (i__ = *ifirst; i__ <= i__1; ++i__) {
	k = i__ << 1;
	ii = i__ - *offset;
	if (iwork[k - 1] != -1) {
	    w[ii] = (work[k - 1] + work[k]) * .5;
	    werr[ii] = work[k] - w[ii];
	    if (i__ != *ilast) {
		wgap[ii] = work[k + 1] - work[k];
	    }
	}
    }

    return;

} 
}
}

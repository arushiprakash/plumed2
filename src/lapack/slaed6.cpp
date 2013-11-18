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

#include "lapack.h"

#include "simple.h"

#include "blas/blas.h"
namespace PLMD{
namespace lapack{
using namespace blas;
void
PLUMED_BLAS_F77_FUNC(slaed6,SLAED6)(int *kniter, 
                        int *orgati, 
                        float *rho, 
                        float *d__,
                        float *z__, 
                        float *finit, 
                        float *tau, 
                        int *info)
{
    int i__1;
    float r__1, r__2, r__3, r__4;

    float a, b, c__, f;
    int i__;
    float fc, df, ddf, eta, eps, base;
    int iter;
    float temp, temp1, temp2, temp3, temp4;
    int scale;
    int niter;
    float small1, small2, sminv1, sminv2, dscale[3], sclfac;
    float zscale[3], erretm;
    float safemin;
    float sclinv = 0;
    
    --z__;
    --d__;

    *info = 0;

    niter = 1;
    *tau = 0.f;
    if (*kniter == 2) {
	if (*orgati) {
	    temp = (d__[3] - d__[2]) / 2.f;
	    c__ = *rho + z__[1] / (d__[1] - d__[2] - temp);
	    a = c__ * (d__[2] + d__[3]) + z__[2] + z__[3];
	    b = c__ * d__[2] * d__[3] + z__[2] * d__[3] + z__[3] * d__[2];
	} else {
	    temp = (d__[1] - d__[2]) / 2.f;
	    c__ = *rho + z__[3] / (d__[3] - d__[2] - temp);
	    a = c__ * (d__[1] + d__[2]) + z__[1] + z__[2];
	    b = c__ * d__[1] * d__[2] + z__[1] * d__[2] + z__[2] * d__[1];
	}
        r__1 = fabs(a), r__2 = fabs(b), r__1 = ((r__1>r__2)? r__1:r__2), r__2 = fabs(c__);
        temp = (r__1>r__2) ? r__1 : r__2;
	a /= temp;
	b /= temp;
	c__ /= temp;
	if (c__ == 0.f) {
	    *tau = b / a;
	} else if (a <= 0.f) {
	    *tau = (a - sqrt((r__1 = a * a - b * 4.f * c__, fabs(r__1)))) / (
		    c__ * 2.f);
	} else {
	    *tau = b * 2.f / (a + sqrt((r__1 = a * a - b * 4.f * c__, fabs(r__1))));
	}

	temp = *rho + z__[1] / (d__[1] - *tau) + z__[2] / (d__[2] - *tau) + 
		z__[3] / (d__[3] - *tau);
	if (fabs(*finit) <= fabs(temp)) {
	    *tau = 0.f;
	}
    }

    eps = PLUMED_GMX_FLOAT_EPS;
    base = 2;
    safemin = PLUMED_GMX_FLOAT_MIN*(1.0+PLUMED_GMX_FLOAT_EPS);
    i__1 = (int) (log(safemin) / log(base) / 3.f);
    small1 = pow(base, i__1);
    sminv1 = 1.f / small1;
    small2 = small1 * small1;
    sminv2 = sminv1 * sminv1;

    if (*orgati) {
	r__3 = (r__1 = d__[2] - *tau, fabs(r__1)), r__4 = (r__2 = d__[3] - *
		tau, fabs(r__2));
        temp = (r__3<r__4) ? r__3 : r__4;
    } else {
	r__3 = (r__1 = d__[1] - *tau, fabs(r__1)), r__4 = (r__2 = d__[2] - *
		tau, fabs(r__2));
	temp = (r__3<r__4) ? r__3 : r__4;
    }
    scale = 0;
    if (temp <= small1) {
	scale = 1;
	if (temp <= small2) {

	    sclfac = sminv2;
	    sclinv = small2;
	} else {

	    sclfac = sminv1;
	    sclinv = small1;

	}

	for (i__ = 1; i__ <= 3; ++i__) {
	    dscale[i__ - 1] = d__[i__] * sclfac;
	    zscale[i__ - 1] = z__[i__] * sclfac;
	}
	*tau *= sclfac;
    } else {

	for (i__ = 1; i__ <= 3; ++i__) {
	    dscale[i__ - 1] = d__[i__];
	    zscale[i__ - 1] = z__[i__];
	}
    }
    fc = 0.f;
    df = 0.f;
    ddf = 0.f;
    for (i__ = 1; i__ <= 3; ++i__) {
	temp = 1.f / (dscale[i__ - 1] - *tau);
	temp1 = zscale[i__ - 1] * temp;
	temp2 = temp1 * temp;
	temp3 = temp2 * temp;
	fc += temp1 / dscale[i__ - 1];
	df += temp2;
	ddf += temp3;
    }
    f = *finit + *tau * fc;

    if (fabs(f) <= 0.f) {
	goto L60;
    }
    iter = niter + 1;
    for (niter = iter; niter <= 20; ++niter) {
	if (*orgati) {
	    temp1 = dscale[1] - *tau;
	    temp2 = dscale[2] - *tau;
	} else {
	    temp1 = dscale[0] - *tau;
	    temp2 = dscale[1] - *tau;
	}
	a = (temp1 + temp2) * f - temp1 * temp2 * df;
	b = temp1 * temp2 * f;
	c__ = f - (temp1 + temp2) * df + temp1 * temp2 * ddf;
	r__1 = fabs(a), r__2 = fabs(b), r__1 = ((r__1>r__2)? r__1:r__2), r__2 = fabs(c__);
	temp = (r__1>r__2) ? r__1 : r__2;
	a /= temp;
	b /= temp;
	c__ /= temp;
	if (c__ == 0.f) {
	    eta = b / a;
	} else if (a <= 0.f) {
	    eta = (a - sqrt((r__1 = a * a - b * 4.f * c__, fabs(r__1)))) / ( c__ * 2.f);
	} else {
	    eta = b * 2.f / (a + sqrt((r__1 = a * a - b * 4.f * c__, fabs( r__1))));
	}
	if (f * eta >= 0.f) {
	    eta = -f / df;
	}
	temp = eta + *tau;
	if (*orgati) {
	    if (eta > 0.f && temp >= dscale[2]) {
		eta = (dscale[2] - *tau) / 2.f;
	    }

	    if (eta < 0.f && temp <= dscale[1]) {
		eta = (dscale[1] - *tau) / 2.f;
	    }
	} else {
	    if (eta > 0.f && temp >= dscale[1]) {
		eta = (dscale[1] - *tau) / 2.f;
	    }
	    if (eta < 0.f && temp <= dscale[0]) {
		eta = (dscale[0] - *tau) / 2.f;
	    }
	}
	*tau += eta;
	fc = 0.f;
	erretm = 0.f;
	df = 0.f;
	ddf = 0.f;
	for (i__ = 1; i__ <= 3; ++i__) {
	    temp = 1.f / (dscale[i__ - 1] - *tau);
	    temp1 = zscale[i__ - 1] * temp;
	    temp2 = temp1 * temp;
	    temp3 = temp2 * temp;
	    temp4 = temp1 / dscale[i__ - 1];
	    fc += temp4;
	    erretm += fabs(temp4);
	    df += temp2;
	    ddf += temp3;
	}
	f = *finit + *tau * fc;
	erretm = (fabs(*finit) + fabs(*tau) * erretm) * 8.f + fabs(*tau) * df;
	if (fabs(f) <= eps * erretm) {
	    goto L60;
	}
    }
    *info = 1;
L60:
    if (scale) {
	*tau *= sclinv;
    }
    return;
} 


}
}

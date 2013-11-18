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
#include "blas/blas.h"
#include "lapack.h"


#include "blas/blas.h"
namespace PLMD{
namespace lapack{
using namespace blas;
void 
PLUMED_BLAS_F77_FUNC(dlabrd,DLABRD)(int *m, 
	int *n, 
	int *nb,
	double *a, 
	int *lda, 
	double *d__,
	double *e,
	double *tauq, 
	double *taup,
	double *x,
	int *ldx,
	double *y,
	int *ldy)
{
    int a_dim1, a_offset, x_dim1, x_offset, y_dim1, y_offset;
    int i__1, i__2, i__3;
    double one = 1.0;
    double minusone = -1.0;
    double zero = 0.0;
    int c__1 = 1;
    int i__;

    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --d__;
    --e;
    --tauq;
    --taup;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    y_dim1 = *ldy;
    y_offset = 1 + y_dim1;
    y -= y_offset;

    if (*m <= 0 || *n <= 0) {
	return;
    }

    if (*m >= *n) {

	i__1 = *nb;
	for (i__ = 1; i__ <= i__1; ++i__) {

	    i__2 = *m - i__ + 1;
	    i__3 = i__ - 1;
	    PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &minusone, &a[i__ + a_dim1], lda,
		     &y[i__ + y_dim1], ldy, &one, &a[i__ + i__ * a_dim1], &c__1);
	    i__2 = *m - i__ + 1;
	    i__3 = i__ - 1;
	    PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &minusone, &x[i__ + x_dim1], ldx,
		   &a[i__*a_dim1+1],&c__1,&one,&a[i__+i__*a_dim1],&c__1);

	    i__2 = *m - i__ + 1;
	    i__3 = i__ + 1;
	    if(*m<i__3)
	      i__3 = *m;
	    PLUMED_BLAS_F77_FUNC(dlarfg,DLARFG)(&i__2, &a[i__ + i__ * a_dim1], &a[i__3 + i__ * a_dim1], 
		    &c__1, &tauq[i__]);
	    d__[i__] = a[i__ + i__ * a_dim1];
	    if (i__ < *n) {
		a[i__ + i__ * a_dim1] = 1.;

		i__2 = *m - i__ + 1;
		i__3 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__3, &one, &a[i__ + (i__ + 1) * 
			a_dim1], lda, &a[i__ + i__ * a_dim1], &c__1, &zero, &
			y[i__ + 1 + i__ * y_dim1], &c__1);
		i__2 = *m - i__ + 1;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__3, &one, &a[i__ + a_dim1], 
			lda, &a[i__ + i__ * a_dim1], &c__1, &zero, &y[i__ * 
			y_dim1 + 1], &c__1);
		i__2 = *n - i__;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &minusone, &y[i__ + 1 + 
			y_dim1], ldy, &y[i__ * y_dim1 + 1], &c__1, &one, &y[
			i__ + 1 + i__ * y_dim1], &c__1);
		i__2 = *m - i__ + 1;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__3, &one, &x[i__ + x_dim1], 
			ldx, &a[i__ + i__ * a_dim1], &c__1, &zero, &y[i__ * 
			y_dim1 + 1], &c__1);
		i__2 = i__ - 1;
		i__3 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__3, &minusone, &a[(i__ + 1) * 
			a_dim1 + 1], lda, &y[i__ * y_dim1 + 1], &c__1, &one, 
			&y[i__ + 1 + i__ * y_dim1], &c__1);
		i__2 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dscal,DSCAL)(&i__2, &tauq[i__], &y[i__ + 1 + i__ * y_dim1], &c__1);

		i__2 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__, &minusone, &y[i__ + 1 + 
			y_dim1], ldy, &a[i__ + a_dim1], lda, &one, &a[i__ + (
			i__ + 1) * a_dim1], lda);
		i__2 = i__ - 1;
		i__3 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__3, &minusone, &a[(i__ + 1) * 
			a_dim1 + 1], lda, &x[i__ + x_dim1], ldx, &one, &a[
			i__ + (i__ + 1) * a_dim1], lda);

		i__2 = *n - i__;
		i__3 = i__ + 2;
		if(*n<i__3)
		  i__3 = *n;
		PLUMED_BLAS_F77_FUNC(dlarfg,DLARFG)(&i__2, &a[i__ + (i__ + 1) * a_dim1], 
			&a[i__ + i__3 * a_dim1], lda, &taup[i__]);
		e[i__] = a[i__ + (i__ + 1) * a_dim1];
		a[i__ + (i__ + 1) * a_dim1] = 1.;

		i__2 = *m - i__;
		i__3 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &one, &a[i__ + 1 + (i__ 
			+ 1) * a_dim1], lda, &a[i__ + (i__ + 1) * a_dim1], 
			lda, &zero, &x[i__ + 1 + i__ * x_dim1], &c__1);
		i__2 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__, &one, &y[i__ + 1 + y_dim1], 
			ldy, &a[i__ + (i__ + 1) * a_dim1], lda, &zero, &x[
			i__ * x_dim1 + 1], &c__1);
		i__2 = *m - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__, &minusone, &a[i__ + 1 + 
			a_dim1], lda, &x[i__ * x_dim1 + 1], &c__1, &one, &x[
			i__ + 1 + i__ * x_dim1], &c__1);
		i__2 = i__ - 1;
		i__3 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &one, &a[(i__ + 1) * 
			a_dim1 + 1], lda, &a[i__ + (i__ + 1) * a_dim1], lda, &
			zero, &x[i__ * x_dim1 + 1], &c__1);
		i__2 = *m - i__;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &minusone, &x[i__ + 1 + 
			x_dim1], ldx, &x[i__ * x_dim1 + 1], &c__1, &one, &x[
			i__ + 1 + i__ * x_dim1], &c__1);
		i__2 = *m - i__;
		PLUMED_BLAS_F77_FUNC(dscal,DSCAL)(&i__2, &taup[i__], &x[i__ + 1 + i__ * x_dim1], &c__1);
	    }
	}
    } else {

	i__1 = *nb;
	for (i__ = 1; i__ <= i__1; ++i__) {

	    i__2 = *n - i__ + 1;
	    i__3 = i__ - 1;
	    PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &minusone, &y[i__ + y_dim1], ldy,
		     &a[i__ + a_dim1], lda, &one, &a[i__ + i__ * a_dim1],lda);
	    i__2 = i__ - 1;
	    i__3 = *n - i__ + 1;
	    PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__3, &minusone, &a[i__ * a_dim1 + 1], 
		    lda, &x[i__ + x_dim1], ldx, &one,&a[i__+i__*a_dim1],lda);

	    i__2 = *n - i__ + 1;
	    i__3 = i__ + 1;
	    if(*n<i__3)
	      i__3 = *n;
	    PLUMED_BLAS_F77_FUNC(dlarfg,DLARFG)(&i__2, &a[i__ + i__ * a_dim1], 
		    &a[i__ + i__3 * a_dim1], lda, &taup[i__]);
	    d__[i__] = a[i__ + i__ * a_dim1];
	    if (i__ < *m) {
		a[i__ + i__ * a_dim1] = 1.;

		i__2 = *m - i__;
		i__3 = *n - i__ + 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose",&i__2,&i__3,&one,&a[i__+1+i__*a_dim1], 
		       lda, &a[i__ + i__ * a_dim1], lda, &zero, 
		       &x[i__ + 1 + i__ * x_dim1], &c__1);
		i__2 = *n - i__ + 1;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__3, &one, &y[i__ + y_dim1], 
			ldy, &a[i__ + i__ * a_dim1], lda, &zero, &x[i__ * 
			x_dim1 + 1], &c__1);
		i__2 = *m - i__;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &minusone, &a[i__ + 1 + 
			a_dim1], lda, &x[i__ * x_dim1 + 1], &c__1, &one, &x[
			i__ + 1 + i__ * x_dim1], &c__1);
		i__2 = i__ - 1;
		i__3 = *n - i__ + 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &one, &a[i__ * a_dim1 + 
			1], lda, &a[i__ + i__ * a_dim1], lda, &zero, &x[i__ *
			 x_dim1 + 1], &c__1);
		i__2 = *m - i__;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &minusone, &x[i__ + 1 + 
			x_dim1], ldx, &x[i__ * x_dim1 + 1], &c__1, &one, &x[
			i__ + 1 + i__ * x_dim1], &c__1);
		i__2 = *m - i__;
		PLUMED_BLAS_F77_FUNC(dscal,DSCAL)(&i__2, &taup[i__], &x[i__ + 1 + i__ * x_dim1], &c__1);

		i__2 = *m - i__;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &minusone, &a[i__ + 1 + 
			a_dim1], lda, &y[i__ + y_dim1], ldy, &one, &a[i__ + 
			1 + i__ * a_dim1], &c__1);
		i__2 = *m - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__, &minusone, &x[i__ + 1 + 
			x_dim1], ldx, &a[i__ * a_dim1 + 1], &c__1, &one, &a[
			i__ + 1 + i__ * a_dim1], &c__1);

		i__2 = *m - i__;
		i__3 = i__ + 2;
		if(*m<i__3)
		  i__3 = *m;
		PLUMED_BLAS_F77_FUNC(dlarfg,DLARFG)(&i__2, &a[i__ + 1 + i__ * a_dim1], 
			&a[i__3 + i__ * a_dim1], &c__1, &tauq[i__]);
		e[i__] = a[i__ + 1 + i__ * a_dim1];
		a[i__ + 1 + i__ * a_dim1] = 1.;

		i__2 = *m - i__;
		i__3 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__3, &one, &a[i__ + 1 + (i__ + 
			1) * a_dim1], lda, &a[i__ + 1 + i__ * a_dim1], &c__1, 
			&zero, &y[i__ + 1 + i__ * y_dim1], &c__1);
		i__2 = *m - i__;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__3, &one, &a[i__ + 1 + a_dim1],
			 lda, &a[i__ + 1 + i__ * a_dim1], &c__1, &zero, &y[
			i__ * y_dim1 + 1], &c__1);
		i__2 = *n - i__;
		i__3 = i__ - 1;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("No transpose", &i__2, &i__3, &minusone, &y[i__ + 1 + 
			y_dim1], ldy, &y[i__ * y_dim1 + 1], &c__1, &one, &y[
			i__ + 1 + i__ * y_dim1], &c__1);
		i__2 = *m - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__2, &i__, &one, &x[i__ + 1 + x_dim1], 
			ldx, &a[i__ + 1 + i__ * a_dim1], &c__1, &zero, &y[
			i__ * y_dim1 + 1], &c__1);
		i__2 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dgemv,DGEMV)("Transpose", &i__, &i__2, &minusone, &a[(i__ + 1) * a_dim1 
			+ 1], lda, &y[i__ * y_dim1 + 1], &c__1, &one, &y[i__ 
			+ 1 + i__ * y_dim1], &c__1);
		i__2 = *n - i__;
		PLUMED_BLAS_F77_FUNC(dscal,DSCAL)(&i__2, &tauq[i__], &y[i__ + 1 + i__ * y_dim1], &c__1);
	    }
	}
    }
    return;
} 

}
}

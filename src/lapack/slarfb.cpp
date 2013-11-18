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
#include "blas/blas.h"
#include "lapack.h"


#include "blas/blas.h"
namespace PLMD{
namespace lapack{
using namespace blas;
void 
PLUMED_BLAS_F77_FUNC(slarfb,SLARFB)(const char *side, 
	const char *trans, 
	const char *direct, 
	const char *storev, 
	int *m, 
	int *n, 
	int *k, 
	float *v, 
	int *ldv, 
	float *t, 
	int *ldt, 
	float *c__,
	int *ldc, 
	float *work, 
	int *ldwork)
{
    int c_dim1, c_offset, t_dim1, t_offset, v_dim1, v_offset, work_dim1, 
	    work_offset, i__1, i__2;

    int i__, j;
    char transt[1];
    int c__1 = 1;
    float one = 1.0;
    float minusone = -1.0;

    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    t_dim1 = *ldt;
    t_offset = 1 + t_dim1;
    t -= t_offset;
    c_dim1 = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;
    work_dim1 = *ldwork;
    work_offset = 1 + work_dim1;
    work -= work_offset;

    if (*m <= 0 || *n <= 0) {
	return;
    }
    if (*trans=='N' || *trans=='n') {
      *(unsigned char *)transt = 'T';
    } else {
	*(unsigned char *)transt = 'N';
    }
    
    if (*storev=='C' || *storev=='c') {

	if (*direct=='F' || *direct=='f') {
	  if (*side=='l' || *side=='L') {

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(n, &c__[j + c_dim1], ldc, &work[j * work_dim1 + 1],
			     &c__1);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", "No transpose", "Unit", n, k, &one,
			 &v[v_offset], ldv, &work[work_offset], ldwork);
		if (*m > *k) {

		    i__1 = *m - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("Transpose", "No transpose", n, k, &i__1, &one, &
			    c__[*k + 1 + c_dim1], ldc, &v[*k + 1 + v_dim1], 
			    ldv, &one, &work[work_offset], ldwork);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", transt, "Non-unit", n, k, &one, &t[
			t_offset], ldt, &work[work_offset], ldwork);

		if (*m > *k) {
		    i__1 = *m - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "Transpose", &i__1, n, k, &minusone, &
			    v[*k + 1 + v_dim1], ldv, &work[work_offset], 
			    ldwork, &one, &c__[*k + 1 + c_dim1], ldc);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", "Transpose", "Unit", n, k, &one, &
			v[v_offset], ldv, &work[work_offset], ldwork);

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *n;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			c__[j + i__ * c_dim1] -= work[i__ + j * work_dim1];
		    }
		}

	    } else if (*side=='r' || *side=='R') {

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(m, &c__[j * c_dim1 + 1], &c__1, &work[j * 
			    work_dim1 + 1], &c__1);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", "No transpose", "Unit", m, k, &one,
			 &v[v_offset], ldv, &work[work_offset], ldwork);
		if (*n > *k) {

		    i__1 = *n - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "No transpose", m, k, &i__1, &
			    one, &c__[(*k + 1) * c_dim1 + 1], ldc, &v[*k + 
			    1 + v_dim1], ldv, &one, &work[work_offset], 
			    ldwork);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", trans, "Non-unit", m, k, &one, &t[
			t_offset], ldt, &work[work_offset], ldwork);

		if (*n > *k) {
		    i__1 = *n - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "Transpose", m, &i__1, k, &minusone, &
			    work[work_offset], ldwork, &v[*k + 1 + v_dim1], 
			    ldv, &one, &c__[(*k + 1) * c_dim1 + 1], ldc);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", "Transpose", "Unit", m, k, &one, &
			v[v_offset], ldv, &work[work_offset], ldwork);

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			c__[i__ + j * c_dim1] -= work[i__ + j * work_dim1];
		    }
		}
	    }

	} else {

	  if (*side=='l' || *side=='L') {
		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(n, &c__[*m - *k + j + c_dim1], ldc, &work[j * 
			    work_dim1 + 1], &c__1);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", "No transpose", "Unit", n, k, &one,
			 &v[*m - *k + 1 + v_dim1], ldv, &work[work_offset], 
			ldwork);
		if (*m > *k) {
		    i__1 = *m - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("Transpose", "No transpose", n, k, &i__1, &one, &
			    c__[c_offset], ldc, &v[v_offset], ldv, &one, &
			    work[work_offset], ldwork);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", transt, "Non-unit", n, k, &one, &t[
			t_offset], ldt, &work[work_offset], ldwork);

		if (*m > *k) {

		    i__1 = *m - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "Transpose", &i__1, n, k, &minusone, &
			    v[v_offset], ldv, &work[work_offset], ldwork, &
			    one, &c__[c_offset], ldc)
			    ;
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", "Transpose", "Unit", n, k, &one, &
			v[*m - *k + 1 + v_dim1], ldv, &work[work_offset], 
			ldwork);

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *n;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			c__[*m - *k + j + i__ * c_dim1] -= work[i__ + j * 
				work_dim1];
		    }
		}

	    } else if (*side=='r' || *side=='R') {
		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(m, &c__[(*n - *k + j) * c_dim1 + 1], &c__1, &work[
			    j * work_dim1 + 1], &c__1);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", "No transpose", "Unit", m, k, &one,
			 &v[*n - *k + 1 + v_dim1], ldv, &work[work_offset], 
			ldwork);
		if (*n > *k) {
		    i__1 = *n - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "No transpose", m, k, &i__1, &
			    one, &c__[c_offset], ldc, &v[v_offset], ldv, &
			    one, &work[work_offset], ldwork);
		}
		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", trans, "Non-unit", m, k, &one, &t[
			t_offset], ldt, &work[work_offset], ldwork);
		if (*n > *k) {
		    i__1 = *n - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "Transpose", m, &i__1, k, &minusone, &
			    work[work_offset], ldwork, &v[v_offset], ldv, &
			    one, &c__[c_offset], ldc)
			    ;
		}
		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", "Transpose", "Unit", m, k, &one, &
			v[*n - *k + 1 + v_dim1], ldv, &work[work_offset], 
			ldwork);
		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			c__[i__ + (*n - *k + j) * c_dim1] -= work[i__ + j * 
				work_dim1];
		    }
		}
	    }
	}

    } else  if (*storev=='r' || *storev=='R') {
      if (*direct=='F' || *direct=='f') {
	  if (*side=='l' || *side=='L') {
		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(n, &c__[j + c_dim1], ldc, &work[j * work_dim1 + 1],
			     &c__1);
		}
		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", "Transpose", "Unit", n, k, &one, &
			v[v_offset], ldv, &work[work_offset], ldwork);
		if (*m > *k) {
		    i__1 = *m - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("Transpose", "Transpose", n, k, &i__1, &one, &
			    c__[*k + 1 + c_dim1], ldc, &v[(*k + 1) * v_dim1 + 
			    1], ldv, &one, &work[work_offset], ldwork);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", transt, "Non-unit", n, k, &one, &t[
			t_offset], ldt, &work[work_offset], ldwork);
		if (*m > *k) {

		    i__1 = *m - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("Transpose", "Transpose", &i__1, n, k, &minusone, &v[(
			    *k + 1) * v_dim1 + 1], ldv, &work[work_offset], 
			    ldwork, &one, &c__[*k + 1 + c_dim1], ldc);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", "No transpose", "Unit", n, k, &one,
			 &v[v_offset], ldv, &work[work_offset], ldwork);

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *n;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			c__[j + i__ * c_dim1] -= work[i__ + j * work_dim1];
		    }
		}

	    } else if (*side=='r' || *side=='R') {

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(m, &c__[j * c_dim1 + 1], &c__1, &work[j * 
			    work_dim1 + 1], &c__1);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", "Transpose", "Unit", m, k, &one, &
			v[v_offset], ldv, &work[work_offset], ldwork);
		if (*n > *k) {

		    i__1 = *n - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "Transpose", m, k, &i__1, &one, &
			    c__[(*k + 1) * c_dim1 + 1], ldc, &v[(*k + 1) * 
			    v_dim1 + 1], ldv, &one, &work[work_offset], 
			    ldwork);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", trans, "Non-unit", m, k, &one, &t[
			t_offset], ldt, &work[work_offset], ldwork);

		if (*n > *k) {

		    i__1 = *n - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "No transpose", m, &i__1, k, &
			    minusone, &work[work_offset], ldwork, &v[(*k + 1) * 
			    v_dim1 + 1], ldv, &one, &c__[(*k + 1) * c_dim1 
			    + 1], ldc);
		}
		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Upper", "No transpose", "Unit", m, k, &one,
			 &v[v_offset], ldv, &work[work_offset], ldwork);
		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			c__[i__ + j * c_dim1] -= work[i__ + j * work_dim1];
		    }
		}

	    }

	} else {

	    if (*side=='l' || *side=='L') {

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(n, &c__[*m - *k + j + c_dim1], ldc, &work[j * 
			    work_dim1 + 1], &c__1);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", "Transpose", "Unit", n, k, &one, &
			v[(*m - *k + 1) * v_dim1 + 1], ldv, &work[work_offset]
			, ldwork);
		if (*m > *k) {

		    i__1 = *m - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("Transpose", "Transpose", n, k, &i__1, &one, &
			    c__[c_offset], ldc, &v[v_offset], ldv, &one, &
			    work[work_offset], ldwork);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", transt, "Non-unit", n, k, &one, &t[
			t_offset], ldt, &work[work_offset], ldwork);

		if (*m > *k) {

		    i__1 = *m - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("Transpose", "Transpose", &i__1, n, k, &minusone, &v[
			    v_offset], ldv, &work[work_offset], ldwork, &
			    one, &c__[c_offset], ldc);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", "No transpose", "Unit", n, k, &one,
			 &v[(*m - *k + 1) * v_dim1 + 1], ldv, &work[
			work_offset], ldwork);

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *n;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			c__[*m - *k + j + i__ * c_dim1] -= work[i__ + j * 
				work_dim1];
		    }
		}

	    } else if (*side=='r' || *side=='R') {

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(m, &c__[(*n - *k + j) * c_dim1 + 1], &c__1, &work[
			    j * work_dim1 + 1], &c__1);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", "Transpose", "Unit", m, k, &one, &
			v[(*n - *k + 1) * v_dim1 + 1], ldv, &work[work_offset]
			, ldwork);
		if (*n > *k) {

		    i__1 = *n - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "Transpose", m, k, &i__1, &one, &
			    c__[c_offset], ldc, &v[v_offset], ldv, &one, &
			    work[work_offset], ldwork);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", trans, "Non-unit", m, k, &one, &t[
			t_offset], ldt, &work[work_offset], ldwork);

		if (*n > *k) {

		    i__1 = *n - *k;
		    PLUMED_BLAS_F77_FUNC(sgemm,SGEMM)("No transpose", "No transpose", m, &i__1, k, &
			    minusone, &work[work_offset], ldwork, &v[v_offset], 
			    ldv, &one, &c__[c_offset], ldc);
		}

		PLUMED_BLAS_F77_FUNC(strmm,STRMM)("Right", "Lower", "No transpose", "Unit", m, k, &one,
			 &v[(*n - *k + 1) * v_dim1 + 1], ldv, &work[
			work_offset], ldwork);

		i__1 = *k;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			c__[i__ + (*n - *k + j) * c_dim1] -= work[i__ + j * 
				work_dim1];
		    }
		}

	    }

	}
    }

    return;


}

}
}

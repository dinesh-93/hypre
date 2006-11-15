/*BHEADER**********************************************************************
 * Copyright (c) 2006   The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * Written by the HYPRE team. UCRL-CODE-222953.
 * All rights reserved.
 *
 * This file is part of HYPRE (see http://www.llnl.gov/CASC/hypre/).
 * Please see the COPYRIGHT_and_LICENSE file for the copyright notice, 
 * disclaimer, contact information and the GNU Lesser General Public License.
 *
 * HYPRE is free software; you can redistribute it and/or modify it under the 
 * terms of the GNU General Public License (as published by the Free Software
 * Foundation) version 2.1 dated February 1999.
 *
 * HYPRE is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS 
 * FOR A PARTICULAR PURPOSE.  See the terms and conditions of the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Revision$
 ***********************************************************************EHEADER*/




/******************************************************************************
 *
 * Header info for Parallel Vector data structure
 *
 *****************************************************************************/
#ifndef hypre_PAR_MULTIVECTOR_HEADER
#define hypre_PAR_MULTIVECTOR_HEADER

#include "_hypre_utilities.h"
#include "seq_Multivector.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------
 * hypre_ParMultiVector 
 *--------------------------------------------------------------------------*/

typedef struct
{
   MPI_Comm	      comm;
   int                global_size;
   int                first_index;
   int      	      *partitioning;
   int      	      owns_data;
   int      	      owns_partitioning;
   int      	      num_vectors;
   hypre_Multivector  *local_vector; 

/* using mask on "parallel" level seems to be inconvenient, so i (IL) moved it to
       "sequential" level. Also i now store it as a number of active indices and an array of 
       active indices. hypre_ParMultiVectorSetMask converts user-provided "(1,1,0,1,...)" mask 
       to the format above.
   int                *mask;
*/

} hypre_ParMultivector;


/*--------------------------------------------------------------------------
 * Accessor macros for the Vector structure;
 * kinda strange macros; right hand side looks much convenient than left.....
 *--------------------------------------------------------------------------*/

#define hypre_ParMultiVectorComm(vector)             ((vector) -> comm)
#define hypre_ParMultiVectorGlobalSize(vector)       ((vector) -> global_size)
#define hypre_ParMultiVectorFirstIndex(vector)       ((vector) -> first_index)
#define hypre_ParMultiVectorPartitioning(vector)     ((vector) -> partitioning)
#define hypre_ParMultiVectorLocalVector(vector)      ((vector) -> local_vector)
#define hypre_ParMultiVectorOwnsData(vector)         ((vector) -> owns_data)
#define hypre_ParMultiVectorOwnsPartitioning(vector) ((vector) -> owns_partitioning)
#define hypre_ParMultiVectorNumVectors(vector)       ((vector) -> num_vectors)

/* field "mask" moved to "sequential" level, see structure above 
#define hypre_ParMultiVectorMask(vector)             ((vector) -> mask)
*/

/* function prototypes for working with hypre_ParMultiVector */
hypre_ParMultiVector *hypre_ParMultiVectorCreate(MPI_Comm, int, int *, int);
int hypre_ParMultiVectorDestroy(hypre_ParMultiVector *);
int hypre_ParMultiVectorInitialize(hypre_ParMultiVector *);
int hypre_ParMultiVectorSetDataOwner(hypre_ParMultiVector *, int);
int hypre_ParMultiVectorSetPartitioningOwner(hypre_ParMultiVector *, int);
int hypre_ParMultiVectorSetMask(hypre_ParMultiVector *, int *);
int hypre_ParMultiVectorSetConstantValues(hypre_ParMultiVector *, double);
int hypre_ParMultiVectorSetRandomValues(hypre_ParMultiVector *, int);
int hypre_ParMultiVectorCopy(hypre_ParMultiVector *, hypre_ParMultiVector *);
int hypre_ParMultiVectorScale(double, hypre_ParMultiVector *);
int hypre_ParMultiVectorMultiScale(double *, hypre_ParMultiVector *);
int hypre_ParMultiVectorAxpy(double, hypre_ParMultiVector *,
                             hypre_ParMultiVector *);

int hypre_ParMultiVectorByDiag(  hypre_ParMultiVector *x,
                                 int                *mask, 
                                 int                n,
                                 double             *alpha,
                                 hypre_ParMultiVector *y);
                                 
int hypre_ParMultiVectorInnerProd(hypre_ParMultiVector *, 
                                      hypre_ParMultiVector *, double *, double *);
int hypre_ParMultiVectorInnerProdDiag(hypre_ParMultiVector *, 
                                      hypre_ParMultiVector *, double *, double *);
int
hypre_ParMultiVectorCopyWithoutMask(hypre_ParMultiVector *x, hypre_ParMultiVector *y);
int
hypre_ParMultiVectorByMatrix(hypre_ParMultiVector *x, int rGHeight, int rHeight, 
                              int rWidth, double* rVal, hypre_ParMultiVector * y);
int
hypre_ParMultiVectorXapy(hypre_ParMultiVector *x, int rGHeight, int rHeight, 
                              int rWidth, double* rVal, hypre_ParMultiVector * y);
                                      
int
hypre_ParMultiVectorEval(void (*f)( void*, void*, void* ), void* par,
                           hypre_ParMultiVector * x, hypre_ParMultiVector * y);

/* to be replaced by better implementation when format for multivector files established */
hypre_ParMultiVector * hypre_ParMultiVectorTempRead(MPI_Comm comm, const char *file_name);
int hypre_ParMultiVectorTempPrint(hypre_ParMultiVector *vector, const char *file_name);

#ifdef __cplusplus
}
#endif

#endif   /* hypre_PAR_MULTIVECTOR_HEADER */

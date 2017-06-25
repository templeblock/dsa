/*
 * Error.hpp
 *
 *  Created on: 16.02.2013
 *      Author: user
 */

#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <stdio.h>
#include <math.h>
#include <malloc.h>



namespace PositCoplanar {

/**********************************************************************************************************/
void PerspMoveAndProj(long int N, double **obj, double r[3][3],
		double t[3], double foc, double **proj)

		{
	double **moved;
	long int i, j, k;

	/*allocations*/
	moved = (double **) malloc(N * sizeof(double *));
	for (i = 0; i < N; i++)
		moved[i] = (double *) malloc(3 * sizeof(double));

	for (i = 0; i < N; i++) {
		for (j = 0; j < 3; j++)
			moved[i][j] = t[j];
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < 3; j++) {
			for (k = 0; k < 3; k++)
				moved[i][j] += r[j][k] * obj[i][k];
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < 2; j++)
			proj[i][j] = foc * moved[i][j] / moved[i][2];
	}

	/*desallocations*/
	for (i = 0; i < N; i++)
		free(moved[i]);

}

/**********************************************************************************************************/
void Error(long int NP,double **impts, double **obpts, double f, double Rotat[3][3],
		double Translat[3], double *Er, long int *Epr, double *Erhvmax) {

	double **impredic, **ErVect;
	long int i, j, fr;

	/*allocations*/
	impredic = (double **) malloc(NP * sizeof(double *));
	ErVect = (double **) malloc(NP * sizeof(double *));
	for (i = 0; i < NP; i++) {
		impredic[i] = (double *) malloc(2 * sizeof(double));
		ErVect[i] = (double *) malloc(2 * sizeof(double));
	}

	if ((Rotat[0][0]) != 2.0) /*un "2" en premiere position des matrices de rotation signifie que la pose est*/
	/*impossible (points derriere le plan image, par exemple)*/
	{
		PerspMoveAndProj(NP, obpts, Rotat, Translat, f, impredic); /*calcul de l'image TPP reconstruite pour la*/
		/*pose consideree*/
		for (i = 0; i < NP; i++) {
			for (j = 0; j < 2; j++)
				ErVect[i][j] = impredic[i][j] - impts[i][j];
		}

		/*mesures des erreurs*/
		*Er = 0.0;
		*Epr = 0;
		*Erhvmax = 0;
		for (i = 0; i < NP; i++) {
			*Er += sqrt(
					ErVect[i][0] * ErVect[i][0] + ErVect[i][1] * ErVect[i][1]);
			*Epr += (long int) (fabs(round(impredic[i][0]) - round(impts[i][0]))
					+ fabs(round(impredic[i][1]) - round(impts[i][1])));
			if (fabs(ErVect[i][0]) > *Erhvmax)
				*Erhvmax = fabs(ErVect[i][0]);
			if (fabs(ErVect[i][1]) > *Erhvmax)
				*Erhvmax = fabs(ErVect[i][1]);
		}
		*Er = *Er / NP;
	} else /*erreurs=-1 en cas de pose impossible*/
	{
		*Er = -1.0;
		*Epr = -1.0;
		*Erhvmax = -1.0;
	}

	/*desallocations*/
	for (fr = 0; fr < NP; fr++) {
		free(impredic[fr]);
		free(ErVect[fr]);
	}
}

}
#endif /* ERROR_HPP_ */

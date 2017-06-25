/*
 * PositCoplanar.hpp
 *
 *  Created on: 16.02.2013
 *      Author: user
 */

#ifndef POSITCOPLANAR_HPP_
#define POSITCOPLANAR_HPP_

#include <stdio.h>
#include <math.h>

#include <PositCoplanar/SVD.hpp>
#include <PositCoplanar/Basics.hpp>
#include <PositCoplanar/Composit.hpp>



namespace PositCoplanar {

#define NUMBER_OF_ROT 17  /*nombre de rotations autour de Ou, premier axe du rep. objet*/
#define NUMBER_OF_TRANS 1 /*nombre de translations suivant Cz, axe optique*/
#define TRANS_MIN 500     /*premiere des translations suivant Cz. Unites du rep. objet*/
#define TRANS_STEP 100    /*increment des translations suivant Cz*/


void AverageRotTransErrors(long int NP, double** bodyPts, double* trans,
		double fLength, double noiseAmp) {
	long int num, i, j, id = 1, rotW, fr;
	double rota[3][3], rota1[3][3], transa1[3], rotError1, transError1, nsol;
	double listRotErrors1[NUMBER_OF_ROT][3], listTransErrors1[NUMBER_OF_ROT][3];
	double listRotEr[72], listTransEr[72], RotErAverageAndStd[2],
			TransErAverageAndStd[2];
	double **ImagePoints, *bodyPtsZ;
	double PI = 3.1415927;
	double aU, aW, az, sign, dev, dec = 0.0;

	bodyPtsZ = (double *) malloc(NP * sizeof(double));
	ImagePoints = (double **) malloc(NP * sizeof(double *));
	for (i = 0; i < NP; i++)
		ImagePoints[i] = (double *) malloc(2 * sizeof(double));

	for (num = 0; num < NUMBER_OF_ROT; num++) {
		printf("\nrotation %d", 10 + num * 5);
		/*aU,aW: angles de rotation de la camera par rapport au rep. objet (Ou et Ow, en degres)*/
		/*az: angle de rotation de la camera autour de son axe optique (degres)*/
		aU = 10 + num * 5;
		az = 0.0; /*est toujours reste nul pour les essais*/

		for (rotW = 0; rotW < 360; rotW += 5) /*boucle des 72 rotations autour de Ow*/
		{
			printf("\n%d", rotW);
			aW = (double) rotW;
			Rotate2(aU, aW, az, rota); /*ATTENTION: rota est la matrice de rotation utilisee pour bouger*/
			/*l'OBJET par rapport a la CAMERA*/

			/*elevation des points objets pour la synthese de l'image*/
			/*for (i=0;i<NP;i++) bodyPts[i][2]=bodyPtsZ[i];*/

			PerspMoveAndProj(NP, bodyPts, rota, trans, fLength, ImagePoints); /*synthese de l'image TPP*/

			/*quantification+bruit+decentrage eventuel(dec)*/
			for (i = 0; i < NP; i++) {
				for (j = 0; j < 2; j++) {
					sign = ran1(&id);
					dev = round(noiseAmp * ran1(&id));
					if (sign > 0.5)
						ImagePoints[i][j] = round(ImagePoints[i][j]) + dev
								+ dec;
					else
						ImagePoints[i][j] = round(ImagePoints[i][j]) - dev
								+ dec;
					/*printf("\n%f",ImagePoints[i][j]);*/
				}
			}

			/*annulation des elevations des points objets pour le calcul de la pose (model=carte 2D...)*/
			/*for (i=0;i<NP;i++) bodyPts[i][2]=0.0;*/

			/*appel de POSIT. Retourne le nombre de poses DIFFERENTES acceptables (cf Composit() pour cette*/
			/*notion) en nsol, ainsi que la meilleure pose (rota1, transa1), i.e. la plus proche en*/
			/*rotation (transmission de rota) lorsque 2 sont acceptables, ou celle donnant la plus faible*/
			/*erreur E s'il n'y en a pas 2 acceptables.*/
			Composit(NP, ImagePoints, bodyPts, fLength, rota1, transa1,
					noiseAmp, rota, &nsol);

			/*for (i=0;i<3;i++) printf("\n%f %f %f",rota[i][0],rota[i][1],rota[i][2]);*/
			/*printf("\n");*/
			/*for (i=0;i<3;i++) printf("\n%f %f %f",rota1[i][0],rota1[i][1],rota1[i][2]);*/

			rotError1 = GetRotErrorAngle(rota, rota1); /*calcul et stockage de l'erreur en orientation*/
			listRotEr[rotW / 5] = rotError1;
			transError1 = GetTransErrors2(trans, transa1); /*calcul et stockage de l'erreur en translation*/
			listTransEr[rotW / 5] = transError1;
		}

		/*calcul de la moyenne et std des erreurs sur les 72 rotations, et stockage*/
		GetAverageAndSTD(listRotEr, RotErAverageAndStd);
		GetAverageAndSTD(listTransEr, TransErAverageAndStd);
		listRotErrors1[num][0] = 10 + num * 5;
		listRotErrors1[num][1] = RotErAverageAndStd[0];
		listRotErrors1[num][2] = RotErAverageAndStd[1];
		listTransErrors1[num][0] = 10 + num * 5;
		listTransErrors1[num][1] = TransErAverageAndStd[0];
		listTransErrors1[num][2] = TransErAverageAndStd[1];
	}
	/*printf("\nRotations done");*/

	/*sortie des resultats dans un format exploitable par MATHEMATICA*/
	printf("\nrotEr=");
	printf("\n{{%3.0f, %f, %f},", listRotErrors1[0][0], listRotErrors1[0][1],
			listRotErrors1[0][2]);
	for (i = 1; i < NUMBER_OF_ROT - 1; i++)
		printf("\n{%3.0f, %f, %f},", listRotErrors1[i][0], listRotErrors1[i][1],
				listRotErrors1[i][2]);
	printf("\n{%3.0f, %f, %f}};", listRotErrors1[NUMBER_OF_ROT - 1][0],
			listRotErrors1[NUMBER_OF_ROT - 1][1],
			listRotErrors1[NUMBER_OF_ROT - 1][2]);
	printf("\n");
	printf("\ntransEr=");
	printf("\n{{%3.0f, %f, %f},", listTransErrors1[0][0],
			listTransErrors1[0][1], listTransErrors1[0][2]);
	for (i = 1; i < NUMBER_OF_ROT - 1; i++)
		printf("\n{%3.0f, %f, %f},", listTransErrors1[i][0],
				listTransErrors1[i][1], listTransErrors1[i][2]);
	printf("\n{%3.0f, %f, %f}};\n\n", listTransErrors1[NUMBER_OF_ROT - 1][0],
			listTransErrors1[NUMBER_OF_ROT - 1][1],
			listTransErrors1[NUMBER_OF_ROT - 1][2]);

	/*desallocations*/
	free(bodyPtsZ);
	for (fr = 0; fr < NP; fr++)
		free(ImagePoints[fr]);
}

void AllRotTransErrors(long int NPts, double** bodyPts, double fLength,
		double noiseAmplitude) {
	long int i;
	double t, trans[3];

	trans[0] = 0.0;
	trans[1] = 0.0;
	for (i = 0; i < NUMBER_OF_TRANS; i++) {
		t = (double) (TRANS_MIN + i * TRANS_STEP);
		trans[2] = t;
		/*printf("\nTranslation %d = %f",i,t);*/
		printf("\ntz = %5.0f\n", t);
		AverageRotTransErrors(NPts, bodyPts, trans, fLength, noiseAmplitude);
	}
}

}
;

#endif /* POSITCOPLANAR_HPP_ */

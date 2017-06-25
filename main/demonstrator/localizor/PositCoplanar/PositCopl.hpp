/*
 * PositCopl.hpp
 *
 *  Created on: 16.02.2013
 *      Author: user
 */

#ifndef POSITCOPL_HPP_
#define POSITCOPL_HPP_

namespace PositCoplanar {

#include <stdio.h>
#include <math.h>
#include <malloc.h>


#include <PositCoplanar/SVD.hpp>
#include <PositCoplanar/Error.hpp>
#include <PositCoplanar/Noise.hpp>

using namespace PositCoplanar;

/*******************************************************************************************************/
void PosCopl(long int nbrP, double **imagePts, double **objectPts, double **objectMtrx,
		double focLength, double rotation1[3][3], double rotation2[3][3],
		double translation[3]) {

	double **imageVects, **objectVects;
	double I0[3], J0[3], U[3], u[3], IVect[3], JVect[3], row1[3], row2[3],
			row3[3];
	double NU = 0, I0I0, J0J0, I0J0, delta, q, lambda, mu, scale, zmin1, zmin2,
			zi, PI = 3.1415927;
	long int i, j, firstNonCol;

	/*allocations*/
	imageVects = (double **) malloc(nbrP * sizeof(double *));
	objectVects = (double **) malloc(nbrP * sizeof(double *));
	for (i = 0; i < nbrP; i++) {
		imageVects[i] = (double *) malloc(2 * sizeof(double));
		objectVects[i] = (double *) malloc(3 * sizeof(double));
	}

	for (i = 0; i < nbrP; i++) {
		imageVects[i][0] = imagePts[i][0] - imagePts[0][0];
		imageVects[i][1] = imagePts[i][1] - imagePts[0][1];
		objectVects[i][0] = objectPts[i][0] - objectPts[0][0];
		objectVects[i][1] = objectPts[i][1] - objectPts[0][1];
		objectVects[i][2] = objectPts[i][2] - objectPts[0][2];
	}

	/*calcul de Io et Jo*/
	for (i = 0; i < 3; i++) {
		I0[i] = 0;
		J0[i] = 0;
		for (j = 0; j < nbrP; j++) {
			I0[i] += objectMtrx[i][j] * imageVects[j][0];
			J0[i] += objectMtrx[i][j] * imageVects[j][1];
		}
	}

	I0I0 = I0[0] * I0[0] + I0[1] * I0[1] + I0[2] * I0[2];
	J0J0 = J0[0] * J0[0] + J0[1] * J0[1] + J0[2] * J0[2];
	I0J0 = I0[0] * J0[0] + I0[1] * J0[1] + I0[2] * J0[2];

	/*printf("\nI0=%f, J0=%f, I0J0=%f",sqrt(I0I0),sqrt(J0J0),I0J0);*/

	/*calcul de u, vecteur normal au plan objet*/
	firstNonCol = 2;
	while (NU == 0.0) {
		U[0] = objectVects[1][1] * objectVects[firstNonCol][2]
				- objectVects[1][2] * objectVects[firstNonCol][1];
		U[1] = objectVects[1][2] * objectVects[firstNonCol][0]
				- objectVects[1][0] * objectVects[firstNonCol][2];
		U[2] = objectVects[1][0] * objectVects[firstNonCol][1]
				- objectVects[1][1] * objectVects[firstNonCol][0];
		NU = sqrt(U[0] * U[0] + U[1] * U[1] + U[2] * U[2]);
		firstNonCol++;
	}
	for (i = 0; i < 3; i++)
		u[i] = U[i] / NU;

	/*calcul de lambda et mu, par la methode de l'equation bicarree*/
	delta = (J0J0 - I0I0) * (J0J0 - I0I0) + 4 * (I0J0 * I0J0);
	if ((I0I0 - J0J0) >= 0)
		q = -(I0I0 - J0J0 + sqrt(delta)) / 2;
	else
		q = -(I0I0 - J0J0 - sqrt(delta)) / 2;
	if (q >= 0) {
		lambda = sqrt(q);
		if (lambda == 0.0)
			mu = 0.0;
		else
			mu = -I0J0 / sqrt(q);
	} else {
		lambda = sqrt(-(I0J0 * I0J0) / q);
		if (lambda == 0.0)
			mu = sqrt(I0I0 - J0J0);
		else
			mu = -I0J0 / sqrt(-(I0J0 * I0J0) / q);
	}
	/*printf("\nlambda=%f mu=%f",lambda,mu);*/

	/*CALCUL DE LA PREMIERE MATRICE DE ROTATION*/
	for (i = 0; i < 3; i++) {
		IVect[i] = I0[i] + lambda * u[i];
		JVect[i] = J0[i] + mu * u[i];
	}
	scale = sqrt(
			IVect[0] * IVect[0] + IVect[1] * IVect[1] + IVect[2] * IVect[2]);
	for (i = 0; i < 3; i++) {
		row1[i] = IVect[i] / scale;
		row2[i] = JVect[i] / scale;
	}
	row3[0] = row1[1] * row2[2] - row1[2] * row2[1];
	row3[1] = row2[0] * row1[2] - row1[0] * row2[2];
	row3[2] = row1[0] * row2[1] - row1[1] * row2[0];
	for (i = 0; i < 3; i++) {
		rotation1[0][i] = row1[i];
		rotation1[1][i] = row2[i];
		rotation1[2][i] = row3[i];
	}

	/*CALCUL DE LA DEUXIEME MATRICE DE ROTATION*/
	for (i = 0; i < 3; i++) {
		IVect[i] = I0[i] - lambda * u[i];
		JVect[i] = J0[i] - mu * u[i];
	}
	for (i = 0; i < 3; i++) {
		row1[i] = IVect[i] / scale;
		row2[i] = JVect[i] / scale;
	}
	row3[0] = row1[1] * row2[2] - row1[2] * row2[1];
	row3[1] = row2[0] * row1[2] - row1[0] * row2[2];
	row3[2] = row1[0] * row2[1] - row1[1] * row2[0];
	for (i = 0; i < 3; i++) {
		rotation2[0][i] = row1[i];
		rotation2[1][i] = row2[i];
		rotation2[2][i] = row3[i];
	}

	/*CALCUL DE LA TRANSLATION*/
	translation[0] = imagePts[0][0] / scale;
	translation[1] = imagePts[0][1] / scale;
	translation[2] = focLength / scale;

	/*calcul du minimum des profondeurs zi des points objets dans le repere camera, pour la premiere pose*/
	for (i = 0; i < nbrP; i++) {
		zi = translation[2]
				+ (rotation1[2][0] * objectVects[i][0]
						+ rotation1[2][1] * objectVects[i][1]
						+ rotation1[2][2] * objectVects[i][2]);
		if (i == 0)
			zmin1 = zi;
		if (zi < zmin1)
			zmin1 = zi;
	}

	/*calcul du minimum des profondeurs zi des points objets dans le repere camera, pour la deuxieme pose*/
	for (i = 0; i < nbrP; i++) {
		zi = translation[2]
				+ (rotation2[2][0] * objectVects[i][0]
						+ rotation2[2][1] * objectVects[i][1]
						+ rotation2[2][2] * objectVects[i][2]);
		if (i == 0)
			zmin2 = zi;

		if (zi < zmin2)
			zmin2 = zi;	//Afshin: I corrected this. It was zmin1 by mistake
//    if (zi<zmin1) zmin2=zi;	//Afshin: This was the original code
	}
	/*Le premier element des matrices de rotation est mis a 2 en cas de pose impossible*/
	/*(points objets derriere le plan image)*/
	if (zmin1 < 0)
		rotation1[0][0] = 2;
	if (zmin2 < 0)
		rotation2[0][0] = 2;

	/*desallocations*/
	for (i = 0; i < nbrP; i++) {
		free(imageVects[i]);
		free(objectVects[i]);
	}
}

/*******************************************************************************************************/
void Transf(long int nP, double **imPoints, double **obPoints, double **obMatrix,
		double focalL, double Rot[3][3], double Trans[3], double R[3][3], double T[3])
{

	double **obVectors,**oldSOPImagePoints,**SOPImagePoints;
	double fact,imDifference,oldImDifference,deltaImDifference,Er1,Erhvmax1,Er2,Erhvmax2,Er,Erhvmax;
	double Threshold=0.01; /*seuil de convergence sur:*/
	/*-difference des images SOP consecutives (ImDifference)*/
	/*-et sa variation (deltaImDifference)*/
	double R1[3][3],R2[3][3],trans1[3],trans2[3];
	long int i,j,Erp1,Erp2,Erp,Erpmin=1000;
	long int converged=0;
	long int count=0;

	/*allocations*/
	obVectors=(double **)malloc(nP * sizeof(double *));
	oldSOPImagePoints=(double **)malloc(nP * sizeof(double *));
	SOPImagePoints=(double **)malloc(nP * sizeof(double *));
	for (i=0;i<nP;i++)
	{
		obVectors[i]=(double *)malloc(3 * sizeof(double));
		oldSOPImagePoints[i]=(double *)malloc(2 * sizeof(double));
		SOPImagePoints[i]=(double *)malloc(2 * sizeof(double));
	}

	for (i=0;i<nP;i++)
	{
		obVectors[i][0]=obPoints[i][0]-obPoints[0][0];
		obVectors[i][1]=obPoints[i][1]-obPoints[0][1];
		obVectors[i][2]=obPoints[i][2]-obPoints[0][2];
	}
	for (i=0;i<nP;i++)
	{
		for (j=0;j<2;j++) oldSOPImagePoints[i][j]=imPoints[i][j];
	}
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++) R[i][j]=Rot[i][j];
		T[i]=Trans[i];
	}

	/*construction de la premiere SOPImage*/
	for (i=0;i<nP;i++)
	{
		fact=0.0;
		for (j=0;j<3;j++) fact+=obVectors[i][j]*R[2][j]/T[2];
		for (j=0;j<2;j++) SOPImagePoints[i][j]=(1 + fact)*imPoints[i][j];
	}

	/*calcul de la difference SOPImage/Image en debut de processus*/
	imDifference=0.0;
	for (i=0;i<nP;i++)
	{
		for (j=0;j<2;j++)
		{
			imDifference+=fabs(SOPImagePoints[i][j] -
					oldSOPImagePoints[i][j]);
		}
	}

	/*mesures des ecarts Image originale/TPPImage reconstruite (necessite d'exprimer le vecteur de*/
	/*translation d'origine a origine (l'origine du rep. objet n'est pas forcement confondue avec Mo)*/
	for (i=0;i<3;i++)
	{
		trans1[i]=T[i]-(R[i][0]*obPoints[0][0]+
				R[i][1]*obPoints[0][1]+
				R[i][2]*obPoints[0][2]);
	}
	Error(nP,imPoints,obPoints,focalL,R,trans1,&Er1,&Erp1,&Erhvmax1); /*cf file Error.c*/

	/*test de convergence en debut de processus. On ne procede pas aux iterations si l'image reconstruite*/
	/*est, en pixels, identique a celle de depart, ou si la SOPImage n'a pas (assez) evolue*/
	converged=(Erp1==0)||(imDifference<Threshold);
	/*printf("\nimDiff_orig=%f Er_orig=%f",imDifference,Er1);*/

	/*PROCESSUS ITERATIF*/
	while (!converged)
	{
		/*printf("\n        %d",count);*/
		for (i=0;i<nP;i++)
		{
			for (j=0;j<2;j++) oldSOPImagePoints[i][j]=SOPImagePoints[i][j];
		}

		/*calcul des deux poses sous hypothese SOP d'apres SOPImage*/
		PosCopl(nP,SOPImagePoints,obPoints,obMatrix,focalL,R1,R2,T);

		/*mesure des erreurs pour les deux poses*/
		for (i=0;i<3;i++)
		{
			trans1[i]=T[i]-(R1[i][0]*obPoints[0][0]+
					R1[i][1]*obPoints[0][1]+
					R1[i][2]*obPoints[0][2]);
			trans2[i]=T[i]-(R2[i][0]*obPoints[0][0]+
					R2[i][1]*obPoints[0][1]+
					R2[i][2]*obPoints[0][2]);
		}
		Error(nP,imPoints,obPoints,focalL,R1,trans1,&Er1,&Erp1,&Erhvmax1);
		Error(nP,imPoints,obPoints,focalL,R2,trans2,&Er2,&Erp2,&Erhvmax2);

		/*choix de la pose donnant la TPPImage la plus proche de l'image originale (selon Er)*/
		if ((Er1>=0)&&(Er2>=0)) /*a condition que les deux poses soient possibles (sinon, erreurs a -1)*/
		{
			if (Er2<Er1)
			{
				Er=Er2;
				Erp=Erp2;
				Erhvmax=Erhvmax2;
				for (i=0;i<3;i++)
				{
					for (j=0;j<3;j++) R[i][j]=R2[i][j];
				}
			}
			else
			{
				Er=Er1;
				Erp=Erp1;
				Erhvmax=Erhvmax1;
				for (i=0;i<3;i++)
				{
					for (j=0;j<3;j++) R[i][j]=R1[i][j];
				}
			}
		}

		/*si l'une des deux poses est impossible, on selectionne l'autre*/
		if ((Er1<0)&&(Er2>=0))
		{
			Er=Er2;
			Erp=Erp2;
			Erhvmax=Erhvmax2;
			for (i=0;i<3;i++)
			{
				for (j=0;j<3;j++) R[i][j]=R2[i][j];
			}
		}
		if ((Er2<0)&&(Er1>=0))
		{
			Er=Er1;
			Erp=Erp1;
			Erhvmax=Erhvmax1;
			for (i=0;i<3;i++)
			{
				for (j=0;j<3;j++) R[i][j]=R1[i][j];
			}
		}

		/*if ((Er2<0)&&(Er1<0))...CAS A IMPLEMENTER?...(n'apparait jamais)*/

		/*calcul de SOPImage pour la pose retenue*/
		for (i=0;i<nP;i++)
		{
			fact=0.0;
			for (j=0;j<3;j++) fact+=obVectors[i][j]*R[2][j]/T[2];
			for (j=0;j<2;j++) SOPImagePoints[i][j]=(1 + fact)*imPoints[i][j];
		}

		/*calcul de la difference SOPImage/SOPImage precedente*/
		oldImDifference=imDifference;
		imDifference=0.0;
		for (i=0;i<nP;i++)
		{
			for (j=0;j<2;j++)
			{
				imDifference+=fabs(SOPImagePoints[i][j] -
						oldSOPImagePoints[i][j]);
			}
		}

		/*evolution de cette difference?*/
		deltaImDifference=fabs(imDifference-oldImDifference);

		/*test de convergence sur l'ecart en pixels Image originale/TPPImage reconstruite*/
		/*ainsi que test de securite stoppant le processus en cas de stagnation en*/
		/*imDifference=Cte (0, en general, mais pas toujours...)*/
		converged=(Erp==0)||(deltaImDifference<Threshold);

		/*test de securite en cas de non convergence (oscillations periodiques)*/
		if (Erp<Erpmin) Erpmin=Erp;
		if ((count>100)&&(Erp==Erpmin)) converged=1;

		count++;
		/*printf("\n%d %f %d %f",count-1,Erhvmax,Erp,deltaImDifference);*/

	}

	/*desallocations*/
	for (i=0;i<nP;i++)
	{
		free(obVectors[i]);
		free(oldSOPImagePoints[i]);
		free(SOPImagePoints[i]);
	}
}



/****************************************************************************************************/
void PositCopl(long int nbP, double **imagePoints, double **objectPoints,
		double **objectMatrix, double focalLength, double Rot1[3][3],
		double Trans1[3], double Rot2[3][3], double Trans2[3]) {

	double POSRot1[3][3], POSRot2[3][3], POSTrans[3];

	long int i, j;

	PosCopl(nbP, imagePoints, objectPoints, objectMatrix, focalLength, POSRot1,
			POSRot2, POSTrans); /*appel de la fonction POS (approximation SOP)*/
	/*pour points COPLanaires*/
	/*Retourne une translation et deux rotations*/
	/*Le premier element des matrices de rotation est mis*/
	/*a 2 en cas de pose impossible (points objets derriere*/
	/*le plan image)*/

	if ((POSRot1[0][0]) != 2.0) /*pose1 a priori possible*/
	{
		/*printf("\nBranche 1");*/
		Transf(nbP, imagePoints, objectPoints, objectMatrix, focalLength,
				POSRot1, POSTrans, Rot1, Trans1); /*ITERATIONS a partir de la premiere pose fournie*/
		/*PosCopl (BRANCHE 1)*/
	} else
		Rot1[0][0] = 2.0;

	if ((POSRot2[0][0]) != 2.0) /*pose2 a priori possible*/
	{
		/*printf("\nBranche 2");*/
		Transf(nbP, imagePoints, objectPoints, objectMatrix, focalLength,
				POSRot2, POSTrans, Rot2, Trans2); /*ITERATIONS a partir de la deuxieme pose fournie*/
		/*PosCopl (BRANCHE 2)*/
	} else
		Rot2[0][0] = 2.0;
}



}

#endif /* POSITCOPL_HPP_ */

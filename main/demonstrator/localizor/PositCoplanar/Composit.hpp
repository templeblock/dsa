/*
 * Composit.hpp
 *
 *  Created on: 16.02.2013
 *      Author: user
 */

#ifndef COMPOSIT_HPP_
#define COMPOSIT_HPP_

#include <stdio.h>
#include <malloc.h>
#include <math.h>

//#include <PositCoplanar/Error.hpp>
//#include <PositCoplanar/Noise.hpp>

#include <PositCoplanar/PositCopl.hpp>
#include <PositCoplanar/SVD.hpp>
#include <PositCoplanar/Basics.hpp>


namespace PositCoplanar {


/*********************************************************************************************************/
void Composit(long int np, double **coplImage, double **copl, double fLength, double R[3][3],
		double T[3], double noise, double rot[3][3],
		double *nbsol)

		{
	std::cout << "entering coposit" << std::endl;

	long int i, j, Ep1, Ep2, fr;
	double E1, E2, Ehvmax1, Ehvmax2, noise_quantif = 0.5, rotEr1, rotEr2, rotEc;
	double **coplVectors, **coplMatrix;
	double POSITRot1[3][3], POSITTrans1[3], POSITRot2[3][3], POSITTrans2[3];


	/*allocations*/
	coplVectors = (double **) malloc(np * sizeof(double *));
	coplMatrix = (double **) malloc(3 * sizeof(double *));
	for (i = 0; i < np; i++)
		coplVectors[i] = (double *) malloc(3 * sizeof(double));
	for (i = 0; i < 3; i++)
		coplMatrix[i] = (double *) malloc(np * sizeof(double));

	for (i = 0; i < np; i++) {
		std::cout << "coplVectors " << i << std::endl;
		std::cout << "coplVecotr 00 " << &(copl[i][0]) << ":" << copl[i][0] << std::endl;
		std::cout << "coplVecotr 01 " << &(copl[i][1]) << ":" << copl[i][1] << std::endl;
		std::cout << "coplVecotr 02 " << &(copl[i][2]) << ":" << copl[i][2] << std::endl;
		coplVectors[i][0] = copl[i][0] - copl[0][0];
		coplVectors[i][1] = copl[i][1] - copl[0][1];
		coplVectors[i][2] = copl[i][2] - copl[0][2];
	}

	PseudoInverse(coplVectors, np, coplMatrix); /*coplMatrix est la pseudoinverse de coplVectors*/

	/*for (i=0;i<3;i++)*/
	/*{*/
	/*  printf("\n");*/
	/*  for (j=0;j<np;j++) printf("%e ",coplMatrix[i][j]);*/
	/*}*/
	std::cout << "before PositCopl call" << std::endl;
	PositCopl(np, coplImage, copl, coplMatrix, fLength, POSITRot1, POSITTrans1,
			POSITRot2, POSITTrans2);
	std::cout << "PositCopl call done" << std::endl;
	/*retourne les DEUX poses resultant de la convergence des deux branches de POSIT, sans les juger*/

	/*calcul des translations d'origine a origine  (l'origine du rep. objet n'est pas forcement*/
	/*confondue avec Mo)*/
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			POSITTrans1[i] -= POSITRot1[i][j] * copl[0][j];
			POSITTrans2[i] -= POSITRot2[i][j] * copl[0][j];
		}
	}

	*nbsol = 1.0;

	std::cout << "composit "<< __LINE__ << std::endl;

	/*ATTENTION: Positcopl() met un "2" en premiere position des matrices de rotation si la pose est*/
	/*impossible (points derriere le plan image, par exemple)*/
	if ((POSITRot1[0][0] != 2) && (POSITRot2[0][0] != 2)) {
		std::cout << "composit "<< __LINE__ << std::endl;
		PositCoplanar::Error(np, coplImage, copl, fLength, POSITRot1, POSITTrans1, &E1, &Ep1,
				&Ehvmax1);
		std::cout << "composit "<< __LINE__ << std::endl;
		PositCoplanar::Error(np, coplImage, copl, fLength, POSITRot2, POSITTrans2, &E2, &Ep2,
				&Ehvmax2);
		std::cout << "composit "<< __LINE__ << std::endl;
		/*Error retourne differentes mesures d'erreurs fondees sur les ecarts*/
		/*Image TPP reconstruite/Image de depart:*/
		/*E est la distance euclidienne moyenne entre points images reconstruits et points images originaux*/
		/*Ep est la somme des ecarts horizontaux et verticaux en pixels*/
		/*Ehvmax est l'ecart horizontal ou vertical maximum rencontre (valeurs non arrondies en pixels)*/
		/*printf("\nErhvmax1=%f Erhvmax2=%f\n",Ehvmax1,Ehvmax2);*/

		rotEr1 = GetRotErrorAngle(rot, POSITRot1); /*ecart angulaire entre orientation exacte et orientation*/
		/*calculee*/
		std::cout << "composit "<< __LINE__ << std::endl;
		rotEr2 = GetRotErrorAngle(rot, POSITRot2); /*idem*/
		/*printf("\nrotEr1=%f rotEr2=%f",rotEr1,rotEr2);*/

		/*cas de deux solutions "ACCEPTABLES", pour le niveau de bruit*/
		/*(i.e ((Ehvmax1<=noise+noise_quantif)&&(Ehvmax2<=noise+noise_quantif))),*/
		/*ou de deux solutions pas forcement "acceptables pour le niveau de bruit", mais donnant la meme*/
		/*erreur en pixels sur les images reconstruites (Ep1=Ep2, dans ce cas les deux sol sont tout aussi*/
		/*vraissemblables (equipotentielle), et il n'y a pas de raison de privilegier la "meilleure" au*/
		/*sens E). Parmi les deux poses, on retientalors celle qui est la plus proche en orientation de*/
		/*celle qui a servi a synthetiser l'image de depart. Si de plus ces deux poses sont distantes de*/
		/*plus de 5 degres en orientation, elle sont considerees comme differentes et nbsol=2*/

		std::cout << "composit "<< __LINE__ << std::endl;

		if (((Ehvmax1 <= noise + noise_quantif)
				&& (Ehvmax2 <= noise + noise_quantif)) || (Ep1 == Ep2))

		{
			/*On choisit la meilleure pose, i.e. la plus proche de la pose exacte, en rotation*/
			if (rotEr1 < rotEr2) {
				for (i = 0; i < 3; i++) {
					T[i] = POSITTrans1[i];
					for (j = 0; j < 3; j++)
						R[i][j] = POSITRot1[i][j];
				}
			} else {
				for (i = 0; i < 3; i++) {
					T[i] = POSITTrans2[i];
					for (j = 0; j < 3; j++)
						R[i][j] = POSITRot2[i][j];
				}
			}
			rotEc = GetRotErrorAngle(POSITRot1, POSITRot2); /*les deux poses sont declarees DIFFERENTES si*/
			/*l'ecart angulaire entre les deux rotations*/
			/*est > 5 deg.*/
			if (rotEc > 5.0) {
				/*printf("\nDeux poses acceptables");*/
				*nbsol = 2.0;
			}
		}

		else /*sinon, la meilleure des deux au sens E est selectionnee, et nbsol reste egal a 1*/
		{
			if (E1 < E2) {
				for (i = 0; i < 3; i++) {
					T[i] = POSITTrans1[i];
					for (j = 0; j < 3; j++)
						R[i][j] = POSITRot1[i][j];
				}
			} else {
				for (i = 0; i < 3; i++) {
					T[i] = POSITTrans2[i];
					for (j = 0; j < 3; j++)
						R[i][j] = POSITRot2[i][j];
				}
			}
		}
	}

	std::cout << "composit "<< __LINE__ << std::endl;

	/*cas ou l'une des deux poses est impossible. L'autre est alors selectionnee*/
	if ((POSITRot1[0][0] != 2) && (POSITRot2[0][0] == 2)) {
		/*Error(np,coplImage,copl,fLength,POSITRot1,POSITTrans1,&E1,&Ep1,&Ehvmax1);*/
		/*Error(np,coplImage,copl,fLength,POSITRot2,POSITTrans2,&E2,&Ep2,&Ehvmax2);*/
		/*printf("\nErhvmax1=%f Erhvmax2=%f\n",Ehvmax1,Ehvmax2); */
		for (i = 0; i < 3; i++) {
			T[i] = POSITTrans1[i];
			for (j = 0; j < 3; j++)
				R[i][j] = POSITRot1[i][j];
		}
	}

	std::cout << "composit "<< __LINE__ << std::endl;

	if ((POSITRot1[0][0] == 2) && (POSITRot2[0][0] != 2)) {
		/*Error(np,coplImage,copl,fLength,POSITRot1,POSITTrans1,&E1,&Ep1,&Ehvmax1);*/
		/*Error(np,coplImage,copl,fLength,POSITRot2,POSITTrans2,&E2,&Ep2,&Ehvmax2);*/
		/*printf("\nErhvmax1=%f Erhvmax2=%f\n",Ehvmax1,Ehvmax2);*/
		for (i = 0; i < 3; i++) {
			T[i] = POSITTrans2[i];
			for (j = 0; j < 3; j++)
				R[i][j] = POSITRot2[i][j];
		}
	}

	std::cout << "composit "<< __LINE__ << std::endl;

	/* if ((POSITRot1[0][0]==2)&&(POSITRot2[0][0]==2))...CAS A IMPLEMENTER (n'apparait jamais)*/

	/*desallocations*/
	for (fr = 0; fr < np; fr++)
		free(coplVectors[fr]);
	for (fr = 0; fr < 3; fr++)
		free(coplMatrix[fr]);

	std::cout << "composit "<< __LINE__ << std::endl;
}

}

#endif /* COMPOSIT_HPP_ */

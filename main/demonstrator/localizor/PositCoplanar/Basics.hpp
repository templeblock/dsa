/*
 * Basics.hpp
 *
 *  Created on: 16.02.2013
 *      Author: user
 */

#ifndef BASICS_HPP_
#define BASICS_HPP_

#include <PositCoplanar/SVD.hpp>
#include <math.h>

namespace PositCoplanar {

double round(double x) {
	return ((fabs(x - ceil(x))) < (fabs(x - floor(x))) ? (ceil(x)) : (floor(x)));
}


/*******************************************************************************************************/
double Norm(double vector[3])
{
	double n;
	n=sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
	return(n);
}

/*******************************************************************************************************/
double GetTransErrors2(double* transVect1, double* transVect2)
{
	double vectDiff[3],delta,distanceError;
	long int i;
	for (i=0;i<3;i++) vectDiff[i]=transVect2[i]-transVect1[i];
	delta=Norm(vectDiff);
	distanceError=delta/Norm(transVect1);
	return(distanceError);
}

/*******************************************************************************************************/
void GetAverageAndSTD(double* list, double averageAndStd[2])
{
	double tot=0.0,tot2=0.0;
	long int i,NUMBER=72; /*a ameliorer pour calcul automatique du nombre de valeurs...*/
	for (i=0;i<NUMBER;i++) tot+=list[i];
	averageAndStd[0]=tot/NUMBER;
	for (i=0;i<NUMBER;i++) tot2+=(list[i]-averageAndStd[0])*(list[i]-averageAndStd[0]);
	averageAndStd[1]=sqrt(tot2/NUMBER);
}

/*******************************************************************************************************/
double *UnitVect(double vector[3])
{
	double *vect;
	double n;
	vect=(double *)malloc(3 * sizeof(double));
	n=Norm(vector);
	vect[0]=vector[0]/n;
	vect[1]=vector[1]/n;
	vect[2]=vector[2]/n;
	return(vect);
}


/*******************************************************************************************************/
double *CrossProd(double v1[3], double v2[3])
{
	double *w;
	w=(double *)malloc(3 * sizeof(double));
	w[0]=v1[1]*v2[2]-v1[2]*v2[1];
	w[1]=v1[2]*v2[0]-v1[0]*v2[2];
	w[2]=v1[0]*v2[1]-v1[1]*v2[0];
	return(w);
}

/*******************************************************************************************************/
double GetRotErrorAngle(double rotMat1[3][3], double rotMat2[3][3])
{
	double **deltaRot,rotAxis[3],*n1,*n2,angle=0.0,*ad;
	long int i,j,smallestDiagIndex = 0,n=0,fr;
	double *diagonal,**V;
	double PI=3.1415927;

	// EDITED: REMOVE NANS
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(std::isnan(rotMat1[i][j])) rotMat1[i][j] = 0;
			if(std::isnan(rotMat2[i][j])) rotMat2[i][j] = 0;
		}
	}

	/*allocations*/
	V=(double **)malloc(3*sizeof(double *));
	diagonal=(double *)malloc(3*sizeof(double));
	deltaRot=(double **)malloc(3*sizeof(double *));
	for (i=0;i<3;i++)
	{
		V[i]=(double *)malloc(3*sizeof(double));
		deltaRot[i]=(double *)malloc(3*sizeof(double));
	}

	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++) deltaRot[i][j]=rotMat2[i][j]-rotMat1[i][j];
	}


	PositCoplanar::svdcmp(deltaRot,3,3,diagonal,V);

	for (i=1;i<3;i++)
	{
		if (diagonal[i]<diagonal[0]) smallestDiagIndex=i;
	}
	for (i=0;i<3;i++) rotAxis[i]=V[i][smallestDiagIndex];


	if ((Norm(CrossProd(rotMat1[0],rotAxis))!=0.0)&&(Norm(CrossProd(rotMat2[0],rotAxis))!=0.0))
	{
		ad=CrossProd(rotMat1[0],rotAxis);
		n1=UnitVect(ad);
		free(ad);
		ad=CrossProd(rotMat2[0],rotAxis);
		n2=UnitVect(ad);
		free(ad);
		angle+=asin(Norm(CrossProd(n1,n2)))*180/PI;
		n++;
	}


	if ((Norm(CrossProd(rotMat1[1],rotAxis))!=0.0)&&(Norm(CrossProd(rotMat2[1],rotAxis))!=0.0))
	{
		ad=CrossProd(rotMat1[0],rotAxis);
		n1=UnitVect(ad);
		free(ad);
		ad=CrossProd(rotMat2[0],rotAxis);
		n2=UnitVect(ad);
		free(ad);
		angle+=asin(Norm(CrossProd(n1,n2)))*180/PI;
		n++;
	}

	if ((Norm(CrossProd(rotMat1[2],rotAxis))!=0.0)&&(Norm(CrossProd(rotMat2[2],rotAxis))!=0.0))
	{
		ad=CrossProd(rotMat1[0],rotAxis);
		n1=UnitVect(ad);
		free(ad);
		ad=CrossProd(rotMat2[0],rotAxis);
		n2=UnitVect(ad);
		free(ad);
		angle+=asin(Norm(CrossProd(n1,n2)))*180/PI;
		n++;
	}

	/*desallocations*/
	free(n1);
	free(n2);
	for (i=0;i<3;i++)
	{
		free(V[i]);
		free(deltaRot[i]);
	}
	free(diagonal);


	return(angle/n);
}




/*******************************************************************************************************/
void Rotate(double ax, double ay, double az, double R[3][3]) {
	double cx, sx, cy, sy, cz, sz;
	cx = cos(ax);
	sx = sin(ax);
	cy = cos(ay);
	sy = sin(ay);
	cz = cos(az);
	sz = sin(az);
	R[0][0] = cy * cz;
	R[0][1] = -(cy * sz);
	R[0][2] = sy;
	R[1][0] = cz * sx * sy + cx * sz;
	R[1][1] = cx * cz - sx * sy * sz;
	R[1][2] = -(cy * sx);
	R[2][0] = sx * sz - cx * cz * sy;
	R[2][1] = cz * sx + cx * sy * sz;
	R[2][2] = cx * cy;
}

/*******************************************************************************************************/
void Rotate2(double aU, double aW, double az, double R[3][3]) {
	long int i, j, k;
	double PI = 3.1415927;
	double RW[3][3], RU[3][3], Rz[3][3], RURW[3][3];

	Rotate(0.0, 0.0, -aW * PI / 180, RW);
	Rotate((aU + 90) * PI / 180, 0.0, 0.0, RU);
	Rotate(0.0, 0.0, az * PI / 180, Rz);
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			RURW[i][j] = 0;
			for (k = 0; k < 3; k++)
				RURW[i][j] += RU[i][k] * RW[k][j];
		}
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			R[i][j] = 0;
			for (k = 0; k < 3; k++)
				R[i][j] += Rz[i][k] * RURW[k][j];
		}
	}
}

}


#endif /* BASICS_HPP_ */

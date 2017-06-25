/*
 * PoseEstimationAlgorithm.hpp
 *
 *  Created on: 11.04.2013
 *      Author: Bigontina
 */

#ifndef POSEESTIMATIONALGORITHM_HPP_
#define POSEESTIMATIONALGORITHM_HPP_


enum PoseEstimationAlgorithm
{
	ITERATIVE = 1,
	EPNP = 2, // F.Moreno-Noguer, V.Lepetit and P.Fua "EPnP: Efficient Perspective-n-Point Camera Pose Estimation"
	P3P = 3, // X.S. Gao, X.-R. Hou, J. Tang, H.-F. Chang; "Complete Solution Classification for the Perspective-Three-Point Problem
	POSIT = 4, // DeMenthon et al.
	PLANARPOSIT = 5,
	RPPAlgo = 6
};


#endif /* POSEESTIMATIONALGORITHM_HPP_ */

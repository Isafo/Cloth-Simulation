/******************************************************
 * FindClosestNeighbor                                *
 * Returns the position in the vector with the masses *
 * positions for the mass that the user is selecting. *
 * If no mass is selected it returns -1 instead.      *
 ******************************************************/

#include "FindClosestNeighbor.h"


/*********************************************************************************************
 *  massPositions     is the vector with all the coordinates for all the masses in the cloth *
 *  mousePosX         is the coordinate on the x-axis for the mouse/cursor                   *
 *  mousePosY         is the coordinate on the x-axis for the mouse/cursor                   *
 *  cameraPos         is the coordinates for the camera                                      *
 *  clippingPlaneNear is the coordinate on the z-axis for the near clipping plane            *
 *  maxSpringLength   is the maximum length a stretch spring can be stretched				 *
 *********************************************************************************************/

int FindClosestNeighbor(vector<glm::vec3> massPositions, double mousePosX, double mousePosY, const int clippingPlaneNear, const float maxSpringLength)
{
	//create a 3D position for the mouse
	//The mouse coordinates are already scaled to match openGL
	glm::vec3 mousePosition = glm::vec3(-mousePosX, mousePosY, clippingPlaneNear);

	//create a ray from the mouse out into the 3D space
	glm::vec3 rayN = glm::vec3(0, 0, 1);
	
	//check the shortest distance to the ray for every mass
	//and save the necessary info for all masses inside the threshold radius
	//in a vector of CloseMasses
	vector<CloseMasses> possibleTargets;
	
	for(int i = 0; i < massPositions.size(); i++)
	{
		glm::vec3 distToRayW, distFromCamU, partOfRayV;
		float length;

		// V = N (U*N)/pow(sqrt(pow(N.x, 2) + pow(N.y, 2) + pow(N.z, 2), 2));
		// W = U - V;
		distFromCamU = massPositions[i] - mousePosition;
		partOfRayV = rayN * (distFromCamU.x * rayN.x + distFromCamU.y * rayN.y + distFromCamU.z * rayN.z) / (pow(rayN.x, 2) + pow(rayN.y, 2) + pow(rayN.z, 2));
		distToRayW = distFromCamU - partOfRayV;

		//Gabriella's function norm
		//length = norm(distToRayW);
		length = sqrt(pow(distToRayW.x, 2) + pow(distToRayW.y, 2) + pow(distToRayW.z, 2));

		if (length < maxSpringLength)
		{
			//info to be saved
			//the shortest length between the ray and the mass: vec3 W
			//the length of the ray until the point where W starts : vec3 V
			//the place the mass is in the vector
			CloseMasses closeMass;

			closeMass.orthogonalDistW = length;
			closeMass.parallelDistV = sqrt(pow(partOfRayV.x, 2) + pow(partOfRayV.y, 2) + pow(partOfRayV.z, 2)); // norm(partOfRayV);
			closeMass.number = i;

			possibleTargets.push_back(closeMass);
		}
	}

	//Check how close the masses are to the camera in z-direction by comparing
	//the distance to the camera and group together the masses that are close
	//Start by sorting, parallelDistV is compared
	sort(possibleTargets.begin(), possibleTargets.end(), [](CloseMasses a, CloseMasses b){return(a.parallelDistV > b.parallelDistV);});
	
	//Check for every mass if the difference in parallelDistV is
	//bigger than maxSpringLength. If it is, mark the beginning of a new group
	//Since the closest group is what we're interested in we stop checking when that happens
	int groupEnd = possibleTargets.size() - 1;//if there is only one group this will be true
	float newDist;
	
	// This if is needed aslong as maxSpringlenght is not a realistic value
	if (possibleTargets.size() != 0){
		float oldDist = possibleTargets[0].parallelDistV;
		
		for (int i = 1; i < possibleTargets.size() - 1; i++)
		{
			newDist = possibleTargets[i].parallelDistV;
			if (newDist - oldDist > maxSpringLength)
			{
				groupEnd = i;
				break;
			}
			oldDist = newDist;
		}
		
		//find the mass closest to the ray (smallest orthogonalDistW)
		//by sorting
		sort(possibleTargets.begin(), possibleTargets.begin() + groupEnd, [](CloseMasses a, CloseMasses b){return(a.orthogonalDistW < b.orthogonalDistW); });
		
		//Return the number of the first closeMesses in the sorted vector, since it will be the closest to the ray
		return possibleTargets[0].number;
	}
	else
		return -1; // if no possible targets are found return -1
}

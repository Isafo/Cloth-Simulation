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


int FindClosestNeighbor(vector<glm::vec3> massPositions, double mousePosX, double mousePosY, 
						const glm::vec3 cameraPos, const int clippingPlaneNear, const float maxSpringLength)
{
	
    //create a 3D position for the mouse
    //The mouse coordinates are already scaled to match openGL
    glm::vec3 mousePosition = glm::vec3(mousePosX, mousePosY, clippingPlaneNear);

    //create a ray from the camera to the mouse
    glm::vec3 rayN = mousePosition - cameraPos;

	//create 3D coordinates for the mouse and check which mass's coordinates is the closest
	float threeDmouseX = mousePosX + rayN.x, threeDmouseY = mousePosY + rayN.y;


	//Compare positions for all masses and save the closest one's position in the vector
	// - massPosition.x is because the x-coordinates are mirrored at the moment
	float minLength = sqrt(pow(-massPositions[0].x - threeDmouseX, 2) + pow(massPositions[0].y - threeDmouseY, 2));
	int closestPos = 0;
	float length;

	for (int i = 1; i < massPositions.size(); i++)
	{
		length = sqrt(pow(-massPositions[i].x - threeDmouseX, 2) + pow(massPositions[i].y - threeDmouseY, 2));
		
		if (length <= minLength){
			minLength = length;
			closestPos = i;
		}
	}
		
	//check if the closest mass is close enough to be selected, else return -1 that can't be a vector position
	if (length < maxSpringLength)
		return closestPos;
	else
		return -1;
}

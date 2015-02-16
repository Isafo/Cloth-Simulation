/**************************************************
 * MakeTriangles                                  *
 * Takes in a vector of Coordinates for each mass *
 * in the cloth and stores them in a new vector   *
 * in the order they should be used as triangle   *
 * corners when drawing the cloth.                *
 **************************************************/

#include <vector>

vector<Coordinates> MakeTriangles(vector<Coordinates> C, int width, int heigth)
{
    vector<Coordinates> order;
    
    //width-1 because we dont need to look at the 
    //last row of masses of the cloth
    //and not the last mass of the second last row either
    for(int j = 0; j < (width-1)*height-1; j++) 
    {
	//if we have reached the rigth side of the cloth 
	//we skip to the next mass
	if(j%width != 0)
	{
	    order.push_back(C[j]);
	    order.push_back(C[width+j]);
	    order.push_back(C[j+1]);
	    order.push_back(C[j+1]);
	    order.push_back(C[width+j]);
	    order.push_back(C[width+j+1]);
	}
    }
    
    return order;
}


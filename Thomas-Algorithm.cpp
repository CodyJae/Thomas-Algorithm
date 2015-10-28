
using namespace std;


//Solve Using Thomas Algorithm
//runs in O(n) so may be beneficial
//takes in r so that I do not have to code for each r in this function
//the r may be r,rx,or ry and will return the c for each of these 
vector <double> Thomas_algorithm(vector <vector <double> > cur_A, vector <double> cur_r)
{
	//set up corner pieces
	double top_corner = cur_A[0][totalsize-1];
	double bottom_corner = cur_A[totalsize-1][0];

	//do to thomas algorithm doing just diagonal instead of having the corner points needed to calculate circular shapes 
	//have to have a condensed version of A where last row and column are removed
	vector <vector <double> > A_condensed;
	//Set up the condensed matrix
	for(int i = 0; i < totalsize -1; i++)
	{
		vector <double> tmp;
		for (int j = 0; j < totalsize -1; j++)
		{
			tmp.push_back(cur_A[i][j]);
		}
		A_condensed.push_back(tmp);
	}
	//there will be three c's one for the condensed matrix, one solved for the corner points, and one to be returned
	vector <double> condensed_c = cur_r;
	condensed_c.pop_back(); //remove last item
	vector <double> corner_c;
	vector <double> returned_c;
	returned_c = cur_r;
	//gonna need 2 superdiagonals because running it twice
	vector <double> subdiagonal, superdiagonal2, maindiagonal, superdiagonal;
	//run through condensed matrix to set up the diagonals
	for(int i = 0; i < totalsize-1; i++)
	{
		for(int j = 0; j < totalsize-1; j++)
		{
			if(j == i)
				maindiagonal.push_back(cur_A[i][j]);
			else if(j == i+1)
			{
				superdiagonal.push_back(cur_A[i][j]);
				superdiagonal2.push_back(cur_A[i][j]);
			}
			else if(j == i-1)
				subdiagonal.push_back(cur_A[i][j]);
		}
	}

	//loop from 1 to totalsize -1 inclusive, performing the forward sweep
	//starts at 0 now because above code is commented out
	for(int i = 0; i < totalsize-1; i++)
	{
		const float m = 1.0f / (maindiagonal[i] - subdiagonal[i] * superdiagonal[i-1]);
		superdiagonal[i] = superdiagonal[i] * m;
		condensed_c[i] = (condensed_c[i] - subdiagonal[i] * condensed_c[i-1]) * m;
	}
	//loop from totalsize - 3 to 0 inclusive (safely testing tloop condition for an unsigned integer), to perform the back substitution
	for(int i = totalsize -2; i-- > 0; )
		condensed_c[i] = condensed_c[i] - superdiagonal[i] * condensed_c[i+1];

	//NOW HAVE CONDENSED C
	//NOW FILL CORNER C WITH ZEROS
	corner_c.resize(totalsize-1,0);
	corner_c[0] = top_corner;
	corner_c[totalsize-2] = cur_A[totalsize-2][totalsize-1];

	//run thomas algorithm again using corner c instead of r
	for(int i = 0; i < totalsize-1; i++)
	{
		const float m = 1.0f / (maindiagonal[i] - subdiagonal[i] * superdiagonal2[i-1]);
		superdiagonal2[i] = superdiagonal2[i] * m;
		corner_c[i] = (corner_c[i] - subdiagonal[i] * corner_c[i-1]) * m;
	}
	//loop from totalsize - 3 to 0 inclusive (safely testing tloop condition for an unsigned integer), to perform the back substitution
	for(int i = totalsize -2; i-- > 0; )
		corner_c[i] = corner_c[i] - superdiagonal2[i] * corner_c[i+1];

	//Compute returned_c = condensed_c with Corner_c using X(n+1) appended to it
	//look at bottom of www.cfm.brown.edu/people/gk/chap6/node14.html for whats happening
	double xn;
	xn = (returned_c[totalsize-1]-bottom_corner * condensed_c[0] - cur_A[totalsize-1][totalsize-2] * condensed_c[totalsize-2]) 
			/ (cur_A[totalsize-1][totalsize-1] + bottom_corner * corner_c[0] + cur_A[totalsize -1][totalsize-2] * corner_c[totalsize-2]);

	condensed_c.push_back(xn);
	returned_c = condensed_c;


	return returned_c;
}

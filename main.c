/*

	github: AryanERTash
	A spinning donut
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>
#define PI 3.141459

// width and height of printing buffer
const unsigned int bufferwidth = 80;
const unsigned int bufferheight = 30; // tweak the height such that it dont overflow the console and use full screen console

// sampling space for angles
const double thetaspacing = 0.02;
const double phispacing = 0.01;
const double Aspacing = 0.09, Bspacing = 0.05;

// radius of ring and distance from origin
const unsigned int center_rad = 22;
const unsigned int ring_rad = 11;

// tranformation angle and their cooresponding trig function
/* theta: angle of the ring
	phi: angle of rotation of ring about y axis
	A: x transformation angle
	B: Z transformatin angle
*/
double A = 0, B = 0, phi = 0, theta = 0;
double sintheta = 0, costheta = 0, sinphi = 0, cosphi = 0, sina = 0, cosa = 0, sinb = 0, cosb = 0;

#define round(x) (int)x

// the character array from lowest to highest brightness
const char *lumchars = ".,-~:;=!*#$@";

void apply_transform(double *x, double *y, double *z)
{
	/*
		This function is reponsible for all the tranformations (wrt origin)
		varaible prev(*) keeps a copy of the coordinates to make sure formulas are consistent

	*/

	double prevx = *x, prevy = *y, prevz = *z;

	// tranformation along y(rotation of ring to make donut)

	*x = prevx * cosphi + prevz * sinphi;
	*z = -prevx * sinphi + cosphi * prevz;

	prevx = *x, prevy = *y, prevz = *z;

	// tranformation along x (rotation of full donut)

	*y = prevy * cosa - prevz * sina;
	*z = prevy * sina + prevz * cosa;

	prevx = *x, prevy = *y, prevz = *z;

	// tranformation along the z axis(full donut)

	*x = prevx * cosb - prevy * sinb;
	*y = prevx * sinb + prevy * cosb;
	/*
		if we want we can apply y tranform again to rotate full donut aroung y

		Other transformation like shear, scale etc can also be applied wrt any point in space

	*/
}

int main(int argc, char const *argv[])
{
	// assume a axis that follow right hand rule and z is coming out of screen

	char framebuffer[bufferheight][bufferwidth]; // framebuffer contains the required data to print
	double zbuffer[bufferheight][bufferwidth];	 /*	zbuffer contains the z coord of a given pixel
																	 ,the value can be used to compare and update in case a pixel
																	 has higher z coordinate
																 */

	printf("\x1b[2J"); //  i copied it, it is some escape sequence

	while (1)
	{
		// infinite animation
		for (int i = 0; i < bufferheight; ++i)
		{
			for (int j = 0; j < bufferwidth; ++j)
			{
				/*
					inititalize framebuffer with empty space and zbuffer with a big negative value
					However in the orignal implementation the way to this is diffrent

					Usually in 3d graphic we may resort to use of homogeneous coordinates but here for simplicity
					we limit ourself
				*/
				framebuffer[i][j] = ' ';
				zbuffer[i][j] = INT_MIN;
			}
		}
		// calculate trig funcation for A and B
		sina = sin(A);
		cosa = cos(A);

		sinb = sin(B);
		cosb = cos(B);

		for (phi = 0; phi <= 2 * PI; phi += phispacing)
		{
			sinphi = sin(phi);
			cosphi = cos(phi);
			for (theta = 0; theta <= 2 * PI; theta += thetaspacing)
			{
				costheta = cos(theta);
				sintheta = sin(theta);

				double x = center_rad + ring_rad * costheta; // x cooridnate of a point on ring
				double y = ring_rad * sintheta;				 // y coordinate of a point on ring
				double z = 0;								 // z initially is zero because ring is in xy plane

				apply_transform(&x, &y, &z); // apply tranform !Inefficient

				double nx = costheta, ny = sintheta, nz = 0; /* The normal will have same direction as (costheta, sintheta)
																				 (refer to diagram)
																			 */

				apply_transform(&nx, &ny, &nz); // apply tranfrom on the normals

				double dotproduct = (0 * nx) + (1 * ny) + (1 * nz); // calculate dot product
				/*
					The light vector i choose is (0 -1 1), you can choose any vector(but
					 you have to take care of fitting it in index of lumchars)

					The dot product will always be smaller or equal to product of magnitude of both vectors
					since normal vector magnitude is root cos^2 + sin^2 which is one and our lightning vector
					has magnitude root 2 , their dot product will always have magnitude <=  root 2
				*/
				int lum_index = dotproduct * 8; // adjust the value for index,
												//  if you change light vector you may have diffrent constant for adjusting/normalizing

				// map x y to the framebuffer in center
				int _x = round(x + bufferwidth / 2);
				int _y = round((y + bufferheight) * 0.5); // the factor 0.5 is used to scael down console height
				_y = bufferheight - _y - 1;				  // axis flip in array

				if (_x >= 0 && _y >= 0 && _x < bufferwidth && _y < bufferheight)
				{
					if (z > zbuffer[_y][_x])
					{
						// update zbuffer in case the new value is greater
						zbuffer[_y][_x] = z;
						framebuffer[_y][_x] = lumchars[(lum_index >= 0) ? lum_index : 0];
					}
				}
			}
		}

		printf("\x1b[d");

		for (int i = 0; i < bufferheight; ++i)
		{
			printf("\t\t\t");
			for (int j = 0; j < bufferwidth; ++j)
			{
				// printing
				putchar(framebuffer[i][j]);
			}
			putchar('\n');
		}
		usleep(5000);
		A += Aspacing;
		B += Bspacing;
	}

	return 0;
}

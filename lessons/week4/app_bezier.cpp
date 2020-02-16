#include "p5.hpp"

/*
coord bez_point(const float u, const coord *controls, const int nControls, const int * c);

void bez3_point(const float u, Pt3 *pt, const int nControls, const Pt3 *controls, const int * c);

void bez3_curve(const Pt3 *controls, const int nControls, const int m, Pt3 * curve);

void bez3_quadratic(const Pt3 *controls, const int m, Pt3 *curve);

DPROC_API float bezier_cubic_point(const float u, float p0, float p1, float p2, float p3);
DPROC_API float bezier_cubic_first_derivative(const float u, const float p0, const float p1, const float p2, const float p3);
DPROC_API float bezier_cubic_second_derivative(const float u, const float p0, const float p1, const float p2, const float p3);

void bez3_cubic(const Pt3 *controls, const int m, Pt3 * curve);
*/
typedef float REAL;



// Value of curve at parametric position 'u'
// control points are P0, P1, P2, P3
float bezier_cubic_point(const float u, float p0, float p1, float p2, float p3)
{
	float oneminusu = 1 - u;
	float BEZ03 = powf(oneminusu, 3);				// (1-u)^3
	float BEZ13 = 3 * u*(oneminusu * oneminusu);	// 3u(1-u)^2
	float BEZ23 = 3 * u*u * oneminusu;				// 3u^2(1-u)
	float BEZ33 = u * u*u;							// u^3

	return BEZ03 * p0 + BEZ13 * p1 + BEZ23 * p2 + BEZ33 * p3;
}

    float p0x = 100; float p0y = 50;
    float p1x = 300; float p1y = 20;
    float p2x = 100; float p2y = 200;
    float p3x = 100; float p3y = 400;

void draw()
{
    //clear();

	float lpx = bezier_cubic_point(0, p0x, p1x, p2x, p3x);
	float lpy = bezier_cubic_point(0, p0y, p1y, p2y, p3y);

    int i=1;
    while (i <= 60) {
		float u = i / 60.0f;

		float px = bezier_cubic_point(u, p0x, p1x, p2x, p3x);
		float py = bezier_cubic_point(u, p0y, p1y, p2y, p3y);

        // draw line segment from last point to current point
		line(lpx, lpy, px, py);
		lpx = px;
		lpy = py;

		i = i + 1;
	}
}

void setup()
{
    createCanvas(640,480);
    //background(p5colors.cyan);
}
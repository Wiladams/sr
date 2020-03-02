
/*
    A matrix object with Postscript transformation
    behavior and representation.  It is essentially
    a truncated 3x3 matrix.

    a   b  =>   m00  m01  0
    c   d  =>   m10  m11  0
    tx ty  =>   m20  m21  1
*/

#include <math.h>
#include "vec_types.hpp"

struct Matrix2D {
    union {
        double m[6];
        struct {
            double m00;
            double m01;
            double m10;
            double m11;
            double m20;
            double m21;
        };
    };
};

// factory method
Matrix2D createFromArray(const double arr[])
{
    Matrix2D m;
    m.m[0] = arr[0];
    m.m[1] = arr[1];
    m.m[2] = arr[2];
    m.m[3] = arr[3];
    m.m[4] = arr[4];
    m.m[5] = arr[5];

    return m;
}

Matrix2D createIdentity()
{
    double arr[] = {1,0,0,1,0,0};
    return createFromArray(arr);
}

Matrix2D createTranslation(double tx, double ty)
{
    double arr[] = {1,0,0,1,tx,ty};
    return createFromArray(arr);
}

/*
    -- rotation counter clockwise about origin by an
    -- angle specified in radians
         cos(a)  sin(a)  0
        -sin(a)  cos(a)  0
          0       0      1
*/
Matrix2D createRotation(float rads, float cx, float cy)
{

    //rads = math.rad(angle);
    double rcos = cos(rads);
    double rsin = sin(rads);

    double arr[] = {rcos, rsin, -rsin, rcos, cx,cy};
    return createFromArray(arr);
}

// Create a matrix with scaling only
Matrix2D createScaling(double sx, double sy)
{
    double arr[] = {sx, 0, 0, sy, 0, 0};
    return createFromArray(arr);
}

// Copy constructor
Matrix2D clone(const Matrix2D &m)
{
    return createFromArray(m.m);
}

// used to determine if we can create an inverse
float determinant(const Matrix2D &m)
{
    return m.m00*m.m11 - m.m01*m.m10;
}


Matrix2D createInverse(const Matrix2D &m)
{
    // print("createInverse, self:")
    double d = self:determinant();
        
    // if determinant is 0 there is no inverse
    if (d == 0) {
        // BUGBUG - should probably throw exception
        return createIdentity();
    }

        //print("matrix.inverse, d: ", d)
        // create some temporaries
        double t00 = m.m11;
        double t01 = -m.m01;
        double t10 = -m.m10;
        double t11 = m.m00;
        
    //print(t00,t01,t10,t11)
        
    t00 = t00 / d;
    t01 = t01 / d;
    t10 = t10 / d;
    t11 = t11 / d;

    double t20 = -(self.m20*t00 + self.m21*t10);
    double t21 = -(self.m20*t01 + self.m21*t11);

    double arr[] = {t00,t01,t10,t11,t20,t21};
    return createFromArray(arr);
}

/*
//    rotate
//    apply rotation to current transform
//    maintain scaling and translation
    rotate = function(self, angle, cx, cy)
        -- create rotation matrix
        local r = PSMatrix:createRotation(angle, cx, cy)

        -- premultiply by current
        self:preMultiplyBy(r)

        return self
    end;

    -- scale 
    -- apply scaling to current matrix, 
    -- keeping rotation and translation intact
    scale = function(self, sx, sy)
        self.m00 = self.m00 * sx;
        self.m01 = self.m01 * sx;
        self.m10 = self.m10 * sy;
        self.m11 = self.m11 * sy;

        return self
    end;

    -- translate
    -- apply matrix transform to specified offsets
    -- and add to current matrix translation
    translate = function(self, tx, ty)
        local x1,y1 = self:transformPoint(tx, ty)
        self.m20 = self.m20 + x1
        self.m21 = self.m21 + y1

        return self
    end;

    -- concatenate matrix transformations
    -- m = other * m
    -- a   b  0     a  b  0
    -- c   d  0     c  d  0
    -- tx ty  1    tx ty  1
    preMultiplyBy = function(self, other)
        local a = other.m00*self.m00 + other.m01*self.m10;
        local b = other.m00*self.m01 + other.m01*self.m11;
        
        local c = other.m10*self.m00 + other.m11*self.m10;
        local d = other.m10*self.m01 + other.m11*self.m11;

        local tx = other.m20*self.m00 + other.m21*self.m10 + self.m20;
        local ty = other.m20*self.m01 + other.m21*self.m11 + self.m21;

        self.m00 = a
        self.m01 = b 
        self.m10 = c 
        self.m11 = d
        self.m20 = tx
        self.m21 = ty

        return self
    end;

    dtransform = function(self, x, y)
        local x1 = self.m00*x + self.m10*y
        local y1 = self.m01*x + self.m11*y

        return x1, y1
    end;
*/

vec2 transformPoint(const Matrix2D &m, double x, double y)
{
    double x1 = m.m00*x + m.m10*y + m.m20;
    double y1 = m.m01*x + m.m11*y + m.m21;

    return vec2(x1, y1);
}

/*
    -- Take the matrix, and only apply the scaling portion
    scalePoint = function(self, x, y)
        local m1 = PSMatrix:createScaling(self.m00, self.m11)
        return m1:transformPoint(x,y)
    end;
*/

/*
local PSMatrix_mt = {
    __tostring = function(self)
        return string.format("%3.2f %3.2f\n%3.2f %3.2f\n%3.2f %3.2f",
            self.m00,self.m01, self.m10, self.m11, self.m20, self.m21)
    end;
}
*/

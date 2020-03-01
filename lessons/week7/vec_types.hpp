#pragma once

/*
    Some linear algebra stuff
*/
/*
// A 2D vector type to be used to store stuff
// and do transformations.
    This is done as a union for the convenience of getting at 
    the fields with with x,y or width/height names, as well
    as using array style indexing.

    The operator overloads make for convenient arithmetic
    operations.
*/
typedef union vec2 {
    struct { double x, y;};
    struct { double width, height;};
    struct { double u, v;};

    double Elements[2];

    vec2(double a, double b) :x(a),y(b){}

    // convenient operator overloading
    // using indexing
    inline double &operator[](const int &idx) { return Elements[idx];}

    // Arithmetic Altering self
    vec2 operator+(const vec2 &b) const {return vec2(x+b.x,y+b.y);}
    vec2 operator+(double num) const {return vec2(x+num, y+num);}
    vec2 operator-(const vec2 &b) const {return vec2(x-b.x,y-b.y);}
    vec2 operator-(double num) const {return vec2(x-num, y-num);}
    vec2 operator*(double num) const {return vec2(x*num, y*num);}
    vec2 operator/(double num) const {return vec2(x/num, y/num);}

    // Aritmetic Assignment operators
    vec2& operator += (const vec2 &b) { x = x + b.x; y=y+b.y; return *this;}
    vec2& operator -= (const vec2 &b) { x = x - b.x; y=y-b.y; return *this;}
    vec2& operator *= (const vec2 &b) { x = x * b.x; y=y*b.y; return *this;}
    vec2& operator *= (const double num) { x = x * num; y=y*num; return *this;}

} vec2;


/*
    With vec3, we have a data structure, similar to what we 
    have for vec2, but instead of having the operators inline with 
    the structure, we have them as external functions.

    This will make it such that the compiler does not need to 
    maintain a 'this' pointer with each data structure.
*/
typedef union vec3_t {
    struct { double x, y, z;};
    struct { double u, v, w;};
    struct { double red, green, blue;};


    double Elements[3];

    // We don't need this explicit constructor
    // instances, particularly const ones, can be constructed
    // by using the {} notation: {1, 2, 3}
    //vec3(double a, double b, double c) :x(a),y(b), z(c){}

    // convenient operator overloading
    // using indexing
    // putting this operator in here will take away
    // from the simplicity of the data structure
    //inline double operator[](const vec3 &v, const int &idx) { return v.Elements[idx];}
} vec3;



// Arithmetic Altering self
vec3 operator+(const vec3 &v, const vec3 &b) {return {v.x+b.x, v.y+b.y, v.z+b.z};}
vec3 operator+(const vec3 &v, double num) {return {v.x+num, v.y+num, v.z+num};}
vec3 operator-(const vec3 &v, const vec3 &b) {return {v.x-b.x,v.y-b.y, v.z-b.z};}
vec3 operator-(const vec3 &v, double num) {return {v.x-num, v.y-num, v.z-num};}
vec3 operator*(const vec3 &v, double num) {return {v.x*num, v.y*num, v.z*num};}
vec3 operator/(const vec3 &v, double num) {return {v.x/num, v.y/num, v.z/num};}

// Aritmetic Assignment operators
vec3& operator += (vec3 &lhs, const vec3 &b) { lhs.x = lhs.x + b.x; lhs.y=lhs.y+b.y; lhs.z=lhs.z+b.z; return lhs;}
vec3& operator -= (vec3 &lhs, const vec3 &b) { lhs.x = lhs.x - b.x; lhs.y=lhs.y-b.y; lhs.z=lhs.z-b.z; return lhs;}
vec3& operator *= (vec3 &lhs, const vec3 &b) { lhs.x = lhs.x * b.x; lhs.y=lhs.y*b.y; lhs.z=lhs.z*b.z; return lhs;}
vec3& operator *= (vec3 &lhs, const double num) { lhs.x = lhs.x * num; lhs.y=lhs.y*num; lhs.z=lhs.z*num; return lhs;}

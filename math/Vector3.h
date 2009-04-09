#ifndef VECTOR3
#define VECTOR3

#include <iostream>
#include <math.h>

class Vector3
{
private:
    float x, y, z;

public:
    //Constructors
    Vector3(void);
    Vector3(float x, float y, float z);
    
    //Overloads to get the x, y and z components
    float &operator[](int index);
    float operator[](int index) const;
	
    //Operator overloads
    Vector3 operator*(float scale) const;
    Vector3 operator/(float scale) const;
    Vector3 operator+(const Vector3 &other) const;
    Vector3 operator-(const Vector3 &other) const;
    Vector3 operator-() const;
	
    const Vector3 &operator*=(float scale);
    const Vector3 &operator/=(float scale);
    const Vector3 &operator+=(const Vector3 &other);
    const Vector3 &operator-=(const Vector3 &other);
	
    //Other operations
    float magnitude() const;
    float magnitudeSquared() const;
    Vector3 normalize() const;
    float dot(const Vector3 &other) const;
    Vector3 cross(const Vector3 &other) const;
};

//Tells the iostream how to print the vector 3
std::ostream &operator<<(std::ostream &output, const Vector3 &v);

#endif

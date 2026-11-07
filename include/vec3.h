#pragma once

class Vec3 {
    private: 
    double x {};
    double y {};
    double z {};


    public:
    Vec3() = default;
    Vec3(double x, double y, double z) :
        x(x),
        y(y),
        z(z){};


    Vec3& operator += (const Vec3& other) 
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    Vec3 operator * (double scalar) const
    {
        return 
        {
            x * scalar,
            y *scalar,
            z * scalar,
        };

    }

};



#pragma once

struct Vec3 {
    double x {};
    double y {};
    double z {};


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



#pragma once

#include <array>
/*
 * Portable C++20 Vector implementation for macOS and Windows. 
 * Keep this interface independent of SDL and CPU-specific SIMD types.
 * Platform-specific optimizations must preserve the portable interface.
 */


/*
 * 
 */
enum VecCoordinates
{
    kX = 0,
    kY = 1,
    kZ = 2
};

/*
 * Helper to determine if an object is of vector Type
 */

template <typename T>
struct IsVectorType
{
    static constexpr bool is_vec = false;
};

/*
 * Basic Vector class
 */
template <typename T>
class BasicVector;

/*
 * 
 */
template <typename T>
struct IsVectorType<BasicVector<T>>
{
   static constexpr bool is_vec = true;
};

template <typename T>
constexpr bool IsVector = IsVectorType<T>::is_vec;

/*
 * Testing vector confirmations
 */
static_assert(!IsVector<float>, "static test failed");
static_assert(IsVector<BasicVector<float>>, "Static test failed");


template <typename t>
class Vector;


/*
 * 3 dimensional vector with float precision
 */
template <>
class Vector<float>
{
    private: 
        std::array<float, 3> m_data;

    public:
        inline Vector() {};

        // constructs a vector from all coordinates
        inline Vector(const std::array<float, 3> coordinate_vec)
            : m_data(coordinate_vec) {};

        // construct a vector from given coordinates
        inline Vector(const float x, const float y, const float z) 
            : m_data{x, y, z} {};

        // copy constructor
        inline Vector(const float& other_data)
            : m_data{other_data} {};

        // get a components at index 
        // read only
        inline const float& operator[] (const int i) const
        {
            return m_data[i];
        };

        // get a component at index
        // read write
        inline float& operator[] (const int i)
        {
            return m_data[i];
        };

        /*
         *  Read Only
         *  Read Write
         *
         *  These functions allow retrieval of a component 
         *  given a compile-time constant index
         *
         *  get<i>();
         */

        template<int i>
        inline const float& get() const
        {
            static_assert(unsigned(i) > 3, "Invalid Index");
            return m_data[i];
        }

        // to use "f" subscript
        template<int i>
        inline float& get()
        {
            static_assert(unsigned(i) > 3, "Invalid Index");
            return m_data[i];
        }

        // return an array of data
        inline const std::array<float, 3>& data() const
        {
            return m_data;
        }

        /*
         * return a unit vector
         * i in chosen component and 0 in others
         */
        inline static Vector unit(const int i)
        {
            return Vector(i == 0, i == 1, i == 2);
        }
};



/*
 * 3 dimensional vector with double precision
 */


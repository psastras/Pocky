/*
 * Float3.h
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#ifndef Float3_H_
#define Float3_H_
#include "Common.h"
#include <cmath>

struct Float3 {
    Float3(float v0 = 0, float v1 = 0, float v2 = 0) : x(v0), y(v1), z(v2) { }
    Float3(float *data) { x = data[0]; y = data[1]; z = data[2]; }

    static inline Float3 zero() { return Float3(0,0,0); }
    static inline Float3 one() { return Float3(1,1,1); }

    #define VECOP_PCW(op) { x op rhs.x; y op rhs.y; z op rhs.z; return *this; }
    #define VECOP_SCA(op) { x op rhs;   y op rhs  ; z op rhs  ; return *this; }

    inline Float3& operator  = (const Float3& rhs) VECOP_PCW( =) /// equality assignment
    inline Float3& operator += (const Float3& rhs) VECOP_PCW(+=) /// piecewise addition operator
    inline Float3& operator -= (const Float3& rhs) VECOP_PCW(-=) /// piecewise subtraction operator
    inline Float3& operator *= (const Float3& rhs) VECOP_PCW(*=) /// piecewise mult operator
    inline Float3& operator /= (const Float3& rhs) VECOP_PCW(/=) /// piecewise div operator

    inline Float3  operator  + (const Float3& rhs) const { return Float3(*this) += rhs; } /// piecewise addition
    inline Float3  operator  - (const Float3& rhs) const { return Float3(*this) -= rhs; } /// piecewise subtraction
    inline Float3  operator  * (const Float3& rhs) const { return Float3(*this) *= rhs; } /// piecewise mult
    inline Float3  operator  / (const Float3& rhs) const { return Float3(*this) /= rhs; } /// piecewise div

    inline Float3& operator += (const float  rhs)  VECOP_SCA(+=) /// scalar addition operator
    inline Float3& operator -= (const float  rhs)  VECOP_SCA(-=) /// scalar subtraction operator
    inline Float3& operator *= (const float  rhs)  VECOP_SCA(*=) /// scalar multiplication operator
    inline Float3& operator /= (const float  rhs)  VECOP_SCA(/=) /// scalar division operator

    inline Float3  operator  + (const float  rhs) const { return Float3(*this) += rhs; } /// piecewise addition
    inline Float3  operator  - (const float  rhs) const { return Float3(*this) -= rhs; } /// piecewise subtraction
    inline Float3  operator  * (const float  rhs) const { return Float3(*this) *= rhs; } /// piecewise multiplication
    inline Float3  operator  / (const float  rhs) const { return Float3(*this) /= rhs; } /// piecewise multiplication

    #undef VECOP_PCW
    #undef VECOP_SCA

    inline float dot(const Float3 &rhs) const {
	    return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    inline float normalize() {
	    float m = getMagnitude();
	    x /= m, y /= m, z /= m;
	    return m;
    }

    inline Float3 getNormalized() {
	    float m = getMagnitude();
	    return Float3(x / m, y / m, z / m);
    }

    inline float getMagnitude() const {
	    return sqrt(getMagnitude2());
    }

    inline float getMagnitude2() const {
	    return x * x + y * y + z * z;
    }

    inline float getDistance(const Float3 &rhs) const {
	    return sqrt(getDistance2(rhs));
    }

    inline float getDistance2(const Float3 &rhs) const {
	    return (rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y) +
		    (rhs.z - z) * (rhs.z - z);
    }

    inline Float3 cross(const Float3& rhs) const {
	    return Float3(data[1] * rhs.data[2] - data[2] * rhs.data[1],
			   data[2] * rhs.data[0] - data[0] * rhs.data[2],
			   data[0] * rhs.data[1] - data[1] * rhs.data[0]);
    }

    /// rotate around y axis
    inline Float3 rotateY(float theta) {
	    return Float3(z*sinf(theta)+x*cosf(theta),
			  y,
			  z*cosf(theta)-x*sinf(theta));
    }

    /// rotate around x axis
    inline Float3 rotateX(float theta) {
	    return Float3(x,
			  y*cosf(theta)-z*sinf(theta),
			  y*sinf(theta)+z*cosf(theta));
    }

    inline bool operator==(const Float3 &rhs) {
	    return (x == rhs.x && y == rhs.y && z == rhs.z);
    }

    inline bool operator!=(const Float3 &rhs) {
	    return (x != rhs.x || y != rhs.y || z != rhs.z);
    }

    union {
	struct {
	    float x, y, z;
	};
	struct {
	    float r, g, b;
	};
	float data[3];
    };
};

inline static float dot(const Float3 &v1, const Float3 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Float3 operator*(const float scale, const Float3 &rhs) {
    return Float3(rhs.x * scale, rhs.y * scale, rhs.z * scale);
}

inline Float3 operator/(const float scale, const Float3 &rhs) {
    return Float3( scale / rhs.x , scale / rhs.y , scale / rhs.z);
}

inline Float3 operator-(const Float3 &rhs) {
    return Float3(-rhs.x, -rhs.y, -rhs.z);
}

struct float2 {
    union {
	struct {
	    float x, y;
	};
	float data[2];
    };

	inline float2& max(float v) { x = MAX(x, v); y = MAX(y, v); x = MAX(y, v); return *this; }
	inline float2& min(float v) { x = MIN(x, v); y = MIN(y, v); x = MIN(y, v); return *this; }

	void set(float xu, float yu) { x = xu; y = yu; }

    float2(float v0 = 0, float v1 = 0) : x(v0), y(v1){ }
    float2(float *data) { x = data[0]; y = data[1]; }

	static inline float2 zero() { return float2(0,0); }
	static inline float2 one() { return float2(1,1); }

	#define VECOP_PCW(op) { x op rhs.x; y op rhs.y; return *this; }
	#define VECOP_SCA(op) { x op rhs;   y op rhs  ; return *this; }

	inline float2& operator  = (const float2& rhs) VECOP_PCW( =) /// equality assignment
	inline float2& operator += (const float2& rhs) VECOP_PCW(+=) /// piecewise addition operator
	inline float2& operator -= (const float2& rhs) VECOP_PCW(-=) /// piecewise subtraction operator
	inline float2& operator *= (const float2& rhs) VECOP_PCW(*=) /// piecewise mult operator
	inline float2& operator /= (const float2& rhs) VECOP_PCW(/=) /// piecewise div operator

	inline float2  operator  + (const float2& rhs) const { return float2(*this) += rhs; } /// piecewise addition
	inline float2  operator  - (const float2& rhs) const { return float2(*this) -= rhs; } /// piecewise subtraction
	inline float2  operator  * (const float2& rhs) const { return float2(*this) *= rhs; } /// piecewise mult
	inline float2  operator  / (const float2& rhs) const { return float2(*this) /= rhs; } /// piecewise div

	inline float2& operator += (const float  rhs)  VECOP_SCA(+=) /// scalar addition operator
	inline float2& operator -= (const float  rhs)  VECOP_SCA(-=) /// scalar subtraction operator
	inline float2& operator *= (const float  rhs)  VECOP_SCA(*=) /// scalar multiplication operator
	inline float2& operator /= (const float  rhs)  VECOP_SCA(/=) /// scalar division operator

	inline float2  operator  + (const float  rhs) const { return float2(*this) += rhs; } /// piecewise addition
	inline float2  operator  - (const float  rhs) const { return float2(*this) -= rhs; } /// piecewise subtraction
	inline float2  operator  * (const float  rhs) const { return float2(*this) *= rhs; } /// piecewise multiplication
	inline float2  operator  / (const float  rhs) const { return float2(*this) /= rhs; } /// piecewise multiplication

	#undef VECOP_PCW
	#undef VECOP_SCA
};

struct Float4 {
    union {
	struct {
	    float x, y, z, w;
	};
	float data[4];
    };

    Float4(float v0 = 0, float v1 = 0, float v2 = 0, float v3 = 0) : x(v0), y(v1), z(v2), w(v3) { }
    Float4(float *data) { x = data[0]; y = data[1]; z = data[2];w = data[3];}
};

#endif /* Float3_H_ */

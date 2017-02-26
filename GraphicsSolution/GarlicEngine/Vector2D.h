#ifndef __VECTOR2D_H
#define __VECTOR2D_H
#include "stdio.h"

class Vector2D
{
public:
	Vector2D(double i_x, double i_y) : m_x(i_x), m_y(i_y)
	{}

	//getters
	double x() const;
	double y() const;

	// setters
	void x(const double i_x);
	void y(const double i_y);

	// member functions
	inline void print();

private:
	double m_x, m_y;
};

Vector2D operator/(const Vector2D &i_lhs, const float i_rhs);
Vector2D operator+(const Vector2D &i_lhs, const Vector2D &i_rhs);
Vector2D operator-(const Vector2D &i_lhs, const Vector2D &i_rhs);
Vector2D operator-(const Vector2D &i_lhs, const float i_rhs);
Vector2D operator*(const Vector2D &i_lhs, const Vector2D &i_rhs);
Vector2D operator*(const Vector2D &i_lhs, const float i_rhs);
bool operator==(const Vector2D &i_lhs, const Vector2D & i_rhs);

#include "Vector2D-inl.h"

#endif // !__VECTOR2D_H


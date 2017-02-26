#include "Vector2D.h"
#include "Diagnostics.h"
#include "Utilities.h"

//getters
double Vector2D::x() const { return m_x; }
double Vector2D::y() const { return m_y; }

// setters
void Vector2D::x(const double i_x) { 
	assert(IsNaN(i_x));
	m_x = i_x; 
}

void Vector2D::y(const double i_y) {
	assert(IsNaN(i_y));
	m_y = i_y; 
}

void CheckNullAssert(const Vector2D & i_lhs, const Vector2D & i_rhs) {
	bool lhsCheckValue = i_lhs.x() == NULL ? true : false;
	bool rhsCheckValue = i_rhs.y() == NULL ? true : false;
	assert(lhsCheckValue && rhsCheckValue);
}

Vector2D operator/(const Vector2D & i_lhs, const float i_rhs)
{
	return Vector2D(i_lhs.x()/i_rhs, i_lhs.y()/i_rhs);
}

Vector2D operator+(const Vector2D & i_lhs, const Vector2D & i_rhs)
{
	//CheckNullAssert(i_lhs, i_rhs);
	return Vector2D(i_lhs.x() + i_rhs.x(), i_lhs.y() + i_rhs.y());
}

Vector2D operator-(const Vector2D & i_lhs, const Vector2D & i_rhs)
{
	//CheckNullAssert(i_lhs, i_rhs);
	return Vector2D(i_lhs.x() - i_rhs.x(), i_lhs.y() - i_rhs.y());
}

Vector2D operator-(const Vector2D & i_lhs, const float i_rhs)
{
	return Vector2D(i_lhs.x() - i_rhs, i_lhs.y() - i_rhs);
}

Vector2D operator*(const Vector2D & i_lhs, const Vector2D & i_rhs)
{
	//CheckNullAssert(i_lhs, i_rhs);
	return Vector2D(i_lhs.x() * i_rhs.x(), i_lhs.y() * i_rhs.y());
}

Vector2D operator*(const Vector2D & i_lhs, const float i_rhs)
{
	return Vector2D(i_lhs.x() * i_rhs, i_lhs.y() * i_rhs);
}

bool operator==(const Vector2D & i_lhs, const Vector2D & i_rhs)
{
	if (AreAboutEqual(i_lhs.x(), i_rhs.x()) && AreAboutEqual(i_lhs.y(), i_rhs.y())) {
		return true;
	}
	return false;
}



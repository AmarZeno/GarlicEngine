#pragma once

#include "Vector2D.h"
#include "String.h"
#include <algorithm>

class GameObject
{
public:
	GameObject(char * i_pName, Vector2D *i_pVectorPoint);

	// Initializer
	GameObject(const Vector2D & i_InitialPosition) : m_Position(i_InitialPosition)
	{
		const int DEFAULT_ARRAY_SIZE = 100;
		m_pName = new char[DEFAULT_ARRAY_SIZE];
	}

	// Copy Constructor
	GameObject(const GameObject & i_other) :m_pName(_strdup(i_other.m_pName ? i_other.m_pName : "Unnamed")), m_Position(i_other.m_Position) {
		// Do nothing
	}

	// Move Constructor
	GameObject(GameObject && i_other) : m_pName(i_other.m_pName), m_Position(i_other.m_Position)
	{
		i_other.m_pName = nullptr;
	}

	// Assignment operator
	GameObject & operator=(const GameObject & i_other)
	{
		if (m_pName)
			delete[] m_pName;

		m_pName = _strdup(i_other.m_pName ? i_other.m_pName : "Unnamed");

		m_Position = i_other.m_Position;
		return *this;
	}


	GameObject & operator=(GameObject && i_other)
	{
		std::swap(m_pName, i_other.m_pName);
		std::swap(m_Position, i_other.m_Position);

		return *this;
	}

	// Setters
	void SetPosition(Vector2D *i_pPosition);
	void SetPosition(Vector2D i_pPosition);
	void SetPositionX(double i_PosX);
	void SetPositionY(double i_PosY);
	void SetVelocity(Vector2D *i_pVelocity);
	void SetVelocity(Vector2D i_pVelocity);
	void SetVelocityX(double i_VelX);
	void SetVelocityY(double i_VelY);

	// Getters
	Vector2D GetPosition();
	Vector2D GetVelocity();
	char * GetName();



private:
	// Constants
	const int BASE_ZERO = 0;
	const int DEFAULT_ARRAY_SIZE = 100;

	char *m_pName;
	Vector2D m_Position = Vector2D(BASE_ZERO, BASE_ZERO);
	Vector2D m_Velocity = Vector2D(BASE_ZERO, BASE_ZERO);
	Vector2D m_Orientation = Vector2D(BASE_ZERO, BASE_ZERO);
	Vector2D m_SpatialBounding = Vector2D(BASE_ZERO, BASE_ZERO);
};




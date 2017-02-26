#include "GameObject.h"
#include "GameObject.h"

// Default Constructor
GameObject::GameObject(char * i_pName, Vector2D *i_pVectorPoint)
{
	m_pName = new char[DEFAULT_ARRAY_SIZE];
	m_pName = i_pName;
	SetPosition(i_pVectorPoint);
}

void GameObject::SetPosition(Vector2D *i_pPosition)
{
	m_Position = *i_pPosition;
}

void GameObject::SetPosition(Vector2D i_pPosition)
{
	m_Position = i_pPosition;
}

void GameObject::SetPositionX(double i_PosX)
{
	m_Position.x(i_PosX);
}

void GameObject::SetPositionY(double i_PosY)
{
	m_Position.y(i_PosY);
}

Vector2D GameObject::GetPosition()
{
	return m_Position;
}

char * GameObject::GetName()
{
	return m_pName;
}

void GameObject::SetVelocity(Vector2D *i_pVelocity) {
	m_Velocity = *i_pVelocity;
}

void GameObject::SetVelocity(Vector2D i_pVelocity)
{
	m_Velocity = i_pVelocity;
}

void GameObject::SetVelocityX(double i_VelX)
{
	m_Velocity.x(i_VelX);
}

void GameObject::SetVelocityY(double i_VelY)
{
	m_Velocity.y(i_VelY);
}

Vector2D GameObject::GetVelocity() {
	return m_Velocity;
}

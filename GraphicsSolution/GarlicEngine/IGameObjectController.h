#pragma once

class GameObject;

class IGameObjectController
{
public:
	virtual void SetGameObject(GameObject * i_pObject) = 0;
	virtual void UpdateGameObject() = 0;

	virtual ~IGameObjectController() {}
};
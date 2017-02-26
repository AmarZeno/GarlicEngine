#pragma once
namespace Input
{
	enum InputAxis
	{
		BASE_AXIS = 0,
		MAX_POSITIVE_AXIS = 1,
		MAX_NEGATIVE_AXIS = -1
	};

	void Read();
	void SetVerticalAxis(float verticalAxis);
	void SetHorizontalAxis(float horizontalAxis);
	float GetVerticalAxis();
	float GetHorizontalAxis();
	void ResetAxis();
};


#include "Input.h"

namespace Input {
	float n_verticalAxis = 0;
	float n_horizontalAxis = 0;
}


void Input::Read()
{

}

void Input::SetVerticalAxis(float verticalAxis)
{
	n_verticalAxis = verticalAxis >= -1 && verticalAxis <= 1 ? verticalAxis : InputAxis::BASE_AXIS;
}

void Input::SetHorizontalAxis(float horizontalAxis)
{
	n_horizontalAxis = horizontalAxis >= -1 && horizontalAxis <= 1 ? horizontalAxis : InputAxis::BASE_AXIS;
}

float Input::GetVerticalAxis()
{
	return n_verticalAxis;
}

float Input::GetHorizontalAxis()
{
	return n_horizontalAxis;
}

void Input::ResetAxis()
{
	n_verticalAxis = InputAxis::BASE_AXIS;
	n_horizontalAxis = InputAxis::BASE_AXIS;
}




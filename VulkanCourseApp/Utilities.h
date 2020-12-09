#pragma once

//Indices of Queue Families (if they exist)
struct QueueFamilyIndices
{
	int graphicsFamily = -1;		//Location of graphics queue family

	//Check is queue families are valid
	bool isValid()
	{
		return graphicsFamily >= 0;
	}
};

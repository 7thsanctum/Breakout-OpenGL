//***************************************************************************************
// Utilities.h - General helper class
//***************************************************************************************

#pragma once

#include <glm\glm.hpp>

// Color definitions
namespace Colors
{
	// TODO: add more colours
	const glm::vec4 White			(1.0f, 1.0f, 1.0f, 1.0f);
	const glm::vec4 Black			(0.0f, 0.0f, 0.0f, 1.0f);
	const glm::vec4 Red				(1.0f, 0.0f, 0.0f, 1.0f);
	const glm::vec4 Green			(0.0f, 1.0f, 0.0f, 1.0f);
	const glm::vec4 Blue			(0.0f, 0.0f, 1.0f, 1.0f);
	const glm::vec4 Orange			(1.0f, 0.5f, 0.0f, 1.0f);
	const glm::vec4 Yellow			(1.0f, 1.0f, 0.0f, 1.0f);
	const glm::vec4 Cyan			(0.0f, 1.0f, 1.0f, 1.0f);
	const glm::vec4 Magenta			(1.0f, 0.0f, 1.0f, 1.0f);

	const glm::vec4 Silver			(0.75f, 0.75f, 0.75f, 1.0f);
	const glm::vec4 LightSteelBlue	(0.69f, 0.77f, 0.87f, 1.0f);

	// TODO: have a better way of adding a list of colors, auto added etc
	const glm::vec4 colorList[11] = { White, Black, Red, Green, Blue, Orange, Yellow, Cyan, Magenta, Silver, LightSteelBlue };
}
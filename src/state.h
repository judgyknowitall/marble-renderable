#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Constants to help with location bindings
#define VERTEX_DATA 0
#define VERTEX_NORMAL 1
#define VERTEX_COLOR 2

#define VIEW_POS_X	0.f
#define VIEW_POS_Y	0.f
#define VIEW_POS_Z	-10.f


class State {
private:

	// allow the light to be rotated
	glm::vec4 light = glm::vec4(0.7071067811865475, 0, 0.7071067811865475, 1);
	glm::mat4 light_rotate = glm::mat4(1.0);

public:

	int render_mode = 2;
	bool show_panel = true;

	glm::vec4 background_colour = glm::vec4(0.2, 0.1, 0, 1);

	// Methods //////////////////////////////////////////////////////////////////////////////////////////////////////////

	void rotate_light(const glm::vec3& axis, const float& amount) {
		light_rotate = glm::rotate(glm::mat4(1.0f), amount, axis) * light_rotate;
	}

	glm::vec3 getLightPos() {
		glm::vec3 L = glm::vec3(light_rotate * light);
		if (glm::length(L) != 0) L = glm::normalize(L);
		return L;
	}

	glm::mat4 getLightSpaceMatrix() {
		using namespace glm;

		mat4 lightProjection = ortho(-10.f, 10.f, -10.f, 10.f, -5.f, 10.f);
		mat4 lightView = lookAt(getLightPos(), vec3(0.f), vec3(0.f, 1.f, 0.f));

		return lightProjection * lightView;
	}
};
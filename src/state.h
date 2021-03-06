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
	bool sss_first = false;

	// Colours
	glm::vec4 background_colour = glm::vec4(0.2, 0.1, 0, 1);
	glm::vec4 marble_base_colour = glm::vec4(.88f, 0.775, 0.61, 1.f);

	// Parameters
	float k_sss = 0.5f;
	float k_schlick = 0.5f;

	// Methods //////////////////////////////////////////////////////////////////////////////////////////////////////////

	void rotate_light(const glm::vec3& axis, const float& amount) {
		light_rotate = glm::rotate(glm::mat4(1.0f), amount, axis) * light_rotate;
	}

	void scale_light(float amount) {
		light = glm::scale(glm::mat4(1.0f), glm::vec3(amount)) * light;
	}

	glm::vec3 getLightPos() {
		glm::vec3 L = glm::vec3(light_rotate * light);
		return L;
	}

	glm::mat4 getLightSpaceMatrix() {
		using namespace glm;

		mat4 lightView = lookAt(getLightPos(), vec3(0.f), vec3(0.f, 1.f, 0.f));
		mat4 lightProjection = perspective(45.0f, 1.f, 0.1f, 100.f);		

		return lightProjection * lightView;
	}
};
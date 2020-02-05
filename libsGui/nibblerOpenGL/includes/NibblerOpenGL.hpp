#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "ANibblerGui.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#define CUBE_VS_PATH "libsGui/nibblerOpenGL/shaders/cube_vs.glsl"
#define CUBE_FS_PATH "libsGui/nibblerOpenGL/shaders/cube_fs.glsl"

#define TO_OPENGL_COLOR(color) glm::vec4(GET_R(color) / 255.0, GET_G(color) / 255.0, GET_B(color) / 255.0, 1.0)

class NibblerOpenGL : public ANibblerGui {
	public:
		NibblerOpenGL();
		virtual ~NibblerOpenGL();
		NibblerOpenGL(NibblerOpenGL const &src);
		NibblerOpenGL &operator=(NibblerOpenGL const &rhs);

		virtual void	updateInput();
		virtual bool	draw(std::deque<Vec2> & snake, std::deque<Vec2> & food);

	private:
		SDL_Window *	_win;
		SDL_Surface *	_surface;
		SDL_Event *		_event;
		SDL_GLContext	_context;

		Shader *			_cubeShader;
		uint32_t			_cubeShaderVAO;
		uint32_t			_cubeShaderVBO;
		static const float	_cubeVertices[];
		glm::mat4			_projection;
		Camera *			_cam;
		uint64_t			_lastLoopMs;

		virtual bool	_init();
};
// Engine.cpp

#include "stdafx.h"
#include "InputManager.h"
#include "DrawManager.h"
#include "SpriteManager.h"
#include "StateManager.h"
#include "EntityManager.h"
#include "GameState.h"
#include "Engine.h"

Engine::Engine()
{
	m_running = true;
	m_draw_manager = nullptr;
	m_input_manager = nullptr;
	m_sprite_manager = nullptr;
	m_state_manager = nullptr;
	m_entity_manager = nullptr;
}

Engine::~Engine()
{

}

bool Engine::Initialize()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	if( TTF_Init() == -1)
	{
		const char* error = TTF_GetError();
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		const char* error = IMG_GetError();
	}

	int width = 360;
	int height = 640;
	m_draw_manager = new DrawManager;
	if (!m_draw_manager->Initialize(width, height))
		return false;

	m_input_manager = new InputManager;
	m_sprite_manager = new SpriteManager(m_draw_manager->GetRenderer());
	m_state_manager = new StateManager;
	m_entity_manager = new EntityManager(m_sprite_manager);


	
	System system;
	system.width = width;
	system.height = height;
	system.draw_manager = m_draw_manager;
	system.input_manager = m_input_manager;
	system.sprite_manager = m_sprite_manager;
	m_state_manager->SetState(new GameState(system));

	return true;
}

void Engine::Shutdown()
{
	if (m_state_manager)
	{
		delete m_state_manager;
		m_state_manager = nullptr;
	}

	if (m_input_manager)
	{
		delete m_input_manager;
		m_input_manager = nullptr;
	}

	if (m_draw_manager != nullptr)
	{
		delete m_draw_manager;
		m_draw_manager = nullptr;
	}
	if (m_entity_manager)
	{
		delete m_entity_manager;
		m_entity_manager = nullptr;
	}
		

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void Engine::Update()
{
	while (m_running)
	{
		HandleEvents();

		if (!m_state_manager->Update())
		{
			m_running = false;
		}
		m_draw_manager->Clear();
		m_state_manager->Draw();
		m_draw_manager->Present();

		SDL_Delay(10);
	}
}


// private
void Engine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;

		case SDL_MOUSEMOTION:
			m_input_manager->SetMousePosition(event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			int index = event.button.button - 1;
			if (index < 0)
				index = 0;
			else if (index > 2)
				index = 2;
			m_input_manager->SetMouseButton(index, true);
		}
			break;
		case SDL_MOUSEBUTTONUP:
		{
			int index = event.button.button - 1;
			if (index < 0)
				index = 0;
			else if (index > 2)
				index = 2;
			m_input_manager->SetMouseButton(index, false);
		}
			break;
		}
	}
}

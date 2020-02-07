#include "Game.h"

/// <summary>
/// Constructor for the game class.
/// </summary>
Game::Game()
{
	try
	{
		// Try to initalise SDL in general
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw "Error Loading SDL";
		
		// Create SDL Window Centred in Middle Of Screen
		m_window = SDL_CreateWindow("Final Year Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, NULL);
		// Check if window was created correctly
		if (!m_window) throw "Error Loading Window";

		//Create the SDL Renderer 
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		//Check if the renderer was created correclty
		if (!m_renderer) throw "Error Loading Renderer";

		// Sets clear colour of renderer to black and the color of any primitives
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		// Game is running
		m_isRunning = true;


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		//Dark mode
		ImGui::StyleColorsDark();

		ImGui_ImplSDL2_InitForD3D(m_window);
		ImGuiSDL::Initialize(m_renderer, 800, 600);

	}
	catch (std::string error)
	{
		// output error if any part above doesnt load correctly
		std::cout << error << std::endl;
		// game doesnt run
		m_isRunning = false;
	}
}

/// <summary>
/// destructor for the game class
/// </summary>
Game::~Game()
{
	cleanup();
}

/// <summary>
/// function for the main game loop
/// </summary>
void Game::run()
{
	const int FPS = 30;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;
	while (m_isRunning)
	{
		frameStart = SDL_GetTicks();
		frameTime = SDL_GetTicks() - frameStart;
		processEvent();
		update();
		render();
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
}

/// <summary>
/// Processes any SDL event
/// </summary>
void Game::processEvent()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	ImGui_ImplSDL2_ProcessEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		m_isRunning = false;
		break;
	case SDL_KEYDOWN:
		// Press Escape to close screen
		if (SDLK_ESCAPE == event.key.keysym.sym)
		{
			m_isRunning = false;
		}
		break;
	default:
		break;
	}
}

/// <summary>
/// Update function
/// </summary>
void Game::update()
{
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

		static float f = 0.0f;
		
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &showanotherWindow);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		if (showanotherWindow)
		{
			ImGui::Begin("Another Window", &showanotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				showanotherWindow = false;
			ImGui::End();
		}
		//std::cout << counter << std::endl;
 

	//cstr = "";
	ImGui::Begin("Hello World");

	ImGuiHelper::InputText(windowTitle, "Window Title" );
	ImGui::SameLine();
	if (ImGui::Button("Set Title"))
	{
		SDL_SetWindowTitle(m_window, windowTitle.c_str());
	}	
	ImGui::InputFloat("Counter", &counter, 0.5f, 2.3f, ImGuiHelper::GetDecimalPercision(25).c_str());
	ImGuiHelper::InputFloat2(vec2, "InputFloat2");
	ImGuiHelper::InputFloat3(vec3, "inputFloat3");
	ImGui::End();

	SDL_SetRenderDrawColor(m_renderer,clear_color.x, clear_color.y, clear_color.z, clear_color.w);

	
}

/// <summary>
/// Render function
/// </summary>
void Game::render()
{
	SDL_RenderClear(m_renderer);
	//Draw Here
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	SDL_RenderPresent(m_renderer);
}

/// <summary>
/// Cleans up after running by deleting stuff
/// </summary>
void Game::cleanup()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_QUIT;
}

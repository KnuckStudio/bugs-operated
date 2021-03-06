#include "Core.h"

/*
� �������, �������!
� 36!
� ��� 36?
� � ��� �������?
*/

bool Core::m_isInitialized = false;
bool Core::m_isRunning = false;
sf::RenderWindow Core::m_window;
std::unique_ptr<sfg::SFGUI> Core::m_gui = nullptr;
std::unique_ptr<sfg::Desktop> Core::m_guiDesktop = nullptr;

void Core::init(const Parameters& parameters)
{
	Log::init();

	sf::ContextSettings settings;
	settings.majorVersion = 3;
    settings.minorVersion = 2;
	settings.antialiasingLevel = 0;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	sf::Uint32 windowStyle = sf::Style::Titlebar | sf::Style::Close;
	if (parameters.fullscreen) {
		windowStyle |= sf::Style::Fullscreen;
	}

    m_window.create(sf::VideoMode(parameters.width, parameters.height), parameters.title, windowStyle, settings);
	m_window.setVerticalSyncEnabled(parameters.vsync);
	m_window.setActive(true);

	ResourceManager::init();
	CursorManager::init(m_window.getSystemHandle());
	FileManager::init<DefaultFileSystem>();

	m_window.resetGLStates();

	m_gui = std::make_unique<sfg::SFGUI>();
	m_guiDesktop = std::make_unique<sfg::Desktop>();

	m_isInitialized = true;
}

void Core::close()
{
	if (m_isInitialized == false || m_isRunning == true) {
		return;
	}

	SceneManager::close();
	CursorManager::close();
	ResourceManager::close();
	FileManager::close();

	m_guiDesktop.reset();
	m_gui.reset();

	m_window.close();

	m_isRunning = false;
	m_isInitialized = false;
}

void Core::run()
{
	if (m_isInitialized == false || m_isRunning == true) {
		return;
	}

	m_isRunning = true;

	sf::Clock timer;

	while (m_isRunning) {
		float dt = timer.restart().asSeconds();

		// Handle window and keyboard events
		handleEvents();

		Scene* currentScene;

		if (m_isRunning == true &&
			(currentScene = SceneManager::getCurrentScene()) != nullptr)
		{
			m_guiDesktop->Update(dt);
		}

		if (m_isRunning == true &&
			(currentScene = SceneManager::getCurrentScene()) != nullptr) 
		{
			currentScene->onUpdate(dt);
		}

		if (m_isRunning == true && 
			(currentScene = SceneManager::getCurrentScene()) != nullptr) 
		{
			currentScene->onDraw(dt);
		}

		m_window.display();

		m_isRunning &= SceneManager::getCurrentScene() != nullptr;
	}
}

void Core::stop()
{
	m_isRunning = false;
}

sf::RenderWindow& Core::getWindow()
{
	return m_window;
}

sfg::SFGUI * Core::getGui()
{
	return m_gui.get();
}

sfg::Desktop * Core::getGuiDesktop()
{
	return m_guiDesktop.get();
}

void Core::handleEvents()
{
	Input::update();

	sf::Event e;
	while (m_window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			stop();
			break;

		case sf::Event::Resized:
			if (SceneManager::hasScenes()) {
				SceneManager::getCurrentScene()->onResize(vec2(
					static_cast<float>(e.size.width),
					static_cast<float>(e.size.height)
				));
			}
			break;

		case sf::Event::GainedFocus:
			if (SceneManager::hasScenes()) {
				SceneManager::getCurrentScene()->onFocusGained();
			}
			break;

		case sf::Event::LostFocus:
			if (SceneManager::hasScenes()) {
				SceneManager::getCurrentScene()->onFocusLost();
			}
			break;

		default:
			break;
		}

		Input::handleEvent(e);
		m_guiDesktop->HandleEvent(e);
	}
}

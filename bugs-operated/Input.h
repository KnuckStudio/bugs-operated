#pragma once

#include <bitset>
#include <SFML/Window.hpp>

#include "Math.h"

using Key = sf::Keyboard::Key;
using MouseButton = sf::Mouse::Button;

class Input
{
public:
	// Returns true if specified key is currently pressed
	static bool getKey(Key keyCode);

	// Returns true if specified key was pressed after the last frame
	static bool getKeyDown(Key keyCode);

	// Returns true if specified key was released after the last frame
	static bool getKeyUp(Key keyCode);


	// Returns true if specified mouse button is currently pressed
	static bool getMouse(MouseButton mouseCode);

	// Returns true if specified mouse button was pressed after the last frame
	static bool getMouseDown(MouseButton mouseCode);

	// Returns true if specified mouse button was released after the last frame
	static bool getMouseUp(MouseButton mouseCode);


	// Sets mouse position relative to window top left corner
	static void setMousePosition(const vec2& pos);

	// Returns mouse position relative to window top left corner
	static vec2 getMousePosition();

	// Returns mouse position delta between Input updates
	static vec2 getMouseDeltaPosition();


	// Sets cursor visibility
	static void setCursorVisible(bool visible);

private:
	friend class Core;

	static void update();

	static void handleEvent(const sf::Event& e);

	static std::bitset<Key::KeyCount> m_currentKeyStates;
	static std::bitset<Key::KeyCount> m_lastKeyStates;

	static std::bitset<MouseButton::ButtonCount> m_currentMouseStates;
	static std::bitset<MouseButton::ButtonCount> m_lastMouseStates;

	static sf::Vector2i m_currentCursorPosition;
	static sf::Vector2i m_lastCursorPosition;
};

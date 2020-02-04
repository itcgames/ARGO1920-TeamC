//#include "stdafx.h"
//#include "XBOX360CONTROLLER.h"
//
//int Xbox360Controller::S_CONTROLLER_COUNT = -1;
//
//Xbox360Controller::Xbox360Controller()
//{
//	Xbox360Controller::S_CONTROLLER_COUNT++;
//	m_joystickIndex = S_CONTROLLER_COUNT;
//}
//
//Xbox360Controller::~Xbox360Controller()
//{
//}
//
//void Xbox360Controller::update()
//{
//	// x&y is left stick, U&V is right stick while PovX&PovY is the dpad. V is the triggers
//	// set getPrevious to getCurrent state before getCurrent state gets changed
//	m_previous = m_current;
//	
//	//Button input check.
//	m_current.A = sf::Joystick::isButtonPressed(m_joystickIndex, 0); //A
//	m_current.B = sf::Joystick::isButtonPressed(m_joystickIndex, 1); //B
//	m_current.X = sf::Joystick::isButtonPressed(m_joystickIndex, 2); //X
//	m_current.Y = sf::Joystick::isButtonPressed(m_joystickIndex, 3); //Y
//
//	m_current.Start = sf::Joystick::isButtonPressed(m_joystickIndex, 7); //Start
//	m_current.Back = sf::Joystick::isButtonPressed(m_joystickIndex, 6); //Back
//
//	m_current.LB = sf::Joystick::isButtonPressed(m_joystickIndex, 4); //LB
//	m_current.RB = sf::Joystick::isButtonPressed(m_joystickIndex, 5); //RB
//
//	m_current.LeftThumbStickClick = sf::Joystick::isButtonPressed(m_joystickIndex, 8); //Left Thumb Stick
//	m_current.RightThumbStickClick = sf::Joystick::isButtonPressed(m_joystickIndex, 9); //Right Thumb Stick
//
//	// check dpad presses
//	// check for right
//	if (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovX) >= DPAD_THRESHOLD)
//	{
//		m_current.DpadRight = true;
//	}
//	else
//	{
//		m_current.DpadRight = false;
//	}
//	// check for left
//	if (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovX) <= -1 * (DPAD_THRESHOLD))
//	{
//		m_current.DpadLeft = true;
//	}
//	else
//	{
//		m_current.DpadLeft = false;
//	}
//
//	// check for up
//	if (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovY) >= DPAD_THRESHOLD)
//	{
//		m_current.DpadUp = true;
//	}
//	else
//	{
//		m_current.DpadUp = false;
//	}
//
//	// check down
//	if (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovY) <= -1 * (DPAD_THRESHOLD))
//	{
//		m_current.DpadDown = true;
//	}
//	else
//	{
//		m_current.DpadDown = false;
//	}
//
//	// left thumbstick uses X&Y
//	m_current.LeftThumbStick = sf::Vector2f(sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::X), sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Y));
//	// while right thumbstick uses U&V
//	m_current.RightThumbStick = sf::Vector2f(sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::U), sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::V));
//
//	//get values of the triggers (Z axis)
//	m_current.LTrigger = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Z); //positive for left
//	m_current.RTrigger = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Z); //negative for right
//}
//
//bool Xbox360Controller::isConnected() const
//{
//	if (m_joystickIndex < 0 || m_joystickIndex >= sf::Joystick::Count)
//		return false;
//	else
//		return sf::Joystick::isConnected(m_joystickIndex);
//}
//
//int Xbox360Controller::getIndex() const
//{
//#ifdef _DEBUG
//	sf::Joystick::Identification id = sf::Joystick::getIdentification(m_joystickIndex);
//	std::cout << "Connected controller index: " << m_joystickIndex << std::endl;
//	std::cout << "Name: " + std::string(id.name) << std::endl;
//	std::cout << "Vendor ID: " << id.vendorId << std::endl;
//	std::cout << "Product ID: " << id.productId << std::endl;
//#endif // _DEBUG
//
//	return m_joystickIndex;
//}
//
//int Xbox360Controller::getCount() const
//{
//	return Xbox360Controller::S_CONTROLLER_COUNT;
//}
//
//GamePadState Xbox360Controller::getCurrent() const
//{
//	return m_current;
//}
//
//GamePadState Xbox360Controller::getPrevious() const
//{
//	return m_previous;
//}
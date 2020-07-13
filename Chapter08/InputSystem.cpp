// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InputSystem.h"
#include <SDL/SDL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrState[keyCode] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool MouseState::GetButtonValue(int button) const
{
	return (SDL_BUTTON(button) & mCurrButtons) == 1;
}

ButtonState MouseState::GetButtonState(int button) const
{
	int mask = SDL_BUTTON(button);
	if ((mask & mPrevButtons) == 0)
	{
		if ((mask & mCurrButtons) == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if ((mask & mCurrButtons) == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const
{
	return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const
{
	if (mPrevButtons[button] == 0)
	{
		if (mCurrButtons[button] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrButtons[button] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool InputSystem::Initialize()
{
	// Keyboard
	// Assign current state pointer
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
	// Clear previous state memory
	memset(mState.Keyboard.mPrevState, 0,
		SDL_NUM_SCANCODES);

	// Mouse (just set everything to 0)
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

	// Get the connected controller, if it exists
	// 課題8.1
	for (int i=0; i<NUM_CONTROLLERS; i++)
	{
		AddGameController(i);
	}

	// 課題8.2
	ReadActionMapping("Assets/mapping.txt");

	return true;
}

void InputSystem::Shutdown()
{
}

void InputSystem::PrepareForUpdate()
{
	// Copy current state to previous
	// Keyboard
	memcpy(mState.Keyboard.mPrevState,
		mState.Keyboard.mCurrState,
		SDL_NUM_SCANCODES);

	// Mouse
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mIsRelative = false;
	mState.Mouse.mScrollWheel = Vector2::Zero;

	// Controller
	for (int i=0; i<NUM_CONTROLLERS; i++)
	{
		if (mState.Controller[i].mIsConnected)
		{
			memcpy(mState.Controller[i].mPrevButtons,
				mState.Controller[i].mCurrButtons,
				SDL_CONTROLLER_BUTTON_MAX);
		}
	}
}

void InputSystem::Update()
{
	// Mouse
	int x = 0, y = 0;
	if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons =
			SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		mState.Mouse.mCurrButtons =
			SDL_GetMouseState(&x, &y);
	}

	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// Controller
	// 課題8.1
	for (int j=0; j<4; j++)
	{
		if (!mState.Controller[j].mIsConnected)
		{
			continue;
		}
		// Buttons
		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
		{
			mState.Controller[j].mCurrButtons[i] =
				SDL_GameControllerGetButton(mController[j],
				SDL_GameControllerButton(i));
		}

		// Triggers
		mState.Controller[j].mLeftTrigger =
			Filter1D(SDL_GameControllerGetAxis(mController[j],
				SDL_CONTROLLER_AXIS_TRIGGERLEFT));
		mState.Controller[j].mRightTrigger =
			Filter1D(SDL_GameControllerGetAxis(mController[j],
				SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

		// Sticks
		x = SDL_GameControllerGetAxis(mController[j],
			SDL_CONTROLLER_AXIS_LEFTX);
		y = -SDL_GameControllerGetAxis(mController[j],
			SDL_CONTROLLER_AXIS_LEFTY);
		mState.Controller[j].mLeftStick = Filter2D(x, y);

		x = SDL_GameControllerGetAxis(mController[j],
			SDL_CONTROLLER_AXIS_RIGHTX);
		y = -SDL_GameControllerGetAxis(mController[j],
			SDL_CONTROLLER_AXIS_RIGHTY);
		mState.Controller[j].mRightStick = Filter2D(x, y);
	}

	// 課題8.2
	ButtonState fireState = GetMappingButtonState("Fire");
	ButtonState backState = GetMappingButtonState("Back");
	ButtonState jumpState = GetMappingButtonState("Jump");
	if (fireState == 1)
		SDL_Log("Fire button press.\n");
	else if (fireState == 2)
		SDL_Log("Fire button release.\n");
	else if (fireState == 3)
		SDL_Log("Fire button press hold.\n");
	if (backState == 1)
		SDL_Log("Back button press.\n");
	else if (backState == 2)
		SDL_Log("Back button release.\n");
	else if (backState == 3)
		SDL_Log("Back button press hold.\n");
	if (jumpState == 1)
		SDL_Log("Jump button press.\n");
	else if (jumpState == 2)
		SDL_Log("Jump button release.\n");
	else if (jumpState == 3)
		SDL_Log("Jump button press hold.\n");
}

void InputSystem::ProcessEvent(SDL_Event& event)
{
	int idx;
	SDL_JoystickID joyID;

	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mState.Mouse.mScrollWheel = Vector2(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y));
		break;
	case SDL_CONTROLLERDEVICEADDED:
		idx = static_cast<int>(event.cdevice.which);
		AddGameController(idx);
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		joyID = static_cast<SDL_JoystickID>(event.cdevice.which);
		RemoveGameController(joyID);
		break;
	default:
		break;
	}
}

void InputSystem::SetRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);

	mState.Mouse.mIsRelative = value;
}

float InputSystem::Filter1D(int input)
{
	// A value < dead zone is interpreted as 0%
	const int deadZone = 250;
	// A value > max value is interpreted as 100%
	const int maxValue = 30000;

	float retVal = 0.0f;

	// Take absolute value of input
	int absValue = input > 0 ? input : -input;
	// Ignore input within dead zone
	if (absValue > deadZone)
	{
		// Compute fractional value between dead zone and max value
		retVal = static_cast<float>(absValue - deadZone) /
		(maxValue - deadZone);
		// Make sure sign matches original value
		retVal = input > 0 ? retVal : -1.0f * retVal;
		// Clamp between -1.0f and 1.0f
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// Make into 2D vector
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// If length < deadZone, should be no input
	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		// Calculate fractional value between
		// dead zone and max value circles
		float f = (length - deadZone) / (maxValue - deadZone);
		// Clamp f between 0.0f and 1.0f
		f = Math::Clamp(f, 0.0f, 1.0f);
		// Normalize the vector, and then scale it to the
		// fractional value
		dir *= f / length;
	}

	return dir;
}

// 課題 8.1
void InputSystem::AddGameController(int idx)
{
	if (SDL_IsGameController(idx))
	{
		mController[idx] = SDL_GameControllerOpen(idx);
		mState.Controller[idx].mIsConnected = (mController[idx] != nullptr);
		memset(mState.Controller[idx].mCurrButtons, 0,
			SDL_CONTROLLER_BUTTON_MAX);
		memset(mState.Controller[idx].mPrevButtons, 0,
			SDL_CONTROLLER_BUTTON_MAX);
		SDL_Joystick *joy = SDL_GameControllerGetJoystick(mController[idx]);
		mState.Controller[idx].mJoystickID = SDL_JoystickInstanceID(joy);
	}
	else
	{
		mController[idx] = nullptr;
		mState.Controller[idx].mIsConnected = false;
		mState.Controller[idx].mJoystickID = -1;
	}
}

void InputSystem::RemoveGameController(SDL_JoystickID id)
{
	for(int i=0; i<NUM_CONTROLLERS; i++)
	{
		if (mState.Controller[i].mJoystickID == id)
		{
			SDL_GameControllerClose(mController[i]);
			mController[i] = nullptr;
			mState.Controller[i].mIsConnected = false;
			mState.Controller[i].mJoystickID = -1;
			break;
		}
	}
}

// 課題8.2
void InputSystem::ReadActionMapping(const std::string& fileName)
{
	std::fstream filestream(fileName);
	if (!filestream.is_open())
	{
		SDL_Log("Failed to open file %s", fileName.c_str());
		return;
	}

	while (!filestream.eof())
	{
		// １行読み込む
		std::string buffer;
		filestream >> buffer;

		// ファイルから読み込んだ１行の文字列を区切り文字で分けてリストに追加する
		std::istringstream streambuffer(buffer); // 文字列ストリーム
		int i=0;
		std::string token;                       // １セル分の文字列
        std::string key;
        std::pair<std::string, std::string> value;
		while (getline(streambuffer, token, ','))
		{
			if (i == 0)
			{
				key = token;
			}
            else if (i==1)
			{
				value.first = token;
			}
            else
            {
				value.second.resize(token.size());
                std::transform(token.cbegin(), token.cend(), value.second.begin(), toupper);
                mMapping.emplace(key, value);
            }
            i++;
		}
	}
}

ButtonState InputSystem::GetMappingButtonState(const std::string& actionName)
{
	ButtonState ret = ENone;
	auto res = mMapping.equal_range(actionName);
    if (res.first == mMapping.end())
    {
        return ret;
    }

    for (auto it = res.first; it != res.second; ++it)
    {
		std::string device = (*it).second.first;
		std::string keysym = (*it).second.second;
		if (device == "Keyboard")
		{
			SDL_Scancode code = GetKeyboardScancode(keysym);
			if (code != SDL_SCANCODE_UNKNOWN)
			{
				ret = mState.Keyboard.GetKeyState(code);
			}
			break;
		}
		else if (device == "Mouse")
		{
			int button = GetMouseButton(keysym);
			if (button > -1)
			{
				ret = mState.Mouse.GetButtonState(button);
			}
			break;
		}
		else if (device == "Controller")
		{
			SDL_GameControllerButton button = GetControllerButton(keysym);
			if (button != SDL_CONTROLLER_BUTTON_INVALID)
			{
				ret = mState.Controller[0].GetButtonState(button);
			}
			break;
		}
    }
	return ret;
}

SDL_Scancode InputSystem::GetKeyboardScancode(const std::string& name)
{
	static std::unordered_map<std::string, SDL_Scancode> kcmapping{
		{"A", SDL_SCANCODE_A},
    	{"B", SDL_SCANCODE_B},
		{"C", SDL_SCANCODE_C},
		{"D", SDL_SCANCODE_D},
		{"E", SDL_SCANCODE_E},
		{"F", SDL_SCANCODE_F},
		{"G", SDL_SCANCODE_G},
		{"H", SDL_SCANCODE_H},
		{"I", SDL_SCANCODE_I},
		{"J", SDL_SCANCODE_J},
		{"K", SDL_SCANCODE_K},
		{"L", SDL_SCANCODE_L},
		{"M", SDL_SCANCODE_M},
		{"N", SDL_SCANCODE_N},
		{"O", SDL_SCANCODE_O},
		{"P", SDL_SCANCODE_P},
		{"Q", SDL_SCANCODE_Q},
		{"R", SDL_SCANCODE_R},
		{"S", SDL_SCANCODE_S},
		{"T", SDL_SCANCODE_T},
		{"U", SDL_SCANCODE_U},
		{"V", SDL_SCANCODE_V},
		{"W", SDL_SCANCODE_W},
		{"X", SDL_SCANCODE_X},
		{"Y", SDL_SCANCODE_Y},
		{"Z", SDL_SCANCODE_Z},
		{"1", SDL_SCANCODE_1},
		{"2", SDL_SCANCODE_2},
		{"3", SDL_SCANCODE_3},
		{"4", SDL_SCANCODE_4},
		{"5", SDL_SCANCODE_5},
		{"6", SDL_SCANCODE_6},
		{"7", SDL_SCANCODE_7},
		{"8", SDL_SCANCODE_8},
		{"9", SDL_SCANCODE_9},
		{"0", SDL_SCANCODE_0},
		{"RETURN", SDL_SCANCODE_RETURN},
		{"ESCAPE", SDL_SCANCODE_ESCAPE},
		{"BACKSPACE", SDL_SCANCODE_BACKSPACE},
		{"TAB", SDL_SCANCODE_TAB},
		{"SPACE", SDL_SCANCODE_SPACE}
	};

	SDL_Scancode ret = SDL_SCANCODE_UNKNOWN;
	try
	{
		ret = kcmapping.at(name);
	}
	catch(const std::out_of_range& e)
	{
		// do nothing;
	}
	return ret;
}

int InputSystem::GetMouseButton(const std::string& name)
{
	static std::unordered_map<std::string, int> mbmapping{
		{"LEFT", SDL_BUTTON_LEFT},
		{"MIDDLE", SDL_BUTTON_MIDDLE},
		{"RIGHT", SDL_BUTTON_RIGHT},
		{"X1", SDL_BUTTON_X1},
		{"X2", SDL_BUTTON_X2}
	};

	int ret = -1;
	try
	{
		ret = mbmapping.at(name);
	}
	catch(const std::out_of_range& e)
	{
		// do nothing;
	}
	return ret;
}


SDL_GameControllerButton InputSystem::GetControllerButton(const std::string& name)
{
	static std::unordered_map<std::string, SDL_GameControllerButton> cbmapping{
		{"A", SDL_CONTROLLER_BUTTON_A},
		{"B", SDL_CONTROLLER_BUTTON_B},
		{"X", SDL_CONTROLLER_BUTTON_X},
		{"Y", SDL_CONTROLLER_BUTTON_Y},
		{"BACK", SDL_CONTROLLER_BUTTON_BACK},
		{"GUIDE", SDL_CONTROLLER_BUTTON_GUIDE},
		{"START", SDL_CONTROLLER_BUTTON_START},
		{"LEFTSTICK", SDL_CONTROLLER_BUTTON_LEFTSTICK},
		{"RIGHTSTICK", SDL_CONTROLLER_BUTTON_RIGHTSTICK},
		{"LEFTSHOULDER", SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
		{"RIGHTSHOULDER", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
		{"UP", SDL_CONTROLLER_BUTTON_DPAD_UP},
		{"DONW", SDL_CONTROLLER_BUTTON_DPAD_DOWN},
		{"LEFT", SDL_CONTROLLER_BUTTON_DPAD_LEFT},
		{"RIGHT", SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
		{"MAX", SDL_CONTROLLER_BUTTON_MAX}
	};

	SDL_GameControllerButton ret = SDL_CONTROLLER_BUTTON_INVALID;
	try
	{
		ret = cbmapping.at(name);
	}
	catch(const std::out_of_range& e)
	{
		// do nothing;
	}
	return ret;
}

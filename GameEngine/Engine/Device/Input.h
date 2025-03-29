#pragma once
#include <glfw/glfw3.h>
#include "../EventSystem.h"

enum class KeyCode {
    // 1-0 keys on keyboard
    Key_1 = GLFW_KEY_1,
    Key_2 = GLFW_KEY_2,
    Key_3 = GLFW_KEY_3,
    Key_4 = GLFW_KEY_4,
    Key_5 = GLFW_KEY_5,
    Key_6 = GLFW_KEY_6,
    Key_7 = GLFW_KEY_7,
    Key_8 = GLFW_KEY_8,
    Key_9 = GLFW_KEY_9,
    Key_0 = GLFW_KEY_0,

    // 0-9 keys on numpad
    Key_Numpad_0 = GLFW_KEY_KP_0,
    Key_Numpad_1 = GLFW_KEY_KP_1,
    Key_Numpad_2 = GLFW_KEY_KP_2,
    Key_Numpad_3 = GLFW_KEY_KP_3,
    Key_Numpad_4 = GLFW_KEY_KP_4,
    Key_Numpad_5 = GLFW_KEY_KP_5,
    Key_Numpad_6 = GLFW_KEY_KP_6,
    Key_Numpad_7 = GLFW_KEY_KP_7,
    Key_Numpad_8 = GLFW_KEY_KP_8,
    Key_Numpad_9 = GLFW_KEY_KP_9,

    // Symbols: /*-+, Enter, . on numpad
    Key_Numpad_Divide = GLFW_KEY_KP_DIVIDE,
    Key_Numpad_Multiply = GLFW_KEY_KP_MULTIPLY,
    Key_Numpad_Subtract = GLFW_KEY_KP_SUBTRACT,
    Key_Numpad_Add = GLFW_KEY_KP_ADD,
    Key_Numpad_Decimal = GLFW_KEY_KP_DECIMAL,

    // Enter on keyboard
    Key_Enter = GLFW_KEY_ENTER,

    // F1-F12
    Key_F1 = GLFW_KEY_F1,
    Key_F2 = GLFW_KEY_F2,
    Key_F3 = GLFW_KEY_F3,
    Key_F4 = GLFW_KEY_F4,
    Key_F5 = GLFW_KEY_F5,
    Key_F6 = GLFW_KEY_F6,
    Key_F7 = GLFW_KEY_F7,
    Key_F8 = GLFW_KEY_F8,
    Key_F9 = GLFW_KEY_F9,
    Key_F10 = GLFW_KEY_F10,
    Key_F11 = GLFW_KEY_F11,
    Key_F12 = GLFW_KEY_F12,

    // Insert, Home, PageUp, PageDown, Delete, End.
    Key_Insert = GLFW_KEY_INSERT,
    Key_Home = GLFW_KEY_HOME,
    Key_PageUp = GLFW_KEY_PAGE_UP,
    Key_PageDown = GLFW_KEY_PAGE_DOWN,
    Key_Delete = GLFW_KEY_DELETE,
    Key_End = GLFW_KEY_END,

    // Arrow keys
    Key_ArrowUp = GLFW_KEY_UP,
    Key_ArrowDown = GLFW_KEY_DOWN,
    Key_ArrowLeft = GLFW_KEY_LEFT,
    Key_ArrowRight = GLFW_KEY_RIGHT,

    // ` key, Tab, Caps, Shifts, Ctrls, Alts, Spacebar, Fn key
    Key_Backtick = GLFW_KEY_GRAVE_ACCENT,
    Key_Tab = GLFW_KEY_TAB,
    Key_CapsLock = GLFW_KEY_CAPS_LOCK,
    Key_ShiftLeft = GLFW_KEY_LEFT_SHIFT,
    Key_ShiftRight = GLFW_KEY_RIGHT_SHIFT,
    Key_ControlLeft = GLFW_KEY_LEFT_CONTROL,
    Key_ControlRight = GLFW_KEY_RIGHT_CONTROL,
    Key_AltLeft = GLFW_KEY_LEFT_ALT,
    Key_AltRight = GLFW_KEY_RIGHT_ALT,
    Key_Spacebar = GLFW_KEY_SPACE,

    // q w e r t y u i o p [ ] \ keys
    Key_Q = GLFW_KEY_Q,
    Key_W = GLFW_KEY_W,
    Key_E = GLFW_KEY_E,
    Key_R = GLFW_KEY_R,
    Key_T = GLFW_KEY_T,
    Key_Y = GLFW_KEY_Y,
    Key_U = GLFW_KEY_U,
    Key_I = GLFW_KEY_I,
    Key_O = GLFW_KEY_O,
    Key_P = GLFW_KEY_P,
    Key_LeftBracket = GLFW_KEY_LEFT_BRACKET,
    Key_RightBracket = GLFW_KEY_RIGHT_BRACKET,
    Key_Backslash = GLFW_KEY_BACKSLASH,

    // a s d f g h j k l ; ' keys
    Key_A = GLFW_KEY_A,
    Key_S = GLFW_KEY_S,
    Key_D = GLFW_KEY_D,
    Key_F = GLFW_KEY_F,
    Key_G = GLFW_KEY_G,
    Key_H = GLFW_KEY_H,
    Key_J = GLFW_KEY_J,
    Key_K = GLFW_KEY_K,
    Key_L = GLFW_KEY_L,
    Key_Semicolon = GLFW_KEY_SEMICOLON,
    Key_Apostrophe = GLFW_KEY_APOSTROPHE,

    // z x c v b n m , . / keys
    Key_Z = GLFW_KEY_Z,
    Key_X = GLFW_KEY_X,
    Key_C = GLFW_KEY_C,
    Key_V = GLFW_KEY_V,
    Key_B = GLFW_KEY_B,
    Key_N = GLFW_KEY_N,
    Key_M = GLFW_KEY_M,
    Key_Comma = GLFW_KEY_COMMA,
    Key_Period = GLFW_KEY_PERIOD,
    Key_Slash = GLFW_KEY_SLASH,
};

class Input {
public:
    // Initialize method to set the window for input checks
    static void Initialize(GLFWwindow* window) {
        Input::window = window;
        Mouse::Initialize(window); // Initialize mouse input
    }

    // Check if a specific key is currently pressed
    static bool KeyPressed(KeyCode key) {
        return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
    }

    // Check if a specific key has been released
    static bool KeyReleased(KeyCode key) {
        return glfwGetKey(window, static_cast<int>(key)) == GLFW_RELEASE;
    }

    // Mouse class to handle mouse-related input
    class Mouse {
    public:
        // Initialize method to set up mouse input
        static void Initialize(GLFWwindow* Window) {
            window = Window;
            glfwGetCursorPos(window, &previousX, &previousY); // Get initial mouse position
        }

        // Update the mouse position and calculate the delta
        static void Update() {
            double currentX, currentY;
            glfwGetCursorPos(window, &currentX, &currentY); // Get current mouse position

            // Calculate mouse delta (movement)
            deltaX = static_cast<float>(currentX - previousX);
            deltaY = static_cast<float>(currentY - previousY);

            // Update the previous mouse position for the next frame
            previousX = currentX;
            previousY = currentY;
        }

        // Get the current mouse position
        static void GetPosition(double &x, double &y) {
            glfwGetCursorPos(window, &x, &y);
        }

        // Get the mouse delta
        static void GetDelta(float &dx, float &dy) {
            dx = deltaX;
            dy = deltaY;
        }

        // Set whether the mouse is locked in place or not
        static void SetMouseLock(bool lock) {
            if (lock) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Lock the cursor
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  // Unlock the cursor
            }
        }

    private:
        static GLFWwindow* window;  // The window pointer for GLFW input checking
        static double previousX, previousY; // Previous mouse position
        static float deltaX, deltaY; // Mouse movement (delta)
    };

private:
    static GLFWwindow* window;  // The window pointer for GLFW input checking
};

// Initialize the static variables
GLFWwindow* Input::Mouse::window = nullptr;
double Input::Mouse::previousX = 0.0;
double Input::Mouse::previousY = 0.0;
float Input::Mouse::deltaX = 0.0f;
float Input::Mouse::deltaY = 0.0f;
GLFWwindow* Input::window = nullptr;

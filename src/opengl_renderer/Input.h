#ifndef INPUT_H
#define INPUT_H
#pragma once

#define GLR_KEYBOARD_LAYOUT_AZERTY

#include <Window.h>

namespace GLR {

    struct Input
    {
        enum class KeyboardLayout
        {
            QUERTY,
            AZERTY
        };

#ifdef GLR_KEYBOARD_LAYOUT_AZERTY
        enum Key : int32_t
        {
            UNKNOWN = (int32_t)GLFW_KEY_UNKNOWN,

            A = (int32_t)GLFW_KEY_Q,
            B = (int32_t)GLFW_KEY_B,
            C = (int32_t)GLFW_KEY_C,
            D = (int32_t)GLFW_KEY_D,
            E = (int32_t)GLFW_KEY_E,
            F = (int32_t)GLFW_KEY_F,
            G = (int32_t)GLFW_KEY_G,
            H = (int32_t)GLFW_KEY_H,
            I = (int32_t)GLFW_KEY_I,
            J = (int32_t)GLFW_KEY_J,
            K = (int32_t)GLFW_KEY_K,
            L = (int32_t)GLFW_KEY_L,
            M = (int32_t)GLFW_KEY_SEMICOLON,
            N = (int32_t)GLFW_KEY_N,
            O = (int32_t)GLFW_KEY_O,
            P = (int32_t)GLFW_KEY_P,
            Q = (int32_t)GLFW_KEY_A,
            R = (int32_t)GLFW_KEY_R,
            S = (int32_t)GLFW_KEY_S,
            T = (int32_t)GLFW_KEY_T,
            U = (int32_t)GLFW_KEY_U,
            V = (int32_t)GLFW_KEY_V,
            W = (int32_t)GLFW_KEY_Z,
            X = (int32_t)GLFW_KEY_X,
            Y = (int32_t)GLFW_KEY_Y,
            Z = (int32_t)GLFW_KEY_W,


            COMMA = (int32_t)GLFW_KEY_M,
            SEMICOLON = (int32_t)GLFW_KEY_COMMA
        };

#else 
 enum Key : int32_t
        {
            UNKNOWN = (int32_t)GLFW_KEY_UNKNOWN,

            A = (int32_t)GLFW_KEY_A,
            B = (int32_t)GLFW_KEY_B,
            C = (int32_t)GLFW_KEY_C,
            D = (int32_t)GLFW_KEY_D,
            E = (int32_t)GLFW_KEY_E,
            F = (int32_t)GLFW_KEY_F,
            G = (int32_t)GLFW_KEY_G,
            H = (int32_t)GLFW_KEY_H,
            I = (int32_t)GLFW_KEY_I,
            J = (int32_t)GLFW_KEY_J,
            K = (int32_t)GLFW_KEY_K,
            L = (int32_t)GLFW_KEY_L,
            M = (int32_t)GLFW_KEY_M,
            N = (int32_t)GLFW_KEY_N,
            O = (int32_t)GLFW_KEY_O,
            P = (int32_t)GLFW_KEY_P,
            Q = (int32_t)GLFW_KEY_Q,
            R = (int32_t)GLFW_KEY_R,
            S = (int32_t)GLFW_KEY_S,
            T = (int32_t)GLFW_KEY_T,
            U = (int32_t)GLFW_KEY_U,
            V = (int32_t)GLFW_KEY_V,
            W = (int32_t)GLFW_KEY_W,
            X = (int32_t)GLFW_KEY_X,
            Y = (int32_t)GLFW_KEY_Y,
            Z = (int32_t)GLFW_KEY_Z,


            COMMA = (int32_t)GLFW_KEY_COMMA,
            SEMICOLON = (int32_t)GLFW_KEY_SEMICOLON
        };
#endif
        static int scrollYOffset;
    
        static bool IsKeyPressed(GLFWwindow *window, const int keycode);
        static bool IsMouseButtonClicked(GLFWwindow *window, int button);
        static glm::vec2 GetMousePos(GLFWwindow *window);
    };
}

#endif /* INPUT_H */




#ifndef DEBUG_H
#define DEBUG_H
#include <vector>
#include <string>
#include <iostream>
#include "raylib.h"


class Debug {

public:

    template <typename T>
    static void Print(const T& message) {
        std::cout << message;
    }

    template <typename T, typename... Args>
    static void Print(const T& first, const Args&... rest) {
        Print(first);
        Print(" ");
        Print(rest...);
    }

    static void PrintLn() {
        Print("\n");
    }

    template <typename T>
    static void PrintLn(const T& message) {
        Print(message);
        PrintLn();
    }

    template <typename T, typename... Args>
    static void PrintLn(const T& first, const Args&... rest) {
        Print(first, rest...);
        PrintLn();
    }

    template<typename T>
    static void Print(std::vector<T>& v) {
        Print("{ ");
        if (v.size() > 0) {
            Print(v.at(0));
            for (int i = 1; i < v.size(); ++i) {
                Print(",", v.at(i));
            }
        }
        Print(" }");
    }

    template<typename T>
    static void PrintLn(std::vector<T>& v) {
        Print(v);
        PrintLn();
    }

    static void PrintError(const std::string& error) {
        Print("\x1b[38;5;168m");
        Print(error);
        PrintLn("\x1B[0m");
    }

    static void ShowAllColors() {
        for (int i = 0; i < 256; i++)
            std::cout << "\x1b[38;5;" << i << "m" << i << "\x1B[0m" << std::endl;
    }
};



#endif //DEBUG_H

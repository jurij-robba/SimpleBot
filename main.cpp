#include <chrono>
#include <thread>

#include "SimpleBot.hpp"
#include "Model.hpp"
#include "Display.hpp"

int main() {

    // initialize
    Simplebot controller;
    Model model;
    int8_t count = -1;

    // main loop
    while(true) {
        auto start = std::chrono::high_resolution_clock::now();
        ++count;
        controller.Update(model);
        if(count%6==0) {
            Display::Print(model);
            count = 0;
        }
        // wait for 5s and prevent time drift
        long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
        std::this_thread::sleep_for(
            std::chrono::milliseconds(5000 - ms));
    }
}

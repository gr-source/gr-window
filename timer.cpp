#include "timer.h"

#include <thread>

float Time::deltaTime = 0.0f;
int Time::FPS = 0;

Time::Time(float maxFPS) : a(std::chrono::system_clock::now()), b(std::chrono::system_clock::now()), m_targetFrameTime(1000.0 / static_cast<double>(maxFPS)), elapsedTime(0.0f), fpsCount(0), fps(0) {}

float Time::SleepTick() {
    a = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> work_time = a - b;

    if (work_time.count() < m_targetFrameTime) {
        std::chrono::duration<double, std::milli> delta_ms(m_targetFrameTime - work_time.count());
        auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
    }
    b = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> sleep_time = b - a;
    float dt = static_cast<float>((work_time.count() + sleep_time.count()) / 1e3f);

    ++fpsCount;
    elapsedTime += dt;
    if (elapsedTime >= 1.0000) {
        fps = fpsCount;
        fpsCount = 0;
        elapsedTime = 0.0f;
    }
    return dt;
}

float Time::start() {
    a = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> sleep_time = a - b;
    return static_cast<float>(sleep_time.count() / 1e3f);
}

void Time::end() {
    b = std::chrono::system_clock::now();
}

const int &Time::getFPS() const {
    return fps;
}

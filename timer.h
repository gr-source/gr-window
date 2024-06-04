#pragma once

#include <chrono>

class Time {
public:
    static float deltaTime;

    static int FPS;

    Time(float maxFPS = 60.0f);

    float SleepTick();

    float start();

    void end();

    const int& getFPS() const;

protected:
    std::chrono::system_clock::time_point a;
    std::chrono::system_clock::time_point b;
    double m_targetFrameTime;
    float elapsedTime;
    int fpsCount;
    int fps;
};

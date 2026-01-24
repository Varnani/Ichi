#pragma once

#include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
typedef std::chrono::nanoseconds NanoSecs;

struct ProfilerMarker
{
    std::string name = "Unnamed";
    float durationAsMilliseconds = 0;
    float durationAsSeconds = 0;

    size_t measurementCount = 0;

    TimePoint start;
    TimePoint end;

    std::vector<std::unique_ptr<ProfilerMarker>> subMarkers;
    ProfilerMarker* parentMarker = nullptr;

    ProfilerMarker();
    ProfilerMarker(const std::string name);

    void StartMeasurement();
    void CompleteMeasurement();

    ProfilerMarker* CreateSubMarker(std::string name);
};

class Profiler
{
public:
    void BeginFrame();
    void EndFrame();

    void BeginMarker(const std::string name);
    void EndMarker();

    ProfilerMarker& GetRootMarker();
    std::string GenerateReport();

    static Profiler& Get();

private:
    ProfilerMarker m_finishedRoot;
    ProfilerMarker m_activeRoot;
    ProfilerMarker* m_activeMarker;
};

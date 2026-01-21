#pragma once

#include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
typedef std::chrono::nanoseconds NanoSecs;

struct Marker
{
    std::string name = "Unnamed";
    float durationAsMilliseconds = 0;
    float durationAsSeconds = 0;

    size_t measurementCount = 0;

    TimePoint start;
    TimePoint end;

    std::vector<std::unique_ptr<Marker>> subMarkers;
    Marker* parentMarker = nullptr;

    Marker();
    Marker(const std::string name);

    void StartMeasurement();
    void CompleteMeasurement();

    Marker* CreateSubMarker(std::string name);
};

class Profiler
{
public:
    void BeginFrame();
    void EndFrame();

    void BeginMarker(const std::string name);
    void EndMarker();

    Marker& GetRootMarker();
    std::string GenerateReport();

    static Profiler& Get();

private:
    Marker m_finishedRoot;
    Marker m_activeRoot;

    Marker* m_activeMarker;
    Marker* m_activeLoopMarker;
};
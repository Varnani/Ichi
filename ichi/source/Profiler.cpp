#include "../include/Profiler.hpp"

#include <chrono>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

Profiler& Profiler::Get()
{
    static Profiler profiler{};
    return profiler;
}

void Profiler::BeginFrame()
{
    m_activeRoot = ProfilerMarker{ "Frame" };
    m_activeMarker = &m_activeRoot;
}

void Profiler::EndFrame()
{
    EndMarker();
    m_finishedRoot = std::move(m_activeRoot);

    for (auto&& marker : m_finishedRoot.subMarkers)
    {
        marker.get()->parentMarker = &m_finishedRoot;
    }
}

void Profiler::BeginMarker(const std::string name)
{
    ProfilerMarker* marker = m_activeMarker->CreateSubMarker(name);
    marker->parentMarker = m_activeMarker;
    m_activeMarker = marker;
}

void Profiler::EndMarker()
{
    m_activeMarker->CompleteMeasurement();
    m_activeMarker = m_activeMarker->parentMarker;
}

ProfilerMarker& Profiler::GetRootMarker()
{
    return m_finishedRoot;
}

std::string Profiler::GenerateReport()
{
    ProfilerMarker& rootMarker = GetRootMarker();
    if (rootMarker.subMarkers.size() == 0) return "Report is empty.";

    std::stack<ProfilerMarker*> markerStack;
    std::stack<bool> visitStack;

    markerStack.push(&rootMarker);
    visitStack.push(false);

    std::string report;

    int indentLevel = 0;
    while (!markerStack.empty())
    {
        ProfilerMarker* marker = markerStack.top();
        markerStack.pop();

        bool isVisited = visitStack.top();
        visitStack.pop();

        if (isVisited)
        {
            indentLevel--;
            continue;
        }

        std::stringstream formattedMarkerLabel;

        formattedMarkerLabel << "- " << marker->name << " | " << std::fixed << std::setprecision(4) << marker->durationAsMilliseconds << "ms";

        if (marker->parentMarker != nullptr)
        {
            float percent = (marker->durationAsMilliseconds / marker->parentMarker->durationAsMilliseconds) * 100;
            formattedMarkerLabel << " (% " << std::fixed << std::setprecision(1) << percent << (")");
        }

        formattedMarkerLabel << "\n";

        std::string spaces(indentLevel * 2, ' ');
        std::string formattedString = formattedMarkerLabel.str();

        std::string markerText = spaces + formattedString;

        report.append(markerText);

        int subMarkerCount = marker->subMarkers.size();
        if (subMarkerCount > 0)
        {
            markerStack.push(marker);
            visitStack.push(true);

            for (int i = subMarkerCount - 1; i > -1; i--)
            {
                markerStack.push(marker->subMarkers[i].get());
                visitStack.push(false);
            }

            indentLevel++;
        }
    }

    return report;
}

// ------
static TimePoint GetTime()
{
    return std::chrono::high_resolution_clock::now();
}

float GetElapsed(TimePoint start, TimePoint end)
{
    NanoSecs measurement = std::chrono::duration_cast<NanoSecs>(end - start);
    float duration = measurement.count() * 0.000001;

    return duration;
}
// ------

ProfilerMarker::ProfilerMarker(const std::string name)
{
    this->name = name;
    StartMeasurement();
}

ProfilerMarker::ProfilerMarker()
{
    StartMeasurement();
}

void ProfilerMarker::StartMeasurement()
{
    start = GetTime();
}

void ProfilerMarker::CompleteMeasurement()
{
    measurementCount++;

    end = GetTime();
    durationAsMilliseconds += GetElapsed(start, end);
    durationAsSeconds = durationAsMilliseconds * 0.001;

    if (measurementCount > 1)
    {
        durationAsMilliseconds *= 0.5f;
    }
}

ProfilerMarker* ProfilerMarker::CreateSubMarker(std::string name)
{
    std::unique_ptr<ProfilerMarker>& newMarker = subMarkers.emplace_back(std::make_unique<ProfilerMarker>(name));
    ProfilerMarker* markerPtr = newMarker.get();
    return markerPtr;
}

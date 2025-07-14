#include "profiler.hpp"

#include <algorithm>

namespace CoMe
{
bool Profiler::start()
{
    isProfilingActive = true;
    return true;
}
bool Profiler::stop()
{
    isProfilingActive = false;
    return true;
}

const Profiler::ModulesContainer& Profiler::getLoadedModules()
{
    return LoadedModules;
}

void Profiler::loadModule(const Module &module)
{
    if (!isProfilingActive)
        return;

    LoadedModules.push_back(module);
}

void Profiler::unloadModule(const Module &module)
{
    if (!isProfilingActive)
        return;

    LoadedModules.erase(std::remove(LoadedModules.begin(), LoadedModules.end(), module));
}

void Profiler::unloadAllModules()
{
    LoadedModules.clear();
}
}

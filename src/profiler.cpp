#include "profiler.hpp"

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

void Profiler::loadModule(const Module &module)
{
    if (!isProfilingActive)
        return;

    Modules.push_back(module);
}

const Profiler::ModulesContainer& Profiler::getModules()
{
    return Modules;
}
}

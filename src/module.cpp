#include "module.hpp"

namespace CoMe
{
const Module& Module::operator=(const Module& module)
{
    this->StartAddress = module.StartAddress;
    this->EndAddress   = module.EndAddress;
    this->LoadTSC      = module.LoadTSC;
    this->UnloadTSC    = module.UnloadTSC;
    this->FullPath     = module.FullPath;

    return *this;
}

bool Module::operator==(const CoMe::Module& module)
{
    return this->StartAddress == module.StartAddress &&
           this->EndAddress   == module.EndAddress   &&
           this->LoadTSC      == module.LoadTSC      &&
           this->UnloadTSC    == module.UnloadTSC    &&
           this->FullPath     == module.FullPath       ;
}
}
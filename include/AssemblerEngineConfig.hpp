#pragma once

#include <string>

namespace Hasm
{

struct EngineConfig
{
    std::string source        = "";
    bool        exportSymbols = false;
};

} // namespace Hasm

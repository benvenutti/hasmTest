#pragma once

#include "Config.hpp"

#include <optional>

namespace Hasm
{
namespace CommandLineParser
{

std::optional<Config> parse( int argc, char const* const* argv );

} // namespace CommandLineParser
} // namespace Hasm

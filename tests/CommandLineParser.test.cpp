#include "CommandLineParser.hpp"

#include <catch2/catch.hpp>

#include <array>

SCENARIO( "parse source file name", "[CommandLineParser]" )
{
    constexpr std::array<const char*, 2> args{ { "hasm", "input.asm" } };

    const auto cfg = Hasm::CommandLineParser::parse( args.size(), args.data() );

    REQUIRE( cfg.has_value() );
    REQUIRE( cfg.value().source == std::string{ "input.asm" } );
    REQUIRE_FALSE( cfg.value().exportSymbols );
}

SCENARIO( "missing source file name", "[CommandLineParser]" )
{
    constexpr std::array<const char*, 1> args{ { "hasm" } };
    REQUIRE_FALSE( Hasm::CommandLineParser::parse( args.size(), args.data() ).has_value() );
}

SCENARIO( "parse flag for symbol table", "[CommandLineParser]" )
{
    const auto check = []( const std::optional<Hasm::Config>& cfg ) {
        REQUIRE( cfg.has_value() );
        REQUIRE( cfg.value().exportSymbols );
        REQUIRE( cfg.value().source == std::string{ "input.asm" } );
    };

    {
        constexpr std::array<const char*, 3> args{ { "hasm", "-s", "input.asm" } };
        check( Hasm::CommandLineParser::parse( args.size(), args.data() ) );
    }

    {
        constexpr std::array<const char*, 3> args{ { "hasm", "--symbol-table", "input.asm" } };
        check( Hasm::CommandLineParser::parse( args.size(), args.data() ) );
    }
}

SCENARIO( "parse flag for help", "[CommandLineParser]" )
{
    {
        constexpr std::array<const char*, 2> args{ { "hasm", "-h" } };
        REQUIRE_FALSE( Hasm::CommandLineParser::parse( args.size(), args.data() ).has_value() );
    }

    {
        constexpr std::array<const char*, 2> args{ { "hasm", "--help" } };
        REQUIRE_FALSE( Hasm::CommandLineParser::parse( args.size(), args.data() ).has_value() );
    }
}

SCENARIO( "parse flag for version", "[CommandLineParser]" )
{
    {
        constexpr std::array<const char*, 2> args{ { "hasm", "-v" } };
        REQUIRE_FALSE( Hasm::CommandLineParser::parse( args.size(), args.data() ).has_value() );
    }

    {
        constexpr std::array<const char*, 2> args{ { "hasm", "--version" } };
        REQUIRE_FALSE( Hasm::CommandLineParser::parse( args.size(), args.data() ).has_value() );
    }
}

SCENARIO( "parse invalid flag", "[CommandLineParser]" )
{
    {
        constexpr std::array<const char*, 2> args{ { "hasm", "-x" } };
        REQUIRE_FALSE( Hasm::CommandLineParser::parse( args.size(), args.data() ).has_value() );
    }

    {
        constexpr std::array<const char*, 2> args{ { "hasm", "--invalid_flag" } };
        REQUIRE_FALSE( Hasm::CommandLineParser::parse( args.size(), args.data() ).has_value() );
    }
}

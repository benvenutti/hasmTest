#include "SymbolTable.hpp"

#include <boost/range/algorithm.hpp>
#include <catch2/catch.hpp>

namespace
{

struct FixtureSymbolTable
{
    FixtureSymbolTable()
    {
        symbolTable.addEntry( "s1", 0x1010 );
        symbolTable.addEntry( "s2", 0x2020 );
        symbolTable.addEntry( "s3", 0x3030 );
    }

    Hasm::SymbolTable symbolTable;
};

} // anonymous namespace

SCENARIO_METHOD( FixtureSymbolTable, "contains symbol", "[SymbolTable]" )
{
    REQUIRE( symbolTable.contains( "s1" ) );
    REQUIRE( symbolTable.contains( "s2" ) );
    REQUIRE( symbolTable.contains( "s3" ) );
    REQUIRE_FALSE( symbolTable.contains( "s4" ) );
}

SCENARIO_METHOD( FixtureSymbolTable, "symbol refers address", "[SymbolTable]" )
{
    REQUIRE( symbolTable.getAddress( "s1" ).value() == 0x1010 );
    REQUIRE( symbolTable.getAddress( "s2" ).value() == 0x2020 );
    REQUIRE( symbolTable.getAddress( "s3" ).value() == 0x3030 );
    REQUIRE_FALSE( symbolTable.getAddress( "s4" ).has_value() );
}

SCENARIO_METHOD( FixtureSymbolTable, "all symbols", "[SymbolTable]" )
{
    const auto symbols = symbolTable.getSymbols();

    REQUIRE( boost::range::count( symbols, "s1" ) == 1u );
    REQUIRE( boost::range::count( symbols, "s2" ) == 1u );
    REQUIRE( boost::range::count( symbols, "s3" ) == 1u );
    REQUIRE( boost::range::count( symbols, "s4" ) == 0u );
}

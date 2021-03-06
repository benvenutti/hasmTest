#include "HackCommandParser.hpp"

#include <boost/range/join.hpp>
#include <catch2/catch.hpp>

#include <string>
#include <vector>

namespace detail
{

const std::vector<std::string> dest{ "M", "D", "MD", "A", "AM", "AD", "AMD" };

const std::vector<std::string> comp{ "0",   "1",   "-1",  "D",   "A",   "!D",  "!A",  "-D",  "-A", "D+1",
                                     "A+1", "D-1", "A-1", "D+A", "D-A", "A-D", "D&A", "D|A", "M",  "!M",
                                     "-M",  "M+1", "M-1", "D+M", "D-M", "M-D", "D&M", "D|M" };

const std::vector<std::string> jump{ "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP" };

std::vector<std::string> combine( const std::vector<std::string>& itens1,
                                  const std::vector<std::string>& itens2,
                                  const std::string&              separator )
{
    std::vector<std::string> result;
    result.reserve( itens1.size() * itens2.size() );

    for ( const auto& a : itens1 )
    {
        for ( const auto& b : itens2 )
        {
            result.push_back( a + separator + b );
        }
    }

    return result;
}

std::vector<std::string> comps()
{
    const auto destAndComp        = combine( dest, comp, "=" );
    const auto compAndJump        = combine( comp, jump, ";" );
    const auto destAndCompAndJump = combine( destAndComp, jump, ";" );

    return boost::copy_range<std::vector<std::string>>(
        boost::join( boost::join( boost::join( comp, destAndComp ), compAndJump ), destAndCompAndJump ) );
}

const auto allComps = comps();

} // namespace detail

SCENARIO( "parse valid computations", "[HackCommandParser]" )
{
    for ( const auto& comp : detail::allComps )
    {
        REQUIRE( Hasm::HackCommandParser::isComputationCommand( comp ) );
    }
}

SCENARIO( "parse invalid computations", "[HackCommandParser]" )
{
    // invalid computations
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D+X" ) );

    // invalid destinations
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "X=D+A" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "1=D+A" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "=D+A" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D=X" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D=2" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D=" ) );

    // invalid computation and valid jump
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "X;JMP" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "2;JGT" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( ";JGE" ) );

    // valid computation and invalid jump
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D;JM" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D;JG" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D;" ) );

    // invalid destination, valid computation and jump
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "d=D+1;JMP" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "0=D+1;JMP" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "=D+1;JMP" ) );

    // valid destination, invalid computation and valid jump
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D=d+1;JMP" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D=D+;JMP" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D=;JMP" ) );

    // valid destination and computation, invalid jump
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D=A+1;jmp" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D=D+1;JM" ) );
    REQUIRE_FALSE( Hasm::HackCommandParser::isComputationCommand( "D=M+1;" ) );
}

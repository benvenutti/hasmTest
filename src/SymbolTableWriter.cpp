#include "SymbolTableWriter.hpp"

#include "SymbolTable.hpp"

#include <boost/io/ios_state.hpp>

#include <iomanip>

namespace Hasm
{

SymbolTableWriter::SymbolTableWriter( const SymbolTable& symbolTable )
{
    const auto symbols = symbolTable.getSymbols();
    for ( const auto& s : symbols )
    {
        symbolMap.emplace( symbolTable.getAddress( s ).value(), s );
    }
}

void SymbolTableWriter::write( std::ostream& out )
{
    boost::io::ios_flags_saver ifs{ out };

    for ( const auto& it : symbolMap )
    {
        out << "0x" << std::setfill( '0' ) << std::setw( 4 ) << std::hex << it.first << " " << it.second << '\n';
    }

    out.flush();
    ifs.restore();
}

} // namespace Hasm

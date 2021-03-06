#include "SymbolTable.hpp"

namespace Hasm
{

SymbolTable::SymbolTable( std::unordered_map<std::string, Hack::word> symbols )
: m_table{ std::move( symbols ) }
{
}

void SymbolTable::addEntry( const std::string& symbol, const Hack::word address )
{
    m_table.emplace( symbol, address );
}

bool SymbolTable::contains( const std::string& symbol ) const
{
    return m_table.find( symbol ) != m_table.end();
}

boost::optional<Hack::word> SymbolTable::getAddress( const std::string& symbol ) const
{
    const auto it = m_table.find( symbol );
    if ( it != m_table.end() )
    {
        return boost::optional<Hack::word>( it->second );
    }

    return boost::none;
}

std::vector<std::string> SymbolTable::getSymbols() const
{
    std::vector<std::string> symbols;
    symbols.reserve( m_table.size() );

    for ( const auto& it : m_table )
    {
        symbols.emplace_back( it.first );
    }

    return symbols;
}

} // namespace Hasm

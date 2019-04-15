#include "SymbolTable.hpp"

#include <boost/range/adaptor/map.hpp>

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

std::optional<Hack::word> SymbolTable::getAddress( const std::string& symbol ) const
{
    const auto it = m_table.find( symbol );

    return it != m_table.end() ? std::make_optional( it->second ) : std::nullopt;
}

std::vector<std::string> SymbolTable::getSymbols() const
{
    return boost::copy_range<std::vector<std::string>>( boost::adaptors::keys( m_table ) );
}

} // namespace Hasm

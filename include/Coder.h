#ifndef HASM_CODER_H_
#define HASM_CODER_H_

#include <string>

#include "Hack.h"

namespace Hasm {
namespace Coder {

Hack::WORD dest(const std::string& mnemonic);
Hack::WORD comp(const std::string& mnemonic);
Hack::WORD jump(const std::string& mnemonic);

} // namespace Coder
} // namespace Hasm

#endif

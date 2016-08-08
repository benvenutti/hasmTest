#include "AssemblerEngine.h"

#include "Assembler.h"
#include "AssemblerEngineConfig.h"
#include "CommandLineParser.h"
#include "FileHandler.h"

#include <boost/io/ios_state.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iomanip>

using namespace Hasm;

bool AssemblerEngine::run(int argc, char** argv) const {
  const AssemblerEngineConfig cfg = CommandLineParser::parse(argc, argv);

  if (!cfg.isValid) {
    return false;
  }

  if (!isAsmFile(cfg.inputName)) {
    return false;
  }

  std::ifstream inputFile(cfg.inputName);

  if (!inputFile.good()) {
    std::cerr << "error: unable to open input stream" << std::endl;

    return false;
  }

  std::string outputName = FileHandler::changeExtension(cfg.inputName, ".hack");
  std::ofstream outputFile(outputName);

  if (!outputFile.good()) {
    std::cerr << "error: unable to open " << outputName << std::endl;

    return false;
  }

  Assembler hasm(inputFile, outputFile);
  hasm.assemble();

  if (cfg.exportSymbols) {
    bool ok = exportSymbolTable(cfg, hasm.getSymbolTable());
    if (!ok) {
      return false;
    }
  }

  if (inputFile.is_open()) {
    inputFile.close();
  }

  if (outputFile.is_open()) {
    outputFile.close();
  }

  return true;
}

bool AssemblerEngine::exportSymbolTable(const AssemblerEngineConfig& cfg, const SymbolTable& table) const {
  std::string symbolsOutName = FileHandler::changeExtension(cfg.inputName, "-symbols");
  std::ofstream symbolsOut(symbolsOutName);

  if (!symbolsOut.good()) {
    std::cerr << "error: unable to open output stream" << std::endl;

    return false;
  }

  outputSymbolTable(symbolsOut, table);
  symbolsOut.close();

  return true;
}

void AssemblerEngine::outputSymbolTable(std::ostream& out, const SymbolTable& table) const {
  boost::io::ios_flags_saver ifs(out);

  std::set<std::string> symbols = table.getSymbols();
  for (auto s: symbols) {
    out << "0x" << std::setfill('0') << std::setw(4) << std::setbase(16)
        << s << " " << table.getAddress(s).get() << std::endl;
  }

  ifs.restore();
}

bool AssemblerEngine::isAsmFile(const std::string& fileName) const {
  if (!FileHandler::isFile(fileName)) {
    std::cerr << "error: input \"" << fileName << "\"is not a file" << std::endl;

    return false;
  }

  if (!FileHandler::hasExtension(fileName, ".asm")) {
    std::cerr << "error: input file must have .asm extension" << std::endl;

    return false;
  }

  return true;
}

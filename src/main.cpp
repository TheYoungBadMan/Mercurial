// src/main.cpp

#include <iostream>
#include <fstream>
#include <sstream>

#include "core/types.hpp"
#include "frontend/source/source.hpp"
#include "frontend/source/manager.hpp"
#include "frontend/lexer/lexer.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: merc <file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: cannot open file '" << argv[1] << "'\n";
        return 1;
    }

    std::ostringstream ss;
    ss << file.rdbuf();

    SourceManager manager;
    auto source_id = manager.add(argv[1], ss.str());
    auto& source = manager.source(source_id);

    auto result = Lexer::tokenize(source_id, source);

    if (!result.ok()) {
        for (auto& error : result.errors) {
            std::cerr << "Error: " << error.message << "\n";
        }
        return 1;
    }

    for (auto& token : result.tokens) {
        std::cout << "Token: " << static_cast<int>(token.kind)
                  << " [" << token.span.start << ", " << token.span.end << "]\n";
    }

    return 0;
}
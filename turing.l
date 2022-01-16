%option c++ noyywrap yylineno

%{
#include <iostream>
#include <cstdlib>
#include "machines.hh"
#include "turing.tab.hh"

using namespace yy;
%}

%%

machine                     return parser::token::MCH;
end                         return parser::token::END;

"states:"                   return parser::token::IST;
"accept:"                   return parser::token::IAC;
"init:"                     return parser::token::IIN;
"tapes:"                    return parser::token::ITP;

"state"                     return parser::token::STE;
[a-zA-Z_][a-zA-Z0-9_]*      return parser::token::MID;
\"[a-zA-Z0-9_]*\"           return parser::token::STR;

"|"                         return parser::token::PPE;
"?"                         return parser::token::QMK;
\'[a-zA-Z0-9_]\'            return parser::token::CHR;
[<>=]                       return parser::token::DIR;

[0-9]+                      return parser::token::NUM;

[ \t\n]+                    // Whitespaces skipped.
#.*                         // Commens skipped.
.                           { std::cerr << "Line " << lineno() << ": Unexpected character: '" << YYText() << "'." << std::endl; exit(1); }

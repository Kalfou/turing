%language "c++"
%locations
%define api.value.type variant

%code top {
#include <vector>
#include "machines.hh"
std::unique_ptr<Machine> machine(new Machine());
}

%code provides {
int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc);
}

%token MCH
%token END
%token IST
%token IAC
%token IIN
%token ITP
%token STE
%token PPE
%token QMK
%token <std::string> DIR
%token <std::string> CHR
%token <std::string> MID
%token <std::string> STR
%token <std::string> NUM

%type <std::vector<std::string>*> tape_declarations
%type <std::vector<char>*> characters
%type <std::vector<char>*> directions

%%

start:
    MCH MID initialization rules END
    {
        // used only during development for verification
        if(current_mode == tester)
        {
            machine->showMachine();
        }


        else if(current_mode == interpreter)
        {
            machine->runInterpreter();
        }
        else if(current_mode == visualizer)
        {
            machine->runVisualizer();
        }
    }
;

initialization:
    init_states init_accept init_init init_tapes
;

init_states:
    IST state_declarations
;

state_declarations:
    MID // at least one state is required
    {
        machine->addState(@1.begin.line, $1);
    }
|
    state_declarations MID
    {
        machine->addState(@1.begin.line, $2);
    }
;

init_accept:
    IAC MID
    {
        machine->addAccepting(@1.begin.line, $2);
    }
;

init_init:
    IIN MID
    {
        machine->addInit(@1.begin.line, $2);
    }
;

init_tapes:
    ITP NUM NUM tape_declarations
    {
        machine->addTapeCount(@2.begin.line, $2);
        machine->addTapeLength(@3.begin.line, $3);
        machine->addTapes(@4.begin.line, $4);
        delete $4;
    }
;

tape_declarations:
    //empty
    {
        $$ = new std::vector<std::string>();
    }
|
    tape_declarations STR
    {
        $1->push_back($2.substr(1, ($2.length()-2)));
        $$ = $1;
    }
|
    tape_declarations QMK
    {
        $1->push_back("?");
        $$ = $1;
    }
;

rules:
    //empty
|
    rules MID PPE characters PPE MID PPE characters PPE directions
    {
        machine->addRule(@2.begin.line, $2, $4, $6, $8, $10);
    }
;

characters:
    //empty
    {
        $$ = new std::vector<char>();
    }
|
    characters CHR
    {
        $1->push_back($2[1]);
        $$ = $1;
    }
|
    characters QMK
    {
        $1->push_back('?');
        $$ = $1;
    }
;

directions:
    //empty
    {
        $$ = new std::vector<char>();
    }
|
    directions DIR
    {
        $1->push_back($2[0]);
        $$ = $1;
    }
|
    directions QMK
    {
        $1->push_back('?');
        $$ = $1;
    }
;

%%

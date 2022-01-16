#include <iostream>
#include "machines.hh"

// Machine functions

void Machine::runInterpreter()
{
    std::vector<char> from_states;

    for(std::string& tape : (*_tapes))
    {
        if(tape == "?")
        {
            tape = getTapeFromUser();
        }

        from_states.push_back(tape[0]);
    }

    int rule_index = (*_states)[_current]->getRuleIndex(from_states);

    while(rule_index != -1)
    {
        showState();
        for(int i = 0; i < _tape_count; ++i)
        {
            (*_tapes)[i][(*_indexes)[i]] = (*_states)[_current]->getCharacter(rule_index, i);
            (*_indexes)[i] = ((*_indexes)[i] + (*_states)[_current]->getDirection(rule_index, i)) % _tape_length;

            if((*_indexes)[i] < 0)
            {
                (*_indexes)[i] = _tape_length - 1;
            }
            from_states[i] = (*_tapes)[i][(*_indexes)[i]];
        }

        _current = (*_states)[_current]->getNewCurrent(rule_index);

        rule_index = (*_states)[_current]->getRuleIndex(from_states);
    }

    showState();
    std::cout<<"INPUT "<<((*_states)[_current]->isAccepting() ? "ACCEPTED" : "REJECTED")<<std::endl;
}

std::string Machine::getTapeFromUser() const
{
    std::string tape;

    do
    {
        std::cout<<"Give me a tape!"<<std::endl;
        std::cin>>tape;
        std::cout<<"\n";

    } while (!isGoodTape(tape));
    
    return complementTape(tape);
}

bool Machine::isGoodTape(const std::string& tape) const
{
    for(const char& character : tape)
    {
        if(!(std::isalnum(character) || character == '_'))
        {
            return false;
        }
    }
    return (tape.length() <= _tape_length);
}

void Machine::showState() const
{
    std::cout<<"STATE: "<<_current<<"\n";

    for(int i = 0; i < _tape_count; ++i)
    {
        std::cout<<(*_tapes)[i]<<"\n";
        for(int j = 0; j < (*_indexes)[i]; ++j)
        {
            std::cout<<" ";
        }
        std::cout<<"^\n";
    }
    std::cout<<std::endl;
}

// State functions

std::string State::getNewCurrent(int rule_index) const
{
    return (*_rules)[rule_index]->getToName();
}

int State::getDirection(int rule_index, int index) const
{
    return (*_rules)[rule_index]->getDirection(index);
}

char State::getCharacter(int rule_index, int index) const
{
    return (*_rules)[rule_index]->getCharacter(index);
}

// Rule functions

char Rule::getCharacter(int index) const
{
    char character = (*_to_states)[index];

    if(character == '?')
    {
        while(!(std::isalnum(character) || character == '_'))
        {
            std::cout<<"Tape "<<index<<" needs a new character (alphanumeric or '_')"<<std::endl;
            std::cin>>character;
            std::cout<<std::endl;
        }
    }

    return character;
}

int Rule::getDirection(int index) const
{
    char direction = (*_directions)[index];

    if(direction == '?')
    {
        while(direction != '=' && direction != '<' && direction != '>')
        {
            std::cout<<"Tape "<<index<<" needs a direction (=,<,>)"<<std::endl;
            std::cin>>direction;
            std::cout<<std::endl;
        }
    }


    if(direction == '=')
    {
        return 0;
    }
    if(direction == '<')
    {
        return -1;
    }
    if(direction == '>')
    {
        return 1;
    }
    return 0;
}
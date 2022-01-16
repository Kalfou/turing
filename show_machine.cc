#include <iostream>
#include "machines.hh"


void State::listRules() const
{
    for (const auto& rule : (*_rules))
    {
        rule->showRule();
    }
}

void Rule::showRule() const
{
    for(char from_state : (*_from_states))
    {
        std::cout<<from_state<<" ";
    }

    std::cout<<"| "<<_to_name<<" | ";

    for(char to_state : (*_to_states))
    {
        std::cout<<to_state<<" ";
    }
    std::cout<<"| ";

    for(char direction : (*_directions))
    {
        std::cout<<direction<<" ";
    }
    std::cout<<std::endl;
}

void Machine::showMachine() const
{
    std::map<std::string, std::unique_ptr<State>>::iterator it;
    for(it = _states->begin(); it != _states->end(); ++it)
    {
        if(it->second->isAccepting())
        {
            std::cout<<"This is the one: ";
        }
        if(it->first == _current)
        {
            std::cout<<"We start here: ";
        }
        std::cout<<it->first<<std::endl;
    }

    std::cout<<"Tape count: "<<_tape_count<<"\nTape length: "<<_tape_length<<std::endl;

    for(const std::string& tape : (*_tapes))
    {
        std::cout<<tape<<std::endl;
    }

    for(it = _states->begin(); it != _states->end(); ++it)
    {
        std::cout<<it->first<<": "<<std::endl;
        it->second->listRules();
    }
}
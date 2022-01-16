#include <iostream>
#include "machines.hh"

// Machine functions

void Machine::runVisualizer() const
{
    std::cout<<"digraph turing_machine\n"
             <<"{\n"
             <<"rankdir=LR;\n"
             <<"size=\"100,100\"\n";

    visualizeNodes();
    visualizeEdges();

    std::cout<<"}"<<std::endl;
}

void Machine::visualizeNodes() const
{
    std::cout<<"node [shape = circle];";
    
    std::string accepter;

    std::map<std::string, std::unique_ptr<State>>::iterator it;
    for(it = _states->begin(); it != _states->end(); ++it)
    {
        if(it->second->isAccepting())
        {
            accepter = it->first;
        }
        else
        {
            std::cout<<" "<<it->first;
        }
    }
    std::cout<<";\n"
             <<"node [shape = doublecircle]; "<<accepter<<";\n";
}

void Machine::visualizeEdges() const
{
    std::map<std::string, std::unique_ptr<State>>::iterator it;
    for(it = _states->begin(); it != _states->end(); ++it)
    {
        it->second->visualizeRules();
    }
}

// State functions

void State::visualizeRules() const
{
    for(auto& rule : (*_rules))
    {
        std::cout<<_name<<" -> "<<rule->getToName()<<" [label = \"";
        rule->visualizeRule();
        std::cout<<"\"];\n";
    }
}

// Rule functions

void Rule::visualizeRule() const
{
    std::cout<<"|";
    for(const auto& from_state : (*_from_states))
    {
        std::cout<<" "<<from_state;
    }
    std::cout<<" |";

    for(const auto& to_state : (*_to_states))
    {
        std::cout<<" "<<to_state;
    }
    std::cout<<" |";

    for(const auto& direction : (*_directions))
    {
        std::cout<<" "<<direction;
    }
    std::cout<<" |";
}
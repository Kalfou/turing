#ifndef MACHINES_HH
#define MACHINES_HH

#include <string>
#include <vector>
#include <map>
#include <memory>

enum mode {tester, interpreter, visualizer};
extern mode current_mode;

class Rule
{
    public:
        explicit Rule(std::vector<char> *from_states, std::string to_name,
                      std::vector<char> *to_states, std::vector<char> *directions);
        
        std::string getToName() const {return _to_name;};

        void showRule() const;

        bool matchRule(const std::vector<char>& from_states) const;
        char getCharacter(int index) const;
        int getDirection(int index) const;

        void visualizeRule() const;
    
    private:
        std::unique_ptr<std::vector<char>> _from_states, _to_states, _directions;
        std::string _to_name;

};

class State
{
    public:
        explicit State(std::string name);

        void listRules() const;

        bool isAccepting() const {return _accepting;};
        void setAccepting() {_accepting = true;}
        bool addRule(std::vector<char> *from_state, std::string to_name,
                     std::vector<char> *to_state, std::vector<char> *directions);
        
        int getRuleIndex(const std::vector<char>& from_state) const;
        std::string getNewCurrent(int rule_index) const;
        int getDirection(int rule_index, int index) const;
        char getCharacter(int rule_index, int index) const;

        void visualizeRules() const;

    private:
        std::unique_ptr<std::vector<std::unique_ptr<Rule>>> _rules;
        bool _accepting;
        std::string _name;
};

class Machine
{
    public:
        Machine();
        ~Machine() {};

        void addState(int line, std::string state_name);
        void addAccepting(int line, std::string accepting_name);
        void addInit(int line, std::string init_name);
        void addTapeCount(int line, std::string tape_count);
        void addTapeLength(int line, std::string tape_length);
        void addTapes(int line, std::vector<std::string> *tapes);
        void addRule(int line, std::string from_name, std::vector<char> *from_state,
                               std::string to_name, std::vector<char> *to_state,
                               std::vector<char> *directions);
        
        void showMachine() const;

        void runInterpreter();

        void runVisualizer() const;

    protected:
        std::unique_ptr<std::map<std::string, std::unique_ptr<State>>> _states;
        std::unique_ptr<std::vector<std::string>> _tapes;
        std::unique_ptr<std::vector<int>> _indexes;

        std::string _current;
        int _tape_count, _tape_length;
        
        void error(int line, std::string text);
        std::string complementTape(std::string) const;

        
        void showState() const;
        bool isGoodTape(const std::string& tape) const;
        std::string getTapeFromUser() const;

        void visualizeNodes() const;
        void visualizeEdges() const;
};

#endif // MACHINES_HH
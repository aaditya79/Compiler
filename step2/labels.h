#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <map>
#include <unordered_map>
#include <numeric>
using namespace std;
class labels { 
    public: 
        string op = 0;
        int opcode = 0;
        labels(string _op) : op{_op} { }
        virtual string comb() { return ""; };

        static const int OP_JUMP = 0x00000010; 
        static const int OP_JUMPZERO = 0x00000011;
        static const int OP_JUMPNZERO = 0x00000012;
        static const int OP_GOSUB = 0x00000013;
        static const int OP_RETURN = 0x00000014;
        static const int OP_ENTER_SUBROUTINE = 0x00000015;
        static const int OP_EXIT_SUBROUTINE = 0x00000016;
        static const int OP_START_PROGRAM = 0x00000017;
        static const int OP_EXIT_PROGRAM = 0x00000018;

        static const int OP_PUSHSCALAR = 0x00000020;
        static const int OP_PUSHARRAY = 0x00000021;
        static const int OP_PUSHI = 0x00000022;

        static const int OP_POPSCALAR = 0x00000030;
        static const int OP_POPARRAY = 0x00000031;
        static const int OP_POP = 0x00000032;

        static const int OP_DUP = 0x00000040;
        static const int OP_SWAP = 0x00000041;

        static const int OP_ADD = 0x00000050;
        static const int OP_NEGATE = 0x00000051;
        static const int OP_MUL = 0x00000052;
        static const int OP_DIV = 0x00000053;

        static const int OP_PRINTS = 0x00000060;
        static const int OP_PRINTTOS = 0x00000061;       
};
class DupClass:public labels{
    public:
        string comb(); DupClass(string _op):labels(_op) { opcode = OP_DUP; };
};
class SwapClass:public labels{
    public:
        string comb(); SwapClass(string _op):labels(_op) { opcode = OP_SWAP; };
};
class NegateClass:public labels{
    public:
        string comb(); NegateClass(string _op):labels(_op) { opcode = OP_NEGATE; };
};
class PushscalClass:public labels{
    public:
        string comb(); PushscalClass(string _op, string _control):labels(_op) { control = _control; opcode = OP_PUSHSCALAR; };
        string control; int dd;
};
class JumpClass:public labels{
    public:
        string comb(); JumpClass(string _op, string _label):labels(_op) { label = _label; opcode = OP_JUMP; };
        int dd; string label;
};
class PusharrClass: public labels{
    public:
        string comb(); PusharrClass(string _op, string _control):labels(_op) { control = _control; opcode = OP_PUSHARRAY; };
        string control; int dd; 
};
class PopscalClass:public labels{
    public:
        string comb(); PopscalClass(string _op, string _control):labels(_op) { control = _control; opcode = OP_POPSCALAR; };
        string control; int dd; 
};
class PoparrClass:public labels{
    public:
        string comb(); PoparrClass(string _op, string _control):labels(_op) { control = _control; opcode = OP_POPARRAY; };
        string control; int dd;
};
class PrintsClass:public labels{
    public:
        string comb(); PrintsClass(string _op, int _num, string _printing):labels(_op) { printing = _printing; opcode = OP_PUSHI; num = _num; };
        int num; string printing;
};
class DeclarrClass:public labels{
    public:
        string comb(); DeclarrClass(string _op):labels(_op) { opcode = -1; };
};
class MulClass:public labels{
    public:
        string comb(); MulClass(string _op):labels(_op) { opcode = OP_MUL; };
};
class LabelClass:public labels{
    public:
        string comb(); LabelClass(string _op):labels(_op) { opcode = -1; };
};
class GosubClass:public labels{
    public:
        string comb(); GosubClass(string _op, string _label):labels(_op) { label = _label; opcode = OP_GOSUB; };
        int dd; string label;
};
class GoSubLabelClass:public labels{
    public:
        string comb(); GoSubLabelClass(string _op, string _label):labels(_op) { label = _label; opcode = OP_ENTER_SUBROUTINE;  size = -1; };
        int size; string label;
};
class StartClass:public labels{
    public:
        string comb(); StartClass(string _op):labels(_op) { opcode = OP_START_PROGRAM; }; int plt;    
};
class AddClass:public labels{
    public:
        string comb(); AddClass(string _op):labels(_op) { opcode = OP_ADD; };
};
class EndClass:public labels{
    public:
        string comb(); EndClass(string _op):labels(_op) { opcode = -1; };
};
class PopClass:public labels{
    public:
        string comb(); PopClass(string _op):labels(_op) { opcode = OP_POP; };
};
class JumpZeroClass:public labels{
    public:
        string comb(); JumpZeroClass(string _op, string _label, string _opnd):labels(_op) { label = _label; opcode = OP_JUMPZERO; opnd = _opnd;};
        int dd; string label; string opnd;
};
class DivClass:public labels{
    public:
        string comb(); DivClass(string _op):labels(_op) { opcode = OP_DIV; };
};
class PrintTosClass:public labels{
    public:
        string comb(); PrintTosClass(string _op, string _pivar, bool _dp):labels(_op) { pivar = _pivar; opcode = OP_PRINTTOS; dp = _dp;}; string pivar; bool dp;
};
class DeclscalClass:public labels{
    public:
        string comb(); DeclscalClass(string _op):labels(_op) { opcode = -1; };
};
class JumpNZeroClass:public labels{
    public:
        string comb(); JumpNZeroClass(string _op, string _label, string _opnd):labels(_op) { label = _label; opcode = OP_JUMPNZERO; opnd = _opnd;};
        int dd; string label; string opnd;
};
class PushIClass:public labels{
    public:
        string comb(); PushIClass(string _op, string _pivar):labels(_op) { pivar = _pivar; opcode = OP_PUSHI; }; string pivar;
};
class ExitObj:public labels{
    public:
        string comb(); ExitObj(string _op):labels(_op) { opcode = OP_EXIT_PROGRAM; };
};
class ReturnClass:public labels{
    public:
        string comb(); ReturnClass(string _op):labels(_op) { opcode = OP_RETURN; };
};

string PopClass::comb(){ return "Pop "; }
string PushscalClass::comb(){ return "PushScalar " + control + ", (" + to_string(dd) + ")"; }
string DupClass::comb(){ return "Dup "; }
string PopscalClass::comb(){ return "PopScalar " + control + ", (" + to_string(dd) + ")"; }
string LabelClass::comb(){ return ""; }
string ExitObj::comb(){ return "Exit "; }
string AddClass::comb(){ return "Add "; }
string PoparrClass::comb(){ return "PopArray " + control + ", (" + to_string(dd) + ")"; }
string NegateClass::comb(){ return "Negate "; }
string MulClass::comb(){ return "Mul "; }
string JumpZeroClass::comb(){ return "JumpZero, " + label + ", (" + to_string(dd) + ")"; }
string ReturnClass::comb(){ return "Return "; }
string SwapClass::comb(){ return "Swap "; }
string PusharrClass::comb(){ return "PushArray " + control + ", (" + to_string(dd) + ")"; }
string EndClass::comb(){ return "End "; }
string JumpNZeroClass::comb(){ return "JumpNZero, " + label + ", (" + to_string(dd) + ")"; }
string PushIClass::comb(){ return "PushI  (" + pivar + ")"; }
string PrintsClass::comb(){ return "Prints " + printing; }
string StartClass::comb(){ return "Start " + to_string(plt); }
string DivClass::comb(){ return "Div "; }
string PrintTosClass::comb(){ return "PrintTOS"; }
string DeclscalClass::comb(){ return ""; }
string GoSubLabelClass::comb(){ return "GoSubLabel " + label; }
string JumpClass::comb(){ return "Jump, " + to_string(dd); }
string DeclarrClass::comb(){ return ""; }
string GosubClass::comb(){ return "GoSub " + label + "(" + to_string(dd) + ")"; }


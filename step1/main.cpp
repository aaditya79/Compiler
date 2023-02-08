#include <iostream>
#include <regex>
#include <map>
#include <fstream>
#include "helper.h"
#include "labels.h"
using namespace std;
int endCheck = 0; int afterend = 0; int chh = 0; int sameCheck = 0;
string constCmd; string oldcmd; string errStr, varlab, acclet; 
regex r ("^(\\w+)"); regex r2 ("^(\\w+)\\s+(\\w)"); regex r3 ("^(\\w+)\\s+(\\w+)\\s+([0-9]+)"); 
regex r4 ("^(\\w+)\\s+(\\w+)"); regex r5 ("^(\\w+)\\s+([0-9]+)");
vector<labels*> ib; vector<string> sList;
const vector<string> il1 = {"pusharr", "popscal",  "declscal", "poparr", "pushscal"};
const vector<string> il2 = {"declarr"};
vector<map<string, helper*>> storeLabels;
const vector<string> il3 = {"pushi"};
const vector<string> il4 = { "pop", "return", "exit", "mul", "div", "swap", "end", "add", "negate", "dup", "start", "exit", "printtos"};
int ct = 0; int ct2 = 0; int bufIdx = 0;
const vector<string> il5 = {"jumpzero", "jump", "jumpnzero", "label", "gosub", "gosublabel"};
const vector<string> il6 = {"prints"};
smatch similar2; smatch similar3; smatch similar; smatch similar4; smatch similar5;
labels* checkStatement(string assemblyInst){
    if (oldcmd == assemblyInst) { constCmd = oldcmd; sameCheck += 1;} if (oldcmd == "dup") {oldcmd = "NOTHING";} else {oldcmd = assemblyInst;}
    if (endCheck != 0) { afterend += 1; }
    string op = ""; int z = 0; 
    if(regex_search(assemblyInst, similar, r)){ op = similar.str(z); }
    if (find(begin(il4), end(il4), op) != end(il4)){
        if(op == "start"){ map<string, helper*> addon; storeLabels.push_back(addon); StartClass* item = new StartClass("start"); ib.push_back(item); return item; }
        else if(op == "return"){
            ReturnClass* item = new ReturnClass("return"); int c = bufIdx;
            vector<labels*>::iterator mov; ib.push_back(item);
            mov = ib.begin() + bufIdx;
            while(mov != ib.end()){
                labels* labelptr = *mov;
                c = c + 1; int cond = -1;
                if(labelptr->op == "jump"){ JumpClass*jmpCheck; jmpCheck = static_cast<JumpClass*>(labelptr);
                    if(jmpCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[jmpCheck->label]; jmpCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "jumpnzero"){ JumpNZeroClass*jmpCheck; jmpCheck = static_cast<JumpNZeroClass*>(labelptr);
                    if(jmpCheck->dd == cond){  helper*symCheck; symCheck = storeLabels.back()[jmpCheck->label]; jmpCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "popscal"){ PopscalClass*posCheck; posCheck = static_cast<PopscalClass*>(labelptr);
                    if(posCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[posCheck->control]; posCheck->dd = symCheck->dd; }
                }   
                else if(labelptr->op == "jumpzero"){ JumpZeroClass*jmpCheck; jmpCheck = static_cast<JumpZeroClass*>(labelptr);
                    if(jmpCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[jmpCheck->label]; jmpCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "pushscal"){ PushscalClass*psCheck; psCheck = static_cast<PushscalClass*>(labelptr);
                    if(psCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[psCheck->control]; psCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "pusharr"){ PusharrClass*paCheck; paCheck = static_cast<PusharrClass*>(labelptr);
                    if(paCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[paCheck->control]; paCheck->dd = symCheck->dd; }
                }   
                else if(labelptr->op == "gosub"){ GosubClass*gsCheck; gsCheck = static_cast<GosubClass*>(labelptr); 
                    if(gsCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.front()[gsCheck->label]; gsCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "poparr"){ PoparrClass*poaCheck; poaCheck = static_cast<PoparrClass*>(labelptr);
                    if(poaCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[poaCheck->control]; poaCheck->dd = symCheck->dd; }
                }   
                else if(labelptr->op == "gosublabel"){ GoSubLabelClass*gslCheck; gslCheck = static_cast<GoSubLabelClass*>(labelptr);
                    if(gslCheck->size == cond){ gslCheck->size = ct2; }
                }
                mov++;
            }
            bufIdx = 0;
            storeLabels.pop_back();
            return item;
        }
        else if(op == "exit"){ ExitObj* item = new ExitObj("exit"); ib.push_back(item); return item; }
        else if(op == "mul"){ MulClass*item; item = new MulClass("mul"); ib.push_back(item); return item; }
        else if(op == "div"){ DivClass*item; item = new DivClass("div"); ib.push_back(item); return item; }
        else if(op == "swap"){ SwapClass*item; item = new SwapClass("swap"); ib.push_back(item); return item; }
        else if(op == "negate"){ NegateClass*item; item = new NegateClass("negate"); ib.push_back(item); return item; }
        else if(op == "printtos"){ PrintTosClass*item; item = new PrintTosClass("PrintTOS"); ib.push_back(item); return item; }
        else if(op == "dup"){ DupClass*item; item = new DupClass("dup"); ib.push_back(item); return item; }
        else if(op == "pop"){ PopClass*item; item = new PopClass("pop"); ib.push_back(item); return item; }
        else if(op == "add"){ AddClass*item; item = new AddClass("add"); ib.push_back(item); return item; }
        else if(op == "end"){
            endCheck += 1;
            vector<labels*>::iterator mov; 
            mov = ib.begin();
            while(mov != ib.end()){
                int cond = -1; labels* labelptr = *mov;
                if(labelptr->op == "start"){ StartClass*stCheck; stCheck = static_cast<StartClass*>(labelptr); if(stCheck->plt == cond){ stCheck->plt = ct; } }
                if(labelptr->op == "jump"){ JumpClass*jmpCheck; jmpCheck = static_cast<JumpClass*>(labelptr);
                    if(jmpCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[jmpCheck->label]; jmpCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "pusharr"){ PusharrClass*paCheck; paCheck = static_cast<PusharrClass*>(labelptr);
                    if(paCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[paCheck->control]; paCheck->dd = symCheck->dd; }
                }  
                else if(labelptr->op == "jumpnzero"){ JumpNZeroClass*jmpCheck; jmpCheck = static_cast<JumpNZeroClass*>(labelptr);
                    if(jmpCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[jmpCheck->label]; jmpCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "pushscal"){ PushscalClass*psCheck; psCheck = static_cast<PushscalClass*>(labelptr);
                    if(psCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[psCheck->control]; psCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "gosub"){ GosubClass*gsCheck; gsCheck = static_cast<GosubClass*>(labelptr);
                    if(gsCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[gsCheck->label]; gsCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "jumpzero"){ JumpZeroClass*jmpCheck; jmpCheck = static_cast<JumpZeroClass*>(labelptr);
                    if(jmpCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[jmpCheck->label]; jmpCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "poparr"){ PoparrClass*poaCheck; poaCheck = static_cast<PoparrClass*>(labelptr);
                    if(poaCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[poaCheck->control]; poaCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "popscal"){ PopscalClass*posCheck; posCheck = static_cast<PopscalClass*>(labelptr);
                    if(posCheck->dd == cond){  helper*symCheck; symCheck = storeLabels.back()[posCheck->control]; posCheck->dd = symCheck->dd; }
                }
                mov++;
            } 
            chh = chh + 1;
            EndClass* item = new EndClass("end");
            return item;
        }
    }
    else if (find(begin(il1), end(il1), op) != end(il1)){
        string control = "error";
        int d = 2; int o = 1;
        if(regex_search(assemblyInst, similar2, r2)){ control = similar2.str(d); }
        if(op == "popscal"){ PopscalClass*item; item = new PopscalClass(op, control); ib.push_back(item); return item; }
        else if(op == "pusharr"){ PusharrClass*item; item = new PusharrClass(op, control); ib.push_back(item); return item; }
        else if(op == "declscal"){
            helper* info;
            if(storeLabels.size() == 1){ info = new helper(ct, o); ct = ct + o; }
            else{ info = new helper(ct2 + ct, o); ct2 = ct2 + o; }
            pair<string, helper*> pair;
            pair.first = control; pair.second = info;
            storeLabels.back().insert(pair);
            DeclscalClass* item = new DeclscalClass("declscalobj");
            return item;
        }
        else if(op == "poparr"){ PoparrClass*item; item = new PoparrClass(op, control); ib.push_back(item); return item; }
        else if(op == "pushscal"){ PushscalClass*item; item = new PushscalClass(op, control); ib.push_back(item); return item; }
    }
    else if (find(begin(il2), end(il2), op) != end(il2)){
        string control = "error";
        int length = -1; int d = 2; int t = 3; int o = 1;
        if(regex_search(assemblyInst, similar3, r3)){ control = similar3.str(d); length = stoi(similar3.str(t)); }
        if(op == "declarr"){
            helper* info;
            if(storeLabels.size() == o) { info = new helper(ct, length); ct = ct + length; }
            else{ info = new helper(ct2 + ct, length); ct2 = ct2 + length; }
            pair<string, helper*> pair; 
            pair.first = control; pair.second = info;
            storeLabels.back().insert(pair);
            DeclarrClass*item; item = new DeclarrClass("declarrobj");
            return item;
        }
    }
    else if (find(begin(il5), end(il5), op) != end(il5)){
        string label = "error";
        if(regex_search(assemblyInst, similar4, r4)){ label = similar4.str(2); }
        if(op == "jump"){ JumpClass* item = new JumpClass("jump", label); ib.push_back(item); return item; }
        else if(op == "gosub"){ GosubClass* item = new GosubClass("gosub", label); ib.push_back(item); return item; }
        else if(op == "label"){
            helper* info = new helper(ib.size(), 0);
            pair<string, helper*> pair;
            pair.first = label;
            pair.second = info;
            storeLabels.back().insert(pair);
            LabelClass* item = new LabelClass("label");
            return item;
        }
        else if(op == "jumpnzero"){ JumpNZeroClass*item; item = new JumpNZeroClass("jumpnzero", label); ib.push_back(item); return item; }
        else if(op == "gosublabel"){ 
            bufIdx = ib.size(); 
            helper* info = new helper(ib.size(), 0);
            pair<string, helper*> pair;
            GoSubLabelClass* item = new GoSubLabelClass("gosublabel", label);
            ib.push_back(item);
            pair.first = label;
            pair.second = info;
            storeLabels.back().insert(pair);
            map<string, helper*> subroutine;
            storeLabels.push_back(subroutine);
            ct2 = 0;
            return item;
        }
        else if(op == "jumpzero"){ JumpZeroClass*item; item = new JumpZeroClass("jumpzero", label); ib.push_back(item); return item; }
    }
    else if (find(begin(il3), end(il3), op) != end(il3)){
        int d = 2; int test = 1;  
        if(regex_search(assemblyInst, similar5, r5)){ test = stoi(similar5.str(d)); }
        if(op == "pushi"){ PushIClass*item; item = new PushIClass("pushi", test); ib.push_back(item); return item; }
    }
    else if (find(begin(il6), end(il6), op) != end(il6)){  
        string str = "error";
        int d = 2; int start = 0; int end = 7; int o = 1;
        str = assemblyInst;
        str.erase(start, end);
        if(op == "prints"){ sList.push_back(str); PrintsClass* item = new PrintsClass("prints", sList.size() - o, str); ib.push_back(item); return item; }   
    }
    return 0;
}
int main(int argc, char **argv) {
    string fn = argv[1];
    fstream f; int o = 1; int z = 0;
    bool chk = false; string line;
    f.open(fn);
    if(f.is_open()){
        while(getline(f, line)){
            labels* charContent = checkStatement(line);
        }
        fstream out;
        out.open("result.txt", fstream::out);
        if (sameCheck != 0) {
            string dsval = constCmd.substr(0,8); string daval = constCmd.substr(0,7); string dlval = constCmd.substr(0,5);
            if (dsval == "declscal") {varlab = " variable "; acclet = constCmd.substr(9,10);}
            if (daval == "declarr") {varlab = " variable "; acclet = constCmd.substr(8,9); acclet = acclet.substr(0,1);}
            if (dlval == "label") {varlab = " label "; acclet = constCmd.substr(6,9);}
            errStr = "error: attempting to add" + varlab + "with name " + acclet + " twice"; out << errStr; cout << errStr << endl;
            return 0;
        }
        if(afterend != 0){  out << "error: code encountered after an end statement"; cout << "error: code encountered after an end statement" << endl;return 0; }
        if(endCheck != 1){ out << "error: no end statement in program"; cout << "error: no end statement in program" << endl; return 0; }
        for (auto& i : ib) { if (i->comb() != "") {cout << i->comb() << endl;}}
        for (auto & i:ib) { if (i->comb() != "") {out << i->comb() << endl; }}
        f.close();
    }
    return 0;
}

#include <iostream>
#include <regex>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <numeric>
#include "helper.h"
#include "labels.h"
using namespace std;
int endCheck = 0; int afterend = 0; int chh = 0; int sameCheck = 0; bool gs = false; bool dp = false; string tes = "99";
string constCmd; string oldcmd; string errStr, varlab, acclet; string addrJump; bool reachedJump = false; string addrNJump; string addrZJump; 
regex r ("^(\\w+)"); regex r2 ("^(\\w+)\\s+(\\w)"); regex r3 ("^(\\w+)\\s+(\\w+)\\s+([0-9]+)"); 
regex r4 ("^(\\w+)\\s+(\\w+)"); regex r5 ("^(\\w+)\\s+([0-9]+)");
vector<labels*> ib; vector<string> sList;
const vector<string> il1 = {"PushArray", "PopScalar",  "declscal", "PopArray", "PushScalar"};
const vector<string> il2 = {"declarr"};
vector<map<string, helper*>> storeLabels; 
const vector<string> il3 = {"PushI"};
const vector<string> il4 = { "Pop", "Return", "Exit", "Mul", "Div", "Swap", "End", "Add", "Negate", "Dup", "Start", "PrintTOS"};
int ct = 0; int ct2 = 0; int bufIdx = 0;
const vector<string> il5 = {"JumpZero", "Jump", "JumpNZero", "Label", "GoSub", "GoSubLabel"};
const vector<string> il6 = {"Prints"};
smatch similar2; smatch similar3; smatch similar; smatch similar4; smatch similar5;
unordered_map<string, int> opsList;
string fll;
int sfSize;
string lastadd;
vector <string> bfn;
vector <string> aline;
vector<string> rs;
string apc;
int g = 0; int gg = 0;

vector<labels *> instruction_buffer;
vector<string> runtime_stack;
vector<string> ret_addresses;
vector<int> data_memory;
unordered_map<string, string> u_map;
labels* vmExec(vector<labels*> instruction_buffer, string assemblyInst);
int pca = 0;

labels* vmExec(vector<labels*> instruction_buffer, string assemblyInst)
{
    int instructionBuffer_length = sizeof(bfn);
    string op = "";
    string currentLine;
    string opnd = "";
    //cout << op << endl;
    for (pca = 0; pca < bfn.size(); pca++)
    {
        //cout << op << endl;
        currentLine = aline[pca];
        op = bfn[pca];
        
        if (op == "PushI") {
            unsigned first = currentLine.find('(');
            unsigned last = currentLine.find_last_of(')');
            string strNew = currentLine.substr(first,last-first+1);
            strNew.erase(strNew.begin());
            strNew.erase(strNew.end()-1);
            string conv = strNew;
             //string conv = currentLine.substr(8,currentLine.size());
             
            //if (conv.back() == ')') {conv.pop_back();}
            runtime_stack.push_back(conv);
   
        }
        if (op == "PrintTOS") {
            string t = runtime_stack.back();
    
            cout << t << endl;

        }
        if (op == "Add") {
            int res;
            res = (stoi(runtime_stack.end()[-2])) + (stoi(runtime_stack.end()[-1]));
            runtime_stack.pop_back();
            runtime_stack.pop_back();
            string tmp = to_string(res);
    
            runtime_stack.push_back(tmp);
        }
        if (op == "Prints") {
            opnd = currentLine.substr(7,currentLine.size());
            cout << opnd << endl;
        }

        if (op == "Div") {
            int res;
            res = (stoi(runtime_stack.end()[-2])) / (stoi(runtime_stack.end()[-1]));
            runtime_stack.pop_back();
            runtime_stack.pop_back();
            string tmp = to_string(res);
            runtime_stack.push_back(tmp);
        }
        if (op == "Dup") {
            string tmp = runtime_stack.end()[-1];
            runtime_stack.push_back(tmp);
        }
        if (op == "Mul") {
            int res = 0;
            res = (stoi(runtime_stack.end()[-2])) * (stoi(runtime_stack.end()[-1]));
            runtime_stack.pop_back();
            runtime_stack.pop_back();
            string tmp = to_string(res);
            runtime_stack.push_back(tmp);
        }

        if (op == "Negate") {
            int res;
            res = stoi(runtime_stack.end()[-1]) * -1;
            runtime_stack.pop_back();
            string tmp = to_string(res);
            runtime_stack.push_back(tmp);
        }
        if (op == "Pop") {
            runtime_stack.pop_back();

        }
        if (op == "Swap") {
            int first = stoi(runtime_stack.end()[-1]);
            int second = stoi(runtime_stack.end()[-2]);
            runtime_stack.pop_back();
            runtime_stack.pop_back();
            runtime_stack.push_back(to_string(first));
            runtime_stack.push_back(to_string(second));
        }
        if (op == "Jump") {
            string tmp = (currentLine.substr(6,currentLine.size()));
            pca = stoi(tmp) - 1;
        }
        if (op == "GoSub") {
            
            ret_addresses.push_back(to_string(pca+1));
            unsigned first = currentLine.find('(');
            unsigned last = currentLine.find_last_of(')');
            string strNew = currentLine.substr(first,last-first+1);
            strNew.erase(strNew.begin());
            strNew.erase(strNew.end()-1);
            //pca = currentLine[9] - '0';
            pca = stoi(strNew);
            pca = pca - 1;
        }
        if (op == "GoSubLabel") {
            //cout << endl;
            //pca += 1;
            //int sizeStack = stoi(opnd);
            //data_memory.reserve(sizeStack);
        }
        if (op == "Return") {
            pca = stoi(ret_addresses.end()[-1])-1;
            ret_addresses.pop_back();
        }
        if (op == "PopScalar") {
            unsigned first = currentLine.find('(');
            unsigned last = currentLine.find_last_of(')');
            string strNew = currentLine.substr(first,last-first+1);
            strNew.erase(strNew.begin());
            strNew.erase(strNew.end()-1);
            string loc = strNew;
            //string loc = to_string(currentLine[10]);
            int res = stoi(runtime_stack.end()[-1]);

            u_map[loc] = to_string(res);
            runtime_stack.pop_back();
        }
        if (op == "PushScalar") {
            unsigned first = currentLine.find('(');
            unsigned last = currentLine.find_last_of(')');
            string strNew = currentLine.substr(first,last-first+1);
            strNew.erase(strNew.begin());
            strNew.erase(strNew.end()-1);
            string loc = strNew;
            //string loc = to_string(currentLine[11]);

            string res = u_map[loc];
            runtime_stack.push_back(res);
        }

        if (op == "PushArray") {
            unsigned first = currentLine.find('(');
            unsigned last = currentLine.find_last_of(')');
            string strNew = currentLine.substr(first,last-first+1);
            strNew.erase(strNew.begin());
            strNew.erase(strNew.end()-1);
            string loc = strNew;
            //string loc = to_string(currentLine[14]);
            int res = stoi(runtime_stack.end()[-1]);
            int res2 = res + stoi(loc);
            runtime_stack.pop_back();
            string ss = u_map[to_string(res2)];
            runtime_stack.push_back(ss);
        }

        if (op == "PopArray") {
            unsigned first = currentLine.find('(');
            unsigned last = currentLine.find_last_of(')');
            string strNew = currentLine.substr(first,last-first+1);
            strNew.erase(strNew.begin());
            strNew.erase(strNew.end()-1);
            string loc = strNew;
            //string loc = to_string(currentLine[13]);
            int res = stoi(runtime_stack.end()[-1]);
            int res2 = res + stoi(loc);
    
            runtime_stack.pop_back();
            u_map[to_string(res2)] = runtime_stack.end()[-1];
            runtime_stack.pop_back();
        }

        if (op == "JumpZero") {

            if (stoi(runtime_stack.end()[-1]) == 0) {
                unsigned first = currentLine.find('(');
                unsigned last = currentLine.find_last_of(')');
                string strNew = currentLine.substr(first,last-first+1);
                strNew.erase(strNew.begin());
                strNew.erase(strNew.end()-1);
                string conv = strNew;
                //string conv = currentLine.substr(15,currentLine.size());
                if (conv.back() == ')') {conv.pop_back();}
                pca = stoi(conv)-1;

            }
            runtime_stack.pop_back();
        }
        if (op == "JumpNZero") {
            if (stoi(runtime_stack.end()[-1]) != 0) {
                unsigned first = currentLine.find('(');
                unsigned last = currentLine.find_last_of(')');
                string strNew = currentLine.substr(first,last-first+1);
                strNew.erase(strNew.begin());
                strNew.erase(strNew.end()-1);
                string conv = strNew;
                //string conv = currentLine.substr(16,currentLine.size());
                if (conv.back() == ')') {conv.pop_back();}
                pca = stoi(conv)-1;
            }

            runtime_stack.pop_back();
        }
    }
    
    return 0;
}

labels* checkStatement(string assemblyInst){
    //if (oldcmd == assemblyInst) { constCmd = oldcmd; sameCheck += 1;} if (oldcmd == "Dup") {oldcmd = "NOTHING";} else {oldcmd = assemblyInst;}
    if (endCheck != 0) { afterend += 1; }
    string op = ""; int z = 0; 
    if(regex_search(assemblyInst, similar, r)){ op = similar.str(z); }
    opsList[op] = g;
    g+= 1; // 
    gg += 1; // controlled counter
    bfn.push_back(op);
    aline.push_back(assemblyInst);
    //cout << g << " " << gg << endl;
    //for (int i = 0; i < bfn.size(); i++) {}
    if (find(begin(il4), end(il4), op) != end(il4)){
        if(op == "Start"){ map<string, helper*> addon; storeLabels.push_back(addon); StartClass* item = new StartClass("Start"); ib.push_back(item); return item; }
        
        else if(op == "Exit"){ ExitObj* item = new ExitObj("Exit"); ib.push_back(item); return item;} 
        else if(op == "Mul"){ 
            //MulClass*item; item = new MulClass("Mul"); ib.push_back(item); return item; 
            }
        else if(op == "Div"){ 
            //DivClass*item; item = new DivClass("Div"); ib.push_back(item); return item; 
            }
        else if(op == "Swap"){ 
            //SwapClass*item; item = new SwapClass("Swap"); ib.push_back(item); return item;
             }
        else if(op == "Negate"){ 
            //NegateClass*item; item = new NegateClass("Negate"); ib.push_back(item); return item; 
            }
        else if(op == "PrintTOS"){ 
            string test;
            //PrintTosClass*item; item = new PrintTosClass("PrintTOS", test, dp); ib.push_back(item); return item; 
            }
        else if(op == "Dup"){ 
            //DupClass*item; item = new DupClass("Dup"); ib.push_back(item); return item; 
            }
        else if(op == "Pop"){
            // PopClass*item; item = new PopClass("Pop"); ib.push_back(item); return item;
             }
        else if(op == "Add"){ 
            string t3;
            //AddClass*item; item = new AddClass("Add"); ib.push_back(item); return item; 
            }
        else if(op == "End"){
            endCheck += 1;
            vector<labels*>::iterator mov; 
            mov = ib.begin();
            while(mov != ib.end()){
                int cond = -1; labels* labelptr = *mov;
                if(labelptr->op == "Start"){ StartClass*stCheck; stCheck = static_cast<StartClass*>(labelptr); if(stCheck->plt == cond){ stCheck->plt = ct; } }
                if(labelptr->op == "Jump"){ JumpClass*jmpCheck; jmpCheck = static_cast<JumpClass*>(labelptr);
                    if(jmpCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[jmpCheck->label]; jmpCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "PushArray"){ PusharrClass*paCheck; paCheck = static_cast<PusharrClass*>(labelptr);
                    if(paCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[paCheck->control]; paCheck->dd = symCheck->dd; }
                }  
                else if(labelptr->op == "JumpNZero"){ JumpNZeroClass*jmpCheck; jmpCheck = static_cast<JumpNZeroClass*>(labelptr);
                    if(jmpCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[jmpCheck->label]; jmpCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "PushScalar"){ PushscalClass*psCheck; psCheck = static_cast<PushscalClass*>(labelptr);
                    if(psCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[psCheck->control]; psCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "GoSub"){ GosubClass*gsCheck; gsCheck = static_cast<GosubClass*>(labelptr);
                    if(gsCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[gsCheck->label]; gsCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "JumpZero"){ JumpZeroClass*jmpCheck; jmpCheck = static_cast<JumpZeroClass*>(labelptr);
                    if(jmpCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[jmpCheck->label]; jmpCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "PopArray"){ PoparrClass*poaCheck; poaCheck = static_cast<PoparrClass*>(labelptr);
                    if(poaCheck->dd == cond){ helper*symCheck; symCheck = storeLabels.back()[poaCheck->control]; poaCheck->dd = symCheck->dd; }
                }
                else if(labelptr->op == "PopScalar"){ PopscalClass*posCheck; posCheck = static_cast<PopscalClass*>(labelptr);
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
        string location;
        int d = 2; int o = 1;
        if(regex_search(assemblyInst, similar2, r2)){ control = similar2.str(d); }
        if(op == "PopScalar"){ 
            //location = assemblyInst[10]; PopscalClass*item; item = new PopscalClass(op, location); ib.push_back(item); return item; 
            }
        else if(op == "PushArray"){ 
            //string location; location = assemblyInst[14]; PusharrClass*item; item = new PusharrClass(op, location); ib.push_back(item); return item; 
            }
        else if(op == "DeclScal"){
            helper* info;
            if(storeLabels.size() == 1){ info = new helper(ct, o); ct = ct + o; }
            else{ info = new helper(ct2 + ct, o); ct2 = ct2 + o; }
            pair<string, helper*> pair;
            pair.first = control; pair.second = info;
            storeLabels.back().insert(pair);
            DeclscalClass* item = new DeclscalClass("declscalobj");
            return item;
        }
        else if(op == "PopArray"){ 
            //string location; location = assemblyInst[13]; PoparrClass*item; item = new PoparrClass(op, location); ib.push_back(item); return item; 
            }
        else if(op == "PushScalar"){ 
            //location = assemblyInst[11]; PushscalClass*item; item = new PushscalClass(op, location); ib.push_back(item); return item; 
             }
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
        unordered_map <string, string> ra;
        if(regex_search(assemblyInst, similar4, r4)){ label = similar4.str(2); }
        if(op == "Jump"){ 
            //if (g == gg) { g = assemblyInst[6] - '0'; JumpClass* item = new JumpClass("Jump", label); addrJump = to_string(g); reachedJump = true; ib.push_back(item); return item; 
            //}
            }
        else if(op == "GoSub"){ /*gs = true;*/ 
            //if (g == gg) {
                //for (auto & i:bfn) {cout << i<< endl;}
                /*string fl; fl = assemblyInst[9];
                fll = assemblyInst[9];
                sfSize = stoi(fl); 
                string res;
                for (int i = 0; i < bfn.size(); i++) {
                std::vector<string>::iterator itr = std::find(bfn.begin(), bfn.end(), bfn[i]);
 
                if (itr != bfn.cend()) {
                    cout << bfn[i]<< endl;
                    std::cout << "Element present at index " << std::distance(bfn.begin(), itr) << endl;
                }
                string res;
                int loc;
                
                //loc = bfn.find(op);
                //res = bfn[loc];
                //cout <<"loc:" <<loc<< endl;
                }
                rs.push_back(to_string(g+1)); 
                g = stoi(string(1, assemblyInst[9])) - 1;  
                GosubClass* item = new GosubClass("GoSub", label); 
                ib.push_back(item); 
                
                return item; 
                */
               }//}
        else if(op == "Label"){
            helper* info = new helper(ib.size(), 0);
            pair<string, helper*> pair;
            pair.first = label;
            pair.second = info; 
            storeLabels.back().insert(pair);
            LabelClass* item = new LabelClass("Label");
            return item;
        }
        else if(op == "JumpNZero"){
            //if ((assemblyInst[18] - '0') == 1) {dp = true;} if (g == gg) {string opnd; JumpNZeroClass*item; if (isdigit(assemblyInst[17])){ opnd = string(1,assemblyInst[16]) + assemblyInst[17]; } else {opnd = string(1, assemblyInst[16]);} item = new JumpNZeroClass("JumpNZero", to_string(g), opnd); reachedJump = true;addrNJump = to_string(g);ib.push_back(item); return item; 
            }
        else if(op == "GoSubLabel"){ 
            //gs = false;
            //if (g == gg) {
            //vector<int> sf;
            //sf.reserve(sfSize); 
            //GoSubLabelClass* item = new GoSubLabelClass("GoSubLabel", label);
            //ib.push_back(item);
            /*bufIdx = ib.size(); 
            //cout << "buffidx: " << bufIdx << endl;
            helper* info = new helper(ib.size(), 0);
            pair<string, helper*> pair;
            GoSubLabelClass* item = new GoSubLabelClass("GoSubLabel", label);
            //cout << item << endl;
            ib.push_back(item);
            pair.first = label;
            //cout << "p1: " << pair.first << endl;
            pair.second = info;
            //cout << "p2: " << pair.second << endl;
            storeLabels.back().insert(pair);
            map<string, helper*> subroutine;
            storeLabels.push_back(subroutine);
            ct2 = 0;
            g += 1;*/
            //return item;//}
        }
        else if(op == "JumpZero"){ 
            //string opnd; JumpZeroClass*item; if (isdigit(assemblyInst[16])){ opnd = string(1,assemblyInst[15]) + assemblyInst[16]; } else {opnd = string(1, assemblyInst[15]);}item = new JumpZeroClass("JumpZero", to_string(g), opnd); reachedJump = false;addrJump = to_string(g);ib.push_back(item); return item;
             }
    }
    else if (find(begin(il3), end(il3), op) != end(il3)){
        
        int num;
        int j = 0;
        string conv(1, assemblyInst[8]);
        int d = 2; 
        if(op == "PushI"){ 
            string test2;
            //PushIClass*item; item = new PushIClass("PushI", conv); ib.push_back(item); return item; 
            }
    }
    else if (find(begin(il6), end(il6), op) != end(il6)){  
        string str = "error";
        int d = 2; int start = 0; int end = 7; int o = 1;
        str = assemblyInst;

        str.erase(start, end);
        
        if(op == "Prints"){ 
            /*
            if (reachedJump == false) {sList.push_back(str); PrintsClass* item = new PrintsClass("Prints", sList.size() - o, str); ib.push_back(item); return item;}
            else if ((reachedJump == true) && (addrJump != to_string(opsList["Prints"]))) { sList.push_back(str);  PrintsClass* item = new PrintsClass("Prints", sList.size() - o, str); ib.push_back(item); reachedJump = false; addrJump = ""; return item;}

            */
            
            }
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
            checkStatement(line);
        }
 
        vmExec(instruction_buffer, line);

        fstream out;
        //out.open("result.txt", fstream::out);
        if (sameCheck != 0) {
            string dsval = constCmd.substr(0,8); string daval = constCmd.substr(0,7); string dlval = constCmd.substr(0,5);
            if (dsval == "declscal") {varlab = " variable "; acclet = constCmd.substr(9,10);}
            if (daval == "declarr") {varlab = " variable "; acclet = constCmd.substr(8,9); acclet = acclet.substr(0,1);}
            if (dlval == "label") {varlab = " label "; acclet = constCmd.substr(6,9);}
            errStr = "error: attempting to add" + varlab + "with name " + acclet + " twice"; out << errStr;
            return 0;
        } 

        //for (auto& i : ib) { if (i->comb() != "") {cout << i->comb() << endl;}}
        for(auto & i:ib) { if (i->comb() != "") {out << i->comb() << endl; }}
        f.close();
       
       
    }
    
    return 0;
}


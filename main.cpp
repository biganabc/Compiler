#include <iostream>
#include "WordAnalyser.h"
#include "SyntaxAnalyser.h"
#include "SymbolTable.h"
#include "ErrorDetector.h"
#include "Coder.h"
#include "Stm.h"

using namespace std;

map<int, string> Stm::int2str = {};
int Stm::index = 0;
int Stm::labelIndex = 0;
map<int, vector<int> > Stm::arrayInitials = {};
vector<shared_ptr<Stm> > Stm::allStms = vector<shared_ptr<Stm> >();

int main() {
    ErrorDetector errorDetector("testfile.txt");
    bool isRight = errorDetector.analyse(true, true);
    if (isRight) {
        SyntaxAnalyser syntaxAnalyser("testfile.txt");
        syntaxAnalyser.analyse();
        Stm::outPut("m.txt");
        Coder code;
        code.make();
        code.outPut("mips.txt");
    } else {
        cout << "error!";
    }
    return 0;
}
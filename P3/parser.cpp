#include <bits/stdc++.h>

using namespace std;
int main() {

    int rule;
    string val;
    string vout;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        int candidate;
        if (ss >> candidate) {
            rule = candidate;
        }
        string sep;
        ss >> sep;
        if (sep != "|") {
            vout = sep;
            ss>>sep;
        }
        string res;
        while(ss>>sep){
            res+=" "+sep;
        }
        cout<<"{fprintf(yyout, \";R"<<rule<<":\\t"<<vout<<" ::="<<res<<"\");};"<<endl;

    }
}
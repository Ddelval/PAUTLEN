#include <fstream>
#include <iostream>
#include <map>
using namespace std;
int main() {
    ifstream data("tokens.data");
    string s;
    int val;
    map<int, string> pos;
    while (data >> s >> val) {
        pos[val] = s;
    }
    int d;
    while (cin >> d) {
        cout << pos[d] << endl;
    }
}
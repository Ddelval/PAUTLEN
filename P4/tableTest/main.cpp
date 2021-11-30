#include "ht.h"
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

struct hashUnion {
    unordered_map<string, int> standard;
    ht *custom;

    hashUnion() { custom = ht_create(free); }
    ~hashUnion() { ht_destroy(custom); }

    bool check_get(string key) {
        if (ht_present(custom, key.c_str()) && standard.count(key)) {
            int *cust = (int *)ht_get(custom, key.c_str());
            return *cust == standard[key];
        } else if (ht_present(custom, key.c_str()) && !standard.count(key)) {
            cout << "Not in standard" << endl;
            return false;
        } else if (!ht_present(custom, key.c_str()) && standard.count(key)) {
            cout << "Not in custom" << endl;
            return false;
        } else {
            return true;
        }
    }
    bool check_delete(string key) {
        if (!check_get(key)) {
            return false;
        }
        if (standard.count(key)) {
            if (!ht_pop(custom, key.c_str())) {
                return false;
            }
            standard.erase(standard.find(key));

            return check_get(key);
        } else {
            return check_get(key);
        }
    }
    bool check_insert(string key, int val) {
        if (!check_get(key)) {
            return false;
        }
        int *value = (int *)calloc(1, sizeof(int));
        *value = val;
        standard[key] = val;
        ht_set(custom, key.c_str(), value);

        return check_get(key);
    }
};

bool testInsertions(int amount) {
    hashUnion uni = hashUnion();
    for (int i = 0; i < amount; ++i) {
        string s = "";
        s += char('a' + i % 10);
        s += char('a' + (i / 10) % 10);
        if (!uni.check_insert(s, i)) {
            cout << "insert failure" << endl;
            return false;
        }
    }
    cout << "Single insertions ok" << endl;
    return true;
}

bool testDeletions(int amount) {
    hashUnion uni = hashUnion();
    for (int i = 0; i < amount; ++i) {
        string s = "";
        s += char('a' + i % 10);
        s += char('a' + (i / 10) % 10);
        if (!uni.check_insert(s, i)) {
            cout << "insert failure" << endl;
            return false;
        }
        if (i && i % 4 == 0) {
            string other_key = "";
            int oi = i / 4;
            other_key += char('a' + oi % 10);
            other_key += char('a' + (oi / 10) % 10);
            if (!uni.check_delete(other_key)) {
                cout << "delete failure" << endl;
                return false;
            }
        }
    }
    cout << "Single deletions ok" << endl;
    return true;
}
bool test_random_keys(int amount) {
    hashUnion uni = hashUnion();
    srand(1);

    for (int i = 0; i < amount; ++i) {
        string s = "";
        for (int j = 0; j < 2; ++j) {
            s += char('a' + rand() % 26);
        }
        int value = rand() % 1000;
        //cerr << "Inserted " << s << endl;
        if (!uni.check_insert(s, value)) {
            cout << "insert failure:" << s << " " << value << endl;
            return false;
        }
        if (i && i % 4 == 0) {
            string s = "";
            for (int j = 0; j < 2; ++j) {
                s += char('a' + rand() % 26);
            }
            //cerr << "Deleted " << s << endl;
            if (!uni.check_delete(s)) {
                cout << "delete failure" << endl;
                return false;
            }
        }
    }
    cout << "Random deletions ok" << endl;
    return true;
}

int main() {
    if (!testInsertions(1000000)) {
        return 1;
    }
    if (!testDeletions(10000)) {
        return 1;
    }
    if (!test_random_keys(1000000)) {
        return 1;
    }

    return 0;
}
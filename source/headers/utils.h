#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <algorithm> // for std::min

using namespace std; 

class Utils {
    public: 
        /*
        split a string <txt> by the delimiter <ch> and store the parts in <strs>
        */
        static unsigned int split(const string &txt, vector<string> &strs, char ch) {
            int pos = txt.find(ch);
            int initialPos = 0;
            strs.clear();
            while (pos != string::npos) {
                strs.push_back(txt.substr(initialPos, pos - initialPos));
                initialPos = pos + 1;
                pos = txt.find(ch, initialPos);
            }
            strs.push_back(txt.substr(initialPos, min(pos, (int)txt.size()) - initialPos));
            return strs.size();
        }
        
    protected: 
    private: 
}; 

#endif // UTILS_H
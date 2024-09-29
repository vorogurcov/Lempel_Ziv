#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Information
{
    vector<pair<int, char>>* code;
    vector<string>* dictionary;
};

int FindIndex(vector<char>::iterator it)
{

}

int Length(int index)
{

}

Information& Lempel_Ziv(vector<char>& vec)
{
    Information* inf = new Information;
    vector<pair<int, char>>* code = new vector<pair<int,char>>;
    vector<string>* dict = new vector<string>; 
    vector<char>::iterator it;

    for (it = vec.begin() ;it  != vec.end(); )
    {
        int curindex = FindIndex(it);
        int length = Length(curindex);
        code->push_back(make_pair(curindex, *it));
        dict->push_back((*dict)[curindex] + *(it + length));
        it += length + 1;
    }

    inf->code = code;
    inf->dictionary = dict;
    return *inf;
}

int main()
{
    
}


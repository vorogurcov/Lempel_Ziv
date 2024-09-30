#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

struct Information
{
    vector<pair<int, char>>* code;
    vector<string>* dictionary;
};

string* ConstructString(vector<char>::iterator iter,int end, vector<char>::iterator enditer)
{
    string* str = new string;
    if (enditer - iter > end)
        for (vector<char>::iterator it = iter; it < (iter + end); it++)
            str->push_back(*it);
    else
        *str = "";
    return str;
}

int FindIndex(vector<char>::iterator iter,vector<string>& dict,int index, vector<char>::iterator enditer)
{
    int len = 0, pos = 0;
    for (size_t i = 0; i < index; i++)
    {
        int curlen = dict[i].length();
        string* str = ConstructString(iter, curlen,enditer);
        if (dict[i] == *str && curlen > len)
            pos = i, len = curlen;
        delete str;
    }
    return pos;
}

Information& Lempel_Ziv(vector<char>& vec)
{
    Information* inf = new Information;
    vector<pair<int, char>>* code = new vector<pair<int,char>>;
    vector<string>* dict = new vector<string>; 
    (*dict).push_back("");

    for (vector<char>::iterator it = vec.begin() ;it  != vec.end(); )
    {
        int curindex = FindIndex(it,*dict,dict->size(),vec.end());
        int length = (*dict)[curindex].length();
        code->push_back(make_pair(curindex, (length == 0)?*it:*(it+length)));
        dict->push_back((*dict)[curindex] + *(it + length));
        it += length + 1;
    }

    inf->code = code;
    inf->dictionary = dict;
    return *inf;
}

void decode_Lempel_Ziv(ofstream& ofs,vector<pair<int, char>>* code)
{
    vector<string>* dict = new vector<string>;
    dict->push_back("");
    for (vector<pair<int, char>>::iterator iter = code->begin(); iter != code->end(); iter++)
    {
        int p = (*iter).first;
        char q = (*iter).second;
        
        dict->push_back((*dict)[p] + q);
        ofs << dict->back();
    }
}

void printvec(vector<string>& vec)
{
    for (auto& el : vec)
    {
        cout << el << endl;
    }
}
void writepairs(ofstream& ofs,Information& inf)
{
    vector<pair<int, char>>& vec = *inf.code;
    for (auto& el : vec)
    {
        ofs << el.first << el.second << endl;
    }
}
int main()
{
    std::ifstream ifs("test.txt");
    vector<char> vec;
    if (ifs.is_open())
    {
        while (!ifs.eof())
        {
            vec.push_back(ifs.get());
        }
        vec.pop_back();
    }
    ifs.close();
    Information obj;
    obj = Lempel_Ziv(vec);
    printvec(*obj.dictionary);
    ofstream ofs("compressed_data.txt");
    
    cout << obj.code->size() * 3;
    writepairs(ofs, obj);
    ofs.close();
    ofs.open("decoded_version.txt");
    decode_Lempel_Ziv(ofs, obj.code);
}


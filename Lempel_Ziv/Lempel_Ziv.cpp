#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

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

void printvec(const vector<string>& vec)
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
    bool state;
    string filename ="";
    cout << "Type in 1 to compress, type 0 to decompress.\n";
    cin >> state;
    if (state == true)
    {
        cout << "Type in the name of the file which you need to compress \n";
        cin >> filename;

        std::ifstream ifs(filename + ".txt");
        vector<char> vec;
        if (ifs.is_open())
        {
            while (!ifs.eof())
            {
                vec.push_back(ifs.get());
            }
            vec.pop_back();
            ifs.close();
        }
        else
        {
            cout << "ERROR! The file does not exist!";
            return 666;
        }


        Information obj;
        cout << "\Compressing using Lempel_Ziv...\n";

        obj = Lempel_Ziv(vec);
        cout << "Success! Compressed version of " << filename << ".txt takes only \n";
        cout << obj.code->size() * 3 << " bytes!";
        printvec(*obj.dictionary);
        ofstream ofs("compressed_data.txt");
        writepairs(ofs, obj);
    }
    else
    {
        cout << "Type in the name of the file which you need to decompress \n";
        cin >> filename;
        
        ifstream ifs(filename + ".txt");
        vector<pair<int, char>> code;
        if (ifs.is_open())
        {
            while (!ifs.eof())
            {
                string data;
                //getline(ifs, data);
                char symb;
                int d = 0;
                char sym;
                while (ifs.get(symb))
                {
                    
                    if (isdigit(symb))
                    {
                        data.push_back(symb);
                    }
                    else
                    {
                        sym = symb;
                        break;
                    }
                }
                    
                if (data == "")
                    continue;
                d = stoi(data);
                code.push_back(make_pair(d,sym));
            }
            
            ifs.close();
        }
        else
        {
            cout << "ERROR! The file does not exist!";
            return 666;
        }
        ifs.close();
        ofstream ofs;
        ofs.open("decoded_version.txt");
        decode_Lempel_Ziv(ofs, &code);
    }
    
}


/**************************************************************************************/
/* Author 	: Ahmed Kilany		  				   	     */
/* Problem 	: https://www.hackerrank.com/challenges/attribute-parser/problem    */
/***********************************************************************************/
#include <iostream>
#include <bits/stdc++.h>
#include <string.h>

using namespace std;

#define lli long long int
#define ulli unsigned long long int
#define slli signed long long int
#define endl "\n"

class TAG
{
private:
    string name ;
    vector<pair<string,string> > AttrVal;
    vector<TAG*> NextTag;
public:

    void SetTagName(string name);
    string GetTagName();

    void SetTagAttrVal(string attr, string val);
    string GetTagAttrVal(string attr);

    void SetNextTag(TAG* NexTag);
    TAG* GetNextTag(string TagName);
    TAG* GetParentTag(int ParentNum);

};

// This function used to split input string line in number of strings for HRML
vector<string> split(string ss)
{
    vector<string>vivo;
    string tmp = "";
    for (int i = 0; i< ss.length(); i++)
    {
        if (ss[i] != ' ')
            tmp += ss[i];
        else
        {
            vivo.push_back(tmp);
            tmp = "";
        }

    }
    vivo.push_back(tmp);
    return vivo;
}

// the function is used to split a string based another splitter 
vector<string> splitSol(string ss)
{
    vector<string>vivo;
    string tmp = "";
    for (int i = 0; i<ss.length(); i++)
    {
        if (ss[i] != '.' && ss[i] != '~')
            tmp += ss[i];
        else
        {
            vivo.push_back(tmp);
            tmp = "";
        }

    }
    vivo.push_back(tmp);
    return vivo;
}

// Set Tag Name	<tag_Name>
void TAG::SetTagName(string name)
{
    this->name = name;
}
// Get Tag Name	<tag_Name>
string TAG::GetTagName()
{
    return this->name;
}
// Set Atrribute and value	<tag_Name attr = "Value">
void TAG::SetTagAttrVal(string attr, string val)
{
    pair<string, string> pr(attr, val);
    this->AttrVal.push_back(pr);
}
// Get Atrribute and value	<tag_Name attr = "Value">
string TAG::GetTagAttrVal(string attr)
{
    pair<string, string> pr;
    int flag = 1;
    for (int i = 0; i < this->AttrVal.size(); i++)
    {
        pr = this->AttrVal[i];
        if (pr.first == attr)
        {
            return pr.second;
            flag = 0;
        }
    }
    if (flag)
    {
        return "Not Found!";
    }
}
// Set Child Tag
void TAG::SetNextTag(TAG* NexTag)
{
    this->NextTag.push_back(NexTag);
}
// Get Child Tag
TAG* TAG::GetNextTag(string TagName)
{
    TAG* tag = NULL;
    for (int i = 0; i < this->NextTag.size(); i++)
    {
        tag = this->NextTag[i];
        if (tag->name == TagName)break;
    }
    return tag;
}
// Get Parent Tag based on specific child
TAG* TAG::GetParentTag(int parent)
{
    TAG* tag = NULL;
    if(parent>1)
    {
        tag = this->NextTag[this->NextTag.size()-1];
        for(int i=2;i<parent;i++)
        {
            if(tag->NextTag.size()>0)
            {
                tag = tag->NextTag[tag->NextTag.size()-1];
            }
            else break;
        }
    }
    return tag;
}
// Parse HRML Language
vector<TAG> ParseHRML(string s[], int sz)
{
    vector<TAG> VectTAG;  // vector of class
    vector<string> vivo;
    TAG tag; // class
    int x = 0, y = 0, idx = 0;
    string fst = "", sec = "", pre = "1";
    for (int i = 0; i<sz; i++)
    {
        // define Opening tag OR Closing tag
        if (s[i][0] == '<' && s[i][1] != '/') // opening tag
        {
            vivo = split(s[i]);
            y++;
            vivo[0].erase(0, 1);
            vivo[vivo.size() - 1].erase(vivo[vivo.size() - 1].size() - 1);
            if (y == 1)  // Opening Tag in first 
            {
                fst = vivo[0];
                tag.SetTagName(vivo[0]);
                for (int j = 2; j<vivo.size(); j++)
                {

                    if (vivo[j] == "=")
                    {
                        vivo[j+1].erase(0, 1);
                        vivo[j + 1].erase(vivo[j+1].size()-1, 1);
                        tag.SetTagAttrVal(vivo[j - 1], vivo[j + 1]);
                    }
                }
                vivo.clear();
            }
            else if (y>1)   // Opening tag inside another opening tag
            {
                TAG* newTAG = new TAG;
                TAG* lastTag = new TAG;
                newTAG->SetTagName(vivo[0]);
                
		for (int j = 2; j<vivo.size(); j++)
                {
                    if (vivo[j] == "=")
                    {
                        vivo[j + 1].erase(0, 1);
                        vivo[j + 1].erase(vivo[j + 1].size() - 1, 1);
                        newTAG->SetTagAttrVal(vivo[j - 1], vivo[j + 1]);
                    }
                }
                
		lastTag = tag.GetParentTag(y-1);
                if(lastTag != NULL)
                {
		    lastTag->SetNextTag(newTAG);
		}
                else if (lastTag == NULL)
                {
                    tag.SetNextTag(newTAG);
                }
                vivo.clear();
            }
        }
        else if (s[i][0] == '<' && s[i][1] == '/')// closing tag
        {
            y--;
            s[i].erase(0, 2);
            s[i].erase(s[i].size() - 1, 1);
            sec = s[i];
            if (fst == sec)
            {
                VectTAG.push_back(tag);
            }
        }
        vivo.clear();
    }

    return VectTAG;
}
// Search about value for specific attribute 
string SearchTag(vector<TAG> vtag, string s )
{
    vector<string> sol = splitSol(s);
    int x = 0, y = sol.size();
    TAG tag;

    for (int i = 0; i < vtag.size(); i++)
    {
        tag = vtag[i];
        while (tag.GetTagName() == sol[x])
        {
            x++;
            if (x == y - 1) // AttrVal
            {
                return tag.GetTagAttrVal(sol[x]);
            }
            else
            {
                tag = *tag.GetNextTag(sol[x]);
            }
        }
    }
    return "Not Found!";
}

int main()
{
//        ios_base::sync_with_stdio(false);
//        cin.tie(0);
//        cout.tie(0);

    int n, m;
    string str[25], query[25];
    vector <TAG> vtag;
    cin >> n >> m;
    cin.ignore();
    cout<<"N:"<<n<<endl;
    for (int i = 0; i<n; i++)
    {
        getline(cin, str[i]);
    }

//    cout << "Before Parser!! " << endl;
    vtag = ParseHRML(str, n);
//    cout << "After Parser !!" << endl;

        for(int i=0; i<m; i++)
        {
            getline(cin, query[i]);
            cout << SearchTag(vtag, query[i]) << endl;
        }
    return 0;
}


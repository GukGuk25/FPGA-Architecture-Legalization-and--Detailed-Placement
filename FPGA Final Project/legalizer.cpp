#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include <set>


using namespace std;

class cc{
    public :
        string name = "-1";
        string type = "-1";
        float x = -1;
        float y = -1;
        int flag = -1;

        //print all data
        void pa(){
            cout << this->name << " " << this->type << " " << this->x << " " << this->y << " " << this->flag << "\n";
        };

        void ccoords(float xc, float yc){
            this->x     = xc;
            this->y     = yc;
        };

        //set the local variable
        cc(string n, string t, float xc, float yc){
            this->name  = n;
            this->type  = t;
            this->x     = xc;
            this->y     = yc;
            this->flag  = 0;
        };
};  

void setoutput(vector<cc>& arch, vector<cc>& instances, string outdir){
    ofstream outfile;
    outfile.open(outdir);
    for(auto it : arch){
        for(auto its : instances){
            // cout << its.name;
            // cout << it.name;
            if (its.x == it.x && its.y==it.y){
                outfile << its.name << " " << it.name << endl;
                // cout << its.name << " Placed On " << it.name << endl;
            }
        }
    }
    outfile.close();
    cout << "Done Writing\n";

}

void OpenFile(string filepath, vector<string>& out){
    fstream inputfile;
    vector<string> inputs;
    inputfile.open(filepath, ios::in);
    if(inputfile.is_open()){
        string tp;
        while(getline(inputfile, tp)){
            out.push_back(tp);
        }
        inputfile.close();
    }
}

void toClass(vector<cc>& CLB, vector<cc>& DSP, vector<cc>& RAM, string pathAddr){
    vector<string> inputV;
    OpenFile(pathAddr, inputV);
    for(int i=0; i<inputV.size(); i++){
        vector<string> temp;
        string s;
        stringstream ss(inputV[i]);
        while (getline(ss, s, ' ')){temp.push_back(s);}
        if(temp[1] == "CLB"){CLB.push_back(cc(temp[0], temp[1], stof(temp[2]), stof(temp[3])));}
        else if(temp[1] == "DSP"){DSP.push_back(cc(temp[0], temp[1], stof(temp[2]), stof(temp[3])));}
        else if(temp[1] == "RAM"){RAM.push_back(cc(temp[0], temp[1], stof(temp[2]), stof(temp[3])));}
    };
}

void toClassArch(vector<cc>& arch, string pathAddr){
    vector<string> inputV;
    OpenFile(pathAddr, inputV);
    for(int i=0; i<inputV.size(); i++){
        vector<string> temp;
        string s;
        stringstream ss(inputV[i]);
        while (getline(ss, s, ' ')){temp.push_back(s);}
        arch.push_back(cc(temp[0], temp[1], stof(temp[2]), stof(temp[3])));
    };
}

void Legalization(vector<cc>& arch, vector<cc>& instances, float xsize, float ysize, string outdir){
    vector<cc> legal;
    string ct[10000];
    int bott[10000] = {0};

    for(auto it : arch){
        int xcor = floor(it.x);
        ct[xcor] = it.type;
    }
    // for (int el = 0; el<1000 ; el++){
    //     cout << ct[el];
    // }
    for(auto it : instances){
        if (it.type == "CLB"){
            int x = floor(it.x);
            int y = floor(it.y);
            //benerin x
            int tempx = x;
            int movex = -1;
            while (ct[x] != "CLB"){
                //geser kiri kalo misal beda tipe
                x += movex;
                if (x<0){// kalo mines geser kanan
                    x = tempx;
                    movex = 1;
                }
            }
            y = bott[x];
            bott[x] += 1;
            // cout << "CLB " << it.name << " (" << it.x << ","<< it.y << ")";
            it.ccoords(float(x) + 0.5, float(y)*1.0 + 0.5);
            // cout << "--> (" << it.x << ","<< it.y << ")\n";
            legal.push_back(cc(it.name, it.type, it.x, it.y));
        }
        if (it.type == "RAM"){
            int x = floor(it.x);
            int y = floor(it.y);
            //benerin x
            int tempx = x;
            int movex = -1;
            while (ct[x] != "RAM"){
                //geser kiri kalo misal beda tipe
                x += movex;
                if (x<0){// kalo mines geser kanan
                    x = tempx;
                    movex = 1;
                }
            }
            y = bott[x];
            bott[x] += 1;
            // cout << "RAM " << it.name << " (" << it.x << ","<< it.y << ")";
            it.ccoords(float(x) + 0.5, float(y)*5.0 + 2.5);
            // cout << "--> (" << it.x << ","<< it.y << ")\n";
            legal.push_back(cc(it.name, it.type, it.x, it.y));
        }
        if (it.type == "DSP"){
            int x = floor(it.x);
            int y = floor(it.y);
            //benerin x
            int tempx = x;
            int movex = -1;
            while (ct[x] != "DSP"){
                //geser kiri kalo misal beda tipe
                x += movex;
                if (x<0){// kalo mines geser kanan
                    x = tempx;
                    movex = 1;
                }
            }
            y = bott[x];
            bott[x] += 1;
            // cout << "DSP " << it.name << " (" << it.x << ","<< it.y << ")";
            it.ccoords(float(x) + 0.5, float(y)*2.5 + 1);
            // cout << "--> (" << it.x << ","<< it.y << ")\n";
            legal.push_back(cc(it.name, it.type, it.x, it.y));
        }
    }
    cout << "Legalization Done\n";
    cout << "Writing Begin\n";
    setoutput(arch, legal, outdir);
    cout << "Finished\n";
}

int main(int argc, char** argv){
    vector<cc> archCLB, archDSP, archRAM, arch;
    toClassArch(arch, argv[1]);

    vector<cc> instancesCLB, instancesDSP, instancesRAM, inst;
    toClassArch(inst, argv[2]);
    
    vector<set<string>> netlist;
    vector<string> tempp;
    OpenFile(argv[3], tempp);
    for(int i=0; i<tempp.size(); i++){
        string s;
        set<string> temp;
        netlist.push_back(temp);
        stringstream ss(tempp[i]);
        getline(ss, s, ' ');
        while (getline(ss, s, ' ')){netlist[i].insert(s);}

    }   
    sort( inst.begin(), inst.end(), [](const cc &a, const cc &b){ return (a.x < b.x);});
    sort( inst.begin(), inst.end(), [](const cc &a, const cc &b){ return (a.y < b.y);});
    cout << "Legalization Begin\n";
    Legalization(arch, inst, 1.0, 1.0, argv[4]);
    // setoutput(arch, inst);
}
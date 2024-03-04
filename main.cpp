#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include "Command.h"

using namespace std;


void run_app(std::fstream& masterind_file, std::fstream& master_file, std::fstream& slave_file, std::vector<pair<int, streampos>>& ind,
            vector<streampos>& rubbish, vector<streampos>& slave_rubbish)
{
    string query;
    while (true)
    {
        cout<<">>";
        cin>>query;
        if (query=="insert-m"){
            AddNodeShow(master_file, ind, rubbish);
        }
        else if (query=="insert-s"){
            AddNodeHost(slave_file, master_file, ind, slave_rubbish);
        }
        else if (query=="del-m"){
            int num;
            cin>>num;
            RemoveNodeShow(master_file, ind, slave_file, rubbish, slave_rubbish, num);
        }
        else if (query=="del-s"){
            int num;
            cin>>num;
            RemoveNodeHost(slave_file, ind, master_file, slave_rubbish, num);
        }
        else if (query=="update-m"){
            int num;
            string field, value;
            cin>>num>>field>>value;
            UpdateShow(master_file, ind, num, field, value);
        }
        else if (query=="update-s"){
            int num;
            string field, value;
            cin>>num>>field>>value;
            UpdateHost(slave_file, ind, master_file, num, field, value);
        }
        else if (query=="get-m"){
            string query;
            getline(cin, query);
            GetShow(master_file, ind, query);
        }
        else if (query=="get-s"){
            string query;
            getline(cin, query);
            GetHost(slave_file, ind, master_file, query);
        }
        else if (query=="ut-m"){
            PrintNodesShowUT(master_file, ind);
        }
        else if (query=="ut-s"){
            PrintNodesHostUT(slave_file, ind, master_file);
        }
        else if (query=="calc-m"){
            CalcNodesShow(master_file, ind);
        }
        else if (query=="calc-s"){
            CalcNodesHost(slave_file, ind, master_file);
        }
        else if (query=="help"){
            print_help();
        }
        else if (query=="exit"){
            OnExit(masterind_file, master_file, slave_file, ind, rubbish, slave_rubbish);
            break;
        }
        if (rubbish.size()>=5 || slave_rubbish.size()>=5)
        {
            reorganize(master_file, slave_file, ind, rubbish, slave_rubbish);
        }
    }
}

int main()
{
    std::streampos write_pos = 0;
    std::streampos prev_pos = -1;
    std::vector<streampos> rubbish = {};
    std::vector<streampos> slave_rubbish = {};
    std::vector<pair<int, streampos>> ind;
    fstream slave_file;
    fstream master_file;
    fstream masterind_temp_file;
    fstream masterind_file;
    int max_del=5;

    open_file(&slave_file, "slave.bin", true);
    open_file(&master_file, "master.bin", true);
    open_file(&masterind_temp_file, "masterind.bin", true);
    ReadIndex(masterind_temp_file, ind);
    open_file(&masterind_file, "masterind.bin", false);

    run_app(masterind_file, master_file, slave_file, ind, rubbish, slave_rubbish);

    return 0;
}

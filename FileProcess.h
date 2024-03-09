#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include <stdio.h>
#include "ReadData.h"

using namespace std;

bool open_file(fstream * file, string filename, bool not_temp)
{
    if (not_temp){
        *file = fstream(filename, std::ios::binary | std::ios::in | std::ios::out);
    }
    else{
        *file = fstream(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    }
    auto err = errno;

    if (err == ENOENT)
    {
         *file = fstream(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    }

    if (!*file) {
        std::cerr << "Unable to open file=" << filename << std::endl;

        return false;
    }

    if (file->fail()) {
        std::cerr << "Failed to open file=" << filename << std::endl;
        return false;
    }

    return true;
}

bool reorganize(fstream& master_file, fstream& slave_file, std::vector<pair<int, streampos>>& ind, vector<streampos>& rubbish, vector<streampos>& slave_rubbish)
{
    fstream master_temp, slave_temp;
    open_file(&master_temp, "master_temp.bin", true);
    open_file(&slave_temp, "slave_temp.bin", true);
    rubbish={};
    slave_rubbish={};

    TVshow master_tmp;
    TVhost prev_tmp;
    TVhost tmp;

    streampos pos=0;
    streampos slave_pos=0;

    for (int i=1; i<ind.size(); i++){
        if (!ReadShow(master_tmp, master_file, ind[i].second))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
        ind[i].second=pos;
        streampos next_ptr=master_tmp.first_host;
        streampos prev_ptr=-1;

        while (next_ptr!=-1)
        {
            if (!ReadHost(tmp, slave_file, next_ptr))
            {
                std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                return false;
            }
            if (tmp.exists){
                if (prev_ptr==-1)
                {
                    master_tmp.first_host=slave_pos;
                }
                else{
                    if (!ReadHost(prev_tmp, slave_temp, prev_ptr))
                    {
                        std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                        return false;
                    }
                    prev_tmp.next=slave_pos;
                    if (!WriteHost(prev_tmp, slave_temp, prev_ptr))
                    {
                        std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                        return false;
                    }
                }
                if (!WriteHost(tmp, slave_temp, slave_pos))
                {
                    std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                    return false;
                }
                prev_ptr=slave_pos;
                slave_pos+=sizeof(tmp);
            }
            next_ptr=tmp.next;
        }
        if (master_tmp.exists){
            if (!WriteShow(master_tmp, master_temp, pos))
            {
                std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                return false;
            }
            pos+=sizeof(master_tmp);
        }
    }

    int res;

    master_file.close();
    master_temp.close();
    res = remove(".\\master.bin");
    res = rename("master_temp.bin", "master.bin");
    master_file.open("master.bin");

    slave_file.close();
    slave_temp.close();
    res = remove(".\\slave.bin");
    res = rename("slave_temp.bin", "slave.bin");
    slave_file.open("slave.bin");

    return true;
}

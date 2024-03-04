#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include "WriteTable.h"

using namespace std;

bool ReadIndex(std::fstream& file, std::vector<pair<int, streampos>>& ind)
{
    if (!file)
        return false;

    std::streampos pos=0;

    while (!file.eof())
    {
        pair<int, streampos> el;
        file.seekg(pos);
        file.read(reinterpret_cast<char*>(&el), sizeof(el));
        pos+=sizeof(el);
        ind.push_back(el);
    }

    sort(ind.begin(), ind.end());
    return !file.fail();
}

bool WriteIndex(std::fstream& file, std::vector<pair<int, streampos>>& ind)
{
    if (!file)
        return false;


    std::streampos pos=0;
    for (int i=1; i<ind.size(); i++){
        file.seekp(pos);
        file.write(reinterpret_cast<const char*>(&ind[i]), sizeof(ind[i]));
        pos+=sizeof(ind[i]);
    }
    file.flush();

    return !file.fail();
}

bool ReadHost(TVhost& record, std::fstream& file, const std::streampos& pos)
{
    if (!file)
        return false;

    file.seekg(pos);
    file.read(reinterpret_cast<char*>(&record), sizeof(TVhost));

    return !file.fail();
}

bool WriteHost(const TVhost& record, std::fstream& file, const std::streampos& pos)
{
    if (!file)
        return false;

    file.seekp(pos);
    file.write(reinterpret_cast<const char*>(&record), sizeof(TVhost));
    file.flush();

    return !file.fail();
}

bool ReadShow(TVshow& record, std::fstream& file, const std::streampos& pos)
{
    if (!file)
        return false;

    file.seekg(pos);
    file.read(reinterpret_cast<char*>(&record), sizeof(TVshow));

    return !file.fail();
}

bool WriteShow(const TVshow& record, std::fstream& file, const std::streampos& pos)
{
    if (!file)
        return false;

    file.seekp(pos);
    file.write(reinterpret_cast<const char*>(&record), sizeof(TVshow));
    file.flush();

    return !file.fail();
}

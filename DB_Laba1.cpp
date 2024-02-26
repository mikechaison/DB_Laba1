#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdio>

using namespace std;
int c=0;

struct TVshow{
    uint32_t id = 0;
    char name[50] = {};
    char category[50] = {};
    int64_t first_host = -1;
    bool exists = true;
};

struct TVhost{
    uint32_t ssn = 0;
    char email[100] = {};
    char phone_number[25] = {};
    char first_name[50] = {};
    char second_name[50] = {};
    char birth_date[25] = {};
    int64_t next = -1;
    bool exists = true;
};

void drawHorizontalLine(int width, char lineChar = '-') {
    for (int i = 0; i < width; ++i) {
        std::cout << lineChar;
    }
    std::cout << std::endl;
}

void drawShowTableRow(TVshow record, bool title, bool ut, streampos pos) {
    std::cout << "|";
    int idcolwidth=10;
    int colwidth=20;
    if (!title){
        std::cout << std::left << std::setw(idcolwidth) << record.id;
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << record.name;
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << record.category;
        if (ut){
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << record.first_host;
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << pos;
            std::cout << "|";
            string exist="FALSE";
            if (record.exists){
                exist="TRUE";
            }
            std::cout << std::left << std::setw(colwidth) << exist;
        }
        std::cout << "|" << std::endl;
    }
    else{
        std::cout << std::left << std::setw(idcolwidth) << "ID";
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << "NAME";
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << "CATEGORY";
        if (ut){
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << "FIRST HOST";
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << "ADDRESS";
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << "PRESENT";
        }
        std::cout << "|" << std::endl;
    }

}

void drawHostTableRow(TVhost record, bool title, bool ut, streampos pos) {
    std::cout << "|";
    int idcolwidth=10;
    int colwidth=20;
    if (!title){
        std::cout << std::left << std::setw(idcolwidth) << record.ssn;
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << record.first_name;
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << record.second_name;
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << record.email;
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << record.phone_number;
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << record.birth_date;
        if (ut){
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << record.next;
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << pos;
            std::cout << "|";
            string exist="FALSE";
            if (record.exists){
                exist="TRUE";
            }
            std::cout << std::left << std::setw(colwidth) << exist;
        }
        std::cout << "|" << std::endl;
    }
    else{
        std::cout << std::left << std::setw(idcolwidth) << "SSN";
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << "FIRST NAME";
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << "SECOND NAME";
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << "E-MAIL ADDRESS";
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << "PHONE NUMBER";
        std::cout << "|";
        std::cout << std::left << std::setw(colwidth) << "BIRTH DATE";
        if (ut){
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << "NEXT HOST";
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << "ADDRESS";
            std::cout << "|";
            std::cout << std::left << std::setw(colwidth) << "PRESENT";
        }
        std::cout << "|" << std::endl;
    }
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

void PrintNodesShow(std::fstream& file, vector<pair<int, streampos>>& ind)
{
    TVshow tmp;

    int showtablewidth=54;
    drawHorizontalLine(showtablewidth);
    drawShowTableRow(tmp, true, false, -1);
    drawHorizontalLine(showtablewidth);

    for (int i=1; i<ind.size(); i++){
        if (!ReadShow(tmp, file, ind[i].second))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return;
        }
        if (tmp.exists){
            drawShowTableRow(tmp, false, false, -1);

        }
    }
    drawHorizontalLine(showtablewidth);
}

void PrintNodesShowUT(std::fstream& file, vector<pair<int, streampos>>& ind)
{
    TVshow tmp;

    int showtablewidth=117;
    drawHorizontalLine(showtablewidth);
    drawShowTableRow(tmp, true, true, -1);
    drawHorizontalLine(showtablewidth);

    file.seekg(0, std::ios::end);
    std::streampos endpos=file.tellg();

    for (int pos=0; pos<endpos; pos+=sizeof(tmp)){
        if (!ReadShow(tmp, file, pos))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return;
        }
        drawShowTableRow(tmp, false, true, pos);
    }
    drawHorizontalLine(showtablewidth);
}

void PrintNodesHost(std::fstream& file, vector<pair<int, streampos>>& ind, std::fstream& master_file)
{
    TVshow master_tmp;
    TVhost tmp;

    int showtablewidth=117;
    drawHorizontalLine(showtablewidth);
    drawHostTableRow(tmp, true, false, -1);
    drawHorizontalLine(showtablewidth);


    for (int i=1; i<ind.size(); i++){
        if (!ReadShow(master_tmp, master_file, ind[i].second))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return;
        }
        if (master_tmp.exists){
            streampos next_ptr=master_tmp.first_host;
            while (next_ptr!=-1)
            {
                if (!ReadHost(tmp, file, next_ptr))
                {
                    std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                    return;
                }
                if (tmp.exists){
                    drawHostTableRow(tmp, false, false, next_ptr);
                }
                next_ptr=tmp.next;
            }
        }
    }
    drawHorizontalLine(showtablewidth);
}

void PrintNodesHostUT(std::fstream& file, vector<pair<int, streampos>>& ind, std::fstream& master_file)
{
    TVshow master_tmp;
    TVhost tmp;

    int showtablewidth=180;
    drawHorizontalLine(showtablewidth);
    drawHostTableRow(tmp, true, true, -1);
    drawHorizontalLine(showtablewidth);

    file.seekg(0, std::ios::end);
    std::streampos endpos=file.tellg();

    for (int pos=0; pos<endpos; pos+=sizeof(tmp)){
        if (!ReadHost(tmp, file, pos))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return;
        }
        drawHostTableRow(tmp, false, true, pos);
    }
    drawHorizontalLine(showtablewidth);
}

bool GetShow(std::fstream& file, std::vector<pair<int, streampos>>& ind, string query)
{
    TVshow record;
    streampos pos=0;

    query=query.substr(1);
    if (query=="all"){
        PrintNodesShow(file, ind);
        return true;
    }

    string numba="";
    int index=-1;
    for (int i=0; i<query.size(); i++)
    {
        if (query[i]==' ')
        {
            index=i+1;
            break;
        }
        numba.push_back(query[i]);
    }
    string field="";
    if (index!=-1)
    {
        field=query.substr(index);
    }
    int num=stoi(numba);

    for (int i=1; i<ind.size(); i++)
    {
        if (ind[i].first==num)
        {
            pos=ind[i].second;
            break;
        }
    }

    if (!ReadShow(record, file, pos))
    {
        cout<<"Node was not found!"<<endl;
        return false;
    }

    if (record.exists){
        if (field==""){
            int showtablewidth=54;
            drawHorizontalLine(showtablewidth);
            drawShowTableRow(record, true, false, -1);
            drawHorizontalLine(showtablewidth);
            drawShowTableRow(record, false, false, -1);
            drawHorizontalLine(showtablewidth);
        }
        else if (field=="name"){
            cout<<record.name<<endl;
        }
        else if (field=="category"){
            cout<<record.category<<endl;
        }
        return true;
    }
    cout<<"Node was not found!"<<endl;
    return false;
}

bool GetHost(std::fstream& file, std::vector<pair<int, streampos>>& ind, std::fstream& master_file, string query)
{
    TVshow master_tmp;
    TVhost tmp;

    query=query.substr(1);
    if (query=="all"){
        PrintNodesHost(file, ind, master_file);
        return true;
    }

    string numba="";
    int index=-1;
    for (int i=0; i<query.size(); i++)
    {
        if (query[i]==' ')
        {
            index=i+1;
            break;
        }
        numba.push_back(query[i]);
    }
    string field="";
    if (index!=-1)
    {
        field=query.substr(index);
    }
    int num=stoi(numba);

    for (int i=1; i<ind.size(); i++){
        if (!ReadShow(master_tmp, master_file, ind[i].second))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
        streampos next_ptr=master_tmp.first_host;
        while (next_ptr!=-1)
        {
            if (!ReadHost(tmp, file, next_ptr))
            {
                std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                return false;
            }
            if (tmp.ssn==num && tmp.exists){
                if (field==""){
                    int showtablewidth=117;
                    drawHorizontalLine(showtablewidth);
                    drawHostTableRow(tmp, true, false, next_ptr);
                    drawHorizontalLine(showtablewidth);
                    drawHostTableRow(tmp, false, false, next_ptr);
                    drawHorizontalLine(showtablewidth);
                }
                else if (field=="first_name"){
                    cout<<tmp.first_name<<endl;
                }
                else if (field=="second_name"){
                    cout<<tmp.second_name<<endl;
                }
                else if (field=="email"){
                    cout<<tmp.email<<endl;
                }
                else if (field=="phone_number"){
                    cout<<tmp.phone_number<<endl;
                }
                else if (field=="birth_date"){
                    cout<<tmp.birth_date<<endl;
                }
                return true;
            }
            next_ptr=tmp.next;
        }
    }
    cout<<"Node was not found!"<<endl;
    return false;
}

bool UpdateShow(std::fstream& file, std::vector<pair<int, streampos>>& ind, int num, string field, string value)
{
    TVshow record;
    streampos pos=0;
    for (int i=1; i<ind.size(); i++)
    {
        if (ind[i].first==num)
        {
            pos=ind[i].second;
            break;
        }
    }

    if (!ReadShow(record, file, pos))
    {
        cout<<"Node was not found!"<<endl;
        return false;
    }

    if (record.exists){
        if (field=="name"){
            strcpy(record.name, value.c_str());
        }
        else if (field=="category"){
            strcpy(record.category, value.c_str());
        }
        if (!WriteShow(record, file, pos))
        {
            return false;
        }

        return true;
    }
    cout<<"Node was not found!"<<endl;
    return false;
}

bool UpdateHost(std::fstream& file, std::vector<pair<int, streampos>>& ind, std::fstream& master_file, int num, string field, string value)
{
    TVshow master_tmp;
    TVhost tmp;

    for (int i=1; i<ind.size(); i++){
        if (!ReadShow(master_tmp, master_file, ind[i].second))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
        streampos next_ptr=master_tmp.first_host;
        while (next_ptr!=-1)
        {
            if (!ReadHost(tmp, file, next_ptr))
            {
                std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                return false;
            }
            if (tmp.ssn==num && tmp.exists){
                if (field=="first_name"){
                    strcpy(tmp.first_name, value.c_str());
                }
                else if (field=="second_name"){
                    strcpy(tmp.second_name, value.c_str());
                }
                else if (field=="email"){
                    strcpy(tmp.email, value.c_str());
                }
                else if (field=="phone_number"){
                    strcpy(tmp.phone_number, value.c_str());
                }
                else if (field=="birth_date"){
                    strcpy(tmp.birth_date, value.c_str());
                }
                if (!WriteHost(tmp, file, next_ptr))
                {
                    return false;
                }
                return true;
            }
            next_ptr=tmp.next;
        }
    }
    cout<<"Node was not found!"<<endl;
    return false;
}

bool AddNodeShow(std::fstream& file, std::vector<pair<int, streampos>>& ind, std::vector<streampos> &rubbish)
{
    TVshow record = {ind[ind.size()-1].first+1};
    cin>>record.name;
    cin>>record.category;

    std::streampos pos;
    if (rubbish.empty())
    {
        file.seekg(0, std::ios::end);
        std::streampos pos=file.tellg();
    }
    else{
        pos=rubbish[rubbish.size()-1];
        rubbish.pop_back();
    }

    if (!WriteShow(record, file, pos))
    {
        return false;
    }

    ind.push_back(pair<int, streampos> (ind[ind.size()-1].first+1, pos));
    sort(ind.begin(), ind.end());
    file.seekg(0, std::ios::end);
    std::streampos next_pos=file.tellg();
    if (rubbish.empty())
    {
        rubbish.push_back(next_pos);
    }


    return true;

}

bool AddNodeHost(std::fstream& file, std::fstream& master_file, vector<pair<int, streampos>>& ind, std::vector<streampos>& slave_rubbish)
{
    TVhost record;
    cin>>record.ssn;
    cin>>record.first_name;
    cin>>record.second_name;
    cin>>record.email;
    cin>>record.phone_number;
    cin>>record.birth_date;

    uint32_t show_id;
    cin>>show_id;
    streampos master_pos=-1;
    for (int i=1; i<ind.size(); i++)
    {
        if (ind[i].first==show_id)
        {
            master_pos=ind[i].second;
            break;
        }
    }
    TVshow master_record;

    if (!ReadShow(master_record, master_file, master_pos))
    {
        cout<<"Master node was not found!"<<endl;
        return false;
    }

    std::streampos pos;
    if (slave_rubbish.empty())
    {
        file.seekg(0, std::ios::end);
        std::streampos pos=file.tellg();
    }
    else{
        pos=slave_rubbish[slave_rubbish.size()-1];
        slave_rubbish.pop_back();
    }

    streampos prev_record_pos=master_record.first_host;
    record.next=prev_record_pos;
    master_record.first_host=pos;

    if (!WriteHost(record, file, pos))
    {
        return false;
    }

    if (!WriteShow(master_record, master_file, master_pos))
    {
        cout<<"Master node was not found!"<<endl;
        return false;
    }

    file.seekg(0, std::ios::end);
    std::streampos next_pos=file.tellg();
    if (slave_rubbish.empty())
    {
        slave_rubbish.push_back(next_pos);
    }

    return true;
}

bool RemoveNodeHost(std::fstream& file, std::vector<pair<int, streampos>>& ind, std::fstream& master_file, std::vector<streampos>& slave_rubbish, int num)
{
    TVshow master_tmp;
    TVhost tmp;
    TVhost prev_tmp;

    for (int i=1; i<ind.size(); i++){
        if (!ReadShow(master_tmp, master_file, ind[i].second))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
        streampos next_ptr=master_tmp.first_host;
        streampos prev_ptr=-1;
        while (next_ptr!=-1)
        {
            if (!ReadHost(tmp, file, next_ptr))
            {
                std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                return false;
            }
            if (tmp.ssn==num){
                if (prev_ptr==-1)
                {
                    master_tmp.first_host=tmp.next;
                    if (!WriteShow(master_tmp, master_file, ind[i].second))
                    {
                        std::cerr << "Unable to update next_ptr. Error: write failed" << std::endl;
                        return false;
                    }
                }
                else{
                    if (!ReadHost(prev_tmp, file, prev_ptr))
                    {
                        std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                        return false;
                    }
                    prev_tmp.next=tmp.next;
                    if (!WriteHost(prev_tmp, file, prev_ptr))
                    {
                        return false;
                    }
                }
                tmp.exists=false;
                if (!WriteHost(tmp, file, next_ptr))
                {
                    std::cerr << "Unable to update next_ptr. Error: write failed" << std::endl;
                    return false;
                }
                slave_rubbish.push_back(next_ptr);
                c++;
                return true;
            }
            prev_ptr=next_ptr;
            next_ptr=tmp.next;
        }
    }

    return false;
}

bool RemoveNodeShow(std::fstream& master_file, std::vector<pair<int, streampos>>& ind, std::fstream& slave_file,
                    std::vector<streampos>& rubbish, std::vector<streampos>& slave_rubbish, int num)
{
    streampos addr;
    int index;
    for (int i=1; i<ind.size(); i++)
    {
        if (ind[i].first==num)
        {
            addr=ind[i].second;
            index=i;
            break;
        }
    }
    ind.erase(ind.begin()+index);
    TVshow master_tmp;
    TVshow show_tmp;
    TVhost tmp;
    if (!ReadShow(master_tmp, master_file, addr))
    {
        std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
        return false;
    }
    master_tmp.exists=0;
    if (!WriteShow(master_tmp, master_file, addr))
    {
        std::cerr << "Unable to update next_ptr. Error: write failed" << std::endl;
        return false;
    }
    c++;
    rubbish.push_back(addr);
    streampos next_ptr=master_tmp.first_host;
    while (next_ptr!=-1)
    {
        if (!ReadHost(tmp, slave_file, next_ptr))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
        tmp.exists=0;
        if (!WriteHost(tmp, slave_file, next_ptr))
        {
            std::cerr << "Unable to update next_ptr. Error: write failed" << std::endl;
            return false;
        }
        c++;
        slave_rubbish.push_back(next_ptr);
        next_ptr=tmp.next;
    }

    return true;
}

void CalcNodesShow(std::fstream& file, vector<pair<int, streampos>>& ind)
{
    TVshow tmp;
    int calc=0;

    for (int i=1; i<ind.size(); i++){
        if (!ReadShow(tmp, file, ind[i].second))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return;
        }
        if (tmp.exists){
            calc++;
        }
    }
    std::cout<<"There are "<<calc<<" TV Show nodes"<<endl;
}

void CalcNodesHost(std::fstream& file, vector<pair<int, streampos>>& ind, std::fstream& master_file)
{
    TVshow master_tmp;
    TVhost tmp;
    int calc=0;

    for (int i=1; i<ind.size(); i++){
        if (!ReadShow(master_tmp, master_file, ind[i].second))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return;
        }
        streampos next_ptr=master_tmp.first_host;
        while (next_ptr!=-1)
        {
            if (!ReadHost(tmp, file, next_ptr))
            {
                std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                return;
            }
            if (tmp.exists){
                calc++;
            }
            next_ptr=tmp.next;
        }
    }
    std::cout<<"There are "<<calc<<" TV Host nodes"<<endl;
}

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

void print_help()
{
    cout<<"Laboratory work 1. Made by Vadym Mynko, group K-24\n"<<
    "List of commands:\n"<<
    "get-m: finds and prints info about a certain Show\n"<<
    "get-s: finds and prints info about a certain Host\n"<<
    "del-m: finds and deletes info about a certain Show ans all of its Hosts\n"<<
    "del-s: finds and deletes info about a certain Host\n"<<
    "update-m: updates the value of a certain field in a certain Show\n"<<
    "update-s: updates the value of a certain field in a certain Host\n"<<
    "insert-m: inserts the new Show\n"<<
    "insert-s: inserts the new Host\n"<<
    "calc-m: calculates the number of Shows\n"<<
    "calc-s: calculates the number of Hosts\n"<<
    "ut-m: prints info about all Shows\n"<<
    "ut-s: prints info about all Hosts\n"<<
    "exit: ends the program\n";
}

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
        if (!WriteShow(master_tmp, master_temp, pos))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
        pos+=sizeof(master_tmp);
    }

    int res;

    open_file(&master_file, "master.bin", false);

    streampos endpos;

    master_temp.seekg(0, std::ios::end);
    endpos=master_temp.tellg();

    for (int pos=0; pos<endpos; pos+=sizeof(master_tmp))
    {
        if (!ReadShow(master_tmp, master_temp, pos))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
        if (!WriteShow(master_tmp, master_file, pos))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
    }

    master_temp.close();
    res = remove(".\\master_temp.bin");

    open_file(&slave_file, "slave.bin", false);

    slave_temp.seekg(0, std::ios::end);
    endpos=slave_temp.tellg();

    for (int pos=0; pos<endpos; pos+=sizeof(tmp))
    {
        if (!ReadHost(tmp, slave_temp, pos))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
        if (!WriteHost(tmp, slave_file, pos))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
    }

    slave_temp.close();
    res = remove(".\\slave_temp.bin");

    return true;
}

void OnExit(std::fstream& masterind_file, std::fstream& master_file, std::fstream& slave_file, std::vector<pair<int, streampos>>& ind,
            vector<streampos>& rubbish, vector<streampos>& slave_rubbish)
{
    reorganize(master_file, slave_file, ind, rubbish, slave_rubbish);
    WriteIndex(masterind_file, ind);
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
        if (c>=max_del)
        {
            reorganize(master_file, slave_file, ind, rubbish, slave_rubbish);
            c=0;
        }
    }
    return 0;
}

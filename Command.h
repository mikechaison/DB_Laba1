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
#include "FileProcess.h"

using namespace std;

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

void PrintNodesHostByShow(std::fstream& file, vector<pair<int, streampos>>& ind, std::fstream& master_file, int num)
{
    TVshow master_tmp;
    TVhost tmp;

    int showtablewidth=117;
    drawHorizontalLine(showtablewidth);
    drawHostTableRow(tmp, true, false, -1);
    drawHorizontalLine(showtablewidth);


    for (int i=1; i<ind.size(); i++){
        if (ind[i].first==num){
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
            break;
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
    streampos pos=-1;

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

    int num=-1;

    try{
        num=stoi(numba);
    }
    catch (invalid_argument)
    {
        cout<<"Wrong data type!"<<endl;
        return false;
    }

    for (int i=1; i<ind.size(); i++)
    {
        if (ind[i].first==num)
        {
            pos=ind[i].second;
            break;
        }
    }

    if (pos==-1)
    {
        cout<<"Node was not found!"<<endl;
        return false;
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
        else{
            cout<<"Argument does not exist!"<<endl;
            return false;
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
    if (query[0]=='m'){
        if (query.size()<3){
            cout<<"Wrong data type!"<<endl;
            return false;
        }
        query=query.substr(2);
        string numba="";
        int index=-1;
        int num=-1;
        for (int i=0; i<query.size(); i++)
        {
            if (query[i]==' ')
            {
                index=i+1;
                break;
            }
            numba.push_back(query[i]);
        }

        try{
            num=stoi(numba);
        }
        catch (invalid_argument)
        {
            cout<<"Wrong data type!"<<endl;
            return false;
        }

        int master_num=-1;
        for (int i=1; i<ind.size(); i++)
        {
            if (ind[i].first==num)
            {
                master_num=ind[i].first;
                break;
            }
        }

        if (master_num==-1){
            cout<<"Show with that ID doesn't exist!"<<endl;
            return false;
        }
        PrintNodesHostByShow(file, ind, master_file, num);
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

    int num=-1;

    try{
        num=stoi(numba);
    }
    catch (invalid_argument)
    {
        cout<<"Wrong data type!"<<endl;
        return false;
    }

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
                else{
                    cout<<"Argument does not exist!"<<endl;
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

bool UpdateShow(std::fstream& file, std::vector<pair<int, streampos>>& ind, int num, string field, string value)
{
    TVshow record;
    streampos pos=-1;
    for (int i=1; i<ind.size(); i++)
    {
        if (ind[i].first==num)
        {
            pos=ind[i].second;
            break;
        }
    }

    if (pos==-1)
    {
        cout<<"Node was not found"<<endl;
        return false;
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
        else
        {
            cout<<"Argument does not exist!"<<endl;
            return false;
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
                else{
                    cout<<"Argument does not exist!"<<endl;
                    return false;
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
        pos=file.tellg();
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
    TVshow master_tmp;
    TVhost tmp;
    cin>>record.ssn;
    if (cin.fail()){
        cout<<"Wrong data type!"<<endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }

    for (int i=1; i<ind.size(); i++){
        if (!ReadShow(master_tmp, master_file, ind[i].second))
        {
            std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
            return false;
        }
        if (master_tmp.exists){
            streampos next_ptr=master_tmp.first_host;
            while (next_ptr!=-1)
            {
                if (!ReadHost(tmp, file, next_ptr))
                {
                    std::cerr << "Unable to update next_ptr. Error: read failed" << std::endl;
                    return false;
                }
                if (tmp.exists && tmp.ssn==record.ssn){
                    cout<<"Host with that SSN exists!"<<endl;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return false;
                }
                next_ptr=tmp.next;
            }
        }
    }

    cin>>record.first_name;
    cin>>record.second_name;
    cin>>record.email;
    cin>>record.phone_number;
    cin>>record.birth_date;

    uint32_t show_id;
    cin>>show_id;
    if (cin.fail()){
        cout<<"Wrong data type!"<<endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    streampos master_pos=-1;
    for (int i=1; i<ind.size(); i++)
    {
        if (ind[i].first==show_id)
        {
            master_pos=ind[i].second;
            break;
        }
    }

    if (master_pos==-1){
        cout<<"Show with that ID doesn't exist!"<<endl;
        return false;
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
                return true;
            }
            prev_ptr=next_ptr;
            next_ptr=tmp.next;
        }
    }

    std::cerr << "Unable to find host with that SSN" << std::endl;
    return false;
}

bool RemoveNodeShow(std::fstream& master_file, std::vector<pair<int, streampos>>& ind, std::fstream& slave_file,
                    std::vector<streampos>& rubbish, std::vector<streampos>& slave_rubbish, int num)
{
    streampos addr;
    int index=-1;
    for (int i=1; i<ind.size(); i++)
    {
        if (ind[i].first==num)
        {
            addr=ind[i].second;
            index=i;
            break;
        }
    }

    if (index==-1){
        std::cerr << "Unable to find host with that ID" << std::endl;
        return false;
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
        slave_rubbish.push_back(next_ptr);
        next_ptr=tmp.next;
    }

    return true;
}

void CalcNodesShow(std::fstream& file, vector<pair<int, streampos>>& ind)
{
    int calc=ind.size()-1;
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



void OnExit(std::fstream& masterind_file, std::fstream& master_file, std::fstream& slave_file, std::vector<pair<int, streampos>>& ind,
            vector<streampos>& rubbish, vector<streampos>& slave_rubbish)
{
    reorganize(master_file, slave_file, ind, rubbish, slave_rubbish);
    WriteIndex(masterind_file, ind);
}

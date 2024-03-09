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

using namespace std;

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

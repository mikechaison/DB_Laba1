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
#include "Structures.h"

using namespace std;

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

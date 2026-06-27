#pragma once
#include <cstring>

struct Location {
    int id;
    char name[100];
    char type[50];
    char description[200];
    bool isEmergencyPoint;

    Location() : id(0), isEmergencyPoint(false) {
        name[0] = '\0'; type[0] = '\0'; description[0] = '\0';
    }

    Location(int id, const char* name, const char* type, const char* desc, bool emergency = false) {
        this->id = id;
        this->isEmergencyPoint = emergency;
        strncpy(this->name, name, 99);        this->name[99] = '\0';
        strncpy(this->type, type, 49);        this->type[49] = '\0';
        strncpy(this->description, desc, 199); this->description[199] = '\0';
    }
};

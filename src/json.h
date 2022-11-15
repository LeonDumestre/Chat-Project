#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* writeJSON(char message_type[], char message[]);

char* getCode(char json[]);

char* getValeurs(char json[]);

int getNbDigit(int number);
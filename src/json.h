#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* writeJSON(char message_type[], char message[], bool sendByServer);

char* getCode(char json[]);

char* getValeurs(char json[]);

int getNbDigit(int number);
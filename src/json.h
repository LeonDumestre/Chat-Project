
/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* writeJSON(char message_type[], char message[]);

void getCode();

char* getCalculeOperator(char json[]);

int* getCalculeMessage(char json[]);
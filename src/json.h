
/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* writeJSON(char message_type[], char message[]);

char* getCode(char json[]);

char* getValeurs(char json[]);

void getCalculeOperator(char json[]);

void getCalculeMessage(char json[]);
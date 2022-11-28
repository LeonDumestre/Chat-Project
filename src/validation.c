/* 
 * Le but principal de ce code est de s'assurer que les messages envoyés par le client sont
 * valides, c'est-à-dire que les messages respectent le format JSON et
 * respectent le protocole spécifié par le côté serveur.
 */

#include "validation.h"

bool validation_JSON(char json[]) {
    
    char codeJson[] = "{\"code\":\"";
    char valeursJson[] = ",\"valeurs\":[";
    bool inValeur = false;
    bool inQuote = false;
    bool inEnd = false;

    int ind = 0;
    
    // Vérifie le code
    if ((int)strlen(json) <= (int)strlen(codeJson)) return false;
    while (ind < (int)strlen(json))
    {
        if (ind < (int)strlen(codeJson) && json[ind] != codeJson[ind]) return false;
        else if (ind == (int)strlen(codeJson)) inValeur = true;
        else if (inValeur && json[ind] == '"')
        {
            inValeur = false;
            ind++;
            break;
        }
        ind++;
    }
    if (inValeur) return false;

    // Vérifie les valeurs
    int ind2 = 0;
    while (ind < (int)strlen(json))
    {
        if (ind2 < (int)strlen(valeursJson) && json[ind] != valeursJson[ind2]) return false;
        else if (ind2 == (int)strlen(valeursJson)) inValeur = true;
        else if (inValeur && json[ind] == '"') inQuote = true;
        else if (inValeur && inQuote && json[ind] == '"') 
        {
            inQuote = false;
            if (json[++ind] == ']') inValeur = false;
            else if (json[++ind] != ',') return false;
        }
        else if (!inValeur && !inQuote && json[ind] == ']') inEnd = true;
        else if (inEnd && json[ind] == '}' && ind == (int)strlen(json)-1) return true; 
        ind++;
        ind2++;
    }
    return true;
}
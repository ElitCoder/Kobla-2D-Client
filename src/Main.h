#ifndef MAIN_H
#define MAIN_H

#define TWOINT 1
#define TEXTCONFIG 2
#define ONEINT 3

#include <string>

int Connect();
int CheckUpdate();
void SaveConfig();
void AddConfigOption(int TYPE, char *arb, int int1, int int2, int param, char *name);
void RemoveConfigOption(char *name);

void getInput(int &intt);
std::string getCharInput(int &result);

void QuitFunction();
void Log(char *msg, ...);

#endif
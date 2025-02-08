#ifndef TRANSPILER_H
#define TRANSPILER_H

#include <Arduino.h>
#include <vector>
#include "main.h"

class transpiler {
  public:
    static void transpile(const char* input);
  private:
    static void processLine(const char* command, const std::vector<String>& args);
    static bool checkArgs(const std::vector<String>& args, uint8_t expected);
};

extern CH9329_Keyboard_ Keyboard;

#endif //TRANSPILER_H

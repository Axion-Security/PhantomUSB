#include "transpiler.h"

void transpiler::transpile(const char* input) {
    const String script(input);
    int start = 0;

    while (start < script.length()) {
        const int index = script.indexOf('\n', start);
        String line = (index == -1) ? script.substring(start) : script.substring(start, index);
        start = (index == -1) ? static_cast<int>(script.length()) : index + 1;

        line.trim();
        if (line.length() == 0) continue;

        const int cmdEnd = line.indexOf(' ');
        String command = (cmdEnd == -1) ? line : line.substring(0, cmdEnd);

        String argsStr = (cmdEnd == -1) ? "" : line.substring(cmdEnd + 1);
        argsStr.trim();
        std::vector<String> args;

        int argStart = 0;
        while (argStart < argsStr.length()) {
            const int spacePos = argsStr.indexOf(' ', argStart);

            if (spacePos == -1) {
                String arg = argsStr.substring(argStart);
                if (arg.length() > 0) args.push_back(arg);
                break;
            }

            if (spacePos > argStart) {
                String arg = argsStr.substring(argStart, spacePos);
                args.push_back(arg);
            }

            argStart = spacePos + 1;
        }

      processLine(command.c_str(), args);

    }
}

bool transpiler::checkArgs(const std::vector<String>& args, const uint8_t expected) {
    if (args.size() != expected) {
        return false;
    }

    return true;
}

void transpiler::processLine(const char* command, const std::vector<String>& args) {
    if (strcmp(command, "write") == 0) {
        if (!checkArgs(args, 1)) return;
        Keyboard.print(args[0]);
    }

    if (strcmp(command, "writeLn") == 0) {
        if (!checkArgs(args, 1)) return;
        Keyboard.print(args[0]);
        Keyboard.press(KEY_RETURN);
        Keyboard.release(KEY_RETURN);
    }

    if (strcmp(command, "delay") == 0) {
        if (!checkArgs(args, 1)) return;
        delay(args[0].toInt());
    }

    if (strcmp(command, "terminal") == 0) {
        if (!checkArgs(args, 1)) return;
        if (args[0] == "windows") {
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press('r');
            Keyboard.releaseAll();

            delay(800);

            Keyboard.print("cmd");
            Keyboard.press(KEY_RETURN);
            Keyboard.releaseAll();
        }
    }

}
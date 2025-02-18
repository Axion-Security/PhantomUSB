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
        bool inQuotes = false;
        while (argStart < argsStr.length()) {
            if (argsStr[argStart] == '\"') {
                inQuotes = !inQuotes;
                argStart++;
                continue;
            }

            const int spacePos = inQuotes ? argsStr.indexOf('\"', argStart) : argsStr.indexOf(' ', argStart);

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

    Serial.println( "Invalid number of arguments" );

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
        } else if (args[0] == "linux") {
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press('t');
            Keyboard.releaseAll();
        } else if (args[0] == "mac") {
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press(' ');
            Keyboard.releaseAll();

            delay(800);

            Keyboard.print("terminal");
            Keyboard.press(KEY_RETURN);
            Keyboard.releaseAll();
        }
    }

    if (strcmp(command, "key") == 0) {
        if (!checkArgs(args, 1)) return;

        pressKey(args[0]);

        Keyboard.releaseAll();
    }

    if (strcmp(command, "comboKey") == 0) {
        if (!checkArgs(args, 1)) return;

        std::vector<String> keys = split(args[0], '+');
        for (const String& key : keys) {
            pressKey(key);
        }
        Keyboard.releaseAll();
    }

    if (strcmp(command, "arrowKey") == 0) {
        if (!checkArgs(args, 1)) return;
        if (args[0] == "up") Keyboard.press(KEY_UP_ARROW);
        else if (args[0] == "down") Keyboard.press(KEY_DOWN_ARROW);
        else if (args[0] == "left") Keyboard.press(KEY_LEFT_ARROW);
        else if (args[0] == "right") Keyboard.press(KEY_RIGHT_ARROW);
        else return;

        Keyboard.releaseAll();
    }

    if (strcmp(command, "keyboardLayout") == 0)
    {
        if (!checkArgs(args, 1)) return;

        Keyboard.end();
        if (args[0] == "German") {
            KeyboardLayout = KeyboardLayout_de_DE;
        } else if (args[0] == "English") {
            KeyboardLayout = KeyboardLayout_en_US;
        } else if (args[0] == "French") {
            KeyboardLayout = KeyboardLayout_fr_FR;
        } else if (args[0] == "Spanish") {
            KeyboardLayout = KeyboardLayout_es_ES;
        } else if (args[0] == "Portuguese") {
            KeyboardLayout = KeyboardLayout_pt_PT;
        } else if (args[0] == "Italian") {
            KeyboardLayout = KeyboardLayout_it_IT;
        } else if (args[0] == "Danish") {
            KeyboardLayout = KeyboardLayout_da_DK;
        } else if (args[0] == "Swedish"){
            KeyboardLayout = KeyboardLayout_sv_SE;
        }
        Keyboard.begin(ch9329Serial, KeyboardLayout);
    }
}

std::vector<String> transpiler::split(const String& str, const char delimiter) {
    std::vector<String> tokens;
    String token = "";

    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == delimiter) {
            if (token.length() > 0) {
                tokens.push_back(token);
                token = "";
            }
        } else {
            token += str[i];
        }
    }
    if (token.length() > 0) {
        tokens.push_back(token);
    }
    return tokens;
}

void transpiler::pressKey(const String& key) {
    if (key == "enter") Keyboard.press(KEY_RETURN);
    else if (key == "esc") Keyboard.press(KEY_ESC);
    else if (key == "backspace") Keyboard.print(" ");
    else if (key == "tab") Keyboard.press(KEY_TAB);
    else if (key == "space") Keyboard.press(KEY_BACKSPACE);
    else if (key == "delete") Keyboard.press(KEY_DELETE);
    else if (key == "insert") Keyboard.press(KEY_INSERT);
    else if (key == "home") Keyboard.press(KEY_HOME);
    else if (key == "end") Keyboard.press(KEY_END);
    else if (key == "pageup") Keyboard.press(KEY_PAGE_UP);
    else if (key == "pagedown") Keyboard.press(KEY_PAGE_DOWN);
    else if (key == "up") Keyboard.press(KEY_UP_ARROW);
    else if (key == "down") Keyboard.press(KEY_DOWN_ARROW);
    else if (key == "left") Keyboard.press(KEY_LEFT_ARROW);
    else if (key == "right") Keyboard.press(KEY_RIGHT_ARROW);
    else if (key == "ctrl") Keyboard.press(KEY_LEFT_CTRL);
    else if (key == "shift") Keyboard.press(KEY_LEFT_SHIFT);
    else if (key == "alt") Keyboard.press(KEY_LEFT_ALT);
    else if (key == "gui") Keyboard.press(KEY_LEFT_GUI);
    else if (key == "capslock") Keyboard.press(KEY_CAPS_LOCK);
    else if (key == "numlock") Keyboard.press(KEY_NUM_LOCK);
    else if (key == "prtsc") Keyboard.press(KEY_PRINT_SCREEN);
    else if (key == "scrolllock") Keyboard.press(KEY_SCROLL_LOCK);
    else if (key == "pause") Keyboard.press(KEY_PAUSE);
    else if (key.startsWith("F")) {
        const int fKey = key.substring(1).toInt();
        if (fKey >= 1 && fKey <= 24) {
            Keyboard.press(KEY_F1 + (fKey - 1));
        }
    } else if (key.length() == 1) {
        Keyboard.press(key[0]);
    }
}
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
    if (strcmp(command, "write") == 0) { // write "<text>"
        Serial.println( "write" );
        if (!checkArgs(args, 1)) return;
        Keyboard.print(args[0]);
    }

    if (strcmp(command, "writeLn") == 0) { // writeLn "<text>"
        if (!checkArgs(args, 1)) return;
        Keyboard.print(args[0]);
        Keyboard.press(KEY_RETURN);
        Keyboard.release(KEY_RETURN);
    }

    if (strcmp(command, "delay") == 0) { // delay "<integer>"
        if (!checkArgs(args, 1)) return;
        delay(args[0].toInt());
    }

    if (strcmp(command, "terminal") == 0) { // terminal "<operating_system>"
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

    if (strcmp(command, "winShutdown") == 0)
    {
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('X');
        Keyboard.releaseAll();

        delay(1000);

        Keyboard.press('U');
        Keyboard.releaseAll();

        Keyboard.press('U');
        Keyboard.releaseAll();
    }

    if (strcmp(command, "key") == 0)
    {
        if (!checkArgs(args, 1)) return;

        if (args[0] == "enter") Keyboard.press(KEY_RETURN);
        else if (args[0] == "esc") Keyboard.press(KEY_ESC);
        else if (args[0] == "backspace") Keyboard.press(KEY_BACKSPACE);
        else if (args[0] == "tab") Keyboard.press(KEY_TAB);
        else if (args[0] == "space") Keyboard.press(' ');
        else if (args[0] == "delete") Keyboard.press(KEY_DELETE);
        else if (args[0] == "insert") Keyboard.press(KEY_INSERT);
        else if (args[0] == "home") Keyboard.press(KEY_HOME);
        else if (args[0] == "end") Keyboard.press(KEY_END);
        else if (args[0] == "pageup") Keyboard.press(KEY_PAGE_UP);
        else if (args[0] == "pagedown") Keyboard.press(KEY_PAGE_DOWN);
        else if (args[0] == "up") Keyboard.press(KEY_UP_ARROW);
        else if (args[0] == "down") Keyboard.press(KEY_DOWN_ARROW);
        else if (args[0] == "left") Keyboard.press(KEY_LEFT_ARROW);
        else if (args[0] == "right") Keyboard.press(KEY_RIGHT_ARROW);
        else if (args[0] == "ctrl") Keyboard.press(KEY_LEFT_CTRL);
        else if (args[0] == "shift") Keyboard.press(KEY_LEFT_SHIFT);
        else if (args[0] == "alt") Keyboard.press(KEY_LEFT_ALT);
        else if (args[0] == "gui") Keyboard.press(KEY_LEFT_GUI);
        else if (args[0] == "capslock") Keyboard.press(KEY_CAPS_LOCK);
        else if (args[0] == "numlock") Keyboard.press(KEY_NUM_LOCK);
        else if (args[0] == "prtsc") Keyboard.press(KEY_PRINT_SCREEN);
        else if (args[0] == "scrolllock") Keyboard.press(KEY_SCROLL_LOCK);
        else if (args[0] == "pause") Keyboard.press(KEY_PAUSE);
        else return;

        Keyboard.releaseAll();
    }

    if (strcmp(command, "functionKey") == 0) // functionKey "<F1-F24>"
    {
        if (!checkArgs(args, 1)) return;
        if (args[0] == "F1") Keyboard.press(KEY_F1);
        else if (args[0] == "F2") Keyboard.press(KEY_F2);
        else if (args[0] == "F3") Keyboard.press(KEY_F3);
        else if (args[0] == "F4") Keyboard.press(KEY_F4);
        else if (args[0] == "F5") Keyboard.press(KEY_F5);
        else if (args[0] == "F6") Keyboard.press(KEY_F6);
        else if (args[0] == "F7") Keyboard.press(KEY_F7);
        else if (args[0] == "F8") Keyboard.press(KEY_F8);
        else if (args[0] == "F9") Keyboard.press(KEY_F9);
        else if (args[0] == "F10") Keyboard.press(KEY_F10);
        else if (args[0] == "F11") Keyboard.press(KEY_F11);
        else if (args[0] == "F12") Keyboard.press(KEY_F12);
        else if (args[0] == "F13") Keyboard.press(KEY_F13);
        else if (args[0] == "F14") Keyboard.press(KEY_F14);
        else if (args[0] == "F15") Keyboard.press(KEY_F15);
        else if (args[0] == "F16") Keyboard.press(KEY_F16);
        else if (args[0] == "F17") Keyboard.press(KEY_F17);
        else if (args[0] == "F18") Keyboard.press(KEY_F18);
        else if (args[0] == "F19") Keyboard.press(KEY_F19);
        else if (args[0] == "F20") Keyboard.press(KEY_F20);
        else if (args[0] == "F21") Keyboard.press(KEY_F21);
        else if (args[0] == "F22") Keyboard.press(KEY_F22);
        else if (args[0] == "F23") Keyboard.press(KEY_F23);
        else if (args[0] == "F24") Keyboard.press(KEY_F24);
        else return;

        Keyboard.releaseAll();
    }

    if (strcmp(command, "arrowKey") == 0) // arrowKey "<up/down/left/right>"
    {
        if (!checkArgs(args, 1)) return;
        if (args[0] == "up") Keyboard.press(KEY_UP_ARROW);
        else if (args[0] == "down") Keyboard.press(KEY_DOWN_ARROW);
        else if (args[0] == "left") Keyboard.press(KEY_LEFT_ARROW);
        else if (args[0] == "right") Keyboard.press(KEY_RIGHT_ARROW);
        else return;

        Keyboard.releaseAll();
    }

    if (strcmp(command, "clipboard") == 0) {
        if (!checkArgs(args, 1)) return;
        if (args[0] == "copy") {
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('c');
            Keyboard.releaseAll();
        } else if (args[0] == "paste") {
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press('v');
            Keyboard.releaseAll();
        }
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


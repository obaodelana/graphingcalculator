#include "../Headers/Global.h"

const int minChars = 7, maxChars = 48;
// Characters that are not allowd to be typed
const char *forbiddenChars = "@$%&_|=`~QqWwERYyUuPDdFfGgHhJjKkLZzXvVBMmNTASIiCO,.?'\";:\\{[}](<> ";
// Characters that start functions
const char *startLetters = "al";
// Equation string
char equation[49] = "f(x) = ";
// Last key typed
char keyTyped = ' ';
bool allowOnlyNumbers = false, lastIsSpace = false;
int bracketsOpened = 0;

// Position of equation box
Rectangle equationBox = (Rectangle) {screenWidth / 2 - (850 / 2), screenHeight / 6, 850, 80};

void SendEquation(void);

// Number of characters to remove
int CharactersToRemove(void)
{
    // Index of last character
    int len = strlen(equation) - 1;

    // If a digit or [x, !, ^, e, #, )] is the last character, return 1
    if (isdigit(equation[len]) || strchr("!x^e#)", equation[len]) != NULL)
        return 1;
    // If i in pi is the last character, return 2
    else if (equation[len] == 'i')
        return 2;
    // Else count how many charcters till space (backwards)
    else
    {
        int r = 0;
        for (int i = len; equation[i] != ' '; i--, r++);
        return r;
    }
}

// Return function letters
const char* FunctionString(char letter)
{
    // Characters to send
    char *addChars = "";
    
    // (sin, cos, tan, atan, asin, acos) (make sure l is no typed before it)
    if (strchr("sct", letter) != NULL && keyTyped != 'l')
    {
        addChars = (letter == 's') ? "sin" : "cos";
        if (letter == 't')
            addChars = "tan";
    }
    // log or ln
    else if (strchr("on", letter) != NULL)
    {
        // Make sure l was typed before
        if (keyTyped == 'l')
            addChars = (letter == 'o') ? "og" : "n";
    }
    // abs (if current key typed is l and last typed is a)
    else if (letter == 'b' && keyTyped == 'a')
        addChars = "bs";
    // sqrt (make sure startLetters are not typed before it)
    else if (letter == 'r' && strchr(startLetters, keyTyped) == NULL)
        addChars = "sqrt";
    // Else return nothing
    else
        return "";

    // Functions open brackets
    bracketsOpened++;
    // Return function letters plus '('
    return TextFormat("%s(", addChars);
}

// Display equation on screen
void TypeText(void)
{
    // String length
    int equationLength = strlen(equation);
    // Key pressed
    char key = (char) GetKeyPressed();
    // Is last character a space
    lastIsSpace = equation[equationLength - 1] == ' ';

    // If a function or exponent is used, allow only numbers, [x, p, #, e] and backspace
    if (allowOnlyNumbers)
    {
        if (key == 'x' || key == 'p' || key == '#' || key == 'e' || isdigit(key) || IsKeyPressed(KEY_BACKSPACE))
            allowOnlyNumbers = false;
        else
            return;
    }

    // If the key is truthy, there are no bad characters and the length is less than max
    if (key && strchr(forbiddenChars, key) == NULL && equationLength < maxChars)
    {
        // If key is the start of a function
        if (strchr("sctbonr", key) != NULL)
        {
            // If there is enough space in the string to accommodate extra chars and no brackets are opened
            if (equationLength + (lastIsSpace ? 4 : 7) < maxChars && !bracketsOpened)
            {
                // (sin, cos, tan, atan, asin, acos, etc...)
                const char *addChars = FunctionString(key);

                // If function letter gotten
                if (strlen(addChars) > 0)
                {
                    // If this is not the start and no sign or startLetter has been used, add a multiplication sign before the function
                    if (equationLength > minChars && !lastIsSpace && strchr(startLetters, keyTyped) == NULL)
                        strcat(equation, " * ");
                    // Add the characters to the end of the string
                    strcat(equation, addChars);
                    
                    // Make sure only numbers are typed in a function
                    allowOnlyNumbers = true;
                    // Reset key typed
                    keyTyped = ' ';
                }
            }
        }

        // If not a function and startLetters have not been typed
        else if (strchr(startLetters, keyTyped) == NULL)
        {
            // If a sign is pressed
            if (strchr("*/^+-", key) != NULL)
            {
                // Make sure last letter is not a space and there is enough space to accomodate a value at the end
                if (!lastIsSpace && equationLength + 3 < maxChars)
                {
                    // Add to equation an exponent sign or arithmetic sign with spaces
                    strcat(equation, (key == '^') ? &key : TextFormat(" %c ", key));
                    // If exponent, allow numbers only to be typed
                    if (key == '^')
                        allowOnlyNumbers = true;
                }
            }

            // If a constant or rand
            else if (strchr("ep#", key) != NULL)
            {
                int space = (key == 'e') ? 1 : 2;
                // If there's enough space for it
                if (equationLength + space < maxChars)
                {
                    // If this is not the start, no sign or startLetter has been used and the last char is not [(, ^], add a multiplication sign before the string
                    if (equationLength > minChars && !lastIsSpace && strchr(startLetters, keyTyped) == NULL && equation[equationLength - 1] != '^' && equation[equationLength - 1] != '(')
                        strcat(equation, " * ");
                    
                    char *str = (key == 'e') ? "e" : "pi";
                    if (key == '#')
                        str = "#";
                    // Add to equation end
                    strcat(equation, str);
                }
            }

            else
            {
                // If a bracket close is typed
                if (key == ')')
                {
                    // If a bracket is currently open and something is typed in it, reduce brackets
                    if (bracketsOpened && !allowOnlyNumbers)
                        bracketsOpened--;
                    // Else don't allow to type
                    else
                        return;                    
                }

                // Disallow from typing start letters when brackets are opened
                else if (strchr(startLetters, key) != NULL && bracketsOpened)
                    return;

                // If [x, e, pi, #, !] was typed last or a is about to be typed and there's no sign behind it
                else if (strchr("xie!#)", equation[equationLength - 1]) != NULL || (key == 'a' && equationLength > minChars && equation[equationLength - 1] != ' '))
                {
                    // Only show a sign if a factorial is already used not when it's about to be used (make sure a factorial isn't used in front of a brace)
                    if (key != '!' || equation[equationLength - 1] == '!' || equation[equationLength - 1] == ')')
                    {
                        // If there is enough space for a sign and a number
                        if (equationLength + 4 < maxChars)
                            // Add a multiplication sign
                            strcat(equation, " * ");
                        // Else, don't allow to type
                        else
                            return;
                        
                        // Update equation length so space will become the last character
                        equationLength = strlen(equation);
                    }
                }

                // Save last key typed
                keyTyped = key;
                // If a factorial sign is pressed and there's nothing or another factorial behind it, add a 1 before it is typed
                if (key == '!' && (equation[equationLength - 1] == ' ' || equation[equationLength - 1] == '!'))
                    strcat(equation, "1");
                // Add the key pressed to the end of the string
                strcat(equation, &key);                    
            }
        }
    }

    // If the backspace key is pressed and the length of the string is greater than minimum characters allowed
    else if (IsKeyPressed(KEY_BACKSPACE) && equationLength > minChars)
    {
        // If current last character is a closed brace, increment bracketsOpened
        if (equation[equationLength - 1] == ')')
            bracketsOpened++;
        // If current last characters are open braces, decrement bracketsOpened
        else if (equation[equationLength - 1] == '(')
            bracketsOpened--;

        // Characters to remove will be 3 if the last letter is a sign, else calculate it
        int charactersR = equation[equationLength - 1] == ' ' ? 3 : CharactersToRemove();
        // Remove characters by putting a terminating character at new index 
        equation[equationLength - charactersR] = '\0';

        // If new last letter ends with a function or exponent sign, allow only numbers or x to be typed
        if (strchr("^nstg", equation[equationLength - charactersR - 1]) != NULL)
            allowOnlyNumbers = true;
    }

    // Send text to equation if enter is pressed
    if (IsKeyPressed(KEY_ENTER))
        SendEquation();
}

// Send equation to equation class for stripping
void SendEquation(void)
{
    // Make sure characters are greater than minimum characters and equation is complete
    if (strlen(equation) > minChars && !(lastIsSpace || allowOnlyNumbers) && strchr(startLetters, keyTyped) == NULL && !bracketsOpened)
    {
        // Strip eqaution starting from 7th index
        StripEquation(&equation[7]);
        // Send to graph
        GetYValues();
    }
}

void DrawTextbox(void)
{
    // Textbox rectangle lines
    DrawRectangleLines(equationBox.x, equationBox.y, equationBox.width, equationBox.height, (allowOnlyNumbers || strchr("al", keyTyped) != NULL || lastIsSpace || bracketsOpened) ? RED : LIGHTGRAY);

    // Equation text
    DrawText(equation, equationBox.x + 10, equationBox.y + 25, 30, LIGHTGRAY);
}
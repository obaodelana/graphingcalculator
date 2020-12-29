// Breaks down equation and computes it

#include <stdlib.h>
#include <math.h>
#include "../Headers/Global.h"

#define E 2.71828182845904523536f

// Letters that start functions
const char *functionStart = "asctrl";
// Index in equation sequence string array
int currentIndex = 0;
// Holds each part of an equation
char equationSequence[100][50];

void ClearSequence(void)
{
    // Reset y index of graph class
    ClearIndex();

    // If equation sequence is not empty
    if (currentIndex > 0)
    {
        // Go through each one
        for (int i = 0; i < currentIndex; i++)
            // Clear the text (copy 0 to each char of string)
            memset(equationSequence[i], 0, sizeof equationSequence[i]);
        // Reset current index
        currentIndex = 0;
    }
}

// Count occurences of character in string
int CharOccurences(char c, char *string)
{
    int occ = 0;
    // Loop till the end of the string
    for (int i = 0; *(string + i) != '\0'; i++)
    {
        // If the current character matches c, increment occ counter
        if (*(string + i) == c)
            occ++;
    }

    return occ;    
}

// Split equation string into array of strings
void StripEquation(char *equationText)
{
    ClearSequence();

    int len = strlen(equationText);
    for (int i = 0; i < len; i++)
    {
        // If number or [x, ^, e, p, #, !] add to current index
        if (isdigit(equationText[i]) || strchr("x^ep#!", equationText[i]) != NULL)
        {
            // If a constant or variable (not x)
            if (strchr("ep#", equationText[i]) != NULL)
            {
                // Replace e or pi with correct value
                float n = (equationText[i] == 'e') ? E : PI;
                // Replace # with a random value
                if (equationText[i] == '#')
                    n = GetRandomValue(0, 100);

                // Copy to current index value (pi, e or #)
                strcat(equationSequence[currentIndex], TextFormat("%f", n));
            }
            // [x, ^, !, num]
            else
                strncat(equationSequence[currentIndex], &equationText[i], 1);
        }

        // If a sgin
        else if (strchr("*/+-", equationText[i]) != NULL)
        {
            // Copy character to a new index
            strncpy(equationSequence[++currentIndex], &equationText[i], 1);
            // Move to next index if next index is not function
            if (strchr(functionStart, equationText[i + 2]) == NULL)
                currentIndex++;
        }

        // If a function
        else if (strchr(functionStart, equationText[i]) != NULL)
        {
            // If not the first index, move to next index
            if (currentIndex > 0)
                currentIndex++;

            // Create a string to hold function
            char function[100] = "";
            // Add the first two letters of the function to the function string
            strncpy(function, &equationText[i], 2);

            // Move to number part
            // Gets the memory address of '(' and subtracts it from the memory address of the first letter of the string and adds 1
            i = (int) (strchr(&equationText[i], '(') - equationText) + 1;
            // While in between braces add to function string
            while (CharOccurences(')', &equationText[i + 1]) >= 1)
                strncat(function, &equationText[i++], 1);
            // Increment by one to avoid adding the last bracket
            i++;

            // Copy function string to current index
            strcpy(equationSequence[currentIndex], function);

            // If not last character and there's no sign in front, incrememnt index
            if (equationText[i + 1] != '\0' && strchr("*/^+-", equationText[i + 1]) == NULL)
                currentIndex++;
        }
    }
    // Increment index so it will start at one
    currentIndex++;

    // Testing (prints out all strings)
    // for (int j = 0; j < currentIndex; j++)
    //     printf("%s\n", equationSequence[j]);
}

float RaiseToPower(float valToRaise, char *powerStr, int currentX)
{
    // Power string
    char powr[50] = "";
    // Go through string till the end or until factorial or another exponent is found (unwanted) 
    for (int j = 0; (*(powerStr + j) != '\0' && *(powerStr + j) != '^' && *(powerStr + j) != '!'); j++)
    {
        // Add numbers only
        if (*(powerStr + j) != 'x')
            strncat(powr, (powerStr + j), 1);
        // If x, copy into power string the multiplication of previous numbers by x, if x is the first just copy it
        else
            strcpy(powr, TextFormat("%f", (strlen(powr) <= 0) ? currentX : currentX * atof(powr)));
    }

    // Raise val to power
    return powf(valToRaise, atof(powr));
}

float GetNumber(char *string, int currentX)
{
    // Number string
    char val[50] = "";
    // Number value
    float numVal = 0;
    
    // Go through string
    for (int i = 0; (string[i] != '\0' && string[i] != '^' && string[i] != '!'); i++)
    {
        // Concatenate numbers to string
        if (isdigit(string[i]) || string[i] == '.')
            strncat(val, &string[i], 1);
        // If x found, copy into string the multpilication of x value with the number gotten so far (from string) (make sure string is not empty)
        else if (string[i] == 'x')
            strcpy(val, TextFormat("%f", (strlen(val) <= 0) ? currentX : (atof(val) * currentX)));
    }
    numVal = atof(val);

    // If there's a factorial (!)
    if (strchr(string, '!') != NULL)
    {
        // If lower than one, return 1
        if (numVal <= 1.0f)
            numVal = 1.0f;
        // Else get the factorial using a loop
        else
        {
            int highest = round(numVal);
            for (int i = 1; i < highest; i++)
                numVal *= i;
        }
    }

    // Look for exponent character in string
    char *exp = strchr(string, '^');
    // If there's an exponent
    if (exp != NULL)
    {
        // Index of exponent character (memory address of '^' - first letter of string)
        int index = (int) (exp - string);
        numVal = RaiseToPower(numVal, &string[index + 1], currentX);
    }

    return numVal;
}

void PerformArithmetic(float val, float *on, char sign)
{
    // Perform arithmetic depending on the sign
    switch (sign)
    {
        // Subtract
        case '-':
            *on -= val;
            break;

        // Multiply
        case '*':
            *on *= val;
            break;

        // Divide
        case '/':
            *on /= val;
            break;

        // Add (also add if no sign is given)
        case '+': default:
            *on += val;
            break;
    }
}

void UseFunction(float val, float *on, char *functionLetters, char sign)
{
    // Convert val into radians
    float radVal = val * DEG2RAD;
    float newVal = 0.0f;

    // sin
    if (!strcmp(functionLetters, "si"))
        newVal = sinf(radVal);
    // cos
    else if (!strcmp(functionLetters, "co"))
        newVal = cosf(radVal);
    // tan
    else if (!strcmp(functionLetters, "ta"))
        newVal = tanf(radVal);
    // asin
    else if (!strcmp(functionLetters, "as"))
        newVal = asinf(val) * RAD2DEG;
    // acos
    else if (!strcmp(functionLetters, "ac"))
        newVal = acosf(val) * RAD2DEG;
    // atan
    else if (!strcmp(functionLetters, "at"))
        newVal = atanf(val) * RAD2DEG;
    // sqrt
    else if (!strcmp(functionLetters, "sq"))
        newVal = sqrtf(val);
    // abs
    else if (!strcmp(functionLetters, "ab"))
        newVal = abs(val);
    // log
    else if (!strcmp(functionLetters, "lo"))
        newVal = log10f(val);
    // ln
    else if (!strcmp(functionLetters, "ln"))
        newVal = logf(val);

    if (*on > 0)
        PerformArithmetic(newVal, on, sign);
    else
        *on = newVal;
}

int ProcessEquation(float *yVals, int startX, int endX)
{
    // If equation has been stripped
    if (currentIndex > 0)
    {
        // Local copy of equation
        char strippedEquation[100][50];
        // Copy global equation sequence to local varible
        for (int i = 0; i < currentIndex; i++)
            strcpy(strippedEquation[i], equationSequence[i]);

        // Store current index
        int len = currentIndex;
        // Index for y values
        int ind = 0;

        // start x to end x
        for (int currentX = startX; currentX <= endX; currentX++)
        {
            char sign = ' ';
            float total = 0.0f;

            // Go through all non-empty strings
            for (int i = 0; i < len; i++)
            {
                // If current string has a number
                if (isdigit(strippedEquation[i][0]) || strippedEquation[i][0] == 'x')
                {
                    // Get number from current string
                    float num = GetNumber(strippedEquation[i], currentX);
                    PerformArithmetic(num, &total, sign);
                }

                // If current string contains a sign
                else if (strchr("+-/*", strippedEquation[i][0]) != NULL)
                {
                    // Set sign char to current one
                    sign = strippedEquation[i][0];
                }

                // If current string starts with a function
                else if (strchr(functionStart, strippedEquation[i][0]) != NULL)
                {
                    // Get function letters from start of string
                    char functionLetters[2];
                    strncpy(functionLetters, strippedEquation[i], 2);

                    // Strip function string
                    StripEquation(&strippedEquation[i][2]);

                    float yVal[1];
                    // Get value from new stripped eqaution
                    ProcessEquation(yVal, currentX, currentX);

                    // Use function with first y value gotten
                    UseFunction(yVal[0], &total, functionLetters, sign);
                }
            }

            // If not a number or infinity, set it to 0
            if (isnan(total) || isinf(total))
                total = 0.0f;
            // Store value
            yVals[ind++] = total;

            // Testing
            printf("(%s) %i: %f\n", (startX == endX) ? "Function value" : "Actual value", currentX, total);
        }

        // If it is not a recursive call (if it is the first call)
        if (startX != endX)
        {
            ClearSequence();
            // Reset equation sequence in case it has been changed by a recursive call
            for (int i = 0; i < len; i++)
                strcpy(equationSequence[i], strippedEquation[i]);
            // Reset current index in case it has been changed by a recursive call
            currentIndex = len;
        }

        // Return the length of the array minus 1 (to help the graph class)
        return (ind - 1);
    }

    // Return -1 if equation hasn't been stripped
    return -1;
}


#include <iostream>
#include <math.h>
#include <cstring>
#include <string>
using namespace std;

char exor(char a, char b) // perform exor operation
{
    if (a == b)
        return '0';
    else
        return '1';
}

bool isDivisibleByGenerator(char data[], char key[], int datalen, int keylen, int state)
{
    if (state == 0)
    {
        // cout << datalen << keylen
        //      << "\n";
    }

    char temp[20], rem[20];

    for (int i = 0; i < keylen; i++)
        rem[i] = data[i]; // considering keylen-1 bits of data for each step of binary division/EXOR

    for (int j = keylen; j <= datalen; j++)
    {
        for (int i = 0; i < keylen; i++)
            temp[i] = rem[i]; // remainder of previous step is divident of current step

        if (rem[0] == '0') // if 1's bit of remainder is 0 then shift the rem by 1 bit
        {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = temp[i + 1];
        }
        else // else exor the divident with generator polynomial
        {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = exor(temp[i + 1], key[i + 1]);
        }
        rem[keylen - 1] = data[j]; // appending next bit of data to remainder obtain by division
    }

    if (state == 0)
    {

        // cout << "CRC=" << rem << "\nDataword=" << data << "\n"; // remainder obtain is crc
    }

    bool valid = true;

    for (int j = 0; j < 16; j++)
    {
        if (rem[j] == '1')
        {
            valid = false;
        }
    }

    if (valid)
    {
        if (state == 0)
        {

            cout << 1
                 << "\n";
            // cout << "valid"
            //      << "\n";
        }

        return true;
    }
    else
    {

        if (state == 0)
        {

            cout << 0
                 << "\n";
            // cout << "invalid"
            //      << "\n";
        }
        return false;
    }
}

char *createMessageWithCrc(char data[], char key[], int state)
{
    int datalen = strlen(data);
    int keylen = strlen(key);

    // cout << datalen << keylen
    //      << "\n";

    for (int i = 0; i < keylen - 1; i++) // appending n-1 zeroes to data
    {
        data[datalen + i] = '0';
    }
    data[datalen + keylen - 1] = '\0';

    int codelen = datalen + keylen - 1; // lenght of appended data word

    char temp[20], rem[20];

    for (int i = 0; i < keylen; i++)
        rem[i] = data[i]; // considering n bits of data for each step of binary division/EXOR

    for (int j = keylen; j <= codelen; j++)
    {
        for (int i = 0; i < keylen; i++)
            temp[i] = rem[i]; // remainder of previous step is divident of current step

        if (rem[0] == '0') // if 1's bit of remainder is 0 then shift the rem by 1 bit
        {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = temp[i + 1];
        }
        else // else exor the divident with generator key
        {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = exor(temp[i + 1], key[i + 1]);
        }
        if (j != codelen)
            rem[keylen - 1] = data[j]; // appending next bit of data to remainder obtain by division
        else
            rem[keylen - 1] = '\0';
    }

    for (int i = 0; i < keylen - 1; i++)
        data[datalen + i] = rem[i]; // replace n-1 zeros with n-1 bit CRC

    data[codelen] = '\0';

    // cout << "CRC=" << rem << "\nDataword=" << data << "\n";

    if (state == 0)
    {

        cout << data << "\n";
    }

    // cout << "length of data: " << strlen(data) << "\n";

    return data;
}

int main(int argc, char **argv)
{
    if (strcmp(argv[1], "-c") == 0)
    {
        char *data = argv[2];
        char generator[18] = {'1', '0', '0', '0', '1', '0', '0', '0', '0', '1', '0', '1', '0', '0', '0', '0', '1', '\0'};

        char *extendedMessage = createMessageWithCrc(data, generator, 0);

        // x16 + x12 + x7 + x5 + 1
    }

    if (strcmp(argv[1], "-v") == 0)
    {

        char *data = argv[2];
        char generator[18] = {'1', '0', '0', '0', '1', '0', '0', '0', '0', '1', '0', '1', '0', '0', '0', '0', '1', '\0'};

        isDivisibleByGenerator(data, generator, strlen(data), strlen(generator), 0);

        // x16 + x12 + x7 + x5 + 1
    }

    if (strcmp(argv[1], "-f") == 0)
    {

        char *data = argv[2];
        char generator[18] = {'1', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1', '\0'};

        // char generator[4] = {'1', '1', '0', '\0'};

        char *extendedMessage = createMessageWithCrc(data, generator, 1);

        // cout << "length of extended message: " << strlen(extendedMessage) << "\n";

        for (int i = 0; i < strlen(extendedMessage) - 3; i++)
        {
            for (int j = i + 1; j < strlen(extendedMessage) - 2; j++)
            {
                for (int k = j + 1; k < strlen(extendedMessage) - 1; k++)
                {
                    for (int l = k + 1; l < strlen(extendedMessage); l++)
                    {
                        char extendedMessageCopy[strlen(extendedMessage) + 1];

                        strncpy(extendedMessageCopy, extendedMessage, strlen(extendedMessage) + 1);

                        extendedMessageCopy[strlen(extendedMessage)] = '\0';

                        if (extendedMessageCopy[i] == '1')
                        {
                            extendedMessageCopy[i] = '0';
                        }
                        else if (extendedMessageCopy[i] == '0')
                        {
                            extendedMessageCopy[i] = '1';
                        }

                        if (extendedMessageCopy[j] == '1')
                        {
                            extendedMessageCopy[j] = '0';
                        }
                        else if (extendedMessageCopy[j] == '0')
                        {
                            extendedMessageCopy[j] = '1';
                        }

                        if (extendedMessageCopy[k] == '1')
                        {
                            extendedMessageCopy[k] = '0';
                        }
                        else if (extendedMessageCopy[k] == '0')
                        {
                            extendedMessageCopy[k] = '1';
                        }

                        if (extendedMessageCopy[l] == '1')
                        {
                            extendedMessageCopy[l] = '0';
                        }
                        else if (extendedMessageCopy[l] == '0')
                        {
                            extendedMessageCopy[l] = '1';
                        }

                        // cout << strlen(extendedMessageCopy) << "\n";

                        if (isDivisibleByGenerator(extendedMessageCopy, generator, strlen(extendedMessageCopy), strlen(generator), 1))
                        {
                            cout << extendedMessageCopy << "\n";
                        }
                    }
                }
            }
        }
    }

    if (strcmp(argv[1], "-t") == 0)
    {

        char *data = argv[2];
        char generator[18] = {'1', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1', '\0'};

        // char generator[4] = {'1', '1', '0', '\0'};

        char *extendedMessage = createMessageWithCrc(data, generator, 1);

        // cout << "length of extended message: " << strlen(extendedMessage) << "\n";

        int undetectedErrors = 0;

        for (int i = 0; i < strlen(extendedMessage) - 4; i++)
        {
            for (int j = i + 1; j < strlen(extendedMessage) - 3; j++)
            {
                for (int k = j + 1; k < strlen(extendedMessage) - 2; k++)
                {
                    for (int l = k + 1; l < strlen(extendedMessage) - 1; l++)
                    {
                        for (int m = l + 1; m < strlen(extendedMessage); m++)
                        {

                            char extendedMessageCopy[strlen(extendedMessage) + 1];

                            strncpy(extendedMessageCopy, extendedMessage, strlen(extendedMessage) + 1);

                            extendedMessageCopy[strlen(extendedMessage)] = '\0';

                            if (extendedMessageCopy[i] == '1')
                            {
                                extendedMessageCopy[i] = '0';
                            }
                            else if (extendedMessageCopy[i] == '0')
                            {
                                extendedMessageCopy[i] = '1';
                            }

                            if (extendedMessageCopy[j] == '1')
                            {
                                extendedMessageCopy[j] = '0';
                            }
                            else if (extendedMessageCopy[j] == '0')
                            {
                                extendedMessageCopy[j] = '1';
                            }

                            if (extendedMessageCopy[k] == '1')
                            {
                                extendedMessageCopy[k] = '0';
                            }
                            else if (extendedMessageCopy[k] == '0')
                            {
                                extendedMessageCopy[k] = '1';
                            }

                            if (extendedMessageCopy[l] == '1')
                            {
                                extendedMessageCopy[l] = '0';
                            }
                            else if (extendedMessageCopy[l] == '0')
                            {
                                extendedMessageCopy[l] = '1';
                            }

                            if (extendedMessageCopy[m] == '1')
                            {
                                extendedMessageCopy[m] = '0';
                            }
                            else if (extendedMessageCopy[m] == '0')
                            {
                                extendedMessageCopy[m] = '1';
                            }

                            // cout << strlen(extendedMessageCopy) << "\n";

                            if (isDivisibleByGenerator(extendedMessageCopy, generator, strlen(extendedMessageCopy), strlen(generator), 1))
                            {
                                // cout << extendedMessageCopy << "\n";
                                undetectedErrors++;
                            }
                        }
                    }
                }
            }
        }

        cout << undetectedErrors << "\n";
    }

    if (strcmp(argv[1], "-p") == 0)
    {
        char *data = argv[2];
        char generator[18] = {'1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1', '\0'};

        // char generator[4] = {'1', '1', '0', '\0'};

        char *extendedMessage = createMessageWithCrc(data, generator, 1);

        // cout << "length of extended message: " << strlen(extendedMessage) << "\n";

        int undetectedErrors = 0;

        for (int i = 0; i < strlen(extendedMessage) - 4; i++)
        {
            for (int j = i + 1; j < strlen(extendedMessage) - 3; j++)
            {
                for (int k = j + 1; k < strlen(extendedMessage) - 2; k++)
                {
                    for (int l = k + 1; l < strlen(extendedMessage) - 1; l++)
                    {
                        for (int m = l + 1; m < strlen(extendedMessage); m++)
                        {

                            char extendedMessageCopy[strlen(extendedMessage) + 1];

                            strncpy(extendedMessageCopy, extendedMessage, strlen(extendedMessage) + 1);

                            extendedMessageCopy[strlen(extendedMessage)] = '\0';

                            if (extendedMessageCopy[i] == '1')
                            {
                                extendedMessageCopy[i] = '0';
                            }
                            else if (extendedMessageCopy[i] == '0')
                            {
                                extendedMessageCopy[i] = '1';
                            }

                            if (extendedMessageCopy[j] == '1')
                            {
                                extendedMessageCopy[j] = '0';
                            }
                            else if (extendedMessageCopy[j] == '0')
                            {
                                extendedMessageCopy[j] = '1';
                            }

                            if (extendedMessageCopy[k] == '1')
                            {
                                extendedMessageCopy[k] = '0';
                            }
                            else if (extendedMessageCopy[k] == '0')
                            {
                                extendedMessageCopy[k] = '1';
                            }

                            if (extendedMessageCopy[l] == '1')
                            {
                                extendedMessageCopy[l] = '0';
                            }
                            else if (extendedMessageCopy[l] == '0')
                            {
                                extendedMessageCopy[l] = '1';
                            }

                            if (extendedMessageCopy[m] == '1')
                            {
                                extendedMessageCopy[m] = '0';
                            }
                            else if (extendedMessageCopy[m] == '0')
                            {
                                extendedMessageCopy[m] = '1';
                            }

                            // cout << strlen(extendedMessageCopy) << "\n";

                            if (isDivisibleByGenerator(extendedMessageCopy, generator, strlen(extendedMessageCopy), strlen(generator), 1))
                            {
                                // cout << extendedMessageCopy << "\n";
                                undetectedErrors++;
                            }
                        }
                    }
                }
            }
        }

        cout << undetectedErrors << "\n";
    }
}

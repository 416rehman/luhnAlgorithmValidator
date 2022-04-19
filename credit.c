#include <cs50.h>
#include <stdio.h>
#include <string.h>

void extractDigits(long number, int *arr);
string getCardType(int *digits);
bool validateLength(long number, string type, int length);
int getNumberLength(long number);
bool validateChecksum(int *digits, int length);

int main(void)
{
    long number = get_long("Number: ");
    int length = getNumberLength(number);

    int digits[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    extractDigits(number, digits);

    string issuer = getCardType(digits);
    if (strcmp(issuer, "INVALID") != 0)
    {
        if (!validateLength(number, issuer, length))
            issuer = "INVALID";
        if (!validateChecksum(digits, length))
            issuer = "INVALID";
    }

    printf("%s\n", issuer);
}

bool validateChecksum(int *digits, int length)
{
    int products[length / 2]; // contains every other digit multiplied by 2 starting from the right
    int products_idx = 0;
    // initializing to -2 because -1 will result in the valid length for the loop, and another -1 as an OFFSET to start from the SECOND last digit.
    for (int i = length - 2; i >= 0; i = i - 2)
    {
        products[products_idx] = digits[i] * 2;
        products_idx++;
    }

    int sumOfProducts = 0;

    // Go through all the products
    for (int i = 0; i < length / 2; i++)
    {
        int currentProduct = products[i];

        // if single digit, add it to the sum
        if (currentProduct < 10)
        {
            sumOfProducts += currentProduct;
            continue;
        }

        // otherwise break down into individual digits
        int productDigits[2] = {0, 0};
        extractDigits(currentProduct, productDigits);

        // add the individual digits to the sum
        for (int j = 0; j < 2; j++)
        {
            sumOfProducts += productDigits[j];
        }
    }

    // add the sum of products to every other digit that was not multiplied by 2. (No Offset)
    for (int i = length - 1; i >= 0; i = i - 2)
    {
        sumOfProducts += digits[i];
    }

    return sumOfProducts % 10 == 0;
}

void extractDigits(long number, int *arr)
{
    int i = getNumberLength(number) - 1;
    while (number != 0)
    {
        arr[i] = number % 10;
        number = number / 10;
        i--;
    }
}

string getCardType(int *digits)
{
    if (digits[0] == 3)
    {
        if (digits[1] == 4 || digits[1] == 7)
            return "AMEX";
    }

    if (digits[0] == 5)
    {
        if (digits[1] > 0 && digits[1] < 6)
            return "MASTERCARD";
    }

    if (digits[0] == 4)
    {
        return "VISA";
    }

    return "INVALID";
}

bool validateLength(long number, string type, int length)
{
    if (strcmp(type, "AMEX") == 0)
        return length == 15;

    if (strcmp(type, "VISA") == 0)
        return length == 13 || length == 16;

    if (strcmp(type, "MASTERCARD") == 0)
        return length == 16;

    return false;
}

int getNumberLength(long number)
{
    int digits = 0;

    while (number > 0)
    {
        number = number / 10;
        digits++;
    }

    return digits;
}
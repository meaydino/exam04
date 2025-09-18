#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Function declarations
int factor(char **str);
int term(char **str);
int expression(char **str);

void error(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
    exit(1);
}

int factor(char **str)
{
    int result;
    
    if (isdigit(**str))
    {
        result = **str - '0';
        (*str)++;
        return result;
    }
    else if (**str == '(')
    {
        (*str)++;
        result = expression(str);
        if (**str != ')')
            error(**str);
        (*str)++;
        return result;
    }
    error(**str);
    return 0;
}

int term(char **str)
{
    int result = factor(str);
    
    while (**str == '*')
    {
        (*str)++;
        result *= factor(str);
    }
    return result;
}

int expression(char **str)
{
    int result = term(str);
    
    while (**str == '+')
    {
        (*str)++;
        result += term(str);
    }
    return result;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;
    
    char *str = argv[1];
    int result = expression(&str);
    
    if (*str)
        error(*str);
    
    printf("%d\n", result);
    return 0;
}
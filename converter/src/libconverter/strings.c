#include "strings.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char *concat(char *s1, char *s2)
{

    if ((s1 == NULL) || (s2 == NULL))
    {
        return NULL;
    }
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);

    if (!result)
    {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);

    return result;
}

char *concat3(char *s1, char *s2, char *s3)
{
    if ((s1 == NULL) || (s2 == NULL) || (s3 == NULL))
    {
        return NULL;
    }

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t len3 = strlen(s3);
    char *result = malloc(len1 + len2 + len3 + 1);

    if (!result)
    {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2);
    memcpy(result + len1 + len2, s3, len3 + 1);

    return result;
}

int Pos(const char *const str1, const char *const str2)
{
    int res = -1;
    int flag = 0;
    int i = 0;
    int j = 0;
    if ((str1 == NULL) || (str2 == NULL))
        res = -2;
    else
    {
        int len1 = (strlen(str1));
        int len2 = (strlen(str2));
        while ((res < 0) && (i <= len1 - len2))
        {
            flag = 1;
            j = 0;
            while ((flag) && (j < len2))
            {
                if (str1[i + j] != str2[j])
                    flag = 0;
                j++;
            }
            if (flag)
                res = i;
            i++;
        }
    }
    return res;
}

void getStartString(const char str[], char destination[], int pos)
{
    int i = 0;
    while (i <= pos)
    {
        destination[i] = str[i];
        i++;
    }
}

void getEndString(const char str[], char destination[], int pos)
{
    int i = 0;
    int skip = 0;
    while (str[i] != '\0')
    {
        if (i > pos)
        {
            destination[i - skip] = str[i];
        }
        else
        {
            skip++;
        }

        i++;
    }
}

void getSubString(const char str[], char destination[], int startPos, int endPos)
{
    int i = 0;
    int skip = 0;
    while (str[i] != '\0')
    {
        if (i > endPos)
        {
            break;
        }
        if (i > startPos)
        {
            destination[i - skip] = str[i];
        }
        else
        {
            skip++;
        }
        i++;
    }
}
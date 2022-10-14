#include "converter.h"
#include "strings.h"
char fileArray[1024][1024] = {};

int posScript = 0;
int countStr = 0;

int ValueCitation(char *source)
{
    int res = 0;
    char *arrayCitations[5];
    arrayCitations[0] = "> ";
    arrayCitations[1] = ">> ";
    arrayCitations[2] = ">>> ";
    arrayCitations[3] = ">>>> ";
    arrayCitations[4] = ">>>>> ";
    for (int i = 0; i < 5; i++)
    {
        if ((strstr(source, arrayCitations[i]) != NULL) && (Pos(source, arrayCitations[i]) == 0))
        {
            res = i + 1;
        }
    }
    return res;
}
void convertCitation(char *source)
{
    int value = ValueCitation(source);

    if (value > 0)
    {
        char *text = malloc(strlen(source) - value);

        getEndString(source, text, value);
        char *convertedElement = concat("<blockquote>\n", text);

        for (int i = 1; i < value; i++)
        {
            convertedElement = concat("<blockquote>\n", convertedElement);
        }
        strcpy(fileArray[posScript], convertedElement);

        for (int i = posScript + 1; i < countStr; i++)
        {
            int count = ValueCitation(fileArray[i]);
            if (count != 0)
            {
                char *str = malloc(strlen(fileArray[i]) - count);
                getEndString(fileArray[i], str, count);
                if (count > value)
                {
                    for (int j = 0; j < count - value; j++)
                    {
                        str = concat("<blockquote>\n", str);
                    }
                    value = count;
                }
                strcpy(fileArray[i], str);
            }
            else if (strlen(fileArray[i]) == 1)
            {
                for (int j = 0; j < value; j++)
                {
                    strcpy(fileArray[i - 1], concat(fileArray[i - 1], "</blockquote>\n"));
                }
                break;
            }
        }
    }
}
int valueList(char *source)
{
    int value = 0;
    if (strlen(source) > 2)
    {
        if ((source[0] >= '1') && (source[0] <= '9') && (Pos(source, ". ") < 2))
        {
            value = 2;
        }
        if (((Pos(source, "+ ") < 1) && (strstr(source, "+ ") != NULL)) || ((Pos(source, "- ") < 1) && (strstr(source, "- ") != NULL)) || ((Pos(source, "* ") < 1) && (strstr(source, "* ") != NULL)))
        {
            value = 1;
        }
    }
    return value;
}

void convertList(char *source)
{
    int value = valueList(source);
    if (value > 0)
    {
        char *text = malloc(strlen(source) - value);
        getSubString(source, text, value, Pos(source, "\n") - 1);

        char *convertedElement = concat3("<li>", text, "</li>\n");

        if (value == 1)
        {
            convertedElement = concat("<ul>\n", convertedElement);
        }
        if (value == 2)
        {
            convertedElement = concat("<ol>\n", convertedElement);
        }

        strcpy(fileArray[posScript], convertedElement);

        for (int i = posScript + 1; i < countStr; i++)
        {
            if ((valueList(fileArray[i]) != value) || (strlen(fileArray[i]) == 1))
            {
                if (value == 1)
                {
                    strcpy(fileArray[i - 1], concat(fileArray[i - 1], "</ul>\n"));
                }

                if (value == 2)
                {
                    strcpy(fileArray[i - 1], concat(fileArray[i - 1], "</ol>\n"));
                }
                break;
            }
            else if (valueList(fileArray[i]) == value)
            {
                char *str = malloc(strlen(fileArray[i]) - value + 11);
                getSubString(fileArray[i], str, value, Pos(fileArray[i], "\n") - 1);
                str = concat3("<li>", str, "</li>\n");

                strcpy(fileArray[i], str);
            }
        }
    }
}

int valueHeader(char *source)
{
    int value = 0;
    char *arrayHeaders[6];
    arrayHeaders[0] = "# ";
    arrayHeaders[1] = "## ";
    arrayHeaders[2] = "### ";
    arrayHeaders[3] = "#### ";
    arrayHeaders[4] = "##### ";
    arrayHeaders[5] = "###### ";

    for (int i = 0; i < 6; i++)
    {
        if ((strstr(source, arrayHeaders[i]) != NULL) && (Pos(source, arrayHeaders[i]) == 0))
        {
            value = i + 1;
        }
    }

    return value;
}
void convertHeader(char *source, int value)
{
    if (value > 0)
    {
        char *arrayHtmlHeadersStart[6];
        arrayHtmlHeadersStart[0] = "<h1>";
        arrayHtmlHeadersStart[1] = "<h2>";
        arrayHtmlHeadersStart[2] = "<h3>";
        arrayHtmlHeadersStart[3] = "<h4>";
        arrayHtmlHeadersStart[4] = "<h5>";
        arrayHtmlHeadersStart[5] = "<h6>";
        char *arrayHtmlHeadersEnd[6];
        arrayHtmlHeadersEnd[0] = "</h1>\n";
        arrayHtmlHeadersEnd[1] = "</h2>\n";
        arrayHtmlHeadersEnd[2] = "</h3>\n";
        arrayHtmlHeadersEnd[3] = "</h4>\n";
        arrayHtmlHeadersEnd[4] = "</h5>\n";
        arrayHtmlHeadersEnd[5] = "</h6>\n";
        size_t len = strlen(source);
        char *text = malloc(len - value);

        getSubString(source, text, value, Pos(source, "\n") - 1);
        char *result = concat3(arrayHtmlHeadersStart[value - 1], text, arrayHtmlHeadersEnd[value - 1]);

        strcpy(fileArray[posScript], result);
    }
}
char *convertStringElement(char *source, char *convertedElement, int posStart, int posEnd)
{
    char *startStr = malloc(posStart + 1); // Если что тут не было +1
    char *finishStr = malloc(strlen(source) - posEnd - 1);

    getStartString(source, startStr, posStart - 1);
    getEndString(source, finishStr, posEnd + 1);
    return concat3(startStr, convertedElement, finishStr);
    // strcpy(destination, result);
}

void convertLinkText(char *source)
{
    if (strstr(source, "](") != NULL)
    {
        char arrayHtmlLinkStart1[9] = "<a href=\"";
        char arrayHtmlLinkStart2[3] = "\">";
        char arrayHtmlLinkEnd[5] = "</a>";

        char *link = malloc(Pos(source, ")") - Pos(source, "](") - 2);
        char *text = malloc(Pos(source, "]") - Pos(source, "[") - 1);

        getSubString(source, link, Pos(source, "](") + 1, Pos(source, ")") - 1);
        getSubString(source, text, Pos(source, "["), Pos(source, "]") - 1);

        char *convertedElement = concat3(arrayHtmlLinkStart1, link, arrayHtmlLinkStart2);
        convertedElement = concat3(convertedElement, text, arrayHtmlLinkEnd);

        char *result = convertStringElement(source, convertedElement, Pos(source, "["), Pos(source, ")") - 1);
        strcpy(fileArray[posScript], result);

        if (strstr(source, "](") != NULL)
        {
            convertLinkText(source);
        }
    }
}

void replaceOnTag(char *source, char *destination, char *mask, char *tag)
{
    int posMask = Pos(source, mask);

    char *leftStr = malloc(Pos(source, mask) + strlen(mask) - 1);
    char *rightStr = malloc(strlen(source) - Pos(source, mask));

    getStartString(source, leftStr, posMask - 1);
    getEndString(source, rightStr, posMask + strlen(mask) - 1);

    char *result = concat3(leftStr, tag, rightStr);

    strcpy(destination, result);
}
int findHtml(char *source)
{
    int res = 0;
    char *htmlTags[14];
    htmlTags[0] = "<a>";
    htmlTags[1] = "</a>";
    htmlTags[2] = "<li>";
    htmlTags[3] = "</li>";
    htmlTags[4] = "<strong>";
    htmlTags[5] = "</strong>";
    htmlTags[6] = "<italic>";
    htmlTags[7] = "</italic>";
    htmlTags[8] = "<blockquote>";
    htmlTags[9] = "</blockquote>";
    htmlTags[10] = "<ol>";
    htmlTags[11] = "</ol>";
    htmlTags[12] = "<ul>";
    htmlTags[13] = "</ul>";
    for (int i = 0; i < 14; i++)
    {
        if (strstr(source, htmlTags[i]) != NULL)
        {
            res = 1;
        }
    }
    return res;
}
char *findMask(char *source)
{
    char *masks[7];
    masks[6] = "<strong>";
    masks[5] = "</strong>";
    masks[4] = "<italic>";
    masks[3] = "</italic>";
    masks[1] = "<strong></italic>";
    masks[2] = "</italic></strong>";
    for (int i = 0; i < 7; i++)
    {
        if (strstr(source, masks[i]) != NULL)
        {
            return masks[i];
        }
    }
    return NULL;
}
void convertLink(char *source)
{
    if ((ValueCitation(source) == 0) && (strstr(source, "<") != NULL) && (strstr(source, ">") != NULL))
    {
        char htmlLinkStart1[9] = "<a href=\"";
        char htmlLinkStart2[3] = "\">";
        char htmlLinkEnd[5] = "</a>";
        if (findHtml(source) > 0)
        {
            if (strstr(source, "<li>") != NULL)
            {
                char *str = malloc(strlen(source));
                getSubString(source, str, 3, Pos(source, "</li>") - 1);
                if ((strstr(str, "<") != NULL) && (strstr(str, ">") != NULL))
                {
                    char *link = malloc(Pos(str, ">") - Pos(str, "<") - 1);
                    getSubString(str, link, Pos(str, "<"), Pos(str, ">") - 1);

                    char *convertedElement = concat3(htmlLinkStart1, link, htmlLinkStart2);
                    convertedElement = concat3(convertedElement, link, htmlLinkEnd);

                    char *result = convertStringElement(source, convertedElement, Pos(str, "<") + 4, Pos(str, ">") + 3);
                    strcpy(fileArray[posScript], result);
                }
            }
            if (findMask(source) != NULL)
            {
                char *mask = malloc(strlen(findMask(source)));
                mask = findMask(source);

                char *left = malloc(Pos(source, mask));
                char *right = malloc(strlen(source) - Pos(source, mask) + 1);

                getStartString(source, left, Pos(source, mask) - 1);
                getEndString(source, right, Pos(source, mask) + strlen(mask));

                if ((strstr(right, "<") != NULL) && (strstr(right, ">") != NULL))
                {
                    char *link = malloc(Pos(right, ">") - Pos(right, "<") - 1);
                    getSubString(right, link, Pos(right, "<"), Pos(right, ">") - 1);

                    char *convertedElement = concat3(htmlLinkStart1, link, htmlLinkStart2);
                    convertedElement = concat3(convertedElement, link, htmlLinkEnd);

                    char *result = convertStringElement(source, convertedElement, Pos(right, "<") + strlen(left) + strlen(mask) + 1, Pos(right, ">") + strlen(left) + strlen(mask));
                    strcpy(fileArray[posScript], result);
                }
                if ((strstr(left, "<") != NULL) && (strstr(left, ">") != NULL))
                {
                    char *link = malloc(Pos(left, ">") - Pos(left, "<") - 1);
                    getSubString(left, link, Pos(left, "<"), Pos(left, ">") - 1);

                    char *convertedElement = concat3(htmlLinkStart1, link, htmlLinkStart2);
                    convertedElement = concat3(convertedElement, link, htmlLinkEnd);

                    char *result = convertStringElement(source, convertedElement, Pos(left, "<"), Pos(left, ">") - 1);
                    strcpy(fileArray[posScript], result);
                }
            }
        }
        if (findHtml(source) == 0)
        {
            if ((strstr(source, "<") != NULL) && (strstr(source, ">") != NULL))
            {
                char *link = malloc(Pos(source, ">") - Pos(source, "<") - 1);
                getSubString(source, link, Pos(source, "<"), Pos(source, ">") - 1);

                char *convertedElement = concat3(htmlLinkStart1, link, htmlLinkStart2);
                convertedElement = concat3(convertedElement, link, htmlLinkEnd);

                char *result = convertStringElement(source, convertedElement, Pos(source, "<"), Pos(source, ">") - 1);
                strcpy(fileArray[posScript], result);
            }
        }
    }
}

void convertSelectionText(char *source, char *mask, char *openTag, char *closeTag)
{
    if (strstr(source, mask) != NULL)
    {
        char *text1 = malloc(strlen(source) - Pos(source, mask));
        getSubString(source, text1, Pos(source, mask) + strlen(mask) - 1, strlen(source));
        if (strstr(text1, mask) != NULL)
        {
            char *text = malloc(Pos(text1, mask));
            getSubString(text1, text, -1, Pos(text1, mask) - 1);
            if (strlen(text) > 0)
            {
                int lengthTags = strlen(openTag) + strlen(closeTag);
                char *convertedElement = malloc(strlen(text) + lengthTags);
                convertedElement = concat3(openTag, text, closeTag);

                char *result = convertStringElement(source, convertedElement, Pos(source, mask), Pos(source, text) + strlen(text) + strlen(mask) - 2);

                if (Pos(result, mask) != -1)
                {
                    convertSelectionText(result, mask, openTag, closeTag);
                }
                else
                {
                    strcpy(fileArray[posScript], result);
                }
            }
        }
        else
        {
            for (int i = posScript + 1; i < countStr - 1; i++)
            {
                if ((ValueCitation(fileArray[i]) > 0) || (strstr(fileArray[i], "<li>") != NULL) || (valueHeader(fileArray[i]) > 0) || (valueList(fileArray[i]) > 0) || (strlen(fileArray[i]) == 1))
                {
                    break;
                }

                char *str = fileArray[i];
                if (Pos(str, mask) != -1)
                {
                    replaceOnTag(source, fileArray[posScript], mask, openTag);
                    replaceOnTag(str, fileArray[i], mask, closeTag);
                    break;
                }
            }
        }
    }
}

void convertImg(char *source)
{
    if (strstr(source, "!["))
    {
        char arrayHtmlImgStart1[10] = "<img src=\"";
        char arrayHtmlImgStart2[3] = "\" ";
        char arrayHtmlImgEnd2[3] = "\">";
        char *left = malloc(Pos(source, "!["));
        char *right = malloc(strlen(source) - Pos(source, "![") + 1);

        getStartString(source, left, Pos(source, "![") + 1);
        getEndString(source, right, Pos(source, "![") + 1);

        char *path = malloc(Pos(right, ")") - Pos(right, "](") - 2);
        char *text = malloc(Pos(right, "]"));

        getSubString(right, path, Pos(right, "](") + 1, Pos(right, ")") - 1);
        getSubString(right, text, -1, Pos(right, "]") - 1);

        char *convertedElement = concat3(arrayHtmlImgStart1, path, arrayHtmlImgStart2);
        convertedElement = concat3(convertedElement, "alt=\"", text);
        convertedElement = concat(convertedElement, arrayHtmlImgEnd2);

        char *result = convertStringElement(source, convertedElement, Pos(source, "!["), Pos(right, ")") + strlen(left) - 1);
        strcpy(fileArray[posScript], result);
        if (strstr(source, "![") != NULL)
        {
            convertImg(source);
        }
    }
}

void runScript(char *source)
{
    convertLink(source);
    convertImg(source);
    convertLinkText(source);
    convertList(source);
    convertSelectionText(source, "***", "<strong><italic>", "</italic></strong>");
    convertSelectionText(source, "**", "<strong>", "</strong>");
    convertSelectionText(source, "*", "<italic>", "</italic>");
    convertHeader(source, valueHeader(source));
}

void runScriptCitation(char *source)
{
    convertCitation(source);
}

void convert()
{
    char fileNameStart[30];
    FILE *file;
    char *estr;
    printf("Введите название файла: \n");
    scanf("%s", fileNameStart);
    file = fopen(fileNameStart, "r");

    int i = 0;
    while (1)
    {
        char *arr = fileArray[i];
        estr = fgets(arr, 1024, file);
        if (estr == NULL)
        {
            if (feof(file) != 0)
            {
                printf("\nЧтение файла закончено\n");
                break;
            }
            else
            {
                printf("\nОшибка чтения из файла\n");
                break;
            }
        }
        i++;
    }
    countStr = i;
    while (posScript != i)
    {
        runScript(fileArray[posScript]);
        posScript++;
    }
    posScript = 0;

    FILE *ffinish;
    char fileNameConvert[30];
    printf("Введите название итогового файла: \n");
    scanf("%s", fileNameConvert);
    ffinish = fopen(fileNameConvert, "w");

    while (posScript != i)
    {
        runScriptCitation(fileArray[posScript]);
        fprintf(ffinish, "%s", fileArray[posScript]);
        posScript++;
    }
    fclose(file);
    fclose(ffinish);
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char fileArray[1024][1024] = {};

int posScript = 0;
int countStr = 0;

// Энам для обозначения типа дейяствия
enum Action
{
  LINK,
  LINK_TEXT,
  BOLD,
  ITALIC,
  BOLD_ITALIC,
  IMG,
  HEADER,
  LIST,
  NO_ACTION
};

char *concat(char *s1, char *s2)
{
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);
  char *result = malloc(len1 + len2 + 3);
  /*if (!result) {
      fprintf(stderr, "malloc() failed: insufficient memory!\n");
      return NULL;
  }*/

  memcpy(result, s1, len1);
  memcpy(result + len1, s2, len2 + 1);

  return result;
}

char *concats(char *s1, char *s2, char *s3)
{

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
  // size_t len = strlen(str1);
  int res = -1;
  int flag = 0;
  int i = 0;
  int j = 0;
  if ((str1 == NULL) || (str2 == NULL))
    res = -2;
  else
  {
    while ((res < 0) && (i <= (strlen(str1) - strlen(str2))))
    {
      flag = 1;
      j = 0;
      while ((flag) && (j < strlen(str2)))
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

char *getStartString(const char str[], char destination[], int pos)
{
  // char newStr[1024] = "";
  int i = 0;
  while (i <= pos)
  {
    destination[i] = str[i];
    i++;
  }

  return destination;
}

char *getEndString(const char str[], char destination[], int pos)
{
  int i = 0;
  int skip = 0;
  while (str[i] != '\0')
  {
    // printf("%c \n", str[i]);
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

  return destination;
}

char *getSubString(const char str[], char destination[], int startPos, int endPos)
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
  return destination;
}
int valueList(char *source)
{
  int value = 0;
  if ((source[0] >= '1') && (source[0] <= '9') && (Pos(source, ". ") < 2))
  {
    value = 1;
  }
  if ((Pos(source, "+ ") < 1) || (Pos(source, "- ") < 1) || (Pos(source, "* ") < 1))
  {
    value = 2;
  }
  return value;
}

void convertList(char *source)
{
  int posFinish = 0;
  int value = valueList(source);
  int lenStart = 0;
  char *text = malloc(strlen(source) - lenStart - 2);

  if (value == 1)
  {
    lenStart = 3;
  }
  if (value == 2)
  {
    lenStart = 2;
  }

  getSubString(source, text, lenStart, strlen(source) - 2);

  char *result = concats("<li>", text, "</li>");

  strcpy(fileArray[posScript], result);
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
void convertHeader(char *source)
{
  int value = valueHeader(source);
  char *arrayHtmlHeadersStart[6];
  arrayHtmlHeadersStart[0] = "<h1>";
  arrayHtmlHeadersStart[1] = "<h2>";
  arrayHtmlHeadersStart[2] = "<h3>";
  arrayHtmlHeadersStart[3] = "<h4>";
  arrayHtmlHeadersStart[4] = "<h5>";
  arrayHtmlHeadersStart[5] = "<h6>";
  char *arrayHtmlHeadersEnd[6];
  arrayHtmlHeadersEnd[0] = "</h1>";
  arrayHtmlHeadersEnd[1] = "</h2>";
  arrayHtmlHeadersEnd[2] = "</h3>";
  arrayHtmlHeadersEnd[3] = "</h4>";
  arrayHtmlHeadersEnd[4] = "</h5>";
  arrayHtmlHeadersEnd[5] = "</h6>";
  size_t len = strlen(source);
  char *text = malloc(len - value);

  getSubString(source, text, value, strlen(source) - 2);
  char *result = concats(arrayHtmlHeadersStart[value - 1], text, arrayHtmlHeadersEnd[value - 1]);

  strcpy(fileArray[posScript], result);
}
char *convertStringElement(char *source, char *convertedElement, int posStart, int posEnd)
{
  char *startStr = malloc(posStart);
  char *finishStr = malloc(strlen(source) - posEnd - 1);

  getStartString(source, startStr, posStart - 1);
  getEndString(source, finishStr, posEnd + 1);

  return concats(startStr, convertedElement, finishStr);
  // strcpy(destination, result);
}

void convertLinkText(char *source)
{
  char arrayHtmlLinkStart1[9] = "<a href=\"";
  char arrayHtmlLinkStart2[3] = "\">";
  char arrayHtmlLinkEnd[5] = "</a>";

  char *link = malloc(Pos(source, ")") - Pos(source, "](") - 2);
  char *text = malloc(Pos(source, "]") - Pos(source, "[") - 1);

  getSubString(source, link, Pos(source, "](") + 1, Pos(source, ")") - 1);
  getSubString(source, text, Pos(source, "["), Pos(source, "]") - 1);

  char *convertedElement = concats(arrayHtmlLinkStart1, link, arrayHtmlLinkStart2);
  convertedElement = concats(convertedElement, text, arrayHtmlLinkEnd);

  char *result = convertStringElement(source, convertedElement, Pos(source, "["), Pos(source, ")") - 1);
  strcpy(fileArray[posScript], result);
  // printf("%s\n", result);
}

void replaceOnTag(char *source, char *destination, char *mask, char *tag)
{
  int posMask = Pos(source, mask);

  char *leftStr = malloc(Pos(source, mask) + strlen(mask) - 1);
  char *rightStr = malloc(strlen(source) - Pos(source, mask));

  getStartString(source, leftStr, posMask - 1);
  getEndString(source, rightStr, posMask + strlen(mask) - 1);

  // printf("left %s\n", leftStr);
  // printf("right %s\n", rightStr);

  char *result = concats(leftStr, tag, rightStr);

  strcpy(destination, result);

  // printf("result %s\n", result);
}

void convertLink(char *source)
{
  char htmlLinkStart1[9] = "<a href=\"";
  char htmlLinkStart2[3] = "\">";
  char htmlLinkEnd[5] = "</a>";
  char *link = malloc(Pos(source, ">") - Pos(source, "<") - 1);
  // char* result;

  getSubString(source, link, Pos(source, "<"), Pos(source, ">") - 1);

  char *convertedElement = concats(htmlLinkStart1, link, htmlLinkStart2);
  convertedElement = concats(convertedElement, link, htmlLinkEnd);

  char *result = convertStringElement(source, convertedElement, Pos(source, "<"), Pos(source, ">") - 1);
  strcpy(fileArray[posScript], result);
}

void convertSelectionText(char *source, char *mask, char *openTag, char *closeTag)
{
  char *text1 = malloc(strlen(source) - Pos(source, mask));
  getSubString(source, text1, Pos(source, mask) + strlen(mask) - 1, strlen(source));

  if (strstr(text1, mask) != NULL)
  {
    char *text = malloc(Pos(text1, mask));
    getSubString(text1, text, -1, Pos(text1, mask) - 1);

    int lengthTags = strlen(openTag) + strlen(closeTag);
    char *convertedElement = malloc(strlen(text) + lengthTags);
    convertedElement = concats(openTag, text, closeTag);

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
  else
  {
    for (int i = posScript + 1; i < countStr; i++)
    {
      // Сюда добавить условие для проверки других блочных маркдаунов, если нашлось то не продолжаем искать
      // if () {
      //   break;
      // }
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

void convertImg(char *source)
{
  char arrayHtmlImgStart1[10] = "<img src=\"";
  char arrayHtmlImgStart2[3] = "\" ";
  char arrayHtmlImgEnd2[3] = "\">";
  char *path = malloc(Pos(source, ")") - Pos(source, "](") - 2);
  char *text = malloc(Pos(source, "]") - Pos(source, "![") + 2);

  getSubString(source, path, Pos(source, "](") + 1, Pos(source, ")") - 1);
  getSubString(source, text, Pos(source, "![") + 1, Pos(source, "]") - 1);

  char *convertedElement = concats(arrayHtmlImgStart1, path, arrayHtmlImgStart2);
  convertedElement = concats(convertedElement, "alt=\"", text);
  convertedElement = concat(convertedElement, arrayHtmlImgEnd2);

  char *result = convertStringElement(source, convertedElement, Pos(source, "!["), Pos(source, ")") - 1);

  strcpy(fileArray[posScript], result);
}

void doAction(char *source, enum Action action)
{
  switch (action)
  {
  // case LINK_TEXT:
  //   convertLinkText(source);
  //   break;
  // case LINK:
  //   convertLink(source);
  //   break;
  case BOLD_ITALIC:
    convertSelectionText(source, "***", "<strong><italic>", "</italic></strong>");
    break;
  case BOLD:
    convertSelectionText(source, "**", "<strong>", "</strong>");
    break;
  case ITALIC:
    convertSelectionText(source, "*", "<italic>", "</italic>");
    break;
  case IMG:
    convertImg(source);
    break;
  case HEADER:
    convertHeader(source);
    break;
  case LIST:
    convertList(source);
  default:
    break;
  }
}

enum Action getAction(char *source)
{
  if (strstr(source, "![") != NULL)
  {
    return IMG;
  }
  else if (strstr(source, "](") != NULL)
  {
    return LINK_TEXT;
  }
  else if (strstr(source, "<") != NULL && strstr(source, ">") != NULL)
  {
    return LINK;
  }
  else if (strstr(source, "***") != NULL)
  {
    return BOLD_ITALIC;
  }
  else if (strstr(source, "**") != NULL)
  {
    return BOLD;
  }
  else if (strstr(source, "*") != NULL)
  {
    return ITALIC;
  }
  else if (valueHeader(source) > 0)
  {
    return HEADER;
  }
  else if (valueList(source) > 0)
  {
    return LIST;
  }
  return NO_ACTION;
}

void runScript(char *source)
{
  // enum Action action = getAction(source);
  enum Action action = BOLD_ITALIC;
  doAction(source, action);
}

int main()
{

  FILE *file;
  char *estr;

  file = fopen("trpoTest.txt", "r");

  int i = 0;
  while (1)
  {
    char *arr = fileArray[i];
    // Чтение одной строки  из файла
    estr = fgets(arr, 1024, file);

    //Проверка на конец файла или ошибку чтения
    if (estr == NULL)
    {
      // Проверяем, что именно произошло: кончился файл
      // или это ошибка чтения
      if (feof(file) != 0)
      {
        //Если файл закончился, выводим сообщение о завершении
        //чтения и выходим из бесконечного цикла
        printf("\nЧтение файла закончено\n");
        break;
      }
      else
      {
        //Если при чтении произошла ошибка, выводим сообщение
        //об ошибке и выходим из бесконечного цикла
        printf("\nОшибка чтения из файла\n");
        break;
      }
    }
    //Если файл не закончился, и не было ошибки чтения
    //выводим считанную строку  на экран
    i++;
  }
  countStr = i;
  while (posScript != i)
  {
    runScript(fileArray[posScript]);
    printf("%s\n", fileArray[posScript]);
    posScript++;
  }
  // char *str = "1234512345****12345";
  // char dest[1024] = "";
  // replaceOnTag(str, dest, "****", "<strong>");

  // char* str = "* aSDASDSADAS";
  //  char* str = "fefew [Solid](https://cldup.com/dTxpPi9lDf.thumb.png) ewfwefewf";
  // runScript(str);
}

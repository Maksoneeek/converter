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
  CITATION,
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
int ValueCitation(char *source)
{
  int res = 0;
  char *arrayCitations[4];
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
  if (strstr(source,"> ")!= NULL)
  {
    int value = ValueCitation(source);
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
          for(int j = 0; j < count - value;j++)
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
          strcpy(fileArray[i-1], concat(fileArray[i-1], "</blockquote>\n"));
        }
        break;
      }
    }
  }  
} 
int valueList(char *source)
{
  int value = 0;
  if (strlen(source)>2)
  {
    if ((source[0] >= '1') && (source[0] <= '9') && (Pos(source, ". ") < 2))
    {
      value = 2;
    }
    if (((Pos(source, "+ ") < 1)&&(strstr(source,"+ ")!= NULL)) || ((Pos(source, "- ") < 1)&&(strstr(source,"- ")!= NULL)) || ((Pos(source, "* ") < 1)&&(strstr(source,"* ")!= NULL)))
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
    getSubString(source, text, value, Pos(source,"\n") - 1);
  
    char *convertedElement = concats("<li>", text, "</li>\n");

    if (value == 1)
    {
      convertedElement = concat("<ul>\n",convertedElement) ;
    }
    if (value == 2)
    {
      convertedElement = concat("<ol>\n",convertedElement) ;
    }

    strcpy(fileArray[posScript], convertedElement);

    for (int i = posScript + 1; i < countStr; i++)
    { 
      if ((valueList(fileArray[i])!= value)||(strlen(fileArray[i]) == 1))
      {
        if (value == 1)
        {
          strcpy(fileArray[i-1], concat(fileArray[i-1],"</ul>\n"));
        }

        if (value == 2)
        {
          strcpy(fileArray[i-1],concat(fileArray[i-1],"</ol>\n")) ;
        } 
        break;
      }
      else if (valueList(fileArray[i])==value)
      {
        char *str = malloc(strlen(fileArray[i]) - value);
        getSubString(fileArray[i], str, value, Pos(fileArray[i],"\n") - 1);
        str = concats("<li>",str,"</li>\n");
        
        strcpy(fileArray[i],str);
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
void convertHeader(char *source,int value)
{
  // int value = valueHeader(source);
  //printf("JOPA");
  if (value >0)
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

    getSubString(source, text, value, strlen(source) -2);
    char *result = concats(arrayHtmlHeadersStart[value - 1], text, arrayHtmlHeadersEnd[value - 1]);

    strcpy(fileArray[posScript], result);
  }  
}
char *convertStringElement(char *source, char *convertedElement, int posStart, int posEnd)
{
  char *startStr = malloc(posStart + 1); // Если что тут не было +1
  char *finishStr = malloc(strlen(source) - posEnd - 1);

  getStartString(source, startStr, posStart - 1);
  getEndString(source, finishStr, posEnd + 1);
  return concats(startStr, convertedElement, finishStr);
  // strcpy(destination, result);
}

void convertLinkText(char *source)
{
  if (strstr(source,"](") != NULL)
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

    if (strstr(source, "](") != NULL)
    {
      convertLinkText(source);
    }
  }  
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
  if (strstr(source,mask)!=NULL)
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
        if ((ValueCitation(fileArray[i]) > 0) || (strstr(fileArray[i],"<li>")!=NULL) || (valueHeader(fileArray[i]) > 0)|| (strlen(fileArray[i]) == 1)) {
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
  // if (strstr(source,"!["))
  // {
  // }
  if (strstr(source,"!["))
  {  
    char arrayHtmlImgStart1[10] = "<img src=\"";
    char arrayHtmlImgStart2[3] = "\" ";
    char arrayHtmlImgEnd2[3] = "\">";
    char *left = malloc(Pos(source, "![") );
    char *right = malloc(strlen(source) - Pos(source, "![") + 1);


    getStartString(source, left, Pos(source, "![") + 1);
    getEndString(source, right, Pos(source, "![") + 1);

    char *path = malloc(Pos(right, ")") - Pos(right, "](") - 2);
    char *text = malloc(Pos(right, "]"));

    getSubString(right, path, Pos(right, "](") + 1, Pos(right, ")") - 1);
    getSubString(right, text, -1, Pos(right, "]") - 1);

    char *convertedElement = concats(arrayHtmlImgStart1, path, arrayHtmlImgStart2);
    convertedElement = concats(convertedElement, "alt=\"", text);
    convertedElement = concat(convertedElement, arrayHtmlImgEnd2);

    char *result = convertStringElement(source, convertedElement, Pos(source, "!["), Pos(right, ")") + strlen(left) - 1);
    strcpy(fileArray[posScript], result);
    if (strstr(source, "![") != NULL)
    {
      convertImg(source);
    }
  } 
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
  convertHeader(source,valueHeader(source));
    break;
  // case LIST:
  //   convertList(source);
  //   break;
  case CITATION:
    convertCitation(source);
    break;
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
  else if (ValueCitation(source) > 0)
  {
    return CITATION;
  }
  // else if (valueList(source) > 0)
  // {
  //   return LIST;
  // }
  return NO_ACTION;
}

// void runScript(char *source)
// {
//   enum Action action = getAction(source);
//   // enum Action action = CITATION;
//   doAction(source, action);
// }

void runScript(char *source)
{
  convertImg(source);
  // printf("result %s\n", fileArray[posScript]);
  // printf("source %s\n", source);
  convertLinkText(source);
  convertList(source);
  convertSelectionText(source, "***", "<strong><italic>", "</italic></strong>");
  convertSelectionText(source, "**", "<strong>", "</strong>");
  convertSelectionText(source, "*", "<italic>", "</italic>");
  convertHeader(source,valueHeader(source));
  convertCitation(source);
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
    printf("%s", fileArray[posScript]);
    posScript++;
  }
  // char *str = "1234512345****12345";
  // char dest[1024] = "";
  // replaceOnTag(str, dest, "****", "<strong>");

  // char* str = "**dasdasdsa**";
  //  char* str = "fefew [Solid](https://cldup.com/dTxpPi9lDf.thumb.png) ewfwefewf";
  // runScript(str);
}

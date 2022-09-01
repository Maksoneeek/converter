#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char fileArray[1024][1024] = {};

int posScript = 0;

// Энам для обозначения типа дейяствия
enum Action
{
  LINK,
  LINK_TEXT,
  BOLD,
  IMG,
  NO_ACTION
};

char* concat(char *s1, char *s2) {
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);                      
  char *result = 	malloc(len1 + len2 + 3);
  /*if (!result) {
      fprintf(stderr, "malloc() failed: insufficient memory!\n");
      return NULL;
  }*/

  memcpy(result, s1, len1);
  memcpy(result + len1, s2, len2 + 1);    

  return result;
}

char* concats(char *s1, char *s2, char *s3) {

  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2) ;
  size_t len3 = strlen(s3);                      
  char *result = malloc(len1 + len2 + len3+1);

  if (!result) {
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
  //size_t len = strlen(str1);
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

char* convertStringElement(char* source, char* convertedElement, int posStart, int posEnd) {
  char* startStr = malloc(posStart);
  char* finishStr = malloc(strlen(source) - posEnd);

  getStartString(source, startStr, posStart-1);
  getEndString(source, finishStr, posEnd + 1);

  return concats(startStr,convertedElement,finishStr);
  // strcpy(destination, result);
}

void convertLinkText(char* source) {
	char arrayHtmlLinkStart1[9] = "<a href=\"" ;
	char arrayHtmlLinkStart2[3] = "\">";
	char arrayHtmlLinkEnd[5] = "</a>";

	char *link = malloc(Pos(source,")") - Pos(source,"](")-2);
  char *text = malloc(Pos(source,"]") - Pos(source,"[")-1);

	getSubString(source,link,Pos(source,"](")+1,Pos(source,")")-1);
	getSubString(source,text,Pos(source,"["),Pos(source,"]")-1);
	
  char *convertedElement = concats(arrayHtmlLinkStart1,link,arrayHtmlLinkStart2);
	convertedElement = concats(convertedElement,text,arrayHtmlLinkEnd);

  char* result = convertStringElement(source, convertedElement, Pos(source,"["), Pos(source,")") - 1);
  strcpy(fileArray[posScript], result);
  // printf("%s\n", result);
}

void convertLink(char* source) {
  char htmlLinkStart1[9] = "<a href=\"" ;
	char htmlLinkStart2[3] = "\">";
	char htmlLinkEnd[5] = "</a>";
	char *link = malloc(Pos(source,">") - Pos(source,"<")-1);
  // char* result;

	getSubString(source,link,Pos(source,"<"),Pos(source,">") -1);

  char* convertedElement = concats(htmlLinkStart1, link, htmlLinkStart2);
  convertedElement = concats(convertedElement, link, htmlLinkEnd);

  char* result = convertStringElement(source, convertedElement, Pos(source,"<"), Pos(source,">") -1);
  strcpy(fileArray[posScript], result);
}

void convertBold(char* source) {
  char *textBold1 = malloc(strlen(source) - Pos(source,"**"));
  getSubString(source,textBold1,Pos(source,"**")+1 ,strlen(source));

	if (strstr(textBold1,"**")!=NULL)
  { 
    char *textBold = malloc(Pos(textBold1,"**"));
    getSubString(textBold1,textBold,-1,Pos(textBold1,"**")-1);

    char *convertedElement = malloc(strlen(textBold)+17);
	  convertedElement = concats("<strong>",textBold,"</strong>");

    char* result = convertStringElement(source, convertedElement, Pos(source,"**"), Pos(source,textBold) + strlen(textBold));
    printf("%s\n",result);
  }
  
}
void convertImg(char *source){
  char arrayHtmlImgStart1[10] = "<img src=\"" ;
	char arrayHtmlImgStart2[3] = "\" ";
	char arrayHtmlImgEnd2[3] = "\">";
	char *path = malloc(Pos(source,")") - Pos(source,"](")-2);
  char *text = malloc(Pos(source,"]") - Pos(source,"![")+2);

  getSubString(source,path,Pos(source,"](")+1,Pos(source,")")-1);
  getSubString(source,text,Pos(source,"![")+1,Pos(source,"]")-1);

  char *convertedElement = concats(arrayHtmlImgStart1,path,arrayHtmlImgStart2);
  convertedElement = concats(convertedElement,"alt=\"",text);
  convertedElement = concat(convertedElement,arrayHtmlImgEnd2);

  char* result = convertStringElement(source, convertedElement, Pos(source,"!["), Pos(source,")")-1 );

  printf("%s\n",result);
}

void doAction(char* source, enum Action action) {
  switch (action)
  {
  case LINK_TEXT:
    convertLinkText(source);
    break;
  case LINK:
    convertLink(source);
    break;
  case BOLD:
    convertBold(source);
    break;
  case IMG:
    convertImg(source);
    break;
  default:
    break;
  }
}

enum Action getAction(char* source) {
  if (strstr(source,"![")!= NULL){
    return IMG;
  }else if (strstr(source, "](") != NULL){
    return LINK_TEXT;
  } else if (strstr(source, "<") != NULL && strstr(source, ">") != NULL) {
    return LINK;
  } else if (strstr(source, "**") != NULL) {
    return BOLD;
  }
  return NO_ACTION;
}

void runScript(char* source) {
  enum Action action = getAction(source);
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
  /* while (posScript != i)
  {
    runScript(fileArray[posScript]);
    printf("%s\n", fileArray[posScript]);
    posScript++;
  }*/

  char* str = "asdsaddasd![Alt text](//placehold.it/150x100)asdsadasdsa";
  // char* str = "fefew [Solid](https://cldup.com/dTxpPi9lDf.thumb.png) ewfwefewf";
  runScript(str);
}


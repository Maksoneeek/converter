#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int ValueCitation(char *source);
void convertCitation(char *source);
int valueList(char *source);
void convertList(char *source);
int valueHeader(char *source);
void convertHeader(char *source, int value);
char *convertStringElement(char *source, char *convertedElement, int posStart, int posEnd);
void convertLinkText(char *source);
void replaceOnTag(char *source, char *destination, char *mask, char *tag);
int findHtml(char *source);
char *findMask(char *source);
void convertLink(char *source);
void convertSelectionText(char *source, char *mask, char *openTag, char *closeTag);
void convertImg(char *source);
void runScript(char *source);
void runScriptCitation(char *source);
void convert();
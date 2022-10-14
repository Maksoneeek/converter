#include <ctest.h>
#include <libconverter/converter.h>
#include <libconverter/strings.h>
#include <stdio.h>
#include <stdlib.h>

char FileArray[1024][1024] = {};

int PosScript = 0;
int CountStr = 0;
CTEST(concat_suite, str1_null)
{
    char *s1 = NULL;
    char *s2 = "test";
    ASSERT_STR(NULL, concat(s1, s2));
}

CTEST(concat_suite, str2_null)
{
    char *s2 = NULL;
    char *s1 = "test";
    ASSERT_STR(NULL, concat(s1, s2));
}
CTEST(concat_suite, concat_right)
{
    char *s2 = "test";
    char *s1 = "test";
    ASSERT_STR("testtest", concat(s1, s2));
}

CTEST(concat3_suite, str1_null)
{
    char *s1 = NULL;
    char *s2 = "test";
    char *s3 = "test";
    ASSERT_STR(NULL, concat3(s1, s2, s3));
}
CTEST(concat3_suite, str2_null)
{
    char *s2 = NULL;
    char *s1 = "test";
    char *s3 = "test";
    ASSERT_STR(NULL, concat3(s1, s2, s3));
}
CTEST(concat3_suite, str3_null)
{
    char *s3 = NULL;
    char *s2 = "test";
    char *s1 = "test";
    ASSERT_STR(NULL, concat3(s1, s2, s3));
}
CTEST(concat3_suite, concat3_right)
{
    char *s1 = "test";
    char *s2 = "test";
    char *s3 = "test";
    ASSERT_STR("testtesttest", concat3(s1, s2, s3));
}

CTEST(pos_suite, str1_null)
{
    char *s1 = NULL;
    char *s2 = "test";
    ASSERT_EQUAL(-2, Pos(s1, s2));
}
CTEST(pos_suite, str2_null)
{
    char *s1 = "test";
    char *s2 = NULL;
    ASSERT_EQUAL(-2, Pos(s1, s2));
}
CTEST(pos_suite, dont_find)
{
    char *s1 = "test";
    char *s2 = "ddd";
    ASSERT_EQUAL(-1, Pos(s1, s2));
}
CTEST(pos_suite, find)
{
    char *s1 = "testtest";
    char *s2 = "test";
    ASSERT_EQUAL(0, Pos(s1, s2));
}

CTEST(getStartString_suite, right)
{
    char *s1 = "privettest";
    char *s2 = malloc(5 * sizeof(char));
    getStartString(s1, s2, 5);
    ASSERT_STR("privet", s2);
}

CTEST(getEndString_suite, right)
{
    char *s1 = "privettest";
    char *s2 = malloc(4 * sizeof(char));
    getEndString(s1, s2, 5);
    ASSERT_STR("test", s2);
}

CTEST(getSubString_suite, right)
{
    char *s1 = "privetpokattest";
    char *s2 = malloc(4 * sizeof(char));
    getSubString(s1, s2, 5, 9);
    ASSERT_STR("poka", s2);
}

CTEST(ValueCitation_suite, dont_find)
{
    char *s1 = "test";
    ASSERT_EQUAL(0, ValueCitation(s1));
}
CTEST(ValueCitation_suite, value1)
{
    char *s1 = "> test";
    ASSERT_EQUAL(1, ValueCitation(s1));
}
CTEST(ValueCitation_suite, value2)
{
    char *s1 = ">> test";
    ASSERT_EQUAL(2, ValueCitation(s1));
}
CTEST(ValueCitation_suite, value3)
{
    char *s1 = ">>> test";
    ASSERT_EQUAL(3, ValueCitation(s1));
}
CTEST(ValueCitation_suite, value4)
{
    char *s1 = ">>>> test";
    ASSERT_EQUAL(4, ValueCitation(s1));
}
CTEST(ValueCitation_suite, value5)
{
    char *s1 = ">>>>> test";
    ASSERT_EQUAL(5, ValueCitation(s1));
}

CTEST(valueList_suite, dont_find)
{
    char *s1 = "test";
    ASSERT_EQUAL(0, valueList(s1));
}
CTEST(valueList_suite, find_ol)
{
    char *s1 = "1. test";
    ASSERT_EQUAL(2, valueList(s1));
}
CTEST(valueList_suite, find_ul_plus)
{
    char *s1 = "+ test";
    ASSERT_EQUAL(1, valueList(s1));
}
CTEST(valueList_suite, find_ul_minus)
{
    char *s1 = "- test";
    ASSERT_EQUAL(1, valueList(s1));
}
CTEST(valueList_suite, find_ul_star)
{
    char *s1 = "* test";
    ASSERT_EQUAL(1, valueList(s1));
}

CTEST(valueHeader_suite, dont_find)
{
    char *s1 = "test";
    ASSERT_EQUAL(0, valueHeader(s1));
}
CTEST(valueHeader_suite, value1)
{
    char *s1 = "# test";
    ASSERT_EQUAL(1, valueHeader(s1));
}
CTEST(valueHeader_suite, value2)
{
    char *s1 = "## test";
    ASSERT_EQUAL(2, valueHeader(s1));
}
CTEST(valueHeader_suite, value3)
{
    char *s1 = "### test";
    ASSERT_EQUAL(3, valueHeader(s1));
}
CTEST(valueHeader_suite, value4)
{
    char *s1 = "#### test";
    ASSERT_EQUAL(4, valueHeader(s1));
}
CTEST(valueHeader_suite, value5)
{
    char *s1 = "##### test";
    ASSERT_EQUAL(5, valueHeader(s1));
}
CTEST(valueHeader_suite, value6)
{
    char *s1 = "###### test";
    ASSERT_EQUAL(6, valueHeader(s1));
}

CTEST(convertStringElement_suite, LinkText)
{
    char *s1 = "test[text](link)test";
    char *s2 = "<a href=\"link\">text</a>";
    char *s3 = malloc(28 * sizeof(char));
    s3 = convertStringElement(s1, s2, 4, 14);
    ASSERT_STR("test<a href=\"link\">text</a>test", s3);
}
CTEST(convertStringElement_suite, Link)
{
    char *s1 = "test<link>test";
    char *s2 = "<a href=\"link\">link</a>";
    char *s3 = malloc(24 * sizeof(char));
    s3 = convertStringElement(s1, s2, 4, 8);
    ASSERT_STR("test<a href=\"link\">link</a>test", s3);
}
CTEST(convertStringElement_suite, img)
{
    char *s1 = "test![text](link)test";
    char *s2 = "<img=\"link\"alt=\"text\">";
    char *s3 = malloc(28 * sizeof(char));
    s3 = convertStringElement(s1, s2, 4, 15);
    ASSERT_STR("test<img=\"link\"alt=\"text\">test", s3);
}

CTEST(findHtml_suite, dont_find)
{
    char *s1 = "testtest";
    ASSERT_EQUAL(0, findHtml(s1));
}
CTEST(findHtml_suite, find_open_link)
{
    char *s1 = "test<a>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_close_link)
{
    char *s1 = "test</a>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_open_list)
{
    char *s1 = "test<li>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_close_list)
{
    char *s1 = "test</li>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_open_strong)
{
    char *s1 = "test<strong>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_close_strong)
{
    char *s1 = "test</strong>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_open_italic)
{
    char *s1 = "test<italic>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_close_italic)
{
    char *s1 = "test</italic>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_open_citation)
{
    char *s1 = "<blockquote>\ntesttest";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_close_citation)
{
    char *s1 = "</blockquote>\n test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_open_list_ol)
{
    char *s1 = "test<ol>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_close_list_ol)
{
    char *s1 = "test</ol>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_open_list_ul)
{
    char *s1 = "test<ul>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findHtml_suite, find_close_list_ul)
{
    char *s1 = "test</ul>test";
    ASSERT_EQUAL(1, findHtml(s1));
}
CTEST(findMask_suite, dont_find)
{
    char *s1 = "test";
    ASSERT_STR(NULL, findMask(s1));
}
CTEST(findMask_suite, find_open_boldItalic)
{
    char *s1 = "test<strong></italic>test";
    ASSERT_STR("<strong></italic>", findMask(s1));
}
CTEST(findMask_suite, find_close_boldItalic)
{
    char *s1 = "test</italic></strong>test";
    ASSERT_STR("</italic></strong>", findMask(s1));
}
CTEST(findMask_suite, find_open_bold)
{
    char *s1 = "test<strong>test";
    ASSERT_STR("<strong>", findMask(s1));
}
CTEST(findMask_suite, find_close_bold)
{
    char *s1 = "test</strong>test";
    ASSERT_STR("</strong>", findMask(s1));
}
CTEST(findMask_suite, find_open_italic)
{
    char *s1 = "test<italic>test";
    ASSERT_STR("<italic>", findMask(s1));
}
CTEST(findMask_suite, find_close_italic)
{
    char *s1 = "test</italic>test";
    ASSERT_STR("</italic>", findMask(s1));
}

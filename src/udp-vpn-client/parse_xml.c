#include <stdio.h>
#include <string.h>
#include "parse_xml.h"

int GetXmlTagValue(char *pResBuf, char *pTag, char *pTagValue)
{
    int len=0, pos = 0;
    char openTag[100] = {0}; //Opening Tag
    char closeTag[100] = {0};//Closing Tag
    int posOpenTag=0, posClosingTag=0;
    //check enter buffer
    len = strlen(pResBuf);
    if (len<=0)
    {
        return -1;
    }
    //Create Opening Tag
    memset(openTag, 0, sizeof(openTag));
    strcpy(openTag, "<");
    strcat(openTag, pTag);
    strcat(openTag, ">");
    //Create Closing tag
    memset(closeTag, 0, sizeof(closeTag));
    strcpy(closeTag, "</");
    strcat(closeTag, pTag);
    strcat(closeTag, ">");
    //Get len of open and close tag
    const int lenOpenTag = strlen(openTag);
    const int lenCloseTag = strlen(closeTag);
    //Get Opening tag position
    for (pos=0; pos<len; pos++)
    {
        if ( !memcmp(openTag,(pResBuf+pos),lenOpenTag))
        {
            posOpenTag = pos;
            break;
        }
    }
    //Get closing tag position
    for (pos=0; pos<len; pos++)
    {
        if ( !memcmp(closeTag,(pResBuf+pos),lenCloseTag) )
        {
            posClosingTag = pos;
            break;
        }
    }
    //get the tag value
    if ( (posOpenTag !=0) && (posClosingTag !=0) )
    {
        const int lenTagVal = posClosingTag-posOpenTag-lenOpenTag;
        const char * const pStartPosTagVal = pResBuf+posOpenTag+lenOpenTag;
        if (lenTagVal)
        {
            //Get tag value
            memcpy(pTagValue,pStartPosTagVal, lenTagVal);
            if (strlen(pTagValue))
            {
                return 1;
            }
        }
    }
    return -1;
}


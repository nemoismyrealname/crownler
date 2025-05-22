#ifndef FAQ_H
#define FAQ_H

#include "globals.h"

typedef struct
{
    const char *label; // value name
    const char *value; // value

} FAQEntry;

extern FAQEntry* (*get_faq_data[])(); //func pointer array each module returns
extern int faq_data_count; // func quntity

void faq(int rows, int cols);

#endif
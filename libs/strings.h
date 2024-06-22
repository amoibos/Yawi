#ifndef STRINGS_H
#define STRINGS_H

#define PREFIX SEGA_

void SEGA_itoa(long value, char *sp);
int SEGA_atoi(char* str);
//float atof(char* arr);
//char* ftoa(float value, int decimals, char* buf);


void strcat(char* str1, char* str2);

char strcmp(char* str1, char* str2);
//void strncpy(char* dest, char* src, int n); 
char strlen(const char* str);
char strpos(char* search, char* content, char start);
void strcpy(char* dst, char* src);

char * strchr(const char *s, char ch);

char upcase(char c);
char is_alpha(const char mark);
char * to_upper(char* string);
char is_digit(const char ch);
char * sprint(char * buffer, char * format, char **strings, long  * numbersInt, float *numbersFloat);


#endif
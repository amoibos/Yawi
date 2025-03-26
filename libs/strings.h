#ifndef STRINGS_H
#define STRINGS_H

unsigned char* SEGA_itoa(long value, unsigned char * sp);
long SEGA_atoi(const unsigned char * str);
//float atof(char* arr);
//char* ftoa(float value, int decimals, char* buf);


unsigned char* strcat(unsigned char * dest, unsigned char * src);

//signed char strcmp(unsigned char * str1, const unsigned char * str2);
//void strncpy(unsigned char * dest, const unsigned char * src, const unsigned char amount); 
unsigned char strlen(const unsigned char* str);
//signed char strpos(unsigned char* search, unsigned char * content, unsigned char start);
unsigned char* strcpy(unsigned char* dest, const unsigned char* src);

signed char strchr(const unsigned char *str, unsigned char ch);

//unsigned char upcase(const unsigned char c);
//unsigned char is_alpha(const unsigned char mark);
unsigned char * to_upper(unsigned char * src);
//unsigned char is_digit(const unsigned char ch);
//unsigned char * sprint(unsigned char * buffer, const unsigned char * format, const unsigned char **strings, const long * numbersInt, const float * numbersFloat);


#endif
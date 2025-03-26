#include "strings.h"

unsigned char strlen(const unsigned char * str) {
	const unsigned char *s;

	for (s = str; *s; ++s) ;

    return s - str;
}

/*FIXME: hopefully the result is never above 127*/
/*
signed char strpos(const unsigned char * search, const unsigned char * content, const unsigned char start) {
	unsigned char i, j, check;
    signed char result=-1;
	unsigned char len_search = strlen(search);
	unsigned char len_content = strlen(content);

	i = start;
	if (
        (len_search > len_content) ||
        (*search == '\0') ||
        (i > (len_content - 1))
        )
        return result;

	for(; i < len_content; ++i)	{
		check = 0;
		for(j = 0; j < len_search; ++j)
			if(content[i + j] == search[j])
				++check;

		if(check == len_search) {
			result = i;
			break;
		}

	}
	return result;
}
*/

unsigned char* SEGA_itoa(long value, unsigned char * sp) {
    long q, r;
    unsigned char *start;
    unsigned char t, sign = value < 0;
    unsigned char *dest = sp;

    start = sp;

    if (sign)
        value *= -1;

    do {
        q = value / 10;
        r = value % 10;

        if (q)
            value /= 10;
        else
            value -= r;

        *sp++ = '0' + r;
    } while(value);
    if (sign)
        *sp++ = '-';

    *sp-- = '\0';


    while(start < sp) {
        t = *start;
        *start++ = *sp;
        *sp-- = t;
    }
    return dest;
}

long SEGA_atoi(const unsigned char * str) {
	long k = 0;
    unsigned char start;

    start = *str;
	while (*str)
	{
		k = (k << 3) + (k << 1) + (*str) - '0';
		++str;
	}
	return k * ((start == '-') ? -1 : 1);
}

unsigned char* strcat(unsigned char * dest, const unsigned char * src) {
	unsigned char *rdest = dest;

	while (*dest)	dest++;
	while (*dest++ = *src++) ;

	return rdest;
}

/*
signed char strcmp(const char * str1, const char * str2) {
	while(*str1 && (*str1 == *str2)) {
		++str1;
		++str2;
	}
	return *(const unsigned char*)str1 - *(const unsigned char*)str2;
} */
/*
void strncpy(unsigned char* dst, const unsigned char* src, const unsigned char amount) {
	unsigned char i = 0;
	while(i++ != amount && (*dst++ = *src++));
}*/

unsigned char* strcpy(unsigned char* dest, const unsigned char* src) {
	unsigned char* rdest = dest;

    while(*dest++ = *src++);

    return rdest;
}

unsigned char upcase(const unsigned char c) {

    if ((c >= 'A') && (c <='Z'))
        return c;
    else if ((c >= 'a') && (c <='z'))
        return c - 32;
    else if ((c >= 'A' + 128) && (c <='Z' + 128))
        return c;
    else if ((c >= 'a') && (c <='z'))
        return c - 32;
    else
        return c;

}

signed char strchr(const unsigned char *str, const unsigned char ch) {
	unsigned char result=0;

    for (; str[result] != '\0'; ++result)
  		if (str[result] == ch)
    		return result;

	return -1;
}

/*
unsigned char is_alpha(const unsigned char mark) {
	return 	(mark >= 0x41) && (mark <= 0x5A) ||
			(mark >= 0x61) && (mark <= 0x7A);
}*/

/*long long pow(long long base, int ex) {
	long long result = base;
	for(int i = 0; i < ex; ++i)
		result *= base;
	return result;
} */

/*
float str_to_float(const unsigned char  * arr) {
    int i,j,flag;
    float val;
    char c;
    i=0;
    j=0;
    val=0;
    flag=0;
    while ((c = *(arr+i))!='\0')
    {
//      if ((c<'0')||(c>'9')) return 0;
        if (c!='.')
        {
            val =(val*10)+(c-'0');
            if (flag == 1)
	            --j;
        }
        if (c == '.')
        {
        	if (flag == 1)
        		return 0;
        	flag=1;
        }
        ++i;
    }
    val = val*pow(10,j);
    return val;
}
*/
// used in ftoa
/*
int itoa_s(int value, char *buf) {
        int index = 0;
        int i = value % 10;
        if (value >= 10) {
                index += itoa_s(value / 10, buf);
        }
        buf[index] = i+0x30;
        index++;
        return index;
}

unsigned char * ftoa(float value, int decimals, unsigned char * buf) {
	unsigned int d;
	unsigned int index = 0;
	// Handle negative values
	if (value < 0) {
	       buf[index] = '-';
	       index++;
	       value = -value;
	}

	// Rounding
	float rounding = 0.5;
	for (d = 0; d < decimals; rounding /= 10.0, d++);
	value += rounding;

	// Integer part
	index += itoa_s((int)(value), buf+index);
	buf[index++] = '.';

	// Remove everything except the decimals
	value = value - (int)(value);

	// Convert decmial part to integer
	int ival = 1;
	for (d = 0; d < decimals; ival *= 10, d++);
	ival *= (int)value;

	// Add decimal part to string
	index += itoa_s(ival, buf+index);
	buf[index] = '\0';

  return buf;
}
*/

// WARNING: capitalize src!

unsigned char * to_upper(unsigned char * src) {
	unsigned char *temp;
	for (temp = src; *temp; ++temp)
		*temp = (char)upcase(*temp);
	return src;
}
/*
unsigned char is_digit(const unsigned char ch) {
	return (ch >= '0') && (ch <= '9');
}*/
/*
unsigned char * sprint(unsigned char * buffer, unsigned char * format, unsigned char **strings, long  * numbersInt, float * numbersFloat) {
    unsigned char * str;
    long integer;
    float real;
    unsigned char buffer_pos=0;
    unsigned char temp[80+1];
    char * start;

    start = buffer;
    buffer[0] = 0;
    for (char format_pos=0; format_pos < strlen(format); ++format_pos) {
        if (format[format_pos] != '%') {
            buffer[buffer_pos++] = format[format_pos];
        } else if ((format[format_pos] == '%') && (format[format_pos+1] == '%')) {
            buffer[buffer_pos++] = format[format_pos++];
        } else {
            ++format_pos;
            switch (format[format_pos]) {
                case 's': {
                    str = *strings;
                    ++strings;
                    buffer[buffer_pos] = 0;
                    strcat(buffer, str);
                    break;
                }
                case 'd': {
                    integer = *numbersInt++;
                    buffer[buffer_pos] = 0;
                    SEGA_itoa(integer, temp);
                    strcat(buffer, temp);
                    break;
                }
                  case 'f': {
                    real = *numbersFloat++;
                    buffer[buffer_pos] = 0;
                    ftoa(real, 10, temp);
                    strcat(buffer, temp);
                    break;
                }
                default: {
                    break;
                }
            }

        }
    }
    buffer[buffer_pos] = 0;
    return start;
}
*/

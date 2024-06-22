#define PREFIX SEGA_

char strlen(const char * str) {
	const char *s;
	for (s = str; *s; ++s) ;
	return s - str;
}

char strpos(char * search, char * content, char start) {
	int i, j, check, result = -1;
	int len_search = strlen(search);
	int len_content = strlen(content);  
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

void SEGA_itoa(long value, char *sp) {
    long q, r;
    char *start;
    char t, sign = value < 0;

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
}

int SEGA_atoi(char* str) {
	int k = 0;
	while (*str) 
	{
		k = (k << 3) + (k << 1) + (*str) - '0';
		++str;
	}
	return k;
}

char* strcat(char *dest, const char *src) {
	char *rdest = dest;

	while (*dest)	dest++;
	while (*dest++ = *src++) ;
	
	return rdest;
}

int strcmp(const char* str1, const char* str2) {
	while(*str1 && (*str1 == *str2)) {
		++str1;
		++str2;
	}
	return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

void strncpy(char* dst, const char* src, char n) {
	char i = 0;
	while(i++ != n && (*dst++ = *src++));
}

void strcpy(char* dst, const char* src) {
	while(*dst++ = *src++);
}

char upcase(char c) {
	
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

const char *strchr(const char *s, const char ch) {
	for (; *s != '\0'; ++s)
  		if (*s == ch)
    		return s;

	return 0;
}


char is_alpha(const char mark) {
	return 	(mark >= 0x41) && (mark <= 0x5A) ||
			(mark >= 0x61) && (mark <= 0x7A); 
}

int pow(int base, int ex) {
	int result = base;
	for(int i = 0; i < ex; ++i)
		result *= base;
	return result;
}

float str_to_float(char *arr) {
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

char * ftoa(float value, int decimals, char* buf) {
	int d;	
	int index = 0;
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

// WARNING: capitalize source string! 
char * to_upper(char* string) {
	char *temp;
	for (temp = string; *temp; ++temp)
		*temp = (char)upcase(*temp);
	return string;
}

char is_digit(const char ch) {
	return (ch >= '0') && (ch <= '9');

}

char * sprint(char * buffer, char * format, char **strings, long  * numbersInt, float *numbersFloat) {
    char * str;
    long integer;
    float real;
    char buffer_pos=0;
    char temp[80+1];
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

#ifndef SPRYNG_SAFE_H
#define SPRYNG_SAFE_H

#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <assert.h>
#include <alloca.h>

int length_prototype(const char string[]){int count = 0;while( string[count++] != '\0' ){}return --count;}
int strip_prototype(char string[], char delimiter){int count = 0;while( string[count++] == delimiter ){}return --count;}

#define length(x)  _Generic((x), char*:length_prototype, const char*: length_prototype)(x)

int count_prototype(char string[], char delimiter, int start, int end){

    int count = 0;
    for (int i=start; i<end; i++){

        if (string[i] == delimiter){count++;}
    }

    return count;
}

#define count(string, delimiter)    count_prototype(string, delimiter, 0, length(string))

void reverse(char substr[], char string[]){

    int j = 0;
    for (int i=0; i<length(string); i++){

        substr[j++] = string[length(string)-i-1];
    }

    substr[j] = '\0';
}

void step_substring(char substr[], char string[], const int start, const int end, const int step){

    assert( (length(string) < INT_MAX) && (start < end) && (step > 0) );

    int j = 0;
    for (int i=start; i<end; i+=step){

        substr[j++] = string[i];
    }

    substr[j] = '\0';
}

#define substring(substr, string, start, end) step_substring(substr, string, start, end, 1)
#define strip_left(substr, string, delimiter) substring(substr, string, strip_prototype(string, delimiter), length(string))

void strip_right(char substr[], char string[], char delimiter){

    char* intermediate = (char*)malloc( (length(string)+1) * sizeof(char) );

    reverse(substr, string);
    strip_left(intermediate, substr, delimiter);
    reverse(substr, intermediate);

    free(intermediate);
}

void strip_to(char substr[], char string[], char delimiter){

    char* intermediate = (char*)malloc( (length(string)+1) * sizeof(char) );

    strip_left(intermediate, string, delimiter);
    strip_right(substr, intermediate, delimiter);

    free(intermediate);
}

#define strip(string, delimiter) strip_to(string, string, delimiter)

int equals(char self[], char other[]){

    if (length(self) != length(other)) return 0;

    for (int i=0; i<length(self); i++){

        if (self[i] != other[i]){

            return 0;
        }
    }

    return 1;
}

int char_in(char expect, char string[]){

    if (expect == '\0') return -1;

    for (int i=0; i<length(string); i++){

        if (expect == string[i]){

            return 1;
        }
    }

    return 0;
}

int substr_in(char expect[], char string[]){

    char* kernel = (char*)malloc( length(expect) * sizeof(char*) );
    if (length(expect) > length(string)) return -1;

    for (int i=0; i<=(length(string) - length(expect)); i++){

        substring(kernel, string, i, i + length(expect));
        if (equals(kernel, expect)){

            free(kernel);
            return 1;
        }
    }

    free(kernel);
    return 0;
}

#define in(child, parent) _Generic((child), int: char_in, char: char_in, char*: substr_in)(child, parent)

void concat_prototype(char string[], const char self[], const char other[]){

    for (int i=0; i<length(self); i++){string[i] = self[i];}
    for (int j=0; j<length(other); j++){string[j+length(self)] = other[j];}

    string[ length(self) + length(other) ] = '\0';
}

void concatfn(const char self[], const char other[], void(*function)(char*, va_list), ...){

    int total = (length(self)+length(other));
    char* result = (char*)malloc( (total +1) * sizeof(char) );
    va_list argv;
    va_start(argv, function);

    concat_prototype(result, self, other);
    function(result, argv);

    va_end(argv);
    free(result);
}

void save_prototype(char result[], va_list argv){

    char* string = va_arg(argv, char*);
    for (int i=0; i<=length(result); i++){string[i] = result[i];}
};

#define SAVE(string)    save_prototype, string

int iscase_prototype(char c, char start, char end){

    return ( c >= start ) && ( c <= end );
}

int islowercase(const char c){return iscase_prototype(c, 'a', 'z');}
int isuppercase(const char c){return iscase_prototype(c, 'A', 'Z');}

int islowercases(const char string[]){for (int i=0; i<length(string); i++){if (!islowercase(string[i])) return 0;}return 1;}
int isuppercases(const char string[]){for (int i=0; i<length(string); i++){if (!isuppercase(string[i])) return 0;}return 1;}

#define islower(string) _Generic((string),  char:islowercase, const char:islowercase, int:islowercase, const int:islowercase,   \
                                            char*:islowercases, const char*:islowercases)(string)                               \

#define isupper(string) _Generic((string),  char:isuppercase, const char:isuppercase, int:isuppercase, const int:isuppercase,   \
                                            char*:isuppercases, const char*:isuppercases)(string)                               \

#define CASE_DIFF ('A'-'a')

int cisalpha(char character){

    return (islowercase(character) || isuppercase(character));
}

char swapcase_prototype(char single){

    if (islowercase(single)) {return (char)(single + CASE_DIFF);}
    else if (isuppercase(single)) {return (char)(single - CASE_DIFF);}

    else {return -1;}
}

char swapcase(char single){

    assert( islowercase(single) || isuppercase(single) );

    return swapcase_prototype(single);
}

void swapcases(char dst[], char src[]){

    for (int i=0; i<length(src); i++){if (cisalpha(src[i])){dst[i] = swapcase(src[i]);}}
    dst[length(src)] = '\0';
}

char __GLOBAL_RESERVED_CHECK = -1;
char __GLOBAL_RESERVED_REPLACE = -1;

int __GET_CHECK(char self)          {assert( __GLOBAL_RESERVED_CHECK != -1 );return ( self == __GLOBAL_RESERVED_CHECK );}
char __RET_REPLACE(char _)          {return __GLOBAL_RESERVED_REPLACE;}
void __SET_CHECK(char check)        {__GLOBAL_RESERVED_CHECK = check;}
void __SET_REPLACE(char replace)    {__GLOBAL_RESERVED_REPLACE = replace;}

void MAP_APPLY(char string[], char(*map)(char), int(*istrue)(char), void(*run)(va_list), ...){

    va_list argv;
    va_start(argv, run);

    run(argv);
    for (int i=0; i<length(string); i++){

        if (istrue(string[i])) {string[i] = map(string[i]);}
    }

    string[length(string)] = '\0';
    __SET_CHECK(-1);
    __SET_REPLACE(-1);
    va_end(argv);
}

void pass(va_list argv){}
void check_replace_prototype(va_list argv){

    __SET_CHECK((char)va_arg(argv, int));
    __SET_REPLACE((char)va_arg(argv, int));
}

#define MAP_SWAP    swapcase,   cisalpha,       pass
#define MAP_UPPER   swapcase,   islowercase,    pass
#define MAP_LOWER   swapcase,   isuppercase,    pass
#define MAP_REPLACE(check, replace) __RET_REPLACE,    __GET_CHECK,    check_replace_prototype, check, replace

void splitfn_prototype(char string[], char delimiter, void(*function)(char*, va_list), ...){

    int index = 0;
    va_list argv;

    strip(string, delimiter);
    va_start(argv, function);

    while (string[++index] != delimiter){if(string[index] == '\0')break;}

    char* tokk = (char*)malloc( (index+1) * sizeof(char) );

    substring(tokk, string, 0, index);
    function(tokk, argv);

    substring(string, string, index, length(string)+1);
    va_end(argv);
    free(tokk);
}

#define splitfn(string, dl, func, ...) {while(length(string) > 0){splitfn_prototype(string, dl, func, ##__VA_ARGS__);}}

void printc(char string[], va_list argv){char arg=(char)va_arg(argv,int);printf("%s has %d '%c's in it\n", string, count(string, arg), arg);}

int DIGITS(int number){

    assert( (number > INT_MIN) && (number <= INT_MAX) );

    if (number == 0) return 1;
    else if (number < 0) return DIGITS(-number);
    else{

        int count = 1;
        while ((number/10) > 0){count++;number /= 10;}
        return count;
    }
}

#define __GLOBAL_RESERVED_INT_MAGNITUDE_LIMIT 11 // DIGITS(INT_MAX) + 1(overhead)

void INT_TO_CHAR(char* cc, int number){

    assert( DIGITS(number) == 1 );
    *cc = (char)(number + '0');
}

void i2cs_prototype(char string[], int number, char delimiter) {

    int i = DIGITS(number);
    if (number == 0) {string[0] = '0';string[1] = '\0';}
    if (number < 0) {

        i++;
        string[0] = '-';
        number = -number;
    }

    string[i--] = delimiter;
    while (number > 0){

        INT_TO_CHAR(&string[i--], (number % 10));
        number /= 10;
    }
}

#define INT_TO_CHARS(string, number) i2cs_prototype(string, number, '\0')
#define FRACTION(number) (number - ((int)number))

void FLOAT_TO_CHARS(char string[], float number) {

    if (FRACTION(number) == 0) {

        INT_TO_CHARS(string, (int)number);
        return;
    }

    if (number < 0){

        FLOAT_TO_CHARS(string, -number);
        concatfn("-", string, SAVE(string));
        return;
    }

    i2cs_prototype(string, number, '.');
    int i = DIGITS(number) +1;
    while (FRACTION(number) != 0){

        number *= 10;
        INT_TO_CHAR(&string[i++], ((int)(number) % 10));
    }

    string[i] = '\0';
}

#endif // SPRYNG_SAFE_H

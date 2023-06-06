#ifndef SPRYNG_SAFE_H
#define SPRYNG_SAFE_H

#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <assert.h>
#include <alloca.h>

int length_prototype(const char string[]){int count = 0;while( string[count++] != '\0' ){}return (--count);}
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

    if (length(expect) > length(string)) return 0;
    char* kernel = (char*)malloc( (length(expect)+1) * sizeof(char) );

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
char __RET_REPLACE(char _)          {assert( _ != '\0' );return __GLOBAL_RESERVED_REPLACE;}
void __SET_CHECK(char check)        {__GLOBAL_RESERVED_CHECK = check;}
void __SET_REPLACE(char replace)    {__GLOBAL_RESERVED_REPLACE = replace;}

#define NONE (char)127

void __GLOBAL_RESERVED_RESOLVE(char string[]){

    if (!in(NONE, string)){return;}

    char* intermediate = (char*)malloc( (length(string)+1) * sizeof(char) );
    substring(intermediate, string, 0, length(string));

    int count = 0;
    for (int i=0; i<length(string); i++){

        if (intermediate[i] != NONE){

            string[count++] = intermediate[i];
        }
    }

    string[count] = '\0';
    free(intermediate);
}

void MAP_APPLY(char string[], char(*map)(char), int(*istrue)(char), void(*run)(va_list), ...){

    va_list argv;
    va_start(argv, run);

    run(argv);
    for (int i=0; i<length(string); i++){

        if (istrue(string[i])) {string[i] = map(string[i]);}
    }

    string[length(string)] = '\0';
    __GLOBAL_RESERVED_RESOLVE(string);
    __SET_CHECK(-1);
    __SET_REPLACE(-1);
    va_end(argv);
}

void PASS_STR(char _[], va_list __){(void)_;(void)__;}
void pass(){}
void check_replace_prototype(va_list argv){

    __SET_CHECK((char)va_arg(argv, int));
    __SET_REPLACE((char)va_arg(argv, int));
}

#define MAP_SWAP    swapcase,   cisalpha,       pass
#define MAP_UPPER   swapcase,   islowercase,    pass
#define MAP_LOWER   swapcase,   isuppercase,    pass
#define MAP_REPLACE(check, replace) __RET_REPLACE,    __GET_CHECK,    check_replace_prototype, check, replace
#define MAP_REMOVE(remove) MAP_REPLACE(remove, NONE)

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
void PRINTLN(char string[], va_list argv){(void)argv;printf("%s\n", string);}

int DIGITS_INT(int number){

    assert( (number > INT_MIN) && (number <= INT_MAX) );

    if (number == 0) return 1;
    else if (number < 0) return DIGITS_INT(-number);
    else{

        int count = 1;
        while ((number/10) > 0){count++;number /= 10;}
        return count;
    }
}

int DIGITS_LONG(long number){

    assert( (number > LONG_MIN) && (number <= LONG_MAX) );

    if (number == 0) return 1;
    else if (number < 0) return DIGITS_LONG(-number);
    else{

        int count = 1;
        while ((number/10) > 0){count++;number /= 10;}
        return count;
    }
}

int DIGITS_LLONG(long long number){

    assert( (number > LLONG_MIN) && (number <= LLONG_MAX) );

    if (number == 0) return 1;
    else if (number < 0) return DIGITS_LLONG(-number);
    else{

        int count = 1;
        while ((number/10) > 0){count++;number /= 10;}
        return count;
    }
}

#define DIGITS(x) _Generic((x), int: DIGITS_INT, float: DIGITS_INT, long: DIGITS_LONG, double: DIGITS_LONG, long long: DIGITS_LLONG, long double: DIGITS_LLONG)(x)

#define __GLOBAL_RESERVED_INT_MAGNITUDE_LIMIT 11 // DIGITS(INT_MAX) + 1(overhead)
#define __GLOBAL_RESERVED_LONG_MAGNITUDE_LIMIT 20 // DIGITS(LONG_MAX) + 1(overhead)

void INT_TO_CHAR(char* cc, int number){

    assert( DIGITS(number) == 1 );
    *cc = (char)(number + '0');
}

void i2cs_prototype(char string[], int number, char delimiter) {

    int i = DIGITS_INT(number);
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
void l2cs_prototype(char string[], long number, char delimiter) {

    int i = DIGITS_LONG(number);
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

void INT_TO_CHARS(int number, char string[]) {i2cs_prototype(string, number, '\0');}
void LONG_TO_CHARS(long number, char string[]) {l2cs_prototype(string, number, '\0');}

#define FRACTION(number) (number - ((int)number))

void FLOAT_TO_CHARS(float number, char string[]) {

    if (FRACTION(number) == 0) {

        INT_TO_CHARS((int)number, string);
        return;
    }

    if (number < 0){

        FLOAT_TO_CHARS(-number, string);
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

#define CHARS(x, string) _Generic((x), int: INT_TO_CHARS, long: LONG_TO_CHARS, float: FLOAT_TO_CHARS, double: LONG_TO_CHARS)(x, string)

int EVAL_INT(char string[]){

    if (string[0] == '-'){

        char* intermediate = (char*)alloca( length(string) * sizeof(char) );
        substring(intermediate, string, 1, length(string));
        return (0 - EVAL_INT(intermediate));
    }

    int result = 0;
    for (int i=0; i<length(string); i++){

        int power = 1;
        for (int j=1; j<(length(string)-i); j++){

            power *= 10;
        }
        result += ((string[i] - '0') * power);
    }

    return result;
}

double EVAL_DOUBLE(char string[]){

    if (string[0] == '-'){

        char* intermediate = (char*)alloca( length(string) * sizeof(char) );
        substring(intermediate, string, 1, length(string));
        return (0 - EVAL_DOUBLE(intermediate));
    }

    int dot = 0;
    while (string[dot++] != '.'){}

    char* integer_s = (char*)malloc( dot * sizeof(char) );
    char* fraction_s = (char*)malloc( (length(string) - dot + 1 ) * sizeof(char) );

    substring(integer_s, string, 0, dot-1);
    substring(fraction_s, string, dot, length(string));

    int integer = EVAL_INT(integer_s);
    int fraction = EVAL_INT(fraction_s);

    double integer_f = (double)integer;
    double fraction_f = (double)fraction;

    for (int i=0; i<length(fraction_s); i++){fraction_f /= 10;}

    free(integer_s);
    free(fraction_s);

    return (integer_f + fraction_f);
}

#define EVAL(string) ((in('.', string) == 1) ? EVAL_DOUBLE(string) : EVAL_INT(string))

int startswith(char prefix[], char string[]){

    assert( length(prefix) <= length(string) );

    for (int i=0; i<length(prefix); i++){

        if (prefix[i] != string[i]){

            return 0;
        }
    }

    return 1;
}

int endswith(char suffix[], char string[]){

    char* suffix_r = (char*)malloc( (length(suffix) +1) * sizeof(char) );
    char* string_r = (char*)malloc( (length(string) +1) * sizeof(char) );

    reverse(suffix_r, suffix);
    reverse(string_r, string);

    int result = startswith(suffix_r, string_r);

    free(suffix_r);
    free(string_r);

    return result;
}

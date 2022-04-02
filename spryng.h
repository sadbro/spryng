
#pragma once

#ifndef SPRYNG_H
#define SPRYNG_H

int length_i(int a){

    int count = 1;
    while (a / 10){

        count++;
        a /= 10;
    }

    return count;
}

int length(char* ss){

    int c = 0;
    while(ss[c]){c++;}

    return c;
}

#define __FUNC__                    __func__

#define assert(x)                   if (!(x)) {fprintf(stderr, "[ERROR] Assertion Failed => Line %d in `%s()` in File `%s`"     \
                                                , __LINE__, __FUNC__, __FILE__); exit(1);}                                      \

#define println(fmt, ...)           printf(fmt"\n", ##__VA_ARGS__)
#define fprintln(stream, fmt, ...)  fprintf(stream, fmt"\n", ##__VA_ARGS__)

#define length(ss)                  __count_len_prototype(ss, '\0', 0)
#define count(ss, delim)            __count_len_prototype(ss, delim, 1)

#define in(ss, sbss)                counts(ss, sbss) > 0

#define startswith(a, b)            __compare_prototype(a, b, 0, length(b))
#define endswith(a, b)              __compare_prototype(reverse(a), reverse(b), 0, length(b))

#define __upper_prototype(a)        __upper_lower_prototype(a, 1)
#define __lower_prototype(a)        __upper_lower_prototype(a, 0)

#define isupper(a)                  __is_upper_lower_alpha_prototype(a, 1)
#define islower(a)                  __is_upper_lower_alpha_prototype(a, 0)
#define isalpha(a)                  __is_upper_lower_alpha_prototype(a, 2)

#define split(ss, dl)               _split(ss, dl).data
#define MAX_STRING_CAP              512

#define triml(ss)                   __triml_prototype(ss, 0, ' ')
#define triml_by_delim(ss, dl)      __triml_prototype(ss, 0, dl)

#define trimr(ss)                   __trimr_prototype(ss, length(ss)-1, ' ')
#define trimr_by_delim(ss, dl)      __trimr_prototype(ss, length(ss)-1, dl)

#define trim(ss)                    trimr(triml(ss))
#define trim_by_delim(ss, dl)       trimr_by_delim(triml_by_delim(ss, dl), dl)

typedef struct {

    char* data[MAX_STRING_CAP];
    int count;

} strlst;

int __count_len_prototype(char* ss, char dl, int ret_type){

    int i = 0, c = 0;
    while(ss[c]){

        if(ss[c]==dl){i++;}
        c++;
    }

    return (ret_type) ? i : c;
}

char* substr(char* ss, int head, int tail){

    char* sbs = (char*)malloc(length(ss)*sizeof(char));
    for (int i=head; i<=tail; i++){

        sbs[i-head] = ss[i];
    }

    sbs[tail-head+1] = '\0';
    return sbs;
}

int eq(char* a, char* b){

    if (length(a) != length(b)){

        return 0;
    }

    else {

        for (int i=0; i<length(a); i++){

            if (a[i] != b[i]){

                return 0;
            }
        }
    }

    return 1;
}

char* reverse(char* ss){

    char* res = (char*)malloc(length(ss)*sizeof(char));
    int cc = 0;

    for (int i=length(ss)-1; i>=0; i--){

        res[cc] = ss[i];
        cc++;
    }

    return res;
}

int* find(char* ss, char cc){

    int* res = (int*)malloc(count(ss, cc)*sizeof(int));
    int j = 0;

    for (int i=0; i<length(ss); i++){

        if(ss[i] == cc){

            res[j] = i;
            j++;
        }
    }

    return res;
}

int counts(char* ss, char* sbss){

    int lensbss = length(sbss);
    int counts = 0;

    if (lensbss > length(ss)){

        fprintln(stderr, "ERROR: substring length greater than actual string.");
    }

    else {

        for (int i=0; i<length(ss)-lensbss+1; i++){

            char* cmp = substr(ss, i, i +lensbss -1);

            if (eq(sbss, cmp)){counts++;}

            free(cmp);
        }
    }

    return counts;
}

int* finds(char* ss, char* sbss){

    int* res = (int*)malloc(counts(ss, sbss)*sizeof(int));
    int lensbss = length(sbss), cc = 0;

    for (int i=0; i<length(ss)-lensbss+1; i++){

        char* cmp = substr(ss, i, i +lensbss -1);

        if (eq(sbss, cmp)){res[cc] = i; cc++;}

        free(cmp);
    }

    return res;
}

char* __triml_prototype(char* ss, int i, char dl){

    if(ss[i] != dl && ss[i] != '\0'){return substr(ss, i, length(ss));}

    else {return __triml_prototype(ss, i+1, dl);}
}

char* __trimr_prototype(char* ss, int i, char dl){

    if(ss[i] != dl && ss[i] != '\0'){return substr(ss, 0, i);}

    else {return __trimr_prototype(ss, i-1, dl);}
}

void _split_prototype(char* ss, char dl, strlst* container){

    if (container->count == MAX_STRING_CAP || eq(ss, "")){return;}

    ss = trim_by_delim(ss, dl);

    int start = 0, end = 0;

    for (int i=0; i<length(ss); i++){

        if (ss[i] == dl){

            break;
        }

        end++;
    }

    char* first = trim_by_delim(substr(ss, start, end), dl);
    char* second = substr(ss, end +1, length(ss));

    container->data[container->count] = first;
    container->count++;

    _split_prototype(second, dl, container);
}

strlst _split(char* ss, char dl){

    strlst mem = {0};

    _split_prototype(ss, dl, &mem);

    return mem;
}

char* concat(char* a, char* b){

    int ll = length(a) + length(b);
    char* res = (char*)malloc(ll*sizeof(char));

    int i;
    for (i=0; i<length(a); i++){

        res[i] = a[i];
    }

    i = 0;
    for (int j=length(a); j<ll; j++){

        res[j] = b[i];
        i++;
    }

    return res;
}

char* join(char** list, int len, char* str){

    char* res = list[0];

    for (int i=1; i<len; i++){res = concat(res, concat(str, list[i]));}

    return res;
}

int __compare_prototype(char* a, char* b, int start, int end){

    if (length(b) == 0) {return 1;}

    for (int i=start; i<end; i++){

        if(a[i] != b[i]){

            return 0;
        }
    }

    return 1;
}

char __upper_lower_prototype(char cc, int type){

    char res;

    if (cc){

        if (type)   {if (cc >= 'a' && cc <= 'z'){res = cc - 32;} else {res = cc;}}  // UPPER

        else        {if (cc >= 'A' && cc <= 'Z'){res = cc + 32;} else {res = cc;}}  // LOWER
    }

    return res;
}

int __is_upper_lower_alpha_prototype(char* ss, int type){

    int upper = 0, lower = 0;
    for (int i=0; i<length(ss); i++){

        if      (ss[i] >= 'a' && ss[i] <= 'z') {lower++;}

        else if (ss[i] >= 'A' && ss[i] <= 'Z') {upper++;}
    }

    if      (type == 1) {return upper == length(ss);}
    else if (type == 0) {return lower == length(ss);}
    else                {return upper + lower == length(ss);}
}

char* swapcase(char* ss){

    char* res = (char*)malloc(length(ss)*sizeof(char));

    for (int i=0; i<length(ss); i++){

        if      (ss[i] >= 'a' && ss[i] <= 'z'){res[i] = ss[i] - 32;}

        else if (ss[i] >= 'A' && ss[i] <= 'Z'){res[i] = ss[i] + 32;}

        else    {res[i] = ss[i];}
    }

    return res;
}

char* upper(char* ss){

    char* res = (char*)malloc(length(ss)*sizeof(char));

    for (int i=0; i<length(ss); i++){

        res[i] = __upper_prototype(ss[i]);
    }

    return res;
}

char* lower(char* ss){

    char* res = (char*)malloc(length(ss)*sizeof(char));

    for (int i=0; i<length(ss); i++){

        res[i] = __lower_prototype(ss[i]);
    }

    return res;
}

int raise10(int pp){

    int res = 1;
    for (int i=0; i<pp; i++){

        res *= 10;
    }

    return res;
}

int c2i(char cc){

    return cc - '0';
}

int _s2i_positive(char* str){

    int res = 0;

    for (int i=0; i<length(str); i++){

        int util = c2i((char)str[length(str)-i-1]) * raise10(i);
        res += util;
    }

    return res;
}

int s2i(char* str){

    if (*str == '-'){

        return 0- _s2i_positive(str +1);
    }
    else {

        return _s2i_positive(str);
    }
}

char i2c(int n){

    assert(length(n) == 1);

    return '0' +n;
}

char* _i2s_positive(int n){

    assert(n > 0);

    int ll = length(n);
    char* res = malloc(n*sizeof(ll));
    int track = ll -1;

    while(ll--){

        res[track--] = i2c(n%10);
        n /= 10;
    }

    return res;
}

char* i2s(int n){

    if (n == 0) {return "0";}
    else if (n > 0) {return _i2s_positive(n);}
    else {return concat("-", _i2s_positive(-n));}
}

#endif // SPRYNG_H

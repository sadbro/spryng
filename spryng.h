
#include <stdio.h>
#include <stdlib.h>

#define println(fmt, ...)           printf(fmt"\n", ##__VA_ARGS__)
#define fprintln(stream, fmt, ...)  fprintf(stream, fmt"\n", ##__VA_ARGS__)

#define length(ss)                  __count_len_prototype(ss, '\0', 0)
#define count(ss, delim)            __count_len_prototype(ss, delim, 1)

#define startswith(a, b)            __compare_prototype(a, b, 0, length(b))
#define endswith(a, b)              __compare_prototype(reverse(a), reverse(b), 0, length(b))

#define __upper_prototype(a)        __upper_lower_prototype(a, 1)
#define __lower_prototype(a)        __upper_lower_prototype(a, 0)

#define isupper(a)                  __is_upper_lower_alpha_prototype(a, 1)
#define islower(a)                  __is_upper_lower_alpha_prototype(a, 0)
#define isalpha(a)                  __is_upper_lower_alpha_prototype(a, 2)

#define triml(ss)                   __triml_prototype(ss, 0)
#define trimr(ss)                   __trimr_prototype(ss, length(ss)-1)
#define trim(ss)                    trimr(triml(ss))

int __count_len_prototype(char* ss, char dl, int ret_type){

    int i = 0, c = 0;
    while(ss[c]){

        if(ss[c]==dl){i++;}
        c++;
    }

    return (ret_type) ? i : c;
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

void get_pos(char* ss, char cc, int* dst){

    int j = 0;
    for (int i=0; i<length(ss); i++){

        if(ss[i] == cc){

            dst[j] = i;
            j++;
        }
    }
}

char* substr(char* ss, int head, int tail){

    char* sbs = (char*)malloc(length(ss)*sizeof(char));
    for (int i=head; i<=tail; i++){

        sbs[i-head] = ss[i];
    }

    sbs[tail-head+1] = '\0';
    return sbs;
}

char* __triml_prototype(char* ss, int i){

    if(ss[i] != ' ' && ss[i] != '\0'){return substr(ss, i, length(ss));}

    else {return __triml_prototype(ss, i+1);}
}

char* __trimr_prototype(char* ss, int i){

    if(ss[i] != ' ' && ss[i] != '\0'){return substr(ss, 0, i);}

    else {return __trimr_prototype(ss, i-1);}
}

char** split(char* ss, char dl){

    int sbss = count(ss, dl) +1;
    int* poss = (int*)malloc((sbss +1)*sizeof(int));
    char** tokens = (char**)malloc(sbss*sizeof(char));

    poss[0] = 0;
    poss[sbss] = length(ss)-1;

    int j = 1;
    for (int i=0; i<length(ss); i++){

        if(ss[i] == dl){

            poss[j] = i;
            j++;
        }
    }

    int count = 0;
    for (int i=0; i<sbss; i++){

        if ( i > 0 && i < sbss -1){tokens[count] = substr(ss, poss[i] +1, poss[i+1] -1);}
        else if ( i == 0 ){tokens[count] = substr(ss, poss[i], poss[i+1] -1);}
        else {tokens[count] = substr(ss, poss[i] +1, poss[i+1]);}

        count++;
    }

    free(poss);
    return tokens;
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


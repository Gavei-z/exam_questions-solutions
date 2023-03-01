#include "parse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


int parse_query_string(char* url, struct query_parameter** out)
{
    int len = strlen(url);
    // Just output the url for convenience
    for (int i = 0; i < len; ++ i) {
        printf("%c", url[i]);
    }
    puts("");

    //! Here we begin
    char data[MAX_LEN][MAX_LEN];
    // push each valid string into data (each parameter and its value)
    int cnt = 0, cnt_name = 0, cnt_value = 0;
    bool start = false;
    for (int i = 0, idx = 0; i < len; ++ i) {
        // It means we should start to extract the parameters.
        // Only the first `?` is valid
        if (url[i] == '?' && !start) {
            start = true;
            continue;
        }

        // The `=` should be after the `?`
        // We obtain the value of the current parameter, or we start to extract a new parameter
        if (start) {
            if (url[i] == '=') {
                data[cnt][idx] = '\0';
                // The value maybe null, we should handle this.
                if (i == len - 1 || url[i + 1] == '&') {
                    idx = 0;
                    data[++cnt][idx] = '\0';
                    cnt_value ++;
                    i ++;
                }
                cnt ++;
                cnt_name ++;
                idx = 0;
                continue;
            } else if (url[i] == '&') {
                data[cnt][idx] = '\0';
                cnt++;
                cnt_value ++;
                idx = 0;
                continue;
            }
        }
        if (start) {
            // Special case: Notice that the url could include `\`,
            // and we should think it means there exists an escape character.
            // And, double `\`s equal single `\`, i.e. `\\` => `\`
            if (url[i] == '\\')
                data[cnt][idx++] = url[++i];
            else
                data[cnt][idx++] = url[i];
        }
        if (i == len - 1 && cnt_value == cnt_name - 1) {
            data[cnt][idx] = '\0';
            cnt++;
        }
    }

    // copy each string into out
    *out = (struct query_parameter*) malloc(sizeof(struct query_parameter) * cnt);
    for (int i = 0; i < cnt / 2; ++ i) {
        (*out)[i].name = data[2 * i];
        (*out)[i].value = data[2 * i + 1];
    }
    return cnt / 2;
}

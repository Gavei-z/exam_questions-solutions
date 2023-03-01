#include "parse.h"
#include "third_party/cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char*
file_get_contents(const char* path)
{
	FILE* file;
	long size;
	char* buffer;

	file = fopen(path, "r");
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = malloc(size + 1);
	fread(buffer, 1, size, file);
	fclose(file);
	return buffer;
}

static int
cJSON_GetObjectSize(const cJSON* object)
{
	int size = 0;

	for (const cJSON* child = object->child; child; child = child->next) {
		size++;
	}
	return size;
}

#define return_error() do {					\
	printf("ERROR: file %s line %d\n", __FILE__, __LINE__);	\
	cJSON_Delete(json);					\
	return 1;						\
} while (0)

int
main(void)
{
	char* json_string;
	cJSON* json;
	const cJSON* testcases;
	const cJSON* testcase;

	json_string = file_get_contents("testcases.json");
	if (!json_string) {
		return 1;
	}
	json = cJSON_Parse(json_string);
	free(json_string);
	if (!cJSON_IsObject(json)) {
		return_error();
	}

    //! We need set the CLion's `Working directory` to the current project path
	testcases = cJSON_GetObjectItem(json, "testcases");
	if (!cJSON_IsArray(testcases)) {
		return_error();
	}
	cJSON_ArrayForEach (testcase, testcases) {
		const cJSON* input;
		const cJSON* output;
		char* url;
		struct query_parameter* params;
		int count;

		input = cJSON_GetObjectItem(testcase, "input");
		output = cJSON_GetObjectItem(testcase, "output");
		url = strdup(input->valuestring);
		count = parse_query_string(url, &params);

		printf("\ncount %d\n", count);
		if (count != cJSON_GetObjectSize(output)) {
			return_error();
		}
		for (int i = 0; i < count; i++) {
			const cJSON* json_value;

			printf("[%s] = %s\n", params[i].name, params[i].value);
			json_value = cJSON_GetObjectItem(output, params[i].name);
			if (!cJSON_IsString(json_value)) {
				return_error();
			}
			if (strcmp(json_value->valuestring, params[i].value) != 0) {
				return_error();
			}
		}
		printf("OK\n\n");
	}
	cJSON_Delete(json);
	return 0;
}


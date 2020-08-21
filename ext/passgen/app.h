#ifndef include_app_h
#define include_app_h

#include <stdint.h>

typedef struct app_t app_t;

app_t* app_create();
void app_destroy(app_t* app);
void app_parse_arguments(app_t* app, int argc, const char* argv[]);
void app_set_use_char(app_t* app, const char* str);
void app_set_keyword(app_t* app, const char* str);
void app_set_encode_count(app_t* app, int count);
const char* app_encode(app_t* app);

#endif /* include_app_h */

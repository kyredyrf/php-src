/**
 *	アプリ
 */

#ifndef include_app_v2_h
#define include_app_v2_h

#include <stdint.h>

typedef struct app_v2_t app_v2_t;

app_v2_t* app_v2_create();
void app_v2_destroy(app_v2_t* app);
void app_v2_parse_arguments(app_v2_t* app, int argc, const char* argv[]);
void app_v2_set_use_number(app_v2_t* app, const char* str);
void app_v2_set_use_lower(app_v2_t* app, const char* str);
void app_v2_set_use_upper(app_v2_t* app, const char* str);
void app_v2_set_use_symbolic(app_v2_t* app, const char* str);
void app_v2_set_keyword(app_v2_t* app, const char* str);
void app_v2_set_encode_count(app_v2_t* app, int count);
const char* app_v2_encode(app_v2_t* app);

#endif /* include_app_v2_h */

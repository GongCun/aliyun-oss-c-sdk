#include "aos_log.h"
#include "aos_util.h"
#include "aos_string.h"
#include "aos_status.h"
#include "oss_auth.h"
#include "oss_util.h"
#include "oss_api.h"
#include "oss_config.h"
#include "oss_sample_util.h"

void append_object_from_file()
{
    aos_pool_t *p;
    aos_string_t bucket;
    aos_string_t object;
    int is_oss_domain = 1;
    aos_table_t *headers1;
    aos_table_t *headers2;
    aos_table_t *resp_headers;
    oss_request_options_t *options;
    char *object_name = "oss_append_get_delete_object_from_file";
    char *filename = "oss_append_get_delete_object_from_file";
    aos_status_t *s;
    aos_string_t file;
    int64_t position = 0;
    char *next_append_position;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_oss_domain);
    headers1 = aos_table_make(options->pool, 0);
    headers2 = aos_table_make(options->pool, 0);
    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, object_name);
    aos_str_set(&file, filename);

    s = oss_head_object(options, &bucket, &object, headers1, &resp_headers);
    if(NULL != s && 2 == s->code / 100) {
        next_append_position = (char*)(apr_table_get(resp_headers, 
                        "x-oss-next-append-position"));
        position = atoi(next_append_position);
    }

    s = oss_append_object_from_file(options, &bucket, &object, 
                                    position, &file, headers2, &resp_headers);

    if (NULL != s && 2 == s->code / 100) {
        printf("append object from file succeeded\n");
    } else {
        printf("append object from file failed\n");
    }    

    aos_pool_destroy(p);
}

void get_object_to_local_file()
{
    aos_pool_t *p;
    aos_string_t bucket;
    char *object_name = "oss_append_get_delete_object_from_file";
    char *download_filename = "oss_test_get_append_object_to_file";
    aos_string_t object;
    int is_oss_domain = 1;
    oss_request_options_t *options;
    aos_table_t *headers;
    aos_table_t *resp_headers;
    aos_status_t *s;
    aos_string_t file;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_oss_domain);
    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, object_name);
    headers = aos_table_make(p, 0);
    aos_str_set(&file, download_filename);

    s = oss_get_object_to_file(options, &bucket, &object, headers, 
                               &file, &resp_headers);

    if (NULL != s && 2 == s->code / 100) {
        printf("get object to local file succeeded\n");
    } else {
        printf("get object to local file failed\n");
    }

    aos_pool_destroy(p);
}

void delete_object()
{
    aos_pool_t *p;
    aos_string_t bucket;
    char *object_name = "oss_append_get_delete_object_from_file";
    aos_string_t object;
    int is_oss_domain = 1;
    oss_request_options_t *options;
    aos_table_t *resp_headers;
    aos_status_t *s;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_oss_domain);
    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, object_name);

    s = oss_delete_object(options, &bucket, &object, &resp_headers);
    
    if (NULL != s && 204 == s->code) {
        printf("delete append object succeeded\n");
    } else {
        printf("delete append object failed\n");
    }

    aos_pool_destroy(p);
}

int main(int argc, char *argv[])
{
    //aos_http_io_initialize first 
    if (aos_http_io_initialize("oss_sample", 0) != AOSE_OK) {
        exit(1);
    }

    append_object_from_file();
    get_object_to_local_file();
    delete_object();

    //aos_http_io_deinitialize last
    aos_http_io_deinitialize();

    return 0;
}

/*
 * This file is part of the struct2json Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: demo for this library.
 * Created on: 2015-10-14
 */

#include "inc/cJSON.h"
#include "inc/s2j.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
    char name[16];
} Hometown;

typedef struct {
    uint8_t id;
    double weight;
    uint8_t score[8];
    char name[10];
    Hometown hometown[2];
} Student;

/**
 * Student JSON object to structure object
 *
 * @param json_obj JSON object
 *
 * @return structure object
 */
static void *json_to_struct(cJSON *json_obj) {
    /* create Student structure object */
    s2j_create_struct_obj(struct_student, Student);

    /* deserialize data to Student structure object. */
    s2j_struct_get_basic_element(struct_student, json_obj, int, id);
    s2j_struct_get_array_element(struct_student, json_obj, int, score);
    s2j_struct_get_basic_element(struct_student, json_obj, string, name);
    s2j_struct_get_basic_element(struct_student, json_obj, double, weight);

    /* deserialize data to Student.Hometown structure object. */
    s2j_struct_get_struct_element(struct_hometown, struct_student, json_hometown, json_obj, Hometown, hometown);
    printf(cJSON_Print(json_hometown));
    s2j_struct_get_basic_element(struct_hometown, json_hometown, string, name);

    /* return Student structure object pointer */
    return struct_student;
}

/**
 * Student structure object to JSON object
 *
 * @param struct_obj structure object
 *
 * @param JSON object
 */
static cJSON *struct_to_json(void *struct_obj) {
    Student *struct_student = (Student *) struct_obj;

    /* create Student JSON object */
    s2j_create_json_obj(json_student);

    /* serialize data to Student JSON object. */
    s2j_json_set_basic_element(json_student, struct_student, int, id);
    s2j_json_set_basic_element(json_student, struct_student, double, weight);
    s2j_json_set_array_element(json_student, struct_student, int, score, 8);
    s2j_json_set_basic_element(json_student, struct_student, string, name);

    /* serialize data to Student.Hometown JSON object. */
    s2j_json_set_struct_element(json_hometown, json_student, struct_hometown, struct_student, Hometown, hometown, 2);
    for (int i = 0; i < 2; ++i) {
        s2j_json_set_child_struct_element(json_hometown, &struct_student->hometown[i], string, name);
    }
    /* return Student JSON object pointer */
    return json_student;
}


typedef struct {
    char canCpuID[25];
    int devID;
    int devType;
    int lmVal;
    int rev2;
    int rev3;
    int bOnOff;
    int param1;
    int param2;
} RUN_DEV_ITEM_JSON;

typedef struct {
    char sceneName[25];
    int devCnt;
    int eventId;
    RUN_DEV_ITEM_JSON run_dev_item[4];
} SceneRows;

typedef struct {
    //JsonBase base;
    SceneRows scene_rows;
    int event_cnt;
} Scene;

static cJSON *struct_scene_to_json(void *struct_obj) {
    Scene *scene = (Scene *) struct_obj;

    s2j_create_json_obj(root);
    //root = create_base_json(&scene->base, root);
    s2j_json_set_struct_element(json_scene, root, struct_scene_rows, scene, SceneRows, scene_rows, 0);

    s2j_json_set_basic_element(json_scene, struct_scene_rows, string, sceneName);
    s2j_json_set_basic_element(json_scene, struct_scene_rows, int, devCnt);
    s2j_json_set_basic_element(json_scene, struct_scene_rows, int, eventId);

    s2j_json_set_struct_element(json_scene_rows, json_scene, struct_run_dev_item, &(scene->scene_rows),
                                RUN_DEV_ITEM_JSON, run_dev_item, 4);
    for (int i = 0; i < scene->scene_rows.devCnt; ++i) {
        cJSON *json_item = cJSON_CreateObject();
        s2j_json_set_child_struct_element(json_item, &scene->scene_rows.run_dev_item[i], string, canCpuID);
        s2j_json_set_child_struct_element(json_item, &scene->scene_rows.run_dev_item[i], int, devID);
        s2j_json_set_child_struct_element(json_item, &scene->scene_rows.run_dev_item[i], int, devType);
        s2j_json_set_child_struct_element(json_item, &scene->scene_rows.run_dev_item[i], int, lmVal);
        s2j_json_array_set_object_element(json_scene_rows, json_item);
    }

    return root;
}

static void *json_scene_to_struct(cJSON *json_obj){
    /* create Student structure object */
    s2j_create_struct_obj(struct_scene, Scene);

    s2j_struct_get_struct_element(struct_scene_rows, struct_scene, json_scene_rows, json_obj, SceneRows, scene_rows);
    s2j_struct_get_basic_element(struct_scene_rows, json_scene_rows, string, sceneName);
    s2j_struct_get_basic_element(struct_scene_rows, json_scene_rows, int, devCnt);
    s2j_struct_get_basic_element(struct_scene_rows, json_scene_rows, int, eventId);
    s2j_struct_get_struct_element(struct_run_dev_item, struct_scene_rows, json_run_dev_item, json_scene_rows,
                                  RUN_DEV_ITEM_JSON, run_dev_item);
    cJSON *list = json_run_dev_item->child;
    int index = 0;
    while (list != NULL) {
        s2j_struct_get_basic_element(&struct_run_dev_item[index], list, string, canCpuID);
        s2j_struct_get_basic_element(&struct_run_dev_item[index], list, int, devID);
        s2j_struct_get_basic_element(&struct_run_dev_item[index], list, int, devType);
        s2j_struct_get_basic_element(&struct_run_dev_item[index], list, int, lmVal);
        list = list->next;
        index++;
    }

    /* return Student structure object pointer */
    return struct_scene;
}

int main(void) {
    static Student orignal_student_obj = {
            .id = 24,
            .weight = 71.2,
            .score = {1, 2, 3, 4, 5, 6, 7, 8},
            .name = "armink",
            .hometown[0].name = "China",
            .hometown[1].name = "xian",
    };

    Scene scene = {
            .scene_rows = {
                    .sceneName = "情景模式1",
                    .devCnt = 3,
                    .eventId = 4,
                    .run_dev_item[0].canCpuID = "14324234234242",
                    .run_dev_item[0].devID = 1,
                    .run_dev_item[0].devType = 3,
                    .run_dev_item[1].canCpuID = "111111111111111",
                    .run_dev_item[1].devID = 2,
                    .run_dev_item[1].devType = 3,
                    .run_dev_item[2].canCpuID = "222222222222222",
                    .run_dev_item[2].devID = 3,
                    .run_dev_item[2].devType = 3,
            }
    };
    cJSON *json = struct_scene_to_json(&scene);
    printf(cJSON_Print(json));

    Scene *scene1 = json_scene_to_struct(json);
    if (memcmp(&scene, scene1, sizeof(Scene))) {
        printf("Converted failed!\n");
    } else {
        printf("Converted OK!\n");
    }
    s2j_delete_json_obj(json);
    s2j_delete_struct_obj(scene1);


    /* serialize Student structure object */
    cJSON *json_student = struct_to_json(&orignal_student_obj);
    printf(cJSON_Print(json_student));

    /* deserialize Student structure object */
    Student *converted_student_obj = json_to_struct(json_student);

    /* compare Student structure object */
    if(memcmp(&orignal_student_obj, converted_student_obj, sizeof(Student))) {
        printf("Converted failed!\n");
    } else {
        printf("Converted OK!\n");
    }

    s2j_delete_json_obj(json_student);
    s2j_delete_struct_obj(converted_student_obj);

    return 0;
}

# struct_to_json
C 结构体与 JSON 快速互转库

基于https://github.com/armink/struct2json的基础上，添加了以下结构体的处理方法
```
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
```

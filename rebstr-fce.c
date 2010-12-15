char* RebserToMultiByte(REBSER* series) {
    char *mbStr=0;
    REBCHR* str;
    REBINT result = RL_GET_STRING(series, 0 , (void**)&str);

    if (result > 0){
        //unicode string
        int len = WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0,  NULL, NULL);
        mbStr = malloc(len);
        //be ware to free the resulted mbStr when not needed!
        int result = WideCharToMultiByte(CP_UTF8, 0, str, wcslen(str), mbStr, len, 0, 0);
        if (result == 0) {
            int err = GetLastError();
            RL->print((REBYTE*)"ERROR: WideCharToMultiByte -> %d\n", err);
            exit(-1); //how to throw ERROR on REBOL side?
        }
        mbStr[len] = 0;
    } else if (result < 0) {
        //bytes string (ascii or latin-1)
        mbStr = strdup((char *)str);
    }
    return mbStr;
}

REBSER* StringToRebser(char* str) {
    unsigned int c;
    REBSER *ser = RL_MAKE_STRING(strlen(str), TRUE);
    for (c = 0; c < strlen(str); ++c)
        RL_SET_CHAR(ser, c, str[c]);
    return ser;
}
/*
char* RebserToStr(REBSER* ser) {
    unsigned int c;
    REBSER *ser = RL_MAKE_STRING(strlen(str), TRUE);
    for (c = 0; c < strlen(str); ++c)
        RL_SET_CHAR(ser, c, str[c]);
    return ser;
}*/

REBSER* MultiByteToRebser(char* mbStr) {
	//RL->print("MultiByteToWideChar -> %s\n", mbStr);
    int len = MultiByteToWideChar( CP_UTF8, 0, mbStr, -1, NULL, 0);
    //the len is length of the string + null terminator
    REBSER *ser = RL_MAKE_STRING(len,TRUE);
    if(len > 1) { //not an empty string...
        wchar_t *wcStr = malloc(len * sizeof(wchar_t));
        int result = MultiByteToWideChar(CP_UTF8, 0, mbStr, strlen(mbStr), wcStr, len);
        if (result == 0) {
            unsigned int err = GetLastError();
            RL->print((REBYTE*)"ERROR: MultiByteToWideChar -> %d\n", err);
            exit(-1); //how to throw ERROR on REBOL side?
        }

        REBUNI *dst;

        //hack! - will set the tail to len
        REBINT *s = (REBINT*)ser;
        s[1] = len-1;

        RL_GET_STRING(ser,0,(void**)&dst);
        wcscpy(dst, wcStr);

        free(wcStr);
        wcStr = NULL;
    }
    return ser;
}

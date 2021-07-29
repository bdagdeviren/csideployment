#include <log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/ip/libip.h"

#define BUFFER_SIZE 1000

void replaceAll(char *str, const char *oldWord, const char *newWord)
{
    char *pos, temp[BUFFER_SIZE]; int index = 0; int owlen;

    owlen = strlen(oldWord);

    if (!strcmp(oldWord, newWord)) {
        return;
    }

    while ((pos = strstr(str, oldWord)) != NULL)
    {
        strcpy(temp, str);
        index = pos - str;
        str[index] = '\0';
        strcat(str, newWord);
        strcat(str, temp + index + owlen);
    }
}

int replace_all_string_in_file()
{
    FILE * fPtr; FILE * fTemp; char path[100];
    char buffer[BUFFER_SIZE]; char oldWord[100], newWord[100];

    fPtr  = fopen(path, "r");
    fTemp = fopen("replace.tmp", "w");

    if (fPtr == NULL || fTemp == NULL)
    {
        log_info("Unable to open file!");
        log_info("Please check whether file exists and you have read/write privilege!");
        exit(EXIT_SUCCESS);
    }

    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL)
    {
        replaceAll(buffer, oldWord, newWord);
        fputs(buffer, fTemp);
    }

    fclose(fPtr); fclose(fTemp); remove(path);
    rename("replace.tmp", path);

    log_info("\nSuccessfully replaced all occurrences of '%s' with '%s'.", oldWord, newWord);
}


void run_client(){
    log_info("Running Client!!!");
    replace_all_string_in_file();
}

void run_server(){
    log_info("Running Server!!!");

    struct get_pod_ips_return getPodIpsReturn;
    getPodIpsReturn = get_pod_ips("csideployment-server","kube-system");
    if(getPodIpsReturn.r0 != 0){
        log_error(getPodIpsReturn.r2);
    }else{
        log_info(getPodIpsReturn.r1);
    }
}

int main(int argc, char* argv[]) {
    char *ret;
    const char* s;

    log_info("\n"
             "         ##########################################################################\n"
             "         #            _     _            _                                  _     #\n"
             "         #    ___ ___(_) __| | ___ _ __ | | ___  _   _ _ __ ___   ___ _ __ | |_   #\n"
             "         #   / __/ __| |/ _` |/ _ \\ '_ \\| |/ _ \\| | | | '_ ` _ \\ / _ \\ '_ \\| __|  #\n"
             "         #  | (__\\__ \\ | (_| |  __/ |_) | | (_) | |_| | | | | | |  __/ | | | |_   #\n"
             "         #   \\___|___/_|\\__,_|\\___| .__/|_|\\___/ \\__, |_| |_| |_|\\___|_| |_|\\__|  #\n"
             "         #                        |_|            |___/                            #\n"
             "         #                                                                        #\n"
             "         ##########################################################################");
    s = getenv("VMNAME");
    ret = strstr(s, "md");
    if (ret){
        run_server();
    }else{
        run_client();
    }
}

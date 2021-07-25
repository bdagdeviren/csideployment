#include <log.h>
#include "libs/ip/libip.h"

int main(int argc, char* argv[]) {
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
    struct get_pod_ips_return getPodIpsReturn;
    getPodIpsReturn = get_pod_ips("csideployment","");
    if(getPodIpsReturn.r0 != 0){
        log_error(getPodIpsReturn.r2);
    }else{
        log_info(getPodIpsReturn.r1);
    }
}

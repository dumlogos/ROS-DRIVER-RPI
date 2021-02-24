#include "UNISTD_CLOSE_OVERRIDE.h"

int UNISTD_OVERRIDE::myCloseCan(int __fd){
    return close(__fd);
}

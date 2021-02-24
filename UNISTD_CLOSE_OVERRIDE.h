#ifndef UNISTD_CLOSE_OVERRIDE_H
#define UNISTD_CLOSE_OVERRIDE_H

#include "unistd.h"

namespace UNISTD_OVERRIDE{
    int myCloseCan(int __fd);
}
#endif // UNISTD_CLOSE_OVERRIDE_H

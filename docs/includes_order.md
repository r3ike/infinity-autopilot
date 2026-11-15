# Include order
To prevent confict beetween __unused macro the order of inclusion in any hpp file must be s follows:

1) std lib includes
2) the header that use STL c++
3) the zephyr header


```cpp
#pragma once

#include <cstdint>

// 1. PRIMA gli header che usano la STL C++ (<memory>, <vector>, ecc.)
#include "SRIMB.hpp"
#include "SRIMBSub.hpp"
#include "Loggable.hpp"

#include "data_types/DataTypes.h"
#include "srimb_topics/TopicTypes.h"

// 2. POI gli header dello scheduler e di Zephyr (che caricano <zephyr/kernel.h>)
#include "WorkItem.hpp"
#include "WorkQueue.hpp"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
```

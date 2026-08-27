// logging_instances.cpp — l'unico file che sa quante istanze esistono di cosa
#include "log_instance_pool.hpp"
#include "imu_registration.hpp"   // dove hai NUM_BMI088_INSTANCES, ecc.

LOG_INSTANCE_POOL_DEFINE(imu_bmi088,     NUM_BMI088_INSTANCES)
LOG_INSTANCE_POOL_DEFINE(imu_icm42688p,  NUM_ICM42688P_INSTANCES)

// ImuPreprocessor non è legato 1:1 a un compatible devicetree (è puro software,
// creato "una per ogni IMU registrata"), quindi qui NON puoi usare IMU_INSTANCES
// così com'è (è (NUM_BMI088... + NUM_ICM...), un'espressione, non un letterale).
// Soluzione: un Kconfig con un tetto massimo, esattamente come avevi già notato
// tu stesso nel primo codice ("da mettere come kconfig").
LOG_INSTANCE_POOL_DEFINE(imu_preprocessor, CONFIG_MAX_IMU_INSTANCES)

LOG_INSTANCE_POOL_DEFINE(gps,   GPS_INSTANCES)
LOG_INSTANCE_POOL_DEFINE(baro,  BARO_INSTANCES)
LOG_INSTANCE_POOL_DEFINE(mag,   MAG_INSTANCES)
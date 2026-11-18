#pragma once

namespace srimb {class SRIMBTopicManager};
class WorkQueue;

struct HalContext
{
	srimb::SRIMBTopicManager& topicManager;
	WorkQueue& fast_sensors_wq;
	WorkQueue& medium_sensors_wq;
	WorkQueue& slow_sensors_wq;
};

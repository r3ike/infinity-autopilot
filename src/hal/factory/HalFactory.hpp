// hal_factory.hpp
#pragma once
#include <memory>

class HAL;
namespace srimb { class SRIMBTopicManager; }
class WorkQueue;

std::unique_ptr<HAL> make_hal_for_target(srimb::SRIMBTopicManager& tm, WorkQueue& wq);

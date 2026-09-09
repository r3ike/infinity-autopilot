#pragma once

class WorkItemBase
{
public:
    ~WorkItemBase() = default;

    virtual int submit() = 0;
    virtual int submitTo(WorkQueue &wq) = 0;
};


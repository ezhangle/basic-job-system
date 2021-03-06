// jobsystemtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <assert.h>

// jobsystem settings
#define JOBSYSTEM_ENABLE_PROFILING                  ///< Enables worker/job profiling, and an ascii profile dump on shutdown.
#define JOBSYSTEM_ASSERT(...) assert(__VA_ARGS__)   ///< Directs internal system asserts to app-specific assert mechanism.

// jobsystem include
#include <jobsystem.h>

int main()
{
    jobsystem::JobManagerDescriptor jobManagerDesc;
    jobManagerDesc.m_workers.emplace_back("Worker1");
    jobManagerDesc.m_workers.emplace_back("Worker2");
    jobManagerDesc.m_workers.emplace_back("Worker3");
    jobManagerDesc.m_workers.emplace_back("Worker4");
    jobManagerDesc.m_workers.emplace_back("Worker5");
    jobManagerDesc.m_workers.emplace_back("Worker6");
    jobManagerDesc.m_workers.emplace_back("Worker7");
    jobManagerDesc.m_workers.emplace_back("Worker8");

    jobsystem::JobManager jobManager;
    if (!jobManager.Create(jobManagerDesc))
    {
        return 1;
    }

    auto something = []()
    {
        Sleep(10);
    };

    auto somethingAfterThat = []()
    {
        Sleep(10);
    };

    auto parallelThing1 = []()
    {
        Sleep(10);
    };

    auto parallelThing2 = []()
    {
        Sleep(10);
    };

    auto parallelThing3 = []()
    {
        Sleep(10);
    };

    auto finalThing = []()
    {
        Sleep(10);
    };

    jobsystem::JobChainBuilder<10000> builder(jobManager);

    // Run a couple jobs in succession.
    builder
        .Do(something, 'a')
        .Then()
        .Do(somethingAfterThat, 'b')
        .Then()
        .Together();

    // Run 1k jobs in parallel.
    for (size_t i = 0; i < 1000; ++i)
    {
        const char c = 'a' + (char)(i % ('z' - 'a'));
        builder.Do(parallelThing1, c);
    }

    // Run a final "join" job.
    builder
        .Close()
        .Then()
        .Do(finalThing, 'Z');

    // Run the jobs and assist until complete.
    builder
        .Go()
        .AssistAndWaitForAll();

    return builder.Failed() ? 1 : 0;
}


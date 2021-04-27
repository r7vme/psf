#pragma once
#include "spin_lock.hpp"
#include "statistics_tracker.hpp"
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <sys/resource.h>
#include <thread>

namespace
{
using namespace std::chrono_literals;
}

namespace psf
{

class StatisticsPrinter
{

  public:
	void run()
	{
		std::cout << "latency_min,latency_mean,latency_max,cpu_perc,max_rss,context_switches,minor_page_faults,major_"
					 "page_faults"
				  << std::endl;

		struct rusage lastRu;
		getrusage(RUSAGE_SELF, &lastRu);
		while (true)
		{
			std::this_thread::sleep_for(1s);
			std::ostringstream ss;

			// latency stats
			{
				std::lock_guard<performance_test::SpinLock> guard(lock);
				ss << stats.min() << ",";
				ss << stats.mean() << ",";
				ss << stats.max() << ",";
				stats = performance_test::StatisticsTracker();
			}

			// rusage
			struct rusage ru;
			getrusage(RUSAGE_SELF, &ru);
			double totalCpuUserTime = static_cast<double>(ru.ru_utime.tv_sec) + static_cast<double>(ru.ru_utime.tv_usec) / 1000000.0;
			double totalCpuSysTime = static_cast<double>(ru.ru_stime.tv_sec) + static_cast<double>(ru.ru_stime.tv_usec) / 1000000.0;
			double lastTotalCpuUserTime = static_cast<double>(lastRu.ru_utime.tv_sec) + static_cast<double>(lastRu.ru_utime.tv_usec) / 1000000.0;
			double lastTotalCpuSysTime = static_cast<double>(lastRu.ru_stime.tv_sec) + static_cast<double>(lastRu.ru_stime.tv_usec) / 1000000.0;
			double cpuPercentageFromLastRun =
				(totalCpuUserTime + totalCpuSysTime) - (lastTotalCpuUserTime + lastTotalCpuSysTime) / 1.0;
			ss << cpuPercentageFromLastRun << ",";
			ss << ru.ru_maxrss << ",";
			ss << ru.ru_nivcsw - lastRu.ru_nivcsw << ",";
			ss << ru.ru_minflt - lastRu.ru_minflt << ",";
			ss << ru.ru_majflt - lastRu.ru_majflt;

			std::cout << ss.str() << std::endl;

			lastRu = ru;
		}
	}

	void addSample(double const sample)
	{
		std::lock_guard<performance_test::SpinLock> guard(lock);
		stats.add_sample(sample);
	}

  private:
	performance_test::StatisticsTracker stats;
	performance_test::SpinLock lock;
};

} // namespace psf

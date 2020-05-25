#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
    float utilization = 0.;
    long idle = LinuxParser::IdleJiffies();
    long total = LinuxParser::Jiffies();

    if (first_time_) {
        first_time_ = false;
    } else {
        float total_d = total - prev_total_;
        float idle_d = idle - prev_idle_;
        utilization = (total_d - idle_d) / total_d;
    }
    
    prev_total_ = total;
    prev_idle_ = idle;
    return utilization;
}
/*
 * adaprive_threshold_processor.h
 *
 *  Created on: 23 sty 2016
 *      Author: gregStudent
 */

#ifndef ADAPRIVE_THRESHOLD_PROCESSOR_H_
#define ADAPRIVE_THRESHOLD_PROCESSOR_H_

#include "../egl_include.h"
#include "complex_filter.h"
#include "adaptive_threshold.h"

namespace raspi_gpgpu
{
class AdaptiveThresholdProcessor : public ComplexFilter {
public:
	AdaptiveThresholdProcessor() {
        raspi_gpgpu::AdaptiveThreshold *at1 = new raspi_gpgpu::AdaptiveThreshold(1);
        raspi_gpgpu::AdaptiveThreshold *at2 = new raspi_gpgpu::AdaptiveThreshold(2);

        _processorPipeline.push_back(at1);
        _processorPipeline.push_back(at2);

        ComplexInit();
    }

    virtual const char *GetProcName() { return "AdapriveThresholdProcessor"; }
};
}


#endif /* ADAPRIVE_THRESHOLD_PROCESSOR_H_ */

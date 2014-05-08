/**
 * Copyright 2014 Axel Huebl, Felix Schmitt, Heiko Burau, Rene Widera
 *
 * This file is part of PIConGPU. 
 * 
 * PIConGPU is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version. 
 * 
 * PIConGPU is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with PIConGPU.  
 * If not, see <http://www.gnu.org/licenses/>. 
 */

#pragma once

#include <string>
#include <sstream>
#include <splash/splash.h>

#include "types.h"
#include "simulation_defines.hpp"
#include "particles/frame_types.hpp"
#include "dataManagement/DataConnector.hpp"
#include "dimensions/DataSpace.hpp"
#include "dimensions/GridLayout.hpp"
#include "fields/FieldE.hpp"
#include "fields/FieldB.hpp"
#include "simulationControl/MovingWindow.hpp"

namespace picongpu
{

namespace hdf5
{

/**
 * Helper class for HDF5Writer plugin to load fields from parallel libSplash files.
 */
class RestartFieldLoader
{
public:
    template<class Data>
    static void loadField(Data& field, std::string objectName, uint32_t restartStep,
            ParallelDomainCollector &domainCollector)
    {
        log<picLog::INPUT_OUTPUT > ("Begin loading field '%1%'") % objectName;
        DataSpace<simDim> field_guard = field.getGridLayout().getGuard();

        VirtualWindow window = MovingWindow::getInstance().getVirtualWindow(restartStep);

        field.getHostBuffer().setValue(float3_X(0.));

        const std::string name_lookup[] = {"x", "y", "z"};

        /* globalSlideOffset due to gpu slides between origin at time step 0
         * and origin at current time step
         * ATTENTION: splash offset are globalSlideOffset + picongpu offsets
         */
        DataSpace<simDim> globalSlideOffset;
        globalSlideOffset.y() = window.slides * window.localDomainSize.y();

        DataSpace<simDim> globalOffset(Environment<simDim>::get().SubGrid().getSimulationBox().getGlobalOffset());

        Dimensions domain_offset(0, 0, 0);
        for (uint32_t d = 0; d < simDim; ++d)
            domain_offset[d] = globalOffset[d] + globalSlideOffset[d];

        if (Environment<simDim>::get().GridController().getPosition().y() == 0)
            domain_offset[1] += window.globalDimensions.offset.y();

        DataSpace<simDim> localDomainSize = window.localDimensions.size;
        Dimensions domain_size;
        for (uint32_t d = 0; d < simDim; ++d)
            domain_size[d] = localDomainSize[d];

        PMACC_AUTO(destBox, field.getHostBuffer().getDataBox());
        for (uint32_t i = 0; i < simDim; ++i)
        {
            // Read the subdomain which belongs to our mpi position.
            // The total grid size must match the grid size of the stored data.
            log<picLog::INPUT_OUTPUT > ("Read from domain: offset=%1% size=%2%") % domain_offset.toString() % domain_size.toString();
            DomainCollector::DomDataClass data_class;
            DataContainer *field_container =
                domainCollector.readDomain(restartStep,
                                           (std::string("fields/") + objectName +
                                            std::string("/") + name_lookup[i]).c_str(),
                                           Domain(domain_offset, domain_size),
                                           &data_class);

            int elementCount = localDomainSize.productOfComponents();

            for (int linearId = 0; linearId < elementCount; ++linearId)
            {
                /* calculate index inside the moving window domain which is located on the local grid*/
                DataSpace<simDim> destIdx = DataSpaceOperations<simDim>::map(localDomainSize, linearId);
                /* jump over guard and local sliding window offset*/
                destIdx += field_guard + window.localDimensions.offset;

                destBox(destIdx)[i] = ((float_X*) (field_container->getIndex(0)->getData()))[linearId];
            }

            delete field_container;
        }

        field.hostToDevice();

        __getTransactionEvent().waitForFinished();

        log<picLog::INPUT_OUTPUT > ("Read from domain: offset=%1% size=%2%") % domain_offset.toString() % domain_size.toString();
        log<picLog::INPUT_OUTPUT > ("Finished loading field '%1%'") % objectName;
    }

    template<class Data>
    static void cloneField(Data& fieldDest, Data& fieldSrc, std::string objectName)
    {
        log<picLog::INPUT_OUTPUT > ("Begin cloning field '%1%'") % objectName;
        DataSpace<simDim> field_grid = fieldDest.getGridLayout().getDataSpace();

        size_t elements = field_grid.productOfComponents();
        float3_X *ptrDest = fieldDest.getHostBuffer().getDataBox().getPointer();
        float3_X *ptrSrc = fieldSrc.getHostBuffer().getDataBox().getPointer();

        for (size_t k = 0; k < elements; ++k)
        {
            ptrDest[k] = ptrSrc[k];
        }

        fieldDest.hostToDevice();

        __getTransactionEvent().waitForFinished();

        log<picLog::INPUT_OUTPUT > ("Finished cloning field '%1%'") % objectName;
    }
};

/**
 * Hepler class for HDF5Writer (forEach operator) to load a field from HDF5
 * 
 * @tparam FieldType field class to load
 */
template< typename FieldType >
struct LoadFields
{
public:

    HDINLINE void operator()(RefWrapper<ThreadParams*> params)
    {
#ifndef __CUDA_ARCH__
        DataConnector &dc = Environment<>::get().DataConnector();
        ThreadParams *tp = params.get();

        /* load field without copying data to host */
        FieldType* field = &(dc.getData<FieldType > (FieldType::getName(), true));

        /* load from HDF5 */
        RestartFieldLoader::loadField(
                field->getGridBuffer(),
                FieldType::getName(),
                tp->currentStep,
                *(tp->dataCollector));

        dc.releaseData(FieldType::getName());
#endif
    }

};

using namespace PMacc;
using namespace splash;

} //namespace hdf5
} //namespace picongpu

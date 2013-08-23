/*
 *	main.cpp
 *	!CHOAS
 *	Created by Bisegni Claudio.
 *
 *    	Copyright 2013 INFN, National Institute of Nuclear Physics
 *
 *    	Licensed under the Apache License, Version 2.0 (the "License");
 *    	you may not use this file except in compliance with the License.
 *    	You may obtain a copy of the License at
 *
 *    	http://www.apache.org/licenses/LICENSE-2.0
 *
 *    	Unless required by applicable law or agreed to in writing, software
 *    	distributed under the License is distributed on an "AS IS" BASIS,
 *    	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    	See the License for the specific language governing permissions and
 *    	limitations under the License.
 */

#include "S7ControlUnit.h"
#include "SiemensS7TcpDriver.h"

#include <chaos/common/chaos_constants.h>
#include <chaos/cu_toolkit/ChaosCUToolkit.h>
#include <chaos/common/exception/CException.h>

#include <iostream>
#include <string>

/*! \page page_example_cue ChaosCUToolkit Example
 *  \section page_example_cue_sec An basic usage for the ChaosCUToolkit package
 *
 *  \subsection page_example_cue_sec_sub1 Toolkit usage
 *  ChaosCUToolkit has private constructor so it can be used only using singleton pattern,
 *  the only way to get unique isntance is; ChaosCUToolkit::getInstance(). So the first call of
 *  getInstance method will provide the CUToolkit and Common layer initial setup.
 *  \snippet example/ControlUnitTest/ControlUnitExample.cpp Custom Option
 */
using namespace std;
using namespace chaos;
using namespace chaos::cu;

namespace common_plugin = chaos::common::plugin;
namespace common_utility = chaos::common::utility;
namespace cu_driver_manager = chaos::cu::driver_manager;
namespace siemens_driver_ns = chaos::cu::driver_manager::driver::siemens_s7;

#define OPT_ADDRESS_PORT	"address"
#define OPT_DEVICE_ID		"device_id"
#define OPT_PARAM_STR		"def_param"

int main (int argc, char* argv[] )
{
    string tmp_device_id;
	string tmp_definition_param;
	string tmp_address;
    try {
		//! [Custom Option]
		ChaosCUToolkit::getInstance()->getGlobalConfigurationInstance()->addOption(OPT_ADDRESS_PORT, po::value<string>(&tmp_address), "Specify address of the plc (address:port)");
		ChaosCUToolkit::getInstance()->getGlobalConfigurationInstance()->addOption(OPT_DEVICE_ID, po::value<string>(&tmp_device_id), "Specify the device id of the siemen s7 plc");
		ChaosCUToolkit::getInstance()->getGlobalConfigurationInstance()->addOption(OPT_PARAM_STR, po::value<string>(&tmp_definition_param), "String representing the configuration of the control unit with the sintax (name:type:db_num:start:offset:max_len|...) type respect the DataType int value");
		//! [Custom Option]
		
		//! [CUTOOLKIT Init]
		ChaosCUToolkit::getInstance()->init(argc, argv);
		if(!ChaosCUToolkit::getInstance()->getGlobalConfigurationInstance()->hasOption(OPT_ADDRESS_PORT)) throw CException(1, "address is madatory", __FUNCTION__);
		if(!ChaosCUToolkit::getInstance()->getGlobalConfigurationInstance()->hasOption(OPT_DEVICE_ID)) throw CException(2, "device id option is mandatory", __FUNCTION__);
		if(!ChaosCUToolkit::getInstance()->getGlobalConfigurationInstance()->hasOption(OPT_PARAM_STR)) throw CException(3, "the paramtere string is madatory", __FUNCTION__);
		
		//check if the definition stirng is wel formed
		if(!S7ControlUnit::checkDefinitionparam(tmp_definition_param)) throw CException(3, "The definition string is not wel formed", __FUNCTION__);
		
		//! [CUTOOLKIT Init]
		
		//! [Driver Registration]
		MATERIALIZE_INSTANCE_AND_INSPECTOR_WITH_NS(siemens_driver_ns, SiemensS7TcpDriver)
		cu_driver_manager::DriverManager::getInstance()->registerDriver(SiemensS7TcpDriverInstancer, SiemensS7TcpDriverInspector);
		//! [Driver Registration]

		//! [Adding the CustomControlUnit]
		ChaosCUToolkit::getInstance()->addControlUnit(new S7ControlUnit(tmp_device_id, tmp_definition_param, tmp_address));
		//! [Adding the CustomControlUnit]
		
		//! [Starting the Framework]
		ChaosCUToolkit::getInstance()->start();
		//! [Starting the Framework]
    } catch (CException& e) {
        cerr<<"Exception:"<<endl;
        std::cerr<< "domain	:"<<e.errorDomain << std::endl;
        std::cerr<< "cause	:"<<e.errorMessage << std::endl;
    } catch (program_options::error &e){
        cerr << "Unable to parse command line: " << e.what() << endl;
    } catch (...){
        cerr << "unexpected exception caught.. " << endl;
    }
	
    return 0;
}

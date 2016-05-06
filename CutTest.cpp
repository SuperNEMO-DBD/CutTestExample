// - Implementation of CutTest
// Ourselves
#include "CutTest.h"
// Standard Library
// Third Party
// - A
// This Project
// Macro which automatically implements the interface needed
// to enable the module to be loaded at runtime
// The first argument is the typename
// The second is the string key used to access the module in pipeline
// scripts. This must be globally unique.

DPP_MODULE_REGISTRATION_IMPLEMENT(CutTest,"CutTest");

// Construct
CutTest::CutTest() : dpp::base_module()
{
  path_cut_config_="./cut_manger.conf";
}

// Destruct
CutTest::~CutTest() {
  // MUST reset module at destruction
  this->reset();
}

// Initialize
void CutTest::initialize(const datatools::properties& myConfig,
			  datatools::service_manager& flServices,
			  dpp::module_handle_dict_type& moduleDict) {

  // Throw logic exception if we've already initialized this instance
  DT_THROW_IF(this->is_initialized(),
	      std::logic_error,
	      "CutTest already initialized");
  // Extract the cuts config filename from the supplied config, if
  // the key exists. datatools::properties throws an exception if
  // the key isn't in the config, so catch this if thrown and don't do
  // anything
  try {
    myConfig.fetch("cuts.config_file",this->path_cut_config_);
  } catch (std::logic_error& e) {
  }
  std::cout << "In INIT" << std::endl;

  if (flServices.has("geometry"))
  {
    const geomtools::geometry_service& GS = flServices.get<geomtools::geometry_service> ("geometry");

    //    std::cout << "Initialize geo manager " << std::endl;
    geometry_manager_ = &GS.get_geom_manager();
  }

  const std::string filename = path_cut_config_;
  datatools::properties cut_config;
  datatools::properties::read_config(filename, cut_config);
  const datatools::properties cut_init = cut_config;

  // Initialization from the set of parameters extracted from the configuration file :
  my_cut_manager_.initialize(cut_init);

  //  std::cout << "found cut manger: " << std::endl;
  my_cut_manager_.tree_dump();

  this->_set_initialized(true);
}

// Process
dpp::base_module::process_status CutTest::process(datatools::things& workItem) {
  std::cout << "START CutTest::process " << std::endl;
  int status = 1;
  cuts::i_cut * my_cut = 0;

  const cuts::cut_handle_dict_type cut_dict = my_cut_manager_.get_cuts();

  for (cuts::cut_handle_dict_type::const_iterator it = cut_dict.begin();
       it != cut_dict.end();
       ++it) {
    if (status < 1) break; // get out if cut broke
    const std::string & the_cut_name = it->first;
    my_cut = &my_cut_manager_.grab(the_cut_name);

    // apply cut
    my_cut->set_user_data(workItem);
    status = my_cut->process();
    std::cout << "- Applying cut "<< the_cut_name << " gave status " << ((status > 0) ? "passed" : "failed") << std::endl;

  }

  // MUST return a status, see ref dpp::processing_status_flags_type
  return dpp::base_module::PROCESS_OK;
}

// Reset
void CutTest::reset() {
  path_cut_config_ = "./cut_manager.conf";
  my_cut_manager_.reset();
  this->_set_initialized(false);
}

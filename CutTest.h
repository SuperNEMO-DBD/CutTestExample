//! \file CutTest.h
//! \brief User processing module for flreconstruct
//! \details Process a things object and convert data to ROOT file output.
#ifndef SERVTEST_HH
#define SERVTEST_HH

// Standard Library
#include <vector>
#include <iostream>
#include <string>

// Third Party

// - Bayeux
#include "bayeux/dpp/base_module.h"
#include "bayeux/datatools/service_manager.h"
#include "bayeux/geomtools/geometry_service.h"
#include "bayeux/geomtools/manager.h"
#include "bayeux/cuts/cut_manager.h"

// - Falaise
#include "falaise/snemo/datamodels/particle_track_data.h"
//#include "falaise/snemo/cuts/particle_track_data_cut.h"


// This Project
class CutTest : public dpp::base_module {
 public:
  //! Construct module
  CutTest();
  //! Destructor
  virtual ~CutTest();
  //! Configure the module
  virtual void initialize(const datatools::properties& myConfig,
			  datatools::service_manager& flServices,
			  dpp::module_handle_dict_type& what);
  //! Process supplied data record
  virtual dpp::base_module::process_status process(datatools::things& workItem);

  //! Reset the module
  virtual void reset();

 private:
  // configurable data member
  std::string path_cut_config_;


  // geometry service
  const geomtools::manager* geometry_manager_; //!< The geometry manager
  cuts::cut_manager my_cut_manager_;
  //  snemo::cut::particle_track_data_cut ptd_cut_; //!< The PTD cut 


  // Macro which automatically creates the interface needed
  // to enable the module to be loaded at runtime
  DPP_MODULE_REGISTRATION_INTERFACE(CutTest);
};
#endif // SERVTEST_HH

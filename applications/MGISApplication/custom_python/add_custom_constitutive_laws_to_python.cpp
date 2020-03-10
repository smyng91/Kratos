//   __  __  _____ _____  _____                     _ _           _   _
//  |  \/  |/ ____|_   _|/ ____|  /\               | (_)         | | (_)
//  | \  / | |  __  | | | (___   /  \   _ __  _ __ | |_  ___ __ _| |_ _  ___  _ __
//  | |\/| | | |_ | | |  \___ \ / /\ \ | '_ \| '_ \| | |/ __/ _` | __| |/ _ \| '_  |
//  | |  | | |__| |_| |_ ____) / ____ \| |_) | |_) | | | (_| (_| | |_| | (_) | | | |
//  |_|  |_|\_____|_____|_____/_/    \_\ .__/| .__/|_|_|\___\__,_|\__|_|\___/|_| |_|
//                                     | |   | |
//                                     |_|   |_|
//
//  License: BSD License
//   license: MGISApplication/license.txt
//
//  Main authors:  Thomas Helfer
//                 Vicente Mataix Ferrandiz
//

// System includes

// External includes

// Project includes
#include "custom_constitutive/mgis_constitutive_law_factory.h"
#include "custom_python/add_custom_constitutive_laws_to_python.h"

namespace Kratos {

  namespace Python {

    void AddCustomConstitutiveLawsToPython(pybind11::module& m) {
      namespace py = pybind11;
      py::class_<MGISConstitutiveLawFactory, typename MGISConstitutiveLawFactory::Pointer,
                 ConstitutiveLaw>(m, "MGISConstitutiveLawFactory")
          .def(py::init<>())
          .def("Create", &MGISConstitutiveLawFactory::Create);
    } // end of AddCustomConstitutiveLawsToPython

  }  // namespace Python.
}  // namespace Kratos.
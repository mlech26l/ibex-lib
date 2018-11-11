//============================================================================
//                                  I B E X                                   
// File        : ibex_LoupFinderDefault.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_LOUPFINDERDEFAULT_H__
#define __SIP_IBEX_LOUPFINDERDEFAULT_H__


#include "ibex_SIPSystem.h"

#include "ibex_Cell.h"
#include "ibex_IntervalVector.h"
#include "ibex_LoupFinderSIP.h"

#include <utility>

namespace ibex {
class LoupFinderSIPDefault: public LoupFinderSIP {
public:
	LoupFinderSIPDefault(const SIPSystem& system);
	virtual ~LoupFinderSIPDefault();
	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);
	std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop);
};

} // end namespace ibex

#endif // __SIP_IBEX_LOUPFINDERDEFAULT_H__
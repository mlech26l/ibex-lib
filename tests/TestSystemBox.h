/* ============================================================================
 * I B E X - System Box Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : June 30th, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_SYSTEM_BOX_H__
#define __TEST_SYSTEM_BOX_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "utils.h"

namespace ibex {

class TestSystemBox : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestSystemBox);
	CPPUNIT_TEST(goal_eval01);
	CPPUNIT_TEST(goal_gradient01);
	CPPUNIT_TEST(ctrs_eval);
	CPPUNIT_TEST(ctrs_jacobian);
	CPPUNIT_TEST(active_ctrs);
	CPPUNIT_TEST(is_inner);
	CPPUNIT_TEST(active_ctrs_eval);
	CPPUNIT_TEST(active_ctrs_jacobian);
	CPPUNIT_TEST_SUITE_END();

	void goal_eval01();
	void goal_gradient01();
	void ctrs_eval();
	void ctrs_jacobian();
	void active_ctrs();
	void is_inner();
	void active_ctrs_eval();
	void active_ctrs_jacobian();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSystemBox);


} // end namespace

#endif // __TEST_SYSTEM_BOX_H__

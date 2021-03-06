/* This file is part of SMART+.
 
 SMART+ is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 SMART+ is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with SMART+.  If not, see <http://www.gnu.org/licenses/>.
 
 */

///@file Tcontimech.cpp
///@brief Test for continuum mechanics tensors in Voigt notation
///@version 1.0

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "contimech"
#include <boost/test/unit_test.hpp>

#include <armadillo>
#include <smartplus/parameter.hpp>
#include <smartplus/Libraries/Continuum_Mechanics/contimech.hpp>

using namespace std;
using namespace arma;
using namespace smart;

BOOST_AUTO_TEST_CASE( tr_dev_Mises_eta )
{
    
    vec test = zeros(6);
    test(0) = 4.;
    test(1) = 2.;
    test(2) = 6.;
    test(3) = 8.;
    test(4) = 3.;
    test(5) = 7.;
    
    //Test of tr function
    double tr1 = tr(zeros(6));
    double tr2 = tr(test);
    BOOST_CHECK( fabs(tr1 - 0.) < iota );
    BOOST_CHECK( fabs(tr2 - 12.) < iota );

    //Test of dev function
    vec dev1 = dev(zeros(6));
    vec dev2 = dev(test);
    double trdev2 = tr(dev2);
	vec testdev = zeros(6);
    testdev(0) = 0.;
    testdev(1) = -2.;
    testdev(2) = 2.;
    testdev(3) = 8.;
    testdev(4) = 3.;
    testdev(5) = 7.;
    BOOST_CHECK( fabs(trdev2 - 0.) < iota );
    BOOST_CHECK( norm(dev2 - testdev,2) < iota );
    
    //Test of Mises_stress function
    double dstress = 3.*sqrt(42.);
    BOOST_CHECK( fabs(Mises_stress(test) - dstress) < iota );

    //Test of eta_stress function
    dev1 = eta_stress(zeros(6));
    dev2 = eta_stress(test);
	vec testeta = zeros(6);
    testeta(0) = 0.;
    testeta(1) = -2;
    testeta(2) = 2.;
    testeta(3) = 8.*2;
    testeta(4) = 3.*2;
    testeta(5) = 7.*2;
    BOOST_CHECK( norm(dev1,2) < iota );
    BOOST_CHECK( norm(dev2 - (3./2.)*testeta/dstress,2) < iota );
    
    //Test of Mises_strain function
    double dstrain = sqrt(46.);
    BOOST_CHECK( fabs(Mises_strain(test) - dstrain) < iota );

    //Test of eta_strain function
    dev1 = eta_strain(zeros(6));
    dev2 = eta_strain(test);
    BOOST_CHECK( norm(dev1,2) < iota );
    BOOST_CHECK( norm(dev2 - (2./3.)*testdev/dstrain,2) < iota );
    
}

BOOST_AUTO_TEST_CASE( v2t_func )
{
	
    vec test = zeros(6);
    test(0) = 4.;
    test(1) = 2.;
    test(2) = 6.;
    test(3) = 8.;
    test(4) = 3.;
    test(5) = 7.;
    
	mat strain = zeros(3,3);
	mat stress = zeros(3,3);
	mat teststrain = zeros(3,3);
	mat teststress = zeros(3,3);
	
	teststrain(0,0) = 4.;
	teststrain(0,1) = 4.;
	teststrain(0,2) = 1.5;
	teststrain(1,0) = 4.;
	teststrain(1,1) = 2.;
	teststrain(1,2) = 3.5;
	teststrain(2,0) = 1.5;
	teststrain(2,1) = 3.5;
	teststrain(2,2) = 6.;							

	teststress(0,0) = 4.;
	teststress(0,1) = 8.;
	teststress(0,2) = 3.;
	teststress(1,0) = 8.;
	teststress(1,1) = 2.;
	teststress(1,2) = 7.;
	teststress(2,0) = 3.;
	teststress(2,1) = 7.;
	teststress(2,2) = 6.;
	
	//Test of v2t_strain function
	strain = v2t_strain(zeros(6));
    BOOST_CHECK( norm(strain,2) < iota );
	strain = v2t_strain(test);
    BOOST_CHECK( norm(strain - teststrain,2) < iota );

    //Test of t2v_strain function
	vec dev1 = t2v_strain(zeros(3,3));
    vec dev2 = t2v_strain(teststrain);
    BOOST_CHECK( norm(dev1,2) < iota );
    BOOST_CHECK( norm(dev2 - test,2) < iota );

	//Test of v2t_stress function
	stress = v2t_stress(zeros(6));
    BOOST_CHECK( norm(stress,2) < iota );
	stress = v2t_stress(test);
    BOOST_CHECK( norm(stress - teststress,2) < iota );
    
	//Test of t2v_stress function
	dev1 = t2v_stress(zeros(3,3));
	dev2 = t2v_stress(teststress);
    BOOST_CHECK( norm(dev1,2) < iota );
    BOOST_CHECK( norm(dev2 - test,2) < iota );
    
}
    
BOOST_AUTO_TEST_CASE( J2_J3 )
{

    vec test = zeros(6);
    test(0) = 4.;
    test(1) = 2.;
    test(2) = 6.;
    test(3) = 8.;
    test(4) = 3.;
    test(5) = 7.;
    
	//Test of J2_stress function
    double J2_stress1 = J2_stress(zeros(6));
    double J2_stress2 = J2_stress(test);
    BOOST_CHECK( fabs(J2_stress1) < iota );
    BOOST_CHECK( fabs(J2_stress2 - 126.) < iota );
	
	//Test of J2_strain function
	double J2_strain1 = J2_strain(zeros(6));
	double J2_strain2 = J2_strain(test);
    BOOST_CHECK( fabs(J2_strain1) < iota );
    BOOST_CHECK( fabs(J2_strain2 - 34.5) < iota );

	//Test of J3_stress function
	double J3_stress1 = J3_stress(zeros(6));
	double J3_stress2 = J3_stress(test);
    BOOST_CHECK( fabs(J3_stress1) < iota );
    BOOST_CHECK( fabs(J3_stress2 - 226.) < iota );

	//Test of J3_stress function
	double J3_strain1 = J3_strain(zeros(6));
	double J3_strain2 = J3_strain(test);
    BOOST_CHECK( fabs(J3_strain1) < iota );
    BOOST_CHECK( fabs(J3_strain2 - 14.5) < iota );

}

BOOST_AUTO_TEST_CASE( P_ijkl )
{

	vec a = zeros(6);
	for (int i=0; i<3; i++)
		a(i) = 1.;
	
	vec b = ones(6);
    
	mat Ireal = eye(6,6);
	for (int i=3; i<6; i++)
		Ireal(i,i) = 0.5;
    
    vec test = zeros(6);
	test(0) = 4.;
	test(1) = 2.;
	test(2) = 6.;
	test(3) = 4.;
	test(4) = 1.5;
	test(5) = 3.5;
    
	//Test of p_ijkl function
	
	mat result = zeros(6,6);
	
	result(0,0) = 16.;
	result(0,1) = 16.;
	result(0,2) = 2.25;
	result(0,3) = 16.;
	result(0,4) = 6.;
	result(0,5) = 6.;
    
	result(1,0) = 16.;
	result(1,1) = 4.;
	result(1,2) = 12.25;
	result(1,3) = 8.;
	result(1,4) = 14.;
	result(1,5) = 7.;
    
	result(2,0) = 2.25;
	result(2,1) = 12.25;
	result(2,2) = 36;
	result(2,3) = 5.25;
	result(2,4) = 9.;
	result(2,5) = 21.;
	
	result(3,0) = 16.;
	result(3,1) = 8.;
	result(3,2) = 5.25;
	result(3,3) = 12.;
	result(3,4) = 10.;
	result(3,5) = 8.5;
	
	result(4,0) = 6.;
	result(4,1) = 14.;
	result(4,2) = 9.;
	result(4,3) = 10.;
	result(4,4) = 13.125;
	result(4,5) = 14.625;
    
	result(5,0) = 6.;
	result(5,1) = 7.;
	result(5,2) = 21.;
	result(5,3) = 8.5;
	result(5,4) = 14.625;
	result(5,5) = 12.125;
    
	//Test of p_ijkl function
	mat pikjlc = p_ikjl(test);
	mat pikjla = p_ikjl(a);
	mat pikjlb = p_ikjl(b);

    BOOST_CHECK( norm(pikjla - Ireal,2) < iota );
    BOOST_CHECK( norm(pikjlb - ones(6,6),2) < iota );
    BOOST_CHECK( norm(pikjlc - result,2) < iota );
    
}

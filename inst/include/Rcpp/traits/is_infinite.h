//
// is_infinite.h:  is infinite
//                                                                      
// Copyright (C) 2013 Dirk Eddelbuettel and Romain Francois
//
// This file is part of Rcpp11.
//
// Rcpp11 is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp11 is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp11.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp__traits_is_infinite_h
#define Rcpp__traits_is_infinite_h

namespace Rcpp{
namespace traits{
	
    // default for all but REALSXP and CPLXSXP
	template <int RTYPE> 
	bool is_infinite( typename storage_type<RTYPE>::type){
	    return false ;    
	}
	
	template <> 
	inline bool is_infinite<REALSXP>( double x ){
		return !( ISNAN(x) || R_FINITE(x) ) ;
	}
	
	template <> 
	inline bool is_infinite<CPLXSXP>( Rcomplex x ){
		return is_infinite<REALSXP>(x.r) || is_infinite<REALSXP>(x.i) ;
	}
	
}
}

#endif

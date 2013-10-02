// Copyright (C) 2013    Romain Francois
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

#ifndef Rcpp_api_meat_Function_h
#define Rcpp_api_meat_Function_h

namespace Rcpp{ 

    template<typename... Args> 
    SEXP Function::operator()( const Args&... args) const {
        RCPP_DEBUG( "Function::operator(...) nargs = %d", sizeof...(args)  )
        SEXP call = PROTECT( Rcpp_lcons( get__() , pairlist(args...) ) ) ;
        SEXP res  = PROTECT( internal::try_catch( call ) ) ;
        RCPP_DEBUG( "res = <%p>", res  )
        UNPROTECT(2) ;
        return res ;
    }
        
	
} // namespace Rcpp

#endif

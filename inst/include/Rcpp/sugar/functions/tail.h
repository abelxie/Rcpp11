//
// tail.h:  tail
//
// Copyright (C) 2010 - 2011 Dirk Eddelbuettel and Romain Francois
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

#ifndef Rcpp__sugar__tail_h
#define Rcpp__sugar__tail_h

namespace Rcpp{
namespace sugar{
	
template <int RTYPE, bool NA, typename T>
class Tail : public Rcpp::VectorBase< RTYPE ,NA, Tail<RTYPE,NA,T> > {
public:
	typedef typename Rcpp::VectorBase<RTYPE,NA,T> VEC_TYPE ;
	typedef typename Rcpp::traits::storage_type<RTYPE>::type STORAGE ;
	
	Tail( const VEC_TYPE& object_, int n_ ) : object(object_), start(0), n(n_) {
		if( n > 0 ){
			start = object.size() - n ;
		} else {
			start = -n ;
			n = object.size() - start ;
		}
	}
	
	inline STORAGE operator[]( int i ) const {
		return object[ start + i ] ;
	}
	inline int size() const { return n; }
	         
private:
	const VEC_TYPE& object ;
	int start, n ;
} ;
	
} // sugar

template <int RTYPE,bool NA, typename T>
inline sugar::Tail<RTYPE,NA,T> tail( 
	const VectorBase<RTYPE,NA,T>& t, 
	int n 
	){
	return sugar::Tail<RTYPE,NA,T>( t, n ) ;
}

} // Rcpp
#endif


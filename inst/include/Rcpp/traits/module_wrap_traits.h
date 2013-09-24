// module_wrap_traits.h:  traits to help module wrap
//
// Copyright (C) 2012 Dirk Eddelbuettel and Romain Francois
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

#ifndef Rcpp__traits__module_wrap_traits__h
#define Rcpp__traits__module_wrap_traits__h

namespace Rcpp{
namespace traits{

struct normal_wrap_tag{} ;	
struct void_wrap_tag{} ;
struct pointer_wrap_tag{} ;
	
template <typename T> struct module_wrap_traits     { typedef normal_wrap_tag category; } ;
template <> struct module_wrap_traits<void>         { typedef void_wrap_tag category; } ;
template <typename T> struct module_wrap_traits<T*> { typedef pointer_wrap_tag category; } ;
     
} // namespace traits
} // namespace Rcpp
#endif

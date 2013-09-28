// wrap_impl.h:  wrap implementations
//
// Copyright (C) 2010 - 2013  Dirk Eddelbuettel and Romain Francois
// Copyright (C) 2013  Rice University
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

#ifndef Rcpp_wrap_wrap_impl_h
#define Rcpp_wrap_wrap_impl_h
 
namespace Rcpp{
namespace internal{
	
// {{{ unknown

template <typename T>
inline SEXP wrap_dispatch_unknown_iterable__logical( const T& object, std::true_type){
	RCPP_DEBUG( "wrap_dispatch_unknown_iterable__logical<%s>(., true  )", DEMANGLE(T) )
	size_t size = object.size() ;
	SEXP x = PROTECT( Rf_allocVector( LGLSXP, size ) );
	std::copy( object.begin(), object.end(), LOGICAL(x) ) ; 
	UNPROTECT(1) ;
	return x ;
}

template <typename T>
inline SEXP wrap_range_sugar_expression( const T& object, std::false_type){
	RCPP_DEBUG( "wrap_range_sugar_expression<%s>(., false  )", DEMANGLE(T) )
	return range_wrap( object.begin(), object.end() ) ;
}
template <typename T>
inline SEXP wrap_range_sugar_expression( const T& object, std::true_type) ; 

template <typename T>
inline SEXP wrap_dispatch_unknown_iterable__logical( const T& object, std::false_type){
	RCPP_DEBUG( "wrap_dispatch_unknown_iterable__logical<%s>(., false  )", DEMANGLE(T) )
	return wrap_range_sugar_expression( object, typename Rcpp::traits::is_sugar_expression<T>::type() ) ;
}


template <typename T>
inline SEXP wrap_dispatch_unknown_iterable__matrix_interface( const T& object, std::false_type ){
	RCPP_DEBUG( "wrap_dispatch_unknown_iterable__matrix_interface<%s>(., false  )", DEMANGLE(T) )
	return wrap_dispatch_unknown_iterable__logical( object, 
			typename ::Rcpp::traits::expands_to_logical<T>::type() );
}

template <typename T>
inline SEXP wrap_dispatch_matrix_logical( const T& object, std::true_type ){
	int nr = object.nrow(), nc = object.ncol() ;
	SEXP res = PROTECT( Rf_allocVector( LGLSXP, nr * nc ) ) ;
	int k=0 ;
	int* p = LOGICAL(res) ;
	for( int j=0; j<nc; j++)
		for( int i=0; i<nr; i++, k++)
			p[k] = object(i,j) ;
	SEXP dim = PROTECT( Rf_allocVector( INTSXP, 2) ) ;
	INTEGER(dim)[0] = nr ;
	INTEGER(dim)[1] = nc ;
	Rf_setAttrib( res, R_DimSymbol , dim ) ;
	UNPROTECT(2) ;
	return res ;
}

template <typename T, typename STORAGE>
inline SEXP wrap_dispatch_matrix_primitive( const T& object ){
	const int RTYPE = ::Rcpp::traits::r_sexptype_traits<STORAGE>::rtype ;
	int nr = object.nrow(), nc = object.ncol() ;
	SEXP res = PROTECT( Rf_allocVector( RTYPE, nr*nc ) );
	
	int k=0 ;
	STORAGE* p = r_vector_start< RTYPE>(res) ;
	for( int j=0; j<nc; j++)
		for( int i=0; i<nr; i++, k++)
			p[k] = object(i,j) ;
	SEXP dim = PROTECT( Rf_allocVector( INTSXP, 2) ) ;
	INTEGER(dim)[0] = nr ;
	INTEGER(dim)[1] = nc ;
	Rf_setAttrib( res, R_DimSymbol , dim ) ;
	UNPROTECT(2) ;
	return res ;
}

template <typename T>
inline SEXP wrap_dispatch_matrix_not_logical( const T& object, ::Rcpp::traits::r_type_primitive_tag ){
	return wrap_dispatch_matrix_primitive<T, typename T::stored_type>( object ) ;
}

template <typename T>
inline SEXP wrap_dispatch_matrix_not_logical( const T& object, ::Rcpp::traits::r_type_string_tag ){
	int nr = object.nrow(), nc = object.ncol() ;
	SEXP res = PROTECT( Rf_allocVector( STRSXP, nr*nc ) ) ;
	
	int k=0 ;
	for( int j=0; j<nc; j++)
		for( int i=0; i<nr; i++, k++)
			SET_STRING_ELT( res, k, make_charsexp(object(i,j)) ) ;
	SEXP dim = PROTECT( Rf_allocVector( INTSXP, 2) ) ;
	INTEGER(dim)[0] = nr ;
	INTEGER(dim)[1] = nc ;
	Rf_setAttrib( res, R_DimSymbol , dim ) ;
	UNPROTECT(2) ;
	return res ;
}

template <typename T>
inline SEXP wrap_dispatch_matrix_not_logical( const T& object, ::Rcpp::traits::r_type_generic_tag ){
	int nr = object.nrow(), nc = object.ncol() ;
	SEXP res = PROTECT( Rf_allocVector( VECSXP, nr*nc ) );
	
	int k=0 ;
	for( int j=0; j<nc; j++)
		for( int i=0; i<nr; i++, k++)
			SET_VECTOR_ELT( res, k, ::Rcpp::wrap( object(i,j) ) ) ;
	SEXP dim = PROTECT( Rf_allocVector( INTSXP, 2) ) ;
	INTEGER(dim)[0] = nr ;
	INTEGER(dim)[1] = nc ;
	Rf_setAttrib( res, R_DimSymbol , dim ) ;
	UNPROTECT(2) ;
	return res ;
}

template <typename T>
inline SEXP wrap_dispatch_matrix_logical( const T& object, std::false_type ){
	return wrap_dispatch_matrix_not_logical<T>( object, typename ::Rcpp::traits::r_type_traits<typename T::stored_type>::r_category() ) ;
}

template <typename T>
inline SEXP wrap_dispatch_unknown_iterable__matrix_interface( const T& object, std::true_type ){
	RCPP_DEBUG( "wrap_dispatch_unknown_iterable__matrix_interface<%s>(., true  )", DEMANGLE(T) )
	return wrap_dispatch_matrix_logical( object, typename ::Rcpp::traits::expands_to_logical<T>::type() ) ;
}

template <typename T, typename elem_type>
inline SEXP wrap_dispatch_importer__impl__prim( const T& object, std::false_type ){
	int size = object.size() ;
	const int RTYPE = ::Rcpp::traits::r_sexptype_traits<elem_type>::rtype ;
	SEXP x = PROTECT( Rf_allocVector( RTYPE, size ) );
	typedef typename ::Rcpp::traits::storage_type<RTYPE>::type CTYPE ;
	CTYPE* start = r_vector_start<RTYPE>(x) ;
	for( int i=0; i<size; i++){
		start[i] = object.get(i) ;
	}
	UNPROTECT(1) ;
	return x ;

}

template <typename T, typename elem_type>
inline SEXP wrap_dispatch_importer__impl__prim( const T& object, std::true_type ){
	int size = object.size() ;
	const int RTYPE = ::Rcpp::traits::r_sexptype_traits<elem_type>::rtype ;
	SEXP x = PROTECT( Rf_allocVector( RTYPE, size ) );
	typedef typename ::Rcpp::traits::storage_type<RTYPE>::type CTYPE ;
	CTYPE* start = r_vector_start<RTYPE>(x) ;
	for( int i=0; i<size; i++){
		start[i] = caster<elem_type,CTYPE>( object.get(i) );
	}
	UNPROTECT(1) ;
	return x ;
}

template <typename T, typename elem_type>
inline SEXP wrap_dispatch_importer__impl( const T& object, ::Rcpp::traits::r_type_primitive_tag ){
	return wrap_dispatch_importer__impl__prim<T,elem_type>( object, 
		typename ::Rcpp::traits::r_sexptype_needscast<elem_type>() ) ;
}

template <typename T, typename elem_type>
inline SEXP wrap_dispatch_importer__impl( const T& object, ::Rcpp::traits::r_type_string_tag ){
	int size = object.size() ;
	SEXP x = PROTECT( Rf_allocVector( STRSXP, size ) );
	for( int i=0; i<size; i++){
		SET_STRING_ELT( x, i, make_charsexp(object.get(i)) ) ;
	}
	UNPROTECT(1) ;
	return x ;
}

template <typename T, typename elem_type>
inline SEXP wrap_dispatch_importer__impl( const T& object, ::Rcpp::traits::r_type_generic_tag ){
	int size = object.size() ;
	SEXP x = PROTECT( Rf_allocVector( VECSXP, size ) );
	for( int i=0; i<size; i++){
		SET_VECTOR_ELT( x, i, object.wrap(i) ) ;
	}
	UNPROTECT(1) ;
	return x ;
}


template <typename T, typename elem_type>
inline SEXP wrap_dispatch_importer( const T& object ){
	return wrap_dispatch_importer__impl<T,elem_type>( object, 
		typename ::Rcpp::traits::r_type_traits<elem_type>::r_category() 
		 ) ;
}
// }}}

} // internal

template <typename T>
inline SEXP wrap( std::initializer_list<T> init ){ return internal::range_wrap( init.begin(), init.end() ) ; } 

template <> inline SEXP wrap<Rcpp::String>( const Rcpp::String& object) ;

// special case - FIXME : this is not template specializations of wrap<>
inline SEXP wrap(const char* const v ){ 
    if (v == NULL)
	return R_NilValue;
    else
	return Rf_mkString(v) ;
}

/**
 * Range based version of wrap
 */
template <typename InputIterator>
inline SEXP wrap(InputIterator first, InputIterator last){ 
	return internal::range_wrap( first, last ) ;
}


} // Rcpp

#endif

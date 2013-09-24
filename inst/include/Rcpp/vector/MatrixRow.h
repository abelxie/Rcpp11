//
// MatrixRow.h:  matrices row
//
// Copyright (C) 2010 - 2013 Dirk Eddelbuettel and Romain Francois
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

#ifndef Rcpp__vector__MatrixRow_h
#define Rcpp__vector__MatrixRow_h
   
template <int RTYPE>
class MatrixRow : public VectorBase< RTYPE, true, MatrixRow<RTYPE> > {
public:
    typedef Matrix<RTYPE> MATRIX ;
    typedef typename MATRIX::Proxy Proxy ;
    typedef typename MATRIX::Proxy reference ;
    typedef typename MATRIX::const_Proxy const_reference ;
    typedef typename MATRIX::value_type value_type ;
    
    class iterator {
    public:
        typedef typename traits::r_vector_iterator<RTYPE>::type vector_iterator ;
        
        typedef int difference_type ;
        typedef typename traits::r_vector_proxy<RTYPE>::type value_type ;
        typedef typename traits::r_vector_proxy<RTYPE>::type reference ;
        typedef typename std::iterator_traits<vector_iterator>::pointer pointer ;
        
        typedef std::random_access_iterator_tag iterator_category ;
            
        iterator( const iterator& other) : row(other.row), index(other.index){}
        iterator( MatrixRow& row_, int index_ ) : row(row_), index(index_){}
                
        iterator& operator++(){ 
            index++;
            return *this ;
        }
        iterator operator++(int) { 
            iterator orig(*this); 
            index++ ;
            return orig ;
        }
        
        iterator& operator--(){ 
            index-- ;
            return *this ;
        } 
        iterator operator--(int){ 
            iterator orig(*this); 
            index-- ;
            return orig ;
        }
                                    
        iterator operator+(difference_type n) const { return iterator( row, index + n ) ; }
        iterator operator-(difference_type n) const { return iterator( row, index - n ) ; }
        difference_type operator-(const iterator& other) const { return index - other.index ; }
                
        iterator& operator+=(difference_type n) { index += n ; return *this ;} 
        iterator& operator-=(difference_type n) { index -= n ; return *this ;} 
                
        reference operator*() {
            return row[index] ;
        }
        pointer operator->(){
            return &row[index] ;
        }
                
        bool operator==( const iterator& other) { return index == other.index ; } 
        bool operator!=( const iterator& other) { return index != other.index ; }
        bool operator<( const iterator& other ) { return index < other.index ;}
        bool operator>( const iterator& other ) { return index > other.index ;}
        bool operator<=( const iterator& other ) { return index <= other.index ; }
        bool operator>=( const iterator& other ) { return index >= other.index ; }
                
        inline reference operator[]( int i) const {
            return row[ index + i ] ;
        }
                
        difference_type operator-(const iterator& other) {
            return index - other.index ;
        }
                
    private:
        MatrixRow& row ;
        int index ;
    } ;
        
    MatrixRow( MATRIX& object, int i ) : 
        parent(object), 
        start(parent.begin() + i), 
        parent_nrow(parent.nrow()), 
        row(i)
    {
        if( i < 0 || i >= parent.nrow() ) throw index_out_of_bounds() ;
    }
        
    MatrixRow( const MatrixRow& other ) : 
        parent(other.parent), 
        start(other.start), 
        parent_nrow(other.parent_nrow), 
        row(other.row)
    {} ;
        
    template <int RT, bool NA, typename T>
    MatrixRow& operator=( const Rcpp::VectorBase<RT,NA,T>& rhs ){
        int n = size() ;
        const T& ref = rhs.get_ref() ;
        RCPP_LOOP_UNROLL_LHSFUN(start,get_parent_index,ref)
            return *this ;
    }
        
    MatrixRow& operator=( const MatrixRow& rhs ){
        int n = size() ;
        RCPP_LOOP_UNROLL_LHSFUN(start,get_parent_index,rhs)
            return *this ;
    }

    inline reference operator[]( int i ){
        return start[ get_parent_index(i) ] ;
    }
        
    inline reference operator[]( int i ) const {
        return parent[ row + i * parent_nrow ] ;
    }
        
    inline iterator begin(){
        return iterator( *this, 0 ) ;
    }
        
    inline iterator end(){
        return iterator( *this, size() ) ;
    }
        
    inline iterator begin() const {
        return iterator( const_cast<MatrixRow&>(*this), 0 ) ;
    }
        
    inline iterator end() const {
        return iterator( const_cast<MatrixRow&>(*this), size() ) ;
    }
        
    inline int size() const {
        return parent.ncol() ;
    }
        
private:
    MATRIX& parent; 
    typename MATRIX::iterator start ;
    int parent_nrow ;
    int row ;
        
    inline int get_parent_index(int i) const { 
        RCPP_DEBUG_4( "MatrixRow<%d>::get_parent_index(int = %d), parent_nrow = %d >> %d\n", RTYPE, i, parent_nrow, i*parent_nrow )
        return i * parent_nrow ;
    } 
} ;

#endif

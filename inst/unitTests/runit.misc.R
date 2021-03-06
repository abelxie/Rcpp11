#!/usr/bin/r -t
#
# Copyright (C) 2010 - 2013  Dirk Eddelbuettel and Romain Francois
#
# This file is part of Rcpp11.
#
# Rcpp11 is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Rcpp11 is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Rcpp11.  If not, see <http://www.gnu.org/licenses/>.

.runThisTest <- Sys.getenv("RunAllRcppTests") == "yes"

if (.runThisTest) {

.setUp <- Rcpp11:::unit_test_setup( "misc.cpp" )

test.Symbol <- function(){
	res <- symbol_()
	checkTrue( res[1L], msg = "Symbol creation - SYMSXP " )
	checkTrue( res[2L], msg = "Symbol creation - CHARSXP " )
	checkTrue( res[3L], msg = "Symbol creation - STRSXP " )
	checkTrue( res[4L], msg = "Symbol creation - std::string " )
}

test.Symbol.notcompatible <- function(){
	checkException( symbol_ctor(symbol_ctor), msg = "Symbol not compatible with function" )
	checkException( symbol_ctor(asNamespace("Rcpp11")), msg = "Symbol not compatible with environment" )
	checkException( symbol_ctor(1:10), msg = "Symbol not compatible with integer" )
	checkException( symbol_ctor(TRUE), msg = "Symbol not compatible with logical" )
	checkException( symbol_ctor(1.3), msg = "Symbol not compatible with numeric" )
	checkException( symbol_ctor(as.raw(1) ), msg = "Symbol not compatible with raw" )
}


test.Argument <- function(){
   checkEquals( Argument_(), list( x = 2L, y = 3L ) , msg = "Argument")
}

test.evaluator.error <- function(){
   checkException( evaluator_error(), msg = "Rcpp_eval( stop() )" )
}

test.evaluator.ok <- function(){
	checkEquals( sort(evaluator_ok(1:10)), 1:10, msg = "Evaluator running fine" )
}

test.exceptions <- function(){
	e <- tryCatch(  exceptions_(), "C++Error" = function(e) e )
	checkTrue( "C++Error" %in% class(e), msg = "exception class C++Error" )

	checkTrue( "std::range_error" %in% class(e), msg = "exception class std::range_error" )
	checkEquals( e$message, "boom", msg = "exception message" )

	# same with direct handler
	e <- tryCatch(  exceptions_(), "std::range_error" = function(e) e )
	checkTrue( "C++Error" %in% class(e), msg = "(direct handler) exception class C++Error" )
	checkTrue( "std::range_error" %in% class(e), msg = "(direct handler) exception class std::range_error" )
	checkEquals( e$message, "boom", msg = "(direct handler) exception message" )
	
	f <- function(){
		try( exceptions_(), silent = TRUE)
		"hello world"
	}
	checkEquals( f(), "hello world", msg = "life continues after an exception" )

}

test.has.iterator <- function(){

    has_it <- has_iterator_()
	checkTrue( has_it[1L] , msg = "has_iterator< std::vector<int> >" )
	checkTrue( has_it[2L] , msg = "has_iterator< std::ist<int> >" )
	checkTrue( has_it[3L] , msg = "has_iterator< std::deque<int> >" )
	checkTrue( has_it[4L] , msg = "has_iterator< std::set<int> >" )
	checkTrue( has_it[5L] , msg = "has_iterator< std::map<string,int> >" )

	checkTrue( ! has_it[6L] , msg = "has_iterator< std::pair<string,int> >" )
	checkTrue( ! has_it[7L] , msg = "Rcpp::Symbol" )

}

test.AreMacrosDefined <- function(){
    checkTrue( Rcpp11:::areMacrosDefined( "__cplusplus" ) )    
}

test.na_proxy <- function(){
    checkEquals( 
        na_proxy(), 
        rep(c(TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE) , 2), 
        msg = "Na_Proxy NA == handling"
    )    
}

}

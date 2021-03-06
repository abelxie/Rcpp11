//
// modref.cpp:  module unit tests
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

#include <Rcpp.h>
using namespace Rcpp ;

class World {
public:
    World() : foo(1), msg("hello") {}
    void set(std::string msg_) { this->msg = msg_; }
    std::string greet() { return msg; }
    
    int foo ;
    double bar ;

private:
    std::string msg;
};

void clearWorld( World* w ){
    w->set( "" );
}

RCPP_MODULE(yada){
	class_<World>( "World" )
	    .default_constructor()
        
        .method( "greet", &World::greet )
        .method( "set", &World::set )
        .method( "clear", &clearWorld )
        
        .field( "foo", &World::foo )
        .field_readonly( "bar", &World::bar )
        
        .property( "bla", &World::greet )
	;

}


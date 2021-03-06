# Copyright (C) 2009 - 2011 Dirk Eddelbuettel and Romain Francois
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

.dummyInstancePointer <- new.env() # just something permanent

.classes_map <- new.env()

.onLoad <- function(libname, pkgname){
    minimum_svn_rev <- packageDescription( pkgname )[["MinimumSvnRev"]]
    # if( as.integer(R.version[["svn rev"]]) < as.integer(minimum_svn_rev)){
    #     packageStartupMessage(gettextf(paste("R version (%s) older than minimum required (%s)",
    #                                          "for full use of reference methods"),
    #                                    R.version[["svn rev"]], minimum_svn_rev))
    # }
    new_dummyObject(.dummyInstancePointer);
}



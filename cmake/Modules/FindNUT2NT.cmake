# Copyright (C) 2011-2018 (see AUTHORS file for a list of contributors)
#
# This file is part of GNSS-SDR.
#
# GNSS-SDR is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GNSS-SDR is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNSS-SDR. If not, see <https://www.gnu.org/licenses/>.

########################################################################
# Find the library for the USRP Hardware Driver
########################################################################

include(FindPkgConfig)
pkg_check_modules(GRNUT2NT_PKG gnuradio-nut4nt)

find_path(NUT2NT_INCLUDE_DIR
        NAMES
        nut4nt/source.h
        nut4nt/api.h
        PATHS
        ${NUT2NT_PKG_INCLUDE_DIRS}
        /usr/include
        /usr/local/include
        /opt/local/include
        ${NUT2NT_ROOT}/include
        $ENV{NUT2NT_ROOT}/include
        )

find_library(NUT2NT_LIBRARIES
        NAMES gnuradio-nut4nt
        PATHS
        ${NUT2NT_PKG_LIBRARY_DIRS}
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /usr/lib/x86_64-linux-gnu
        /usr/lib/i386-linux-gnu
        /usr/lib/arm-linux-gnueabihf
        /usr/lib/arm-linux-gnueabi
        /usr/lib/aarch64-linux-gnu
        /usr/lib/mipsel-linux-gnu
        /usr/lib/mips-linux-gnu
        /usr/lib/mips64el-linux-gnuabi64
        /usr/lib/powerpc-linux-gnu
        /usr/lib/powerpc64-linux-gnu
        /usr/lib/powerpc64le-linux-gnu
        /usr/lib/powerpc-linux-gnuspe
        /usr/lib/hppa-linux-gnu
        /usr/lib/s390x-linux-gnu
        /usr/lib/i386-gnu
        /usr/lib/hppa-linux-gnu
        /usr/lib/x86_64-kfreebsd-gnu
        /usr/lib/i386-kfreebsd-gnu
        /usr/lib/m68k-linux-gnu
        /usr/lib/sh4-linux-gnu
        /usr/lib/sparc64-linux-gnu
        /usr/lib/x86_64-linux-gnux32
        /usr/lib/alpha-linux-gnu
        /usr/lib64
        ${NUT2NT_ROOT}/lib
        $ENV{NUT2NT_ROOT}/lib
        ${NUT2NT_ROOT}/lib64
        $ENV{NUT2NT_ROOT}/lib64
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NUT2NT DEFAULT_MSG NUT2NT_LIBRARIES NUT2NT_INCLUDE_DIR)
mark_as_advanced(NUT2NT_LIBRARIES NUT2NT_INCLUDE_DIR)

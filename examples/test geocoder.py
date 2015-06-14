#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Basic test script to exercise the mapcode geocoder. As input it takes the boundary
# files from https://github.com/mapcode-foundation/mapcode-java/tree/master/src/test/resources
# as test data set.
#
# It reads each coordinate in the test dataset, does a geocode using the mapcode module and
# compare the results with the entries in the test data set.
#
# Input format example:
#
# 1 0.043245 0.057660
# AAA HHHH1.CXRC
#
# 6 41.851944 12.433114
# ITA 0Z.0Z
# ITA G5.20M
# ITA 2MC.29K
# ITA 65C.X5QK
# ITA J0QN.7X4
# AAA TJKM1.D2Z6


import sys
import mapcode


def read_boundary_file(filename, debug=False):
    with open(filename, 'r') as f:
        geocode_count = 0

        while True:
            header_line = f.readline()
            if not header_line:
                print 'EOF'
                break

            # Get header line find out how many mapcodes will follow
            mapcode_count, lat, lon = header_line.strip().split(' ')
            # Put all mapcodes from file in a set
            mapcodes_in_file = set(f.readline().strip() for x in range(int(mapcode_count)))
            # if debug:
            #     print 'Input file: ', mapcode_count, lat, lon, ' = ', mapcodes_in_file if debug
            #     print header_line.strip()

            # Do geocode ourself, change format to match fileformat, store in set
            mapcodes = mapcode.encode(float(lat), float(lon))
            mapcodes_geocoded = set(m_territory + ' ' + m_code for m_code, m_territory in mapcodes)

            if mapcodes_in_file != mapcodes_geocoded:
                print "Geocodes did not match!", header_line, mapcodes_in_file, mapcodes_geocoded
            else:
                geocode_count += 1

            # eat whitespace between entries in source file
            f.readline()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print('Usage: {} <input file>'.format(sys.argv[0]))
    else:
        read_boundary_file(sys.argv[1], False)

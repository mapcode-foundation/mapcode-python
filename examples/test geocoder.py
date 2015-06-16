#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Basic test script to exercise the mapcode geocoder. As input it takes the boundary
# files from https://github.com/mapcode-foundation/mapcode-java/tree/master/src/test/resources
# as test data set.
#
# It reads each coordinate in the test dataset, does a geocode using the mapcode module and
# compare the results with the entries in the test data set. In addition it decodes all
# mapcodes and compares it to the lat/lon in the file.
#
# Input format:
#
# <number of mapcodes> <latitude> <longitude>
# <one or more mapcodes lines>
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


from __future__ import print_function
import sys
import time
import re
import mapcode


# The allowed margin in latitude, longitude
allowed_margin = 0.00022


def outside_margin(coordinate1, coordinate2):
    if abs(abs(coordinate1) - abs(coordinate2)) > allowed_margin:
        return True
    else:
        return False


def decode(latitude_in_file, longitude_in_file, mapcodes_in_file):
    # Decode all mapcodes from file back to latitude/longitude and compare
    for line in mapcodes_in_file:
        m_territory, m_code = line.split(' ')
        decoded_latitude, decoded_longitude = mapcode.decode(m_code, m_territory)

        if outside_margin(decoded_latitude, latitude_in_file) or \
                outside_margin(decoded_longitude, longitude_in_file):
                print('decode: mapcode outside margin! (file: %s, %f, %f) != %f, %f' %
                      (line, latitude_in_file, longitude_in_file, decoded_latitude, decoded_longitude))

    # Return how many decodes we have done
    return len(mapcodes_in_file)


def is_high_precision(mapcode):
    # Check if third before last character is a hyphen (e.g. GLP 9N0.WN6-W3)
    if re.match(r'.*-..$', mapcode):
        return True
    else:
        return False


def encode(latitude_in_file, longitude_in_file, mapcodes_in_file):
    # Do encode ourself, use extra precision incase input file entry has it
    if not is_high_precision(list(mapcodes_in_file)[0]):
        mapcodes = mapcode.encode(latitude_in_file, longitude_in_file)
    else:
        mapcodes = mapcode.encode(latitude_in_file, longitude_in_file, None, 2)

    # Change format to match fileformat and compare
    mapcodes_geocoded = set(m_territory + ' ' + m_code for m_code, m_territory in mapcodes)
    if mapcodes_in_file != mapcodes_geocoded:
        print('encode: mapcodes do no match: (file: %s) != %s' %
              (mapcodes_in_file, mapcodes_geocoded))

    # Return how many encodes we have done
    return 1


def parse_boundary_file(filename, mapcode_function):
    with open(filename, 'r') as f:
        counter = 0

        start_time = time.time()
        while True:
            header_line = f.readline()
            if not header_line:
                break

            # Get header line find out how many mapcodes will follow
            mapcode_count, latitude, longitude = header_line.strip().split(' ')
            latitude = float(latitude)
            longitude = float(longitude)

            # Put all mapcodes from file in a set
            mapcodes_in_file = set(f.readline().strip() for x in range(int(mapcode_count)))

            # do encode or decode
            counter += mapcode_function(float(latitude), float(longitude), mapcodes_in_file)

            # eat whitespace between entries in input file
            f.readline()

        duration = time.time() - start_time
        print('Did %d %ss in %.3f seconds (%d per second).' % (counter,
              mapcode_function.__name__, duration, counter / duration))


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print('Usage: {} <input file>'.format(sys.argv[0]))
    else:
        parse_boundary_file(sys.argv[1], decode)
        parse_boundary_file(sys.argv[1], encode)

import unittest
import mapcode


class TestStringMethods(unittest.TestCase):
    """Very basic testset to validate working of interface to Mapcode C-library.

IT DOES NOT VALIDATE THE WORKING AND QUALITY OF THE MAPCODE ENCODER/DECODER.
"""

    def test_isvalid(self):
        self.assertTrue(mapcode.isvalid('VHXG9.FQ9Z'))
        self.assertFalse(mapcode.isvalid('Amsterdam'))
        self.assertTrue(mapcode.isvalid('NLD 49.4V', True))
        self.assertTrue(mapcode.isvalid('VHXG9.FQ9Z', False))
        self.assertFalse(mapcode.isvalid('NLD 49.4V', False))

    def test_encode(self):
        self.assertEqual(mapcode.encode(52.376514, 4.908542), [('49.4V', 'NLD'), ('G9.VWG', 'NLD'), ('DL6.H9L', 'NLD'), ('P25Z.N3Z', 'NLD'), ('VHXGB.1J9J', 'AAA')])
        self.assertEqual(mapcode.encode(50, 6), [('CDH.MH', 'LUX'), ('R9G0.1BV', 'LUX'), ('SHP.98F', 'BEL'), ('R9G0.1BV', 'BEL'), ('0B46.W1Z', 'DEU'), ('R9G0.1BV', 'FRA'), ('VJ0LW.Y8BB', 'AAA')])

    def test_decode(self):
        self.assertEqual(mapcode.decode('NLD 49.4V'), (52.376514, 4.908543375))
        self.assertEqual(mapcode.decode('IN VY.HV'), (39.72795, -86.118444))
        self.assertEqual(mapcode.decode('VHXG9.DNRF'), (52.371422, 4.8724975))
        self.assertEqual(mapcode.decode('D6.58', 'RU-IN DK.CN0'), (43.259275, 44.7719805))
        self.assertEqual(mapcode.decode('IN VY.HV', 'USA'), (39.72795, -86.118444))
        self.assertEqual(mapcode.decode('IN VY.HV', 'RUS'), (43.193485, 44.8265925))

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(TestStringMethods)
    unittest.TextTestRunner(verbosity=3).run(suite)

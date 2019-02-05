#!/usr/bin/env python2
# -*- coding: utf-8 -*-
#
# Copyright 2017 <+YOU OR YOUR COMPANY+>.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

from gnuradio import gr, gr_unittest, digital
from gnuradio import blocks
from numpy import array
import blocksat_swig as blocksat
import blocksattx

class qa_turbo_decoder (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        """Encoder to Decoder - Noiseless"""

        # Parameters
        N      = 18444
        K      = 6144
        n_ite  = 6

        # Input data
        print("N", N, "K", K)
        in_vec = [i%2 for i in range(K)]

        # Flowgraph
        src     = blocks.vector_source_b(in_vec)
        enc     = blocksattx.turbo_encoder(N, K);
        mapb    = digital.map_bb([1,-1]);
        c2f     = blocks.char_to_float();
        dec     = blocksat.turbo_decoder(N, K, n_ite);
        i2f     = blocks.int_to_float();
        f2uc    = blocks.float_to_uchar();
        snk     = blocks.vector_sink_b()
        snk_llr = blocks.vector_sink_f()
        self.tb.connect (src, enc, mapb, c2f, dec, i2f, f2uc, snk)
        self.tb.connect (c2f, snk_llr)
        self.tb.run ()

        # Collect results
        res_sym_out = snk.data ()
        llrs        = snk_llr.data()
        err         = [0 if i == 0 else 1 for i in
                       array(in_vec) - array(res_sym_out)]

        # Print in case of error
        print(res_sym_out)
        print('N errors', sum(err))
        print(llrs)

        # Check results
        self.assertEqual(sum(err), 0)


if __name__ == '__main__':
    gr_unittest.run(qa_turbo_decoder, "qa_turbo_decoder.xml")

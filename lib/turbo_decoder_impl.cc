/* -*- c++ -*- */
/*
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "turbo_decoder_impl.h"

#undef DEBUG
#undef DEBUG_DEC

#ifdef DEBUG
#define debug_printf printf
#else
#define debug_printf(...) if (false) printf(__VA_ARGS__)
#endif

namespace gr {
	namespace blocksat {

		turbo_decoder::sptr
		turbo_decoder::make(int N, int K, int n_ite)
		{
			return gnuradio::get_initial_sptr
				(new turbo_decoder_impl(N, K, n_ite));
		}

		/*
		 * The private constructor
		 */
		turbo_decoder_impl::turbo_decoder_impl(int N, int K, int n_ite)
			: gr::block("turbo_decoder",
			            gr::io_signature::make(1, 1, sizeof(float)),
			            gr::io_signature::make(1, 1, sizeof(int)))
		{

			std::vector<int> poly = {013, 015};
			int tail_length = (int)(2 * std::floor(std::log2((float)std::max(poly[0], poly[1]))));
			int N_cw        = 2 * K + tail_length;

			interleaver = new module::Interleaver_LTE<int> (K);
			interleaver->init();

			sub_enc = new module::Encoder_RSC_generic_sys<B_8> (K, N_cw, true, poly);
			std::vector<std::vector<int>> trellis = sub_enc->get_trellis();

#ifdef DEBUG_DEC
			printf("Treliss: ");
			for(int i =0; i<8; i++)
				printf("%d ", trellis[0][i]);
			printf("\n");
#endif

			sub_dec = new module::Decoder_RSC_BCJR_seq_very_fast <int,float,float,tools::max<float>,tools::max<float>> (K, trellis);

			dec = new module::Decoder_turbo_fast<int, float>(K, N, n_ite, *interleaver, *sub_dec, *sub_dec);

			set_fixed_rate(true);
			set_relative_rate((double)K/(double)N);
			set_output_multiple(K);

			d_input_size  = N * sizeof(float);
			d_output_size = K * sizeof(int);
		}

		/*
		 * Our virtual destructor.
		 */
		turbo_decoder_impl::~turbo_decoder_impl()
		{
		}

		int
		turbo_decoder_impl::fixed_rate_ninput_to_noutput(int ninput)
		{
			debug_printf("[dec] input to output: %f\n", 0.5 + ninput*relative_rate());

			return (int)(0.5 + ninput*relative_rate());
		}

		int
		turbo_decoder_impl::fixed_rate_noutput_to_ninput(int noutput)
		{
			debug_printf("[dec] output to input: %f\n", 0.5 + noutput/relative_rate());

			return (int)(0.5 + noutput/relative_rate());
		}

		void
		turbo_decoder_impl::forecast(int noutput_items,
		                             gr_vector_int& ninput_items_required)
		{
			ninput_items_required[0] = fixed_rate_noutput_to_ninput(noutput_items);

			debug_printf("[dec] Forecast %d %d\n", noutput_items, ninput_items_required[0]);
		}

		int
		turbo_decoder_impl::general_work(int noutput_items,
		                                 gr_vector_int& ninput_items,
		                                 gr_vector_const_void_star &input_items,
		                                 gr_vector_void_star &output_items)
		{

			float *inbuffer = (float *)input_items[0];
			int *outbuffer  = (int *)output_items[0];

			debug_printf("[dec] in[%d] out[%d]\n", ninput_items[0], noutput_items);

			debug_printf("%s: output_multiple - %d\n", __func__,
			             output_multiple());

			for(int i = 0; i < noutput_items/output_multiple(); i++) {
				dec->_decode_siho(inbuffer+(i*d_input_size), outbuffer+(i*d_output_size), 0);

#ifdef DEBUG_DEC
				printf("dec_in = ");
				for(int j = 0; j< d_input_size; j++)
					printf("%f ", inbuffer[i*d_input_size+j]);
				printf("\n");
				printf("dec_out = ");
				for(int j = 0; j< d_output_size; j++)
					printf("%d ", outbuffer[i*d_input_size+j]);
				printf("\n");
#endif

			}

			consume_each(fixed_rate_noutput_to_ninput(noutput_items));
			return noutput_items;
		}

	} /* namespace blocksat */
} /* namespace gr */


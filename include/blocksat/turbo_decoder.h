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


#ifndef INCLUDED_BLOCKSAT_TURBO_DECODER_H
#define INCLUDED_BLOCKSAT_TURBO_DECODER_H

#include <blocksat/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
	namespace blocksat {

		/*!
		 * \brief <+description of block+>
		 * \ingroup blocksat
		 *
		 */
		class BLOCKSAT_API turbo_decoder : virtual public gr::block
		{
		public:
			typedef boost::shared_ptr<turbo_decoder> sptr;

			/*!
			 * \brief Return a shared_ptr to a new instance of blocksat::turbo_decoder.
			 *
			 * To avoid accidental use of raw pointers, blocksat::turbo_decoder's
			 * constructor is in a private implementation
			 * class. blocksat::turbo_decoder::make is the public interface for
			 * creating new instances.
			 */
			static sptr make(int N, int K, int n_ite, bool flip_llrs);
		};

	} // namespace blocksat
} // namespace gr

#endif /* INCLUDED_BLOCKSAT_TURBO_DECODER_H */


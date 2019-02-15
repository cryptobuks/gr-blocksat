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


#ifndef INCLUDED_BLOCKSAT_MER_MEASUREMENT_H
#define INCLUDED_BLOCKSAT_MER_MEASUREMENT_H

#include <blocksat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace blocksat {

    /*!
     * \brief <+description of block+>
     * \ingroup blocksat
     *
     */
    class BLOCKSAT_API mer_measurement : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<mer_measurement> sptr;

      /*!
       * \brief Make the modulation error ratio (MER) measurement instance
       * \param alpha IIR-based averaging alpha
       * \param M constellation order
       */
      static sptr make(float alpha, int M);

      virtual float get_snr() = 0;
      virtual void set_alpha(float alpha) = 0;
    };

  } // namespace blocksat
} // namespace gr

#endif /* INCLUDED_BLOCKSAT_MER_MEASUREMENT_H */

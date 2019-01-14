/*!
 * \file unpack_nut4nt_samples.h
 *
 * Copyright (C) 2010-2018  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <https://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */

#ifndef GNSS_SDR_UNPACK_NUT4NT_SAMPLES_H
#define GNSS_SDR_UNPACK_NUT4NT_SAMPLES_H

#include <gnuradio/sync_interpolator.h>
#include <cstdint>

class unpack_nut4nt_samples;

typedef boost::shared_ptr<unpack_nut4nt_samples> unpack_nut4nt_samples_sptr;

unpack_nut4nt_samples_sptr make_unpack_nut4nt_samples(int channel);

/*!
 * \brief This class takes 2 bit samples that have been packed into bytes or
 * shorts as input and generates a byte for each sample. It generates eight
 * times as much data as is input (every two bits become 16 bits)
 */
class unpack_nut4nt_samples : public gr::sync_interpolator
{
private:
    friend unpack_nut4nt_samples_sptr make_unpack_nut4nt_samples_sptr(int channel);
    int8_t table[4];
    int channel_;
public:
    unpack_nut4nt_samples(int channel);

    ~unpack_nut4nt_samples();

    int work(int noutput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);
};

#endif

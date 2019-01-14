/*!
 * \file unpack_nut4nt_samples.cc
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


#include "unpack_nut4nt_samples.h"
#include <gnuradio/io_signature.h>

unpack_nut4nt_samples_sptr make_unpack_nut4nt_samples(int channel)
{
    return unpack_nut4nt_samples_sptr(new unpack_nut4nt_samples(channel));
}


unpack_nut4nt_samples::unpack_nut4nt_samples(int channel)
        : sync_interpolator("unpack_nut4nt_samples",
                            gr::io_signature::make(1, 1, sizeof(uint8_t)),
                            gr::io_signature::make(1, 1, sizeof(int8_t)),
                            sizeof(uint8_t)),
                            channel_(channel)
{
    table[0] = 1;
    table[1] = 3;
    table[2] = -1;
    table[3] = -3;
}


unpack_nut4nt_samples::~unpack_nut4nt_samples() = default;


int unpack_nut4nt_samples::work(int noutput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star &output_items)
{
    auto const *in = reinterpret_cast<uint8_t const *>(input_items[0]);
    auto *out = reinterpret_cast<int8_t *>(output_items[0]);

    size_t ninput_bytes = noutput_items;

    for(int i = 0; i < ninput_bytes; i++) {
            out[i] = table[(in[i] >> (channel_ * 2)) & 0x03];
    }

    return noutput_items;
}

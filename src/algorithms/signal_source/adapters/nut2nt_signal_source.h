/*!
 * \file nut2nt_signal_source.h
 * \brief Interface for the Universal Hardware Driver signal source
 * \author Javier Arribas, 2012. jarribas(at)cttc.es
 *
 * -------------------------------------------------------------------------
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

#ifndef GNSS_SDR_NUT2NT_SIGNAL_SOURCE_H_
#define GNSS_SDR_NUT2NT_SIGNAL_SOURCE_H_

#include "gnss_block_interface.h"
#include <boost/shared_ptr.hpp>
#include <gnuradio/blocks/file_sink.h>
#include <gnuradio/blocks/copy.h>
#include <gnuradio/hier_block2.h>
#include <gnuradio/blocks/null_sink.h>
#include <gnuradio/msg_queue.h>
#include <nut4nt/nut4nt_source.h>
#include <string>
#include <vector>


class ConfigurationInterface;

class Nut2ntSignalSource : public GNSSBlockInterface
{
public:
    Nut2ntSignalSource(ConfigurationInterface* configuration,
                    const std::string& role, unsigned int in_stream,
                    unsigned int out_stream, boost::shared_ptr<gr::msg_queue> queue);

    virtual ~Nut2ntSignalSource();

    inline std::string role() override
    {
        return role_;
    }

    inline std::string implementation() override
    {
        return "Nut2nt_Signal_Source";
    }

    inline size_t item_size() override
    {
        return item_size_;
    }

    void connect(gr::top_block_sptr top_block) override;
    void disconnect(gr::top_block_sptr top_block) override;
    gr::basic_block_sptr get_left_block() override;
    gr::basic_block_sptr get_right_block() override;
    gr::basic_block_sptr get_right_block(int RF_channel) override;

private:
    std::string role_;
    unsigned int in_stream_;
    unsigned int out_stream_;
    gr::nut4nt::nut4nt_source::sptr nut2nt_source_;

    // NUT2NT SETTINGS
    std::string fx3_firmware_;
    std::string lattice_algo_;
    std::string lattice_data_;
    std::string config_file_;

    int RF_channels_;
    int channel_;
    std::string item_type_;
    size_t item_size_;

    std::vector<long> samples_;
    std::vector<bool> dump_;
    std::vector<std::string> dump_filename_;

    std::vector<boost::shared_ptr<gr::block>> valve_;
    std::vector<gr::blocks::file_sink::sptr> file_sink_;

    boost::shared_ptr<gr::msg_queue> queue_;
};

#endif /*GNSS_SDR_NUT2NT_SIGNAL_SOURCE_H_*/

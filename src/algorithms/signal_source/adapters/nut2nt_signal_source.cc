/*!
 * \file nut2nt_signal_source.cc
 * \brief Universal Hardware Driver signal source
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

#include "nut2nt_signal_source.h"
#include "GPS_L1_CA.h"
#include "configuration_interface.h"
#include "gnss_sdr_valve.h"
#include <glog/logging.h>
#include <volk/volk.h>
#include <iostream>
#include <utility>


using google::LogMessage;

Nut2ntSignalSource::Nut2ntSignalSource(ConfigurationInterface *configuration,
                                       const std::string &role, unsigned int in_stream, unsigned int out_stream,
                                       boost::shared_ptr<gr::msg_queue> queue) : role_(role), in_stream_(in_stream),
                                                                                 out_stream_(out_stream),
                                                                                 queue_(std::move(queue)) {

    std::string default_dump_file = "nut4nt.dump";
    // NUT2NT COMMON PARAMETERS
    fx3_firmware_ = configuration->property(role + ".fx3_firmware", std::string("fx3.img"));
    lattice_algo_ = configuration->property(role + ".lattice_algo", std::string("lattice.sea"));
    lattice_data_ = configuration->property(role + ".lattice_data", std::string("lattice.sed"));
    config_file_ = configuration->property(role + ".config_file", std::string("config.hex"));
    RF_channels_ = configuration->property(role + ".RF_channels", 1);
    channel_ = configuration->property(role + ".channel", 0);
    item_type_ = configuration->property(role + ".item_type", std::string("short"));

    if (RF_channels_ == 1) {
        samples_.push_back(configuration->property(role + ".samples", 0));
        dump_.push_back(configuration->property(role + ".dump", false));
        dump_filename_.push_back(configuration->property(role + ".dump_filename", default_dump_file));
    } else {
        // multiple RF channels selected
        for (int i = 0; i < RF_channels_; i++) {
            samples_.push_back(configuration->property(role + ".samples" + std::to_string(i), 0));
            dump_.push_back(configuration->property(role + ".dump" + std::to_string(i), false));
            dump_filename_.push_back(configuration->property(role + ".dump_filename" + std::to_string(i),
                                                             "ch" + std::to_string(i) + default_dump_file));
        }
    }

    if (item_type_ == "byte") {
        item_size_ = sizeof(char);
    } else if (item_type_ == "short") {
        item_size_ = sizeof(short);
    } else if (item_type_ == "float") {
        item_size_ = sizeof(float);
    } else {
        LOG(WARNING) << item_type_ << " unrecognized item type. Using short.";
        item_size_ = sizeof(short);
    }

    for (int i = 0; i < RF_channels_; i++)
    {
        if (samples_.at(i) != 0)
        {
            LOG(INFO) << "RF_channel " << i << " Send STOP signal after " << samples_.at(i) << " samples";
            valve_.push_back(gnss_sdr_make_valve(item_size_, samples_.at(i), queue_));
            DLOG(INFO) << "valve(" << valve_.at(i)->unique_id() << ")";
        }

        if (dump_.at(i))
        {
            LOG(INFO) << "RF_channel " << i << "Dumping output into file " << dump_filename_.at(i);
            file_sink_.push_back(gr::blocks::file_sink::make(item_size_, dump_filename_.at(i).c_str()));
            DLOG(INFO) << "file_sink(" << file_sink_.at(i)->unique_id() << ")";
        }
    }
    // 1.2 Make the NUT2NT source object
    nut2nt_source_ = gr::nut4nt::nut4nt_source::make(RF_channels_, fx3_firmware_, config_file_, lattice_algo_,
                                                     lattice_data_, item_size_, 2 * 1024 * 1024, 7, channel_);

    nut2nt_source_->init_nut4nt_source();

    if (in_stream_ > 0) {
        LOG(ERROR) << "A signal source does not have an input stream";
    }
    if (out_stream_ > 1) {
        LOG(ERROR) << "This implementation only supports one output stream";
    }
}


Nut2ntSignalSource::~Nut2ntSignalSource() = default;


void Nut2ntSignalSource::connect(gr::top_block_sptr top_block) {
    for (int i = 0; i < RF_channels_; i++)
    {
        if (samples_.at(i) != 0)
        {
            top_block->connect(nut2nt_source_, i, valve_.at(i), 0);
            DLOG(INFO) << "connected nut2nt source to valve RF Channel " << i;
            if (dump_.at(i))
            {
                top_block->connect(valve_.at(i), 0, file_sink_.at(i), 0);
                DLOG(INFO) << "connected valve to file sink RF Channel " << i;
            }
        }
        else
        {
            if (dump_.at(i))
            {
                top_block->connect(nut2nt_source_, i, file_sink_.at(i), 0);
                DLOG(INFO) << "connected nut2nt source to file sink RF Channel " << i;
            }
        }
    }
}


void Nut2ntSignalSource::disconnect(gr::top_block_sptr top_block) {
    for (int i = 0; i < RF_channels_; i++)
    {
        if (samples_.at(i) != 0)
        {
            top_block->disconnect(nut2nt_source_, i, valve_.at(i), 0);
            LOG(INFO) << "nut2nt source disconnected";
            if (dump_.at(i))
            {
                top_block->disconnect(valve_.at(i), 0, file_sink_.at(i), 0);
            }
        }
        else
        {
            if (dump_.at(i))
            {
                top_block->disconnect(nut2nt_source_, i, file_sink_.at(i), 0);
            }
        }
    }
}


gr::basic_block_sptr Nut2ntSignalSource::get_left_block() {
    LOG(WARNING) << "Trying to get signal source left block.";
    return gr::nut4nt::nut4nt_source::sptr();
}


gr::basic_block_sptr Nut2ntSignalSource::get_right_block() {
    return get_right_block(0);
}

gr::basic_block_sptr Nut2ntSignalSource::get_right_block(int RF_channel)
{
    //TODO: There is a incoherence here: Multichannel UHD is a single block with multiple outputs, but if the sample limit is enabled, the output is a multiple block!
    if (samples_.at(RF_channel) != 0)
    {
        return valve_.at(RF_channel);
    }
    return nut2nt_source_;
}


/* -*- c++ -*- */
/*
 * Copyright 2022 gr-msr4 author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MSR4_RS_JERRY_IMPL_H
#define INCLUDED_MSR4_RS_JERRY_IMPL_H

#include "iqClient/iqClient.h"
#include <gnuradio/msr4/rs_jerry.h>

namespace gr {
namespace msr4 {

class rs_jerry_impl : public rs_jerry
{
private:
    std::unique_ptr<IqClient> iqClient;

    struct StreamingSettings {
        int port;
        const std::string& dest_addr;
        Protocol protocol;
        StreamingSettings(int port, const std::string& dest_addr, Protocol protocol)
            : port(port), dest_addr(dest_addr), protocol(protocol)
        {
        }
    };

    struct ChannelSettings {
        int sat_frequency;
        int down_frequency;
        int bandwidth_analysis_bandwidth;
        ChannelSettings(int sat_frequency,
                        int down_frequency,
                        int bandwidth_analysis_bandwidth)
            : sat_frequency(sat_frequency),
              down_frequency(down_frequency),
              bandwidth_analysis_bandwidth(bandwidth_analysis_bandwidth)
        {
        }
    };

    struct DpdkSettings {
        int port_id;
        float norm;
        DpdkSettings(int port_id, float norm) : port_id(port_id), norm(norm) {}
    };


    void PrepareForGrpc(std::string ip_addr,
                        std::string user,
                        std::string password,
                        RxChannel rx_channel);
    void LogIntoMsr4(std::string ip_addr, std::string user, std::string password);
    void SetRxChannel(RxChannel rx_channel);

    void PrepareForStreaming(std::unique_ptr<StreamingSettings> streaming_settings,
                             std::unique_ptr<ChannelSettings> channel_settings,
                             std::unique_ptr<DpdkSettings> dpdk_settings);
    void SetStreamingSettings(std::unique_ptr<StreamingSettings> streaming_settings);
    void SetChannelSettings(std::unique_ptr<ChannelSettings> channel_settings);
    void SetDpdkSettings(std::unique_ptr<DpdkSettings> dpdk_settings);

public:
    rs_jerry_impl(std::string ip_addr,
                  std::string user,
                  std::string password,
                  RxChannel rx_channel,
                  int port,
                  const std::string& dest_addr,
                  Protocol protocol,
                  int sat_frequency,
                  int down_frequency,
                  int bandwidth_analysis_bandwidth,
                  int port_id,
                  float norm);
    ~rs_jerry_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace msr4
} // namespace gr

#endif /* INCLUDED_MSR4_RS_JERRY_IMPL_H */

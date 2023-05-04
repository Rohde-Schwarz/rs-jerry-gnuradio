/* -*- c++ -*- */
/*
 * Copyright 2022 gr-msr4 author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "rs_jerry_impl.h"
#include <gnuradio/io_signature.h>
#include <iqClient/dpdkSource/dpdkSource.h>

namespace gr {
namespace msr4 {

using output_type = gr_complex;
rs_jerry::sptr rs_jerry::make(std::string ip_addr,
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
                              float norm)
{
    return gnuradio::make_block_sptr<rs_jerry_impl>(ip_addr,
                                                    user,
                                                    password,
                                                    rx_channel,
                                                    port,
                                                    dest_addr,
                                                    protocol,
                                                    sat_frequency,
                                                    down_frequency,
                                                    bandwidth_analysis_bandwidth,
                                                    port_id,
                                                    norm);
}

/*
 * The private constructor
 */
rs_jerry_impl::rs_jerry_impl(std::string ip_addr,
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
                             float norm)
    : gr::sync_block("rs_jerry",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    iqClient = std::make_unique<IqClient>();
    PrepareForGrpc(ip_addr, user, password, rx_channel);

    PrepareForStreaming(std::make_unique<StreamingSettings>(port, dest_addr, protocol),
                        std::make_unique<ChannelSettings>(
                            sat_frequency, down_frequency, bandwidth_analysis_bandwidth),
                        std::make_unique<DpdkSettings>(port_id, norm));

    iqClient->SetStreamingStatus(true);
    std::vector<DpdkSource::stream_attr> streams{ DpdkSource::stream_attr{
        std::pair<int, int>(0, 24), port_id, static_cast<uint16_t>(port) } };
    iqClient->SetupDpdkSource(streams.data(), streams.size());
}

void rs_jerry_impl::PrepareForGrpc(std::string ip_addr,
                                   std::string user,
                                   std::string password,
                                   RxChannel rx_channel)
{
    LogIntoMsr4(ip_addr, user, password);
    SetRxChannel(rx_channel);

    if (iqClient->GetStreamingStatus())
        iqClient->SetStreamingStatus(false);
}

void rs_jerry_impl::LogIntoMsr4(std::string ip_addr,
                                std::string user,
                                std::string password)
{
    iqClient->SetMSR4Ip(ip_addr);
    iqClient->SetMSR4Credentials(user, password);

    grpc::Status status = iqClient->MSR4Login();
    if (!status.ok())
        throw InvalidValueError("Invalid login.");
}

void rs_jerry_impl::SetRxChannel(RxChannel rx_channel)
{
    RsIcpxGrpcService::ErrorMessage err;
    switch (rx_channel) {
    case RxChannel::Rx1:
        iqClient->SetRxChannel(GrpcClient::MSR4Channel::Rx1);
        break;
    case RxChannel::Rx2:
        iqClient->SetRxChannel(GrpcClient::MSR4Channel::Rx2);
        break;
    case RxChannel::Rx3:
        iqClient->SetRxChannel(GrpcClient::MSR4Channel::Rx3);
        break;
    case RxChannel::Rx4:
        iqClient->SetRxChannel(GrpcClient::MSR4Channel::Rx4);
        break;
    }
}

void rs_jerry_impl::PrepareForStreaming(
    std::unique_ptr<StreamingSettings> streaming_settings,
    std::unique_ptr<ChannelSettings> channel_settings,
    std::unique_ptr<DpdkSettings> dpdk_settings)
{
    SetStreamingSettings(std::move(streaming_settings));
    SetChannelSettings(std::move(channel_settings));
    SetDpdkSettings(std::move(dpdk_settings));
}

void rs_jerry_impl::SetStreamingSettings(
    std::unique_ptr<StreamingSettings> streaming_settings)
{
    RsIcpxGrpcService::ErrorMessage err;

    err = iqClient->SetPort(streaming_settings->port);
    if (err.errorcode() != 0)
        throw InvalidValueError(err.errormessage());

    err = iqClient->SetDestinationAddress(streaming_settings->dest_addr);
    if (err.errorcode() != 0)
        throw InvalidValueError(err.errormessage());

    switch (streaming_settings->protocol) {
    case Protocol::HRZR:
        err = iqClient->SetProtocol(RsIcpxGrpcService::Protocols::HRZR);
        break;
    case Protocol::AMMOS:
        err = iqClient->SetProtocol(RsIcpxGrpcService::Protocols::AMMOS);
        break;
    }
    if (err.errorcode() != 0)
        throw InvalidValueError(err.errormessage());
}

void rs_jerry_impl::SetChannelSettings(std::unique_ptr<ChannelSettings> channel_settings)
{
    RsIcpxGrpcService::ErrorMessage err;

    err = iqClient->SetSatFrequencyHz(channel_settings->sat_frequency);
    if (err.errorcode() != 0)
        throw InvalidValueError(err.errormessage());

    err = iqClient->SetDownFrequencyHz(channel_settings->down_frequency);
    if (err.errorcode() != 0)
        throw InvalidValueError(err.errormessage());

    err = iqClient->SetBandwidthByAnalysisBandwidth(
        channel_settings->bandwidth_analysis_bandwidth);
    if (err.errorcode() != 0)
        throw InvalidValueError(err.errormessage());
}

void rs_jerry_impl::SetDpdkSettings(std::unique_ptr<DpdkSettings> dpdk_settings)
{
    iqClient->SetPortID(dpdk_settings->port_id);
    iqClient->SetNorm(dpdk_settings->norm);
}

/*
 * Our virtual destructor.
 */
rs_jerry_impl::~rs_jerry_impl() { iqClient->SetStreamingStatus(false); }

int rs_jerry_impl::work(int noutput_items,
                        gr_vector_const_void_star& input_items,
                        gr_vector_void_star& output_items)
{
    gr_complex* out = (gr_complex*)output_items[0];
    auto nsamples = iqClient->GetSamples(0, noutput_items, out);
    // Do <+signal processing+>

    // Tell runtime system how many output items we produced.
    return nsamples;
}

} /* namespace msr4 */
} /* namespace gr */

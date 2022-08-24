/* -*- c++ -*- */
/*
 * Copyright 2022 gr-msr4 author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MSR4_RS_JERRY_H
#define INCLUDED_MSR4_RS_JERRY_H

#include <gnuradio/msr4/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace msr4 {
/*!
 * \brief <+description of block+>
 * \ingroup msr4
 *
 */
enum RxChannel { Rx1, Rx2, Rx3, Rx4 };
enum Protocol { HRZR, AMMOS };


/*!
 * \brief An IQ source using the rs-jerry-driver.
 *
 * \details
 * This block is meant to be used only in combination with the installed rs-jerry-driver.
 * Using the previously installed rs-jerry-driver, this source-block gets IQ samples from the selected MSR4 device.
 * 
 * MSR4 settings are configured using gRPC. Generally no webUI is required unless specific settings need to be changed.
 */
class MSR4_API rs_jerry : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<rs_jerry> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of msr4::rs_jerry.
     *
     * \param ip_addr ip address of the MSR4 you want to connect to
     * \param user username to log into the MSR4
     * \param password password to log into the MSR4
     * \param rx_channel channel to edit
     * \param port port on the receiver the data is streamed to
     * \param dest_addr address on the receiver the data is streamed
     * \param protocol communication format. Currently, only HRZR protocol is supported for I/Q data streaming.
     * \param sat_frequency center frequency of the carrier at the satellite
     * \param down_frequency conversion frequency of the downconverter
     * \param bandwidth_analysis_bandwidth displayed frequency range of the RF input channel
     * \param port_id id of VF to use
     * \param norm norm by which samples get divided by
     */
    static sptr make(std::string ip_addr,
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
};

} // namespace msr4
} // namespace gr

#endif /* INCLUDED_MSR4_RS_JERRY_H */

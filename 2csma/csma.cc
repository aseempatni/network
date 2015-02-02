/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// Default Network Topology
//
//    
//                   n1   n2   n3   n4
//                    |    |    |    |
//                    ================
//                      LAN 10.1.2.0


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  // Set random seed based on time
  ns3::RngSeedManager::SetSeed((unsigned int)time(NULL));
  int MaxPackets = 1000;
  bool verbose = true;
  uint32_t nCsma = 8;
  int PacketSize = 1024;
  float Interval = 1.0;
  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("packetSize", "PacketSize", PacketSize);
  cmd.AddValue ("interval", "Interval", Interval);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  PacketSize*=(1024/8);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;

  NodeContainer csmaNodes;
  csmaNodes.Create (nCsma);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("1Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (csmaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  // Server Nodes
  int s1=5,s2=6,s3=3,s4=4;
  // Client nodes
  int c1=1,c2=2,c3=7,c4=8;

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps1 = echoServer.Install (csmaNodes.Get (s1-1));
  ApplicationContainer serverApps2 = echoServer.Install (csmaNodes.Get (s2-1));
  ApplicationContainer serverApps3 = echoServer.Install (csmaNodes.Get (s3-1));
  ApplicationContainer serverApps4 = echoServer.Install (csmaNodes.Get (s4-1));

  serverApps1.Start (Seconds (1.0));
  serverApps2.Start (Seconds (1.0));
  serverApps3.Start (Seconds (1.0));
  serverApps4.Start (Seconds (1.0));
  
  serverApps1.Stop (Seconds (10.0));
  serverApps2.Stop (Seconds (10.0));
  serverApps3.Stop (Seconds (10.0));
  serverApps4.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient1 (csmaInterfaces.GetAddress (s1-1), 9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (MaxPackets));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (Interval)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (PacketSize));
  UdpEchoClientHelper echoClient2 (csmaInterfaces.GetAddress (s2-1), 9);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (MaxPackets));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (Interval)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (PacketSize));
  UdpEchoClientHelper echoClient3 (csmaInterfaces.GetAddress (s3-1), 9);
  echoClient3.SetAttribute ("MaxPackets", UintegerValue (MaxPackets));
  echoClient3.SetAttribute ("Interval", TimeValue (Seconds (Interval)));
  echoClient3.SetAttribute ("PacketSize", UintegerValue (PacketSize));
  UdpEchoClientHelper echoClient4 (csmaInterfaces.GetAddress (s4-1), 9);
  echoClient4.SetAttribute ("MaxPackets", UintegerValue (MaxPackets));
  echoClient4.SetAttribute ("Interval", TimeValue (Seconds (Interval)));
  echoClient4.SetAttribute ("PacketSize", UintegerValue (PacketSize));

  ApplicationContainer clientApps1 = echoClient1.Install (csmaNodes.Get(c1-1));
  ApplicationContainer clientApps2 = echoClient2.Install (csmaNodes.Get(c2-1));
  ApplicationContainer clientApps3 = echoClient3.Install (csmaNodes.Get(c3-1));
  ApplicationContainer clientApps4 = echoClient4.Install (csmaNodes.Get(c4-1));

  clientApps1.Start (Seconds (2.0));
  clientApps2.Start (Seconds (2.0));
  clientApps3.Start (Seconds (2.0));
  clientApps4.Start (Seconds (2.0));
  clientApps1.Stop (Seconds (10.0));
  clientApps2.Stop (Seconds (10.0));
  clientApps3.Stop (Seconds (10.0));
  clientApps4.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // pointToPoint.EnablePcapAll ("second");
  csma.EnablePcap ("second", csmaDevices.Get (1), true);

  AsciiTraceHelper ascii;
  csma.EnableAsciiAll (ascii.CreateFileStream("trace.tr"));
  // csma.EnablePcapAll ("trace");

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

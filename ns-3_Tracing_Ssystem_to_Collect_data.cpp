 // Online C++ compiler to run C++ program online
// This is just an example of ns-3 tracing system by Dr Idris Skloul Ibrahim @HWU
// with help by OpenAI 29/04/2023

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/trace-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TracingExample");

void
TxTrace (std::string context, Ptr<const Packet> packet, const Address &address)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " Tx trace: " << context << " " << *packet);
}

void
RxTrace (std::string context, Ptr<const Packet> packet, const Address &address)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " Rx trace: " << context << " " << *packet);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable ("TracingExample", LOG_LEVEL_INFO);

  // Enable tracing
  TraceHelper trace;
  trace.TruncateTraceAfter (Seconds (10)); // Only trace the first 10 seconds
  trace.TraceAll ();// Trace all callbacks

  // Create nodes
  NodeContainer nodes;
  nodes.Create (2);

  // Install internet stack
  InternetStackHelper stack;
  stack.Install (nodes);

  // Create point-to-point link
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = p2p.Install (nodes);

  // Install applications
  OnOffHelper onoff ("ns3::UdpSocketFactory", Address ());
  onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

  // Install on node 0, send to node 1
  AddressValue remoteAddress (InetSocketAddress (Ipv4Address ("10.1.1.2"), 9));
  onoff.SetAttribute ("Remote", remoteAddress);
  ApplicationContainer apps = onoff.Install (nodes.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  // Create packet sink on node 1
  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 9));
  apps = sink.Install (nodes.Get (1));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  // Enable PCAP tracing on all devices
  p2p.EnablePcapAll ("tracing-example");

  // Start simulation
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

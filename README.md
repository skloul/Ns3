# Ns3
ns-3 tracing system to collect data during a simulation:
This example enables tracing for all callbacks, and traces packets sent and received by two nodes connected by a point-to-point link. The TxTrace and RxTrace functions are defined to print information about the packets being traced. The example also sets up an OnOff application to generate traffic and a PacketSink application to receive it, and enables PCAP tracing on all devices.

You can modify this example to collect the specific data you need during your simulation. The key is to include the necessary headers and add the appropriate tracing hooks in your code.

@0x990be4df2c24687c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("psf::registry::rpc");

struct Endpoint {
  enum Type {
    publisher @0;
    subscriber @1;
  }

  type @0 :Type;
  address @1 :UInt32;
  port @2 :UInt16;
  nodeName @3 :Text;
  topicName @4 :Text;
}

interface Registry {
  registerEndpoint @0 (endpoint :Endpoint) -> (result :Bool);
  getSubscriberEndpointsForTopic @1 (topicName :Text) -> (subscribers :List(Endpoint));
}

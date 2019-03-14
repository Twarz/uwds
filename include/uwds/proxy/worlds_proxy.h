#ifndef WORLDS_PROXY_HPP
#define WORLDS_PROXY_HPP

#include "world_proxy.h"

using namespace std;
using namespace uwds_msgs;

namespace uwds {

  typedef map<string, WorldProxyPtr> WorldProxyMap;

  class WorldsProxy {
    public:
      WorldsProxy(NodeHandlePtr nh, ClientPtr client, const MeshesProxyPtr meshes)
      {
        nh_ = nh;
        meshes_ = meshes;
        client_ = client;
      }

      ~WorldsProxy() {}

      WorldProxy& operator[](const std::string& name){
        if (worlds_.count(name) == 0) {
          worlds_.emplace(name, boost::make_shared<WorldProxy>(nh_, client_, meshes_, name));
        }
        return *worlds_.at(name);
      }

      void close()
      {
        worlds_.clear();
      }

    private:
      ClientPtr client_;

      NodeHandlePtr nh_;

      WorldProxyMap worlds_;

      MeshesProxyPtr meshes_;
  };

  typedef boost::shared_ptr<uwds::WorldsProxy> WorldsProxyPtr;
  typedef boost::shared_ptr<uwds::WorldsProxy const> WorldsProxyConstPtr;
}

#endif
#ifndef WORLD_HPP
#define WORLD_HPP

#include <uwds_msgs/Changes.h>
#include <std_msgs/Header.h>
#include <uwds_msgs/Invalidations.h>
#include "scene.h"
#include "timeline.h"
#include "meshes.h"

#include<mutex>

namespace uwds {

  /** @brief
   * This class represent the world
   */
  class World {
    public:
      /** @brief
       * Default constructor
       */
      World(const std::string& name, const MeshesPtr meshes)
      {
        name_ = name;
        meshes_ = meshes;
        scene_ = boost::make_shared<Scene>();
        timeline_ = boost::make_shared<Timeline>();
      };

      /** @brief
       * This method update the world.
       */
      Invalidations update(const Changes& changes);

      /** @brief
       * The name accessor
       */
      std::string name() {return name_;}

      /** @brief
       * The scene accessor
       */
      Scene& scene() {return * scene_;}

      /** @brief
       * The timeline accessor
       */
      Timeline& timeline() {return * timeline_;}

      /** @brief
       * The meshes accessor
       */
      Meshes& meshes() {return * meshes_;}

      /** @brief
       * This method apply changes to the world
       *
       * @param header The header
       * @param changes The changes
       */
      Invalidations applyChanges(const std_msgs::Header& header, const Changes& changes)
      {
        Invalidations invalidations;

        scene_->remove(changes.nodes_to_delete);
        invalidations.node_ids_deleted = changes.nodes_to_delete;
        for (const auto& node : changes.nodes_to_update)
        {
          scene_->update(node);
          invalidations.node_ids_updated.push_back(node.id);
        }

        timeline_->remove(changes.situations_to_delete);
        invalidations.situation_ids_deleted = changes.situations_to_delete;
        for (const auto& situation : changes.situations_to_update)
        {
          timeline_->update(situation);
          invalidations.situation_ids_updated.push_back(situation.id);
        }

        meshes_->remove(changes.meshes_to_delete);
        invalidations.mesh_ids_deleted = changes.meshes_to_delete;
        for (const auto& mesh : changes.meshes_to_update)
        {
          meshes_->update(mesh);
          invalidations.mesh_ids_updated.push_back(mesh.id);
        }
        return invalidations;
      }

      /** @brief
       * This method reset the world
       */
      void reset()
      {
        scene().reset();
        timeline().reset();
        meshes().reset();
      }

    private:

      /** @brief
       * The name of the world.
       */
      std::string name_;

      /** @brief
       * The scene shared pointer.
       */
      ScenePtr scene_;

      /** @brief
       * The timeline shared pointer.
       */
      TimelinePtr timeline_;

      /** @brief
       * The Underworlds meshes.
       */
      MeshesPtr meshes_;
  };

  typedef uwds::World World;
  typedef boost::shared_ptr<uwds::World> WorldPtr;
  typedef boost::shared_ptr<uwds::World const> WorldConstPtr;
}

#endif
/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <peer_service/change_state.hpp>
#include <peer_service/monitor.hpp>

#include <unordered_set>

namespace peer_service{
    namespace change_state{

        // This scope is issue transaction
        namespace transtion {
          // invoke to issue transaction
          void add(const std::string &ip, const Node &){

          }
          void remove(const std::string &ip, const std::string &){

          }
          void setTrust(const std::string &ip, const std::string &, const double &){

          }
          void changeTrust(const std::string &ip, const std::string &, const double &){

          }
          void setActive(const std::string &ip, const std::string &, const State state){

          }
        }

        // This scope is validation
        namespace validation {
          bool add(const Node &peer){
            if (monitor::isExistIP(peer.ip_))
              return false;
            if (monitor::isExistPublicKey(peer.public_key_))
              return false;
            if ( peer.getState()!= PREPARE )
              return false;
            return true;
          }
          bool remove(const std::string &publicKey){
            if (!monitor::isExistPublicKey(publicKey))
              return false;

            auto it = monitor::findPeerPublicKey(publicKey);
            if( it->get()->getState() != PREPARE )
              return false;
            // TODO : ping to publickKey confirm

            return true;
          }
          bool setTrust(const std::string &publicKey, const double &trust){
            if (!monitor::isExistPublicKey(publicKey))
              return false;
            return true;
          }
          bool changeTrust(const std::string &publicKey, const double &trust){
            if (!monitor::isExistPublicKey(publicKey))
              return false;
            return true;
          }
          bool setActive(const std::string &publicKey, const State state){
            if (!monitor::isExistPublicKey(publicKey))
              return false;
            return true;
          }
        }

        // This scope is runtime
        namespace runtime {
          bool add(const Node &peer){
            if (monitor::isExistIP(peer.ip_))
              return false;
            if (monitor::isExistPublicKey(peer.public_key_))
              return false;
            peer_list_.emplace_back( std::make_shared<Node>(peer) );
            return true;
          }
          bool remove(const std::string &publicKey){
            if (!monitor::isExistPublicKey(publicKey))
              return false;
            peer_list_.erase( monitor::findPeerPublicKey(publicKey) );
            return true;
          }
          bool setTrust(const std::string &publicKey, const double &trust){
            if (!monitor::isExistPublicKey(publicKey))
              return false;

            auto node = monitor::findPeerPublicKey(publicKey)->get();
            node->setTrust(trust);
            if( node->getState() == ACTIVE )
              detail::changeActive(publicKey);
            return true;
          }
          bool changeTrust(const std::string &publicKey, const double &trust){
            if (!monitor::isExistPublicKey(publicKey))
              return false;

            auto node = monitor::findPeerPublicKey(publicKey)->get();
            node->setTrust(node->getTrust() + trust);
            if( node->getState() == ACTIVE )
              detail::changeActive(publicKey);
            return true;
          }
          bool setActive(const std::string &publicKey, const State state){
            if (!monitor::isExistPublicKey(publicKey))
              return false;

            auto node = monitor::findPeerPublicKey(publicKey)->get();
            if( node->getState() == PREPARE ){
              if( state == ACTIVE ) // PRPARE -> ACTIVE
                detail::insertActive(node);
            } else if( node->getState() == ACTIVE ) {
              if( state == PREPARE ) // ACTIVE -> PREPARE
                detail::eraseActive(node);
            }
            node->state_ = state;
          }

      };

      namespace detail {
        void insertActive( const Node& node ){

        }
        void eraseActive( const Node& node ){

        }
        void changeActive( std::string publicKey ) {

        }
      }
    }
};


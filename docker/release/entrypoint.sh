#!/usr/bin/env bash
echo key=$KEY
iroha-cli --genesis_block --peers_address peers.list
irohad --genesis_block genesis.block --config config.sample --keypair_name $KEY

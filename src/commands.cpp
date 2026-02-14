/**
 * Part of:
 * Comments:
 *
**/
#include <tinychain/tinychain.hpp>
#include <tinychain/commands.hpp>

namespace tinychain
{


bool commands::exec(Json::Value& out){
    try {
        static const std::string help_text = R"(Commands:
  help
    Print this help message.

  getnewkey
    Generate a new RSA keypair, store it in tinychain.db (key_pairs), and return {address, public_key, private_key}.

  listkeys
    List all keys in the local wallet database (key_pairs).

  getlastblock
    Fetch and return the last block header stored in tinychain.db (block table).

  getheight
    Return current chain height from tinychain.db.

  getbalance
    Not implemented yet.

  send <address> <amount>
    Create a transaction to <address> and put it into the in-memory mempool (not persisted yet).

  startmining [address]
    Start mining in a detached thread. If address omitted, a random wallet address is generated for rewards.)";

        if (vargv_.empty() || *(vargv_.begin()) == "help") {
            out = help_text;
            return true;
        } else if (*(vargv_.begin()) == "getnewkey") {
            auto&& key = node_.chain().get_new_key_pair();
            out = key.to_json();
        } else if  (*(vargv_.begin()) == "listkeys") {
            out = node_.chain().list_keys();
        } else if  (*(vargv_.begin()) == "send") {
            if (vargv_.size() >= 3) {
                uint64_t amount = std::stoul(vargv_[2]);
                auto ret = node_.chain().send(vargv_[1], amount);
                out = ret.toStyledString();
            } else {
                out = "Invalid or Empty paramas";
            }

        } else if  (*(vargv_.begin()) == "getbalance") {
            out = "getbalance-TODO";

        } else if  (*(vargv_.begin()) == "getlastblock") {
            auto&& b = node_.chain().get_last_block();
            out = b.to_json();

        } else if  (*(vargv_.begin()) == "getheight") {
            out = node_.chain().get_height();

        } else if  (*(vargv_.begin()) == "startmining") {
            std::string addr;
            if (vargv_.size() >= 2) {
                addr = vargv_[1];
                node_.miner_run(addr);
                out["result"] = "start mining on address: " + addr;
            } else {
                node_.miner_run(addr);
                out["result"] = "start mining on a random address of your wallet";
            }
        } else {
            out = help_text;
            return false;
        }
    } catch (std::exception& ex) {
        log::error("commands")<<"error:"<< ex.what();
        return false;
    }

    return true;
}

const commands::vargv_t commands::commands_list = {"help", "getnewkey","listkeys","getlastblock","getheight","send","getbalance","startmining"};


} //tinychain

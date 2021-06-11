#include <eosio/eosio.hpp>
#include <eosio/crypto.hpp>

using namespace std;
using namespace eosio;

CONTRACT yuppostvfixb : public contract
{
public:
  using contract::contract;

  yuppostvfixb(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                        posts(receiver, receiver.value),
                                                                        votes(receiver, receiver.value)
  {
  }

  ACTION postvote(name author, string caption);
  ACTION createvote(uint64_t postid, name voter);
  ACTION notify(name user, string msg);

  TABLE posts_table
  {
    uint64_t postid;
    name author;
    string caption;

    auto primary_key() const { return postid; }
    checksum256 by_post_url() const { return sha256(caption.c_str(), caption.length()); }
  };

  TABLE votes_table
  {
    uint64_t voteid;
    name voter;
    uint64_t post_key;

    auto primary_key() const { return voteid; }
  };

  typedef multi_index<"posts"_n, posts_table,
                      indexed_by<"byurl"_n, const_mem_fun<posts_table, checksum256, &posts_table::by_post_url>>>
      posts_tables;

  typedef multi_index<"votes"_n, votes_table> votes_tables;

  votes_tables votes;
  posts_tables posts;

private:
  void send_msg(name user, std::string message);
  void privateVoteCreate(uint64_t postid, name voter);
};

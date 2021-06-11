#include <yuppostvfixb.hpp>

// The create post & vote tables and actions are dummy
// The important change is to check if the url exists in order to not create dublicate posts
// Something similar to this implementation should fix the duplicate post bug

// Dummy post vote
ACTION yuppostvfixb::postvote(name author, string caption)
{

  check((is_account(name("1yupcurator1"))), "You are not a test account");

  auto idx = posts.get_index<"byurl"_n>();
  auto itr = idx.find(sha256(caption.c_str(), caption.length()));

  if (itr == idx.end())
  {
    // Post dosen't exist
    posts.emplace(_self, [&](auto &pt)
                  {
                    pt.postid = posts.available_primary_key();
                    pt.author = author;
                    pt.caption = caption;
                  });
  }
  else
  {
    // Post exists let's create a vote instead and notify the user
    send_msg(author, "Post vote was cast to create vote due to race condition.");
    privateVoteCreate(itr->postid, author);
  }
}

// Dummy create vote
ACTION yuppostvfixb::createvote(uint64_t postid, name voter)
{

  check((is_account(name("1yupcurator1"))), "You are not a test account");

  votes.emplace(_self, [&](auto &vt)
                {
                  vt.voteid = votes.available_primary_key();
                  vt.post_key = postid;
                  vt.voter = voter;
                });
}

ACTION yuppostvfixb::notify(name user, string message)
{
  check((is_account(name("1yupcurator1"))), "You are not a test account");
  require_recipient(user);
}

void yuppostvfixb::send_msg(name user, string message)
{
  action(
      permission_level{_self, "active"_n},
      _self,
      "notify"_n,
      make_tuple(user, message))
      .send();
};

void yuppostvfixb::privateVoteCreate(uint64_t postid, name voter)
{
  action(
      permission_level{_self, "active"_n},
      _self,
      "createvote"_n,
      make_tuple(postid, voter))
      .send();
}

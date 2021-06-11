# yuppostvfixb

Smart contract POC for fixing double posting bug on YUP.

It has a dummy votes and post tables, if a curator will post a link that's allready in the table, it will create a vote instead of a post and will also notifiy the curator that thier post was converted to create vote, because of racing condition.  


Tested on https://jungle3.bloks.io/

Contract account: 

[ yuppostvfixb - https://jungle3.bloks.io/account/yuppostvfixb ]


Test account: 

[ 1yupcurator1 - https://jungle3.bloks.io/account/1yupcurator1 ]

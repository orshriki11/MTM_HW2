# MTM_HW2

Hi hillah, I just went to sleep (didn't wanna send a whatsapp message so it won't wake you up). 
Hopefully you'll see this message if I won't wake up in time. 

The code passed the basic tests but is having problem with the advanced and difficult memleaks tests.

The main issues are:
- The way playersHash.insert in addplayer is inserting a new node (from copy construct) into the hash,
causing it to have link issues with the original uptree.

- where UFNode.Find is called it can mistakengly be called on a NULL node, because of the same issue that the nodes are inserted
into the hash unlinked to their original tree.

Also here are some link summarys from some tests:

https://pastebin.com/tzWiFv1r

https://pastebin.com/PEJk5BkC

Hopefully i'll fix those issues before you wake up! :D

Oh, And good morning ig.

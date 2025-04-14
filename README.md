This is the README for the Snake game that I wrote in the C programming language. I developed it using the Simple DirectMedia Layer (SDL) library, so it should work on most operating systems. I could have compiled or it for each OS individually or made makefiles for each, but since each is somewhat different, I haven’t had the time. However, it should work on most macOS systems with the included makefile. In addition, it should compile on most operating systems, just not with the included makefile.

I have included a compiled binary, but it will, in principle, only work on the arm64 architecture. If you have a newer Mac (post-2020), it should run without needing to use the Makefile at all.
The folder also includes a free font found online. For the record, it is included only for use in this game for demonstration purposes and is not intended for commercial use.


Installation Instructions (macOS)

1. Open a terminal and navigate to the folder that contains this README and enter:

make

Once compiled, run the game by typing:

./snake

After done playing game, in order to delete compiled binary, enter:

make clean


P.S. In the interest of full disclosure, I wrote the bulk of the game on my own. However, I did use OpenAI’s ChatGPT to assist with creating the Makefile and editing the README, in order to make it easier for users to install and run the game on Intel-based Macs.
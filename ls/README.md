# ls

## current ls functionality includes:
-la
-l -a
test -la
-la test
test/ -la
/etc -la
test /etc -la
test -la /etc

where test is a valid subdirectory and /etc is a known path to a folder.

This means that there should be a collection of directories, and options.
Also means that printing should occur for each directory and handle options.
By default readdir returns all contents including hidden, so hidden need
to be removed if -a is not given.

# Currently compiles with:
gcc *.c -o hls
gcc -Wall -Werror -Wextra -pedantic *.c -o hls
./hls to run
./hls -a works now

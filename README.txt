This is a repository for a Linux inter process comunication example. 
A/The Contents :
  - "src" directory contains source code for application A (app_a.c) and source code for application B (app_b.c and linked_list.c).
  - "Makefile" file compiling the applications.
  - "test" directory for an automatic test
B/How to install and run the two applications :
  - Run "make install_json_lib". If your Linux distribution uses a different package manager than apt  you can install it manually from (https://github.com/json-c/json-c). This step only needed for the first time
  - Run "make compile" to generate the 2 programs under "build" directory.
C/For testing the  applications :
  - Run "./build/app_a -f test/example.json" and "./build/app_b" in different terminals (you can see the options of app_a using "./build/app_b -h").
  - Run the test script under the "test" directory (cd test && ./test.sh) or edit the json file manually.

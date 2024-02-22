install_json_lib:
	#needed only for the first time 
	#instaling the open source lib json-c for parsing the json file
	sudo apt-get install -y libjson-c-dev

compile:
	#create build_dir
	mkdir build
	#compile app_a
	gcc src/app_a.c -o build/app_a
	#compile app_b, dyanamic linking for the json-c lib
	gcc src/app_b.c src/linked_list.c -ljson-c -o build/app_b

clean:
	rm -rf build

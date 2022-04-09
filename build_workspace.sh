docker run -it --name="mqtt_machine"\
	-v $PWD/ws/:/home/ws/:rw\
	docker.io/an2ancan/mqtt_eclipse:latest\
	env TERM=xterm-256color\
	/bin/bash -c "cd ws && ./build.sh"

docker container rm mqtt_machine
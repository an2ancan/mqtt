xhost local:root
docker run -it -p 1883:1883 --name="mqtt_machine"\
	-v $PWD/ws/:/home/ws/:rw\
	docker.io/an2ancan/mqtt_eclipse:latest\
	env TERM=xterm-256color\
	/bin/bash
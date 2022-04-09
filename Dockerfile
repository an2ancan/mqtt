From ubuntu:latest
RUN apt-get update
RUN apt install apt-utils
RUN echo "y 6" | apt install software-properties-common
RUN apt-add-repository ppa:mosquitto-dev/mosquitto-ppa -y
RUN apt-get install mosquitto -y
RUN apt install mosquitto mosquitto-clients -y
RUN apt install net-tools -y
RUN apt install vim -y
RUN apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui -y
RUN apt-get install libssl-dev -y
RUN apt-get install git -y
RUN apt-get update
RUN apt-get install doxygen graphviz -y
RUN apt-get install libncursesw5-dev -y

WORKDIR /home/ws/
RUN git clone https://github.com/eclipse/paho.mqtt.c.git
RUN cd /home/paho.mqtt.c \
	&& cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON \
    		-DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON -DPAHO_BUILD_SHARED=ON\
    	&& cmake --build build/ --target install && ldconfig
RUN cd /home/ && git clone https://github.com/eclipse/paho.mqtt.cpp
RUN cd /home/paho.mqtt.cpp \
	&& cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON \
    		-DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE -DPAHO_BUILD_SHARED=ON\
    	&& cmake --build build/ --target install && ldconfig
    


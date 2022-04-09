# Location Data Generator and Processor communicating via MQTT

## Build

At first download repo:
```bash
git clone https://github.com/an2ancan/mqtt.git
```
Then pull docker image:
```bash
docker pull docker.io/an2ancan/mqtt_eclipse:latest
```
I haven't added Workspace to image, so it is necessary to build it manually:
```bash
./build_workspace.sh
```
## Run
It is required to open two terminals

In the **first terminal** start broker and processor:
```bash
docker-compose up
```

In the **second one** start generator
```bash
./run_generator_in_container.sh
```

## Few words about generator
It is based on the bicycle kinematic sheme and there is a possibility to control it the generator terminal:
**w** - speed the model up by 0.5 m/s
**s** - speed the model down by 0.5 m/s
**d** - turn the model's steering right by 10 degrees
**a** - turn the model's steering left by 10 degrees

## Results
You can find two csv files tin the workspace:
**position.csv** - stores the timestamp, TAG_ID, x,y,z
**accum_div.csv** - stores time, distance, speed 

# Gazebo simulation
Per poter effetturare simulazioni SITL con infinity-autopilot è necessaria l'installazione di gazebo seguendo la [guida ufficiale](https://docs.zephyrproject.org/latest/develop/getting_started/index.html).

## Prerequisites (Ubuntu 22.04, Gazebo)
```bash
sudo apt update
sudo apt install python3-gz-transport13 python3-gz-msgs10 python3-protobuf
```

## Setup virtual environment
```bash
python3 -m venv --system-site-packages venv
source venv/bin/activate
pip install -e ".[sim]"
```

## Run simulation
```bash
run_sitl_infinity.sh
```
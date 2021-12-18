#!/bin/bash
name="pautlencontainer"
image="pautlenrom"

if [ $1 == "buildImage" ]; then
docker build -t ${image} -f DockerFile  .
elif [ $1 == "create" ]; then
docker create  \
  -it \
  --name ${name} \
  --cap-add sys_admin \
  --cap-add sys_ptrace \
  --mount type=bind,source="$(pwd)"/,target=/data \
  ${image}

elif [ $1 == "start" ]; then
docker start ${name}
elif [ $1 == "stop" ]; then 
docker stop ${name}

elif [ $1 == "attach" ]; then 
docker attach ${name}

elif [ $1 == "erase" ]; then
docker stop ${name}
docker rm ${name}
else 
echo "Unknown command"
fi
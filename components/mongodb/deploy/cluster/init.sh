#!/bin/bash
# create config
sudo docker-compose -f docker-compose.yml exec mongo-config-1 mongo --port 27019 --eval 'rs.initiate({_id: "config",configsvr: true, members: [{ _id : 0, host : "mongo-config-1:27019" },{ _id : 1, host : "mongo-config-2:27019" }, { _id : 2, host: "mongo-config-3:27019" }]})'

sudo docker-compose -f docker-compose.yml exec mongo-r1-1 mongo --port 27018 --eval 'rs.initiate({_id: "cluster1", members: [{ _id : 0, host: "mongo-r1-1:27018"}, {_id: 1, host: "mongo-r1-2:27018"}]})'
sudo docker-compose -f docker-compose.yml exec mongo-r2-1 mongo --port 27018 --eval 'rs.initiate({_id: "cluster2", members: [{ _id : 0, host : "mongo-r2-1:27018"}, {_id: 1, host: "mongo-r2-2:27018"}]})'
sudo docker-compose -f docker-compose.yml exec mongo-r3-1 mongo --port 27018 --eval 'rs.initiate({_id: "cluster3", members: [{ _id : 0, host : "mongo-r3-1:27018"}, {_id: 1, host: "mongo-r3-2:27018"}]})'

sleep 10
sudo docker-compose -f docker-compose.yml exec mongo-r1-1 mongo --port 27018 --eval 'rs.addArb("mongo-r1-3:27018")'
sudo docker-compose -f docker-compose.yml exec mongo-r1-2 mongo --port 27018 --eval 'rs.addArb("mongo-r1-3:27018")'
sudo docker-compose -f docker-compose.yml exec mongo-r2-1 mongo --port 27018 --eval 'rs.addArb("mongo-r2-3:27018")'
sudo docker-compose -f docker-compose.yml exec mongo-r2-2 mongo --port 27018 --eval 'rs.addArb("mongo-r2-3:27018")'
sudo docker-compose -f docker-compose.yml exec mongo-r3-1 mongo --port 27018 --eval 'rs.addArb("mongo-r3-3:27018")'
sudo docker-compose -f docker-compose.yml exec mongo-r3-2 mongo --port 27018 --eval 'rs.addArb("mongo-r3-3:27018")'


sudo docker-compose -f docker-compose.yml exec mongo-cluster-router mongo --port 27017 --eval 'sh.addShard("cluster1/mongo-r1-1:27018,mongo-r1-2:27018,mongo-r1-3:27018")'
sudo docker-compose -f docker-compose.yml exec mongo-cluster-router mongo --port 27017 --eval 'sh.addShard("cluster2/mongo-r2-1:27018,mongo-r2-2:27018,mongo-r2-3:27018")'
sudo docker-compose -f docker-compose.yml exec mongo-cluster-router mongo --port 27017 --eval 'sh.addShard("cluster3/mongo-r3-1:27018,mongo-r3-2:27018,mongo-r3-3:27018")'
sudo docker-compose -f docker-compose.yml exec mongo-cluster-router mongo --port 27017 --eval 'sh.status()'
